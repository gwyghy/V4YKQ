/********************************************************************************
* 文件名：	main.c
* 作者：	马如意   
* 版本：   	v1.0
* 日期：    2015.04.26
* 功能描述:  本项目为无线遥控器的应用程序。

* 修改说明：   
*
*       >>>>  在工程中的位置  <<<<
*          √ 3-应用层
*             2-协议层
*             1-硬件驱动层
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

/********************************************************************************
* 变量定义
*********************************************************************************/
OS_STK  Appmng_task_stk[APPMNG_TASK_STK_SIZE];				//开辟任务堆栈
OS_STK  Interface_task_stk[INTERFACE_TASK_STK_SIZE];		//开辟任务堆栈
OS_STK  ScanKey_task_stk[KEYSCAN_TASK_STK_SIZE];			//开辟任务堆栈
OS_STK  BatteryMng_task_stk[BATTERYMNG_TASK_STK_SIZE];		//开辟任务堆栈
#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
	OS_STK  BuzzerMng_task_stk[BUZZERMNG_TASK_STK_SIZE];	//开辟任务堆栈
#endif
#ifdef	IWDG_ENABLED
	OS_STK  Iwdg_task_stk[IWDG_TASK_STK_SIZE];				//开辟任务堆栈
#endif

#ifdef HARDWARE_TEST
	OS_STK  hardware_task_stk[HARAWARE_TEST_STK_SIZE];				//开辟任务堆栈
#endif

OS_STK  Logic_task_stk[LOGIC_TASK_STK_SIZE];				//开辟任务堆栈
OS_STK  IrMng_task_stk[IRMNG_TASK_STK_SIZE];				//开辟任务堆栈
OS_STK  RfidMng_task_stk[RFIDMNG_TASK_STK_SIZE];			//开辟任务堆栈
OS_STK  Usart_task_stk[USARTHANDLE_TASK_STK_SIZE];			//开辟任务堆栈

/********************************************************************************
* 函数定义
*********************************************************************************/

/*******************************************************************************************
**函数名称：APPMng_Task
**函数作用：应用层主任务
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
static void APPMng_Task(void *p_arg) 
{
	/* Initialize the SysTick. */
	SysTick_Init();     
	/**逻辑相关初始化**/
	LogicInint();
	/**显示相关初始化***/
	Interface_Init();
		
	/**********创建逻辑层各任务***************************************/
	/**逻辑处理任务**/
	OSTaskCreate(Logic_task, (void *)0, &Logic_task_stk[LOGIC_TASK_STK_SIZE - 1], LOGIC_TASK_PRIO); 
	
	

	/**红外处理任务**/
	OSTaskCreate(IrMng_Task, (void *)0, &IrMng_task_stk[IRMNG_TASK_STK_SIZE - 1], IRMNG_TASK_PRIO); 
	/**无线处理任务**/
	if(g_u32ID != B_type)
	{
		OSTaskCreate(Usart_Task, (void *)0, &Usart_task_stk[USARTHANDLE_TASK_STK_SIZE - 1], USART_TASK_PRIO); 
	}
	
	OSTaskCreate(RfidMng_Task, (void *)0, &RfidMng_task_stk[RFIDMNG_TASK_STK_SIZE - 1], RFIDMNG_TASK_PRIO); 
	
	/********* 创建人机交互方面各任务*********************************/
	/**显示处理任务**/
	#ifdef HARDWARE_TEST
		OSTaskCreate(hardwaretest_task, (void *)0, &hardware_task_stk[HARAWARE_TEST_STK_SIZE - 1], HARAWARE_TEST_TASK_PRIO); 
	#else			
		OSTaskCreate(Interface_task, (void *)0, &Interface_task_stk[INTERFACE_TASK_STK_SIZE - 1], INTERFACE_TASK_PRIO); 
	#endif
	/*创建键盘扫描任务*/
	OSTaskCreate(ScanKey_task, 0, &ScanKey_task_stk[KEYSCAN_TASK_STK_SIZE - 1], SCAN_KEY_TASK_PRIO); 
	/*创建电池管理任务*/
	OSTaskCreate(BatteryMngTask, 0, &BatteryMng_task_stk[BATTERYMNG_TASK_STK_SIZE - 1], BATTERYMNG_TASK_PRIO); 
	/*创建蜂鸣器管理任务*/
	#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
		OSTaskCreate(BuzzerMng_Task, 0, &BuzzerMng_task_stk[BUZZERMNG_TASK_STK_SIZE - 1], BUZZERMNG_TASK_PRIO); 
	#endif
	
	#ifndef APPMNG_TEST
		OSTaskSuspend(APPMNG_TASK_PRIO);
	#else
		/***进行任务的堆栈校验****/
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
**函数名称：main
**函数作用：主函数
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
int main(void)
{
	BSP_Init();// 系统初始化
	OSInit();// uCOS-II初始化
	Driver_Init();		//

	// 创建应用层主任务
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
