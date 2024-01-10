/********************************************************************************
* �ļ�����	led.c
* ���ߣ�	������   
* �汾��   	V1.0
* ���ڣ�    2015.04.24
* ��������:  ʵ��LED������

* �޸�˵����   
*
*       >>>>  �ڹ����е�λ��  <<<<
*             3-Ӧ�ò�
*             2-Э���
*         ��   1-Ӳ��������
*********************************************************************************
* @copy
* <h2><center>&copy; COPYRIGHT ������������޹�˾ �з����� �����</center></h2>
*********************************************************************************/
/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/
#include	"includes.h"
/********************************************************************************
* #define�궨��
*********************************************************************************/
#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
/********************************************************************************
* ��������
*********************************************************************************/
#define		KEY_LED_NUMB_MAX			12//���Ϊ12��������
#define   KEY_LED_NUMB_MAX_D    16

#define		KEY_LED_TIMER_CLOSED		0xFFFFFFFF//��ʱ���ر�
#define		KEY_LED_TIMER_EXPIRE		0x00//��ʱ�����

#define		KEY_LED_SCAN_PERIOD			75//7.5msɨ������
/********************************************************************************
* ��������
*********************************************************************************/
u32 s_u32LedKeyStatus = 0x00;
u8 u8D_typeLedFalg = 0;
#if (KEY_LED_TOGGLE_ENABLE_FLAG == ENABLED)
	u32 s_u32KeyLedOnTimeSet[KEY_LED_NUMB_MAX_D];//��ȡ����ʱ�䣬��λms
	u32 s_u32KeyLedOffTimeSet[KEY_LED_NUMB_MAX_D];//��ȡϨ��ʱ�䣬��λms
	u32 s_u32KeyLedTimer[KEY_LED_NUMB_MAX_D];//�������Ϩ���ʱ����λms
#endif
/********************************************************************************
* ��������
*********************************************************************************/
/***************************************************************************************
** ��������:KeyLedInit
** ��������������Ledָʾ�Ƴ�ʼ��
** �������룺��
** ���������1��������0���쳣
** ע�����
******************************************************************************************/
u32 KeyLedInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;			             // ���嶨ʱ���ṹ��
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

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);							// ��ʼ����ʱ��2�ṹ��
	TIM_DeInit(TIM5);														// ����TIM2Ĭ��ģʽ

	/* TIM2 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	TIM_TimeBaseStructure.TIM_Period=14999;									// ���ó�ʼֵ,��ʱ7.5ms�ж�
	TIM_TimeBaseStructure.TIM_Prescaler=9;									// ���ö�ʱ��3�ķ�Ƶֵ��ʱ��Ϊ2MHz
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;						// ����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;					// TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);							// ���ṹ���ֵ����ʼ����ʱ��2
	TIM_InternalClockConfig(TIM5);											// ����TIM5��ʱ��Ϊ�ڲ�ʱ��
	TIM5->CNT = 0x0000;														// ��ʱ��3����������
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);								// ʹ��TIM5����ж�Դ
	TIM_ARRPreloadConfig(TIM5, ENABLE);										// ʹ���Զ���װ
	TIM_Cmd(TIM5, DISABLE);													// ��ֹTIM5

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;							// ʹ�ܻ���ʧ��ָ����IRQͨ�� TIM3ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x06;			// ��ʹ���ж����ȼ�Ƕ�ס���ΪSysTick���ж����ȼ�Ϊ0x0f
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//1;						// ���ó�Ա NVIC_IRQChannel�еĴ����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							// ʹ�� NVIC_IRQChannel
	NVIC_Init(&NVIC_InitStructure);    

	#if (KEY_LED_TOGGLE_ENABLE_FLAG == ENABLED)
		//��ɱ����ĳ�ʼ��

		for(u32I = 0x00; u32I < KEY_LED_NUMB_MAX_D;u32I++)
		{
			s_u32KeyLedOnTimeSet[u32I] = 0x00;//��ȡ����ʱ�䣬��λms
			s_u32KeyLedOffTimeSet[u32I] = 0x00;//��ȡϨ��ʱ�䣬��λms
			s_u32KeyLedTimer[u32I] = 0x00;//�������Ϩ���ʱ����λms			
		}
	#endif
	
	TIM_Cmd(TIM5, ENABLE);	
	
	return 0x01;
}

/***************************************************************************************
** ��������:SH_CP_595
** ������������λ  
** �������룺��
** �����������
** ע�����
******************************************************************************************/
static void SH_CP_595(void)
{ 
	GPIO_ResetBits(KEY_LED_SCK_PORT, KEY_LED_SCK_PIN);
	GPIO_ResetBits(KEY_LED_SCK_PORT, KEY_LED_SCK_PIN);
	GPIO_SetBits(KEY_LED_SCK_PORT, KEY_LED_SCK_PIN);	//��������λ    
	GPIO_SetBits(KEY_LED_SCK_PORT, KEY_LED_SCK_PIN);	//��������λ    
}  

/***************************************************************************************
** ��������:ST_CP_595
** �������������浽�������ʾ 
** �������룺��
** �����������
** ע�����
******************************************************************************************/
static void ST_CP_595(void)//  
{ 
	GPIO_ResetBits(KEY_LED_LCK_PORT, KEY_LED_LCK_PIN);
	GPIO_ResetBits(KEY_LED_LCK_PORT, KEY_LED_LCK_PIN);
	GPIO_SetBits(KEY_LED_LCK_PORT, KEY_LED_LCK_PIN);	//�����ؽ������͵����������    
	GPIO_SetBits(KEY_LED_LCK_PORT, KEY_LED_LCK_PIN);	//�����ؽ������͵����������    
}  


static u32 D_typeKeyLedMum[14] =  {0x01,0x02,0x004,0x008,0x10,0x20,0x40,0x80,0x100,0x200,0x400,0x800,0x1000,0x2000};
static u32 D_typeKeyLedMum1[14] = {0x02,0x08,0x100,0x200,0x40,0x80,0x20,0x10,0x400,0x800,0x1000,0x2000,0x01,0x04};
/***************************************************************************************
** ��������:D��ң��������תB��
** ����������
** �������룺��
** �����������
** ע�����
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
	if((u32KeyVal&(u32KeyVal-1)) == 0)  //ֻ��һ����
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
** ��������:KeyLed
** ���������� ������Ϩ�𰴼�LED
** �������룺��
** �����������
** ע�����
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
		s_u32LedKeyStatus |= u32OnKey;	//��������		
	}
	
	if (u32OffKey)
	{
		s_u32LedKeyStatus &= (~u32OffKey);	//Ϩ�𲿷�
	}

	return 0x01;

}



/***************************************************************************************
** ��������:KeyLedOnOff
** ���������� ������Ϩ�𰴼�LED
** �������룺��
** �����������
** ע�����
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
		s_u32LedKeyStatus |= u32OnKey;	//��������	
		#if (KEY_LED_TOGGLE_ENABLE_FLAG == ENABLED)
			u32J = u32OnKey;
			for(u32I = 0x00;u32I < KEY_LED_NUMB_MAX_D;u32I++)
			{
				if((u32J&(1<<u32I)) != 0x00)
				{
					s_u32KeyLedOnTimeSet[u32I] = KEY_LED_TIMER_EXPIRE;//��ȡ����ʱ�䣬��λms
					s_u32KeyLedOffTimeSet[u32I] = KEY_LED_TIMER_CLOSED;//��ȡϨ��ʱ�䣬��λms
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
		s_u32LedKeyStatus &= (~u32OffKey);	//Ϩ�𲿷�
		#if (KEY_LED_TOGGLE_ENABLE_FLAG == ENABLED)
			u32J = u32OffKey;
			for(u32I = 0x00;u32I < KEY_LED_NUMB_MAX_D;u32I++)
			{
				if((u32J&(1<<u32I)) != 0x00)
				{
					s_u32KeyLedOnTimeSet[u32I] = KEY_LED_TIMER_EXPIRE;//��ȡ����ʱ�䣬��λms
					s_u32KeyLedOffTimeSet[u32I] = KEY_LED_TIMER_EXPIRE;//��ȡϨ��ʱ�䣬��λms
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
** ��������:KeyLedToggle
** ��������:��˸LED
** �������룺��
** �����������
** ע�����
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
			s_u32LedKeyStatus |= u32ToggleKey;	//��������	
			u32J = u32ToggleKey;
			for(u32I = 0x00;u32I < KEY_LED_NUMB_MAX_D;u32I++)
			{
				s_u32KeyLedOnTimeSet[u32I] = 0x00;//����˸�ļ������� �������ʱ��Ч��ͬʱ������˸�쳣������	2020.04.27 parry
				s_u32KeyLedOffTimeSet[u32I] = 0x00;
				if((u32J&(1<<u32I)) != 0x00)
				{
					s_u32KeyLedOnTimeSet[u32I] = u32OnTime;//��ȡ����ʱ�䣬��λms
					s_u32KeyLedOffTimeSet[u32I] = u32OffTime;//��ȡϨ��ʱ�䣬��λms	
					s_u32KeyLedTimer[u32I] = 0x00;		
				}
			}
			return 0x01;
		}
		
		return 0x00;
	}
#endif
	
/***************************************************************************************
** ��������:LedShift_595
** ���������� ��λIO���1byte����
** �������룺��
** �����������
** ע�����
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
** ��������:KeyLedScan
** ��������������LEDɨ��������ú������ü��10ms
** �������룺��
** �����������
** ע�����
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
				if(s_u32KeyLedOnTimeSet[u32I] != KEY_LED_TIMER_EXPIRE)//��˸״̬
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
		//����ɨ�裬��4��
		u16Temp = (u16)((u32LedKeyStatusBackup >> (s_u16Turn<<2 )) & 0x00ff);	//ȡ��ֵ		
	#else
		//����ɨ�裬��4��
		u16Temp = (u16)((s_u32LedKeyStatus >> (s_u16Turn<<2 )) & 0x00ff);	//ȡ��ֵ
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
	ST_CP_595();	//����
	if(g_u32ID != B_type)
		KEY595ChipOUTOff();
	//�����´�����ִ�
	s_u16Turn++;
	s_u16Turn &= 0x0003;
}


/***************************************************************************************
** ��������:TIM5_IRQHandler
** ������������ʱ��3����жϺ���
** �������룺��
** ���������1��������0���쳣
** ע�����
******************************************************************************************/
void TIM5_IRQHandler(void)
{	  
	OS_CPU_SR  cpu_sr;

	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
	OSIntEnter();
	OS_EXIT_CRITICAL();

	if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET)							// �ж��Ƿ�����ʱ������ж�
	{
		KeyLedScan();
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);							// ��ʱ������жϴ�����������
		TIM_ClearFlag(TIM5, TIM_FLAG_Update);								// ���Ͼ书����ͬ
	}
	OSIntExit();
}
#endif





