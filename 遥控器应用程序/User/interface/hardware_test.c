/********************************************************************************
* �ļ�����	hardware_test.c
* ���ߣ�	������   
* �汾��   	V1.0
* ���ڣ�      
* ��������: ��ģ����SC�˻������������ģ�顣��Ҫ�У��漰���ݵĳ�ʼ����װ���������� 
* 			����ɨ������֮���������а���������ͣ����ֹͣ״̬��ء���Ϣ��ʾ��
*			���� 
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
#include "includes.h"

/********************************************************************************
* #define�궨��
*********************************************************************************/
#define	INTERFACE_TASK_TIME_SET				10//����ִ������
#define	INTERFACE_PWR_TIME_SET				2000//������ʱ

/**������ʾ����״̬�����������͵�RFID�����֡���ʾ�������Ƶ����Ƽ���֮��Ĺ���**/
typedef	struct
{
	u32 u32InterfaceKeyValue;
	
	u8 u8DisplayMsg[17];	
}HARDWARE_KEY_CORRESPOND_TYPE;

/********************************************************************************
* ��������
*********************************************************************************/
static const HARDWARE_KEY_CORRESPOND_TYPE s_stHardWareKeyTable[] = 
{
	{KEY_72,"��1��,��1��"},

	{KEY_52,"��2��,��1��"},
	{KEY_42,"��2��,��2��"},
	{KEY_43,"��2��,��3��"},
	{KEY_55,"��2��,��4��"},

	{KEY_34,"��3��,��1��"},
	{KEY_33,"��3��,��2��"},
	{KEY_51,"��3��,��3��"},

	{KEY_13,"��4��,��1��"},
	{KEY_14,"��4��,��2��"},

	{KEY_22,"��5��,��1��"},
	{KEY_23,"��5��,��2��"},

	{KEY_31,"��6��,��1��"},
	{KEY_32,"��6��,��2��"},

	{KEY_35,"��7��,��1��"},
	{KEY_41,"��7��,��2��"},

	{KEY_15,"��8��,��1��"},
	{KEY_21,"��8��,��2��"},

	{KEY_24,"��9��,��1��"},
	{KEY_25,"��9��,��2��"},
	
	{KEY_71,"�Ա���"},
	
	{0x00,0x00}
};


static const HARDWARE_KEY_CORRESPOND_TYPE s_stHardWareKeyTable1[] = 
{
	{KEY_72,"��1��,��1��"},

	{KEY_52,"��1��,��2��"},
	{KEY_42,"��3��,��1��"},
	{KEY_43,"��3��,��2��"},
	{KEY_55,"��1��,��3��"},

	{KEY_34,"��2��,��3��"},
	{KEY_33,"��2��,��1��"},
	{KEY_51,"��2��,��2��"},

	{KEY_13,"��4��,��1��"},
	{KEY_14,"��5��,��1��"},

	{KEY_22,"��4��,��2��"},
	{KEY_23,"��5��,��2��"},

	{KEY_31,"��8��,��2��"},
	{KEY_32,"��8��,��1��"},

	{KEY_35,"��7��,��1��"},
	{KEY_41,"��7��,��2��"},

	{KEY_15,"��6��,��1��"},
	{KEY_21,"��6��,��2��"},

	{KEY_24,"��9��,��1��"},
	{KEY_25,"��9��,��2��"},
	
	
	{KEY_12,"��10��,��1��"},
	{KEY_44,"��10��,��2��"},
	
	{KEY_71,"�Ա���"},
	
	{0x00,0x00}
};

/********************************************************************************
* ��������
*********************************************************************************/
/*********************************************************************************************
** ������   ��HardwareSeachKeyDisplayMsg()  
** �������� : ��ģ���ǽ��洦���������ѭ��֮ǰ�Ĳ���
** ������� :	void *p_arg��������
** ����ֵ   ����
** ����     ��
** ����     ��2013-7-24
** ��ע     ��
**********************************************************************************************/
u32 HardwareSeachKeyDisplayMsg(u32 u32KeyValue,char *s)
{
	u8 u8I = 0x00 ;
	u32 u32ReturnValue = 0x00;
	
	if(g_u32ID == B_type)
	{
		for(u8I = 0x00;s_stHardWareKeyTable[u8I].u32InterfaceKeyValue != 0x00; u8I++)
		{
			if(u32KeyValue == s_stHardWareKeyTable[u8I].u32InterfaceKeyValue)
			{
				strcat(s,(const char *)&(s_stHardWareKeyTable[u8I].u8DisplayMsg[0x00]));
				u32ReturnValue = 0x01;
				break;
			}
		}
	}
	else
	{
		for(u8I = 0x00;s_stHardWareKeyTable1[u8I].u32InterfaceKeyValue != 0x00; u8I++)
		{
			if(u32KeyValue == s_stHardWareKeyTable1[u8I].u32InterfaceKeyValue)
			{
				strcat(s,(const char *)&(s_stHardWareKeyTable1[u8I].u8DisplayMsg[0x00]));
				u32ReturnValue = 0x01;
				break;
			}
		}
	}
	
	return u32ReturnValue;
}
/*********************************************************************************************
** ������   ��interface_task()  
** �������� : ��ģ����SC�˻������������ģ�顣��������ɨ������֮���������а������� 
** 			��ͣ����ֹͣ��״̬��ء���Ϣ��ʾ��ʱ����ȵ�
** ������� :	void *p_arg��������
** ����ֵ   ����
** ����     ��
** ����     ��2013-7-24
** ��ע     ��
**********************************************************************************************/
void hardwaretest_task(void *p_arg) 
{
	u16 u16Temp = 0x00;
	u32 u32Temp = 0x00,u32Temp2 = 0x00;	
	s32 s32Temp = 0x00;
	u32 u32Numb1,u32Numb2;
	u8 u8buf[50];
	char s[50];
	u8 err,u8temp;
	stKEY	sNewKey;

	u32 u32DelayTime = 0x00;
	u32 u32PwrCsr;
	u32 u32ShutDownFlag;
	#if 0
		char s1[40];
		char s2[40];
		char s3[40];
	#endif
	
	u32PwrCsr = PWR->CSR ;
	LogicParamApi(LOGIC_GET_SHUTDOWM_FLAG,&u32ShutDownFlag);

	#if 0	
		sprintf(s1,"%x",u32PwrCsr);
		ShowStr(OLED_DISPLAY_NORMAL,s1,0, 24);
		sprintf(s2,"%x",u32ShutDownFlag);
		ShowStr(OLED_DISPLAY_NORMAL,s2,2, 24);	
		LogicParamApi(LOGIC_GET_BATTERY_COULDATA_BACKUP,&u32DelayTime);
		sprintf(s2,"%x",u32DelayTime);
		ShowStr(OLED_DISPLAY_NORMAL,s2,4, 24);	
		OSTimeDly(1000/TICK_TIME);	//showing for 1s	
	#endif

	if(((u32ShutDownFlag&YKQ_SHUTDOWN_SLEEP_WAKEUP1) == YKQ_SHUTDOWN_SLEEP_WAKEUP1)|| (u32ShutDownFlag&YKQ_SHUTDOWN_SLEEP_WAKEUP2) == YKQ_SHUTDOWN_SLEEP_WAKEUP2)
	{
		if((u32PwrCsr&PWR_CSR_WUF)==PWR_CSR_WUF)//���ѵ������
		{
			PWR_ClearFlag(PWR_FLAG_WU);
			if(((u32ShutDownFlag&YKQ_SHUTDOWN_SLEEP_WAKEUP2)==YKQ_SHUTDOWN_SLEEP_WAKEUP2) && (GPIO_ReadInputDataBit(KEY_R5WKUP2_PORT,KEY_R5WKUP2_PIN) == Bit_SET))
			{
				while(1)
				{
					OSTimeDly(INTERFACE_TASK_TIME_SET/TICK_TIME);	
					if(GPIO_ReadInputDataBit(KEY_R5WKUP2_PORT,KEY_R5WKUP2_PIN) == Bit_SET)//���ѵ������
					{
						u32DelayTime += INTERFACE_TASK_TIME_SET;
						if(u32DelayTime >= INTERFACE_PWR_TIME_SET)
						{
							break;	
						}
					}
					else
					{
						OSTaskSuspend(BATTERYMNG_TASK_PRIO);					
						LogicShutdownProc();						
					}
				}			
			}		
			else if(((u32ShutDownFlag&YKQ_SHUTDOWN_SLEEP_WAKEUP1)==YKQ_SHUTDOWN_SLEEP_WAKEUP1) && (GPIO_ReadInputDataBit(INPUT_WAKEUP1_PORT,INPUT_WAKEUP1_PIN) == Bit_SET))//���ѵ������
			{
				while(1)
				{
					OSTimeDly(INTERFACE_TASK_TIME_SET/TICK_TIME);	
					if(GPIO_ReadInputDataBit(INPUT_WAKEUP1_PORT,INPUT_WAKEUP1_PIN) == Bit_SET)//���ѵ������
					{
						u32DelayTime += INTERFACE_TASK_TIME_SET;
						if(u32DelayTime >= INTERFACE_PWR_TIME_SET)
						{
							break;	
						}
					}
					else
					{
						OSTaskSuspend(BATTERYMNG_TASK_PRIO);
						LogicShutdownProc();					
					}
				}
			}		
			else
			{
				OSTaskSuspend(BATTERYMNG_TASK_PRIO);
				LogicShutdownProc();						
			}		
		}	
		else if((u32ShutDownFlag&YKQ_SHUTDOWN_SLEEP_IWDG) == YKQ_SHUTDOWN_SLEEP_IWDG)
		{
			OSTaskSuspend(BATTERYMNG_TASK_PRIO);
			LogicShutdownProc();						
		}			
		else if(((u32PwrCsr&PWR_CSR_VREFINTRDYF)==PWR_CSR_VREFINTRDYF))//�ڲ��ο���ѹ�߱��������
		{
			PWR_ClearFlag(PWR_CSR_VREFINTRDYF);
			if(((u32ShutDownFlag&YKQ_SHUTDOWN_SLEEP_WAKEUP1)==YKQ_SHUTDOWN_SLEEP_WAKEUP1)||((u32ShutDownFlag&YKQ_SHUTDOWN_SLEEP_WAKEUP2)==YKQ_SHUTDOWN_SLEEP_WAKEUP2))
			{
				if(((u32ShutDownFlag&YKQ_SHUTDOWN_SLEEP_WAKEUP2)==YKQ_SHUTDOWN_SLEEP_WAKEUP2) && (GPIO_ReadInputDataBit(KEY_R5WKUP2_PORT,KEY_R5WKUP2_PIN) == Bit_SET))
				{
					while(1)
					{
						OSTimeDly(INTERFACE_TASK_TIME_SET/TICK_TIME);	
						if(GPIO_ReadInputDataBit(KEY_R5WKUP2_PORT,KEY_R5WKUP2_PIN) == Bit_SET)//���ѵ������
						{
							u32DelayTime += INTERFACE_TASK_TIME_SET;
							if(u32DelayTime >= INTERFACE_PWR_TIME_SET)
							{
								break;	
							}
						}
						else
						{
							OSTaskSuspend(BATTERYMNG_TASK_PRIO);
							LogicShutdownProc();						
						}
					}			
				}		
				else if(((u32ShutDownFlag&YKQ_SHUTDOWN_SLEEP_WAKEUP1)==YKQ_SHUTDOWN_SLEEP_WAKEUP1) && (GPIO_ReadInputDataBit(INPUT_WAKEUP1_PORT,INPUT_WAKEUP1_PIN) == Bit_SET))//���ѵ������
				{
					while(1)
					{
						OSTimeDly(INTERFACE_TASK_TIME_SET/TICK_TIME);	
						if(GPIO_ReadInputDataBit(INPUT_WAKEUP1_PORT,INPUT_WAKEUP1_PIN) == Bit_SET)//���ѵ������
						{
							u32DelayTime += INTERFACE_TASK_TIME_SET;
							if(u32DelayTime >= INTERFACE_PWR_TIME_SET)
							{
								break;	
							}
						}
						else
						{
							OSTaskSuspend(BATTERYMNG_TASK_PRIO);
							LogicShutdownProc();					
						}
					}
				}
				else
				{
					OSTaskSuspend(BATTERYMNG_TASK_PRIO);
					LogicShutdownProc();						
				}			
			}
		}
		else
		{	
			OSTaskSuspend(BATTERYMNG_TASK_PRIO);
			LogicShutdownProc();
		}
	}	
	else
		;	
	
	/**��ʼ����ʾ**/
chushi_hua_part:	
	while(1)
	{
		StateLed(ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED|KEY_SHIFT_LED,0x00);
		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"����ң����",1, 24);
		ShowStr(OLED_DISPLAY_NORMAL,"���Գ���V6.1",3, 24);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s	
		StateLed(0x00, ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED|KEY_SHIFT_LED);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s	
		break;
	}
	

	if(g_u32ID == B_type)
	{	
		IrUsartChipPwrOn();	
	}
	else
		GPIO_ResetBits(GPIOC, GPIO_Pin_6);
	IrUsartRxEnable();	

shuoming_part_0:
	u16Temp = 0x00;
	LcdClear();
	ShowStr(OLED_DISPLAY_NORMAL,"     ˵��     ",0, 0);
	ShowStr(OLED_DISPLAY_NORMAL,"1)�����ּ�ѡ��",2, 0);
	ShowStr(OLED_DISPLAY_NORMAL,"2)���˳�����һ��",4, 0);
	ShowStr(OLED_DISPLAY_NORMAL,"3)�����ؼ���һ��",6, 0);

	g_stKeySem->OSEventCnt = 0x00;
	KeyEmpty();
	while(1)
	{
		OSSemPend(g_stKeySem,10/TICK_TIME, &err);
		if(err == OS_ERR_NONE)			
		{
			GetKey(&sNewKey);
			if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_72)&&(u16Temp == 0x01))
			{
				break;
			}
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_72)&&(u16Temp == 0x00))
			{
				LcdClear();
				ShowStr(OLED_DISPLAY_NORMAL,"     ˵��     ",0, 0);
				ShowStr(OLED_DISPLAY_NORMAL,"4)���������ٲ���",2, 0);
				ShowStr(OLED_DISPLAY_NORMAL,"5)���˵���������",4, 0);
				ShowStr(OLED_DISPLAY_NORMAL,"6)���Խ����ɲ���",6, 0);
				u16Temp = 0x01;
			}			
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_64))
			{
				u16Temp = 0x00;
				goto shuoming_part_0;
			}
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_71))
			{
				goto shuoming_part_0;
			}
			else
				;
		}
	}
	OSTaskSuspend(IRMNG_TASK_PRIO);
test_part_select:
	LcdClear();
	ShowStr(OLED_DISPLAY_NORMAL,"1.����2.�̺��3.������4.����5.����6.����7.���8.���߹���",0, 0);	
	g_stKeySem->OSEventCnt = 0x00;
	KeyEmpty();
	while(1)
	{
		OSSemPend(g_stKeySem,10/TICK_TIME, &err);
		if(err == OS_ERR_NONE)			
		{
			GetKey(&sNewKey);
			if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_72))
			{
				break;
			}
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_55))
			{
				goto shuoming_part_0;
			}
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_51))
			{
				goto test_part_select;
			}
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_14))
			{
				goto qidong_ceshi_part_1;
			}	
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_23))
			{
				goto honglvdeng_ceshi_part_2;
			}
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_15))
			{
				goto anjiandeng_ceshi_part_3;
			}	
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_21))
			{
				goto anjian_ceshi_part_4;
			}	
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_35))
			{
				goto hongwai_ceshi_part_5;
			}	
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_41))
			{
				goto usart_ceshi_part_6;
			}	
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_32))
			{
				goto dianchi_ceshi_part_8;
			}	
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_31))
			{
				goto nexttest_part_9;
			}
//			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_24))
//			{
//				goto nexttest_part_9;
//			}
			else
				;
		}
	}
	
qidong_ceshi_part_1:	
	/**1����������**/
	while(1)
	{
		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"1.��������....",0, 0);
		OSTimeDly(500/TICK_TIME);	//showing for 1s	
		ShowStr(OLED_DISPLAY_NORMAL,"1).OLED��:  ����",2, 0);
		OSTimeDly(500/TICK_TIME);	//showing for 1s	
		BatteryGetCoulomeData(&u16Temp);
		BatteryGetStatus((BATTERY_STATUS_TYPE *)&u32Temp);	
		if((u16Temp == 0x00)||(u16Temp > 130))
		{
			ShowStr(OLED_DISPLAY_NORMAL,"2).��ص�ѹ:����",4, 0);
		}
		else
		{
			if(u32Temp < BATTERY_TOO_LOW)
			{
				ShowStr(OLED_DISPLAY_NORMAL,"2).��ص�ѹ:����",4, 0);
			}
			else
			{
				ShowStr(OLED_DISPLAY_NORMAL,"2).��ص�ѹ:����",4, 0);
			}
		}
		OSTimeDly(1000/TICK_TIME);	//showing for 1s
		g_stKeySem->OSEventCnt = 0x00;	
		KeyEmpty();		
		break;
	}
	
	/**�ȴ���������**/
	g_stKeySem->OSEventCnt = 0x00;
	KeyEmpty();
	while(1)
	{
		OSSemPend(g_stKeySem,10/TICK_TIME, &err);
		if(err == OS_ERR_NONE)			
		{
			GetKey(&sNewKey);
			if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_72))
			{
				break;
			}
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_51))
			{
				goto qidong_ceshi_part_1;
			}	
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_55))
			{
				goto test_part_select;
			}				
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_33))
			{
				goto test_part_select;
			}
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_34))
			{
				goto honglvdeng_ceshi_part_2;
			}			
			else
				;
		}
	}

honglvdeng_ceshi_part_2:	
	/**������һ������**/
	while(1)
	{
		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.��ɫLED�Ʋ���:��",0, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"���Խ�����",4, 0);
		StateLed(ADJUST_CODE_ING_LED,0x00);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s
		
		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.��ɫLED�Ʋ���:��",0, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"���Խ�����",4, 0);
		StateLed(0x00,ADJUST_CODE_ING_LED);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s
		
		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.��ɫLED�Ʋ���:��",0, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"���Խ�����",4, 0);
		StateLed(ADJUST_CODE_ING_LED,0x00);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s

		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.��ɫLED�Ʋ���:��",0, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"���Խ�����",4, 0);
		StateLed(0x00,ADJUST_CODE_ING_LED);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s
		
		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.��ɫLED�Ʋ���:��",0, 0);	
		ShowStr(OLED_DISPLAY_NORMAL,"���Խ�����",4, 0);		
		StateLed(ADJUST_CODE_ING_LED,0x00);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s

		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.��ɫLED�Ʋ���:��",0, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"���Խ�����",4, 0);
		StateLed(0x00,ADJUST_CODE_ING_LED);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s

		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.��ɫLED�Ʋ���:��",0, 0);		
		ShowStr(OLED_DISPLAY_NORMAL,"�����ѽ���",4, 0);
		StateLed(ADJUST_CODE_ING_LED,0x00);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s
		
		StateLed(0x00,ADJUST_CODE_ING_LED);
		
		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.��ɫLED�Ʋ���:��",0, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"���Խ�����",4, 0);
		StateLed(ADJUST_CODE_SUCCEED_LED,0x00);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s
		
		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.��ɫLED�Ʋ���:��",0, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"���Խ�����",4, 0);
		StateLed(0x00,ADJUST_CODE_SUCCEED_LED);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s
		
		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.��ɫLED�Ʋ���:��",0, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"���Խ�����",4, 0);
		StateLed(ADJUST_CODE_SUCCEED_LED,0x00);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s

		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.��ɫLED�Ʋ���:��",0, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"���Խ�����",4, 0);
		StateLed(0x00,ADJUST_CODE_SUCCEED_LED);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s
		
		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.��ɫLED�Ʋ���:��",0, 0);	
		ShowStr(OLED_DISPLAY_NORMAL,"���Խ�����",4, 0);		
		StateLed(ADJUST_CODE_SUCCEED_LED,0x00);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s

		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.��ɫLED�Ʋ���:��",0, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"���Խ�����",4, 0);
		StateLed(0x00,ADJUST_CODE_SUCCEED_LED);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s

		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.��ɫLED�Ʋ���:��",0, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"���Խ�����",4, 0);
		StateLed(ADJUST_CODE_SUCCEED_LED,0x00);
		ShowStr(OLED_DISPLAY_NORMAL,"�����ѽ���",4, 0);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s
		break;
		
		g_stKeySem->OSEventCnt = 0x00;
		KeyEmpty();
		break;
	}	

	/**�ȴ���������**/
	g_stKeySem->OSEventCnt = 0x00;
	KeyEmpty();
	while(1)
	{
		OSSemPend(g_stKeySem,10/TICK_TIME, &err);
		if(err == OS_ERR_NONE)			
		{
			GetKey(&sNewKey);
			if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_72))
			{
				StateLed(0x00,ADJUST_CODE_SUCCEED_LED);
				break;
			}
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_51))
			{
				StateLed(0x00,ADJUST_CODE_SUCCEED_LED);
				goto honglvdeng_ceshi_part_2;
			}
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_55))
			{
				StateLed(0x00,ADJUST_CODE_SUCCEED_LED);
				goto test_part_select;
			}				
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_33))
			{
				StateLed(0x00,ADJUST_CODE_SUCCEED_LED);
				goto qidong_ceshi_part_1;
			}
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_34))
			{
				StateLed(0x00,ADJUST_CODE_SUCCEED_LED);
				goto anjiandeng_ceshi_part_3;
			}			
			else
				;
		}
	}
	StateLed(0x00,ADJUST_CODE_ING_LED);
	

	
anjiandeng_ceshi_part_3:	
	/**������һ������**/
	while(1)
	{
		u32 u32I = 0x00;
		u32 u32J = 0x00;

		for(u32I = 0x00;u32I <= 0x01;u32I++)
		{
			LcdClear();
			ShowStr(OLED_DISPLAY_NORMAL,"3.����LED�Ʋ���",0, 0);
			sprintf(s,"%s","�������:��");
			ShowStr(OLED_DISPLAY_NORMAL,s,2, 0);
			ShowStr(OLED_DISPLAY_NORMAL,"���Խ�����",4, 0);
			StateLed(0x00,KEY_SHIFT_LED);
			OSTimeDly(1000/TICK_TIME);	//showing for 1s	
			
			LcdClear();		
			ShowStr(OLED_DISPLAY_NORMAL,"3.����LED�Ʋ���:��",0, 0);
			sprintf(s,"%s","�������:��");
			ShowStr(OLED_DISPLAY_NORMAL,s,2, 0);			
			ShowStr(OLED_DISPLAY_NORMAL,"���Խ�����",4, 0);
			StateLed(KEY_SHIFT_LED,0x00);
			OSTimeDly(1000/TICK_TIME);	//showing for 1s
		}
		for(u32J = 0x00;u32J < 14;u32J++)
		{
			for(u32I = 0x00;u32I <= 0x01;u32I++)
			{
				LcdClear();		
				ShowStr(OLED_DISPLAY_NORMAL,"3.����LED�Ʋ���",0, 0);
				sprintf(s,"%d#%s",u32J+0x01,"��������:��");
				ShowStr(OLED_DISPLAY_NORMAL,s,2, 0);				
				ShowStr(OLED_DISPLAY_NORMAL,"���Խ�����",4, 0);
				KeyLed111(0x00,(KEY_LED_1<<u32J));
				OSTimeDly(1000/TICK_TIME);	//showing for 1s
				
				LcdClear();
				ShowStr(OLED_DISPLAY_NORMAL,"3.����LED�Ʋ���",0, 0);
				sprintf(s,"%d#%s",u32J+0x01,"��������:��");
				ShowStr(OLED_DISPLAY_NORMAL,s,2, 0);				
				ShowStr(OLED_DISPLAY_NORMAL,"���Խ�����",4, 0);
				KeyLed111((KEY_LED_1<<u32J),0x00);
				OSTimeDly(1000/TICK_TIME);	//showing for 1s	
			}
		}
		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"3.����LED�Ʋ���",0, 0);	
		sprintf(s,"%s","���뼰������:��");
		ShowStr(OLED_DISPLAY_NORMAL,s,2, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"�����ѽ���",4, 0);
		g_stKeySem->OSEventCnt = 0x00;
		KeyEmpty();
		break;		
	}

	/**�ȴ���������**/
	g_stKeySem->OSEventCnt = 0x00;
	KeyEmpty();
	while(1)
	{
		OSSemPend(g_stKeySem,10/TICK_TIME, &err);
		if(err == OS_ERR_NONE)			
		{
			GetKey(&sNewKey);
			if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_72))
			{
				StateLed(0x00,KEY_SHIFT_LED);
				KeyLed111(0x00,KEY_LED_ALL);	
				break;
			}
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_51))
			{
				StateLed(0x00,KEY_SHIFT_LED);
				KeyLed111(0x00,KEY_LED_ALL);	
				goto anjian_ceshi_part_4;
			}	
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_55))
			{
				StateLed(0x00,KEY_SHIFT_LED);
				KeyLed111(0x00,KEY_LED_ALL);	
				goto test_part_select;
			}				
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_33))
			{
				StateLed(0x00,KEY_SHIFT_LED);
				KeyLed111(0x00,KEY_LED_ALL);	
				goto anjiandeng_ceshi_part_3;
			}
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_34))
			{
				StateLed(0x00,KEY_SHIFT_LED);
				KeyLed111(0x00,KEY_LED_ALL);
				goto hongwai_ceshi_part_5;
			}			
			else
				;
		}
	}
	
anjian_ceshi_part_4:	
	/**������һ������**/
	LcdClear();	
	ShowStr(OLED_DISPLAY_NORMAL,"4.��������",0, 0);	
	ShowStr(OLED_DISPLAY_NORMAL,"�˳��򷵻��볤��",2, 0);
	g_stKeySem->OSEventCnt = 0x00;
	KeyEmpty();
	TIM_Cmd(TIM5, DISABLE);	
	while(1)
	{
		OSSemPend(g_stKeySem,10/TICK_TIME, &err);
		if(err == OS_ERR_NONE)			
		{
			GetKey(&sNewKey);
			if(sNewKey.u32Status == KEY_DOWN)
			{
				memset(s,0x00,sizeof(s));
				u32Temp = HardwareSeachKeyDisplayMsg(sNewKey.u32Key,s);
				if(u32Temp == 0x01)
				{
					LcdAreaClear(OLED_DISPLAY_NORMAL,4,0,5,127);
					ShowStr(OLED_DISPLAY_NORMAL,s,4, 0);	
				}
			}
			else if((sNewKey.u32Status == KEY_LONGDOWN)&&(sNewKey.u32Key == KEY_51))
			{
				LcdAreaClear(OLED_DISPLAY_NORMAL,4,0,5,127);
				ShowStr(OLED_DISPLAY_NORMAL,"��������",4, 0);
				OSTimeDly(1000/TICK_TIME);	//showing for 1s
				goto anjian_ceshi_part_4;
			}			
			else if((sNewKey.u32Status == KEY_LONGDOWN)&&(sNewKey.u32Key == KEY_72))
			{
				LcdAreaClear(OLED_DISPLAY_NORMAL,4,0,5,127);
				ShowStr(OLED_DISPLAY_NORMAL,"��һ��",4, 0);
				OSTimeDly(1000/TICK_TIME);	//showing for 1s
				break;
			}
			else if((sNewKey.u32Status == KEY_LONGDOWN)&&(sNewKey.u32Key == KEY_55))
			{
				LcdAreaClear(OLED_DISPLAY_NORMAL,4,0,5,127);
				ShowStr(OLED_DISPLAY_NORMAL,"�������˵�",4, 0);
				OSTimeDly(1000/TICK_TIME);	//showing for 1s	
				TIM_Cmd(TIM5, ENABLE);					
				goto test_part_select;
			}			
			else
				;
		}
	}
	g_stKeySem->OSEventCnt = 0x00;
	KeyEmpty();
	TIM_Cmd(TIM5, ENABLE);	
	
hongwai_ceshi_part_5:	
	/**������һ������**/
	u16Temp = 100;
	LcdClear();	
	ShowStr(OLED_DISPLAY_NORMAL,"5.�������....",0, 0);	
	ShowStr(OLED_DISPLAY_NORMAL,"���Խ�����",6, 0);
	ShowStr(OLED_DISPLAY_NORMAL,"1)����:0��",2, 0);
	ShowStr(OLED_DISPLAY_NORMAL,"2)����:0��",4, 0);
	u32Numb1 = 0x00;
	u32Numb2 = 0x00;
	while(u16Temp)
	{	
		u16Temp--;		
		s_stIrRecvd->OSEventCnt = 0x00;
		u8temp = IrSendInsertTxBuf(0x0102,0x03);
		if(u8temp != 0x00)
		{
			LcdAreaClear(OLED_DISPLAY_NORMAL,2,56,3,127);
			u32Numb1++;
			memset(s,0x00,sizeof(s));
			sprintf(s,"%d",u32Numb1);
			strcat(s,"��");				
			ShowStr(OLED_DISPLAY_NORMAL,s,2, 57);
		}
		else
		{
			;
		}
		OSSemPend(s_stIrRecvd,50/TICK_TIME,&err);//�ȴ����ݽ������
		OSSemPend(s_stIrRecvd,50/TICK_TIME,&err);//�ȴ����ݽ������
		OSSemPend(s_stIrRecvd,50/TICK_TIME,&err);//�ȴ����ݽ������
		if((err == OS_ERR_NONE)||(err == OS_ERR_NONE))
		{
			IrUsartRxWtrRtrGet(&u32Temp);
			IrUsartRxRdRtrGet(&u32Temp2);
			if(u32Temp >= u32Temp2)
				u8temp = u32Temp-u32Temp2;
			else
				u8temp = 200-u32Temp2+u32Temp;//IR_USART_RX_BUF_MAX
			if(u8temp==0x03)
			{
				IrUsartReadRxBuf(u8buf,u32Temp2);
				u32Temp2++;
				u32Temp2 %= 200;
				IrUsartReadRxBuf(u8buf+0x01,u32Temp2);
				u32Temp2++;
				u32Temp2 %= 200;
				IrUsartReadRxBuf(u8buf+0x02,u32Temp2);
				IrUsartRxRdRtrSet(&u32Temp);	
				if((u8buf[0x00] == 0x02)&&(u8buf[0x01] == 0x01)&&(u8buf[0x02] == 0x03))
				{
					LcdAreaClear(OLED_DISPLAY_NORMAL,4,56,5,127);
					u32Numb2++;
					memset(s,0x00,sizeof(s));
					sprintf(s,"%d",u32Numb2);
					strcat(s,"��");				
					ShowStr(OLED_DISPLAY_NORMAL,s,4, 57);
				}
			}
			else
			{
					IrUsartRxRdRtrSet(&u32Temp);
			}
		}
		else
		{
			;
		}
		OSTimeDly(100/TICK_TIME);	//showing for 1s		
	}
	LcdAreaClear(OLED_DISPLAY_NORMAL,6,0,7,127);;
	ShowStr(OLED_DISPLAY_NORMAL,"�����ѽ���",6, 0);
	OSTimeDly(3000/TICK_TIME);	//showing for 1s
	/**�ȴ���������**/
	g_stKeySem->OSEventCnt = 0x00;
	KeyEmpty();
	while(1)
	{
		OSSemPend(g_stKeySem,10/TICK_TIME, &err);
		if(err == OS_ERR_NONE)			
		{
			GetKey(&sNewKey);
			if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_72))
			{
				break;
			}
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_51))
			{
				goto usart_ceshi_part_6;
			}	
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_55))
			{
				goto test_part_select;
			}			
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_33))
			{
				goto anjian_ceshi_part_4;
			}
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_34))
			{
				goto hongwai_ceshi_part_5;
			}			
			else
				;
		}
	}

	
	usart_ceshi_part_6:
	
	
	/**������һ������**/
	u16Temp = 100;
	LcdClear();	
	ShowStr(OLED_DISPLAY_NORMAL,"6.���ڲ���....",0, 0);	
	ShowStr(OLED_DISPLAY_NORMAL,"���Խ�����",6, 0);
	ShowStr(OLED_DISPLAY_NORMAL,"1)����:0��",2, 0);
	ShowStr(OLED_DISPLAY_NORMAL,"2)����:0��",4, 0);
	u32Numb1 = 0x00;
	u32Numb2 = 0x00;
	while(u16Temp)
	{	
		u16Temp--;		
		s_stUsartRecvd->OSEventCnt = 0x00;
		
		
		if(g_u32ID != B_type)
		{
			SendSetupToWl(RFID_ININT_SYNC1);
		}
		if(u8temp != 0x00)
		{
			LcdAreaClear(OLED_DISPLAY_NORMAL,2,56,3,127);
			u32Numb1++;
			memset(s,0x00,sizeof(s));
			sprintf(s,"%d",u32Numb1);
			strcat(s,"��");				
			ShowStr(OLED_DISPLAY_NORMAL,s,2, 57);
		}
		else
		{
			;
		}
		OSSemPend(s_stUsartRecvd,50/TICK_TIME,&err);//�ȴ����ݽ������

		if(err == OS_ERR_NONE)
		{
			LcdAreaClear(OLED_DISPLAY_NORMAL,4,56,5,127);
			u32Numb2++;
			memset(s,0x00,sizeof(s));
			sprintf(s,"%d",u32Numb2);
			strcat(s,"��");				
			ShowStr(OLED_DISPLAY_NORMAL,s,4, 57);
		}
		OSTimeDly(100/TICK_TIME);	//showing for 1s		
	}
	LcdAreaClear(OLED_DISPLAY_NORMAL,6,0,7,127);;
	ShowStr(OLED_DISPLAY_NORMAL,"�����ѽ���",6, 0);
	OSTimeDly(3000/TICK_TIME);	//showing for 1s
	/**�ȴ���������**/
	g_stKeySem->OSEventCnt = 0x00;
	KeyEmpty();
	while(1)
	{
		OSSemPend(g_stKeySem,10/TICK_TIME, &err);
		if(err == OS_ERR_NONE)			
		{
			GetKey(&sNewKey);
			if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_72))
			{
				break;
			}
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_51))
			{
				goto usart_ceshi_part_6;
			}	
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_55))
			{
				goto test_part_select;
			}			
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_33))
			{
				goto anjian_ceshi_part_4;
			}
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_34))
			{
				goto anjian_ceshi_part_4;
			}			
			else
				;
		}
	}
	
	
	
//wl_ceshi_part_7:	
//	/**������һ������**/
//	LcdClear();	
//	ShowStr(OLED_DISPLAY_NORMAL,"7.���߲���",0, 0);
//	ShowStr(OLED_DISPLAY_NORMAL,"���Խ�����",6, 0);
//	ShowStr(OLED_DISPLAY_NORMAL,"1)����:0��",2, 0);
//	ShowStr(OLED_DISPLAY_NORMAL,"2)����:0��",4, 0);		
//	u16Temp = 100;
//	u32Numb1 = 0x00; 
//	u32Numb2 = 0x00; 
//	
//	u32Temp = 0x00;
//	LogicParamApi(LOGIC_SET_YKQ_NUMBER, &u32Temp);

//	while(u16Temp)
//	{
//		u16Temp--;
//		g_stLogicSem->OSEventCnt = 0x00;
//		u8temp = LOGIC_STATUS_IR_COLIBARAE_SUCCESS;
//		LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u8temp);
//		if((u16Temp%10000) == 0x00)
//			OSSemPend(g_stLogicSem,60000/TICK_TIME,&err);	
//		else
//			OSSemPend(g_stLogicSem,100/TICK_TIME,&err);	
//	
//		if(err == OS_ERR_NONE)//��Ӧ��	
//		{
//			LogicRunInfApi(LOGIC_GET_RUN_STATUS, &u8temp);
//			if(u8temp == LOGIC_STATUS_RFID_COLIBARAE_SUCCESS)
//			{
//				LcdAreaClear(OLED_DISPLAY_NORMAL,2,56,3,127);
//				LcdAreaClear(OLED_DISPLAY_NORMAL,4,56,5,127);
//				u32Numb1++;
//				u32Numb2++;
//				memset(s,0x00,sizeof(s));
//				sprintf(s,"%d",u32Numb1);
//				strcat(s,"��");				
//				ShowStr(OLED_DISPLAY_NORMAL,s,2, 57);
//				memset(s,0x00,sizeof(s));
//				sprintf(s,"%d",u32Numb2);
//				strcat(s,"��");					
//				ShowStr(OLED_DISPLAY_NORMAL,s,4, 57);
//			}
//			else
//			{
//				LcdAreaClear(OLED_DISPLAY_NORMAL,2,56,3,127);
//				u32Numb1++;
//				memset(s,0x00,sizeof(s));
//				sprintf(s,"%d",u32Numb1);
//				strcat(s,"��");				
//				ShowStr(OLED_DISPLAY_NORMAL,s,2, 57);
//			}
//		}
//		else
//		{			
//				LcdAreaClear(OLED_DISPLAY_NORMAL,2,56,3,127);
//				u32Numb1++;
//				memset(s,0x00,sizeof(s));
//				sprintf(s,"%d",u32Numb1);
//				strcat(s,"��");				
//				ShowStr(OLED_DISPLAY_NORMAL,s,2, 57);		
//		}
//		OSTimeDly(100/TICK_TIME);	//showing for 1s
//	}
//	LcdAreaClear(OLED_DISPLAY_NORMAL,6,0,7,127);;
//	ShowStr(OLED_DISPLAY_NORMAL,"�����ѽ���",6, 0);
//	OSTimeDly(3000/TICK_TIME);	//showing for 1s
//	/**�ȴ���������**/
//	g_stKeySem->OSEventCnt = 0x00;
//	KeyEmpty();
//	while(1)
//	{
//		OSSemPend(g_stKeySem,10/TICK_TIME, &err);
//		if(err == OS_ERR_NONE)			
//		{
//			GetKey(&sNewKey);
//			if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_72))
//			{
//				break;
//			}
//			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_51))
//			{
//				goto wl_ceshi_part_7;
//			}	
//			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_55))
//			{
//				goto test_part_select;
//			}			
//			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_33))
//			{
//				goto usart_ceshi_part_6;
//			}
//			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_34))
//			{
//				goto dianchi_ceshi_part_8;
//			}			
//			else
//				;
//		}
//	}

dianchi_ceshi_part_8:	
	LcdClear();	
	ShowStr(OLED_DISPLAY_NORMAL,"7.��ز���",0, 0);
	/**�ȴ���������**/
	u16Temp = 0x00;
	g_stKeySem->OSEventCnt = 0x00;
	KeyEmpty();
	while(1)
	{
		OSSemPend(g_stKeySem,10/TICK_TIME, &err);
		if(err == OS_ERR_NONE)			
		{
			GetKey(&sNewKey);
			if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_72))
			{
				break;
			}
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_51))
			{
				goto dianchi_ceshi_part_8;
			}	
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_55))
			{
				goto test_part_select;
			}			
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_33))
			{
				goto usart_ceshi_part_6;
			}
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_34))
			{
				goto nexttest_part_9;
			}			
			else
				;			
		}

		if(u16Temp == 0x00)
		{
			LcdAreaClear(OLED_DISPLAY_NORMAL,2,0,7,127);
			/**����ѹ**/
			//Ds2756Api(DS2756_READ_VOLTAGE,&s32Temp);
			u32Temp = BatteryGetVoltageTempProc();
			u32Temp2 = BatteryGetVoltageTempProc_D();
			memset(s,0x00,sizeof(s));
			if(u32Temp >=  1000)
				u32Temp /= 10;
			if(u32Temp2 >=  1000)
				u32Temp2 /= 10;
			sprintf(s,"1:%d.%d%d%dV2:%d.%d%d%dV",(u32Temp/1000),(u32Temp/100)%10,(u32Temp/10)%10,(u32Temp%10),(u32Temp2/1000),(u32Temp2/100)%10,(u32Temp2/10)%10,(u32Temp2%10));
			ShowStr(OLED_DISPLAY_NORMAL,s,2, 0);			

			u32Temp = 0x00;
			BatteryGetCoulomeData((u16 *)&u32Temp);
			u32Temp2 = 0;
			BatteryGetCoulomeData_D((u16 *)&u32Temp2);
//			if(u32Temp == 0x00)
//				BatteryReRalculateCurrent();			
			memset(s,0x00,sizeof(s));
			sprintf(s,"1����%d 2����%d",u32Temp,u32Temp2);
//			strcat(s,"%");
			ShowStr(OLED_DISPLAY_NORMAL,s,4, 0);
			
			u32Temp = 0x00;
			BatteryGetStatus((BATTERY_STATUS_TYPE *)&u32Temp);
			if(u32Temp == BATTERY_WORK_NORMAL)
			{
				ShowStr(OLED_DISPLAY_NORMAL,"1:δ���",6, 0);
			}
			else if((u32Temp >= BATTERY_START_CHARGED)&&(u32Temp <= BATTERY_CHARGE_OVER))
			{
				ShowStr(OLED_DISPLAY_NORMAL,"1:�����",6, 0);
			}		
			else if(u32Temp == BATTERY_TOO_LOW)
			{
				ShowStr(OLED_DISPLAY_NORMAL,"1:��������",6, 0);
			}
			else			
			{
				ShowStr(OLED_DISPLAY_NORMAL,"1:����",6, 0);
			}
			
			u32Temp = 0x00;
			BatteryGetStatus((BATTERY_STATUS_TYPE *)&u32Temp);
			if(u32Temp == BATTERY_WORK_NORMAL)
			{
				ShowStr(OLED_DISPLAY_NORMAL,"2:δ���",6, 64);
			}
			else if((u32Temp >= BATTERY_START_CHARGED)&&(u32Temp <= BATTERY_CHARGE_OVER))
			{
				ShowStr(OLED_DISPLAY_NORMAL,"2:�����",6, 64);
			}		
			else if(u32Temp == BATTERY_TOO_LOW)
			{
				ShowStr(OLED_DISPLAY_NORMAL,"2:��������",6, 64);
			}
			else			
			{
				ShowStr(OLED_DISPLAY_NORMAL,"2:����",6, 64);
			}
			
			OSTimeDly(1000/TICK_TIME);	//showing for 1s
		}
		u16Temp ++;
		u16Temp %= 20;//200
	}
	
	/**������һ������**/
nexttest_part_9:	
	OSTaskResume(IRMNG_TASK_PRIO);
	LcdClear();	
	ShowStr(OLED_DISPLAY_NORMAL,"8.���߹���",0, 0);	
	ShowStr(OLED_DISPLAY_NORMAL,"�볤���ػ���",2, 0);
	ShowStr(OLED_DISPLAY_NORMAL,"�����Կ��շ�",4, 0);
	StateLed(ADJUST_CODE_ING_LED,0x00);		
	g_stKeySem->OSEventCnt = 0x00;
	KeyEmpty();
	while(1)
	{
		OSSemPend(g_stKeySem,10/TICK_TIME, &err);
		if(err == OS_ERR_NONE)			
		{
			GetKey(&sNewKey);
			if((sNewKey.u32Status == KEY_LONGDOWN)&&(sNewKey.u32Key == KEY_72))
			{
				StateLed(0x00,ADJUST_CODE_ING_LED);	
				u16Temp = LOGIC_STATUS_SHUTUP;
				LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u16Temp);
				#ifdef HARDWARE_TEST
					OSTaskSuspend(HARAWARE_TEST_TASK_PRIO);
				#endif
			}
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_51))
			{
				StateLed(0x00,ADJUST_CODE_ING_LED);	
				goto nexttest_part_9;
			}
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_55))
			{
				StateLed(0x00,ADJUST_CODE_ING_LED);	
				goto test_part_select;
			}
			else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_33))
			{
				StateLed(0x00,ADJUST_CODE_ING_LED);	
				goto dianchi_ceshi_part_8;
			}
			else
				;			
		}
	}
}

