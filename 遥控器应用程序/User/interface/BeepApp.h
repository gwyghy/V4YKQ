/********************************************************************************
* 文件名称：	BeepApp.h
* 作	者：	马如意   
* 当前版本：   	V1.0
* 完成日期：    2015.04.24
* 功能描述: 	定义BeepApp.h头文件
* 历史信息：   
*           	版本信息     完成时间      原作者        注释
*
*       >>>>  在工程中的位置  <<<<
*             3-应用层
*          √  2-协议层
*          	  1-硬件驱动层
*********************************************************************************
* Copyright (c) 2014,天津华宁电子有限公司 All rights reserved.
*********************************************************************************/
#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
#ifndef __BEEPAPP_H__
#define __BEEPAPP_H__

/********************************************************************************
* .h头文件
*********************************************************************************/
#include "includes.h"

/********************************************************************************
* #define宏定义
*********************************************************************************/
#define	BUZZER_ON()		BEEP_ON()
#define	BUZZER_OFF()	BEEP_OFF()

/********************************************************************************
* 常量定义
*********************************************************************************/

/********************************************************************************
* 全局变量声明
*********************************************************************************/

/********************************************************************************
* 函数声明
*********************************************************************************/
#define	BEEP_KEY_PRESSED			(u16)1	//按键音
#define BEEP_KEY_COUNT				(u16)1	//发声次数
#define BEEP_KEY_ON_TIMER			(u16)10	//发声时长（*10ms）
#define BEEP_KEY_OFF_TIMER			(u16)0	//间歇时长（*10ms）

#define	BEEP_POWER_NOTE				(u16)2	//电量相关提示音
#define BEEP_POWER_COUNT			(u16)1	//发声次数
#define BEEP_POWER_ON_TIMER			(u16)10	//发声时长（*10ms）
#define BEEP_POWER_OFF_TIMER		(u16)0	//间歇时长（*10ms?

#define	BEEP_SHUTDOWN_NOTE			(u16)3	//电量相关提示音
#define BEEP_SHUTDOWN_COUNT			(u16)1	//发声次数
#define BEEP_SHUTDOWN_ON_TIMER		(u16)300	//发声时长（*10ms）
#define BEEP_SHUTDOWN_OFF_TIMER		(u16)0	//间歇时长（*10ms?

#define	BEEP_COLIBRATE_START		(u16)4	//开始对码
#define BEEP_COLIBRAT_COUNT			(u16)0x1	//发声次数
#define BEEP_COLIBRAT_ON_TIMER		(u16)50	//发声时长（*10ms）
#define BEEP_COLIBRAT_OFF_TIMER		(u16)50	//间歇时长（*10ms）

#define	BEEP_UNCOLIBRAT		(u16)5	//开始对码
#define BEEP_UNCOLIBRAT_COUNT		(u16)0x1	//发声次数
#define BEEP_UNCOLIBRAT_ON_TIMER	(u16)10	//发声时长（*10ms）
#define BEEP_UNCOLIBRAT_OFF_TIMER	(u16)0	//间歇时长（*10ms）

#define	BEEP_ACTION					(u16)6	//操作动作
#define BEEP_ACTION_COUNT			(u16)0x01	//发声次数
#define BEEP_ACTION_ON_TIMER		(u16)10	//发声时长（*10ms）
#define BEEP_ACTION_OFF_TIMER		(u16)0	//间歇时长（*10ms）

/********************************************************************************
* 函数声明
*********************************************************************************/
u32 BuzzerInit(void)
void BuzzerProc(void);
void BuzzerOn(u16 u16Case);
void BuzzerOff(void);
void BuzzerMng_Task(void *p_arg);
#endif
#endif

