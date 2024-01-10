/********************************************************************************
* 文件名：	led.c
* 作者：	马如意   
* 版本：   	V1.0
* 日期：    2015.04.24
* 功能描述:  实现LED的驱动

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
#include	"includes.h"
/********************************************************************************
* #define宏定义
*********************************************************************************/
#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
/********************************************************************************
* 常量定义
*********************************************************************************/
#define		KEY_LED_NUMB_MAX			12//最大为12个按键灯
#define   KEY_LED_NUMB_MAX_D    16

#define		KEY_LED_TIMER_CLOSED		0xFFFFFFFF//计时器关闭
#define		KEY_LED_TIMER_EXPIRE		0x00//计时器溢出

#define		KEY_LED_SCAN_PERIOD			75//7.5ms扫描周期
/********************************************************************************
* 变量定义
*********************************************************************************/
u32 s_u32LedKeyStatus = 0x00;
u8 u8D_typeLedFalg = 0;
#if (KEY_LED_TOGGLE_ENABLE_FLAG == ENABLED)
	u32 s_u32KeyLedOnTimeSet[KEY_LED_NUMB_MAX_D];//存取点亮时间，单位ms
	u32 s_u32KeyLedOffTimeSet[KEY_LED_NUMB_MAX_D];//存取熄灭时间，单位ms
	u32 s_u32KeyLedTimer[KEY_LED_NUMB_MAX_D];//总体点亮熄灭计时，单位ms
#endif
/********************************************************************************
* 函数定义
*********************************************************************************/
/***************************************************************************************
** 函数名称:KeyLedInit
** 功能描述：按键Led指示灯初始化
** 函数输入：无
** 函数输出：1：正常，0：异常
** 注意事项：
******************************************************************************************/
u32 KeyLedInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;			             // 定义定时器结构体
	NVIC_InitTypeDef NVIC_InitStructure;
	#if (KEY_LED_TOGGLE_ENABLE_FLAG == ENABLED)
		u32 u32I = 0x00;
	#endif
	/* GPIO Periph clock enable */
	RCC_AHBPeriphClockCmd(KEY_MOSI_GPIO_RCC|KEY_LCK_GPIO_RCC|KEY_SCK_GPIO_RCC, ENABLE);
	
	if(g_u32ID != B_type)
		RCC_AHBPeriphClockCmd(KEY_595_ENABLE_RCC,ENABLE);

	/* Configure KEY_LED_MOSI_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = KEY_LED_MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(KEY_LED_MOSI_PORT, &GPIO_InitStructure);

	/* Reset KEY_LED_MOSI_PIN */
	//GPIO_ResetBits(KEY_LED_MOSI_PORT, KEY_LED_MOSI_PIN);
	GPIO_SetBits(KEY_LED_MOSI_PORT, KEY_LED_MOSI_PIN);
	
	/* Configure KEY_LED_SCK_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = KEY_LED_SCK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(KEY_LED_SCK_PORT, &GPIO_InitStructure);

	/* Reset KEY_LED_SCK_PIN */
	//GPIO_ResetBits(KEY_LED_SCK_PORT, KEY_LED_SCK_PIN);
	GPIO_SetBits(KEY_LED_SCK_PORT, KEY_LED_SCK_PIN);
	
	/* Configure KEY_LED_LCK_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = KEY_LED_LCK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(KEY_LED_LCK_PORT, &GPIO_InitStructure);

	/* Reset KEY_LED_LCK_PIN */
	//GPIO_ResetBits(KEY_LED_LCK_PORT, KEY_LED_LCK_PIN);
	GPIO_SetBits(KEY_LED_LCK_PORT, KEY_LED_LCK_PIN);
	
	
	
	if(g_u32ID == B_type)
	{
		/* Configure LED_STOP_PIN in output pushpull mode */
		GPIO_InitStructure.GPIO_Pin = KEY_595_ENABLE_PIN;	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(KEY_595_ENABLE_PORT, &GPIO_InitStructure);
		
		KEY595ChipOUTOn();
	}

	
	s_u32LedKeyStatus = 0x00;

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);							// 初始化定时器2结构体
	TIM_DeInit(TIM5);														// 设置TIM2默认模式

	/* TIM2 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	TIM_TimeBaseStructure.TIM_Period=14999;									// 设置初始值,定时7.5ms中断
	TIM_TimeBaseStructure.TIM_Prescaler=9;									// 设置定时器3的分频值，时钟为2MHz
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;						// 设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;					// TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);							// 按结构体的值，初始化定时器2
	TIM_InternalClockConfig(TIM5);											// 设置TIM5的时钟为内部时钟
	TIM5->CNT = 0x0000;														// 定时器3计数器清零
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);								// 使能TIM5溢出中断源
	TIM_ARRPreloadConfig(TIM5, ENABLE);										// 使能自动重装
	TIM_Cmd(TIM5, DISABLE);													// 禁止TIM5

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;							// 使能或者失能指定的IRQ通道 TIM3全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x06;			// 不使用中断优先级嵌套。因为SysTick的中断优先级为0x0f
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//1;						// 设置成员 NVIC_IRQChannel中的从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							// 使能 NVIC_IRQChannel
	NVIC_Init(&NVIC_InitStructure);    

	#if (KEY_LED_TOGGLE_ENABLE_FLAG == ENABLED)
		//完成变量的初始化

		for(u32I = 0x00; u32I < KEY_LED_NUMB_MAX_D;u32I++)
		{
			s_u32KeyLedOnTimeSet[u32I] = 0x00;//存取点亮时间，单位ms
			s_u32KeyLedOffTimeSet[u32I] = 0x00;//存取熄灭时间，单位ms
			s_u32KeyLedTimer[u32I] = 0x00;//总体点亮熄灭计时，单位ms			
		}
	#endif
	
	TIM_Cmd(TIM5, ENABLE);	
	
	return 0x01;
}

/***************************************************************************************
** 函数名称:SH_CP_595
** 功能描述：移位  
** 函数输入：无
** 函数输出：无
** 注意事项：
******************************************************************************************/
static void SH_CP_595(void)
{ 
	GPIO_ResetBits(KEY_LED_SCK_PORT, KEY_LED_SCK_PIN);
	GPIO_ResetBits(KEY_LED_SCK_PORT, KEY_LED_SCK_PIN);
	GPIO_SetBits(KEY_LED_SCK_PORT, KEY_LED_SCK_PIN);	//上升沿移位    
	GPIO_SetBits(KEY_LED_SCK_PORT, KEY_LED_SCK_PIN);	//上升沿移位    
}  

/***************************************************************************************
** 函数名称:ST_CP_595
** 功能描述：锁存到输出并显示 
** 函数输入：无
** 函数输出：无
** 注意事项：
******************************************************************************************/
static void ST_CP_595(void)//  
{ 
	GPIO_ResetBits(KEY_LED_LCK_PORT, KEY_LED_LCK_PIN);
	GPIO_ResetBits(KEY_LED_LCK_PORT, KEY_LED_LCK_PIN);
	GPIO_SetBits(KEY_LED_LCK_PORT, KEY_LED_LCK_PIN);	//上升沿将数据送到输出锁存器    
	GPIO_SetBits(KEY_LED_LCK_PORT, KEY_LED_LCK_PIN);	//上升沿将数据送到输出锁存器    
}  


static u32 D_typeKeyLedMum[14] =  {0x01,0x02,0x004,0x008,0x10,0x20,0x40,0x80,0x100,0x200,0x400,0x800,0x1000,0x2000};
static u32 D_typeKeyLedMum1[14] = {0x02,0x08,0x100,0x200,0x40,0x80,0x20,0x10,0x400,0x800,0x1000,0x2000,0x01,0x04};
/***************************************************************************************
** 函数名称:D版遥控器键盘转B版
** 功能描述：
** 函数输入：无
** 函数输出：无
** 注意事项：
******************************************************************************************/
static u32 keyLed_Btokey_DLed(u32 u32KeyVal)
{
	u32 u32key_DVal = 0;
	u8 i;
	u8 tmp[14];
	u8 isOneNumFlag = 0;
	
	if(u32KeyVal == 0)
	{
		return 0;
	}
//		if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_UNCERTIFIED){
//				if((u32KeyVal&(u32)0x1000) == 0x1000)
//					u32KeyVal^=0x1000;
//				if((u32KeyVal&(u32)0x2000) == 0x2000)
//					u32KeyVal^=0x2000;
//		}
	memset(tmp,0,14);
	if((u32KeyVal&(u32KeyVal-1)) == 0)  //只有一个灯
		isOneNumFlag = 1;
	for(i = 0;i<14;i++)
	{
		if((u32KeyVal&D_typeKeyLedMum[i]) != 0)
		{
			u32KeyVal -= D_typeKeyLedMum[i];
			tmp[i] = 1;
			if(isOneNumFlag)
				break;
		}
	}
	
	u32key_DVal = u32KeyVal;
	
	for(i = 0;i<14;i++)
	{
		if(tmp[i] != 0)
		{
			u32key_DVal|= D_typeKeyLedMum1[i];
			if(isOneNumFlag)
				break;
		}
	}
	return u32key_DVal;
}

/***************************************************************************************
** 函数名称:KeyLed
** 功能描述： 点亮和熄灭按键LED
** 函数输入：无
** 函数输出：无
** 注意事项：
******************************************************************************************/
u32 KeyLed111(u32 u32OnKey, u32 u32OffKey)
{
	
	if(g_u32ID != B_type)
	{
		u32OnKey = keyLed_Btokey_DLed(u32OnKey);
		u32OffKey = keyLed_Btokey_DLed(u32OffKey);
	}
	
	if((u32OnKey > KEY_LED_MAX)||(u32OffKey > KEY_LED_MAX))
		return 0x00;
	
	if (u32OnKey)
	{
		s_u32LedKeyStatus |= u32OnKey;	//点亮部分		
	}
	
	if (u32OffKey)
	{
		s_u32LedKeyStatus &= (~u32OffKey);	//熄灭部分
	}

	return 0x01;

}



/***************************************************************************************
** 函数名称:KeyLedOnOff
** 功能描述： 点亮和熄灭按键LED
** 函数输入：无
** 函数输出：无
** 注意事项：
******************************************************************************************/
u32 KeyLedOnOff(u32 u32OnKey, u32 u32OffKey)
{
	#if (KEY_LED_TOGGLE_ENABLE_FLAG == ENABLED)
		u32 u32I = 0x00;
		u32 u32J = 0x00;
	#endif
	
//	if(g_u32ID != B_type)
//	{			
//		if(InterfaceGetCtrlMode() == INTERFACE_MODE_IDLE)
//		{		
//			if(u32OffKey == (u32)0x2000)
//				u32OffKey =(u32)0x1000;
//			else if(u32OffKey == (u32)0x1000)
//				u32OffKey = (u32)0x2000;
//			
//			if(u32OnKey == (u32)0x2000)
//				u32OnKey = (u32)0x1000;
//			else if(u32OnKey == (u32)0x1000)
//				u32OnKey = (u32)0x2000;			
//		}
//		
//		if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_ZIBAO_CONTROL || InterfaceGetCtrlMode() == INTERFACE_MODE_SC_SELF_CONTROL)
//		{
//			if(u32OnKey != 0)
//				u32OnKey &=0xCFFF;
//		}
//		
//		if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_GRP_CONTROL)
//		{		
//			if(u32OnKey != 0 &&u32OnKey != 0xffff)
//				u32OnKey |=(u32)0xC000;		
//		}
//		if(InterfaceGetCtrlMode() != INTERFACE_MODE_IDLE)
//		{			
//			u32OnKey = keyLed_Btokey_DLed(u32OnKey);
//			u32OffKey = keyLed_Btokey_DLed(u32OffKey);
//		}	
//	}
	
	if((u32OnKey > KEY_LED_MAX)||(u32OffKey > KEY_LED_MAX))
		return 0x00;
	
	if (u32OnKey)
	{
		s_u32LedKeyStatus |= u32OnKey;	//点亮部分	
		#if (KEY_LED_TOGGLE_ENABLE_FLAG == ENABLED)
			u32J = u32OnKey;
			for(u32I = 0x00;u32I < KEY_LED_NUMB_MAX_D;u32I++)
			{
				if((u32J&(1<<u32I)) != 0x00)
				{
					s_u32KeyLedOnTimeSet[u32I] = KEY_LED_TIMER_EXPIRE;//存取点亮时间，单位ms
					s_u32KeyLedOffTimeSet[u32I] = KEY_LED_TIMER_CLOSED;//存取熄灭时间，单位ms
					s_u32KeyLedTimer[u32I] = 0x00;	
					u32J &= ~(1<<u32I);	
				}
				if(u32J == 0x00)
				{
					break;
				}				
			}
		#endif		
	}
	
	if (u32OffKey)
	{
		s_u32LedKeyStatus &= (~u32OffKey);	//熄灭部分
		#if (KEY_LED_TOGGLE_ENABLE_FLAG == ENABLED)
			u32J = u32OffKey;
			for(u32I = 0x00;u32I < KEY_LED_NUMB_MAX_D;u32I++)
			{
				if((u32J&(1<<u32I)) != 0x00)
				{
					s_u32KeyLedOnTimeSet[u32I] = KEY_LED_TIMER_EXPIRE;//存取点亮时间，单位ms
					s_u32KeyLedOffTimeSet[u32I] = KEY_LED_TIMER_EXPIRE;//存取熄灭时间，单位ms
					s_u32KeyLedTimer[u32I] = 0x00;	
					u32J &= ~(1<<u32I);	
				}
				if(u32J == 0x00)
				{
					break;
				}
			}	
		#endif		
	}
	return 0x01;
}

/***************************************************************************************
** 函数名称:KeyLedToggle
** 功能描述:闪烁LED
** 函数输入：无
** 函数输出：无
** 注意事项：
******************************************************************************************/
#if (KEY_LED_TOGGLE_ENABLE_FLAG == ENABLED)
	u32 KeyLedToggle(u32 u32ToggleKey, u32 u32OnTime, u32 u32OffTime)
	{
		u32 u32I = 0x00;
		u32 u32J = 0x00;
		
	if(InterfaceGetCtrlMode() != INTERFACE_MODE_IDLE)
	{
		if(g_u32ID != B_type)
		{
			u32ToggleKey = keyLed_Btokey_DLed(u32ToggleKey);
		}
	}
		

		if(u32ToggleKey > KEY_LED_MAX)
			return 0x00;
			
		if(u32ToggleKey)
		{
			s_u32LedKeyStatus |= u32ToggleKey;	//点亮部分	
			u32J = u32ToggleKey;
			for(u32I = 0x00;u32I < KEY_LED_NUMB_MAX_D;u32I++)
			{
				s_u32KeyLedOnTimeSet[u32I] = 0x00;//不闪烁的键须清零 解决成组时有效键同时按下闪烁异常的问题	2020.04.27 parry
				s_u32KeyLedOffTimeSet[u32I] = 0x00;
				if((u32J&(1<<u32I)) != 0x00)
				{
					s_u32KeyLedOnTimeSet[u32I] = u32OnTime;//存取点亮时间，单位ms
					s_u32KeyLedOffTimeSet[u32I] = u32OffTime;//存取熄灭时间，单位ms	
					s_u32KeyLedTimer[u32I] = 0x00;		
				}
			}
			return 0x01;
		}
		
		return 0x00;
	}
#endif
	
/***************************************************************************************
** 函数名称:LedShift_595
** 功能描述： 移位IO输出1byte数据
** 函数输入：无
** 函数输出：无
** 注意事项：
******************************************************************************************/
static void LedShift_595(u16 u16Val) 
{ 
	u16 u16i;
	  
    for (u16i = 0; u16i < 8; u16i++) 
    { 
	 	if (u16Val & 0x80) 
			GPIO_SetBits(KEY_LED_MOSI_PORT, KEY_LED_MOSI_PIN);
		else
			GPIO_ResetBits(KEY_LED_MOSI_PORT, KEY_LED_MOSI_PIN);
		u16Val = u16Val;
		u16Val <<= 1;
		u16Val = u16Val;
		SH_CP_595(); 
    }         
}

/***************************************************************************************
** 函数名称:KeyLedScan
** 功能描述：按键LED扫描点亮。该函数调用间隔10ms
** 函数输入：无
** 函数输出：无
** 注意事项：
******************************************************************************************/
void KeyLedScan(void)
{
	static u16 s_u16Turn=0;
	u16 u16Temp;
	u16 u16Row;

	#if (KEY_LED_TOGGLE_ENABLE_FLAG == ENABLED)
		u32 u32LedKeyStatusBackup = 0x00;
		u32 u32I = 0x00;
		u32 u32J = 0x00;
	#endif
	
	#if (KEY_LED_TOGGLE_ENABLE_FLAG == ENABLED)
		u32LedKeyStatusBackup = s_u32LedKeyStatus;	
		for(u32I = 0x00;u32I < KEY_LED_NUMB_MAX_D;u32I++)
		{
			if((u32LedKeyStatusBackup&(1<<u32I)) != 0x00)
			{
				if(s_u32KeyLedOnTimeSet[u32I] != KEY_LED_TIMER_EXPIRE)//闪烁状态
				{
					u32J = s_u32KeyLedTimer[u32I]/10;
					u32J %= (s_u32KeyLedOnTimeSet[u32I]+s_u32KeyLedOffTimeSet[u32I]);
					if(u32J > s_u32KeyLedOnTimeSet[u32I] )
					{
						u32LedKeyStatusBackup &=  ~(1<<u32I);
					}
					s_u32KeyLedTimer[u32I] += KEY_LED_SCAN_PERIOD;
				}
			}
		}
		//按行扫描，共4行
		u16Temp = (u16)((u32LedKeyStatusBackup >> (s_u16Turn<<2 )) & 0x00ff);	//取行值		
	#else
		//按行扫描，共4行
		u16Temp = (u16)((s_u32LedKeyStatus >> (s_u16Turn<<2 )) & 0x00ff);	//取行值
	#endif
	if(u16Temp == 0)
		u16Row= 0x00;
	else
		u16Row = (0x0001<<s_u16Turn);
	u16Temp ^= 0x0F;
	u16Temp = (u16Temp<<4)|u16Row;
	if(g_u32ID != B_type)
		KEY595ChipOUTOn();
	LedShift_595(u16Temp);
	ST_CP_595();	//锁存
	if(g_u32ID != B_type)
		KEY595ChipOUTOff();
	//调整下次输出轮次
	s_u16Turn++;
	s_u16Turn &= 0x0003;
}


/***************************************************************************************
** 函数名称:TIM5_IRQHandler
** 功能描述：定时器3溢出中断函数
** 函数输入：无
** 函数输出：1：正常，0：异常
** 注意事项：
******************************************************************************************/
void TIM5_IRQHandler(void)
{	  
	OS_CPU_SR  cpu_sr;

	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
	OSIntEnter();
	OS_EXIT_CRITICAL();

	if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET)							// 判断是否发生定时器溢出中断
	{
		KeyLedScan();
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);							// 定时器溢出中断待处理标记清零
		TIM_ClearFlag(TIM5, TIM_FLAG_Update);								// 与上句功能相同
	}
	OSIntExit();
}
#endif





