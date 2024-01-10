/********************************************************************************
* �ļ�����	ir_send.c
* ���ߣ�	������   
* �汾��   	V1.0
* ���ڣ�    2015.05.05
* ��������:  ʵ�ֺ������ݷ��͵ĵײ�����

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
#include "includes.h"

/********************************************************************************
* #define�궨��
*********************************************************************************/

/********************************************************************************
* ��������
*********************************************************************************/

#define CCR_VALUE_SET				131//��ʱ����������
#define	IR_SEND_ARRAY_MAX			69//��ʱ����������

#define	IR_SEND_PRECODE_FIRST_SET	342
#define	IR_SEND_PRECODE_SECND_SET	171
#define	IR_SEND_DATA1_FIRST_SET		21
#define	IR_SEND_DATA1_SECND_SET		64
#define	IR_SEND_DATA0_FIRST_SET		21
#define	IR_SEND_DATA0_SECND_SET		21

#define	IR_SEND_TIME_MAX			30000//����IR_SEND_ARRAY_MAX���ݷ��͵����ʱ�䣬���ڵȴ���������ж�
/********************************************************************************
* ��������
*********************************************************************************/
u16 s_u16TimeArray[IR_SEND_ARRAY_MAX] ; 
u32	s_u32SendPointer = 0x00;	//TIM1�жϷ�������Ҫ��ָ�룬����������Ҫ��ʼ��Ϊ0
u16  s_u16SendCount = 0x00;		//�����������
OS_EVENT	 *g_stIrSendSem;//���ⷢ���¼��ź���
/********************************************************************************
* ��������
*********************************************************************************/

/**********************************************************************************************************
*	�� �� ��: IrSendTIMConfig
*	����˵��: ��ʹ�õĶ�ʱ����ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
**********************************************************************************************************/
void IrSendTIMConfig(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		            //ʱ������趨
	TIM_OCInitTypeDef        TIM_OCInitStructure;				    //TIMx�����ʼ��
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
*	�� �� ��: IR_SEND_CC_IRQHandler
*	����˵��: ��ʱ���жϴ���
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: IR_SEND_CC_IRQHandler
*	����˵��: ��ʱ���жϴ���
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: IrSendInsertTxBuf
*	����˵��: ���뷢�Ͷ���
*	��    ��: ��
*	�� �� ֵ: 1:�������ͣ�0:���ݷ����쳣
**********************************************************************************************************/
u32 IrSendInsertTxBuf(u16 u16customcode,u8 u8customdata)
{
	u16 u16code,u16data1;
	u8  u8I,u8J,u8data;
	INT8U	err;
	
	s_u32SendPointer = 0;							            //TIM1�жϷ�������Ҫ��ָ�룬����������Ҫ��ʼ��Ϊ0
	s_u16SendCount = 0;								            //�����������
	u16code = u16customcode;
	u8data = u8customdata;										//customdata���䷴�����16λ����
	u16data1=(u16)((~u8data)<<8) + (u16)u8data;
	s_u16TimeArray[0]=IR_SEND_PRECODE_FIRST_SET;										//����ǰ���룬���ز�
	s_u16TimeArray[1]=IR_SEND_PRECODE_SECND_SET;										//���ز�
	for(u8I=0,u8J=0;u8I<32;u8I+=2,u8J++)								//�����û�����
	{
		if((u16code>>u8J)&1)
		{
		                                                    // ��� 1 
			s_u16TimeArray[2+u8I]=IR_SEND_DATA1_FIRST_SET;
			s_u16TimeArray[3+u8I]=IR_SEND_DATA1_SECND_SET;
		}
		else
		{
			s_u16TimeArray[2+u8I]=IR_SEND_DATA0_FIRST_SET;								// ��� 0
			s_u16TimeArray[3+u8I]=IR_SEND_DATA0_SECND_SET;
		}
	}
	
	for(u8I=0,u8J=0;u8I<32;u8I+=2,u8J++)								//��������
	{
		if((u16data1>>u8J)&1)
		{
			s_u16TimeArray[34+u8I]=IR_SEND_DATA1_FIRST_SET;								//���1
			s_u16TimeArray[35+u8I]=IR_SEND_DATA1_SECND_SET;								
		}
		else
		{
			s_u16TimeArray[34+u8I]=IR_SEND_DATA0_FIRST_SET;								//���0
			s_u16TimeArray[35+u8I]=IR_SEND_DATA0_SECND_SET;
		}
	}
	/**����ʱ���룬��ʽ������**/
	s_u16TimeArray[66]=342;										//���ͽ�����
	s_u16TimeArray[67]=85;
	s_u16TimeArray[68]=21;
	
	#if 1
	if(g_u32ID == B_type)
		TIM_CCxCmd(IR_SEND_TIM,IR_SEND_TIM_CHANNEL, TIM_CCx_Enable);
	else
		TIM_CCxCmd(TIM3,TIM_Channel_2, TIM_CCx_Enable);
	#endif
	
	g_stIrSendSem->OSEventCnt = 0x00;//���������
	
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
	
	/**�ȴ����ݷ������**/
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
*	�� �� ��: IrSendInsertTxBuf
*	����˵��: ���뷢�Ͷ���
*	��    ��: ��
*	�� �� ֵ: 1:�������ͣ�0:���ݷ����쳣
**********************************************************************************************************/
u32 IrLOCSendInsertTxBuf(u16 u16customcode,u8 u8customdata)
{
	u16 u16code,u16data1;
	u8  u8I,u8J,u8data;
	INT8U	err;
	
	s_u32SendPointer = 0;							            //TIM1�жϷ�������Ҫ��ָ�룬����������Ҫ��ʼ��Ϊ0
	s_u16SendCount = 0;								            //�����������
	u16code = u16customcode;
	u8data = ~((u8)(u16customcode>>8) + (u8)(u16customcode&0xff));									//customdata���䷴�����16λ����
	u16data1=(u16)((~u8data)<<8) + (u16)u8data;
	s_u16TimeArray[0]=IR_SEND_PRECODE_FIRST_SET;										//����ǰ���룬���ز�
	s_u16TimeArray[1]=IR_SEND_PRECODE_SECND_SET;										//���ز�
	for(u8I=0,u8J=0;u8I<32;u8I+=2,u8J++)								//�����û�����
	{
		if((u16code>>u8J)&1)
		{
		                                                    // ��� 1 
			s_u16TimeArray[2+u8I]=IR_SEND_DATA1_FIRST_SET;
			s_u16TimeArray[3+u8I]=IR_SEND_DATA1_SECND_SET;
		}
		else
		{
			s_u16TimeArray[2+u8I]=IR_SEND_DATA0_FIRST_SET;								// ��� 0
			s_u16TimeArray[3+u8I]=IR_SEND_DATA0_SECND_SET;
		}
	}
	
	for(u8I=0,u8J=0;u8I<32;u8I+=2,u8J++)								//��������
	{
		if((u16data1>>u8J)&1)
		{
			s_u16TimeArray[34+u8I]=IR_SEND_DATA1_FIRST_SET;								//���1
			s_u16TimeArray[35+u8I]=IR_SEND_DATA1_SECND_SET;								
		}
		else
		{
			s_u16TimeArray[34+u8I]=IR_SEND_DATA0_FIRST_SET;								//���0
			s_u16TimeArray[35+u8I]=IR_SEND_DATA0_SECND_SET;
		}
	}
	/**����ʱ���룬��ʽ������**/
	s_u16TimeArray[66]=342;										//���ͽ�����
	s_u16TimeArray[67]=85;
	s_u16TimeArray[68]=21;
	
	#if 1
	if(g_u32ID == B_type)
		TIM_CCxCmd(IR_SEND_TIM,IR_SEND_TIM_CHANNEL, TIM_CCx_Enable);
	else
		TIM_CCxCmd(TIM3,TIM_Channel_2, TIM_CCx_Enable);
	#endif
	
	g_stIrSendSem->OSEventCnt = 0x00;//���������
	
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
	
	/**�ȴ����ݷ������**/
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

