/********************************************************************************* 文件名称：	iwdg.c* 作	者：	马如意   * 当前版本：   	V1.0* 完成日期：    2015.05.05* 功能描述: 	完成看门狗初始化及喂狗等操作处理。* 历史信息：   *           	版本信息     完成时间      原作者        注释**       >>>>  在工程中的位置  <<<<*          	  3-应用层*          √  2-协议层*             1-硬件驱动层********************************************************************************** Copyright (c) 2014,天津华宁电子有限公司 All rights reserved.*********************************************************************************//********************************************************************************* .h头文件*********************************************************************************/#include "includes.h"/********************************************************************************* #define宏定义*********************************************************************************//********************************************************************************* 常量定义*********************************************************************************/#define	IWDG_FEED_TIME_SET		250//喂狗时间间隔,单位ms#define IWDG_TASK_INTERVAL		IWDG_FEED_TIME_SET/TICK_TIME//任务延时，单位时钟节拍/********************************************************************************* 变量定义*********************************************************************************//********************************************************************************* 函数定义*********************************************************************************/extern u16 Iwdg_ScanKey;extern u16 Iwdg_Interface;extern u16 Iwdg_Logic;/******************************************************************************************** 函数名称： IWDG_Configuration()* 功能描述： 看门狗初始化     * 入口参数：无* 出口参数：无* 使用说明：初始化调用*******************************************************************************************/#ifdef	IWDG_ENABLED	void IWDG_Configuration(void)	{		/* 使能寄存器 IWDG_PR 和 IWDG_RLR 写访问 */		IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);		/* 看门狗分频时钟为: LSI/256 *//* 内部低速时钟256分频 40K/256=156HZ(6.4ms) */		IWDG_SetPrescaler(IWDG_Prescaler_256);		/* 设置看门狗的重载值.5S*//* 喂狗时间 5s/6.4MS=781 .注意不能大于0xfff*/		IWDG_SetReload(781);		/* 喂狗 */		IWDG_ReloadCounter();		/* 使能看门狗 */		IWDG_Enable();	}#endif/******************************************************************************************** 函数名称： IWDG_SleepConfiguration()* 功能描述： 看门狗初始化     * 入口参数：无* 出口参数：无* 使用说明：初始化调用*******************************************************************************************/#ifdef	IWDG_ENABLED	void IWDG_SleepConfiguration(void)	{		/* 使能寄存器 IWDG_PR 和 IWDG_RLR 写访问 */		IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);		/* 设置看门狗的重载值.5S*//* 喂狗时间 0.5s/6.4MS=78注意不能大于0xfff*/		IWDG_SetReload(78);		/* 喂狗 */		IWDG_ReloadCounter();	}#endif	/******************************************************************************************** 函数名称： IWDG_Feed()* 功能描述： 完成喂狗处理。    * 入口参数：无* 出口参数：无* 使用说明：无*******************************************************************************************/#ifdef	IWDG_ENABLED	void IWDG_Feed(void)	{		IWDG_ReloadCounter();	}#endif/*********************************************************************************************函数名称：IWDG_task**函数作用：看门狗主任务，实现喂狗等操作。**函数参数：无**函数输出：无**注意事项：无*******************************************************************************************/	#ifdef	IWDG_ENABLED		void IWDG_Task(void *p_arg) 	{		#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */			OS_CPU_SR  cpu_sr = 0;		#endif		IWDG_Configuration();		//任务运行循环		while(1)		{			if(Iwdg_ScanKey>0x100||Iwdg_Interface>0x100||Iwdg_Logic>0x100)			{//					NVIC_SystemReset();			}			else			{					OS_ENTER_CRITICAL();				Iwdg_ScanKey++;				Iwdg_Interface++;				Iwdg_Logic++;				OS_EXIT_CRITICAL();			}			IWDG_Feed();				//喂狗			OSTimeDly(IWDG_TASK_INTERVAL);	//任务间隔		}	}#endif			//#ifdef IWDG_ENABLED		