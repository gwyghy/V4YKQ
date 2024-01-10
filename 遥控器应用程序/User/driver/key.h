/********************************************************************************
* 文件名称：	key.h
* 作	者：	马如意   
* 当前版本：   	V1.0
* 完成日期：    2015.04.24
* 功能描述: 	定义key.h头文件
* 历史信息：   
*           	版本信息     完成时间      原作者        注释
*
*       >>>>  在工程中的位置  <<<<
*             3-应用层
*          	  2-协议层
*          √  1-硬件驱动层
*********************************************************************************
* Copyright (c) 2014,天津华宁电子有限公司 All rights reserved.
*********************************************************************************/
#ifndef __KEY_H__
#define __KEY_H__

/********************************************************************************
* .h头文件
*********************************************************************************/
#include	"includes.h"

/********************************************************************************
* #define宏定义
*********************************************************************************/
#define	SCANKEY_TASK_TIME_SET	10//任务执行周期

// 按键事件类型定义
enum
{
	KEY_UP,
	KEY_DOWN ,	
	KEY_LONGDOWN
};

// 按键值结构
typedef struct
{
	u32	u32Key;	// 矩阵键盘扫描值
	u32	u32Status;	// 按键状态：00-释放；01-按下；02-连续按下
}stKEY;

/********************************************************************************
* 常量定义
*********************************************************************************/
/**按键键值定义(实际存在的按键)**/


/*************************************************B版**********D版***********************/
#define	KEY_11				(u32)0x00000001			
#define	KEY_12				(u32)0x00000002                   //9		9
#define	KEY_13				(u32)0x00000004     //N键         //c		a
#define	KEY_14				(u32)0x00000008		//Q键         //d		c
#define	KEY_15				(u32)0x00000010		//7           //7		1

#define	KEY_21				(u32)0x00000020		//8           //8		2
#define	KEY_22				(u32)0x00000040	    //1	          //1		b
#define	KEY_23				(u32)0x00000080	    //2           //2		d
#define	KEY_24				(u32)0x00000100	    //9           //9		7
#define	KEY_25				(u32)0x00000200	    //0           //0		8

#define	KEY_31				(u32)0x00000400		//3           //3		6
#define	KEY_32				(u32)0x00000800		//4           //4		5
#define	KEY_33				(u32)0x00001000		//-           //+		+
#define	KEY_34				(u32)0x00002000	    //＋          //-		-
#define	KEY_35				(u32)0x00004000	    //5           //5		3

#define	KEY_41				(u32)0x00008000		//6           //6		4
#define	KEY_42				(u32)0x00010000		//左成组      //N
#define	KEY_43				(u32)0x00020000		//右成组      //Q
#define	KEY_44				(u32)0x00040000	                  //0		0
#define	KEY_45				(u32)0x00080000     

#define	KEY_51				(u32)0x00100000	 	//对码
#define	KEY_52				(u32)0x00200000	 	//确认	
#define	KEY_53				(u32)0x00400000		//a键 zzw2023.12.24
#define	KEY_54				(u32)0x00800000		//b键 zzw2023.12.24
#define	KEY_55				(u32)0x01000000  	//启动

/**予以保留**/
#define	KEY_61				(u32)0x02000000//硬件中不存在	
#define	KEY_62				(u32)0x04000000//硬件中不存在	
#define	KEY_63				(u32)0x08000000//硬件中不存在	
#define	KEY_64				(u32)0x10000000//硬件中不存在	
#define	KEY_65				(u32)0x20000000//用作复用键值使用

/**特殊定义按键**/
#define	KEY_71				(u32)0x40000000//默认为保持键
#define	KEY_72				(u32)0x80000000//默认为开关机键

#define	KEY_INVALID			(u32)0xFFFFFFFF//无效按键

#define ACT_KEY_NUMBER_1        KEY_13
#define ACT_KEY_NUMBER_2        KEY_14
#define ACT_KEY_NUMBER_3        KEY_22
#define ACT_KEY_NUMBER_4        KEY_23
#define ACT_KEY_NUMBER_5		KEY_31
#define ACT_KEY_NUMBER_6		KEY_32
#define ACT_KEY_NUMBER_7		KEY_35
#define ACT_KEY_NUMBER_8		KEY_41
#define ACT_KEY_NUMBER_9		KEY_15
#define ACT_KEY_NUMBER_10		KEY_21
#define ACT_KEY_NUMBER_11		KEY_24
#define ACT_KEY_NUMBER_12		KEY_25
#define ACT_KEY_NUMBER_13		(KEY_13|KEY_MULRIPLE_VALUE)
#define ACT_KEY_NUMBER_14		(KEY_14|KEY_MULRIPLE_VALUE)
#define ACT_KEY_NUMBER_15		(KEY_22|KEY_MULRIPLE_VALUE)
#define ACT_KEY_NUMBER_16		(KEY_23|KEY_MULRIPLE_VALUE)
#define ACT_KEY_NUMBER_17		(KEY_31|KEY_MULRIPLE_VALUE)
#define ACT_KEY_NUMBER_18		(KEY_32|KEY_MULRIPLE_VALUE)
#define ACT_KEY_NUMBER_19		(KEY_35|KEY_MULRIPLE_VALUE)
#define ACT_KEY_NUMBER_20		(KEY_41|KEY_MULRIPLE_VALUE)
#define ACT_KEY_NUMBER_21		(KEY_15|KEY_MULRIPLE_VALUE)
#define ACT_KEY_NUMBER_22		(KEY_21|KEY_MULRIPLE_VALUE)
#define ACT_KEY_NUMBER_23		(KEY_24|KEY_MULRIPLE_VALUE)
#define ACT_KEY_NUMBER_24		(KEY_25|KEY_MULRIPLE_VALUE)


/********************************************************************************
* 全局变量声明
*********************************************************************************/

/********************************************************************************
* 函数声明
*********************************************************************************/
extern OS_EVENT	 *g_stKeySem;//按键事件信号量

u32 KeyInit(void);
#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
u32 KeyCorrespendLed(u32 u32KeyVal,u32 *pLedVal);
#endif
u32 AcceptKey(u32 u32KeyVal, u32 u32Info);

void KeyScanProc(void);
//从按键队列取键
u32 GetKey(stKEY *stKey);
//从按键队列查询按键，不从按键队列中清除
u16 InquireKey(stKEY *stKey);
//取按键队列计数
u16 GetKeyNum(void);
//清空GetKeyNum按键队列
void KeyEmpty(void);

#endif

