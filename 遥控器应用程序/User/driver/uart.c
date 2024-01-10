/********************************************************************************
* 文件名：	usart.c
* 作者：	马如意   
* 版本：   	V1.0
* 日期：    2015.04.24
* 功能描述:  实现UART的驱动，实现红外数据接收的底层驱动。

* 修改说明：   
*
*       >>>>  在工程中的位置  <<<<
*             3-应用层
*             2-协议层
*         √   1-硬件驱动层
*********************************************************************************
* @copy
* <h2><center>&copy; COPYRIGHT 天津华宁电子有限公司 研发中心 软件部</center></h2>
*********************************************************************************/
/********************************************************************************
* .h头文件
*********************************************************************************/
#include "includes.h"

/********************************************************************************
* #define宏定义
*********************************************************************************/

/********************************************************************************
* 常量定义
*********************************************************************************/
#define  IR_USART_RX_BUF_MAX              200
#define  IR_UASRT_TX_BUF_MAX              200

/********************************************************************************
* 变量定义
*********************************************************************************/
static uint8_t		s_IrUsartRxBuf[IR_USART_RX_BUF_MAX];	//接收缓存
static uint32_t	s_u32IrUsartRxWrtPtr = 0;			   	//接收写指针
static uint32_t	s_u32IrUsartRxRdPtr = 0;			   		//接收读指针

static uint8_t  	s_IrUsartTxBuf[IR_UASRT_TX_BUF_MAX];	//发送缓存
static uint32_t	s_u32IrUsartTxWrtPtr = 0;			   	//发送写指针
static uint32_t	s_u32IrUsartTxRdPtr = 0;			   		//发送读指针

OS_EVENT  *s_stIrRecvd;	//红外接收信号量

/********************************************************************************
* 函数定义
*********************************************************************************/

/**********************************************************************************************************
*	函 数 名: IrUsartParamInit
*	功能说明: IrUsart参数初始化
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
void IrUsartParamInit(void)
{
	s_u32IrUsartRxWrtPtr = 0;
	s_u32IrUsartRxRdPtr = 0;
	memset(s_IrUsartTxBuf,0x00,sizeof(s_IrUsartTxBuf));
	
	s_u32IrUsartTxWrtPtr = 0;
	s_u32IrUsartTxRdPtr = 0;	
	memset(s_IrUsartRxBuf,0x00,sizeof(s_IrUsartRxBuf));
	
}
/**********************************************************************************************************
*	函 数 名: IrUsartInit
*	功能说明: IrUsart设备初始化
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
void IrUsartInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;   
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	IrUsartParamInit();
	
	RCC_AHBPeriphClockCmd(RCC_IR_USART_RX_GPIO|RCC_IR_USART_ENABLE_GPIO|RCC_AHBPeriph_GPIOC, ENABLE);//串口端口时钟使能
	IR_USART_APBxCmd();
	
	/* Configure USART2 Rx as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//GPIO_Mode_AF
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//GPIO_OType_PP	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = IR_USART_RX_PIN;
	GPIO_Init(IR_USART_RX_GPIO, &GPIO_InitStructure); 

	GPIO_PinAFConfig(IR_USART_RX_GPIO,IR_USART_RX_PinSource,IR_USART_PIN_AF_SET); //重映射	
	
	if(g_u32ID == B_type)
	{
		/**接收使能端口初始化***/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//GPIO_OType_PP
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = IR_USART_ENABLE_PIN;	
		GPIO_Init(IR_USART_ENABLE_GPIO, &GPIO_InitStructure);

		/* Set LED_LOCK_PIN */
		IrUsartChipPwrOff();//关闭接收
	}
	else
	{
		/**接收使能端口初始化***/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//GPIO_OType_PP
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		/* Set LED_LOCK_PIN */
		GPIO_SetBits(GPIOC, GPIO_Pin_6);//关闭接收
	}

	////下面那两句很重要，不能缺
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,ENABLE);//复位串口2
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,DISABLE);//停止复位
	
	USART_InitStructure.USART_BaudRate = IR_USART_BAUNDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;		//无校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;//|USART_Mode_Tx; 
	
	USART_Init(IR_USART, &USART_InitStructure);				//红外

	/* Configure the NVIC Preemption Priority Bits */  
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = IR_USART_IRQN;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x07;//不使用中断优先级嵌套。因为SysTick的中断优先级为0x0f
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	s_stIrRecvd  = OSSemCreate(0);	//创建接收数据信号量

	USART_ITConfig(IR_USART, USART_IT_RXNE, ENABLE);	//中断使能
	USART_ITConfig(IR_USART, USART_IT_TXE, DISABLE);
	USART_ITConfig(IR_USART, USART_IT_TC, DISABLE);
	

	IrUsartRxDisable();
}

/**********************************************************************************************************
*	函 数 名: IrUsartReadRxBuf
*	功能说明: 读取IrUsart的接收缓冲区
*	形    参: u8Data:数据帧的指针
*	返 回 值: 0x00:无数据需要处理，0x01:有数据需要处理
**********************************************************************************************************/
u32 IrUsartReadRxBuf(u8 *u8Data,u32 u32Ptr)
{
	*u8Data =  s_IrUsartRxBuf[u32Ptr];

	return 0x01;
}

/**********************************************************************************************************
*	函 数 名: IrUsartRxBufSizeGet
*	功能说明: 读取IrUsart接收缓冲区的大小
*	形    参: u32Size:数据帧的指针
*	返 回 值: 0x01:函数正常返回
**********************************************************************************************************/
u32 IrUsartRxBufSizeGet(u32 *u32Size)
{
	*u32Size = IR_USART_RX_BUF_MAX;
	
	return 0x01;
}

/**********************************************************************************************************
*	函 数 名: IrUsartRxRdRtrGet
*	功能说明: 读取IrUsart的接收缓冲区读指针
*	形    参: u8Data:数据的指针
*	返 回 值: 0x01:函数正常返回
**********************************************************************************************************/
u32 IrUsartRxRdRtrGet(u32 *u32Ptr)
{
	*u32Ptr = s_u32IrUsartRxRdPtr;
	
	return 0x01;
}

/**********************************************************************************************************
*	函 数 名: IrUsartReadRxBuf
*	功能说明: 读取IrUsart的接收缓冲区
*	形    参: u8Data:数据帧的指针
*	返 回 值: 0x01:函数正常返回
**********************************************************************************************************/
u32 IrUsartRxRdRtrSet(u32 *u32Data)
{
	s_u32IrUsartRxRdPtr = *u32Data;
	s_u32IrUsartRxRdPtr %= IR_USART_RX_BUF_MAX;
	
	return 0x01;	
}

/**********************************************************************************************************
*	函 数 名: IrUsartRxWtrRtrGet
*	功能说明: 读取IrUsart的接收缓冲区写指针
*	形    参: u8Data:数据的指针
*	返 回 值: 0x00:无数据需要处理，0x01:有数据需要处理
**********************************************************************************************************/
u32 IrUsartRxWtrRtrGet(u32 *u32Data)
{
	*u32Data = s_u32IrUsartRxWrtPtr;
	
	return 0x01;
}

/**********************************************************************************************************
*	函 数 名: IR_USART_IRQHandler
*	功能说明: 中断处理
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
void IR_USART_IRQHandler(void)
{
	uint16_t  data;
	uint16_t c;
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
			OS_CPU_SR  cpu_sr = 0;
	#endif


	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
	OSIntEnter();
	OS_EXIT_CRITICAL();
	if(USART_GetITStatus(IR_USART, USART_IT_RXNE) != RESET)
	{
		data = USART_ReceiveData(IR_USART);
		OS_ENTER_CRITICAL();
		s_IrUsartRxBuf[s_u32IrUsartRxWrtPtr ++] = (u8)(data & 0xff);
        s_u32IrUsartRxWrtPtr %= IR_USART_RX_BUF_MAX;
		OS_EXIT_CRITICAL();	
		#if 0//test
			s_IrUsartRxBuf[0x00] = 0x82;
			s_IrUsartRxBuf[0x01] = 0x41;
			s_IrUsartRxBuf[0x02] = 0x47;
			s_u32IrUsartRxRdPtr = 0x00;
			s_u32IrUsartRxWrtPtr = 0x03;	
		#endif		
		OSSemPost(s_stIrRecvd);
	}
	
	if (USART_GetITStatus(IR_USART, USART_IT_TC) != RESET)
	{ 	
		USART_ClearITPendingBit(IR_USART, USART_FLAG_TC);
		s_u32IrUsartTxRdPtr %= IR_UASRT_TX_BUF_MAX;
		USART_SendData(IR_USART, c);
		if(s_u32IrUsartTxRdPtr == s_u32IrUsartTxWrtPtr)
		{
			USART_ITConfig(IR_USART, USART_IT_TXE, DISABLE);		
		}
	}
	OSIntExit();
}

