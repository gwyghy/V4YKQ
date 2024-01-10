/********************************************************************************
* �ļ�����	adc.c
* ���ߣ�	������ 
* �汾��   	v1.0
* ���ڣ�    2015.4.27
* ��������:  ����STM32Lϵ���ڲ�ADת����
* �޸�˵����   
*
*       >>>>  �ڹ����е�λ��  <<<<
*             3-Ӧ�ò�
*             2-Э���
*           �� 1-Ӳ��������
*********************************************************************************
* @copy
* <h2><center>&copy; COPYRIGHT ������������޹�˾ �з����� �����</center></h2>
*********************************************************************************/
/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/
#include "includes.h"

#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_ADC_MNG)//��Ӳ���У�ʹ��AD��ʽ�������
/********************************************************************************
* #define�궨��
*********************************************************************************/
#define ADC_BUF_SIZE  							60
#define	ADC_CHANNEL_MAX							3
#define	ADC_PERCHANNEL_SAMPLE_TIMES	(ADC_BUF_SIZE/(ADC_CHANNEL_MAX-1))

#define	ADC_PERCHANNEL_SAMPLE_TIMES_D	(ADC_BUF_SIZE/(ADC_CHANNEL_MAX))

/********************************************************************************
* ��������
*********************************************************************************/
static uint16_t s_u16ConvertedValue[ADC_BUF_SIZE];


/********************************************************************************
* ��������
*********************************************************************************/

/********************************************************************************
* ��������
*********************************************************************************/
/*********************************************************
** �������ƣ�Input1AdcInit
** ����������ADC��ʼ��
** �������룺��
** �����������
** ע�����
************************************************************/
u32 InputAdcInit(void)
{
	DMA_InitTypeDef DMA_InitStructure;	
	NVIC_InitTypeDef NVIC_InitTypeStructure;	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure; 


	
	RCC_HSICmd(ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	ADC_TempSensorVrefintCmd((FunctionalState)ENABLED);
	RCC_INPUT1_ADC_PortCmd();////�˿�ʱ��

	//PB1 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = PIN_INPUT1_ADC;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//ģ����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIO_INPUT1_ADC, &GPIO_InitStructure);
	
	if(g_u32ID != B_type)
	{
		//PB1 ��Ϊģ��ͨ����������                         
		GPIO_InitStructure.GPIO_Pin = PIN_INPUT1_ADC1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//ģ����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
		GPIO_Init(GPIO_INPUT1_ADC1, &GPIO_InitStructure);	
	}

	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInit(&ADC_CommonInitStructure);   //ADC Ԥ��Ƶ	

	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//�ⲿ������ƽѡ��ʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	if(g_u32ID == B_type)
		ADC_InitStructure.ADC_NbrOfConversion = ADC_CHANNEL_MAX-1;	//˳����й���ת����ADCͨ������Ŀ
	else
		ADC_InitStructure.ADC_NbrOfConversion = ADC_CHANNEL_MAX;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

	ADC_RegularChannelConfig(INPUT_ADC, INPUT1_ADC_CHANNEL, 1, ADC_SampleTime_384Cycles ); //Sample time equal to 96 cycles
	
	if(g_u32ID != B_type)
		ADC_RegularChannelConfig(INPUT_ADC, INPUT1_ADC_CHANNEL1, 3, ADC_SampleTime_384Cycles ); //Sample time equal to 96 cycles
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_Vrefint, 2, ADC_SampleTime_384Cycles ); // Sample time equal to 96 cycles

	NVIC_InitTypeStructure.NVIC_IRQChannel=ADC1_IRQn;
	NVIC_InitTypeStructure.NVIC_IRQChannelPreemptionPriority=0x09;//��ʹ���ж����ȼ�Ƕ�ס���ΪSysTick���ж����ȼ�Ϊ0x0f
	NVIC_InitTypeStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitTypeStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitTypeStructure);

	ADC_ITConfig(INPUT_ADC,ADC_IT_OVR,ENABLE); //OVR�ж�ʹ��

	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1

	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)          // Wait until ADC1 ON status 
	{
	}
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_RCNR) != RESET);          // 0 regular channel ready;1 regular channel not ready,SWSTART must not be set 

	/* Enable DMA1 clock */
	RCC_INPUT_ADC_DMA_Cmd();

	/* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_DeInit(DMA_CHANNEL_INPUT_ADC);
	DMA_InitStructure.DMA_PeripheralBaseAddr = INPUT_ADC_DR_ADDRESS;    //DMA��Ӧ���������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&s_u16ConvertedValue;   //�ڴ�洢����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	//DMA��ת��ģʽΪSRCģʽ����������Ƶ��ڴ�
	DMA_InitStructure.DMA_BufferSize = ADC_BUF_SIZE;		   //DMA�����С��ADC_BUF_SIZE��,��λΪDMA_MemoryDataSize
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//����һ�����ݺ��豸��ַ��ֹ����
	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;//��������һ�����ݺ�Ŀ���ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //�����������ݿ��Ϊ16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //DMA�����ݳߴ磬HalfWord����Ϊ16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   //ת��ģʽ��ѭ������ģʽ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//DMA���ȼ���
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		  //M2Mģʽ����
	DMA_Init(DMA_CHANNEL_INPUT_ADC, &DMA_InitStructure);    

	/* Enable ADC1 DMA */
	ADC_DMACmd(INPUT_ADC, ENABLE);	  //ADC���ʹ��

	/* Disable ADC1 DMA REQUEST*/
	ADC_DMARequestAfterLastTransferCmd(INPUT_ADC, ENABLE);//ADC��������

	ADC_SoftwareStartConv(ADC1);//Enables the selected ADC software start conversion of the regular channels.

	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA_CHANNEL_INPUT_ADC, ENABLE);

	return 0x01;

}

/*********************************************************
** �������ƣ�AdcInit
** ����������ADC��ʼ��
** �������룺��
** �����������
** ע�����
************************************************************/
u32 AdcInit(void)
{
	u16 u16I = 0x00;

	for (u16I=0;u16I<ADC_BUF_SIZE;u16I++)
	{
		s_u16ConvertedValue[u16I] = 0x00;
	}	
	
	InputAdcInit();

	return 0x01;
}

/*********************************************************
** �������ƣ�GetBatteryAdcAverageValue
** ������������ȡAD������ƽ��ֵ
** �������룺u16Data������ָ��
** ���������1:�����ɹ�
** ע�����
************************************************************/
u32 GetAdcAverageValue(ADINPUT_CHANNEL_TYPE channel,u16 *u16Data)
{
	u32 u32sum = 0x00;
	u16 u16I = 0x00;
	u32 u32returnvalue = 0x00;
	
	if(g_u32ID == B_type)
	{
		switch(channel)
		{
			case ADINPUT_1:
				for (u16I=0;u16I<ADC_BUF_SIZE;u16I++,u16I++)
				{
					u32sum += s_u16ConvertedValue[u16I];
				}
				u32returnvalue = 0x01;
				break;

			case ADINPUT_2:
				for (u16I=1;u16I<ADC_BUF_SIZE;u16I++,u16I++)
				{
					u32sum += s_u16ConvertedValue[u16I];
				}
				u32returnvalue = 0x01;
				break;
				
			default:
				u32returnvalue = 0x00;
				break;
		}
		*u16Data = (u16)(u32sum/ADC_PERCHANNEL_SAMPLE_TIMES);
	}
	else if(g_u32ID != B_type)
	{
		switch(channel)
		{
			case ADINPUT_1:
				for (u16I=0;u16I<ADC_BUF_SIZE;u16I++,u16I++,u16I++)
				{
					u32sum += s_u16ConvertedValue[u16I];
				}
				u32returnvalue = 0x01;
				break;

			case ADINPUT_2:
				for (u16I=1;u16I<ADC_BUF_SIZE;u16I++,u16I++,u16I++)
				{
					u32sum += s_u16ConvertedValue[u16I];
				}
				u32returnvalue = 0x01;
				break;
				
			case ADINPUT_3:
				for (u16I=2;u16I<ADC_BUF_SIZE;u16I++,u16I++,u16I++)
				{
					u32sum += s_u16ConvertedValue[u16I];
				}
				u32returnvalue = 0x01;
				break;
				
			default:
				u32returnvalue = 0x00;
				break;
		}		
	*u16Data = (u16)(u32sum/ADC_PERCHANNEL_SAMPLE_TIMES_D);
	}	
	return u32returnvalue;
}

/*********************************************************
** �������ƣ�ADC_IRQHandler
** ����������ADC�жϴ���
** �������룺��
** �����������
** ע�����
************************************************************/
void ADC1_IRQHandler(void)
{
	OSIntEnter();

	if(ADC_GetFlagStatus(INPUT_ADC,ADC_FLAG_OVR))
	{
		ADC_ClearFlag(INPUT_ADC,ADC_FLAG_OVR);
		/* Enable DMA1 channel1 */
		DMA_Cmd(DMA_CHANNEL_INPUT_ADC, DISABLE);
		InputAdcInit();
	}
	
	OSIntExit();
}
#endif
