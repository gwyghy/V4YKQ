/********************************************************************************
* 文件名：	buzzer.c
* 作者：	马如意   
* 版本：   	V1.0
* 日期：    2015.04.28
* 功能描述:  实现BEEP的协议层处理

* 修改说明：   
*
*       >>>>  在工程中的位置  <<<<
*             3-应用层
*          √  2-协议层
*             1-硬件驱动层
*********************************************************************************
* @copy
* <h2><center>&copy; COPYRIGHT 天津华宁电子有限公司 研发中心 软件部</center></h2>
*********************************************************************************/
/********************************************************************************
* .h头文件
*********************************************************************************/
#include	"includes.h"

/********************************************************************************
* #define宏定义
*********************************************************************************/

/********************************************************************************
* 常量定义
*********************************************************************************/

/********************************************************************************
* 变量定义
*********************************************************************************/
static OS_EVENT  *MutexBeep;	//发声互斥信号量

static vu16 vu16BeepCase=0;	//告警种类 =0：空闲
static vu16 vu16BeepState=0;	//告警阶段；=0：告警期；=1：间歇
static vu16 vu16BeepOffTimer=0xffff;	//间歇计时（x*10ms）
static vu16 vu16BeepOnTimer=0xffff;	//告警计时（x*10ms）
static vu16 vu16BeepCnt=0;	//告警次数

/********************************************************************************
* 函数定义
*********************************************************************************/

/***************************************************************************************
** 函数名称:Beep_Init
** 功能描述：蜂鸣器初始化
** 函数输入：无
** 函数输出：None
** 注意事项：
******************************************************************************************/
u32 BuzzerInit(void)
{
	INT8U err;
	
	vu16BeepCase = 0;	//告警种类 =0：空闲
	vu16BeepState = 0;	//告警阶段；=0：告警期；=1：间歇
	vu16BeepOffTimer = 0xffff;	//间歇计时（x*10ms）
	vu16BeepOnTimer = 0xffff;	//告警计时（x*10ms）
	vu16BeepCnt = 0;	//告警次数

	MutexBeep = OSMutexCreate(MUTEX_BEEP_PRIO, &err);	//创建发声互斥信号量
	
	return 0x01;
}

/***************************************************************************************
** 函数名称:GetBeepOffTimer
** 功能描述：依据不同发声种类的不同阶段，返回相应的发声间隔延时
** 函数输入：u16Case:所处于的情况,u16Order:.
** 函数输出：None
** 注意事项：
******************************************************************************************/
static u16 GetBuzzerOffTimer(u16 u16Case, u16 u16Order)
{
	u16 u16ReturnValue = 0x00;
	
	switch (u16Case)
	{
		case BEEP_KEY_PRESSED:
			u16ReturnValue = BEEP_KEY_OFF_TIMER;
			break;	

		case BEEP_NEAR_SELECTED:
			u16ReturnValue = BEEP_NEAR_OFF_TIMER;
			break;	

		case BEEP_AUTO_ACTION:
			u16ReturnValue = BEEP_AUTO_OFF_TIMER;
			break;	

		case BEEP_LOCKED:
			u16ReturnValue = BEEP_LOCKED_OFF_TIMER;
			break;	

		case BEEP_UNLOCKED:
			u16ReturnValue = BEEP_UNLOCKED_OFF_TIMER;
			break;	

		case BEEP_END_PRESSED:
			u16ReturnValue = BEEP_END_OFF_TIMER;
			break;	

		default:
			u16ReturnValue = 0x00;
			break;			
	}
	return u16ReturnValue;
}

/***************************************************************************************
** 函数名称:GetBeepOnTimer
** 功能描述：依据不同发声种类的不同阶段，返回相应的发声延时
** 函数输入：u16Case:所处于的情况,u16Order:.
** 函数输出：None
** 注意事项：
******************************************************************************************/
static u16 GetBuzzerOnTimer(u16 u16Case, u16 u16Order)
{
	u16 u16ReturnValue = 0x00;
	
	switch (u16Case)
	{
		case BEEP_KEY_PRESSED:
			u16ReturnValue = BEEP_KEY_ON_TIMER;
			break;	

		case BEEP_NEAR_SELECTED:
			u16ReturnValue = BEEP_NEAR_ON_TIMER;
			break;	

		case BEEP_AUTO_ACTION:
			u16ReturnValue = BEEP_AUTO_ON_TIMER;
			break;	

		case BEEP_LOCKED:
			u16ReturnValue = BEEP_LOCKED_ON_TIMER;
			break;	

		case BEEP_UNLOCKED:
			u16ReturnValue = BEEP_UNLOCKED_ON_TIMER;
			break;	

		case BEEP_END_PRESSED:
			u16ReturnValue = BEEP_END_ON_TIMER;
			break;	

		default:
			u16ReturnValue = 0x00;
			break;
	}
	
	return u16ReturnValue;
}

/***************************************************************************************
** 函数名称:BeepProc
** 功能描述：发生处理
** 函数输入：无
** 函数输出：None
** 注意事项：
******************************************************************************************/
void BuzzerProc(void)
{
	INT8U err;

	//lock
	OSMutexPend(MutexBeep, 0, &err);	//等待发声互斥信号量

	if (vu16BeepCase)
	{
		switch (vu16BeepState)
		{
			case 0:	//发声阶段
				if (!vu16BeepOnTimer)	//计时结束
				{
					BUZZER_OFF();
					vu16BeepOnTimer = 0xffff;
					if ((vu16BeepCnt != 0xffff) && vu16BeepCnt)
						vu16BeepCnt--;
					if (vu16BeepCnt)
					{
						//如果是多次发声，则，依据vu16BeepCnt不同，调整发声间歇时间
						vu16BeepOffTimer = GetBuzzerOffTimer(vu16BeepCase, vu16BeepCnt);
						vu16BeepState = 1;
					}
					else	//结束发声
					{
						vu16BeepOffTimer = 0xffff;
						vu16BeepState = 0;
						vu16BeepCase = 0;
					}
				}
				else
					vu16BeepOnTimer--;
			break;
				
			case 1:	//发声间歇
				if (!vu16BeepOffTimer)
				{
					vu16BeepOffTimer = 0xffff;
					if (vu16BeepCnt)
					{
						BUZZER_ON();
						vu16BeepState = 0;
						//如果是多次发声，则，依据vu16BeepCnt不同，调整发声时间
						vu16BeepOnTimer = GetBuzzerOnTimer(vu16BeepCase, vu16BeepCnt);
					}
					else
					{
						vu16BeepOnTimer = 0xffff;
						vu16BeepState = 0;
						vu16BeepCase = 0;
					}
				}
				else
					vu16BeepOffTimer--;
			break;
		}
	}

	//unlock
	OSMutexPost(MutexBeep);	//释放发声互斥信号量
}

/***************************************************************************************
** 函数名称:BeepOn
** 功能描述：启动发声
** 函数输入：u16Case:所处于的阶段
** 函数输出：None
** 注意事项：
******************************************************************************************/
void BuzzerOn(u16 u16Case)
{
	INT8U err;

	if (vu16BeepCase != 0 && u16Case == BEEP_KEY_PRESSED)
		return;
	if (vu16BeepCase == BEEP_NEAR_SELECTED && u16Case == BEEP_NEAR_SELECTED)
		return;
	//lock
	OSMutexPend(MutexBeep, 0, &err);	//等待发声互斥信号量

	vu16BeepCase = u16Case;
	vu16BeepState = 0;
	vu16BeepOffTimer = 0xffff;
	switch(u16Case)
	{
		case BEEP_KEY_PRESSED:
			vu16BeepOnTimer = BEEP_KEY_ON_TIMER;
			vu16BeepCnt = BEEP_KEY_COUNT;
		break;

		case BEEP_NEAR_SELECTED:
			vu16BeepOnTimer = BEEP_NEAR_ON_TIMER_1ST;
			vu16BeepCnt = BEEP_NEAR_COUNT;
		break;

		case BEEP_AUTO_ACTION:
			vu16BeepOnTimer = BEEP_AUTO_ON_TIMER;
			vu16BeepCnt = BEEP_AUTO_COUNT;
		break;

		case BEEP_LOCKED:
			vu16BeepOnTimer = BEEP_LOCKED_ON_TIMER;
			vu16BeepCnt = BEEP_LOCKED_COUNT;
		break;

		case BEEP_UNLOCKED:
			vu16BeepOnTimer = BEEP_UNLOCKED_ON_TIMER;
			vu16BeepCnt = BEEP_UNLOCKED_COUNT;
		break;

		case BEEP_END_PRESSED:
			vu16BeepOnTimer = BEEP_END_ON_TIMER;
			vu16BeepCnt = BEEP_END_COUNT;
		break;
		
		default:
			break;
	}
	BUZZER_ON();

	//unlock
	OSMutexPost(MutexBeep);	//释放发声互斥信号量
}

/***************************************************************************************
** 函数名称:BeepOff
** 功能描述：停止告警
** 函数输入：无
** 函数输出：None
** 注意事项：
******************************************************************************************/
void BuzzerOff(void)
{
	INT8U err;

	//lock
	OSMutexPend(MutexBeep, 0, &err);	//等待发声互斥信号量

	vu16BeepCase = 0;
	vu16BeepState = 0;
	vu16BeepOffTimer = 0xffff;
	vu16BeepOnTimer = 0xffff;
	vu16BeepCnt = 0xffff;
	BUZZER_OFF();

	//unlock
	OSMutexPost(MutexBeep);	//释放发声互斥信号量
}
