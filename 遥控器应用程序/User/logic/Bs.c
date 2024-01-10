/********************************************************************************
* �ļ�����	buzzer.c
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

/********************************************************************************
* ��������
*********************************************************************************/

/********************************************************************************
* ��������
*********************************************************************************/
static OS_EVENT  *MutexBeep;	//���������ź���

static vu16 vu16BeepCase=0;	//�澯���� =0������
static vu16 vu16BeepState=0;	//�澯�׶Σ�=0���澯�ڣ�=1����Ъ
static vu16 vu16BeepOffTimer=0xffff;	//��Ъ��ʱ��x*10ms��
static vu16 vu16BeepOnTimer=0xffff;	//�澯��ʱ��x*10ms��
static vu16 vu16BeepCnt=0;	//�澯����

/********************************************************************************
* ��������
*********************************************************************************/

/***************************************************************************************
** ��������:Beep_Init
** ������������������ʼ��
** �������룺��
** ���������None
** ע�����
******************************************************************************************/
u32 BuzzerInit(void)
{
	INT8U err;
	
	vu16BeepCase = 0;	//�澯���� =0������
	vu16BeepState = 0;	//�澯�׶Σ�=0���澯�ڣ�=1����Ъ
	vu16BeepOffTimer = 0xffff;	//��Ъ��ʱ��x*10ms��
	vu16BeepOnTimer = 0xffff;	//�澯��ʱ��x*10ms��
	vu16BeepCnt = 0;	//�澯����

	MutexBeep = OSMutexCreate(MUTEX_BEEP_PRIO, &err);	//�������������ź���
	
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
** ��������:BeepProc
** ������������������
** �������룺��
** ���������None
** ע�����
******************************************************************************************/
void BuzzerProc(void)
{
	INT8U err;

	//lock
	OSMutexPend(MutexBeep, 0, &err);	//�ȴ����������ź���

	if (vu16BeepCase)
	{
		switch (vu16BeepState)
		{
			case 0:	//�����׶�
				if (!vu16BeepOnTimer)	//��ʱ����
				{
					BUZZER_OFF();
					vu16BeepOnTimer = 0xffff;
					if ((vu16BeepCnt != 0xffff) && vu16BeepCnt)
						vu16BeepCnt--;
					if (vu16BeepCnt)
					{
						//����Ƕ�η�����������vu16BeepCnt��ͬ������������Ъʱ��
						vu16BeepOffTimer = GetBuzzerOffTimer(vu16BeepCase, vu16BeepCnt);
						vu16BeepState = 1;
					}
					else	//��������
					{
						vu16BeepOffTimer = 0xffff;
						vu16BeepState = 0;
						vu16BeepCase = 0;
					}
				}
				else
					vu16BeepOnTimer--;
			break;
				
			case 1:	//������Ъ
				if (!vu16BeepOffTimer)
				{
					vu16BeepOffTimer = 0xffff;
					if (vu16BeepCnt)
					{
						BUZZER_ON();
						vu16BeepState = 0;
						//����Ƕ�η�����������vu16BeepCnt��ͬ����������ʱ��
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
	OSMutexPost(MutexBeep);	//�ͷŷ��������ź���
}

/***************************************************************************************
** ��������:BeepOn
** ������������������
** �������룺u16Case:�����ڵĽ׶�
** ���������None
** ע�����
******************************************************************************************/
void BuzzerOn(u16 u16Case)
{
	INT8U err;

	if (vu16BeepCase != 0 && u16Case == BEEP_KEY_PRESSED)
		return;
	if (vu16BeepCase == BEEP_NEAR_SELECTED && u16Case == BEEP_NEAR_SELECTED)
		return;
	//lock
	OSMutexPend(MutexBeep, 0, &err);	//�ȴ����������ź���

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
	OSMutexPost(MutexBeep);	//�ͷŷ��������ź���
}

/***************************************************************************************
** ��������:BeepOff
** ����������ֹͣ�澯
** �������룺��
** ���������None
** ע�����
******************************************************************************************/
void BuzzerOff(void)
{
	INT8U err;

	//lock
	OSMutexPend(MutexBeep, 0, &err);	//�ȴ����������ź���

	vu16BeepCase = 0;
	vu16BeepState = 0;
	vu16BeepOffTimer = 0xffff;
	vu16BeepOnTimer = 0xffff;
	vu16BeepCnt = 0xffff;
	BUZZER_OFF();

	//unlock
	OSMutexPost(MutexBeep);	//�ͷŷ��������ź���
}
