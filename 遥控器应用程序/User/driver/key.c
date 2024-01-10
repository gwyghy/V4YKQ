/********************************************************************************
* 文件名：	key.c
* 作者：	马如意   
* 版本：   	V1.0
* 日期：    2015.04.28
* 功能描述:  实现按键的驱动

* 修改说明：   
*
*       >>>>  在工程中的位置  <<<<
*             3-应用层
*             2-协议层
*         √   1-硬件驱动层
*********************************************************************************
* @copy
* <h2><center>&copy; COPYRIGHT 天津华宁电子有限公司 研发中心 软件部</center></h2>
*********************************************************************************/
/********************************************************************************
* .h头文件
*********************************************************************************/
#include	"includes.h"
#include	"InterfaceApp.h"
/********************************************************************************
* #define宏定义
*********************************************************************************/
#define	KEY_NUMB_MAX			26//按键的最大值
#define	KEY_SAMETIME_MAX		2//同时按下键的键数最大值
#define	KEY_LONGDOWN_TIME_SET	(2000/SCANKEY_TASK_TIME_SET)//长按时间定义

#define KEY_QUEUE_CNT			20// 按键对列长度

/**按键与指示灯之间的对应关系**/
#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
	typedef struct
	{
		u32 u32KeyNumb;
		u32 u32KeyLedNumb;
	}KEY_LED_CORRESPOND_TYPE;
#endif
	
#define KeyLedList_B   12
#define KeyLedList_D   16	
	
#define	KeyBtnNum   16
/********************************************************************************
* 常量定义
*********************************************************************************/
#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
const KEY_LED_CORRESPOND_TYPE s_stKeyLedList[] = 
{
	{KEY_13,		KEY_LED_1},
	{KEY_14,		KEY_LED_2},
	
	{KEY_22,		KEY_LED_3},
	{KEY_23,		KEY_LED_4},
	
	{KEY_31,		KEY_LED_5},
	{KEY_32,		KEY_LED_6},
	
	{KEY_35,		KEY_LED_7},
	{KEY_41,		KEY_LED_8},	

	{KEY_15,		KEY_LED_9},
	{KEY_21,		KEY_LED_10},
	
	{KEY_24,		KEY_LED_11},			
	{KEY_25,		KEY_LED_12},
	
	{KEY_44,		KEY_LED_13},			
	{KEY_12,		KEY_LED_14},
	
	{KEY_42,		KEY_LED_15},			
	{KEY_43,		KEY_LED_16},	

	{KEY_INVALID,KEY_LED_INVALID}	
};

const KEY_LED_CORRESPOND_TYPE s_stKeyLedList_D[] = 
{
	{KEY_42,		KEY_LED_1},
	{KEY_43,		KEY_LED_2},
	
	{KEY_13,		KEY_LED_3},
	{KEY_22,		KEY_LED_4},
	
	{KEY_14,		KEY_LED_5},
	{KEY_23,		KEY_LED_6},
	
	{KEY_15,		KEY_LED_7},
	{KEY_21,		KEY_LED_8},	

	{KEY_35,		KEY_LED_9},
	{KEY_41,		KEY_LED_10},
	
	{KEY_32,		KEY_LED_11},			
	{KEY_31,		KEY_LED_12},

	{KEY_24,		KEY_LED_13},			
	{KEY_25,		KEY_LED_14},
	
	{KEY_12,		KEY_LED_15},			
	{KEY_44,		KEY_LED_16},	

	{KEY_INVALID,KEY_LED_INVALID}	
};

#endif



/********************************************************************************
* 变量定义
*********************************************************************************/
static stKEY	s_stKeyQueue[KEY_QUEUE_CNT];	// 按键值队列
static u16	s_u16KeyQueueStart;	// 读取始地址
static u16	s_u16KeyQueueEnd;	// 存入始地址
static u16	s_u16KeyNum;	// 个数
OS_EVENT	 *g_stKeySem;//按键事件信号量
static u16 	s_u16KeyLongDownAndUpEnabled;	//按键长按下使能

extern u32 s_u32StartKeyFlag;
extern u32 s_u32SpecialFxKeyFlag3;
extern u32 s_u32SpecialFxKeyFlag2;
extern u32 s_u32SpecialFxKeyFlag1;

extern u16 s_u16RfidSendBufWrite;
extern u16 s_u16RfidSendBufRead;

/********************************************************************************
* 函数定义
*********************************************************************************/

/***************************************************************************************
** 函数名称:Key_Init
** 功能描述：key初始化
** 函数输入：无
** 函数输出：1：正常，0：异常
** 注意事项：
******************************************************************************************/
u32 KeyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* GPIOC Periph clock enable */
	RCC_AHBPeriphClockCmd(KEY_L0_GPIO_RCC 
							| KEY_L1_GPIO_RCC 
							| KEY_L2_GPIO_RCC 
							| KEY_L3_GPIO_RCC 
							| KEY_L4_GPIO_RCC 
							| KEY_R0_GPIO_RCC 
							| KEY_R1_GPIO_RCC 
							| KEY_R2_GPIO_RCC 
							| KEY_R3_GPIO_RCC 
							| KEY_R4_GPIO_RCC 
							| KEY_R5WKUP2_GPIO_RCC 
							|KEY_ZIBAO_GPIO_RCC,
							ENABLE);

	/* Configure KEY_L0 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = KEY_L0_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(KEY_L0_PORT, &GPIO_InitStructure);
	/* Set KEY_L0_PIN */
	GPIO_ResetBits(KEY_L0_PORT, KEY_L0_PIN);

	/* Configure KEY_L1 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = KEY_L1_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(KEY_L1_PORT, &GPIO_InitStructure);
	/* Set KEY_L1_PIN */
	GPIO_ResetBits(KEY_L1_PORT, KEY_L1_PIN);

	/* Configure KEY_L2 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = KEY_L2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(KEY_L2_PORT, &GPIO_InitStructure);
	/* Set KEY_L2_PIN */
	GPIO_ResetBits(KEY_L2_PORT, KEY_L2_PIN);

	/* Configure KEY_L3 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = KEY_L3_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(KEY_L3_PORT, &GPIO_InitStructure);
	/* Set KEY_L3_PIN */
	GPIO_ResetBits(KEY_L3_PORT, KEY_L3_PIN);

	/* Configure KEY_L4 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = KEY_L4_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(KEY_L4_PORT, &GPIO_InitStructure);
	/* Set KEY_L4_PIN */
	GPIO_ResetBits(KEY_L4_PORT, KEY_L4_PIN);
	
	/* Configure KEY_R0 in input pulldown mode */
	GPIO_InitStructure.GPIO_Pin = KEY_R0_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(KEY_R0_PORT, &GPIO_InitStructure);

	/* Configure KEY_R1 in input pulldown mode */
	GPIO_InitStructure.GPIO_Pin = KEY_R1_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(KEY_R1_PORT, &GPIO_InitStructure);

	/* Configure KEY_R2 in input pulldown mode */
	GPIO_InitStructure.GPIO_Pin = KEY_R2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(KEY_R2_PORT, &GPIO_InitStructure);

	/* Configure KEY_R3 in input pulldown mode */
	GPIO_InitStructure.GPIO_Pin = KEY_R3_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(KEY_R3_PORT, &GPIO_InitStructure);

	/* Configure KEY_R4 in input pulldown mode */
	GPIO_InitStructure.GPIO_Pin = KEY_R4_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(KEY_R4_PORT, &GPIO_InitStructure);

	/* Configure KEY_R5WKUP2_PIN in input pulldown mode */
	GPIO_InitStructure.GPIO_Pin = KEY_R5WKUP2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(KEY_R5WKUP2_PORT, &GPIO_InitStructure);

	/* Configure KEY_ZIBAO_PIN in input pulldown mode */
	GPIO_InitStructure.GPIO_Pin = KEY_ZIBAO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(KEY_ZIBAO_PORT, &GPIO_InitStructure);
	
	s_u16KeyQueueStart = 0;	// 读取始地址
	s_u16KeyQueueEnd = 0;	// 存入始地址
	s_u16KeyNum = 0;	// 个数
	s_u16KeyLongDownAndUpEnabled = 1;
	
	g_stKeySem = OSSemCreate(0);
	
	
	return 0x01;
}

/***************************************************************************************
** 函数名称:KeyIsValid
** 功能描述：判断按下键是否对应指示灯
** 函数输入：无
** 函数输出：1：有效，0：无效
** 注意事项：
******************************************************************************************/
#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
u32 KeyCorrespendLed(u32 u32KeyVal,u32 *pLedVal)
{
	u32 u32ReturnValue = 0x00;
	u32 u32I = 0x00;
	
	if(g_u32ID == B_type)
	{
		for(u32I = 0x00;u32I<KeyLedList_B;u32I++)
		{
			if((u32KeyVal&s_stKeyLedList[u32I].u32KeyNumb)==s_stKeyLedList[u32I].u32KeyNumb)
			{
				*pLedVal |= s_stKeyLedList[u32I].u32KeyLedNumb;
				u32ReturnValue = 0x01;
			}		
		}
	}
	else
	{
		for(u32I = 0x00;u32I<KeyLedList_D;u32I++)
		{
			if((u32KeyVal&s_stKeyLedList_D[u32I].u32KeyNumb)==s_stKeyLedList_D[u32I].u32KeyNumb)
			{
				*pLedVal |= s_stKeyLedList_D[u32I].u32KeyLedNumb;
				u32ReturnValue = 0x01;
			}		
		}
	
	}

	return u32ReturnValue;
	
}
#endif
/***************************************************************************************
** 函数名称:KeyIsValid
** 功能描述：判断按下键是否合法
** 函数输入：无
** 函数输出：1：有效，0：无效
** 注意事项：
******************************************************************************************/
static u32 KeyIsValid(u32 u32KeyVal)
{
	u32 u32ReturnValue = 0x01;
	
	switch (u32KeyVal)
	{
		case KEY_11:
			break;			
		case KEY_12:
			break;			
		case KEY_13:
			break;			
		case KEY_14:
			break;	
		case KEY_15:
			break;				
		case KEY_21:
			break;			
		case KEY_22:
			break;			
		case KEY_23:
			break;			
		case KEY_24:
			break;	
		case KEY_25:
			break;			
		case KEY_31:
			break;			
		case KEY_32:
			break;			
		case KEY_33:
			break;			
		case KEY_34:
			break;	
		case KEY_35:
			break;			
		case KEY_41:
			break;			
		case KEY_42:
			break;			
		case KEY_43:
			break;			
		case KEY_44:
			break;	
		case KEY_45:
			break;			
		case KEY_51:
			break;			
		case KEY_52:
			break;			
		case KEY_53:
			break;			
		case KEY_54:
			break;
		case KEY_55:
			break;	
		case KEY_71:
			break;			
		case KEY_72:
			break;
		case KEY_61:
			u32ReturnValue = 0x00;	
			break;			
		case KEY_62:
			u32ReturnValue = 0x00;
			break;			
		case KEY_63:
			u32ReturnValue = 0x00;
			break;			
		case KEY_64:
			u32ReturnValue = 0x00;
			break;	
		case KEY_65:
			u32ReturnValue = 0x00;
			break;				
		default:
			u32ReturnValue = 0x00;
	}

	if(u32ReturnValue == 0x00)
	{
		if((u32KeyVal!= KEY_INVALID)\
		&&((u32KeyVal&KEY_61)!= KEY_61)&&((u32KeyVal&KEY_62)!= KEY_62)\
		&&((u32KeyVal&KEY_63)!= KEY_63)&&((u32KeyVal&KEY_64)!= KEY_64)\
		&&((u32KeyVal&KEY_65)!= KEY_65))
			u32ReturnValue = 0x01;
	}
	
	return	u32ReturnValue;
}

/***************************************************************************************
** 函数名称:D版遥控器键盘转B版
** 功能描述：
** 函数输入：无
** 函数输出：无
** 注意事项：
******************************************************************************************/
static u32 D_typeKeyNum[KeyBtnNum]  = {0x08,0x80,0x10,0x20,0x4000,0x8000,0x800 ,0x400 ,0x100,0x200,0x2     ,0x40000 ,0x1000,0x2000   ,0x04     ,0x40     };
static u32 D_typeKeyNum1[KeyBtnNum] = {0x04,0x08,0x40,0x80,0x400 ,0x800 ,0x4000,0x8000,0x10 ,0x20 ,0x100   ,0x200   ,0x2000,0x1000   ,0x0400000,0x0800000};

static u32 key_Btnkey_D(u32 u32KeyVal)
{
	u32 u32key_DVal = 0;
	u8 i;
	u8 tmp[KeyBtnNum];
	u8 isOneNumFlag = 0;
	
	if(u32KeyVal == 0)
	{
		return 0;
	}
	memset(tmp,0,KeyBtnNum);
	if((u32KeyVal&(u32KeyVal-1)) == 0)
		isOneNumFlag = 1;
	for(i = 0;i<KeyBtnNum;i++)
	{
		if((u32KeyVal&D_typeKeyNum[i]) != 0)
		{
			u32KeyVal ^= D_typeKeyNum[i];
			tmp[i] = 1;
			if(isOneNumFlag)
				break;
		}
	}
	u32key_DVal = u32KeyVal;
	
	for(i = 0;i<KeyBtnNum;i++)
	{
		if(tmp[i] != 0)
		{
			u32key_DVal|= D_typeKeyNum1[i];
			if(isOneNumFlag)
				break;
		}
	}
	return u32key_DVal;
}


static u8 CERTIFY_flag = 0;
static u8 NQ_flag = 0;
/***************************************************************************************
** 函数名称:D版遥控器成组键和NQ键复用（仅适用参数设置）
** 功能描述：
** 函数输入：u32KeyVal：键值   u32Info：按键状态  1为按下
** 函数输出：无
** 注意事项：
******************************************************************************************/

u32 IsMenuLeftOrMenuQ(u32 u32KeyVal, u32 u32Info)
{

	if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_SET||InterfaceGetCtrlMode() == INTERFACE_MODE_SELF_SET)
	{
		if(u32KeyVal == KEY_52&& CERTIFY_flag == 0&&u32Info == 1)//上来按下确认键
		{
			CERTIFY_flag = 1;
		}
		else if(u32KeyVal == KEY_52 && CERTIFY_flag == 1&&u32Info == 1)  //设置完密码按下确认键
		{
			CERTIFY_flag = 2;
		}
		else if(u32KeyVal == KEY_52 && NQ_flag == 1 && CERTIFY_flag == 2&&u32Info == 1) //保存参数按下确认键
		{
			CERTIFY_flag = 3;
			NQ_flag = 0;
		}
		
		if((u32KeyVal&(u32)0x00010000)==(u32)0x00010000&&CERTIFY_flag == 2&&u32Info == 1)
		{
			u32KeyVal = (u32KeyVal^0x00010000)|0x00000008;
			NQ_flag = 1;
		}
		if((u32KeyVal&(u32)0x00020000)==(u32)0x00020000&&CERTIFY_flag == 2&&u32Info == 1)
		{
			u32KeyVal = (u32KeyVal^0x00020000)|0x00000080;
			NQ_flag = 1;
		}
		
		if((u32KeyVal&(u32)0x00010000)==(u32)0x00010000&&CERTIFY_flag == 1&&u32Info == 1)
		{
			CERTIFY_flag = 0;
		}
		if((u32KeyVal&(u32)0x00020000)==(u32)0x00020000&&CERTIFY_flag == 1&&u32Info == 1)
		{
			CERTIFY_flag = 0;
		}
		
		if((u32KeyVal&(u32)0x00010000)==(u32)0x00010000&&CERTIFY_flag == 3&&u32Info == 1)
		{
			u32KeyVal = (u32KeyVal^0x00010000)|0x00000008;
			CERTIFY_flag = 1;
			NQ_flag = 0;
		}
		if((u32KeyVal&(u32)0x00020000)==(u32)0x00020000&&CERTIFY_flag == 3&&u32Info == 1)
		{
			u32KeyVal = (u32KeyVal^0x00020000)|0x00000080;
			CERTIFY_flag = 1;
			NQ_flag = 0;
		}
		if(u32KeyVal == KEY_72)
		{
			CERTIFY_flag = 1;
			NQ_flag = 0;
		}
	}
	if(InterfaceGetCtrlMode() == INTERFACE_MODE_IDLE||InterfaceGetCtrlMode() == INTERFACE_MODE_SC_UNCERTIFIED)
	{	
		CERTIFY_flag = 0;
	}
	return u32KeyVal;  
} 
  
  
/***************************************************************************************
** 函数名称:AcceptKey
** 功能描述：新按键存入键盘队列
** 函数输入：无
** 函数输出：无
** 注意事项：
******************************************************************************************/
u32 AcceptKey(u32 u32KeyVal, u32 u32Info)
{
	INT8U u8Err;
	uint8_t tmp;
	u32 u32ReturnValue = 0x00;
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
		OS_CPU_SR  cpu_sr = 0;
	#endif
	
	if (!KeyIsValid(u32KeyVal))
	{
		return u32ReturnValue;
	}
	
	//关全局中断？

	if (s_u16KeyNum < KEY_QUEUE_CNT)
	{
		//自保键
//		if(g_u32ID != B_type)
//		{		
//			if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_SELF_CONTROL||InterfaceGetCtrlMode() == INTERFACE_MODE_SC_ZIBAO_CONTROL)
//			{
//				if((u32KeyVal&(u32)KEY_CERTIFY)==(u32)KEY_CERTIFY&&	(s_u32StartKeyFlag != KEY_DOWN && s_u32SpecialFxKeyFlag3 != KEY_DOWN && s_u32SpecialFxKeyFlag1 != KEY_DOWN && s_u32SpecialFxKeyFlag2 != KEY_DOWN))  //KEY_52  确认键
//				{
//					u32KeyVal = (u32KeyVal^KEY_CERTIFY)|KEY_ZI_BAOCHI;
//				}
//			}
//		}	
			
		//+、-键对码时，又按下加减停止键，处理不及时操作
		if(getColiScNumStatus() > NOKEYDOWN)
		{
			if((u32KeyVal == KEY_INCREASE||u32KeyVal == KEY_DECREASE)&&u32Info == KEY_DOWN)
			{
				tmp = getColiScNumStatus();
				tmp++;
				setColiScNumStatus(tmp);
			}	
			else if((u32KeyVal == KEY_STOP_SHUTDOWN)&&u32Info == KEY_DOWN)
			{
				tmp = getColiScNumStatus();
				setColiScNumStatus((tmp++));
			}
			else
				return 0x00;
		}
		
		OS_ENTER_CRITICAL();			
		#ifndef HARDWARE_TEST	
		if(g_u32ID != B_type)
		{
			u32KeyVal = key_Btnkey_D(u32KeyVal);
		}
			
		#endif
		s_stKeyQueue[s_u16KeyQueueEnd].u32Key = u32KeyVal;

		s_stKeyQueue[s_u16KeyQueueEnd].u32Status = u32Info;
		s_u16KeyQueueEnd = (s_u16KeyQueueEnd+1) % KEY_QUEUE_CNT;
		s_u16KeyNum++;
		//开全局中断？
		OS_EXIT_CRITICAL();	
		
		u8Err = OSSemPost(g_stKeySem);
		if(u8Err == OS_ERR_NONE)
		{
			u32ReturnValue = 0x01;
		}
		else
		{
			u32ReturnValue = 0x00;	
		}
		
	}
	else//进行强制处理。2018.02.06
	{		
		s_u16KeyQueueEnd = 0x00;
		s_u16KeyQueueStart = 0x00;		
		g_stKeySem->OSEventCnt = 0x00;	
//		if(g_u32ID != B_type)
//		{		
//			if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_SELF_CONTROL||InterfaceGetCtrlMode() == INTERFACE_MODE_SC_ZIBAO_CONTROL)
//			{
//				if((u32KeyVal&(u32)KEY_CERTIFY)==(u32)KEY_CERTIFY&&	s_u32StartKeyFlag != KEY_DOWN)  //KEY_52  确认键
//				{
//					u32KeyVal = (u32KeyVal^KEY_CERTIFY)|KEY_ZI_BAOCHI;
//				}
//			}
//		}		
		
		OS_ENTER_CRITICAL();	
		#ifndef HARDWARE_TEST
		if(g_u32ID != B_type)
			s_stKeyQueue[s_u16KeyQueueEnd].u32Key = key_Btnkey_D(u32KeyVal);
		else	
		#endif
			s_stKeyQueue[s_u16KeyQueueEnd].u32Key = u32KeyVal;
	
		s_stKeyQueue[s_u16KeyQueueEnd].u32Status = u32Info;
		s_u16KeyQueueEnd = (s_u16KeyQueueEnd+1) % KEY_QUEUE_CNT;
		s_u16KeyNum	= 0x01;
		
		OS_EXIT_CRITICAL();	
		
		u8Err = OSSemPost(g_stKeySem);
		if(u8Err == OS_ERR_NONE)
		{
			u32ReturnValue = 0x01;
		}
		else
		{		
			u32ReturnValue = 0x00;	
		}			
	}		
	return u32ReturnValue;	
}

/***************************************************************************************
** 函数名称:GetKey
** 功能描述：从按键队列取键
** 函数输入：stKey:数据指针
** 函数输出：1:正常，0:异常
** 注意事项：
******************************************************************************************/
u32 GetKey(stKEY *stKey)
{
	u32 u32ReturnValue = 0x00;
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
		OS_CPU_SR  cpu_sr = 0;
	#endif
	
	//关全局中断？
	OS_ENTER_CRITICAL();	
	if (s_u16KeyNum)
	{		
		stKey->u32Key = s_stKeyQueue[s_u16KeyQueueStart].u32Key;
		stKey->u32Status = s_stKeyQueue[s_u16KeyQueueStart].u32Status;
		s_u16KeyQueueStart = (s_u16KeyQueueStart+1) % KEY_QUEUE_CNT;
		s_u16KeyNum--;
		u32ReturnValue = 0x01;
	}
	else
	{
		u32ReturnValue = 0x00;
	}
	
	//开全局中断？
	OS_EXIT_CRITICAL();
	
	return u32ReturnValue;
}

/***************************************************************************************
** 函数名称:InquireKey
** 功能描述：从按键队列查询按键，不从按键队列中清除
** 函数输入：stKey:数据指针
** 函数输出：1:正常，0:异常
** 注意事项：
******************************************************************************************/
u16 InquireKey(stKEY *stKey)
{
	if (s_u16KeyNum)
	{
		stKey->u32Key = s_stKeyQueue[s_u16KeyQueueStart].u32Key;
		stKey->u32Status = s_stKeyQueue[s_u16KeyQueueStart].u32Status;
		return (1);
	}
	else
		return (0);
}

/***************************************************************************************
** 函数名称:GetKeyNum
** 功能描述：取按键队列计数?
** 函数输入：stKey:数据指针
** 函数输出：1:正常，0:异常
** 注意事项：
******************************************************************************************/
u16 GetKeyNum(void)
{
	return (s_u16KeyNum);
}

/***************************************************************************************
** 函数名称:KeyEmpty
** 功能描述：清空按键队列
** 函数输入：无
** 函数输出：1:正常，0:异常
** 注意事项：
******************************************************************************************/
void KeyEmpty(void)
{
	stKEY stTmpKey;

	while (GetKey(&stTmpKey));
}


/***************************************************************************************
** 函数名称:ScanKey_task
** 功能描述：矩阵键盘扫描任务
** 函数输入：无
** 函数输出：无
** 注意事项：
******************************************************************************************/
void KeyScanProc(void)
{
	static u32 u32KeyActive=0;	//有效键值
	static u32 u32KeyLast=0;	//准有效键值
	static u16 u16KeyStatus;	//抬起、按下、持续按下
	static u16 u16DownTimer=0xffff;	//按下去抖延时
	static u16 u16UpTimer=0xffff;	//抬起去抖延时
	static u16 u16DownDelayTimer=0xFFFF;	//按键按下计时
	u32	u32KeyNowTemp = 0x00,u32Temp1 = 0x00;	// 临时键值
	u16 u16ReadVal = 0x00;	// 临时端口值
	u16 u16i = 0x00;
	#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
		u32 u32ReturnValue = 0x00;
		u32 u32LedValue = 0x00;
	#endif
	static u16 u16DownDelayTimerBackup = 0xFFFF;//u16DownDelayTimer备份
	static u16 u16KeyStatusBackup = 0x00;//u16KeyStatus备份
	
   	 // 准备读数据
	u32KeyNowTemp = 0;
	for (u16i = 0; u16i < 5; u16i++)
	{
		for (u16ReadVal = 0; u16ReadVal < 5; u16ReadVal++)
		{
			if(u16i == 0x00)
			{
				/* Set KEY_L0_PIN */
				GPIO_SetBits(KEY_L0_PORT,KEY_L0_PIN);
				/* ReSet KEY_L1_PIN,KEY_L2_PIN,KEY_L3_PIN,KEY_L4_PIN */
				GPIO_ResetBits(KEY_L1_PORT,KEY_L1_PIN);
				GPIO_ResetBits(KEY_L2_PORT,KEY_L2_PIN);
				GPIO_ResetBits(KEY_L3_PORT,KEY_L3_PIN);
				GPIO_ResetBits(KEY_L4_PORT,KEY_L4_PIN);		
			}		
			else if(u16i == 0x01)
			{
				/* Set KEY_L1_PIN */
				GPIO_SetBits(KEY_L1_PORT,KEY_L1_PIN);
				/* RESet KEY_L0_PIN,KEY_L2_PIN,KEY_L3_PIN,KEY_L4_PIN */
				GPIO_ResetBits(KEY_L0_PORT,KEY_L0_PIN);
				GPIO_ResetBits(KEY_L2_PORT,KEY_L2_PIN);
				GPIO_ResetBits(KEY_L3_PORT,KEY_L3_PIN);
				GPIO_ResetBits(KEY_L4_PORT,KEY_L4_PIN);				
			}
					
			else if(u16i == 0x02)
			{
				/* Set KEY_L2_PIN */
				GPIO_SetBits(KEY_L2_PORT,KEY_L2_PIN);
				/* Set  KEY_L0_PIN,KEY_L1_PIN,KEY_L3_PIN,KEY_L4_PIN */
				GPIO_ResetBits(KEY_L0_PORT,KEY_L0_PIN);
				GPIO_ResetBits(KEY_L1_PORT,KEY_L1_PIN);
				GPIO_ResetBits(KEY_L3_PORT,KEY_L3_PIN);
				GPIO_ResetBits(KEY_L4_PORT,KEY_L4_PIN);		

			}		
			else if(u16i == 0x03)
			{
				/* Reset KEY_L3_PIN */
				GPIO_SetBits(KEY_L3_PORT,KEY_L3_PIN);
				/* Set KEY_L0_PIN,KEY_L1_PIN,KEY_L2_PIN,KEY_L4_PIN */
				GPIO_ResetBits(KEY_L0_PORT,KEY_L0_PIN);
				GPIO_ResetBits(KEY_L1_PORT,KEY_L1_PIN);
				GPIO_ResetBits(KEY_L2_PORT,KEY_L2_PIN);
				GPIO_ResetBits(KEY_L4_PORT,KEY_L4_PIN);
			}		
			else
			{
				/* Reset KEY_L4_PIN */
				GPIO_SetBits(KEY_L4_PORT,KEY_L4_PIN);
				/* Set KEY_L0_PIN,KEY_L1_PIN,KEY_L2_PIN,KEY_L3_PIN */
				GPIO_ResetBits(KEY_L0_PORT,KEY_L0_PIN);
				GPIO_ResetBits(KEY_L1_PORT,KEY_L1_PIN);
				GPIO_ResetBits(KEY_L2_PORT,KEY_L2_PIN);
				GPIO_ResetBits(KEY_L3_PORT,KEY_L3_PIN);
			}		
		}
		
		/*Read the KEY_H1~8_PIN input data port*/
		u16ReadVal = 0;
		if (GPIO_ReadInputDataBit(KEY_R0_PORT,KEY_R0_PIN) != 0)
			u16ReadVal |= 0x0001;
		if (GPIO_ReadInputDataBit(KEY_R1_PORT,KEY_R1_PIN) != 0)
			u16ReadVal |= 0x0002;
		if (GPIO_ReadInputDataBit(KEY_R2_PORT,KEY_R2_PIN) != 0)
			u16ReadVal |= 0x0004;
		if (GPIO_ReadInputDataBit(KEY_R3_PORT,KEY_R3_PIN) != 0)
			u16ReadVal |= 0x0008;
		if (GPIO_ReadInputDataBit(KEY_R4_PORT,KEY_R4_PIN) != 0)
			u16ReadVal |= 0x0010;
		/*The whole 32keys are scanned*/
		u32Temp1 = (u32)u16ReadVal;
		u32Temp1 <<= (u16i *5);
		u32KeyNowTemp |= u32Temp1;
	}
	
	if (GPIO_ReadInputDataBit(KEY_ZIBAO_PORT,KEY_ZIBAO_PIN) == Bit_SET)		
	{
		u32Temp1 = KEY_71;
		u32KeyNowTemp |= u32Temp1;
	}	
	
	if (GPIO_ReadInputDataBit(KEY_R5WKUP2_PORT,KEY_R5WKUP2_PIN) == Bit_SET)		
	{
		u32Temp1 = KEY_72;
		u32KeyNowTemp |= u32Temp1;
	}
	
	/* ReSet the KEY_L0~4_PIN */
	GPIO_ResetBits(KEY_L0_PORT,KEY_L0_PIN);
	GPIO_ResetBits(KEY_L1_PORT,KEY_L1_PIN);
	GPIO_ResetBits(KEY_L2_PORT,KEY_L2_PIN);
	GPIO_ResetBits(KEY_L3_PORT,KEY_L3_PIN);
	GPIO_ResetBits(KEY_L4_PORT,KEY_L4_PIN);

	if(u32KeyNowTemp == 0x00)		//=0:松开
	{
		if (u32KeyLast)	//Some keys were pressed
		{
			if (!u16UpTimer)	//time over
			{
				//抬起键熄灭键LED
				if (KeyIsValid(u32KeyLast))
				{
					//接受按键抬起
					AcceptKey((u32KeyActive != 0) ? u32KeyActive : u32KeyLast, 0);	//防止在复合按键中，刚按下即抬起的情况
					#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
						u32LedValue = 0x00;
						u32ReturnValue = KeyCorrespendLed((u32KeyActive != 0) ? u32KeyActive : u32KeyLast,&u32LedValue);
						if(u32ReturnValue != 0x00)
						{
							if(g_u32ID == B_type||(g_u32ID != B_type &&(InterfaceGetCtrlMode() == INTERFACE_MODE_IDLE)))
								KeyLedOnOff(0x00,u32LedValue);
						}
					#endif
					s_u16KeyLongDownAndUpEnabled = 1;
					u16KeyStatusBackup = KEY_UP;
				}
				//复位按键相关变量
				u32KeyLast = 0;
				u32KeyActive = 0;
				u16KeyStatus = 0;
				u16DownTimer = 0xffff;
				u16UpTimer = 0xffff;
				u16DownDelayTimer = 0xFFFF;
			}
			else if (u16UpTimer == 0xffff)	//to time
			{
				u16UpTimer = 3;	//键抬起计时30ms
			}
		}
	}
	else	//键按下
	{
		u16UpTimer = 0xffff;	//关闭键抬起计时	
		if( (((u32KeyNowTemp | u32KeyLast) != u32KeyLast)&&(u32KeyNowTemp >= u32KeyLast))||(((u32KeyLast >= u32KeyNowTemp)&&(u32KeyLast|u32KeyNowTemp ) != u32KeyNowTemp)) )//是否有其它键按下
		{
			//?接受按键抬起。不是合法按键，认为原键抬起
			if((KeyIsValid(u32KeyActive) && (u16DownDelayTimerBackup >= 3)) && (u16KeyStatusBackup >= KEY_DOWN))
			{
				if(((u32KeyActive&u32KeyNowTemp) != u32KeyNowTemp)&&((u32KeyActive&u32KeyNowTemp) != u32KeyActive))
				{
					s_u16KeyLongDownAndUpEnabled = 1;
					AcceptKey(u32KeyActive, 0);
					u16KeyStatusBackup = KEY_UP;
					#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
						u32LedValue = 0x00;
						u32ReturnValue = KeyCorrespendLed(u32KeyActive,&u32LedValue);
						if(u32ReturnValue != 0x00)
							if(g_u32ID == B_type||(g_u32ID != B_type &&(InterfaceGetCtrlMode() == INTERFACE_MODE_IDLE)))
								KeyLedOnOff(0x00,u32LedValue);
					#endif				
					u32KeyActive = 0;				//清有效键
					u16DownDelayTimer = 0x00;	
					u16DownDelayTimerBackup = u16DownDelayTimer;					
				}
				else
				{
					if(KeyIsValid(u32KeyLast))
					{	
						u16DownDelayTimer  = 0xFFFF;		
					}
				}				
			}
			else
			{
				if(KeyIsValid(u32KeyLast))
				{	
					u16DownDelayTimer  = 0xFFFF;		
				}
			}	

			if(u32KeyNowTemp >= u32KeyLast)
				u32KeyLast |= u32KeyNowTemp;		//新键按下
			else//抬起了至少两个中的一个按键
			{
				u32KeyLast &= u32KeyNowTemp;		//新键按下
			}
			u16DownTimer = 3;		//去抖计时 30ms			
			u16KeyStatus = 0;		//清按下持续按下	
	    }		
		else
		{
			if (u32KeyNowTemp == u32KeyLast)	//按下键没有变化
			{
				if(u16KeyStatus == KEY_DOWN)
				{
					u16DownDelayTimer ++;
					u16DownDelayTimerBackup = u16DownDelayTimer;
				}
				if((u16DownDelayTimer >= KEY_LONGDOWN_TIME_SET)&& (u16KeyStatus == 1)&& KeyIsValid(u32KeyActive) &&s_u16KeyLongDownAndUpEnabled)
				{
					u16KeyStatus++;
					AcceptKey(u32KeyActive, u16KeyStatus);
					u16KeyStatusBackup = u16KeyStatus;
					#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
						u32LedValue = 0x00;
						u32ReturnValue = KeyCorrespendLed(u32KeyActive,&u32LedValue);
						if((u32ReturnValue != 0x00)&&(u16KeyStatus == KEY_DOWN))
						{
							if(g_u32ID == B_type||(g_u32ID != B_type &&(InterfaceGetCtrlMode() == INTERFACE_MODE_IDLE)))
								KeyLedOnOff(u32LedValue,0x00);
						}
						else
						{
							//KeyLedOnOff(0x00,KEY_LED_ALL);
						}
					#endif					
					s_u16KeyLongDownAndUpEnabled = 0;		
				}
				if (!u16DownTimer && (u16DownDelayTimer == 0xFFFF))	//去抖结束
				{
					//test zzw增加组合键抬起功能
					if((u32KeyActive != 0) && (u32KeyLast != 0)&& (u32KeyActive > u32KeyLast))
					{
						u32KeyActive = u32KeyActive^ u32KeyLast;
						u16KeyStatus = KEY_UP;
						AcceptKey(u32KeyActive, u16KeyStatus);
					}
					
					u32KeyActive = u32KeyLast;
					//键按下或连续按下
					if (u16KeyStatus < KEY_DOWN)
						u16KeyStatus++;
					//接受该键
					if ((u16KeyStatus == KEY_DOWN)&& KeyIsValid(u32KeyActive))
					{
						AcceptKey(u32KeyActive, u16KeyStatus);
						u16KeyStatusBackup = u16KeyStatus;
						#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
							BuzzerOn(BEEP_KEY_PRESSED);			//按键音
						#endif
						#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
							u32LedValue = 0x00;
							u32ReturnValue = KeyCorrespendLed(u32KeyActive,&u32LedValue);
							if((u32ReturnValue != 0x00)&&(u16KeyStatus == KEY_DOWN))
							{
//								KeyLedOnOff(u32LedValue,0x00);       //jhy
							}
							else
							{
								//KeyLedOnOff(0x00,KEY_LED_ALL);
							}							
						#endif	
						
						s_u16KeyLongDownAndUpEnabled = 1;					
					}
					//重新去抖计时
					u16DownTimer = 3;
					u16DownDelayTimer = 3;
				}
			}
			else	//有键抬起
			{
				//?接受按键抬起
				u32KeyLast = u32KeyNowTemp;	//新键
				u32KeyActive = 0;			//清有效键
				u16KeyStatus = 0;		//清键按下和持续按下
				u16DownTimer = 3;
			}
		}
	}

	//调整按下去抖计时
	if ((u16DownTimer > 0) && (u16DownTimer != 0xffff))
		u16DownTimer--; 
	//调整抬起去抖计时
	if ((u16UpTimer > 0) && (u16UpTimer != 0xffff))
		u16UpTimer--; 
}

