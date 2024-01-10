/********************************************************************************
* �ļ�����	bsp.c
* ���ߣ�	������   
* �汾��   	V1.0
* ���ڣ�    2015.04.28
* ��������:  ʵ��BSP��ʱ�ӵĳ�ʼ��

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
#include <includes.h>
u32 g_u32ID;
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
/*********************************************************
** �������ƣ�BSP_Init
** ����������BSP��ʼ��
** �������룺��
** �����������
** ע�����
************************************************************/
void  BSP_Init (void)
{
u32 i;
	GPIO_InitTypeDef GPIO_InitStructure;
	SystemInit();

	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);				//û�д����ȼ�
	
	// ���ýӿ�
	/* GPIOC Periph clock enable */
	RCC_AHBPeriphClockCmd(ID_GPIO_RCC, ENABLE);			   //ʹ������ʹ�ö˿�ʱ��

	/* Configure ID0_PIN in input nopull mode */
	GPIO_InitStructure.GPIO_Pin = ID0_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(ID_PORT, &GPIO_InitStructure);
	
	
	/* Configure ID1_PIN in input nopull mode */
	GPIO_InitStructure.GPIO_Pin = ID1_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(ID_PORT, &GPIO_InitStructure);


	i = GPIO_ReadInputData(ID_PORT);
	i = GPIO_ReadInputData(ID_PORT);
	i = GPIO_ReadInputData(ID_PORT);
	i &= (ID0_PIN|ID1_PIN);
	g_u32ID = i;
}

/*********************************************************
** �������ƣ�SysTick_Init
** ����������ʱ�ӳ�ʼ��
** �������룺��
** �����������
** ע�����
************************************************************/
void SysTick_Init(void) 
{ 
    RCC_ClocksTypeDef  rcc_clocks; 

    RCC_GetClocksFreq(&rcc_clocks);   //���ñ�׼�⺯������ȡϵͳʱ�ӡ�
    SysTick_Config(rcc_clocks.HCLK_Frequency / OS_TICKS_PER_SEC); //��ʼ����ʹ��SysTick��OS_TICKS_PER_SECָ��ÿ���������
}
