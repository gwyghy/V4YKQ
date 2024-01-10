/********************************************************************************
* 文件名：	BeepApp.c
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
#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
/********************************************************************************
* 常量定义
*********************************************************************************/
#define	BEEPMNG_TASK_TIME_SET			10
#define	BUZZERMNG_TASK_POWERUP_DELAY	10
/********************************************************************************
* 变量定义
*********************************************************************************/
static OS_EVENT  *s_stMutexBeep;	//发声互斥信号量

static vu16 s_vu16BeepCase=0;	//告警种类 =0：空闲
static vu16 s_vu16BeepState=0;	//告警阶段；=0：告警期；=1：间歇
static vu16 s_vu16BeepOffTimer=0xffff;	//间歇计时（x*10ms）
static vu16 s_vu16BeepOnTimer=0xffff;	//告警计时（x*10ms）
static vu16 s_vu16BeepCnt=0;	//告警次数

/********************************************************************************
* 函数定义
*********************************************************************************/

/***************************************************************************************
** 函数名称:BuzzerInit
** 功能描述：蜂鸣器初始化
** 函数输入：无
** 函数输出：None
** 注意事项：
******************************************************************************************/
u32 BuzzerInit(void)
{
	INT8U err;
	
	s_vu16BeepCase = 0;	//告警种类 =0：空闲
	s_vu16BeepState = 0;	//告警阶段；=0：告警期；=1：间歇
	s_vu16BeepOffTimer = 0xffff;	//间歇计时（x*10ms）
	s_vu16BeepOnTimer = 0xffff;	//告警计时（x*10ms）
	s_vu16BeepCnt = 0;	//告警次数

	s_stMutexBeep = OSMutexCreate(MUTEX_BEEP_PRIO, &err);	//创建发声互斥信号量
	
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

		case BEEP_POWER_NOTE:
			u16ReturnValue = BEEP_POWER_OFF_TIMER;
			break;	

		case BEEP_SHUTDOWN_NOTE:
			u16ReturnValue = BEEP_SHUTDOWN_OFF_TIMER;
			break;	

		case BEEP_COLIBRATE_START:
			u16ReturnValue = BEEP_COLIBRAT_OFF_TIMER;
			break;	

		case BEEP_UNCOLIBRAT:
			u16ReturnValue = BEEP_UNCOLIBRAT_OFF_TIMER;
			break;	

		case BEEP_ACTION:
			u16ReturnValue = BEEP_ACTION_OFF_TIMER;
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

		case BEEP_POWER_NOTE:
			u16ReturnValue = BEEP_POWER_ON_TIMER;
			break;	

		case BEEP_SHUTDOWN_NOTE:
			u16ReturnValue = BEEP_SHUTDOWN_ON_TIMER;
			break;	

		case BEEP_COLIBRATE_START:
			u16ReturnValue = BEEP_COLIBRAT_ON_TIMER;
			break;	

		case BEEP_UNCOLIBRAT:
			u16ReturnValue = BEEP_UNCOLIBRAT_ON_TIMER;
			break;	

		case BEEP_ACTION:
			u16ReturnValue = BEEP_ACTION_ON_TIMER;
			break;	

		default:
			u16ReturnValue = 0x00;
			break;
	}
	
	return u16ReturnValue;
}

/***************************************************************************************
** 函数名称:BuzzerProc
** 功能描述：发声处理
** 函数输入：无
** 函数输出：None
** 注意事项：
******************************************************************************************/
void BuzzerProc(void)
{
	INT8U err;

	//lock
	OSMutexPend(s_stMutexBeep, 0, &err);	//等待发声互斥信号量

	if (s_vu16BeepCase)
	{
		switch (s_vu16BeepState)
		{
			case 0:	//发声阶段
				if (!s_vu16BeepOnTimer)	//计时结束
				{
					BUZZER_OFF();
					s_vu16BeepOnTimer = 0xffff;
					if ((s_vu16BeepCnt != 0xffff) && s_vu16BeepCnt)
						s_vu16BeepCnt--;
					if (s_vu16BeepCnt)
					{
						//如果是多次发声，则，依据vu16BeepCnt不同，调整发声间歇时间
						s_vu16BeepOffTimer = GetBuzzerOffTimer(s_vu16BeepCase, s_vu16BeepCnt);
						s_vu16BeepState = 1;
					}
					else	//结束发声
					{
						s_vu16BeepOffTimer = 0xffff;
						s_vu16BeepState = 0;
						s_vu16BeepCase = 0;
					}
				}
				else
					s_vu16BeepOnTimer--;
			break;
				
			case 1:	//发声间歇
				if (!s_vu16BeepOffTimer)
				{
					s_vu16BeepOffTimer = 0xffff;
					if (s_vu16BeepCnt)
					{
						BUZZER_ON();
						s_vu16BeepState = 0;
						//如果是多次发声，则，依据vu16BeepCnt不同，调整发声时间
						s_vu16BeepOnTimer = GetBuzzerOnTimer(s_vu16BeepCase, s_vu16BeepCnt);
					}
					else
					{
						s_vu16BeepOnTimer = 0xffff;
						s_vu16BeepState = 0;
						s_vu16BeepCase = 0;
					}
				}
				else
					s_vu16BeepOffTimer--;
			break;
		}
	}

	//unlock
	OSMutexPost(s_stMutexBeep);	//释放发声互斥信号量
}

/***************************************************************************************
** 函数名称:BuzzerOn
** 功能描述：启动发声
** 函数输入：u16Case:所处于的阶段
** 函数输出：None
** 注意事项：
******************************************************************************************/
void BuzzerOn(u16 u16Case)
{
	INT8U err;

	if (s_vu16BeepCase != 0 && u16Case == BEEP_KEY_PRESSED)
		return;
	//lock
	OSMutexPend(s_stMutexBeep, 0, &err);	//等待发声互斥信号量

	s_vu16BeepCase = u16Case;
	s_vu16BeepState = 0;
	s_vu16BeepOffTimer = 0xffff;
	switch(u16Case)
	{
		case BEEP_KEY_PRESSED:
			s_vu16BeepOnTimer = BEEP_KEY_ON_TIMER;
			s_vu16BeepCnt = BEEP_KEY_COUNT;
		break;

		case BEEP_POWER_NOTE:
			s_vu16BeepOnTimer = BEEP_POWER_ON_TIMER;
			s_vu16BeepCnt = BEEP_POWER_COUNT;
		break;

		case BEEP_SHUTDOWN_NOTE:
			s_vu16BeepOnTimer = BEEP_SHUTDOWN_ON_TIMER;
			s_vu16BeepCnt = BEEP_SHUTDOWN_COUNT;
		break;

		case BEEP_COLIBRATE_START:
			s_vu16BeepOnTimer = BEEP_COLIBRAT_ON_TIMER;
			s_vu16BeepCnt = BEEP_COLIBRAT_COUNT;
		break;

		case BEEP_UNCOLIBRAT:
			s_vu16BeepOnTimer = BEEP_UNCOLIBRAT_ON_TIMER;
			s_vu16BeepCnt = BEEP_UNCOLIBRAT_COUNT;
		break;

		case BEEP_ACTION:
			s_vu16BeepOnTimer = BEEP_ACTION_ON_TIMER;
			s_vu16BeepCnt = BEEP_ACTION_COUNT;
		break;
		
		default:
			break;
	}
	BUZZER_ON();

	//unlock
	OSMutexPost(s_stMutexBeep);	//释放发声互斥信号量
}

/***************************************************************************************
** 函数名称:BuzzerOff
** 功能描述：停止告警
** 函数输入：无
** 函数输出：None
** 注意事项：
******************************************************************************************/
void BuzzerOff(void)
{
	INT8U err;

	//lock
	OSMutexPend(s_stMutexBeep, 0, &err);	//等待发声互斥信号量

	s_vu16BeepCase = 0;
	s_vu16BeepState = 0;
	s_vu16BeepOffTimer = 0xffff;
	s_vu16BeepOnTimer = 0xffff;
	s_vu16BeepCnt = 0xffff;
	BUZZER_OFF();

	//unlock
	OSMutexPost(s_stMutexBeep);	//释放发声互斥信号量
}

/*********************************************************************************
** 函数名称:BuzzerMng_Task
** 功能描述：蜂鸣器管理任务
** 函数输入：无
** 函数输出：None
** 注意事项：
**********************************************************************************/
void BuzzerMng_Task(void *p_arg)
{
	BuzzerInit();
	OSTimeDly(BUZZERMNG_TASK_POWERUP_DELAY/TICK_TIME);//在上电时，延时进行处理	
	while(1)
	{
		OSTimeDly(BEEPMNG_TASK_TIME_SET/TICK_TIME);//
		BuzzerProc();
	}

}
#endif

