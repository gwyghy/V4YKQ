/********************************************************************************
* �ļ�����	main.c
* ���ߣ�	������   
* �汾��   	v1.0
* ���ڣ�    2015.04.26
* ��������:  ����ĿΪ����ң������Ӧ�ó���

* �޸�˵����   
*
*       >>>>  �ڹ����е�λ��  <<<<
*          �� 3-Ӧ�ò�
*             2-Э���
*             1-Ӳ��������
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
OS_STK  Appmng_task_stk[APPMNG_TASK_STK_SIZE];				//���������ջ
OS_STK  Interface_task_stk[INTERFACE_TASK_STK_SIZE];		//���������ջ
OS_STK  ScanKey_task_stk[KEYSCAN_TASK_STK_SIZE];			//���������ջ
OS_STK  BatteryMng_task_stk[BATTERYMNG_TASK_STK_SIZE];		//���������ջ
#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
	OS_STK  BuzzerMng_task_stk[BUZZERMNG_TASK_STK_SIZE];	//���������ջ
#endif
#ifdef	IWDG_ENABLED
	OS_STK  Iwdg_task_stk[IWDG_TASK_STK_SIZE];				//���������ջ
#endif

#ifdef HARDWARE_TEST
	OS_STK  hardware_task_stk[HARAWARE_TEST_STK_SIZE];				//���������ջ
#endif

OS_STK  Logic_task_stk[LOGIC_TASK_STK_SIZE];				//���������ջ
OS_STK  IrMng_task_stk[IRMNG_TASK_STK_SIZE];				//���������ջ
OS_STK  RfidMng_task_stk[RFIDMNG_TASK_STK_SIZE];			//���������ջ
OS_STK  Usart_task_stk[USARTHANDLE_TASK_STK_SIZE];			//���������ջ

/********************************************************************************
* ��������
*********************************************************************************/

/*******************************************************************************************
**�������ƣ�APPMng_Task
**�������ã�Ӧ�ò�������
**������������
**�����������
**ע�������
*******************************************************************************************/
static void APPMng_Task(void *p_arg) 
{
	/* Initialize the SysTick. */
	SysTick_Init();     
	/**�߼���س�ʼ��**/
	LogicInint();
	/**��ʾ��س�ʼ��***/
	Interface_Init();
		
	/**********�����߼��������***************************************/
	/**�߼���������**/
	OSTaskCreate(Logic_task, (void *)0, &Logic_task_stk[LOGIC_TASK_STK_SIZE - 1], LOGIC_TASK_PRIO); 
	
	

	/**���⴦������**/
	OSTaskCreate(IrMng_Task, (void *)0, &IrMng_task_stk[IRMNG_TASK_STK_SIZE - 1], IRMNG_TASK_PRIO); 
	/**���ߴ�������**/
	if(g_u32ID != B_type)
	{
		OSTaskCreate(Usart_Task, (void *)0, &Usart_task_stk[USARTHANDLE_TASK_STK_SIZE - 1], USART_TASK_PRIO); 
	}
	
	OSTaskCreate(RfidMng_Task, (void *)0, &RfidMng_task_stk[RFIDMNG_TASK_STK_SIZE - 1], RFIDMNG_TASK_PRIO); 
	
	/********* �����˻��������������*********************************/
	/**��ʾ��������**/
	#ifdef HARDWARE_TEST
		OSTaskCreate(hardwaretest_task, (void *)0, &hardware_task_stk[HARAWARE_TEST_STK_SIZE - 1], HARAWARE_TEST_TASK_PRIO); 
	#else			
		OSTaskCreate(Interface_task, (void *)0, &Interface_task_stk[INTERFACE_TASK_STK_SIZE - 1], INTERFACE_TASK_PRIO); 
	#endif
	/*��������ɨ������*/
	OSTaskCreate(ScanKey_task, 0, &ScanKey_task_stk[KEYSCAN_TASK_STK_SIZE - 1], SCAN_KEY_TASK_PRIO); 
	/*������ع�������*/
	OSTaskCreate(BatteryMngTask, 0, &BatteryMng_task_stk[BATTERYMNG_TASK_STK_SIZE - 1], BATTERYMNG_TASK_PRIO); 
	/*������������������*/
	#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
		OSTaskCreate(BuzzerMng_Task, 0, &BuzzerMng_task_stk[BUZZERMNG_TASK_STK_SIZE - 1], BUZZERMNG_TASK_PRIO); 
	#endif
	
	#ifndef APPMNG_TEST
		OSTaskSuspend(APPMNG_TASK_PRIO);
	#else
		/***��������Ķ�ջУ��****/
		while(1)
		{
			static uint32_t i = 0;
			
			OSTimeDly(1000/TICK_TIME);
			
			if (i++ > 59)
			{				
				OS_TCB      ptcb;
				OS_STK_DATA stk_data;
				uint32_t    prio;
				uint8_t     err;
				
				for (prio = 1; prio < OS_LOWEST_PRIO; prio++)
				{
					err = OSTaskQuery((INT8U)prio,&ptcb);
					if (err == OS_ERR_NONE)
					{
						err = OSTaskStkChk(prio,&stk_data);
						#if 0
							RUN_TRACE("   %2d  %5d  %5d    %02d%%   %s\r\n", 
								ptcb.OSTCBPrio, 
								stk_data.OSUsed, 
								stk_data.OSFree, 
								(stk_data.OSUsed * 100) / (stk_data.OSUsed + stk_data.OSFree),						
								ptcb.OSTCBTaskName);
						#endif
					}				
				}	
				i = 0;
			}	
		}
	#endif
}

/*******************************************************************************************
**�������ƣ�main
**�������ã�������
**������������
**�����������
**ע�������
*******************************************************************************************/
int main(void)
{
	BSP_Init();// ϵͳ��ʼ��
	OSInit();// uCOS-II��ʼ��
	Driver_Init();		//

	// ����Ӧ�ò�������
    OSTaskCreateExt(APPMng_Task, 
					(void *)0, 
					&Appmng_task_stk[APPMNG_TASK_STK_SIZE - 1], 
					APPMNG_TASK_PRIO,
					APPMNG_TASK_PRIO,
					&Appmng_task_stk[0],
					APPMNG_TASK_STK_SIZE,
					(void* )0,
					(uint16_t)(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
	
    OSStart(); 
}

/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/
