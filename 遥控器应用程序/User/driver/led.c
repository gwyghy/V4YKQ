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

/********************************************************************************
* ��������
*********************************************************************************/

/********************************************************************************
* ��������
*********************************************************************************/
static u32	s_u32LedState = 0x00;	//LEDָʾ�Ƶ�״̬

/********************************************************************************
* ��������
*********************************************************************************/

/***************************************************************************************
** ��������:Led_Init
** ����������LED�Ƴ�ʼ��
** �������룺��
** ���������1��������0���쳣
** ע�����
******************************************************************************************/
u32 LedInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIO Periph clock enable */
	RCC_AHBPeriphClockCmd(LED_1_GPIO_RCC|LED_2_GPIO_RCC|LED_3_GPIO_RCC, ENABLE);

	/* Configure LED_LOCK_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = LED_1_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_1_PORT, &GPIO_InitStructure);

	/* Set LED_LOCK_PIN */
	GPIO_SetBits(LED_1_PORT, LED_1_PIN);

	/* Configure LED_STOP_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = LED_2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_2_PORT, &GPIO_InitStructure);

	/* Set LED_STOP_PIN */
	GPIO_SetBits(LED_2_PORT, LED_2_PIN);

	/* Configure LED_STOP_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = LED_3_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_3_PORT, &GPIO_InitStructure);

	/* Set LED_STOP_PIN */
	GPIO_ResetBits(LED_3_PORT, LED_3_PIN);
	
	s_u32LedState = 0x00;
	
	return 0x01;
}

/***************************************************************************************
** ��������:StateLed
** ����������LED�ƵĴ���
** �������룺u16On:��Ҫ������LED��,u16Off:��ҪϨ���LED��
** ���������1��������0���쳣
** ע�����
******************************************************************************************/
u32 StateLed(u16 u16On, u16 u16Off)
{
	if (u16On)
	{
		s_u32LedState |= u16On;
		if (u16On & LED_1)
			GPIO_ResetBits(LED_1_PORT, LED_1_PIN);
		if (u16On & LED_2)
			GPIO_ResetBits(LED_2_PORT, LED_2_PIN);
		if (u16On & LED_3)
			GPIO_SetBits(LED_3_PORT, LED_3_PIN);		
	}
	
	if (u16Off)
	{
		s_u32LedState &= (~u16Off);
		if (u16Off & LED_1)
			GPIO_SetBits(LED_1_PORT, LED_1_PIN);
		if (u16Off & LED_2)
			GPIO_SetBits(LED_2_PORT, LED_2_PIN);
		if (u16Off & LED_3)
			GPIO_ResetBits(LED_3_PORT, LED_3_PIN);		
	}
	
	return 0x01;
}

/***************************************************************************************
** ��������:LedToggle
** ����������LED�ƽ�����˸�Ĵ���
** �������룺u16Toggle:��Ҫ��˸��LED��
** ���������1��������0���쳣
** ע�����
******************************************************************************************/
u32 LedToggle(u16 u16Toggle)
{
	if (u16Toggle)
	{
		if (u16Toggle & LED_1)
		{
			if((s_u32LedState&LED_1) == 0x00)
			{	
				GPIO_ResetBits(LED_1_PORT, LED_1_PIN);
				s_u32LedState |= u16Toggle;
			}
			else
			{	
				GPIO_SetBits(LED_1_PORT, LED_1_PIN);			
				s_u32LedState &= (~u16Toggle);
			}
		}
		if (u16Toggle & LED_2)
		{	
			if((s_u32LedState&LED_2) == 0x00)
			{	
				GPIO_ResetBits(LED_2_PORT, LED_2_PIN);
				s_u32LedState |= u16Toggle;
			}
			else
			{	
				GPIO_SetBits(LED_2_PORT, LED_2_PIN);			
				s_u32LedState &= (~u16Toggle);
			}
		}
		if (u16Toggle & LED_3)
		{	
			if((s_u32LedState&LED_3) == 0x00)
			{	
				GPIO_SetBits(LED_3_PORT, LED_3_PIN);;
				s_u32LedState |= u16Toggle;
			}
			else
			{	
				GPIO_ResetBits(LED_3_PORT, LED_3_PIN);				
				s_u32LedState &= (~u16Toggle);
			}		
		}
		return 0x01;	
	}
	return 0x00;
}
