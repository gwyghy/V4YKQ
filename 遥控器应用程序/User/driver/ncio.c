/********************************************************************************
* �ļ�����	ncio.c
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


/********************************************************************************
* ��������
*********************************************************************************/

/***************************************************************************************
** ��������:NcIoInit
** ����������δʹ�ö˿ڵƳ�ʼ��
** �������룺��
** ���������1��������0���쳣
** ע�����
******************************************************************************************/
u32 NcIoInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	if(g_u32ID == B_type)
	{
		/* GPIO Periph clock enable */
		RCC_AHBPeriphClockCmd(RCC_NCIO_1_GPIO|RCC_NCIO_2_GPIO, ENABLE);
		
		
		/* Configure LED_LOCK_PIN in output pushpull mode */
		GPIO_InitStructure.GPIO_Pin = NCIO_1_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(NCIO_1_PORT, &GPIO_InitStructure);

	/* Set LED_LOCK_PIN */
	GPIO_ResetBits(NCIO_1_PORT, NCIO_1_PIN);

	/* Configure LED_LOCK_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = NCIO_2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(NCIO_2_PORT, &GPIO_InitStructure);

		/* Set LED_LOCK_PIN */
		GPIO_ResetBits(NCIO_2_PORT, NCIO_2_PIN);	
	}
	
	return 0x01;
}
