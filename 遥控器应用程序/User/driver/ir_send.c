/********************************************************************************
* 文件名：	ir_send.c
* 作者：	马如意   
* 版本：   	V1.0
* 日期：    2015.05.05
* 功能描述:  实现红外数据发送的底层驱动

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

#define CCR_VALUE_SET				131//定时器计数设置
#define	IR_SEND_ARRAY_MAX			69//定时器发送数组

#define	IR_SEND_PRECODE_FIRST_SET	342
#define	IR_SEND_PRECODE_SECND_SET	171
#define	IR_SEND_DATA1_FIRST_SET		21
#define	IR_SEND_DATA1_SECND_SET		64
#define	IR_SEND_DATA0_FIRST_SET		21
#define	IR_SEND_DATA0_SECND_SET		21

#define	IR_SEND_TIME_MAX			30000//进行IR_SEND_ARRAY_MAX数据发送的最大时间，用于等待发送完成判断
/********************************************************************************
* 变量定义
*********************************************************************************/
u16 s_u16TimeArray[IR_SEND_ARRAY_MAX] ; 
u32	s_u32SendPointer = 0x00;	//TIM1中断发送中需要的指针，程序启动后要初始化为0
u16  s_u16SendCount = 0x00;		//发送脉冲计数
OS_EVENT	 *g_stIrSendSem;//红外发射事件信号量
/********************************************************************************
* 函数定义
*********************************************************************************/

/**********************************************************************************************************
*	函 数 名: IrSendTIMConfig
*	功能说明: 所使用的定时器初始化
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
void IrSendTIMConfig(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		            //时间基数设定
	TIM_OCInitTypeDef        TIM_OCInitStructure;				    //TIMx外设初始化
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	u16 PrescalerValue;
	
	if(g_u32ID == B_type)
	{

		/* TIM3 clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		/* GPIOA and GPIOB clock enable */
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);


		/* GPIOB Configuration: Pin 0 and 1 */
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;

		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_TIM2);

		/*--------------------------------- NVIC Configuration -------------------------*/
		/* Enable the TIM3 global Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		/* Compute the prescaler value */
		PrescalerValue = (uint16_t) (SystemCoreClock / 10000000) - 1;

		/* Time base configuration */
		TIM_TimeBaseStructure.TIM_Period = 65535;
		TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		/* Output Compare Toggle Mode configuration: Channel1 */
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = CCR_VALUE_SET;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC3Init(TIM2, &TIM_OCInitStructure);

		TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);

		/* TIM enable counter */
		TIM_Cmd(TIM2, DISABLE);

		/* TIM IT enable */
		TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);
	}
	else
	{
		/* TIM3 clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		/* GPIOA and GPIOB clock enable */
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);


		/* GPIOB Configuration: Pin 0 and 1 */
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;

		GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);

		/*--------------------------------- NVIC Configuration -------------------------*/
		/* Enable the TIM3 global Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		/* Compute the prescaler value */
		PrescalerValue = (uint16_t) (SystemCoreClock / 10000000) - 1;

		/* Time base configuration */
		TIM_TimeBaseStructure.TIM_Period = 65535;
		TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/* Output Compare Toggle Mode configuration: Channel1 */
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = CCR_VALUE_SET;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC2Init(TIM3, &TIM_OCInitStructure);

		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);

		/* TIM enable counter */
		TIM_Cmd(TIM3, DISABLE);

		/* TIM IT enable */
		TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);
	}
	g_stIrSendSem = OSSemCreate(0);
} 


/**********************************************************************************************************
*	函 数 名: IR_SEND_CC_IRQHandler
*	功能说明: 定时器中断处理
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
void TIM3_IRQHandler(void)
{
	uint16_t capture = 0;
	
//	OSIntEnter();
	
	if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
		capture = TIM_GetCapture2(TIM3);
		TIM_SetCompare2(TIM3, capture + CCR_VALUE_SET);
		if((s_u16TimeArray[s_u32SendPointer]*2) != s_u16SendCount)
		{
			s_u16SendCount++;	
		}
		else
		{
			s_u16SendCount=0;
			s_u32SendPointer++;
			#if 1		
				TIM_CCxCmd(TIM3,TIM_Channel_2, (s_u32SendPointer&0x1)?TIM_CCx_Disable:TIM_CCx_Enable);
			#endif

			if(s_u32SendPointer == IR_SEND_ARRAY_MAX)
			{
				TIM_Cmd(TIM3, DISABLE);
				OSSemPost(g_stIrSendSem);
			}
		}
	}
	
//	OSIntExit();
}


/**********************************************************************************************************
*	函 数 名: IR_SEND_CC_IRQHandler
*	功能说明: 定时器中断处理
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
void TIM2_IRQHandler(void)
{
	uint16_t capture = 0;
	
	//OSIntEnter();
	
	if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
		capture = TIM_GetCapture3(TIM2);
		TIM_SetCompare3(TIM2, capture + CCR_VALUE_SET);
		if((s_u16TimeArray[s_u32SendPointer]*2) != s_u16SendCount)
		{
			s_u16SendCount++;	
		}
		else
		{
			s_u16SendCount=0;
			s_u32SendPointer++;
			#if 1		
				TIM_CCxCmd(IR_SEND_TIM,IR_SEND_TIM_CHANNEL, (s_u32SendPointer&0x1)?TIM_CCx_Disable:TIM_CCx_Enable);
			#endif

			if(s_u32SendPointer == IR_SEND_ARRAY_MAX)
			{
				TIM_Cmd(IR_SEND_TIM, DISABLE);
				OSSemPost(g_stIrSendSem	);
			}
		}
	}
	
	//OSIntExit();
}

/**********************************************************************************************************
*	函 数 名: IrSendInsertTxBuf
*	功能说明: 加入发送队列
*	形    参: 无
*	返 回 值: 1:正常发送，0:数据发送异常
**********************************************************************************************************/
u32 IrSendInsertTxBuf(u16 u16customcode,u8 u8customdata)
{
	u16 u16code,u16data1;
	u8  u8I,u8J,u8data;
	INT8U	err;
	
	s_u32SendPointer = 0;							            //TIM1中断发送中需要的指针，程序启动后要初始化为0
	s_u16SendCount = 0;								            //发送脉冲计数
	u16code = u16customcode;
	u8data = u8customdata;										//customdata与其反码组成16位数据
	u16data1=(u16)((~u8data)<<8) + (u16)u8data;
	s_u16TimeArray[0]=IR_SEND_PRECODE_FIRST_SET;										//发送前导码，有载波
	s_u16TimeArray[1]=IR_SEND_PRECODE_SECND_SET;										//无载波
	for(u8I=0,u8J=0;u8I<32;u8I+=2,u8J++)								//发送用户编码
	{
		if((u16code>>u8J)&1)
		{
		                                                    // 输出 1 
			s_u16TimeArray[2+u8I]=IR_SEND_DATA1_FIRST_SET;
			s_u16TimeArray[3+u8I]=IR_SEND_DATA1_SECND_SET;
		}
		else
		{
			s_u16TimeArray[2+u8I]=IR_SEND_DATA0_FIRST_SET;								// 输出 0
			s_u16TimeArray[3+u8I]=IR_SEND_DATA0_SECND_SET;
		}
	}
	
	for(u8I=0,u8J=0;u8I<32;u8I+=2,u8J++)								//发送数据
	{
		if((u16data1>>u8J)&1)
		{
			s_u16TimeArray[34+u8I]=IR_SEND_DATA1_FIRST_SET;								//输出1
			s_u16TimeArray[35+u8I]=IR_SEND_DATA1_SECND_SET;								
		}
		else
		{
			s_u16TimeArray[34+u8I]=IR_SEND_DATA0_FIRST_SET;								//输出0
			s_u16TimeArray[35+u8I]=IR_SEND_DATA0_SECND_SET;
		}
	}
	/**测试时加入，正式不适用**/
	s_u16TimeArray[66]=342;										//发送结束码
	s_u16TimeArray[67]=85;
	s_u16TimeArray[68]=21;
	
	#if 1
	if(g_u32ID == B_type)
		TIM_CCxCmd(IR_SEND_TIM,IR_SEND_TIM_CHANNEL, TIM_CCx_Enable);
	else
		TIM_CCxCmd(TIM3,TIM_Channel_2, TIM_CCx_Enable);
	#endif
	
	g_stIrSendSem->OSEventCnt = 0x00;//清零计数器
	
	if(g_u32ID == B_type)
	{
		TIM_Cmd(IR_SEND_TIM, ENABLE);
		TIM_CCxCmd(IR_SEND_TIM,IR_SEND_TIM_CHANNEL, TIM_CCx_Enable);
	}
	else
	{
		TIM_Cmd(TIM3, ENABLE);
		TIM_CCxCmd(TIM3,TIM_Channel_2, TIM_CCx_Enable);
	}
	
	/**等待数据发送完毕**/
	OSSemPend(g_stIrSendSem, IR_SEND_TIME_MAX/TICK_TIME, &err);
	if(err == OS_ERR_NONE)
	{
		return 0x01;
	}
	else
	{
		return 0x00;
	}
}
/**********************************************************************************************************
*	函 数 名: IrSendInsertTxBuf
*	功能说明: 加入发送队列
*	形    参: 无
*	返 回 值: 1:正常发送，0:数据发送异常
**********************************************************************************************************/
u32 IrLOCSendInsertTxBuf(u16 u16customcode,u8 u8customdata)
{
	u16 u16code,u16data1;
	u8  u8I,u8J,u8data;
	INT8U	err;
	
	s_u32SendPointer = 0;							            //TIM1中断发送中需要的指针，程序启动后要初始化为0
	s_u16SendCount = 0;								            //发送脉冲计数
	u16code = u16customcode;
	u8data = ~((u8)(u16customcode>>8) + (u8)(u16customcode&0xff));									//customdata与其反码组成16位数据
	u16data1=(u16)((~u8data)<<8) + (u16)u8data;
	s_u16TimeArray[0]=IR_SEND_PRECODE_FIRST_SET;										//发送前导码，有载波
	s_u16TimeArray[1]=IR_SEND_PRECODE_SECND_SET;										//无载波
	for(u8I=0,u8J=0;u8I<32;u8I+=2,u8J++)								//发送用户编码
	{
		if((u16code>>u8J)&1)
		{
		                                                    // 输出 1 
			s_u16TimeArray[2+u8I]=IR_SEND_DATA1_FIRST_SET;
			s_u16TimeArray[3+u8I]=IR_SEND_DATA1_SECND_SET;
		}
		else
		{
			s_u16TimeArray[2+u8I]=IR_SEND_DATA0_FIRST_SET;								// 输出 0
			s_u16TimeArray[3+u8I]=IR_SEND_DATA0_SECND_SET;
		}
	}
	
	for(u8I=0,u8J=0;u8I<32;u8I+=2,u8J++)								//发送数据
	{
		if((u16data1>>u8J)&1)
		{
			s_u16TimeArray[34+u8I]=IR_SEND_DATA1_FIRST_SET;								//输出1
			s_u16TimeArray[35+u8I]=IR_SEND_DATA1_SECND_SET;								
		}
		else
		{
			s_u16TimeArray[34+u8I]=IR_SEND_DATA0_FIRST_SET;								//输出0
			s_u16TimeArray[35+u8I]=IR_SEND_DATA0_SECND_SET;
		}
	}
	/**测试时加入，正式不适用**/
	s_u16TimeArray[66]=342;										//发送结束码
	s_u16TimeArray[67]=85;
	s_u16TimeArray[68]=21;
	
	#if 1
	if(g_u32ID == B_type)
		TIM_CCxCmd(IR_SEND_TIM,IR_SEND_TIM_CHANNEL, TIM_CCx_Enable);
	else
		TIM_CCxCmd(TIM3,TIM_Channel_2, TIM_CCx_Enable);
	#endif
	
	g_stIrSendSem->OSEventCnt = 0x00;//清零计数器
	
	if(g_u32ID == B_type)
	{
		TIM_Cmd(IR_SEND_TIM, ENABLE);
		TIM_CCxCmd(IR_SEND_TIM,IR_SEND_TIM_CHANNEL, TIM_CCx_Enable);
	}
	else
	{
		TIM_Cmd(TIM3, ENABLE);
		TIM_CCxCmd(TIM3,TIM_Channel_2, TIM_CCx_Enable);
	}
	
	/**等待数据发送完毕**/
	OSSemPend(g_stIrSendSem, IR_SEND_TIME_MAX/TICK_TIME, &err);
	if(err == OS_ERR_NONE)
	{
		return 0x01;
	}
	else
	{
		return 0x00;                  
	} 
}

