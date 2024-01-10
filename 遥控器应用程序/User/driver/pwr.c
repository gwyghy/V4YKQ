/********************************************************************************
* �ļ�����	pwr.c
* ���ߣ�	������  
* �汾��   	v1.0
* ���ڣ�    2015.4.28
* ��������:  ����STM32Lϵ�е͹��ĵ�Դ����
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

/********************************************************************************
* #define�궨��
*********************************************************************************/

/********************************************************************************
* ��������
*********************************************************************************/
/********************************************************************************
* ��������
*********************************************************************************/

/********************************************************************************
* ��������
*********************************************************************************/
/***************************************************************************************
** ��������:PwrPinInit
** ����������OLED��Դ�����ʼ��
** �������룺��
** ���������1��������0���쳣
** ע�����
******************************************************************************************/
u32 PwrPinInint(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIO Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_PWR_OLED_GPIO|RCC_PWR_WL_GPIO|RCC_LTC3440_MODE_GPIO, ENABLE);

	/* Configure LTC3440_MODE_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = LTC3440_MODE_PIN;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LTC3440_MODE_PORT, &GPIO_InitStructure);
	if(g_u32ID == B_type)
	{
		Ltc3440BurstModeDisabled();
	}
	else
	{
		Ltc3440BurstModeEnabled();
	}
	
	/* Configure LED_STOP_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = PWR_WL_PIN;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(PWR_WL_PORT, &GPIO_InitStructure);

	/* �����߹����Դ */
	WirelessChipPwrOn();

	/* Configure PWR_OLED_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = PWR_OLED_PIN;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(PWR_OLED_PORT, &GPIO_InitStructure);

	/* ��OLED�����Դ */
	OledChipPwrOn();
	
	return 0x01;
	
}

