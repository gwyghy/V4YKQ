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
#define  USART_TMP_BUF_MAX            160    //临时接收数组最大值
#define  USART_RX_BUF_MAX              64   //接收队列最大值

uint8_t  UsartReveiveDataBuf[USART_TMP_BUF_MAX];   //临时接收数组

uint8_t  tmpTxWrtPtr  = 0;  //临时变量写指针

stUSARTDATA  USARTKEYRxBuf[USART_RX_BUF_MAX];						//接收缓存
uint8_t	 USARTKEYRxWrtPtr = 0;			   						    //接收写指针
uint8_t	 USARTKEYRxRdPtr =  0;			   							//接收读指针

static uint16_t  u16UsartSendPtr;

extern st_RFIDRcvFrame st_RFIDRcvFrmPool[];		// RFID接收数据存储池
extern u32 u32RcvFrmPoolWritePtr;
extern u32 u32RcvFrmPoolCnt;

extern OS_EVENT *s_stUsartSend ;   //串口接收信号量

extern stUSARTDATA	s_stRfidUsartSendBuf[];
extern u16 s_u16RfidUsartSendBufRead;

extern u32 WlHeartTime;

/********************************************************************************
* 变量定义
*********************************************************************************/


u16 const u16CrcUsarttab[256] = 
{
0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};

/********************************************************************************
* 函数定义
*********************************************************************************/

/*******************************************************************************************
**函数作用：计算CRC16校验值
**函数参数：u8Buf:需要校验的数据起始地址，u32Len:需要校验数据的长度，*u16CheckOld:计算后的CRC校验值
**函数输出：无
**注意事项：无
*******************************************************************************************/
void Crc16UsartSendChick(const u8 *u8Buf, u32 u32Len, u16 *u16CheckOld)
{
	u32 u32Cnt = 0x00;
	u16 u16Crc = *u16CheckOld;
	
	for( u32Cnt = 0; u32Cnt < u32Len; u32Cnt++)
		u16Crc = (u16Crc<<8) ^ u16CrcUsarttab[((u16Crc>>8) ^ *(u8 *)u8Buf++)&0x00FF];
	*u16CheckOld = u16Crc;
}
/*******************************************************************************************
**函数作用：计算CRC16校验值
**函数参数：u8Buf:需要校验的数据起始地址，u32Len:需要校验数据的长度，*u16CheckOld:计算后的CRC校验值
**函数输出：无
**注意事项：无
*******************************************************************************************/
void Crc16UsartReceiveChick(const u8 *u8Buf, u32 u32Len, u16 *u16CheckOld)
{
	u32 u32Cnt = 0x00;
	
	u16 u16Crc = *u16CheckOld;
	
	for( u32Cnt = 0; u32Cnt < u32Len; u32Cnt++)
		u16Crc = (u16Crc<<8) ^ u16CrcUsarttab[((u16Crc>>8) ^ *(u8 *)u8Buf++)&0x00FF];
	*u16CheckOld = u16Crc;
}
/***********************************************************************************************
** 函 数 名：	RFID_GetFrame
** 功能描述：	从接收缓存中取出一个数据包
** 输　  入：	
** 输　  出：	
** 作　  者：	沈万江
** 日　  期：	2014.12.26
** 版    本：	V1.0.0
** 更新记录：
** 更新记录：
** 					日    期      姓    名                    描      述
** 					==========  =============  ========================================
**
************************************************************************************************/
u32 Usart_GetFrame(stUSARTDATA *UsartcvFrm)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr = 0;
	#endif
	if(USARTKEYRxRdPtr != USARTKEYRxWrtPtr)
	{
		OS_ENTER_CRITICAL();//关全局中断
		*UsartcvFrm = USARTKEYRxBuf[USARTKEYRxRdPtr];
		USARTKEYRxRdPtr++;
		USARTKEYRxRdPtr %= USART_RX_BUF_MAX;
		
		OS_EXIT_CRITICAL();//开全局中断
		return 1;
	}
	else
	{
		return 0;
	}
}
/**********************************************************************************************************
*	函 数 名: WlUsartInit
*	功能说明: WlUsart设备初始化
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
void WlUsartInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;   
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
		
	RCC_APB1PeriphClockCmd(RCC_WL_USART_TX_GPIO, ENABLE);//串口端口时钟使能
	WL_USART_APBxCmd();	
	
  /* Configure USART2 Tx as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//GPIO_Mode_AF
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_Pin = WL_USART_TX_PIN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(WL_USART_TX_GPIO, &GPIO_InitStructure);

	GPIO_PinAFConfig(WL_USART_TX_GPIO,WL_USART_TX_PinSource,WL_USART_PIN_AF_SET); //fuyong

	/* Configure USART2 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = WL_USART_RX_PIN;
	GPIO_Init(WL_USART_RX_GPIO, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(WL_USART_RX_GPIO,WL_USART_RX_PinSource,WL_USART_PIN_AF_SET); //复用
	
	USART_InitStructure.USART_BaudRate = WL_USART_BAUNDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;		//无校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;//|USART_Mode_Tx; 
	USART_Init(WL_USART, &USART_InitStructure);				//	

	/* Configure the NVIC Preemption Priority Bits */  
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = WL_USART_IRQN;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//不使用中断优先级嵌套。因为SysTick的中断优先级为0x0f
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(WL_USART, ENABLE);						//串口使能
	USART_ITConfig(WL_USART, USART_IT_RXNE, ENABLE);	//中断使能
}


/**********************************************************************************************************
*	函 数 名: WL_USART_IRQHandler
*	功能说明: 中断处理
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
void WL_USART_IRQHandler(void)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
			OS_CPU_SR  cpu_sr = 0;
	#endif

	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
	OSIntEnter();
	OS_EXIT_CRITICAL();
	//usart接收
	if(USART_GetITStatus(WL_USART, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(WL_USART, USART_IT_RXNE);
		UsartReveiveDataBuf[tmpTxWrtPtr] = (u8)WL_USART->DR;
		//接收数据第一个打开TIM4
		if(tmpTxWrtPtr == 0)
		{
			TIM_Cmd(TIM4, ENABLE);  //使能TIM4
		}
		//接收到的其他数据清零计数器
		else
		{
			TIM4->CNT = 0;//清零TIM4计时
		}
		tmpTxWrtPtr ++;
		tmpTxWrtPtr %= USART_TMP_BUF_MAX;
	}
	//usart接收异常
	else if(USART_GetFlagStatus(WL_USART,USART_FLAG_ORE) != RESET)
	{
		USART_ClearFlag(WL_USART,USART_FLAG_ORE);
		WL_USART->DR;
		TIM4->CNT = 0;
	}
	else
	{
		//usart发送
		if ((WL_USART->SR & USART_FLAG_TXE) != 0)//数据移位完成将新数据存入
		{
			u16UsartSendPtr++;
			if(u16UsartSendPtr == (sizeof(s_stRfidUsartSendBuf[0])-1))//发送一包中最后一个数
			{
				USART_SendData(WL_USART, *((u8*)&s_stRfidUsartSendBuf[s_u16RfidUsartSendBufRead]+u16UsartSendPtr));
				USART_ITConfig(WL_USART, USART_IT_TXE, DISABLE); //关闭USART_IT_TXE中断
				USART_ClearFlag(WL_USART, USART_FLAG_TC); 
				USART_ITConfig(WL_USART, USART_IT_TC, ENABLE);//USART_IT_TC中断
			}
			else if(u16UsartSendPtr < (sizeof(s_stRfidUsartSendBuf[0])-1))
			{
				if(u16UsartSendPtr == (s_stRfidUsartSendBuf[s_u16RfidUsartSendBufRead].u8DLC + 5))
					u16UsartSendPtr = sizeof(s_stRfidUsartSendBuf[0]) - 2;
				USART_SendData(WL_USART, *((u8*)&s_stRfidUsartSendBuf[s_u16RfidUsartSendBufRead]+u16UsartSendPtr));
				USART_ClearFlag(WL_USART, USART_FLAG_TC); //清除USART_FLAG_TC中断标志
			}			
		}
		if ((WL_USART->SR & USART_FLAG_TC) != 0)
		{		
			if (u16UsartSendPtr >= (sizeof(s_stRfidUsartSendBuf[0])-1))//最后一个数据发送完成
			{
				USART_ClearFlag(WL_USART, USART_FLAG_TC); //清除USART_FLAG_TC中断标志
				USART_ITConfig(WL_USART, USART_IT_TC, DISABLE);	//USART_IT_TC中断
				u16UsartSendPtr = 0;
				OSSemPost(s_stUsartSend);
			}
		}
	}
	OSIntExit();
}

/***********************************************************************************************
** 功能描述：	TIM4中断服务函数
** 输　  入：	无
** 输　  出：	无
************************************************************************************************/
void TIM4_IRQHandler(void)   //TIM4中断
{   
	u16 u16CrcTemp = 0x00;
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
			OS_CPU_SR  cpu_sr = 0;
	#endif

	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
	OSIntEnter();
	OS_EXIT_CRITICAL();
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
		tmpTxWrtPtr = 0;
		
		if(UsartReveiveDataBuf[0] == USART_WL_ADDRESS)//数据为遥控器数据
		{
			/*接收处理逻辑*/				
			Crc16UsartReceiveChick(UsartReveiveDataBuf,UsartReveiveDataBuf[4]+5,&u16CrcTemp);
			//校验接收的数据是否正确
			if(UsartReveiveDataBuf[UsartReveiveDataBuf[4]+5] == (u8)((u16CrcTemp&0xFF00)>>8)  
			 && UsartReveiveDataBuf[UsartReveiveDataBuf[4]+6] == (u8)(u16CrcTemp&0x00FF))
			{
				//正确
				if(UsartReveiveDataBuf[2] == WIRELESSDATA && UsartReveiveDataBuf[4]>1) //数据类型为无线数据，并且无线数据长度大于1，为接收到的无线数据，直接存入原来RFID队列，并发送信号量
				{
					//数据存入原来RFID的数据缓存池中
					OS_ENTER_CRITICAL(); 
					st_RFIDRcvFrmPool[u32RcvFrmPoolWritePtr].u8DataLen = UsartReveiveDataBuf[4]-1;
					st_RFIDRcvFrmPool[u32RcvFrmPoolWritePtr].u8DestAddr = 0x00;
					memcpy(st_RFIDRcvFrmPool[u32RcvFrmPoolWritePtr].u8Data,&UsartReveiveDataBuf[5],UsartReveiveDataBuf[4]-1);
					st_RFIDRcvFrmPool[u32RcvFrmPoolWritePtr].u8AppendStatus[0] = UsartReveiveDataBuf[UsartReveiveDataBuf[4]+4];
					u32RcvFrmPoolWritePtr++;
					u32RcvFrmPoolWritePtr %= 50;
					u32RcvFrmPoolCnt++;				
					OS_EXIT_CRITICAL();
					
					OSSemPost(s_stRfidRecvd);
				}
				//心跳和设置数据存入usart的缓存中
				else
				{
					memset(&(USARTKEYRxBuf[USARTKEYRxWrtPtr]), 0x00  , sizeof(stUSARTDATA));
					memcpy(&(USARTKEYRxBuf[USARTKEYRxWrtPtr]), UsartReveiveDataBuf,UsartReveiveDataBuf[4]+5);
					//发应答
					if(UsartReveiveDataBuf[3] == 1)
						SendResponseData((USARTKEYRxBuf[USARTKEYRxWrtPtr].Serial),1,UsartReveiveDataBuf[2]);
					
					USARTKEYRxWrtPtr ++;
					USARTKEYRxWrtPtr %= USART_RX_BUF_MAX;		
				}				
			}
			else
				;
			WlHeartTime = SendHeartTimerMAX;//WL心跳计时器赋值
		}		
		TIM_Cmd(TIM4, DISABLE);  //关闭TIMx
	}
	OSIntExit();
}

