/********************************************************************************
* 文件名：	hardware_test.c
* 作者：	马如意   
* 版本：   	V1.0
* 日期：      
* 功能描述: 本模块是SC人机交互界面的主模块。主要有：涉及内容的初始化封装函数；创建 
* 			键盘扫描任务，之后，自身运行按键处理、急停闭锁停止状态监控、信息提示等
*			任务。 
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
#include "includes.h"

/********************************************************************************
* #define宏定义
*********************************************************************************/
#define	INTERFACE_TASK_TIME_SET				10//任务执行周期
#define	INTERFACE_PWR_TIME_SET				2000//唤醒延时

/**定义显示控制状态、按键、发送的RFID动作字、显示动作名称的名称几者之间的关心**/
typedef	struct
{
	u32 u32InterfaceKeyValue;
	
	u8 u8DisplayMsg[17];	
}HARDWARE_KEY_CORRESPOND_TYPE;

/********************************************************************************
* 常量定义
*********************************************************************************/
static const HARDWARE_KEY_CORRESPOND_TYPE s_stHardWareKeyTable[] = 
{
	{KEY_72,"第1行,第1列"},

	{KEY_52,"第2行,第1列"},
	{KEY_42,"第2行,第2列"},
	{KEY_43,"第2行,第3列"},
	{KEY_55,"第2行,第4列"},

	{KEY_34,"第3行,第1列"},
	{KEY_33,"第3行,第2列"},
	{KEY_51,"第3行,第3列"},

	{KEY_13,"第4行,第1列"},
	{KEY_14,"第4行,第2列"},

	{KEY_22,"第5行,第1列"},
	{KEY_23,"第5行,第2列"},

	{KEY_31,"第6行,第1列"},
	{KEY_32,"第6行,第2列"},

	{KEY_35,"第7行,第1列"},
	{KEY_41,"第7行,第2列"},

	{KEY_15,"第8行,第1列"},
	{KEY_21,"第8行,第2列"},

	{KEY_24,"第9行,第1列"},
	{KEY_25,"第9行,第2列"},
	
	{KEY_71,"自保键"},
	
	{0x00,0x00}
};


static const HARDWARE_KEY_CORRESPOND_TYPE s_stHardWareKeyTable1[] = 
{
	{KEY_72,"第1行,第1列"},

	{KEY_52,"第1行,第2列"},
	{KEY_42,"第3行,第1列"},
	{KEY_43,"第3行,第2列"},
	{KEY_55,"第1行,第3列"},

	{KEY_34,"第2行,第3列"},
	{KEY_33,"第2行,第1列"},
	{KEY_51,"第2行,第2列"},

	{KEY_13,"第4行,第1列"},
	{KEY_14,"第5行,第1列"},

	{KEY_22,"第4行,第2列"},
	{KEY_23,"第5行,第2列"},

	{KEY_31,"第8行,第2列"},
	{KEY_32,"第8行,第1列"},

	{KEY_35,"第7行,第1列"},
	{KEY_41,"第7行,第2列"},

	{KEY_15,"第6行,第1列"},
	{KEY_21,"第6行,第2列"},

	{KEY_24,"第9行,第1列"},
	{KEY_25,"第9行,第2列"},
	
	
	{KEY_12,"第10行,第1列"},
	{KEY_44,"第10行,第2列"},
	
	{KEY_71,"自保键"},
	
	{0x00,0x00}
};

/********************************************************************************
* 函数定义
*********************************************************************************/
/*********************************************************************************************
** 任务名   ：HardwareSeachKeyDisplayMsg()  
** 功能描述 : 本模块是界面处理任务进如循环之前的操作
** 输入参数 :	void *p_arg，无作用
** 返回值   ：无
** 作者     ：
** 日期     ：2013-7-24
** 备注     ：
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
** 任务名   ：interface_task()  
** 功能描述 : 本模块是SC人机交互界面的主模块。创建键盘扫描任务，之后，自身运行按键处理、 
** 			急停闭锁停止、状态监控、信息提示、时间调度等
** 输入参数 :	void *p_arg，无作用
** 返回值   ：无
** 作者     ：
** 日期     ：2013-7-24
** 备注     ：
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
		if((u32PwrCsr&PWR_CSR_WUF)==PWR_CSR_WUF)//唤醒的情况下
		{
			PWR_ClearFlag(PWR_FLAG_WU);
			if(((u32ShutDownFlag&YKQ_SHUTDOWN_SLEEP_WAKEUP2)==YKQ_SHUTDOWN_SLEEP_WAKEUP2) && (GPIO_ReadInputDataBit(KEY_R5WKUP2_PORT,KEY_R5WKUP2_PIN) == Bit_SET))
			{
				while(1)
				{
					OSTimeDly(INTERFACE_TASK_TIME_SET/TICK_TIME);	
					if(GPIO_ReadInputDataBit(KEY_R5WKUP2_PORT,KEY_R5WKUP2_PIN) == Bit_SET)//唤醒的情况下
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
			else if(((u32ShutDownFlag&YKQ_SHUTDOWN_SLEEP_WAKEUP1)==YKQ_SHUTDOWN_SLEEP_WAKEUP1) && (GPIO_ReadInputDataBit(INPUT_WAKEUP1_PORT,INPUT_WAKEUP1_PIN) == Bit_SET))//唤醒的情况下
			{
				while(1)
				{
					OSTimeDly(INTERFACE_TASK_TIME_SET/TICK_TIME);	
					if(GPIO_ReadInputDataBit(INPUT_WAKEUP1_PORT,INPUT_WAKEUP1_PIN) == Bit_SET)//唤醒的情况下
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
		else if(((u32PwrCsr&PWR_CSR_VREFINTRDYF)==PWR_CSR_VREFINTRDYF))//内部参考电压具备的情况下
		{
			PWR_ClearFlag(PWR_CSR_VREFINTRDYF);
			if(((u32ShutDownFlag&YKQ_SHUTDOWN_SLEEP_WAKEUP1)==YKQ_SHUTDOWN_SLEEP_WAKEUP1)||((u32ShutDownFlag&YKQ_SHUTDOWN_SLEEP_WAKEUP2)==YKQ_SHUTDOWN_SLEEP_WAKEUP2))
			{
				if(((u32ShutDownFlag&YKQ_SHUTDOWN_SLEEP_WAKEUP2)==YKQ_SHUTDOWN_SLEEP_WAKEUP2) && (GPIO_ReadInputDataBit(KEY_R5WKUP2_PORT,KEY_R5WKUP2_PIN) == Bit_SET))
				{
					while(1)
					{
						OSTimeDly(INTERFACE_TASK_TIME_SET/TICK_TIME);	
						if(GPIO_ReadInputDataBit(KEY_R5WKUP2_PORT,KEY_R5WKUP2_PIN) == Bit_SET)//唤醒的情况下
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
				else if(((u32ShutDownFlag&YKQ_SHUTDOWN_SLEEP_WAKEUP1)==YKQ_SHUTDOWN_SLEEP_WAKEUP1) && (GPIO_ReadInputDataBit(INPUT_WAKEUP1_PORT,INPUT_WAKEUP1_PIN) == Bit_SET))//唤醒的情况下
				{
					while(1)
					{
						OSTimeDly(INTERFACE_TASK_TIME_SET/TICK_TIME);	
						if(GPIO_ReadInputDataBit(INPUT_WAKEUP1_PORT,INPUT_WAKEUP1_PIN) == Bit_SET)//唤醒的情况下
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
	
	/**初始化显示**/
chushi_hua_part:	
	while(1)
	{
		StateLed(ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED|KEY_SHIFT_LED,0x00);
		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"无线遥控器",1, 24);
		ShowStr(OLED_DISPLAY_NORMAL,"测试程序V6.1",3, 24);
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
	ShowStr(OLED_DISPLAY_NORMAL,"     说明     ",0, 0);
	ShowStr(OLED_DISPLAY_NORMAL,"1)按数字键选择",2, 0);
	ShowStr(OLED_DISPLAY_NORMAL,"2)按退出键下一项",4, 0);
	ShowStr(OLED_DISPLAY_NORMAL,"3)按返回键上一项",6, 0);

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
				ShowStr(OLED_DISPLAY_NORMAL,"     说明     ",0, 0);
				ShowStr(OLED_DISPLAY_NORMAL,"4)按继续键再测试",2, 0);
				ShowStr(OLED_DISPLAY_NORMAL,"5)按菜单键主界面",4, 0);
				ShowStr(OLED_DISPLAY_NORMAL,"6)测试结束可操作",6, 0);
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
	ShowStr(OLED_DISPLAY_NORMAL,"1.启动2.绿红灯3.按键灯4.按键5.红外6.串口7.电池8.休眠功耗",0, 0);	
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
	/**1、启动测试**/
	while(1)
	{
		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"1.启动测试....",0, 0);
		OSTimeDly(500/TICK_TIME);	//showing for 1s	
		ShowStr(OLED_DISPLAY_NORMAL,"1).OLED屏:  正常",2, 0);
		OSTimeDly(500/TICK_TIME);	//showing for 1s	
		BatteryGetCoulomeData(&u16Temp);
		BatteryGetStatus((BATTERY_STATUS_TYPE *)&u32Temp);	
		if((u16Temp == 0x00)||(u16Temp > 130))
		{
			ShowStr(OLED_DISPLAY_NORMAL,"2).电池电压:正常",4, 0);
		}
		else
		{
			if(u32Temp < BATTERY_TOO_LOW)
			{
				ShowStr(OLED_DISPLAY_NORMAL,"2).电池电压:正常",4, 0);
			}
			else
			{
				ShowStr(OLED_DISPLAY_NORMAL,"2).电池电压:正常",4, 0);
			}
		}
		OSTimeDly(1000/TICK_TIME);	//showing for 1s
		g_stKeySem->OSEventCnt = 0x00;	
		KeyEmpty();		
		break;
	}
	
	/**等待按键操作**/
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
	/**进行下一步测试**/
	while(1)
	{
		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.绿色LED灯测试:亮",0, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"测试进行中",4, 0);
		StateLed(ADJUST_CODE_ING_LED,0x00);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s
		
		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.绿色LED灯测试:灭",0, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"测试进行中",4, 0);
		StateLed(0x00,ADJUST_CODE_ING_LED);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s
		
		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.绿色LED灯测试:亮",0, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"测试进行中",4, 0);
		StateLed(ADJUST_CODE_ING_LED,0x00);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s

		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.绿色LED灯测试:灭",0, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"测试进行中",4, 0);
		StateLed(0x00,ADJUST_CODE_ING_LED);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s
		
		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.绿色LED灯测试:亮",0, 0);	
		ShowStr(OLED_DISPLAY_NORMAL,"测试进行中",4, 0);		
		StateLed(ADJUST_CODE_ING_LED,0x00);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s

		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.绿色LED灯测试:灭",0, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"测试进行中",4, 0);
		StateLed(0x00,ADJUST_CODE_ING_LED);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s

		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.绿色LED灯测试:亮",0, 0);		
		ShowStr(OLED_DISPLAY_NORMAL,"测试已结束",4, 0);
		StateLed(ADJUST_CODE_ING_LED,0x00);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s
		
		StateLed(0x00,ADJUST_CODE_ING_LED);
		
		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.红色LED灯测试:亮",0, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"测试进行中",4, 0);
		StateLed(ADJUST_CODE_SUCCEED_LED,0x00);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s
		
		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.红色LED灯测试:灭",0, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"测试进行中",4, 0);
		StateLed(0x00,ADJUST_CODE_SUCCEED_LED);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s
		
		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.红色LED灯测试:亮",0, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"测试进行中",4, 0);
		StateLed(ADJUST_CODE_SUCCEED_LED,0x00);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s

		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.红色LED灯测试:灭",0, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"测试进行中",4, 0);
		StateLed(0x00,ADJUST_CODE_SUCCEED_LED);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s
		
		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.红色LED灯测试:亮",0, 0);	
		ShowStr(OLED_DISPLAY_NORMAL,"测试进行中",4, 0);		
		StateLed(ADJUST_CODE_SUCCEED_LED,0x00);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s

		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.红色LED灯测试:灭",0, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"测试进行中",4, 0);
		StateLed(0x00,ADJUST_CODE_SUCCEED_LED);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s

		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"2.红色LED灯测试:亮",0, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"测试进行中",4, 0);
		StateLed(ADJUST_CODE_SUCCEED_LED,0x00);
		ShowStr(OLED_DISPLAY_NORMAL,"测试已结束",4, 0);
		OSTimeDly(1000/TICK_TIME);	//showing for 1s
		break;
		
		g_stKeySem->OSEventCnt = 0x00;
		KeyEmpty();
		break;
	}	

	/**等待按键操作**/
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
	/**进行下一步测试**/
	while(1)
	{
		u32 u32I = 0x00;
		u32 u32J = 0x00;

		for(u32I = 0x00;u32I <= 0x01;u32I++)
		{
			LcdClear();
			ShowStr(OLED_DISPLAY_NORMAL,"3.按键LED灯测试",0, 0);
			sprintf(s,"%s","对码键灯:灭");
			ShowStr(OLED_DISPLAY_NORMAL,s,2, 0);
			ShowStr(OLED_DISPLAY_NORMAL,"测试进行中",4, 0);
			StateLed(0x00,KEY_SHIFT_LED);
			OSTimeDly(1000/TICK_TIME);	//showing for 1s	
			
			LcdClear();		
			ShowStr(OLED_DISPLAY_NORMAL,"3.按键LED灯测试:亮",0, 0);
			sprintf(s,"%s","对码键灯:亮");
			ShowStr(OLED_DISPLAY_NORMAL,s,2, 0);			
			ShowStr(OLED_DISPLAY_NORMAL,"测试进行中",4, 0);
			StateLed(KEY_SHIFT_LED,0x00);
			OSTimeDly(1000/TICK_TIME);	//showing for 1s
		}
		for(u32J = 0x00;u32J < 14;u32J++)
		{
			for(u32I = 0x00;u32I <= 0x01;u32I++)
			{
				LcdClear();		
				ShowStr(OLED_DISPLAY_NORMAL,"3.按键LED灯测试",0, 0);
				sprintf(s,"%d#%s",u32J+0x01,"动作键灯:灭");
				ShowStr(OLED_DISPLAY_NORMAL,s,2, 0);				
				ShowStr(OLED_DISPLAY_NORMAL,"测试进行中",4, 0);
				KeyLed111(0x00,(KEY_LED_1<<u32J));
				OSTimeDly(1000/TICK_TIME);	//showing for 1s
				
				LcdClear();
				ShowStr(OLED_DISPLAY_NORMAL,"3.按键LED灯测试",0, 0);
				sprintf(s,"%d#%s",u32J+0x01,"动作键灯:亮");
				ShowStr(OLED_DISPLAY_NORMAL,s,2, 0);				
				ShowStr(OLED_DISPLAY_NORMAL,"测试进行中",4, 0);
				KeyLed111((KEY_LED_1<<u32J),0x00);
				OSTimeDly(1000/TICK_TIME);	//showing for 1s	
			}
		}
		LcdClear();
		ShowStr(OLED_DISPLAY_NORMAL,"3.按键LED灯测试",0, 0);	
		sprintf(s,"%s","对码及动作灯:亮");
		ShowStr(OLED_DISPLAY_NORMAL,s,2, 0);
		ShowStr(OLED_DISPLAY_NORMAL,"测试已结束",4, 0);
		g_stKeySem->OSEventCnt = 0x00;
		KeyEmpty();
		break;		
	}

	/**等待按键操作**/
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
	/**进行下一步测试**/
	LcdClear();	
	ShowStr(OLED_DISPLAY_NORMAL,"4.按键测试",0, 0);	
	ShowStr(OLED_DISPLAY_NORMAL,"退出或返回请长按",2, 0);
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
				ShowStr(OLED_DISPLAY_NORMAL,"按键测试",4, 0);
				OSTimeDly(1000/TICK_TIME);	//showing for 1s
				goto anjian_ceshi_part_4;
			}			
			else if((sNewKey.u32Status == KEY_LONGDOWN)&&(sNewKey.u32Key == KEY_72))
			{
				LcdAreaClear(OLED_DISPLAY_NORMAL,4,0,5,127);
				ShowStr(OLED_DISPLAY_NORMAL,"下一项",4, 0);
				OSTimeDly(1000/TICK_TIME);	//showing for 1s
				break;
			}
			else if((sNewKey.u32Status == KEY_LONGDOWN)&&(sNewKey.u32Key == KEY_55))
			{
				LcdAreaClear(OLED_DISPLAY_NORMAL,4,0,5,127);
				ShowStr(OLED_DISPLAY_NORMAL,"返回主菜单",4, 0);
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
	/**进行下一步测试**/
	u16Temp = 100;
	LcdClear();	
	ShowStr(OLED_DISPLAY_NORMAL,"5.红外测试....",0, 0);	
	ShowStr(OLED_DISPLAY_NORMAL,"测试进行中",6, 0);
	ShowStr(OLED_DISPLAY_NORMAL,"1)发送:0次",2, 0);
	ShowStr(OLED_DISPLAY_NORMAL,"2)接收:0次",4, 0);
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
			strcat(s,"次");				
			ShowStr(OLED_DISPLAY_NORMAL,s,2, 57);
		}
		else
		{
			;
		}
		OSSemPend(s_stIrRecvd,50/TICK_TIME,&err);//等待数据接收完成
		OSSemPend(s_stIrRecvd,50/TICK_TIME,&err);//等待数据接收完成
		OSSemPend(s_stIrRecvd,50/TICK_TIME,&err);//等待数据接收完成
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
					strcat(s,"次");				
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
	ShowStr(OLED_DISPLAY_NORMAL,"测试已结束",6, 0);
	OSTimeDly(3000/TICK_TIME);	//showing for 1s
	/**等待按键操作**/
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
	
	
	/**进行下一步测试**/
	u16Temp = 100;
	LcdClear();	
	ShowStr(OLED_DISPLAY_NORMAL,"6.串口测试....",0, 0);	
	ShowStr(OLED_DISPLAY_NORMAL,"测试进行中",6, 0);
	ShowStr(OLED_DISPLAY_NORMAL,"1)发送:0次",2, 0);
	ShowStr(OLED_DISPLAY_NORMAL,"2)接收:0次",4, 0);
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
			strcat(s,"次");				
			ShowStr(OLED_DISPLAY_NORMAL,s,2, 57);
		}
		else
		{
			;
		}
		OSSemPend(s_stUsartRecvd,50/TICK_TIME,&err);//等待数据接收完成

		if(err == OS_ERR_NONE)
		{
			LcdAreaClear(OLED_DISPLAY_NORMAL,4,56,5,127);
			u32Numb2++;
			memset(s,0x00,sizeof(s));
			sprintf(s,"%d",u32Numb2);
			strcat(s,"次");				
			ShowStr(OLED_DISPLAY_NORMAL,s,4, 57);
		}
		OSTimeDly(100/TICK_TIME);	//showing for 1s		
	}
	LcdAreaClear(OLED_DISPLAY_NORMAL,6,0,7,127);;
	ShowStr(OLED_DISPLAY_NORMAL,"测试已结束",6, 0);
	OSTimeDly(3000/TICK_TIME);	//showing for 1s
	/**等待按键操作**/
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
//	/**进行下一步测试**/
//	LcdClear();	
//	ShowStr(OLED_DISPLAY_NORMAL,"7.无线测试",0, 0);
//	ShowStr(OLED_DISPLAY_NORMAL,"测试进行中",6, 0);
//	ShowStr(OLED_DISPLAY_NORMAL,"1)发送:0次",2, 0);
//	ShowStr(OLED_DISPLAY_NORMAL,"2)接收:0次",4, 0);		
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
//		if(err == OS_ERR_NONE)//有应答	
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
//				strcat(s,"次");				
//				ShowStr(OLED_DISPLAY_NORMAL,s,2, 57);
//				memset(s,0x00,sizeof(s));
//				sprintf(s,"%d",u32Numb2);
//				strcat(s,"次");					
//				ShowStr(OLED_DISPLAY_NORMAL,s,4, 57);
//			}
//			else
//			{
//				LcdAreaClear(OLED_DISPLAY_NORMAL,2,56,3,127);
//				u32Numb1++;
//				memset(s,0x00,sizeof(s));
//				sprintf(s,"%d",u32Numb1);
//				strcat(s,"次");				
//				ShowStr(OLED_DISPLAY_NORMAL,s,2, 57);
//			}
//		}
//		else
//		{			
//				LcdAreaClear(OLED_DISPLAY_NORMAL,2,56,3,127);
//				u32Numb1++;
//				memset(s,0x00,sizeof(s));
//				sprintf(s,"%d",u32Numb1);
//				strcat(s,"次");				
//				ShowStr(OLED_DISPLAY_NORMAL,s,2, 57);		
//		}
//		OSTimeDly(100/TICK_TIME);	//showing for 1s
//	}
//	LcdAreaClear(OLED_DISPLAY_NORMAL,6,0,7,127);;
//	ShowStr(OLED_DISPLAY_NORMAL,"测试已结束",6, 0);
//	OSTimeDly(3000/TICK_TIME);	//showing for 1s
//	/**等待按键操作**/
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
	ShowStr(OLED_DISPLAY_NORMAL,"7.电池测试",0, 0);
	/**等待按键操作**/
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
			/**检测电压**/
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
			sprintf(s,"1电量%d 2电量%d",u32Temp,u32Temp2);
//			strcat(s,"%");
			ShowStr(OLED_DISPLAY_NORMAL,s,4, 0);
			
			u32Temp = 0x00;
			BatteryGetStatus((BATTERY_STATUS_TYPE *)&u32Temp);
			if(u32Temp == BATTERY_WORK_NORMAL)
			{
				ShowStr(OLED_DISPLAY_NORMAL,"1:未充电",6, 0);
			}
			else if((u32Temp >= BATTERY_START_CHARGED)&&(u32Temp <= BATTERY_CHARGE_OVER))
			{
				ShowStr(OLED_DISPLAY_NORMAL,"1:充电中",6, 0);
			}		
			else if(u32Temp == BATTERY_TOO_LOW)
			{
				ShowStr(OLED_DISPLAY_NORMAL,"1:电量过低",6, 0);
			}
			else			
			{
				ShowStr(OLED_DISPLAY_NORMAL,"1:故障",6, 0);
			}
			
			u32Temp = 0x00;
			BatteryGetStatus((BATTERY_STATUS_TYPE *)&u32Temp);
			if(u32Temp == BATTERY_WORK_NORMAL)
			{
				ShowStr(OLED_DISPLAY_NORMAL,"2:未充电",6, 64);
			}
			else if((u32Temp >= BATTERY_START_CHARGED)&&(u32Temp <= BATTERY_CHARGE_OVER))
			{
				ShowStr(OLED_DISPLAY_NORMAL,"2:充电中",6, 64);
			}		
			else if(u32Temp == BATTERY_TOO_LOW)
			{
				ShowStr(OLED_DISPLAY_NORMAL,"2:电量过低",6, 64);
			}
			else			
			{
				ShowStr(OLED_DISPLAY_NORMAL,"2:故障",6, 64);
			}
			
			OSTimeDly(1000/TICK_TIME);	//showing for 1s
		}
		u16Temp ++;
		u16Temp %= 20;//200
	}
	
	/**进行下一步测试**/
nexttest_part_9:	
	OSTaskResume(IRMNG_TASK_PRIO);
	LcdClear();	
	ShowStr(OLED_DISPLAY_NORMAL,"8.休眠功耗",0, 0);	
	ShowStr(OLED_DISPLAY_NORMAL,"请长按关机键",2, 0);
	ShowStr(OLED_DISPLAY_NORMAL,"无线仍可收发",4, 0);
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

