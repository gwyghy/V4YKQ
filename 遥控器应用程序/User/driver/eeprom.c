/*************************************************************
** 文件名:  eeprom.c
** Copyright (c) 1998-1999 天津华宁电子技术有限公司技术开发部
** 创建人:  李微辰
** 日　期:  2013.03.01
** 修改人:	
** 日　期:	
** 描　述:  电液控项目UCOS_II系统应用之eeprom操作驱动。
**
** 版　本:	V1.0A
**--------------------------------------------------------------------------
*************************************************************/ 
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
__IO uint16_t  sEEAddress = 0;   								  //I2C地址寄存变量
__IO uint32_t  sEETimeout = e24LCXX_LONG_TIMEOUT;   			  //I2C通讯超时变量
__IO uint16_t* sEEDataReadPointer;   							  //I2C读取数据总数变量
__IO uint8_t*  sEEDataWritePointer;  							  //I2C写入数据总数变量
__IO uint8_t   sEEDataNum; 										  //I2C读取页内数据总数变量

#define ARRY_DIV(cn)  ((cn/2)*4)								  //计算参数序号和内存存储地址(基址)之间关系
#define ARRY_SIT(cn)  (cn%2) 									  //计算参数序号和内存存储地址(偏移)之间关系

#define SUCCESS_NUM       				0
#define FAIL_ERROR_TIME_OUT				1						  //超时
#define FAIL_ERROR_CHECK_SUM			2						  //校验错误
#define FAIL_ERROR_NUM3 				3						  //非4单元整体写入
#define FAIL_ERROR_OVER_BOUNDARY		4						  //越界

volatile uint16_t NumDataTran = 0;								  //参数传递数量变量

u8 CRC_TABLE[]= {0x00,0x31,0x62,0x53,0xC4,0xF5,0xA6,0x97,0xB9,0x88,0xDB,0xEA,0x7D,0x4C,0x1F,0x2E};	 //CRC校验表
DMA_InitTypeDef   sEEDMA_InitStructure; 						  //DMA处理结构

OS_EVENT  *MutexParSaved;	//参数保存互斥信号量
OS_EVENT  *MutexParRead;	//参数读取互斥信号量

typedef struct{
u16 u16Offset;
u16 u16Count;
} stParSavedItem;

#define PAR_SAVED_QUEUE_COUNT		20
#define E2PROM_CAPACITY				0x7ff

static stParSavedItem stParSavedQueue[PAR_SAVED_QUEUE_COUNT];
static vu32 vu32ParSavedQueueCnt;
static u16 u16ParSavedQueueBegin;
static u16 u16ParSavedQueueEnd;

static u16 u16OffsetTmp;
static u16 u16CountTmp;
static u8 group_tmp_rd[4]={0,0,0,0};
static u8 group_tmp_wr[4]={0,0,0,0};

/*********************************************************
** 函数名：	 e24LCXX_LowLevel_DeInit
** 输　入：  无
** 输　出：  无
** 功能描述：EEPROM(24LC16)接口卸载子程序。
** 作　者:	 李微辰
** 日　期：  2013.03.06
** 版本：    V1.0A
************************************************************/
void e24LCXX_LowLevel_DeInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  NVIC_InitTypeDef NVIC_InitStructure;    
   
  /* e24LCXX_I2C Peripheral Disable */
  I2C_Cmd(e24LCXX_I2C, DISABLE);									   //关闭I2C
 
  /* e24LCXX_I2C DeInit */
  I2C_DeInit(e24LCXX_I2C);											   //关闭I2C端口

  /*!< e24LCXX_I2C Periph clock disable */
  RCC_APB1PeriphClockCmd(e24LCXX_I2C_CLK, DISABLE);					   //关闭I2C时钟
    
  /*!< GPIO configuration */  
  /*!< Configure e24LCXX_I2C pins: SCL */							   //配置I2C端口为输入浮空
  GPIO_InitStructure.GPIO_Pin = e24LCXX_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(e24LCXX_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure e24LCXX_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = e24LCXX_I2C_SDA_PIN;
  GPIO_Init(e24LCXX_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
  
//  GPIO_PinAFConfig(GPIO_Remap_I2C1, ENABLE);
  
  /* Configure and enable I2C DMA TX Channel interrupt */				//关闭发送中断
  NVIC_InitStructure.NVIC_IRQChannel = e24LCXX_I2C_DMA_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;			//不使用中断优先级嵌套。因为SysTick的中断优先级为0x0f
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//e24LCXX_I2C_DMA_SUBPRIO;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Configure and enable I2C DMA RX Channel interrupt */				//关闭接收中断
  NVIC_InitStructure.NVIC_IRQChannel = e24LCXX_I2C_DMA_RX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;			//不使用中断优先级嵌套。因为SysTick的中断优先级为0x0f
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//e24LCXX_I2C_DMA_SUBPRIO;
  NVIC_Init(&NVIC_InitStructure);   
  
  /* Disable and Deinitialize the DMA channels */
  DMA_Cmd(e24LCXX_I2C_DMA_CHANNEL_TX, DISABLE);							//关闭DMA中断
  DMA_Cmd(e24LCXX_I2C_DMA_CHANNEL_RX, DISABLE);
  DMA_DeInit(e24LCXX_I2C_DMA_CHANNEL_TX);
  DMA_DeInit(e24LCXX_I2C_DMA_CHANNEL_RX);
}

/*********************************************************
** 函数名：	 e24LCXX_LowLevel_Init
** 输　入：  无
** 输　出：  无
** 功能描述：EEPROM(24LC16)接口初始化子程序。
** 作　者:	 李微辰
** 日　期：  2013.03.06
** 版本：    V1.0A
************************************************************/
void e24LCXX_LowLevel_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  NVIC_InitTypeDef NVIC_InitStructure;    
    
  /*!< e24LCXX_I2C_SCL_GPIO_CLK and e24LCXX_I2C_SDA_GPIO_CLK Periph clock enable */
  RCC_APB2PeriphClockCmd(e24LCXX_I2C_SCL_GPIO_CLK | e24LCXX_I2C_SDA_GPIO_CLK | e24LCXX_I2C_WP_GPIO_CLK, ENABLE);		//使能I2C管脚时钟

  /*!< e24LCXX_I2C Periph clock enable */
  RCC_APB1PeriphClockCmd(e24LCXX_I2C_CLK, ENABLE);																		//使能I2C功能时钟
    
  /*!< GPIO configuration */  
  /*!< Configure e24LCXX_I2C pins: SCL */
  GPIO_InitStructure.GPIO_Pin = e24LCXX_I2C_SCL_PIN;																	//I2C管脚配置
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(e24LCXX_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure e24LCXX_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = e24LCXX_I2C_SDA_PIN;
  GPIO_Init(e24LCXX_I2C_SDA_GPIO_PORT, &GPIO_InitStructure); 

//  GPIO_PinAFConfig(GPIO_Remap_I2C1, ENABLE);
  
  /*!< Configure e24LCXX_I2C pins: WP */
  GPIO_InitStructure.GPIO_Pin = e24LCXX_I2C_WP_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(e24LCXX_I2C_WP_GPIO_PORT, &GPIO_InitStructure);


  /* Configure and enable I2C DMA TX Channel interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = e24LCXX_I2C_DMA_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;			//不使用中断优先级嵌套。因为SysTick的中断优先级为0x0f
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//e24LCXX_I2C_DMA_SUBPRIO;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Configure and enable I2C DMA RX Channel interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = e24LCXX_I2C_DMA_RX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;			//不使用中断优先级嵌套。因为SysTick的中断优先级为0x0f
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//e24LCXX_I2C_DMA_SUBPRIO;
  NVIC_Init(&NVIC_InitStructure);  
  
  /*!< I2C DMA TX and RX channels configuration */
  /* Enable the DMA clock */
  RCC_AHBPeriphClockCmd(e24LCXX_I2C_DMA_CLK, ENABLE);													   //使能DMA功能时钟

  /* I2C TX DMA Channel configuration */
  DMA_DeInit(e24LCXX_I2C_DMA_CHANNEL_TX);																   //DMA配置
  sEEDMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)e24LCXX_I2C_DR_Address;
  sEEDMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)0;   /* This parameter will be configured durig communication */
  sEEDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;    /* This parameter will be configured durig communication */
  sEEDMA_InitStructure.DMA_BufferSize = 0xFFFF;            /* This parameter will be configured durig communication */
  sEEDMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  sEEDMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  sEEDMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
  sEEDMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  sEEDMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  sEEDMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  sEEDMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(e24LCXX_I2C_DMA_CHANNEL_TX, &sEEDMA_InitStructure);  											//I2C使用的发送DMA初始化
  
  /* I2C RX DMA Channel configuration */
  DMA_DeInit(e24LCXX_I2C_DMA_CHANNEL_RX);
  DMA_Init(e24LCXX_I2C_DMA_CHANNEL_RX, &sEEDMA_InitStructure);  											//I2C使用的接收DMA初始化
  
  /* Enable the DMA Channels Interrupts */
  DMA_ITConfig(e24LCXX_I2C_DMA_CHANNEL_TX, DMA_IT_TC, ENABLE);												//I2C使用的发送DMA中断使能
  DMA_ITConfig(e24LCXX_I2C_DMA_CHANNEL_RX, DMA_IT_TC, ENABLE);  											//I2C使用的接收DMA中断使能
  
  
  GPIO_SetBits(e24LCXX_I2C_WP_GPIO_PORT, e24LCXX_I2C_WP_PIN);												//EEPROM写保护。
  //GPIO_ResetBits(e24LCXX_I2C_WP_GPIO_PORT, e24LCXX_I2C_WP_PIN);												//关闭EEPROM写保护。
  //GPIO_SetBits(W25QXX_CS_GPIO_PORT, W25QXX_CS_PIN)  
}


/*********************************************************
** 函数名：	 e24LCXX_LowLevel_DMAConfig
** 输　入：  uint32_t pBuffer     ---- DMA数据源
**           uint32_t BufferSize  ---- DMA缓冲区大小
**           uint32_t Direction	  ---- DMA方向
** 输　出：  无
** 功能描述：EEPROM(24LC16)DMA初始化子程序。
** 作　者:	 李微辰
** 日　期：  2013.03.06
** 版本：    V1.0A
************************************************************/
void e24LCXX_LowLevel_DMAConfig(uint32_t pBuffer, uint32_t BufferSize, uint32_t Direction)
{ 
  /* Initialize the DMA with the new parameters */
  if (Direction == e24LCXX_DIRECTION_TX)
  {
    /* Configure the DMA Tx Channel with the buffer address and the buffer size */
    sEEDMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pBuffer;
    sEEDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;    
    sEEDMA_InitStructure.DMA_BufferSize = (uint32_t)BufferSize;  
    DMA_Init(e24LCXX_I2C_DMA_CHANNEL_TX, &sEEDMA_InitStructure);  
  }
  else
  { 
    /* Configure the DMA Rx Channel with the buffer address and the buffer size */
    sEEDMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pBuffer;
    sEEDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    sEEDMA_InitStructure.DMA_BufferSize = (uint32_t)BufferSize;      
    DMA_Init(e24LCXX_I2C_DMA_CHANNEL_RX, &sEEDMA_InitStructure);    
  }
}
/*********************************************************
** 函数名：CRC_8
** 输　入：unsigned char *PData,unsigned char Len
** 输　出：CRC校验结果
** 功能描述：8位CRC校验函数
** 作　者：翟菲菲
** 日　期：2012-03-19
** 修　改：
** 日　期：
** 版本：V1.0
************************************************************/
unsigned char CRC_8(unsigned short PData)
{
   unsigned char CRC_Temp=0;
   unsigned char Temp;
   unsigned char PData_H=0;
   unsigned char PData_L=0;
   PData_H=PData>>8;
   PData_L=(unsigned char)PData;

   Temp=CRC_Temp>>4;
   CRC_Temp<<=4;
   CRC_Temp^=CRC_TABLE[Temp^(PData_H>>4)];
   Temp=CRC_Temp>>4;
   CRC_Temp<<=4;
   CRC_Temp^=CRC_TABLE[Temp^(PData_H&0x0F)];

   Temp=CRC_Temp>>4;
   CRC_Temp<<=4;
   CRC_Temp^=CRC_TABLE[Temp^(PData_L>>4)];
   Temp=CRC_Temp>>4;
   CRC_Temp<<=4;
   CRC_Temp^=CRC_TABLE[Temp^(PData_L&0x0F)];

   return (CRC_Temp);
}
/*********************************************************
** 函数名：	 e24LCXX_DeInit
** 输　入：  无
** 输　出：  无
** 功能描述：EEPROM(24LC16)卸载子程序。
** 作　者:	 李微辰
** 日　期：  2013.03.06
** 版本：    V1.0A
************************************************************/
void e24LCXX_DeInit(void)
{
  e24LCXX_LowLevel_DeInit(); 
}

/*********************************************************
** 函数名：	 e24LCXX_Init
** 输　入：  无
** 输　出：  无
** 功能描述：EEPROM(24LC16)初始化子程序。
** 作　者:	 李微辰
** 日　期：  2013.03.06
** 版本：    V1.0A
************************************************************/
void e24LCXX_Init(void)
{ 
	I2C_InitTypeDef  I2C_InitStructure;
	INT8U err;
  
	e24LCXX_LowLevel_Init();
  
	/*!< I2C configuration */
	/* e24LCXX_I2C configuration */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;											//配置I2C模式
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  
	/* e24LCXX_I2C Peripheral Enable */
	I2C_Cmd(e24LCXX_I2C, ENABLE);															 //使能I2C
	/* Apply e24LCXX_I2C configuration after enabling it */
	I2C_Init(e24LCXX_I2C, &I2C_InitStructure);											 //配置I2C

	/* Enable the e24LCXX_I2C peripheral DMA requests */									  
	I2C_DMACmd(e24LCXX_I2C, ENABLE);														 //使能I2C

	/*!< depending on the sEE Address selected in the i2c_ee.h file */

	sEEAddress = e24LCXX_Block0_ADDRESS;  

	vu32ParSavedQueueCnt = 0;
	u16ParSavedQueueBegin = 0;
	u16ParSavedQueueEnd = 0;

	MutexParSaved = OSMutexCreate(MUTEX_PAR_SAVED_PRIO, &err);	//创建参数保存互斥信号量
	MutexParRead = OSMutexCreate(MUTEX_PAR_READ_PRIO, &err);	//创建参数读取互斥信号量
}

/*********************************************************
** 函数名：	 e24LCXX_ReadBuffer
** 输　入：  uint8_t* pBuffer   ----写入数据指针
**           uint16_t ReadAddr  ----读取地址
**           uint16_t NumByteToRead  ----读取数据字节总数
** 输　出：  成功返回0 失败返回非0
** 功能描述：EEPROM(24LC16)读子程序。
** 作　者:	 李微辰
** 日　期：  2013.03.06
** 版本：    V1.0A
************************************************************/
uint32_t e24LCXX_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead)
{  
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif

	/* Set the pointer to the Number of data to be read. This pointer will be used 
	  by the DMA Transfer Completer interrupt Handler in order to reset the 
	  variable to 0. User should check on this variable in order to know if the 
	  DMA transfer has been complete or not. */
	if(ReadAddr+(*NumByteToRead) > E2PROM_CAPACITY) return FAIL_ERROR_OVER_BOUNDARY;
	sEEDataReadPointer = NumByteToRead;
  
	/*!< While the bus is busy */
	sEETimeout = e24LCXX_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(e24LCXX_I2C, I2C_FLAG_BUSY))
	{
		if((sEETimeout--) == 0) return e24LCXX_TIMEOUT_UserCallback();
	}
  
	/*!< Send START condition */
	I2C_GenerateSTART(e24LCXX_I2C, ENABLE);
  
	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	sEETimeout = e24LCXX_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(e24LCXX_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((sEETimeout--) == 0) return e24LCXX_TIMEOUT_UserCallback();
	}
  
	/*!< Send EEPROM address for write */
	I2C_Send7bitAddress(e24LCXX_I2C, sEEAddress+BLOCK_ADD(ReadAddr), I2C_Direction_Transmitter);

	/*!< Test on EV6 and clear it */
	sEETimeout = e24LCXX_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(e24LCXX_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((sEETimeout--) == 0) return e24LCXX_TIMEOUT_UserCallback();
	} 
  
	/*!< Send the EEPROM's internal address to read from: Only one byte address */
	I2C_SendData(e24LCXX_I2C, ReadAddr);  

	/*!< Test on EV8 and clear it */
	sEETimeout = e24LCXX_FLAG_TIMEOUT;
	while(I2C_GetFlagStatus(e24LCXX_I2C, I2C_FLAG_BTF) == RESET)
	{
		if((sEETimeout--) == 0) return e24LCXX_TIMEOUT_UserCallback();
	}
  
	/*!< Send STRAT condition a second time */  
	I2C_GenerateSTART(e24LCXX_I2C, ENABLE);
  
	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	sEETimeout = e24LCXX_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(e24LCXX_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((sEETimeout--) == 0) return e24LCXX_TIMEOUT_UserCallback();
	} 
  
	/*!< Send EEPROM address for read */
	I2C_Send7bitAddress(e24LCXX_I2C, sEEAddress+BLOCK_ADD(ReadAddr), I2C_Direction_Receiver);  
  
	/* If number of data to be read is 1, then DMA couldn't be used */
	/* One Byte Master Reception procedure (POLLING) ---------------------------*/
	if ((uint16_t)(*NumByteToRead) < 2)
	{
		/* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
		sEETimeout = e24LCXX_FLAG_TIMEOUT;
		while(I2C_GetFlagStatus(e24LCXX_I2C, I2C_FLAG_ADDR) == RESET)
		{
			if((sEETimeout--) == 0) return e24LCXX_TIMEOUT_UserCallback();
		}     
    
		/*!< Disable Acknowledgement */
		I2C_AcknowledgeConfig(e24LCXX_I2C, DISABLE);   

		/* Call User callback for critical section start (should typically disable interrupts) */
		//e24LCXX_EnterCriticalSection_UserCallback();
		OS_ENTER_CRITICAL();
    
		/* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
		(void)e24LCXX_I2C->SR2;
    
		/*!< Send STOP Condition */
		I2C_GenerateSTOP(e24LCXX_I2C, ENABLE);
   
		/* Call User callback for critical section end (should typically re-enable interrupts) */
		//e24LCXX_ExitCriticalSection_UserCallback();
		OS_EXIT_CRITICAL();
    
		/* Wait for the byte to be received */
		sEETimeout = e24LCXX_FLAG_TIMEOUT;
		while(I2C_GetFlagStatus(e24LCXX_I2C, I2C_FLAG_RXNE) == RESET)
		{
			if((sEETimeout--) == 0) return e24LCXX_TIMEOUT_UserCallback();
		}
    
		/*!< Read the byte received from the EEPROM */
		*pBuffer = I2C_ReceiveData(e24LCXX_I2C);
    
		/*!< Decrement the read bytes counter */
		(uint16_t)(*NumByteToRead)--;        
    
		/* Wait to make sure that STOP control bit has been cleared */
		sEETimeout = e24LCXX_FLAG_TIMEOUT;
		while(e24LCXX_I2C->CR1 & I2C_CR1_STOP)
		{
			if((sEETimeout--) == 0) return e24LCXX_TIMEOUT_UserCallback();
		}
    
		/*!< Re-Enable Acknowledgement to be ready for another reception */
		I2C_AcknowledgeConfig(e24LCXX_I2C, ENABLE);    
	}
	else/* More than one Byte Master Reception procedure (DMA) -----------------*/
	{
		/*!< Test on EV6 and clear it */
		sEETimeout = e24LCXX_FLAG_TIMEOUT;
		while(!I2C_CheckEvent(e24LCXX_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		{
			if((sEETimeout--) == 0) return e24LCXX_TIMEOUT_UserCallback();
		}  
    
		/* Configure the DMA Rx Channel with the buffer address and the buffer size */
		e24LCXX_LowLevel_DMAConfig((uint32_t)pBuffer, (uint16_t)(*NumByteToRead), e24LCXX_DIRECTION_RX);
    
		/* Inform the DMA that the next End Of Transfer Signal will be the last one */
		I2C_DMALastTransferCmd(e24LCXX_I2C, ENABLE); 
    
		/* Enable the DMA Rx Channel */
		DMA_Cmd(e24LCXX_I2C_DMA_CHANNEL_RX, ENABLE);  
	}
  
	/* If all operations OK, return e24LCXX_OK (0) */
	return e24LCXX_OK;
}

/*********************************************************
** 函数名：	 e24LCXX_WritePage
** 输　入：  uint8_t* pBuffer   ----写入数据指针
**           uint16_t WriteAddr ----写入地址
**           uint16_t NumByteToWrite  ----写入数据字节总数
** 输　出：  成功返回0 失败返回非0
** 功能描述：EEPROM(24LC16)按页写子程序。
** 日　期：  2013.03.18
** 版本：    V1.0
************************************************************/
uint32_t e24LCXX_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite)
{ 
  /* Set the pointer to the Number of data to be written. This pointer will be used 
      by the DMA Transfer Completer interrupt Handler in order to reset the 
      variable to 0. User should check on this variable in order to know if the 
      DMA transfer has been complete or not. */
  sEEDataWritePointer = NumByteToWrite;  
  
  /*!< While the bus is busy */
  sEETimeout = e24LCXX_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(e24LCXX_I2C, I2C_FLAG_BUSY))
  {
    if((sEETimeout--) == 0) return e24LCXX_TIMEOUT_UserCallback();
  }
  
  /*!< Send START condition */
  I2C_GenerateSTART(e24LCXX_I2C, ENABLE);
  
  /*!< Test on EV5 and clear it */
  sEETimeout = e24LCXX_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(e24LCXX_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((sEETimeout--) == 0) return e24LCXX_TIMEOUT_UserCallback();
  }
  
  /*!< Send EEPROM address for write */
  sEETimeout = e24LCXX_FLAG_TIMEOUT;
  I2C_Send7bitAddress(e24LCXX_I2C, sEEAddress+BLOCK_ADD(WriteAddr), I2C_Direction_Transmitter);

  /*!< Test on EV6 and clear it */
  sEETimeout = e24LCXX_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(e24LCXX_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((sEETimeout--) == 0) return e24LCXX_TIMEOUT_UserCallback();
  }

  /*!< Send the EEPROM's internal address to write to : only one byte Address */
  I2C_SendData(e24LCXX_I2C, WriteAddr);  
  
  /*!< Test on EV8 and clear it */
  sEETimeout = e24LCXX_FLAG_TIMEOUT; 
  while(!I2C_CheckEvent(e24LCXX_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((sEETimeout--) == 0) return e24LCXX_TIMEOUT_UserCallback();
  }  
  
  /* Configure the DMA Tx Channel with the buffer address and the buffer size */
  e24LCXX_LowLevel_DMAConfig((uint32_t)pBuffer, (uint8_t)(*NumByteToWrite), e24LCXX_DIRECTION_TX);
    
  /* Enable the DMA Tx Channel */
  DMA_Cmd(e24LCXX_I2C_DMA_CHANNEL_TX, ENABLE);
  /* If all operations OK, return e24LCXX_OK (0) */
  return e24LCXX_OK;
}

/*********************************************************
** 函数名：	 e24LCXX_WriteBuffer
** 输　入：  uint8_t* pBuffer   ----写入数据指针
**           uint16_t WriteAddr ----写入地址
**           uint16_t NumByteToWrite  ----写入数据字节总数
** 输　出：  成功返回0 失败返回非0
** 功能描述：EEPROM(24LC16)写子程序。
** 作　者:	 李微辰
** 日　期：  2013.03.06
** 版本：    V1.0A
************************************************************/
uint32_t e24LCXX_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
	uint8_t NumOfPage = 0, NumOfSingle = 0, count = 0;
	uint16_t Addr = 0;

	if ((WriteAddr + NumByteToWrite) > E2PROM_CAPACITY)
		return FAIL_ERROR_OVER_BOUNDARY;
	Addr = WriteAddr % e24LCXX_PAGESIZE;
	count = e24LCXX_PAGESIZE - Addr;
	NumOfPage =  NumByteToWrite / e24LCXX_PAGESIZE;
	NumOfSingle = NumByteToWrite % e24LCXX_PAGESIZE;
 
  /*!< If WriteAddr is e24LCXX_PAGESIZE aligned  */
  if(Addr == 0) 
  {
    /*!< If NumByteToWrite < e24LCXX_PAGESIZE */
    if(NumOfPage == 0) 
    {
      /* Store the number of data to be written */
      sEEDataNum = NumOfSingle;
      /* Start writing data */
      e24LCXX_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
      /* Wait transfer through DMA to be complete */
      sEETimeout = e24LCXX_LONG_TIMEOUT;
      while (sEEDataNum > 0)
      {
        if((sEETimeout--) == 0) {return e24LCXX_TIMEOUT_UserCallback();};
      }
      e24LCXX_WaitEepromStandbyState();
    }
    /*!< If NumByteToWrite > e24LCXX_PAGESIZE */
    else  
    {
      while(NumOfPage--)
      {
        /* Store the number of data to be written */
        sEEDataNum = e24LCXX_PAGESIZE;        
        e24LCXX_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum)); 
        /* Wait transfer through DMA to be complete */
        sEETimeout = e24LCXX_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {return e24LCXX_TIMEOUT_UserCallback();};
        }      
        e24LCXX_WaitEepromStandbyState();
        WriteAddr +=  e24LCXX_PAGESIZE;
        pBuffer += e24LCXX_PAGESIZE;
      }

      if(NumOfSingle!=0)
      {
        /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;          
        e24LCXX_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = e24LCXX_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {return e24LCXX_TIMEOUT_UserCallback();};
        }    
        e24LCXX_WaitEepromStandbyState();
      }
    }
  }
  /*!< If WriteAddr is not e24LCXX_PAGESIZE aligned  */
  else 
  {
    /*!< If NumByteToWrite < e24LCXX_PAGESIZE */
    if(NumOfPage== 0) 
    {
      /*!< If the number of data to be written is more than the remaining space 
      in the current page: */
      if (NumByteToWrite > count)
      {
        /* Store the number of data to be written */
        sEEDataNum = count;        
        /*!< Write the data conained in same page */
        e24LCXX_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = e24LCXX_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {return e24LCXX_TIMEOUT_UserCallback();};
        }          
        e24LCXX_WaitEepromStandbyState();      
        
        /* Store the number of data to be written */
        sEEDataNum = (NumByteToWrite - count);          
        /*!< Write the remaining data in the following page */
        e24LCXX_WritePage((uint8_t*)(pBuffer + count), (WriteAddr + count), (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = e24LCXX_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {return e24LCXX_TIMEOUT_UserCallback();};
        }     
        e24LCXX_WaitEepromStandbyState();        
      }      
      else      
      {
        /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;         
        e24LCXX_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = e24LCXX_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {return e24LCXX_TIMEOUT_UserCallback();};
        }          
        e24LCXX_WaitEepromStandbyState();        
      }     
    }
    /*!< If NumByteToWrite > e24LCXX_PAGESIZE */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / e24LCXX_PAGESIZE;
      NumOfSingle = NumByteToWrite % e24LCXX_PAGESIZE;
      
      if(count != 0)
      {  
        /* Store the number of data to be written */
        sEEDataNum = count;         
        e24LCXX_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = e24LCXX_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {return e24LCXX_TIMEOUT_UserCallback();};
        }     
        e24LCXX_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        /* Store the number of data to be written */
        sEEDataNum = e24LCXX_PAGESIZE;          
        e24LCXX_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = e24LCXX_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {return e24LCXX_TIMEOUT_UserCallback(); };
        }        
        e24LCXX_WaitEepromStandbyState();
        WriteAddr +=  e24LCXX_PAGESIZE;
        pBuffer += e24LCXX_PAGESIZE;  
      }
      if(NumOfSingle != 0)
      {
        /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;           
        e24LCXX_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum)); 
        /* Wait transfer through DMA to be complete */
        sEETimeout = e24LCXX_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {return e24LCXX_TIMEOUT_UserCallback(); };
        }         
        e24LCXX_WaitEepromStandbyState();
      }
    }
  } 
  return TRUE; 
}

/*********************************************************
** 函数名：	 e24LCXX_WaitEepromStandbyState
** 输　入：  无
** 输　出：  成功返回0 失败返回非0
** 功能描述：EEPROM(24LC16)等待写入完成子程序。
** 作　者:	 李微辰
** 日　期：  2013.03.06
** 版本：    V1.0A
************************************************************/
uint32_t e24LCXX_WaitEepromStandbyState(void)      
{
  __IO uint16_t tmpSR1 = 0;
  __IO uint32_t sEETrials = 0;

  /*!< While the bus is busy */
  sEETimeout = e24LCXX_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(e24LCXX_I2C, I2C_FLAG_BUSY))
  {
    if((sEETimeout--) == 0) return e24LCXX_TIMEOUT_UserCallback();
  }

  /* Keep looping till the slave acknowledge his address or maximum number 
     of trials is reached (this number is defined by e24LCXX_MAX_TRIALS_NUMBER define
     in stm32_eval_i2c_ee.h file) */
  while (1)
  {
    /*!< Send START condition */
    I2C_GenerateSTART(e24LCXX_I2C, ENABLE);

    /*!< Test on EV5 and clear it */
    sEETimeout = e24LCXX_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(e24LCXX_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
      if((sEETimeout--) == 0) return e24LCXX_TIMEOUT_UserCallback();
    }    

    /*!< Send EEPROM address for write */
    I2C_Send7bitAddress(e24LCXX_I2C, sEEAddress, I2C_Direction_Transmitter);
    
    /* Wait for ADDR flag to be set (Slave acknowledged his address) */
    sEETimeout = e24LCXX_LONG_TIMEOUT;
    do
    {     
      /* Get the current value of the SR1 register */
      tmpSR1 = e24LCXX_I2C->SR1;
      
      /* Update the timeout value and exit if it reach 0 */
      if((sEETimeout--) == 0) return e24LCXX_TIMEOUT_UserCallback();
    }
    /* Keep looping till the Address is acknowledged or the AF flag is 
       set (address not acknowledged at time) */
    while((tmpSR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) == 0);
     
    /* Check if the ADDR flag has been set */
    if (tmpSR1 & I2C_SR1_ADDR)
    {
      /* Clear ADDR Flag by reading SR1 then SR2 registers (SR1 have already 
         been read) */
      (void)e24LCXX_I2C->SR2;
      
      /*!< STOP condition */    
      I2C_GenerateSTOP(e24LCXX_I2C, ENABLE);
        
      /* Exit the function */
      return e24LCXX_OK;
    }
    else
    {
      /*!< Clear AF flag */
      I2C_ClearFlag(e24LCXX_I2C, I2C_FLAG_AF);                  
    }
    
    /* Check if the maximum allowed numbe of trials has bee reached */
    if (sEETrials++ == e24LCXX_MAX_TRIALS_NUMBER)
    {
      /* If the maximum number of trials has been reached, exit the function */
      return e24LCXX_TIMEOUT_UserCallback();
    }
  }
}

/*********************************************************
** 函数名：	 e24LCXX_I2C_DMA_TX_IRQHandler
** 输　入：  无
** 输　出：  无
** 功能描述：EEPROM(24LC16)DMA发送中断子程序。
** 作　者:	 李微辰
** 日　期：  2013.03.06
** 版本：    V1.0A
************************************************************/
void e24LCXX_I2C_DMA_TX_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr;

	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
	OSIntEnter();
	OS_EXIT_CRITICAL();

	/* Check if the DMA transfer is complete */ 
  if(DMA_GetFlagStatus(e24LCXX_I2C_DMA_FLAG_TX_TC) != RESET)
  {  
    /* Disable the DMA Tx Channel and Clear all its Flags */  
    DMA_Cmd(e24LCXX_I2C_DMA_CHANNEL_TX, DISABLE);
    DMA_ClearFlag(e24LCXX_I2C_DMA_FLAG_TX_GL);

    /*!< Wait till all data have been physically transferred on the bus */
    sEETimeout = e24LCXX_LONG_TIMEOUT;
    while(!I2C_GetFlagStatus(e24LCXX_I2C, I2C_FLAG_BTF))
    {
      if((sEETimeout--) == 0) e24LCXX_TIMEOUT_UserCallback();
    }
    
    /*!< Send STOP condition */
    I2C_GenerateSTOP(e24LCXX_I2C, ENABLE);
    
    /* Perform a read on SR1 and SR2 register to clear eventualaly pending flags */
    (void)e24LCXX_I2C->SR1;
    (void)e24LCXX_I2C->SR2;
    
    /* Reset the variable holding the number of data to be written */
    *sEEDataWritePointer = 0;  
  }
	OSIntExit();
}

/*********************************************************
** 函数名：	 e24LCXX_I2C_DMA_RX_IRQHandler
** 输　入：  无
** 输　出：  无
** 功能描述：EEPROM(24LC16)DMA接收子程序。
** 作　者:	 李微辰
** 日　期：  2013.03.06
** 版本：    V1.0A
************************************************************/
void e24LCXX_I2C_DMA_RX_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr;

	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
	OSIntEnter();
	OS_EXIT_CRITICAL();

	/* Check if the DMA transfer is complete */
  if(DMA_GetFlagStatus(e24LCXX_I2C_DMA_FLAG_RX_TC) != RESET)
  {      
    /*!< Send STOP Condition */
    I2C_GenerateSTOP(e24LCXX_I2C, ENABLE);    
    
    /* Disable the DMA Rx Channel and Clear all its Flags */  
    DMA_Cmd(e24LCXX_I2C_DMA_CHANNEL_RX, DISABLE);
    DMA_ClearFlag(e24LCXX_I2C_DMA_FLAG_RX_GL);
    
    /* Reset the variable holding the number of data to be read */
    *sEEDataReadPointer = 0;
  }
	OSIntExit();
}

/*********************************************************
** 函数名：e24LCXX_TIMEOUT_UserCallback
** 输　入：无
** 输　出：无
** 功能描述：错误操作处理子函数。
** 作　者:	李微辰
** 日　期： 2012.12.20
** 版本：   V1.0A
************************************************************/
uint32_t e24LCXX_TIMEOUT_UserCallback(void)
{
	/* Block communication and all processes */
	return (FAIL_ERROR_TIME_OUT);
}
/*********************************************************
** 函数名：	 e24LCXX_EnterCriticalSection_UserCallback
** 输　入：  无
** 输　出：  无
** 功能描述：EEPROM(24LC16)屏蔽中断子程序。
** 作　者:	 李微辰
** 日　期：  2013.03.06
** 版本：    V1.0A
************************************************************/
void e24LCXX_EnterCriticalSection_UserCallback(void)
{
  __disable_irq();  
}
/*********************************************************
** 函数名：	 e24LCXX_ExitCriticalSection_UserCallback
** 输　入：  无
** 输　出：  无
** 功能描述：EEPROM(24LC16)使能中断子程序。
** 作　者:	 李微辰
** 日　期：  2013.03.06
** 版本：    V1.0A
************************************************************/
void e24LCXX_ExitCriticalSection_UserCallback(void)
{
  __enable_irq();
}
//----------------------------------------------------------
#if !defined(INIT_FLASH1) && !defined(INIT_FLASH2)
/*********************************************************
** 函数名：Para_Read
** 输　入：u16 u16Off  ----  参数地址（与参数表偏移相同） 
**         u16 *u16Dst ----  读取数据保存容器
** 输　出：成功返回0 失败返回非0
** 功能描述：读取1-2字节数据
** 日　期：2013-03-18
** 修　改：
** 日　期：
** 版本：V1.0
************************************************************/
u16 Para_Read(u16 u16Off, u16 *u16Dst, u16 u16len)
{
	uint32_t TimeOut = e24LCXX_LONG_TIMEOUT;
	INT8U err;

	//lock
	OSMutexPend(MutexParRead, 0, &err);	//等待参数读取互斥信号量

	//GPIO_ResetBits(e24LCXX_I2C_WP_GPIO_PORT, e24LCXX_I2C_WP_PIN);												//关闭EEPROM写保护。

	NumDataTran = 4;
	e24LCXX_ReadBuffer((uint8_t*)group_tmp_rd, (u16Off&0xfffe)<<1, (uint16_t*)(&NumDataTran));	  //读取整体（4字节）参数块
	while (NumDataTran > 0)
	{ 
		if((TimeOut--) == 0)
		{
			//GPIO_SetBits(e24LCXX_I2C_WP_GPIO_PORT, e24LCXX_I2C_WP_PIN);												//EEPROM写保护。
			//unlock
			OSMutexPost(MutexParRead);	//释放参数读取互斥信号量
			return FAIL_ERROR_TIME_OUT;	//超时退出
		}
	}
	if(CRC_8(((uint16_t)group_tmp_rd[1]<<8)|group_tmp_rd[0]) != group_tmp_rd[3])
	{
		//GPIO_SetBits(e24LCXX_I2C_WP_GPIO_PORT, e24LCXX_I2C_WP_PIN);												//EEPROM写保护。
		//unlock
		OSMutexPost(MutexParRead);		//释放参数读取互斥信号量
		return FAIL_ERROR_CHECK_SUM;	//校验错误，退出
	}
	*u16Dst = group_tmp_rd[u16Off&0x01];
	if (u16len > 1)
	{
		*u16Dst |= ((u16)group_tmp_rd[1] << 8);
	}
	//GPIO_SetBits(e24LCXX_I2C_WP_GPIO_PORT, e24LCXX_I2C_WP_PIN);												//EEPROM写保护。
	//unlock
	OSMutexPost(MutexParRead);			//释放参数读取互斥信号量
	return SUCCESS_NUM;
}
/*********************************************************
** 函数名：Para_u16Write
** 输　入：u16 u16Off   ---- 参数地址（与参数表偏移相同） 
**         u16 u16Data  ---- 写入数据
** 输　出：成功返回0 失败返回非0
** 功能描述：写入2字节
** 日　期：2013-03-18
** 修　改：
** 日　期：
** 版本：V1.0
************************************************************/
static u16 Para_u16Write(u16 u16Off, u16 u16Data)
{
	uint32_t TimeOut;
	volatile uint8_t data_num;

	//GPIO_ResetBits(e24LCXX_I2C_WP_GPIO_PORT, e24LCXX_I2C_WP_PIN);												//关闭EEPROM写保护。

	group_tmp_wr[0] = u16Data&0xff;
	group_tmp_wr[1] = u16Data>>8;
	group_tmp_wr[2] = 0;
	group_tmp_wr[3] = CRC_8((unsigned short)u16Data);
	data_num = 4;
	e24LCXX_WritePage(group_tmp_wr, u16Off, (uint8_t*)&data_num);
	/* Wait transfer through DMA to be complete */
	TimeOut = e24LCXX_LONG_TIMEOUT;
	while (data_num > 0)
	{
		if((TimeOut--) == 0)
		{
			//GPIO_SetBits(e24LCXX_I2C_WP_GPIO_PORT, e24LCXX_I2C_WP_PIN);												//EEPROM写保护。
			return e24LCXX_TIMEOUT_UserCallback();
		}
	}

	//GPIO_SetBits(e24LCXX_I2C_WP_GPIO_PORT, e24LCXX_I2C_WP_PIN);												//EEPROM写保护。
	return SUCCESS_NUM;			//写入
}
/***********************************************************/
u16 Para_Save(u16 u16Off, u16 u16Cnt)
{
	OS_TCB pdata;
	INT8U err;

	//lock
	OSMutexPend(MutexParSaved, 0, &err);	//等待参数保存互斥信号量

	if ((vu32ParSavedQueueCnt < PAR_SAVED_QUEUE_COUNT) && u16Cnt)
	{
		stParSavedQueue[u16ParSavedQueueEnd].u16Offset = u16Off&0xfffe;			//变换为偶数
		stParSavedQueue[u16ParSavedQueueEnd].u16Count = (u16Cnt+1)&0xfffe;		//变换为偶数
		u16ParSavedQueueEnd = (u16ParSavedQueueEnd+1) % PAR_SAVED_QUEUE_COUNT;
		vu32ParSavedQueueCnt++;

		
		OSTaskQuery(DRIVER_TASK_PRIO, &pdata);	//PAR_SAVED_TASK_PRIO, &pdata);						//查询写入任务时候被挂起
		if(pdata.OSTCBStat == OS_STAT_SUSPEND)							//挂起
		{
			OSTaskResume(DRIVER_TASK_PRIO);	//PAR_SAVED_TASK_PRIO);							//唤醒任务
	  	}

		//unlock
		OSMutexPost(MutexParSaved);			//释放参数保存互斥信号量
		return(1);
	}
	//unlock
	OSMutexPost(MutexParSaved);				//释放参数保存互斥信号量
	return(0);
}
/***********************************************************/
static void DelParSavedQueue(void)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif

	//关全局中断？
	OS_ENTER_CRITICAL();
	if (vu32ParSavedQueueCnt)
	{
		u16ParSavedQueueBegin = (u16ParSavedQueueBegin+1) % PAR_SAVED_QUEUE_COUNT;
		vu32ParSavedQueueCnt--;
	}
	//开全局中断？
	OS_EXIT_CRITICAL();
}

/*********************************************************
** 函数名：	 Task_Par_Saved
** 输　入：  无
** 输　出：  无
** 功能描述：EEPEOM(24LC16)写入参数任务函数。每次写2bytes，间隔时间10ms
** 作　者:	 田东升
** 日　期：  2013.03.06
** 版本：    V1.0A
************************************************************/
void Task_Par_Saved(void)
{
	for(;;)
	{
		if (vu32ParSavedQueueCnt)
		{
			GPIO_ResetBits(e24LCXX_I2C_WP_GPIO_PORT, e24LCXX_I2C_WP_PIN);		//关闭EEPROM写保护。

			u16OffsetTmp = stParSavedQueue[u16ParSavedQueueBegin].u16Offset;	//已变换为偶数
			u16CountTmp = stParSavedQueue[u16ParSavedQueueBegin].u16Count;		//已变换为偶数
//			if (!Para_u16Write(u16OffsetTmp<<1, GET_PT_U16(u16OffsetTmp)))		//保存2bytes
//			{
//				u16CountTmp -= 2;
//				if (!u16CountTmp)												//保存字节数不大于2，则删除队列中首项
//					DelParSavedQueue();
//				else															//保存字节数大于2，则修改队列中首项中的位移和保存字节数
//				{
//					u16OffsetTmp += 2;
//					stParSavedQueue[u16ParSavedQueueBegin].u16Offset = u16OffsetTmp;
//					stParSavedQueue[u16ParSavedQueueBegin].u16Count = u16CountTmp;
//				}
//			}
			//执行周期20ms
			OSTimeDly(20/TICK_TIME);											//确保至少延时10ms

			GPIO_SetBits(e24LCXX_I2C_WP_GPIO_PORT, e24LCXX_I2C_WP_PIN);			//EEPROM写保护。
		}
		else
		{
			OSTaskSuspendIf0(OS_PRIO_SELF, (INT32U *)&vu32ParSavedQueueCnt);	//否 将自己挂起
		}
	}
}
#endif

