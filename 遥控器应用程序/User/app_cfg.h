/********************************************************************************
* 文件名称：	app_cfg.h
* 作	者：		马如意   
* 当前版本：  V1.0
* 完成日期：  2015.05.11
* 功能描述: 	定义app_cfg.h头文件
* 历史信息：   
*           	版本信息     完成时间      原作者        注释
*
*       >>>>  在工程中的位置  <<<<
*         √   3-应用层
*          	  2-协议层
*          		1-硬件驱动层
*********************************************************************************
* Copyright (c) 2014,天津华宁电子有限公司 All rights reserved.
*********************************************************************************/
/********************************************************************************
* .h头文件
*********************************************************************************/
#ifndef __APP_CFG_H__
#define __APP_CFG_H__

/*******************task priority 任务优先级定义*********************************/ 
#define MUTEX_BEEP_PRIO						5	//发声信号互斥量
#define MUTEX_EEPROM_PRIO					6	//EEPROM参数保存信号互斥量

#define SCAN_KEY_TASK_PRIO					7	//键盘扫描任务优先级
#define RFIDMNG_TASK_PRIO					8 	//RFID数据处理任务优先级
#define USART_TASK_PRIO					12 	//USART数据处理任务优先级

#define IRMNG_TASK_PRIO						9//红外数据处理任务优先级
#define LOGIC_TASK_PRIO						10	//逻辑层任务优先级
#define INTERFACE_TASK_PRIO					11	//人机界面显示与按键处理任务优先级

#ifdef HARDWARE_TEST
	#define	HARAWARE_TEST_TASK_PRIO			14//逻辑层任务堆栈大小
#endif

#define BATTERYMNG_TASK_PRIO				13   //电池管理任务
#define BUZZERMNG_TASK_PRIO					16//蜂鸣器管理任务

#define RFIDMNG_UPDATE_TASK_PRIO			15//无线升级任务

#define APPMNG_TASK_PRIO					28	//应用层主任务优先级


#define IWDG_TASK_PRIO						29	//独立看门狗任务	//注意：这是定义的最低优先级，如果继续降级，需要改变OS_LOWEST_PRIO
	
//OS_LOWEST_PRIO：用于OS空闲任务；OS_LOWEST_PRIO-1：用于OS统计任务。

/*******************任务堆栈大小定义***************************************************/
#define APPMNG_TASK_STK_SIZE				TASK_STK_SIZE_256	//应用层主任务堆栈大小

#define INTERFACE_TASK_STK_SIZE				TASK_STK_SIZE_512	//人机界面任务堆栈大小
#define KEYSCAN_TASK_STK_SIZE				TASK_STK_SIZE_128	//按键扫描任务堆栈大小
#define BATTERYMNG_TASK_STK_SIZE			TASK_STK_SIZE_128	//电池管理任务堆栈大小
#define IWDG_TASK_STK_SIZE					TASK_STK_SIZE_64	//看门狗管理任务堆栈大小


#define BUZZERMNG_TASK_STK_SIZE				TASK_STK_SIZE_128	//电池管理任务堆栈大小

#define LOGIC_TASK_STK_SIZE					TASK_STK_SIZE_256//逻辑层任务堆栈大小
#define IRMNG_TASK_STK_SIZE					TASK_STK_SIZE_256//逻辑层任务堆栈大小
#define RFIDMNG_TASK_STK_SIZE				TASK_STK_SIZE_512//逻辑层任务堆栈大小
#define USARTHANDLE_TASK_STK_SIZE				TASK_STK_SIZE_256//逻辑层任务堆栈大小

#define RFIDMNG_UPDATE_TASK_STK_SIZE		TASK_STK_SIZE_512//逻辑层任务堆栈大小

#ifdef HARDWARE_TEST
	#define	HARAWARE_TEST_STK_SIZE			TASK_STK_SIZE_256//逻辑层任务堆栈大小
#endif

#endif
