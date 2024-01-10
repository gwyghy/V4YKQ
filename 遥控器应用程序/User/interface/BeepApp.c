/********************************************************************************
* �ļ�����	BeepApp.c
* ���ߣ�	������   
* �汾��   	V1.0
* ���ڣ�    2015.04.28
* ��������:  ʵ��BEEP��Э��㴦��

* �޸�˵����   
*
*       >>>>  �ڹ����е�λ��  <<<<
*             3-Ӧ�ò�
*          ��  2-Э���
*             1-Ӳ��������
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
#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
/********************************************************************************
* ��������
*********************************************************************************/
#define	BEEPMNG_TASK_TIME_SET			10
#define	BUZZERMNG_TASK_POWERUP_DELAY	10
/********************************************************************************
* ��������
*********************************************************************************/
static OS_EVENT  *s_stMutexBeep;	//���������ź���

static vu16 s_vu16BeepCase=0;	//�澯���� =0������
static vu16 s_vu16BeepState=0;	//�澯�׶Σ�=0���澯�ڣ�=1����Ъ
static vu16 s_vu16BeepOffTimer=0xffff;	//��Ъ��ʱ��x*10ms��
static vu16 s_vu16BeepOnTimer=0xffff;	//�澯��ʱ��x*10ms��
static vu16 s_vu16BeepCnt=0;	//�澯����

/********************************************************************************
* ��������
*********************************************************************************/

/***************************************************************************************
** ��������:BuzzerInit
** ������������������ʼ��
** �������룺��
** ���������None
** ע�����
******************************************************************************************/
u32 BuzzerInit(void)
{
	INT8U err;
	
	s_vu16BeepCase = 0;	//�澯���� =0������
	s_vu16BeepState = 0;	//�澯�׶Σ�=0���澯�ڣ�=1����Ъ
	s_vu16BeepOffTimer = 0xffff;	//��Ъ��ʱ��x*10ms��
	s_vu16BeepOnTimer = 0xffff;	//�澯��ʱ��x*10ms��
	s_vu16BeepCnt = 0;	//�澯����

	s_stMutexBeep = OSMutexCreate(MUTEX_BEEP_PRIO, &err);	//�������������ź���
	
	return 0x01;
}

/***************************************************************************************
** ��������:GetBeepOffTimer
** �������������ݲ�ͬ��������Ĳ�ͬ�׶Σ�������Ӧ�ķ��������ʱ
** �������룺u16Case:�����ڵ����,u16Order:.
** ���������None
** ע�����
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
** ��������:GetBeepOnTimer
** �������������ݲ�ͬ��������Ĳ�ͬ�׶Σ�������Ӧ�ķ�����ʱ
** �������룺u16Case:�����ڵ����,u16Order:.
** ���������None
** ע�����
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
** ��������:BuzzerProc
** ������������������
** �������룺��
** ���������None
** ע�����
******************************************************************************************/
void BuzzerProc(void)
{
	INT8U err;

	//lock
	OSMutexPend(s_stMutexBeep, 0, &err);	//�ȴ����������ź���

	if (s_vu16BeepCase)
	{
		switch (s_vu16BeepState)
		{
			case 0:	//�����׶�
				if (!s_vu16BeepOnTimer)	//��ʱ����
				{
					BUZZER_OFF();
					s_vu16BeepOnTimer = 0xffff;
					if ((s_vu16BeepCnt != 0xffff) && s_vu16BeepCnt)
						s_vu16BeepCnt--;
					if (s_vu16BeepCnt)
					{
						//����Ƕ�η�����������vu16BeepCnt��ͬ������������Ъʱ��
						s_vu16BeepOffTimer = GetBuzzerOffTimer(s_vu16BeepCase, s_vu16BeepCnt);
						s_vu16BeepState = 1;
					}
					else	//��������
					{
						s_vu16BeepOffTimer = 0xffff;
						s_vu16BeepState = 0;
						s_vu16BeepCase = 0;
					}
				}
				else
					s_vu16BeepOnTimer--;
			break;
				
			case 1:	//������Ъ
				if (!s_vu16BeepOffTimer)
				{
					s_vu16BeepOffTimer = 0xffff;
					if (s_vu16BeepCnt)
					{
						BUZZER_ON();
						s_vu16BeepState = 0;
						//����Ƕ�η�����������vu16BeepCnt��ͬ����������ʱ��
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
	OSMutexPost(s_stMutexBeep);	//�ͷŷ��������ź���
}

/***************************************************************************************
** ��������:BuzzerOn
** ������������������
** �������룺u16Case:�����ڵĽ׶�
** ���������None
** ע�����
******************************************************************************************/
void BuzzerOn(u16 u16Case)
{
	INT8U err;

	if (s_vu16BeepCase != 0 && u16Case == BEEP_KEY_PRESSED)
		return;
	//lock
	OSMutexPend(s_stMutexBeep, 0, &err);	//�ȴ����������ź���

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
	OSMutexPost(s_stMutexBeep);	//�ͷŷ��������ź���
}

/***************************************************************************************
** ��������:BuzzerOff
** ����������ֹͣ�澯
** �������룺��
** ���������None
** ע�����
******************************************************************************************/
void BuzzerOff(void)
{
	INT8U err;

	//lock
	OSMutexPend(s_stMutexBeep, 0, &err);	//�ȴ����������ź���

	s_vu16BeepCase = 0;
	s_vu16BeepState = 0;
	s_vu16BeepOffTimer = 0xffff;
	s_vu16BeepOnTimer = 0xffff;
	s_vu16BeepCnt = 0xffff;
	BUZZER_OFF();

	//unlock
	OSMutexPost(s_stMutexBeep);	//�ͷŷ��������ź���
}

/*********************************************************************************
** ��������:BuzzerMng_Task
** ������������������������
** �������룺��
** ���������None
** ע�����
**********************************************************************************/
void BuzzerMng_Task(void *p_arg)
{
	BuzzerInit();
	OSTimeDly(BUZZERMNG_TASK_POWERUP_DELAY/TICK_TIME);//���ϵ�ʱ����ʱ���д���	
	while(1)
	{
		OSTimeDly(BEEPMNG_TASK_TIME_SET/TICK_TIME);//
		BuzzerProc();
	}

}
#endif

