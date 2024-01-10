/********************************************************************************
* 文件名：	interface.c
* 作者：	马如意   
* 版本：   	V1.0
* 日期：    2015.05.15  
* 功能描述: 本模块是遥控器人机交互界面的主模块。主要有：开机处理、按键处理、界面显
* 			示、蜂鸣器处理等。
*			 
* 修改说明：   
*
*       >>>>  在工程中的位置  <<<<
*             3-应用层
*         √  2-协议层
*             1-硬件驱动层
*********************************************************************************
* @copy
* <h2><center>&copy; COPYRIGHT 天津华宁电子有限公司 研发中心 软件部</center></h2>
*********************************************************************************/
/********************************************************************************
* .h头文件
*********************************************************************************/
#ifndef __INTERFACE_C__
#define __INTERFACE_C__

/********************************************************************************
* .h头文件
*********************************************************************************/
#include "includes.h"
#include "InterfaceApp.h"

/********************************************************************************
* #define宏定义
*********************************************************************************/
/**定义电池电量与信号强度的符号类型***/
enum
{
	//电池电量所对应的特殊符号
	INTERFACE_SYMBOL_NONE	= 0,//无任何字符
	INETRFACE_BATTERY_SYMBOL_0 , 
	INETRFACE_BATTERY_SYMBOL_20, 
	INETRFACE_BATTERY_SYMBOL_40, 
	INETRFACE_BATTERY_SYMBOL_60, 
	INETRFACE_BATTERY_SYMBOL_80, 
	INETRFACE_BATTERY_SYMBOL_100, 
	INETRFACE_BATTERY_SYMBOL_WRONG,

	/**以下为天线所对应的特殊符号**/
	INETRFACE_RFIDSIGNAL_SYMBOL_0 , 
	INETRFACE_RFIDSIGNAL_SYMBOL_20, 
	INETRFACE_RFIDSIGNAL_SYMBOL_40, 
	INETRFACE_RFIDSIGNAL_SYMBOL_60, 
	INETRFACE_RFIDSIGNAL_SYMBOL_80, 
	INETRFACE_RFIDSIGNAL_SYMBOL_100, 
	INETRFACE_RFIDSIGNAL_SYMBOL_WRONG	
};

/**定义屏幕刷新类型***/
typedef enum
{
	INTERFACE_SCR_IDLE = 0,
	INTERFACE_SCR_AWAKE,
	INTERFACE_SCR_PROTECED,
	INTERFACE_SCR_CLOSED,
	INTERFACE_SCR_MAX
}INTERFACE_SCR_TYPE;

/********************************************************************************
* 常量定义
*********************************************************************************/

#define INVALID_ACTION    (0)
#define	INTERFACE_TASK_TIME_SET				10//任务执行周期

#define	INTERFACE_PWR_TIME_SET				2000//唤醒延时
#define	INTERFACE_INTERCTRL_TIMER_MAX		30000
#define	INTERFACE_SCR_TIMER_STOP			0xFFFFFFFF//屏保定时器停止计数
#define	INTERFACE_NOTE_TIMER_STOP			0xFFFFFFFF//显示提示信息的最大时间
#define	INTERFACE_CTRL_TIMER_STOP			0xFFFFFFFF//主控计时停止计数
#define	INTERFACE_COLIBRATE_TIMER_STOP		0xFFFFFFFF//对码计时停止计数
#define	INTERFACE_AUTOSHUTDOWN_TIMER_STOP	0xFFFFFFFF//自动关机定时器停止计数

#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
	#define	INTERFACE_PRGSET_TIMER_SET		120000//进行参数设置以及程序传输的超时时间
#endif

/**成组动作的默认动作字**/
#if (INTERFACE_DISPLAY_GRP_LAJIA_FLAG	== ENABLED)
	#define	INTERFACE_GRP_ACTION_DEFAULT		INTERFACE_ACTION_GRP_LA_JIA
#else
	#define	INTERFACE_GRP_ACTION_DEFAULT		INTERFACE_ACTION_GRP_YI_JIA
#endif

/**界面显示动作名称的最大值**/
#define	INTERFACE_DISPLAY_ACTION_MAX		3

SIGNAL_ACTION_HEART Heart = {0};
/**电量所对应的电池电量显示符号定义(与电池电量状态一一对应)**/
static const u16 c_u16InterfaceBatterSymbol[] = 
{
	INTERFACE_SYMBOL_NONE,//无任何字符
	INETRFACE_BATTERY_SYMBOL_0, 
	INETRFACE_BATTERY_SYMBOL_20,
	INETRFACE_BATTERY_SYMBOL_40, 
	INETRFACE_BATTERY_SYMBOL_60, 
	INETRFACE_BATTERY_SYMBOL_80, 
	INETRFACE_BATTERY_SYMBOL_100,
	INETRFACE_BATTERY_SYMBOL_WRONG
};

/**信号强度所对应的信号强度显示符号定义(与信号强度状态一一对应)**/
static const u16 c_u16InterfaceRfidSignalSymbol[] = 
{
	INTERFACE_SYMBOL_NONE,//无任何字符
	INETRFACE_RFIDSIGNAL_SYMBOL_0  , 
	INETRFACE_RFIDSIGNAL_SYMBOL_20, 
	INETRFACE_RFIDSIGNAL_SYMBOL_40, 
	INETRFACE_RFIDSIGNAL_SYMBOL_60, 
	INETRFACE_RFIDSIGNAL_SYMBOL_80, 
	INETRFACE_RFIDSIGNAL_SYMBOL_100, 
	INETRFACE_RFIDSIGNAL_SYMBOL_WRONG	
};

const u8 u8OperateTittle[13]="动作信息";
static const u8  u8OperatInfoMsg[INTERFACE_ACTION_ENUM_MAX][17] =
{
	/*无效*/
	"",
	/**以下为按键信息**/
	"左邻架键",
	"右邻架键",	
	"左成组键",
	"右成组键",	
	"菜单设置键",
	"启动键",
	"确认键",
	"停止键",	
	"菜单上键",
	"菜单下键",		
	"菜单左键",
	"菜单右键" ,
	"菜单N/C键",
	"菜单Q/D键",		
	"SHIFT键",	
	
	//"菜单菜单菜单菜单"
	"1键",
	"2键",
	"3键",
	"4键",
	"5键",
	"6键",
	"7键",
	"8键",
	"9键",
	"0键",
	
	#if  (KEY_AUTO_PRESS_PROC_FLAG	!=	DISABLED)
		"自动补压键",
	#endif
};
	
/**运行信息**/
const u8 u8NoteTittle[]="运行状态";
static const u8 u8RunInfoMsg[INTERFACE_NOTE_ENUM_MAX][17] =
{
	/*无效*/
 	"无效",	
	"未对码",
	"设置WL编号",
	"超范围",
	"停止设置",
	"编号设置成功",
	
	"正在对码",
	"对码失败",
	"对码停止",
	"对码成功",
	"解除对码",
	"被动解除对码",
	
	"请按操作键",
	"按键无效",

	#if (BOARD_BATTERY_TEST == BOARD_BATTERY_COULOME_NUMBER)
		"充电中",//会携带电池电量的百分比参数
	#else
		"充电中",
	#endif
	
	"电量低,请充电!",
	"电量过低,10s关闭",
	"电量过低,9s关闭",
	"电量过低,8s关闭",
	"电量过低,7s关闭",
	"电量过低,6s关闭",
	"电量过低,5s关闭",
	"电量过低,4s关闭",
	"电量过低,3s关闭",
	"电量过低,2s关闭",
	"电量过低,1s关闭",	
	"电池故障",
	"充电已完成",
	"充电线缆已断开",
	"无操作,自动关机",
	
	"支架菜单设置",
	"支架单架控制",
	"支架邻架控制",	
	"支架成组控制",	

	"无应答",
	"按键接受",
	"按键拒绝",
	"按键无效",
	"按键抬起",
	"按键按下",
	"按键长按",
	"无应答,解除对码",
	"请确认",

	"shift按下",
	"shift抬起",
	
	"即将关机",
	"无线数据发送失败",

	"未使能",
	"无任何动作",
	"无执行时间",

	"预警",
	"动作中",
	"降移已完成",
	"降移已停止",
	
	"中间架手持终端",//"华宁手持终端"	
	"华宁手持终端",

	#if (ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)
		"自保被禁止",
		"动作被禁止",
		"自保停止",
		"动作超范围",
		"某动作停止",		
	#endif

	#if (GRP_LALIU_BUTTONUP_STOP_FLAG != DISABLED)
		"成组拉溜启动",
		"成组拉溜停止",
	#endif

	#if (KEY_AUTO_PRESS_PROC_FLAG != DISABLED)
		"自动补压允许",
		"自动补压禁止",
		"设置失败",
		"设置成功",
	#endif	
	
	#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
		"开始搜索设备",
		"搜索失败",
		"搜索停止",
		"搜索成功",
		
		"请勿动",
		"即将重启",
		"已完成",
		"接收失败",
		
		"停止发送",
		"停止接收",
		"无线模块异常",
		"请靠近",
		"手动模式",
		"自动模式",
		"请启动",
	#endif
};
//zzw
//
ActionMess ActionInfoMsg[INTERFACE_KEY_TOGETHER_MAX] = {0};
u8 RuningActionCnt = 0;
extern ACTION_INFO ActionInformation ;

/********************************************************************************
* 变量定义
*********************************************************************************/
OS_EVENT    *g_stSemInterface;//界面显示信号量
static INTERFACE_CONTROL_MODE_TYPE	s_stInterfaceControlMode;//界面控制模式
static u32 s_u32MainUpdateMethod;//界面显示方式
static INTERFACE_ACTION_TYPE s_stInterfaceActionName[INTERFACE_KEY_TOGETHER_MAX];
static u64 s_u64InterfaceRfidDataBackup;//上一次发送数据的备份
	
static u8 s_u8NoteItem[INTERFACE_NOTE_ENUM_MAX][17];//运行信息显示条目
static u16 s_u16NoteItemWrite;//运行信息显示条目设置指针
static u16 s_u16NoteItemRead;//运行信息显示条目读取指针

static INTERFACE_SCR_TYPE s_stScrAwake;//唤醒屏幕标志。ENABLE/DISABLE。
static u32 s_u32InterfaceColibrateTimer;//对码计时.0xFFFFFF:停止计数
static u32 s_u32InterfaceControlTimer;//屏幕进行休眠计时器.0xFFFFFF:停止计数
static u32 s_u32ScrPrtectedTimer;//屏幕进行休眠计时器.0xFFFFFF:停止计数
static u32 s_u32NoteShowTimer;//屏幕显示提示信息时的计时

static u32 s_u32ShiftKeyEnableFlag;//按键使能标志
static u32 s_u32AllKeyStatus;//所有按键的状态，按下或抬起

static u32 s_u32ScCtrlKeyTimer;//支架控制器被控状态下，按键延时显示的时间。超过此时间，将退回本架控制状态
static u32 s_u32SelfSetIntfState = INTERFACEMENU_STATE_MENU_START;//设置参数状态定义

static u32 s_u32AutoShutdownTimer;//自动关机计时
u32 s_u32StartKeyFlag= KEY_UP;

static u32 s_u32ScGrpKeyValue = 0x00;		//消除键盘灯在成组时按住某键再按其他键会导致灯异常的现象  2019.08.29 parry
static u32 s_u32ScGrpShiftKeyValue = 0x00;
static u8  s_u8ScGrpInvalidActionKeyNum = 0x00;


static u8 s_u8pleaseNearFlag = 0;

//static u32 KEY_SHENG_ZHU;		//动作需要根据u16KeySetActionNameRam中的按键布局来赋值	2020.01.15 parry			
//static u32 KEY_JIANG_ZHU;	
//static u32 KEY_TUILIU;
//static u32 KEY_YIJIA;
//static u32 KEY_SHEN_SHENSUOLIANG;

//static u32 KEY_SHOU_SHENSUOLIANG;
//static u32 KEY_SHEN_BAILIANG;
//static u32 KEY_SHOU_BAILIANG;
//static u32 KEY_SHEN_HUBANG;
//static u32 KEY_SHOU_HUBANG;

//static u32 KEY_SHEN_CEHU;
//static u32 KEY_SHOU_CEHU;
//static u32 KEY_TAI_DIZUO;
//static u32 KEY_JIANG_DIZUO;
//static u32 KEY_SHEN_HUBANG2;		

//static u32 KEY_SHOU_HUBANG2;
//static u32 KEY_WEILIANG_SHEN;
//static u32 KEY_WEILIANG_SHOU;
//static u32 KEY_SHEN_CHABAN;		
//static u32 KEY_SHOU_CHABAN;

//static u32 KEY_SHENG_HOUZHU;
//static u32 KEY_JIANG_HOUZHU;
//static u32 KEY_PENWU;	
//static u32 KEY_FANGMEI_PENWU;	
//static u32 KEY_SONG_HOULIU;

//static u32 KEY_LA_HOULIU;
//static u32 KEY_SHEN_DITIAO;
//static u32 KEY_SHOU_DITIAO;
//static u32 KEY_QIANLIANG_SHEN;
//static u32 KEY_QIANLIANG_SHOU;

//static u32 KEY_FANCHONG1;	
//static u32 KEY_FANCHONG2;	
//static u32 KEY_SHEN_YANCE;
//static u32 KEY_SHOU_YANCE;	
//static u32 KEY_SHEN_HUBANG3;

//static u32 KEY_SHOU_HUBANG3;	
//static u32 KEY_SHEN_PINGHENG;
//static u32 KEY_SHOU_PINGHENG;
//static u32 KEY_SHENG_TIAOLIU;
//static u32 KEY_JIANG_TIAOLIU;

//static u32 KEY_SHORTCUT_F1;
//static u32 KEY_SHORTCUT_F2;
//static u32 KEY_SHENG_QIANZHU;		
//static u32 KEY_JIANG_QIANZHU;	
//static u32 KEY_SHEN_DINGCE;

//static u32 KEY_SHOU_DINGCE;

static u16 fastColiearateScNum = 0;

static u8 u8FastColiearateScNumFlag = 0;

u16 Iwdg_Interface = 0;

u8 u8SwitchColiScNumStatus = NOKEYDOWN;//按下加减键切换对码架时，为了能快速响应再次按下加减键标志位

//extern u16 u16KeySetActionNameRam[LOGIC_KEYSET_ACTIONNUMB_MAX];
//extern u16 u16KeyFilmNameRam;
extern stEDIT_PAR stEdit;

u16 KEY_SC_GRP_OFF_LED; 	//成组模式下第一功能中不能成组的键的灯值	2020.04.27  parry
u16 KEY_SC_GRP_SHIFT_OFF_LED; 	//成组模式下第二功能中不能成组的键的灯值

#if (SPECIAL_FX_ENABLE_FLAG != DISABLED)
	#if (SPECIAL_F1_ENABLE_FLAG != DISABLED)
		u32 s_u32SpecialFxKeyFlag1= KEY_UP;
	#endif

	#if (SPECIAL_F2_ENABLE_FLAG != DISABLED)
		u32 s_u32SpecialFxKeyFlag2= KEY_UP;
	#endif

	#if (SPECIAL_F3_ENABLE_FLAG != DISABLED)
	 u32 s_u32SpecialFxKeyFlag3= KEY_UP;
	#endif

	#if (SPECIAL_F4_ENABLE_FLAG != DISABLED)
		static u32 s_u32SpecialFxKeyFlag4= KEY_UP;
	#endif
#endif

#if (ZIBAO_CTRL_ENABLE_FLAG != DISABLED)
	static u32 s_u32ZibaoActionKeyBackup;
#endif

#if  (KEY_AUTO_PRESS_PROC_FLAG	!=	DISABLED)
	static u32 s_u32AutoPressKeyFlag = KEY_UP;
#endif

#if (GRP_LALIU_BUTTONUP_STOP_FLAG != DISABLED)
	static u32 s_u32GrpLaliuKeyFlag = KEY_UP;
#endif

#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
	static u32 s_u32DownloadRemainTimer;
	static u32 s_u32DownloadRemainTimerBackup;
	
	extern OS_STK  RfidMng_task_stk[RFIDMNG_TASK_STK_SIZE];			//开辟任务堆栈
	
#endif		


//u32 timestest = 0;
//u32  timesnoack = 0;
//extern OS_EVENT *s_stKeyTest ;   //串口接收信号量

/********************************************************************************
* 函数定义
*********************************************************************************/
static u32 InterfaceModeInintProc(stKEY	sNewKey);
static u32 InterfaceModeIdleProc(stKEY	sNewKey);
static u32 InterfaceModeSelfSetProc(stKEY	sNewKey);
u64 InterfaceTakeOutReverseKey(INTERFACE_CONTROL_MODE_TYPE stMode,u32 *pKeyValue);

static u32 InterfaceModeScSetProc(stKEY	sNewKey);

#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
	static u32 InterfaceModeScUncertifiedProc(stKEY	sNewKey);	
#endif

#if (QUICK_KEY_CTRL_ENABLE_FLAG	!= DISABLED)
	static u32 InterfaceModeScSelfQuickProc(stKEY sNewKey);	
#endif

static u32 InterfaceModeScSelfControlProc(stKEY	sNewKey);	

#if (LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG !=	DISABLED)
	static u32 InterfaceModeScNeighborControlProc(stKEY	sNewKey);
#endif

#if (GRP_CTRL_ENABLE_FLAG != DISABLED)
	static u32 InterfaceModeScGrpControlProc(stKEY	sNewKey);
#endif

#if (SET_YKQ_PRGPT_USE_FLAG != DISABLED)
	static u32 InterfaceModeYkqSelfControlProc(stKEY	sNewKey);
	static u32 InterfaceModeYkqSetControlProc(stKEY	sNewKey);
#endif

#if (ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)
	static u32 InterfaceModeScZibaoControlProc(stKEY	sNewKey);
#endif
static u32 InterfaceModeShutdownProc(stKEY	sNewKey);

static u32 InterfaceModeInchargeDisplayProc(stKEY	sNewKey);

#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
	static u32 InterfaceModeSelfRecvTransPrgProc(stKEY	sNewKey);
#endif	

static u32 InterfaceModeWLUnConnectProc(stKEY	sNewKey);
static u32 InterfaceModeActionInfoSyn(stKEY	sNewKey);	//zzw
/*INTERFACE状态模式下按键处理函数指针数组*/
u32 (*InterfaceModeKeyProcFuncs[INTERFACE_MODE_MAX])(stKEY	sNewKey) =
{
	NULL,
	
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
		InterfaceModeScUncertifiedProc,
	#endif	

	InterfaceModeScSetProc,
	InterfaceModeScSelfControlProc,

	#if (ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)
		InterfaceModeScZibaoControlProc,
	#endif

	#if (LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG !=	DISABLED)
		InterfaceModeScNeighborControlProc,
	#endif

	#if (QUICK_KEY_CTRL_ENABLE_FLAG != DISABLED)
		InterfaceModeScSelfQuickProc,
	#endif

	#if (GRP_CTRL_ENABLE_FLAG != DISABLED)
		InterfaceModeScGrpControlProc,
	#endif
	
	#if (SET_YKQ_PRGPT_USE_FLAG != DISABLED)
		InterfaceModeYkqSelfControlProc,
		InterfaceModeYkqSetControlProc,
	#endif
	
	InterfaceModeSelfSetProc,
	
	InterfaceModeShutdownProc,	
	
	NULL,
	NULL,	

	InterfaceModeInchargeDisplayProc,

	InterfaceModeIdleProc,
	
	InterfaceModeInintProc,
	NULL,

	#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
		InterfaceModeSelfRecvTransPrgProc,
		InterfaceModeSelfRecvTransPrgProc,
		InterfaceModeSelfRecvTransPrgProc,
	#endif
	//配置文件同步
	InterfaceModeActionInfoSyn,	
	InterfaceModeWLUnConnectProc,

	NULL,
};

//zzw
/*******************************************************************************************
**函数名称：ReadActionInfo
**函数作用：从内部flash中读配置文件
**函数参数：无
**函数输出：1：不需要读。0：成功
**注意事项：无
*******************************************************************************************/
u8 ReadActionInfo(void)
{
	//开机后读
	u8 temp[4];
	u32 Version;
	IapReadBuf(temp,ACTION_ALLOCATION_INFO_ADDRESS,0x04);
	Version = ((u32)temp[0]<<24)|((u32)temp[1]<<16)|((u32)temp[2]<<8)|((u32)temp[3]);
	if(Version == 0)
		return 1;
	//如果flash中没有，则不用读，使用内存中默认的
	memset(&ActionInformation,0,sizeof(ActionInformation));
	IapReadBuf((u8 *)&ActionInformation,ACTION_ALLOCATION_INFO_ADDRESS,sizeof(ActionInformation));
	return 0;
}
/*******************************************************************************************
**函数名称：WriteActionInfo
**函数作用：内部FLASH中写入配置文件
**函数参数：无
**函数输出：1：crc相同，不需要写入。0：成功
**注意事项：无
*******************************************************************************************/
u8 WriteActionInfo(void)
{
	u8 temp[4];
	u16 OldCRC = 0;
	//获取rom中配置文件crc信息
	IapReadBuf(temp,ACTION_ALLOCATION_INFO_ADDRESS+4,0x04);
	OldCRC = (((u16)temp[2])<<8) |((u16)temp[3]);
	
	if(OldCRC == ActionInformation.Head.Verify)
		return 1;
	//写入数据
	#if (BSP_PRG_STORAGE_FLASH_FLAG	==	BSP_PRG_STORAGE_INFLASH_USE)
		__disable_fault_irq();
		FLASH_SetLatency(THE_DEV_FLASH_Latency);//120 MHz< HCLK <= 168 MHz
		FLASH_Unlock();			
	#endif	
	
	IapEraserSector(ACTION_ALLOCATION_INFO_ADDRESS);
	#ifdef IWDG_ENABLED
		IWDG_Feed();
	#endif
	IapWriteBuf((u8 *)&ActionInformation, ACTION_ALLOCATION_INFO_ADDRESS, sizeof(ActionInformation));	

	#if (BSP_PRG_STORAGE_FLASH_FLAG	==	BSP_PRG_STORAGE_INFLASH_USE)
		FLASH_Lock();
		__enable_fault_irq();			
	#endif
	return 0;
}

u8 ActionInfoInit(void)
{
	u16 Crc;
	u32 temp;
	
	IapReadBuf((u8 *)&temp, ACTION_ALLOCATION_INFO_ADDRESS+4, 0x04);
//	Crc = temp&0x0000ffff;
	Crc = 0;
	if(Crc == 0)
		WriteActionInfo();
	else
	{
		ReadActionInfo();
	}
//	memset(&ActionInformation,0,sizeof(ActionInformation));
}
/*******************************************************************************************
**函数名称：ActionKeyValueTurnAround
**函数作用：将按键值转换为配置文件中的键值
**函数参数：stKey：按键值。ShiftKeyFlag：shift键，pKey：转换后的按键值
**函数输出：按键个数
**注意事项：无
*******************************************************************************************/
typedef struct
{
	u32 OriginalKey;
	u8 NowKey;
}KeyAroundTab;

const KeyAroundTab keyTab[32] = {	{KEY_11,0x00},{KEY_12,0x00},{KEY_13,0x01},{KEY_14,0x02},
									{KEY_15,0x09},{KEY_21,0x0a},{KEY_22,0x03},{KEY_23,0x04},
									{KEY_24,0x0b},{KEY_25,0x0c},{KEY_31,0x05},{KEY_32,0x06},
									{KEY_33,0x00},{KEY_34,0x00},{KEY_35,0x07},{KEY_41,0x08},
									{KEY_42,0x00},{KEY_43,0x00},{KEY_44,0x00},{KEY_45,0x00},
									{KEY_51,0x00},{KEY_52,0x00},{KEY_53,25},{KEY_54,26},
									{KEY_55,0x00},{KEY_61,0x00},{KEY_62,0x00},{KEY_63,0x00},	//zzw 2023.12.24 将左右邻架键变成动作键
									{KEY_64,0x00},{KEY_65,0x00},{KEY_71,0x00},{KEY_71,0x00},};
u8 ActionKeyValueTurnAround(u32 u32Key,u32 ShiftKeyFlag,u8 *pKey)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
		 OS_CPU_SR  cpu_sr = 0;
	#endif
	u8 KeyCont = 0;
	u8 i;
//	u32 TempKey;
	OS_ENTER_CRITICAL();
	for(i =0;i<32;i++)			//统计同时按下的个数
	{
//		if(((u32Key>>i)&0x00000001))
//			KeyCont++;
		if((u32Key&(0x00000001<<i)) ==0)
			continue;
		if(((keyTab[i].OriginalKey&u32Key) == keyTab[i].OriginalKey)
			&&(keyTab[i].NowKey != 0))
		{
			if(ShiftKeyFlag == DISABLE)	
				*pKey = keyTab[i].NowKey;
			else
				*pKey = keyTab[i].NowKey + 0x0c;
			pKey++;
			KeyCont++;
		}
	}
	OS_EXIT_CRITICAL();
	return KeyCont;
//	
//	TempKey = u32Key;
//	if(ShiftKeyFlag == DISABLE)		//shif键未按下
//	{
//		if((TempKey&KEY_13) == KEY_13)			//c
//		{
//			*pKey = 0x01;
//			pKey++;
//			TempKey = u32Key;
//		}
//		if((TempKey&KEY_14) == KEY_14)		//d
//		{
//			*pKey = 0x02;
//			pKey++;
//			TempKey = u32Key;
//		}
//		if((TempKey&KEY_22) == KEY_22)			//1
//		{
//			*pKey = 0x03;
//			pKey++;
//			TempKey = u32Key;
//		}
//		if((TempKey&KEY_23) == KEY_23)	//2
//		{
//			*pKey = 0x04;
//			pKey++;
//			TempKey = u32Key;
//		}
//		if((TempKey&KEY_31) == KEY_31)	//3
//		{
//			*pKey = 0x05;
//			pKey++;
//			TempKey = u32Key;
//		}
//		if((TempKey&KEY_32) == KEY_32)	//4
//		{
//			*pKey = 0x06;
//			pKey++;
//			TempKey = u32Key;
//		}
//		if((TempKey&KEY_35) == KEY_35)			//5
//		{
//			*pKey = 0x07;
//			pKey++;
//			TempKey = u32Key;
//		}
//		if((TempKey&KEY_41) == KEY_41)	//6
//		{
//			*pKey = 0x08;
//			pKey++;
//			TempKey = u32Key;
//		}
//		if((TempKey&KEY_15) == KEY_15)			//7
//		{
//			*pKey = 0x09;
//			pKey++;
//			TempKey = u32Key;
//		}
//		if((TempKey&KEY_21) == KEY_21)	//8
//		{
//			*pKey = 0x0a;
//			pKey++;
//			TempKey = u32Key;
//		}
//		if((TempKey&KEY_24) == KEY_24)	//9
//		{
//			*pKey = 0x0b;
//			pKey++;
//			TempKey = u32Key;
//		}
//		if((TempKey&KEY_25) == KEY_25)	//0
//		{
//			*pKey = 0x0c;
//			pKey++;
//			TempKey = u32Key;
//		}	
//	}
//	else		//shif键未按下
//	{
//		if((TempKey&KEY_13) == KEY_13)			//c
//		{
//			*pKey = 0x0d;
//			pKey++;
//			TempKey = u32Key;
//		}
//		if((TempKey&KEY_14) == KEY_14)	//d
//		{
//			*pKey = 0x0e;
//			pKey++;
//			TempKey = u32Key;
//		}
//		if((TempKey&KEY_22) == KEY_22)			//1
//		{
//			*pKey = 0x0f;
//			pKey++;
//			TempKey = u32Key;
//		}
//		if((TempKey&KEY_23) == KEY_23)	//2
//		{
//			*pKey = 0x10;
//			pKey++;
//			TempKey = u32Key;
//		}
//		if((TempKey&KEY_31) == KEY_31)	//3
//		{
//			*pKey = 0x11;
//			pKey++;
//			TempKey = u32Key;
//		}
//		if((TempKey&KEY_32) == KEY_32)	//4
//		{
//			*pKey = 0x12;
//			pKey++;
//			TempKey = u32Key;
//		}
//		if((TempKey&KEY_35) == KEY_35)			//5
//		{
//			*pKey = 0x13;
//			pKey++;
//			TempKey = u32Key;
//		}
//		if((TempKey&KEY_41) == KEY_41)	//6
//		{
//			*pKey = 0x14;
//			pKey++;
//			TempKey = u32Key;
//		}
//		if((TempKey&KEY_15) == KEY_15)			//7
//		{
//			*pKey = 0x15;
//			pKey++;
//			TempKey = u32Key;
//		}
//		if((TempKey&KEY_21) == KEY_21)	//8
//		{
//			*pKey = 0x16;
//			pKey++;
//			TempKey = u32Key;
//		}
//		if((TempKey&KEY_24) == KEY_24)	//9
//		{
//			*pKey = 0x17;
//			pKey++;
//			TempKey = u32Key;
//		}
//		if((TempKey&KEY_25) == KEY_25)	//0
//		{
//			*pKey = 0x18;
//			pKey++;
//			TempKey = u32Key;
//		}	
//	}

//	return KeyCont;
}

/*******************************************************************************************
**函数名称：ActionKeyValueTurnAround
**函数作用：将配置文件的键值转换为实际键值
**函数参数：stKey：按键值。ShiftKeyFlag：shift键，pKey：转换后的按键值
**函数输出：按键个数
**注意事项：无
*******************************************************************************************/
u32 ActionKeyValueTurnBackToLED(u8 u8Key,u32 ShiftKeyFlag)
{
	u32 TempKey = 0;
	
	if(ShiftKeyFlag == DISABLE)
	{
		switch(u8Key)
		{
			case 1:
				TempKey = TempKey|KEY_LED_5;
			break;
			case 2:
				TempKey = TempKey|KEY_LED_6;
			break;
			case 3:
				TempKey = TempKey|KEY_LED_7;
			break;
			case 4:
				TempKey = TempKey|KEY_LED_8;
			break;	
			case 5:
				TempKey = TempKey|KEY_LED_9;
			break;
			case 6:
				TempKey = TempKey|KEY_LED_10;
			break;
			case 7:
				TempKey = TempKey|KEY_LED_11;
			break;
			case 8:
				TempKey = TempKey|KEY_LED_12;
			break;	
			case 9:
				TempKey = TempKey|KEY_LED_13;
			break;
			case 10:
				TempKey = TempKey|KEY_LED_14;
			break;
			case 11:
				TempKey = TempKey|KEY_LED_15;
			break;
			case 12:
				TempKey = TempKey|KEY_LED_16;
			break;
			default:
				TempKey = 0;
			break;
		}
	}
	else
	{
		switch(u8Key)
		{
			case 13:
				TempKey = TempKey|KEY_LED_5;
			break;
			case 14:
				TempKey = TempKey|KEY_LED_6;
			break;
			case 15:
				TempKey = TempKey|KEY_LED_7;
			break;
			case 16:
				TempKey = TempKey|KEY_LED_8;
			break;	
			case 17:
				TempKey = TempKey|KEY_LED_9;
			break;
			case 18:
				TempKey = TempKey|KEY_LED_10;
			break;
			case 19:
				TempKey = TempKey|KEY_LED_11;
			break;
			case 20:
				TempKey = TempKey|KEY_LED_12;
			break;	
			case 21:
				TempKey = TempKey|KEY_LED_13;
			break;
			case 22:
				TempKey = TempKey|KEY_LED_14;
			break;
			case 23:
				TempKey = TempKey|KEY_LED_15;
			break;
			case 24:
				TempKey = TempKey|KEY_LED_16;
			break;
			default:
				TempKey = 0;
			break;
		}	
	}
	return TempKey;
}

u32 InterfaceGetKeyLedValue(INTERFACE_CONTROL_MODE_TYPE stMode,u32 u32ShiftKeyStatus,u8 CtrlMode)
{
	u8 i;
	u32 SignalNearAutoKeyLED = 0;
	u32 ShiftSignalNearAutoKeyLED = 0;
	u32 SignalNearMannulKeyLED = 0;
	u32 ShiftSignalNearMannulKeyLED = 0;
	
	u32 GrpAutoKeyLED = 0;
	u32 ShiftGrpAutoKeyLED = 0;
	u32 GrpMannulKeyLED = 0;
	u32 ShiftGrpMannulKeyLED = 0;
	
	if((stMode == INTERFACE_MODE_SC_UNCERTIFIED)
		|| (stMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL))
	{
		for(i=0; i<ActionInformation.Head.TotalAction; i++)
		{
			if((ActionInformation.Body[i].AutoCtrlMode == ENABLE) 
				&& (CtrlMode == AUTO_CTRL)
				&&(ActionInformation.Body[i].ActionType != GROUP_ACTION))
			{
				SignalNearAutoKeyLED = SignalNearAutoKeyLED | ActionKeyValueTurnBackToLED(ActionInformation.Body[i].ActionKey,DISABLE);
				ShiftSignalNearAutoKeyLED = SignalNearAutoKeyLED | ActionKeyValueTurnBackToLED(ActionInformation.Body[i].ActionKey,ENABLE);
			}
			if((ActionInformation.Body[i].MannulCtrlMode == ENABLE) 
				&& (CtrlMode == MANUAL_CTRL)
				&&(ActionInformation.Body[i].ActionType != GROUP_ACTION))
			{
				SignalNearMannulKeyLED = SignalNearAutoKeyLED | ActionKeyValueTurnBackToLED(ActionInformation.Body[i].ActionKey,DISABLE);
				ShiftSignalNearMannulKeyLED = SignalNearAutoKeyLED | ActionKeyValueTurnBackToLED(ActionInformation.Body[i].ActionKey,ENABLE);
			}
		}	
	}
	else if(stMode == INTERFACE_MODE_SC_GRP_CONTROL)
	{
		for(i=0; i<ActionInformation.Head.TotalAction; i++)
		{
			if((ActionInformation.Body[i].AutoCtrlMode == ENABLE) 
				&& (CtrlMode == AUTO_CTRL)
				&&(ActionInformation.Body[i].ActionType == GROUP_ACTION))
			{
				GrpAutoKeyLED = SignalNearAutoKeyLED | ActionKeyValueTurnBackToLED(ActionInformation.Body[i].ActionKey,DISABLE);
				ShiftGrpAutoKeyLED = SignalNearAutoKeyLED | ActionKeyValueTurnBackToLED(ActionInformation.Body[i].ActionKey,ENABLE);
			}
			if((ActionInformation.Body[i].MannulCtrlMode == ENABLE) 
				&& (CtrlMode == MANUAL_CTRL)
				&&(ActionInformation.Body[i].ActionType == GROUP_ACTION))
			{
				GrpMannulKeyLED = SignalNearAutoKeyLED | ActionKeyValueTurnBackToLED(ActionInformation.Body[i].ActionKey,DISABLE);
				ShiftGrpMannulKeyLED = SignalNearAutoKeyLED | ActionKeyValueTurnBackToLED(ActionInformation.Body[i].ActionKey,ENABLE);
			}
		}		
	}
	return 1;
}
/*******************************************************************************************
**函数名称：GetActionIDByKey
**函数作用：按键获取动作结构体
**函数参数：InterfaceCtrlMode：动作界面。Key：按键（配置文件中），Action：动作类型
**函数输出：动作个数
**注意事项：无
*******************************************************************************************/
u8 GetActionIDByKey(INTERFACE_CONTROL_MODE_TYPE InterfaceCtrlMode,u8 Key)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
		 OS_CPU_SR  cpu_sr = 0;
	#endif
	u8 i,CtrlMode;
	LogicParamApi(LOGIC_GET_CTRL_MODE,&CtrlMode);
	//本架控制、邻架控制
	OS_ENTER_CRITICAL();
	if((InterfaceCtrlMode == INTERFACE_MODE_SC_UNCERTIFIED)
		||(InterfaceCtrlMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL))
	{
		for(i=0; i<ActionInformation.Head.TotalAction&&i<ACTION_NUM_MAX; i++)
		{
			if((ActionInformation.Body[i].ActionKey == Key)&&(ActionInformation.Body[i].ActionType != GROUP_ACTION))
			{
				//控制模式不符
				if(((ActionInformation.Body[i].AutoCtrlMode ==DISABLE)&&(CtrlMode == AUTO_CTRL))
					||((ActionInformation.Body[i].MannulCtrlMode ==DISABLE)&&(CtrlMode == MANUAL_CTRL)))
				{
					continue;
				}
				else
				{
					ActionInfoMsg[RuningActionCnt] = ActionInformation.Body[i];
					RuningActionCnt++;	
					RuningActionCnt = RuningActionCnt % INTERFACE_KEY_TOGETHER_MAX;
				}
			}
			else
			{
				continue;
			}
		}
	}
	//成组控制
	else if(InterfaceCtrlMode == INTERFACE_MODE_SC_GRP_CONTROL)
	{
		for(i=0; i<ACTION_NUM_MAX; i++)
		{
			if((ActionInformation.Body[i].ActionKey == Key)
				&&(ActionInformation.Body[i].ActionType == GROUP_ACTION))
			{
				//控制模式不符
				if(((ActionInformation.Body[i].AutoCtrlMode ==DISABLE)&&(CtrlMode == AUTO_CTRL))
					||((ActionInformation.Body[i].MannulCtrlMode ==DISABLE)&&(CtrlMode == MANUAL_CTRL)))
					continue;
				ActionInfoMsg[RuningActionCnt] = ActionInformation.Body[i];
				RuningActionCnt++;
				RuningActionCnt = RuningActionCnt % INTERFACE_KEY_TOGETHER_MAX;
			}
		}		
	}
	else
	{
		RuningActionCnt = 0;
	}
	OS_EXIT_CRITICAL();
	return 1;
}

u8 GetActionInfoByID(u8 ActionID,INTERFACE_CONTROL_MODE_TYPE InterfaceCtrlMode)
{
	u8 i,Cont = 0;
	//本架控制、邻架控制
	if((InterfaceCtrlMode == INTERFACE_MODE_SC_UNCERTIFIED)
		||(InterfaceCtrlMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL))
	{
		for(i=0; i<ACTION_NUM_MAX; i++)
		{
			if((ActionInformation.Body[i].ActionID == ActionID)
				&&(ActionInformation.Body[i].ActionType != GROUP_ACTION))
			{
				ActionInfoMsg[Cont] = ActionInformation.Body[i];
				Cont++;
			}
		}
	}
	//成组控制
	else if(InterfaceCtrlMode == INTERFACE_MODE_SC_GRP_CONTROL)
	{
		for(i=0; i<ACTION_NUM_MAX; i++)
		{
			if((ActionInformation.Body[i].ActionID == ActionID)
				&&(ActionInformation.Body[i].ActionType == GROUP_ACTION))
			{
				ActionInfoMsg[Cont] = ActionInformation.Body[i];
				Cont++;
				Cont++;
			}
		}		
	}
	else
		Cont = 0;
	
	return Cont;
}

uint8_t getColiScNumStatus(void)
{
	return u8SwitchColiScNumStatus;
}

void setColiScNumStatus(uint8_t tmp)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
		OS_CPU_SR  cpu_sr = 0;
	#endif
	OS_ENTER_CRITICAL();
	u8SwitchColiScNumStatus = tmp;
	OS_EXIT_CRITICAL();
}

/*******************************************************************************************
**函数名称：Interface_Init
**函数作用：界面主任务下的变量及信号量的初始化
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void Interface_Init(void)
{
	u16 u16I;
	
	s_stInterfaceControlMode = INTERFACE_MODE_ININT;
	s_u32MainUpdateMethod = INTERFACE_SHOW_MAIN_NONE;

	for(u16I = 0x00;u16I < INTERFACE_KEY_TOGETHER_MAX;u16I++)
	{
		s_stInterfaceActionName[u16I] = INTERFACE_ACTION_NONE;
	}
	s_u64InterfaceRfidDataBackup = 0x00;
	
	s_u16NoteItemWrite = 0x00;
	s_u16NoteItemRead = 0x00;

	s_stScrAwake = INTERFACE_SCR_AWAKE;
	s_u32ScrPrtectedTimer = 0x00;
	s_u32AutoShutdownTimer = 0x00;//自动关机计时
	
	LogicParamApi(LOGIC_GET_OLED_SCR_LIGHT,&u16I);	
	OledContrastAdj(u16I);	//亮度参数			
	LogicParamApi(LOGIC_GET_INTER_CONTROL_TIMER,&s_u32InterfaceControlTimer);
	if(!s_u32InterfaceControlTimer)
		s_u32InterfaceControlTimer = INTERFACE_INTERCTRL_TIMER_MAX;
	LogicParamApi(LOGIC_GET_OLED_PROTECTED_TIMER,&s_u32ScrPrtectedTimer);
	if(!s_u32ScrPrtectedTimer)
		s_u32ScrPrtectedTimer = INTERFACE_SCR_TIMER_STOP;
	LogicParamApi(LOGIC_GET_NOTE_SHOW_TIMER,&s_u32NoteShowTimer);
	if(!s_u32NoteShowTimer)
		s_u32NoteShowTimer = INTERFACE_NOTE_TIMER_STOP;
	
	s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;

	s_u32ScCtrlKeyTimer = 0x00;
	
	s_u32ShiftKeyEnableFlag = DISABLED;
	s_u32AllKeyStatus = 0xFFFF;

	s_u32StartKeyFlag= KEY_UP;
	#if (SPECIAL_FX_ENABLE_FLAG != DISABLED)
		#if (SPECIAL_F1_ENABLE_FLAG != DISABLED)
			s_u32SpecialFxKeyFlag1= KEY_UP;
		#endif
		#if (SPECIAL_F2_ENABLE_FLAG != DISABLED)
			s_u32SpecialFxKeyFlag2= KEY_UP;
		#endif
		#if (SPECIAL_F3_ENABLE_FLAG != DISABLED)
			s_u32SpecialFxKeyFlag3= KEY_UP;
		#endif
		#if (SPECIAL_F4_ENABLE_FLAG != DISABLED)
			s_u32SpecialFxKeyFlag4= KEY_UP;
		#endif
	#endif	


	#if (ZIBAO_CTRL_ENABLE_FLAG != DISABLED)
		s_u32ZibaoActionKeyBackup = 0x00;
	#endif

	#if  (KEY_AUTO_PRESS_PROC_FLAG	!=	DISABLED)
		s_u32AutoPressKeyFlag = KEY_UP;
	#endif

	#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
		s_u32DownloadRemainTimer = 0x00;
		s_u32DownloadRemainTimerBackup = 0x00;
	#endif	

	s_u64InterfaceRfidDataBackup = 0x00;
	
	g_stSemInterface = OSSemCreate(0);	//创建信息显示信号量
}

/*********************************************************************************
** 函数名称:InterfaceGetKeyRfidData
** 功能描述：获取发送RFID的动作字以及显示内容
** 函数输入：stMode:模式，u32ShiftKeyStatus,shift状态32KeyValue:输入的按键
** 函数输出：RFID_DATA_TYPE动作字，*pInterAction:显示的内容
** 注意事项：
**********************************************************************************/
u64 InterfaceGetKeyRfidDataShowName(INTERFACE_CONTROL_MODE_TYPE stMode,u32 u32ShiftKeyStatus,u32 u32KeyValue,INTERFACE_ACTION_TYPE *pInterAction,RFID_FRAME_TYPE *pRfidFType)
{
	u16 u16I = 0x00;
	u16 u16J = 0x00;
	u16 u16K = 0x00;
	static u64 u64RfidData;
	u32 u32KeyValueBackup = u32KeyValue;
	u32 u32KeyShiftBackup = 0x00;
	u32 u32OneKeyOfKeyValue = 0x00;
	u32 u32ShiftBit = 0x00;
	
	INTERFACE_LOGIC_CORRESPOND_TYPE *p = NULL;
	u64RfidData = RFID_ACTION_NONE;
	
	memset(pInterAction,INTERFACE_ACTION_NONE,INTERFACE_KEY_TOGETHER_MAX);

	#if (SHIFT_MULTIPLE_CTRL_ENABLE_FLAG	!= DISABLED)
		if(u32ShiftKeyStatus == ENABLED)
			u32KeyShiftBackup = KEY_MULRIPLE_VALUE;
		else
			u32KeyShiftBackup = 0x00;
	#else
		u32KeyShiftBackup = 0x00;
	#endif
	
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG	!= DISABLED)
		if((stMode == INTERFACE_MODE_SC_UNCERTIFIED)||(stMode == INTERFACE_MODE_SC_SET))
	#else
		if(stMode == INTERFACE_MODE_SC_SET)
	#endif
		{	
			u16J = 0x00;
			for(u16I = 0x00; u16I <32; u16I++)		
			{
				if((u32KeyValueBackup&0x01) == 0x01)
					u16J++;
				u32KeyValueBackup >>= 1;
			}
			if(u16J == 0x01)
				u32KeyValueBackup = u32KeyValue;
			else
			{
				return u64RfidData;
			}
		}
	
	if(stMode == INTERFACE_MODE_SC_SELF_CONTROL)
		;//p = (INTERFACE_LOGIC_CORRESPOND_TYPE *)&(s_stInterfaceLogicTable_ScSelf);
	else if(stMode == INTERFACE_MODE_SC_SET)
	{
		p = (INTERFACE_LOGIC_CORRESPOND_TYPE *)&(s_stInterfaceLogicTable_ScSet);
	}
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
		else if(stMode == INTERFACE_MODE_SC_UNCERTIFIED)
			p = (INTERFACE_LOGIC_CORRESPOND_TYPE *)&(s_stInterfaceLogicTable_Uncertified);	
	#endif
		
	#if (ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)
		else if(stMode == INTERFACE_MODE_SC_ZIBAO_CONTROL)
			p = (INTERFACE_LOGIC_CORRESPOND_TYPE *)&(s_stInterfaceLogicTable_ScZibao);	
	#endif
	
	#if (LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG !=	DISABLED)
		else if(stMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
			p = (INTERFACE_LOGIC_CORRESPOND_TYPE *)&(s_stInterfaceLogicTable_ScNeighbor);			
	#endif
	
	#if (QUICK_KEY_CTRL_ENABLE_FLAG	!= DISABLED)
		else if(stMode == INTERFACE_MODE_SC_SELF_QUICK)
			p = (INTERFACE_LOGIC_CORRESPOND_TYPE *)&(s_stInterfaceLogicTable_ScQuick);		
	#endif

	#if (GRP_CTRL_ENABLE_FLAG != DISABLED)
		else if(stMode == INTERFACE_MODE_SC_GRP_CONTROL)
			p = (INTERFACE_LOGIC_CORRESPOND_TYPE *)&(s_stInterfaceLogicTable_ScGrp);		
	#endif
		else
			return u64RfidData;
		
	/***判断出是否有功能按键与其他的按键同时按下***/	
	/**2018.02.26，新增加***/	
	if((u32KeyValueBackup&KEY_SHIFT) == KEY_SHIFT)
	{
		if((u32KeyValueBackup^KEY_SHIFT) != 0)
		{
			return u64RfidData;
		}
	}
	if((u32KeyValueBackup&KEY_INCREASE) == KEY_INCREASE)
	{
		if((u32KeyValueBackup^KEY_INCREASE) != 0)
		{
			return u64RfidData;
		}
	}	
	if((u32KeyValueBackup&KEY_DECREASE) == KEY_DECREASE)
	{
		if((u32KeyValueBackup^KEY_DECREASE) != 0)
		{
			return u64RfidData;
		}
	}	
	
	//jhy
	if((u32KeyValueBackup&KEY_LEFT_NEIGHBOR) == KEY_LEFT_NEIGHBOR)
	{
		if((u32KeyValueBackup^KEY_LEFT_NEIGHBOR) != 0)
		{
			return u64RfidData;
		}
	}	
	if((u32KeyValueBackup&KEY_RIGHT_NEIGHBOR) == KEY_RIGHT_NEIGHBOR)
	{
		if((u32KeyValueBackup^KEY_RIGHT_NEIGHBOR) != 0)
		{
			return u64RfidData;
		}
	}	
	
	
	
	if((u32KeyValueBackup&KEY_LEFT_GRP) == KEY_LEFT_GRP)
	{
		if((u32KeyValueBackup^KEY_LEFT_GRP) != 0)
		{
			return u64RfidData;
		}
	}	
	if((u32KeyValueBackup&KEY_RIGHT_GRP) == KEY_RIGHT_GRP)
	{
		if((u32KeyValueBackup^KEY_RIGHT_GRP) != 0)
		{
			return u64RfidData;
		}
	}	
	if((u32KeyValueBackup&KEY_CERTIFY) == KEY_CERTIFY)
	{
		if((u32KeyValueBackup^KEY_CERTIFY) != 0)
		{
			return u64RfidData;
		}
	}
	if(u32ShiftKeyStatus == DISABLED)
	{
		if((u32KeyValueBackup&KEY_MENU_START) == KEY_MENU_START)
		{
			if((u32KeyValueBackup^KEY_MENU_START) != 0)
			{
				return u64RfidData;
			}
		}	
	}
	if((u32KeyValueBackup&KEY_STOP_SHUTDOWN) == KEY_STOP_SHUTDOWN)//停止键优先级最高
	{
		if((u32KeyValueBackup^KEY_STOP_SHUTDOWN) != 0)
		{
			u32KeyValueBackup = KEY_STOP_SHUTDOWN;
		}
	}	

//	if((((u32KeyValueBackup|u32KeyShiftBackup)&KEY_SHORTCUT_F1) == KEY_SHORTCUT_F1) \
//		&&((((u32KeyValueBackup|u32KeyShiftBackup) > KEY_MULRIPLE_VALUE) && (KEY_SHORTCUT_F1 > KEY_MULRIPLE_VALUE))\
//		||(((u32KeyValueBackup|u32KeyShiftBackup) < KEY_MULRIPLE_VALUE) && (KEY_SHORTCUT_F1 < KEY_MULRIPLE_VALUE))))
//	{
//		if(((u32KeyValueBackup|u32KeyShiftBackup)^KEY_SHORTCUT_F1) != 0)
//		{
//			return u64RfidData;
//		}		
//	}
	
//	if((((u32KeyValueBackup|u32KeyShiftBackup)&KEY_SHORTCUT_F2) == KEY_SHORTCUT_F2) \
//		&&((((u32KeyValueBackup|u32KeyShiftBackup) > KEY_MULRIPLE_VALUE) && (KEY_SHORTCUT_F2 > KEY_MULRIPLE_VALUE))\
//		||(((u32KeyValueBackup|u32KeyShiftBackup) < KEY_MULRIPLE_VALUE) && (KEY_SHORTCUT_F2 < KEY_MULRIPLE_VALUE))))		
//	{
//		if(((u32KeyValueBackup|u32KeyShiftBackup)^KEY_SHORTCUT_F2) != 0)
//		{
//			return u64RfidData;
//		}		
//	}
	
	if((((u32KeyValueBackup|u32KeyShiftBackup)&KEY_SHORTCUT_F3) == KEY_SHORTCUT_F3) \
		&&((((u32KeyValueBackup|u32KeyShiftBackup) > KEY_MULRIPLE_VALUE) && (KEY_SHORTCUT_F3 > KEY_MULRIPLE_VALUE))\
		||(((u32KeyValueBackup|u32KeyShiftBackup) < KEY_MULRIPLE_VALUE) && (KEY_SHORTCUT_F3 < KEY_MULRIPLE_VALUE))))
	{
		if(((u32KeyValueBackup|u32KeyShiftBackup)^KEY_SHORTCUT_F3) != 0)
		{
			return u64RfidData;
		}		
	}
	
	if((((u32KeyValueBackup|u32KeyShiftBackup)&KEY_SHORTCUT_F4) == KEY_SHORTCUT_F4) \
		&&((((u32KeyValueBackup|u32KeyShiftBackup) > KEY_MULRIPLE_VALUE) && (KEY_SHORTCUT_F4 > KEY_MULRIPLE_VALUE))\
		||(((u32KeyValueBackup|u32KeyShiftBackup) < KEY_MULRIPLE_VALUE) && (KEY_SHORTCUT_F4 < KEY_MULRIPLE_VALUE))))
	{
		if(((u32KeyValueBackup|u32KeyShiftBackup)^KEY_SHORTCUT_F4) != 0)
		{
			return u64RfidData;
		}		
	}
	
	#if  (KEY_AUTO_PRESS_PROC_FLAG	!=	DISABLED)
		if((((u32KeyValueBackup|u32KeyShiftBackup)&KEY_AUTO_PRESS_SPECIAL) == KEY_AUTO_PRESS_SPECIAL)\
			&&((((u32KeyValueBackup|u32KeyShiftBackup) > KEY_MULRIPLE_VALUE) && (KEY_AUTO_PRESS_SPECIAL > KEY_MULRIPLE_VALUE))\
			||(((u32KeyValueBackup|u32KeyShiftBackup) < KEY_MULRIPLE_VALUE) && (KEY_AUTO_PRESS_SPECIAL < KEY_MULRIPLE_VALUE))))			
		{
			if(((u32KeyValueBackup|u32KeyShiftBackup)^KEY_AUTO_PRESS_SPECIAL) != 0)
			{
				return u64RfidData;
			}
		}
	#endif	
		
	/**判断是否有相反的动作**/
// 	#if ((LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG != DISABLED)&&(ZIBAO_CTRL_ENABLE_FLAG != DISABLED))
// 		if((stMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)||(stMode == INTERFACE_MODE_SC_ZIBAO_CONTROL))
// 	#elif (LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG != DISABLED)
// 		if(stMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
// 	#elif ((INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)&&(ZIBAO_CTRL_ENABLE_FLAG != DISABLED))
// 		if((stMode == INTERFACE_MODE_SC_SELF_CONTROL)||(stMode == INTERFACE_MODE_SC_ZIBAO_CONTROL))
// 	#elif (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
// 		if(stMode == INTERFACE_MODE_SC_SELF_CONTROL)
// 	#endif
// 		{
// 			for(u16I = 0x00;;u16I++)
// 			{
// 				if(p[u16I].stInterfaceCtrlMode == INTERFACE_MODE_NONE)
// 					break;
// 				if((p[u16I].stInterfaceCtrlMode == stMode)&&(((u32KeyValueBackup|u32KeyShiftBackup)&p[u16I].u32InterfaceKeyValue) == p[u16I].u32InterfaceKeyValue)\
// 					&&((p[u16I].u32InterfaceKeyValue&u32KeyShiftBackup) == u32KeyShiftBackup))
// 				{
// 					for(u16J = u16I;;u16J++)
// 					{
// 						if(p[u16J].stInterfaceCtrlMode == INTERFACE_MODE_NONE)
// 							break;	

// 						#if ((LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG != DISABLED)&&(ZIBAO_CTRL_ENABLE_FLAG != DISABLED))
// 							if((stMode != INTERFACE_MODE_SC_NEIGHBOR_CONTROL)&&(stMode != INTERFACE_MODE_SC_ZIBAO_CONTROL))
// 						#elif (LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG != DISABLED)
// 							if(stMode != INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
// 						#elif ((INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)&&(ZIBAO_CTRL_ENABLE_FLAG != DISABLED))
// 							if((stMode != INTERFACE_MODE_SC_SELF_CONTROL)&&(stMode != INTERFACE_MODE_SC_ZIBAO_CONTROL))
// 						#elif (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
// 							if(stMode != INTERFACE_MODE_SC_SELF_CONTROL)
// 						#endif	
// 								break;
// 						if((p[u16J].stInterfaceCtrlMode == stMode)&&(((u32KeyValueBackup|u32KeyShiftBackup)&p[u16J].u32InterfaceKeyValue) == p[u16J].u32InterfaceKeyValue)\
// 							&&((p[u16J].u32InterfaceKeyValue&u32KeyShiftBackup) == u32KeyShiftBackup))
// 						{
// 							for(u16K = 0x00;;u16K++)
// 							{
// 								if(( s_stRfidReverseActionTable[u16K].u64RfidActionName) == RFID_ACTION_NONE)
// 									break;
// 	
// 								if((p[u16I].u64RfidSendActionData|p[u16J].u64RfidSendActionData)
// 									== (s_stRfidReverseActionTable[u16K].u64RfidActionName|s_stRfidReverseActionTable[u16K].u64RfidReverseName))
// 								{				
// 									u32KeyValueBackup &= ~(p[u16I].u32InterfaceKeyValue);
// 									u32KeyValueBackup &= ~(p[u16J].u32InterfaceKeyValue);						
// 									break;
// 								}
// 							}
// 						}
// 					}					

// 				}
// 			}
// 		}

	if(((stMode == INTERFACE_MODE_SC_SELF_CONTROL)|| (stMode == INTERFACE_MODE_SC_GRP_CONTROL) || \
		(stMode == INTERFACE_MODE_SC_UNCERTIFIED)||(stMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)) && ((0xCFFC & u32KeyValueBackup) == u32KeyValueBackup))	//jhy //仅对四种模式下的动作键进行相反动作判断 0xCFFC是所有12个动作按键的键值和 
	{
		InterfaceTakeOutReverseKey(stMode,&u32KeyValueBackup);
	}
	
	for(u16K = 0; u16K < 32; u16K++)	//含有多个键值时必须分开单独处理，因为每个键值对应一个动作序号，只有动作序号才能查询出无线动作码 2020.01.15 parry
	{
		u32ShiftBit = (u32)(0x01 << u16K);
		if((u32KeyValueBackup & u32ShiftBit) == 0x00) 
			continue;
		else
			u32OneKeyOfKeyValue = u32ShiftBit;
		if(((stMode == INTERFACE_MODE_SC_SELF_CONTROL)  || (stMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)|| \
			(stMode == INTERFACE_MODE_SC_GRP_CONTROL) || (stMode == INTERFACE_MODE_SC_UNCERTIFIED)) && ((0xCFFC & u32OneKeyOfKeyValue) == u32OneKeyOfKeyValue))//jhy	//仅对四种模式下的动作键进行转换 0xCFFC是所有12个动作按键的键值和 
		{
			for(u16I = 0x00; u16I < 12; u16I++)		//将键值转换为按键序号(0~11)	2020.01.15 parry
			{
				if(s_stInterfaceKeySNTable[u16I] == u32OneKeyOfKeyValue)
					break;
			}
//			if(u16I < 12)		//是12个动作键
//			{
//				if(u32KeyShiftBackup > 0)		//通过按键序号和shift值得出动作序号
//					u32OneKeyOfKeyValue = u16KeySetActionNameRam[u16I + 12];
//				else
//					u32OneKeyOfKeyValue = u16KeySetActionNameRam[u16I];
//			}
		}
		else if((stMode == INTERFACE_MODE_SC_SELF_CONTROL) && ((u32KeyValueBackup == KEY_MENU_START) | (u32KeyValueBackup == KEY_SHORTCUT_F3)) && (u32ShiftKeyStatus == ENABLED)) //一键放煤不是12动作键，须单独处理	2020.01.15 parry
		{
			u32OneKeyOfKeyValue = KEY_SHORTCUT_F3;
			u32KeyValueBackup = 0x00;
		}

		/**分析出具体执行的动作**/
		for(u16I = 0x00;;u16I++)
		{
			if(p[u16I].stInterfaceCtrlMode == INTERFACE_MODE_NONE)
				break;

			if((p[u16I].stInterfaceCtrlMode == stMode)&&(u32OneKeyOfKeyValue == p[u16I].u32InterfaceKeyValue))
			{
				u64RfidData |= p[u16I].u64RfidSendActionData;
		  	*pInterAction =  p[u16I].stInterfaceShowActionName;
			  pInterAction++;
			  *pRfidFType = p[u16I].stRfidFrameType;
			 
			  u32KeyValueBackup ^= u32ShiftBit;
				break;
			}
		}
	  if(u32KeyValueBackup == 0x00)	
			break;
	}
	
	return u64RfidData;
}
/*********************************************************************************
** 函数名称:InterfaceGetKeyLedValue
** 功能描述：获取发送某种模式默认点亮的按键指示灯
** 函数输入：stMode:模式，u32ShiftKeyStatus,shift状态
** 函数输出：RFID_DATA_TYPE动作字，*pInterAction:显示的内容
** 注意事项：
**********************************************************************************/
u32 InterfaceGetKeyLedDefaultValue(INTERFACE_CONTROL_MODE_TYPE stMode,u32 u32ShiftKeyStatus)
{
	u32 u32I = 0x00,u32K = 0x00;
	u32 u32KeyLedValue = 0x00;
	u32 u32KeyValue = 0x00;
	static u16 lastcrc = 0;
	u8 i,CtrlMode;
	static u32 SignalNearAutoKeyLED = 0;
	static u32 ShiftSignalNearAutoKeyLED = 0;
	static u32 SignalNearMannulKeyLED = 0;
	static u32 ShiftSignalNearMannulKeyLED = 0;
	
	static u32 GrpAutoKeyLED = 0;
	static u32 ShiftGrpAutoKeyLED = 0;
	static u32 GrpMannulKeyLED = 0;
	static u32 ShiftGrpMannulKeyLED = 0;
	
	LogicParamApi(LOGIC_GET_CTRL_MODE,&CtrlMode);
	if(lastcrc != ActionInformation.Head.Verify)
	{
		for(i=0; i<ActionInformation.Head.TotalAction; i++)
		{
			if(ActionInformation.Body[i].AutoCtrlMode == ENABLE) 
			{
				if((ActionInformation.Body[i].ActionType == SIGNAL_ACTION)
					||(ActionInformation.Body[i].ActionType == COMBINATION_ACTION))
				{
					SignalNearAutoKeyLED = SignalNearAutoKeyLED | ActionKeyValueTurnBackToLED(ActionInformation.Body[i].ActionKey,DISABLE);
					ShiftSignalNearAutoKeyLED = ShiftSignalNearAutoKeyLED | ActionKeyValueTurnBackToLED(ActionInformation.Body[i].ActionKey,ENABLE);
				}
				if((ActionInformation.Body[i].ActionType == GROUP_ACTION))
				{
					GrpAutoKeyLED = GrpAutoKeyLED | ActionKeyValueTurnBackToLED(ActionInformation.Body[i].ActionKey,DISABLE);
					ShiftGrpAutoKeyLED = ShiftGrpAutoKeyLED | ActionKeyValueTurnBackToLED(ActionInformation.Body[i].ActionKey,ENABLE);
				}
				else
					;
			}
			if(ActionInformation.Body[i].MannulCtrlMode == ENABLE) 
			{
				if((ActionInformation.Body[i].ActionType == SIGNAL_ACTION)
					||(ActionInformation.Body[i].ActionType == COMBINATION_ACTION))
				{
					SignalNearMannulKeyLED = SignalNearMannulKeyLED | ActionKeyValueTurnBackToLED(ActionInformation.Body[i].ActionKey,DISABLE);
					ShiftSignalNearMannulKeyLED = ShiftSignalNearMannulKeyLED | ActionKeyValueTurnBackToLED(ActionInformation.Body[i].ActionKey,ENABLE);
				}
				if ((ActionInformation.Body[i].ActionType == GROUP_ACTION))
				{
					GrpMannulKeyLED = GrpMannulKeyLED | ActionKeyValueTurnBackToLED(ActionInformation.Body[i].ActionKey,DISABLE);
					ShiftGrpMannulKeyLED = ShiftGrpMannulKeyLED | ActionKeyValueTurnBackToLED(ActionInformation.Body[i].ActionKey,ENABLE);
				}
				else
					;
			}
		}	
		lastcrc = ActionInformation.Head.Verify;
	}
	
	/***区分不同模式进行处理***/
	if((stMode == INTERFACE_MODE_SC_SET) ||(stMode == INTERFACE_MODE_SELF_SET))
	{

		u32KeyValue = KEY_LED_SET;
	}
	else if(stMode == INTERFACE_MODE_SC_SELF_CONTROL)
	{
			u32KeyValue = 0x1FFFFFF;
	}
	#if (LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG !=	DISABLED)
		else if(stMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
		{
			if((CtrlMode == AUTO_CTRL)&&(u32ShiftKeyStatus ==DISABLE))
				u32KeyValue = SignalNearAutoKeyLED;
			else if((CtrlMode == AUTO_CTRL)&&(u32ShiftKeyStatus ==ENABLE))
				u32KeyValue = ShiftSignalNearAutoKeyLED;
			else if ((CtrlMode == MANUAL_CTRL)&&(u32ShiftKeyStatus ==DISABLE))
				u32KeyValue = SignalNearMannulKeyLED;
			else if ((CtrlMode == MANUAL_CTRL)&&(u32ShiftKeyStatus ==ENABLE))
				u32KeyValue = ShiftSignalNearMannulKeyLED;
			else
				u32KeyValue =0;
			u32KeyValue |= KEY_LED_1|KEY_LED_2|KEY_LED_3|KEY_LED_4;
		}	
	#endif
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
		else if(stMode == INTERFACE_MODE_SC_UNCERTIFIED)
		{
			if((CtrlMode == AUTO_CTRL)&&(u32ShiftKeyStatus ==DISABLE))
				u32KeyValue = SignalNearAutoKeyLED;
			else if((CtrlMode == AUTO_CTRL)&&(u32ShiftKeyStatus ==ENABLE))
				u32KeyValue = ShiftSignalNearAutoKeyLED;
			else if ((CtrlMode == MANUAL_CTRL)&&(u32ShiftKeyStatus ==DISABLE))
				u32KeyValue = SignalNearMannulKeyLED;
			else if ((CtrlMode == MANUAL_CTRL)&&(u32ShiftKeyStatus ==ENABLE))
				u32KeyValue = ShiftSignalNearMannulKeyLED;
			else
				u32KeyValue =0;		
			u32KeyValue |= KEY_LED_1|KEY_LED_2|KEY_LED_3|KEY_LED_4;
		}	
	#endif		
	#if (ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)	
		else if(stMode == INTERFACE_MODE_SC_ZIBAO_CONTROL)//自保时除去快捷键	
		{
			u32KeyValue = 0x1FFFFFF;
			if(u32ShiftKeyStatus == ENABLED)
			{
				if((KEY_SHORTCUT_F1&KEY_MULRIPLE_VALUE) == KEY_MULRIPLE_VALUE)
				{
					u32KeyValue &= ~KEY_SHORTCUT_F1;
				}
				if((KEY_SHORTCUT_F2&KEY_MULRIPLE_VALUE) == KEY_MULRIPLE_VALUE)
				{
					u32KeyValue &= ~KEY_SHORTCUT_F2;
				}
				if((KEY_SHORTCUT_F3&KEY_MULRIPLE_VALUE) == KEY_MULRIPLE_VALUE)
				{
					u32KeyValue &= ~KEY_SHORTCUT_F3;
				}
				if((KEY_SHORTCUT_F4&KEY_MULRIPLE_VALUE) == KEY_MULRIPLE_VALUE)
				{
					u32KeyValue &= ~KEY_SHORTCUT_F4;
				}				
			}
			else
			{
				if((KEY_SHORTCUT_F1&KEY_MULRIPLE_VALUE) != KEY_MULRIPLE_VALUE)
				{
					u32KeyValue &= ~KEY_SHORTCUT_F1;
				}
				if((KEY_SHORTCUT_F2&KEY_MULRIPLE_VALUE) != KEY_MULRIPLE_VALUE)
				{
					u32KeyValue &= ~KEY_SHORTCUT_F2;
				}
				if((KEY_SHORTCUT_F3&KEY_MULRIPLE_VALUE) != KEY_MULRIPLE_VALUE)
				{
					u32KeyValue &= ~KEY_SHORTCUT_F3;
				}
				if((KEY_SHORTCUT_F4&KEY_MULRIPLE_VALUE) != KEY_MULRIPLE_VALUE)
				{
					u32KeyValue &= ~KEY_SHORTCUT_F4;
				}	
				u32I = 0x00;
				u32K = 0x00;
				LogicParamApi(LOGIC_GET_ZIBAO_ENABLE_FLAG,&u32I);
				LogicParamApi(LOGIC_GET_ZIBAO_ACTION_MAX,&u32K);
				if((u32I == DISABLED)||(u32K == 0x00))
					u32KeyValue = 0x00;
			}
		}
	#endif
	#if (QUICK_KEY_CTRL_ENABLE_FLAG	!= DISABLED)
		else if(stMode == INTERFACE_MODE_SC_SELF_QUICK)
		{			
			u32KeyValue = KEY_QUICK_CTRL_1;
		}
	#endif	
	#if (GRP_CTRL_ENABLE_FLAG != DISABLED)
		else if(stMode == INTERFACE_MODE_SC_GRP_CONTROL)
		{
			if((CtrlMode == AUTO_CTRL)&&(u32ShiftKeyStatus ==DISABLE))
				u32KeyValue = GrpAutoKeyLED;
			else if((CtrlMode == AUTO_CTRL)&&(u32ShiftKeyStatus ==ENABLE))
				u32KeyValue = ShiftGrpAutoKeyLED;
			else if ((CtrlMode == MANUAL_CTRL)&&(u32ShiftKeyStatus ==DISABLE))
				u32KeyValue = GrpMannulKeyLED;
			else if ((CtrlMode == MANUAL_CTRL)&&(u32ShiftKeyStatus ==ENABLE))
				u32KeyValue = ShiftGrpMannulKeyLED;
			else
				u32KeyValue =0;	
			u32KeyValue |= KEY_LED_1|KEY_LED_2|KEY_LED_3|KEY_LED_4;			
		}
	#endif	
	else
	{
		return 0x00;
	}
	
	return u32KeyValue;
}

/*********************************************************************************
** 函数名称:InterfaceGetKeyRfidData
** 功能描述：获取发送RFID的动作字以及显示内容
** 函数输入：stMode:模式，u32ShiftKeyStatus,shift状态32KeyValue:输入的按键
** 函数输出：RFID_DATA_TYPE动作字，*pInterAction:显示的内容
** 注意事项：
**********************************************************************************/
u64 InterfaceTakeOutReverseKey(INTERFACE_CONTROL_MODE_TYPE stMode,u32 *pKeyValue)  //去掉相反动作	2020.01.15 parry
{
	u16 u16I = 0x00;
	u16 u16J = 0x00;
	u16 u16K = 0x00;
	u16 u16L = 0x00;
	static u64 u64RfidData;
	u32 u32KeyValueBackup = *pKeyValue;
	static INTERFACE_LOGIC_CORRESPOND_TYPE *p = NULL;
	u32 u32OneKeyOfKeyValue = 0x00;
	u32 u32ShiftBit = 0x00;
	static INTERFACE_KEYSINGLE_CORRESPOND_TYPE MultiKeyActionTable[INTERFACE_KEY_TOGETHER_MAX];		

	u64RfidData = RFID_ACTION_NONE;
	memset(MultiKeyActionTable, 0x00 ,sizeof(INTERFACE_KEYSINGLE_CORRESPOND_TYPE) * INTERFACE_KEY_TOGETHER_MAX);
	
	if(stMode == INTERFACE_MODE_SC_SELF_CONTROL)
		;//p = (INTERFACE_LOGIC_CORRESPOND_TYPE *)&(s_stInterfaceLogicTable_ScSelf);
	else if(stMode == INTERFACE_MODE_SC_SET)
	{
		p = (INTERFACE_LOGIC_CORRESPOND_TYPE *)&(s_stInterfaceLogicTable_ScSet);
	}
	#if (ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)
		else if(stMode == INTERFACE_MODE_SC_ZIBAO_CONTROL)
			p = (INTERFACE_LOGIC_CORRESPOND_TYPE *)&(s_stInterfaceLogicTable_ScZibao);	
	#endif
	
	#if (LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG !=	DISABLED)
		else if(stMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
			p = (INTERFACE_LOGIC_CORRESPOND_TYPE *)&(s_stInterfaceLogicTable_ScNeighbor);			
	#endif
	
	#if (QUICK_KEY_CTRL_ENABLE_FLAG	!= DISABLED)
		else if(stMode == INTERFACE_MODE_SC_SELF_QUICK)
			p = (INTERFACE_LOGIC_CORRESPOND_TYPE *)&(s_stInterfaceLogicTable_ScQuick);		
	#endif

	#if (GRP_CTRL_ENABLE_FLAG != DISABLED)
		else if(stMode == INTERFACE_MODE_SC_GRP_CONTROL)
			p = (INTERFACE_LOGIC_CORRESPOND_TYPE *)&(s_stInterfaceLogicTable_ScGrp);		
	#endif
		else
			return u64RfidData;
		
	/**判断是否有相反的动作**/
	#if ((LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG != DISABLED)&&(ZIBAO_CTRL_ENABLE_FLAG != DISABLED))
		if((stMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)||(stMode == INTERFACE_MODE_SC_ZIBAO_CONTROL))
	#elif (LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG != DISABLED)
		if(stMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
	#elif ((INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)&&(ZIBAO_CTRL_ENABLE_FLAG != DISABLED))
		if((stMode == INTERFACE_MODE_SC_SELF_CONTROL)||(stMode == INTERFACE_MODE_SC_ZIBAO_CONTROL))
	#elif (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
		if(stMode == INTERFACE_MODE_SC_SELF_CONTROL)
	#endif
		{
			for(u16K = 0; u16K < 32; u16K++)	//含有多个键值时必须分开单独处理，因为每个键值对应一个动作序号，只有动作序号才能查询出无线动作码
			{
				u32ShiftBit = (u32)(0x01 << u16K);
				if((u32KeyValueBackup & u32ShiftBit) == 0x00) 
					continue;
				else
					u32OneKeyOfKeyValue = u32ShiftBit;
				for(u16I = 0x00; u16I < 12; u16I++)		//将键值转换为按键序号(0~11)	
				{
					if(s_stInterfaceKeySNTable[u16I] == u32OneKeyOfKeyValue)
						break;
				}
//				if(u16I < 12)		//是12个动作键
//				{
//					if(s_u32ShiftKeyEnableFlag > 0)		//通过按键序号和shift值得出动作序号
//						u32OneKeyOfKeyValue = (u32)u16KeySetActionNameRam[u16I + 12];
//					else
//						u32OneKeyOfKeyValue = (u32)u16KeySetActionNameRam[u16I];
//				}

				/**分析出具体执行的动作**/
				for(u16I = 0x00;;u16I++)
				{
					if(p[u16I].stInterfaceCtrlMode == INTERFACE_MODE_NONE)
						break;

					if((p[u16I].stInterfaceCtrlMode == stMode)&&(u32OneKeyOfKeyValue == p[u16I].u32InterfaceKeyValue))
					{
						MultiKeyActionTable[u16J].u32KeyValueSingle = u32ShiftBit;	//对每个动作都进行处理，最终建立《按键键值与动作无线码的关系表》
						MultiKeyActionTable[u16J].u64RfidSendActionDataSingle = p[u16I].u64RfidSendActionData;
						u16J++;
					 
						u32KeyValueBackup ^= u32ShiftBit;
						break;
					}
				}
				if(u32KeyValueBackup == 0x00)	
					break;
			}
			
			if(u16J < 2)	//不含相反动作
				return u64RfidData;
			else
			{
				u32KeyValueBackup = *pKeyValue;
				for(u16I = 0x00; u16I < (u16J - 1); u16I++)  //对《按键键值与动作无线码的关系表》逐行判断：是否符合s_stRfidReverseActionTable中的约束条件
				{
					for(u16K = (u16I + 1); u16K < u16J ; u16K++)
					{
						for(u16L = 0x00; ;u16L++)
						{
							if(( s_stRfidReverseActionTable[u16L].u64RfidActionName) == RFID_ACTION_NONE)
								break;
							if((MultiKeyActionTable[u16I].u64RfidSendActionDataSingle | MultiKeyActionTable[u16K].u64RfidSendActionDataSingle)\
								== (s_stRfidReverseActionTable[u16L].u64RfidActionName|s_stRfidReverseActionTable[u16L].u64RfidReverseName))
							{				
								u32KeyValueBackup &= ~(MultiKeyActionTable[u16I].u32KeyValueSingle);
								u32KeyValueBackup &= ~(MultiKeyActionTable[u16K].u32KeyValueSingle);
								*pKeyValue = u32KeyValueBackup;		
								return u64RfidData;		//不会同时存在多个相反动作
							}
						}
					}
				}
			}
		} 

	*pKeyValue = u32KeyValueBackup;
	
	return u64RfidData;
}

/*********************************************************************************
** 函数名称:SetScrAwake
** 功能描述：唤醒屏幕
** 函数输入：
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32 SetScrAwake(void)
{
	u16 u16Brightness;
	
	if ((s_stScrAwake == INTERFACE_SCR_PROTECED) || (s_stScrAwake == INTERFACE_SCR_CLOSED))
	{
		#if 1
			OledShutOn();
			LogicParamApi(LOGIC_GET_OLED_SCR_LIGHT,&u16Brightness);	
			OledContrastAdj(u16Brightness);	//亮度参数		
		#else
			LogicParamApi(LOGIC_GET_OLED_SCR_LIGHT,&u16Brightness);	
			OledContrastAdj(u16Brightness);	//亮度参数
		#endif
		
		s_stScrAwake = INTERFACE_SCR_AWAKE;
	}
	
	LogicParamApi(LOGIC_GET_OLED_PROTECTED_TIMER,&s_u32ScrPrtectedTimer);
	if(s_u32ScrPrtectedTimer == 0x00)
		s_u32ScrPrtectedTimer = INTERFACE_SCR_TIMER_STOP;
	
	s_u32AutoShutdownTimer = INTERFACE_AUTOSHUTDOWN_TIMER_STOP;//自动关机停止计数
	
	return 0x01;
}

/*********************************************************************************
** 函数名称:SetScrClosed
** 功能描述：关闭屏幕
** 函数输入：
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32 SetScrClosed(void)
{
	LcdClear();
	
	OledContrastAdj(0x00);	//亮度参数
	
	s_stScrAwake = INTERFACE_SCR_CLOSED;
	
	return 0x01;
}
/*********************************************************************************
** 函数名称:SetScrProtected
** 功能描述：屏幕保护
** 函数输入：
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32 SetScrProtected(void)
{
	u16 u16Brightness;
	
	if (s_stScrAwake == INTERFACE_SCR_AWAKE)
	{
		LogicParamApi(LOGIC_GET_OLED_PROTECT_BRIGHTNESS,&u16Brightness);	
		
		if(u16Brightness == 0x00)
			OledShutDown();
		else
			OledContrastAdj(u16Brightness);	//亮度参数
		
		s_stScrAwake = INTERFACE_SCR_PROTECED;
		s_u32ScrPrtectedTimer = INTERFACE_SCR_TIMER_STOP;

		LogicParamApi(LOGIC_GET_OLED_AUTOSHUTDOWN_TIMER,&s_u32AutoShutdownTimer);
		if(s_u32AutoShutdownTimer == 0x00)//当为0时，立即关机
			s_u32AutoShutdownTimer = INTERFACE_TASK_TIME_SET*2;	
	}

	return 0x01;	
}

/*********************************************************************************
** 函数名称:InterfaceInsertNote
** 功能描述：设置提示信息(内部及外部均可使用)
** 函数输入：
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32 InterfaceInsertNote(INTERFACE_NOTE_TYPE stNoteType)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
			OS_CPU_SR  cpu_sr = 0;
	#endif
	#if (BOARD_BATTERY_TEST == BOARD_BATTERY_COULOME_NUMBER)//此标志定义在电includes中
		char s[20];
	#endif
	#if (BOARD_BATTERY_TEST == BOARD_BATTERY_COULOME_NUMBER)//此标志定义在电includes中
		u16 u16Temp;
	#endif
	
	OS_ENTER_CRITICAL(); 

	s_u8NoteItem[s_u16NoteItemWrite][0x00] = 0x00;
	strcat((char *)&s_u8NoteItem[s_u16NoteItemWrite],(char *)&u8RunInfoMsg[stNoteType]);
	#if (BOARD_BATTERY_TEST == BOARD_BATTERY_COULOME_NUMBER)//此标志定义在电includes中
		if(stNoteType == INTERFACE_NOTE_BATTERY_INCHARGE)
		{
			LogicRunInfApi(LOGIC_GET_BATTERY_COULOM_DATA, &u16Temp);
			sprintf(s,",%d%s",u16Temp,"%");
			strcat((char *)&s_u8NoteItem[s_u16NoteItemWrite],s);
		}
	#endif
	s_u16NoteItemWrite++;
	s_u16NoteItemWrite %= INTERFACE_NOTE_ENUM_MAX;

	OS_EXIT_CRITICAL();

	return 0x01;
}

/*********************************************************************************
** 函数名称:InterfaceSetCtrlMode
** 功能描述：设置主屏控制方式(内部及外部均可使用)
** 函数输入：stNewMode，新状态
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32 InterfaceSetCtrlMode(INTERFACE_CONTROL_MODE_TYPE stNewMode)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
			OS_CPU_SR  cpu_sr = 0;
	#endif
	
	OS_ENTER_CRITICAL(); 

	s_stInterfaceControlMode = stNewMode;

	#if ((ZIBAO_CTRL_ENABLE_FLAG	==	DISABLED)\
		||((ZIBAO_CTRL_ENABLE_FLAG	==	ENABLED)&&(ZIBAO_CTRL_METHOD_TYPE == ZIBAO_MANUAL_CTRL_METHOD)))
		LogicParamApi(LOGIC_GET_INTER_CONTROL_TIMER,&s_u32InterfaceControlTimer);
		#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
			if((stNewMode == INTERFACE_MODE_TRANS_PRG)\
				||(stNewMode == INTERFACE_MODE_RECV_PRG_ING)\
			    ||(stNewMode == INTERFACE_MODE_RECV_PRG_START)
				||(stNewMode == INTERFACE_MODE_ACTIONINFI_SYN))		//配置文件同步	zzw
				s_u32InterfaceControlTimer = INTERFACE_PRGSET_TIMER_SET;
		#endif
		if(!s_u32InterfaceControlTimer)
			s_u32InterfaceControlTimer = INTERFACE_INTERCTRL_TIMER_MAX;

		s_u32ScCtrlKeyTimer = 0x00;
		LogicParamApi(LOGIC_GET_SC_CTRL_DISPALY_TIMER,&s_u32ScCtrlKeyTimer);
		if(!s_u32ScCtrlKeyTimer)
			s_u32ScCtrlKeyTimer = INTERFACE_INTERCTRL_TIMER_MAX;	
	#else
		if (stNewMode !=  INTERFACE_MODE_SC_ZIBAO_CONTROL)
		{
			LogicParamApi(LOGIC_GET_INTER_CONTROL_TIMER,&s_u32InterfaceControlTimer);
			#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
			if((stNewMode == INTERFACE_MODE_TRANS_PRG)\
				||(stNewMode == INTERFACE_MODE_RECV_PRG_ING)\
				||(stNewMode == INTERFACE_MODE_RECV_PRG_START))			
				s_u32InterfaceControlTimer = INTERFACE_PRGSET_TIMER_SET;
			#endif
			if(!s_u32InterfaceControlTimer)
				s_u32InterfaceControlTimer = INTERFACE_INTERCTRL_TIMER_MAX;

			s_u32ScCtrlKeyTimer = 0x00;
			LogicParamApi(LOGIC_GET_SC_CTRL_DISPALY_TIMER,&s_u32ScCtrlKeyTimer);
			if(!s_u32ScCtrlKeyTimer)
				s_u32ScCtrlKeyTimer = INTERFACE_INTERCTRL_TIMER_MAX;	
		}
		else
		{
			LogicParamApi(LOGIC_GET_INTER_CONTROL_TIMER,&s_u32InterfaceControlTimer);
			if(!s_u32InterfaceControlTimer)
				s_u32InterfaceControlTimer = INTERFACE_INTERCTRL_TIMER_MAX;

			s_u32ScCtrlKeyTimer = 0x00;
			LogicParamApi(LOGIC_GET_ZIBAO_TIMEOUT,&s_u32ScCtrlKeyTimer);//此处获取自保超时时间
			if(!s_u32ScCtrlKeyTimer)
				s_u32ScCtrlKeyTimer = INTERFACE_TASK_TIME_SET;	

			LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &s_u32InterfaceColibrateTimer);			
		}
	#endif
	
	OS_EXIT_CRITICAL();
	
	return 0x01;
}

/*********************************************************************************
** 函数名称:InterfaceGetCtrlMode
** 功能描述：获取主屏控制方式(内部及外部均可使用)
** 函数输入：stNewMode，新状态
** 函数输出：None
** 注意事项：
**********************************************************************************/
INTERFACE_CONTROL_MODE_TYPE InterfaceGetCtrlMode(void)
{
  	return s_stInterfaceControlMode;
}

/*********************************************************************************
** 函数名称:InterfaceGetActionNameNumb
** 功能描述：获取主屏显示动作的序列号(内部及外部均可使用)
** 函数输入：stNewMode，新状态
** 函数输出：None
** 注意事项：
**********************************************************************************/
#if ((QUICK_KEY_CTRL_ENABLE_FLAG != DISABLED)||((ZIBAO_CTRL_ENABLE_FLAG != DISABLED)&&(ZIBAO_CTRL_METHOD_TYPE == ZIBAO_PARAM_CTRL_METHOD)))
u32 InterfaceGetActionNameNumb(const u8 *s,u32 *pNumb)
{
	u32 u32I = 0x00;
	u32 u32ReturnFlag = 0x00;
	
	for(u32I = 0x00;u32I < sizeof(u8OperatInfoMsg); u32I++)
	{
		if((strcmp((const char *)s,(const char *)u8OperatInfoMsg[u32I])) == 0x00)	
		{
			u32ReturnFlag = 0x01;
			*pNumb = u32I;
			break;
		}
	}

	return u32ReturnFlag;	
}
#endif

/*********************************************************************************
** 函数名称:InterfaceSetActionName
** 功能描述：设置更新主屏的方式(内部及外部均可使用)
** 函数输入：u16All，全部显示还是部分显示
** 函数输出：None
** 注意事项：
**********************************************************************************/
#if ((QUICK_KEY_CTRL_ENABLE_FLAG != DISABLED)||((ZIBAO_CTRL_ENABLE_FLAG != DISABLED)&&(ZIBAO_CTRL_METHOD_TYPE == ZIBAO_PARAM_CTRL_METHOD)))
	u32 InterfaceSetActionName(INTERFACE_ACTION_TYPE *pName)
	{
		u16 u16I;
		#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
				OS_CPU_SR  cpu_sr = 0;
		#endif
		
		OS_ENTER_CRITICAL(); 	
		for(u16I = 0x00; u16I < INTERFACE_KEY_TOGETHER_MAX;u16I++)
		{
			s_stInterfaceActionName[u16I] = *pName;//默认屏幕显示
			pName++;
		}

		OS_EXIT_CRITICAL();
		
		return 0x01;
	}
#endif

/*********************************************************************************
** 函数名称:InterfaceGetActionName
** 功能描述：设置更新主屏的方式(内部及外部均可使用)
** 函数输入：u16All，全部显示还是部分显示
** 函数输出：None
** 注意事项：
**********************************************************************************/
#if ((QUICK_KEY_CTRL_ENABLE_FLAG != DISABLED)||((ZIBAO_CTRL_ENABLE_FLAG != DISABLED)&&(ZIBAO_CTRL_METHOD_TYPE == ZIBAO_PARAM_CTRL_METHOD)))
	u32 InterfaceGetActionName(INTERFACE_ACTION_TYPE *pName)
	{
		u16 u16I;
		#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
				OS_CPU_SR  cpu_sr = 0;
		#endif
		
		OS_ENTER_CRITICAL(); 	
		for(u16I = 0x00; u16I < INTERFACE_KEY_TOGETHER_MAX;u16I++)
		{
			*pName = s_stInterfaceActionName[u16I];//默认屏幕显示
			pName++;
		}

		OS_EXIT_CRITICAL();
		
		return 0x01;
	}
#endif
/*********************************************************************************
** 函数名称:InterfaceGetZibaoActionKey
** 功能描述：回读自保模式的按键存储值
** 函数输入：pData，数据指针
** 函数输出：None
** 注意事项：
**********************************************************************************/
#if ((ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)&&(ZIBAO_CTRL_METHOD_TYPE == ZIBAO_PARAM_CTRL_METHOD))
	u32 InterfaceGetZibaoActionKey(u32 *pData)
	{
		#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
				OS_CPU_SR  cpu_sr = 0;
		#endif

		OS_ENTER_CRITICAL(); 	
		
		*(u32 *)pData =  s_u32ZibaoActionKeyBackup;
		
		OS_EXIT_CRITICAL();
		
		return 0x01;
	}
#endif
/*********************************************************************************
** 函数名称:InterfaceSetZibaoActionKey
** 功能描述：设置自保模式的按键存储值
** 函数输入：pData，数据指针
** 函数输出：None
** 注意事项：
**********************************************************************************/
#if ((ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)&&(ZIBAO_CTRL_METHOD_TYPE == ZIBAO_PARAM_CTRL_METHOD))
	u32 InterfaceSetZibaoActionKey(u32 *pData)
	{
		#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
				OS_CPU_SR  cpu_sr = 0;
		#endif

		OS_ENTER_CRITICAL(); 	
		
		s_u32ZibaoActionKeyBackup= *(u32 *)pData;
		
		OS_EXIT_CRITICAL();
		
		return 0x01;
	}
#endif
/*********************************************************************************
** 函数名称:InterfaceSetZibaoActionKey
** 功能描述：设置自保模式的按键存储值
** 函数输入：pData，数据指针
** 函数输出：None
** 注意事项：
**********************************************************************************/
#if ((ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)&&(ZIBAO_CTRL_METHOD_TYPE == ZIBAO_PARAM_CTRL_METHOD))
	u32 InterfaceSetZibaoRfidBackup(u32 *pData)
	{
		#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
				OS_CPU_SR  cpu_sr = 0;
		#endif

		OS_ENTER_CRITICAL(); 	
		
		s_u64InterfaceRfidDataBackup = *(u32 *)pData;
		
		OS_EXIT_CRITICAL();
		
		return 0x01;
	}
#endif

/*********************************************************************************
** 函数名称:InterfaceSetZibaoKeyLedStatus
** 功能描述：设置自保模式时的
** 函数输入：pData，数据指针
** 函数输出：None
** 注意事项：
**********************************************************************************/
#if ((ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)&&(ZIBAO_CTRL_METHOD_TYPE == ZIBAO_PARAM_CTRL_METHOD))
static u32 ShowScZibaoKeyLedProc(u32 sNewKey,u32 u32KeyStatus,u32 u32ShiftKeyStatus);
u32 InterfaceSetZibaoKeyLedStatus(u32 sNewKey,u32 u32KeyStatus)
{
	return (ShowScZibaoKeyLedProc(sNewKey,u32KeyStatus,s_u32ShiftKeyEnableFlag));
}
#endif

/*******************************************************************************************
**函数名称：InterfaceSetDownloadRemainTimer
**函数作用：设置s_u32DownloadRemainTimer的值
**函数参数：无
**注意事项：无
*******************************************************************************************/
#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
	void InterfaceSetDownloadRemainTimer(u32 u32value)
	{
		#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
				OS_CPU_SR  cpu_sr = 0;
		#endif
		
		OS_ENTER_CRITICAL(); 
		
		s_u32DownloadRemainTimer = u32value;
		/**只显示10秒的整数倍**/
		s_u32DownloadRemainTimerBackup = (u32value/1000);//秒
		s_u32DownloadRemainTimerBackup /= 10;
		s_u32DownloadRemainTimerBackup ++;
		s_u32DownloadRemainTimerBackup ++;//确保每次设置时间均能正确显示
		s_u32DownloadRemainTimerBackup *= 10;
		s_u32DownloadRemainTimerBackup *= 1000;		
		
		OS_EXIT_CRITICAL();		
	}
#endif

/*****************************************************************************************
**函数名称：InterfaceDecDownloadRemainTimer
**函数作用：减少s_u32DownloadRemainTimer的值
**函数参数：无
**注意事项：无
*******************************************************************************************/
#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
	void InterfaceDecDownloadRemainTimer(u32 u32value)
	{
		#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
				OS_CPU_SR  cpu_sr = 0;
		#endif
	
		OS_ENTER_CRITICAL(); 
		
		if(s_u32DownloadRemainTimer >= u32value)
			s_u32DownloadRemainTimer -= u32value;
		else
			s_u32DownloadRemainTimer = 0x00;
		
		OS_EXIT_CRITICAL();	
	}
#endif
/*****************************************************************************************************************************************
** 函数名   ：FormatRemainTimer()
** 函数作用 ：格式化时间数据
** 输入参数 :	u16 u16Prs：压力数据, u8 *u8dst
** 返回值   ：无
** 作者     ：
** 日期     ：2013-7-24
** 备注     ：
****************************************************************************************************************************************/
#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
	void FormatRemainTimer(u32 u32time, u8 *u8dst)
	{
		if(u32time)
		{
			u8dst[0] = (u8)((u32time /1000/ 60 /10) + 0x30);//分钟的个位
			u8dst[1] = (u8)((u32time /1000/ 60)%10 + 0x30);//分钟的个位
			u8dst[2] = (u8)((((u32time/1000) % 60)/10)+ 0x30);//秒的十位
			u8dst[3] = (u8)((((u32time/1000) % 60)%10)+ 0x30);//秒的个位
		}
		else
		{
			u8dst[0] = 0x30;//分钟的个位
			u8dst[1] = 0x30;//分钟的个位
			u8dst[2] = 0x30;//秒的十位
			u8dst[3] = 0x30;//秒的个位		
		}
	}
#endif

/*********************************************************************************
** 函数名称:SetUpdateMainMethod
** 功能描述：设置更新主屏的方式(内部及外部均可使用)
** 函数输入：u16All，全部显示还是部分显示
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32 InterfaceSetUpdateMainMethod(u32  u32ShowType)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
			OS_CPU_SR  cpu_sr = 0;
	#endif
	
	OS_ENTER_CRITICAL(); 

	s_u32MainUpdateMethod |= u32ShowType;

	OS_EXIT_CRITICAL();

	return 0x01;
}
/*********************************************************************************
** 函数名称:ClearUpdateMainMethod
** 功能描述：设置更新主屏的方式为无效(内部刷新屏幕之后可使用)
** 函数输入：u16All，全部显示还是部分显示
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32 ClearUpdateMainMethod(void)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
			OS_CPU_SR  cpu_sr = 0;
	#endif
	
	OS_ENTER_CRITICAL(); 

	s_u32MainUpdateMethod  =  INTERFACE_SHOW_MAIN_NONE;

	OS_EXIT_CRITICAL();

	return 0x01;
}
 
/*********************************************************************************
** 函数名称:ShowMainAll
** 功能描述：显示屏幕的所有信息(依据工作模式进行屏幕的所有更新)
** 函数输入：none
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32  ShowMainAll(u32 u32Type)
{
	u8 u8I = 0x00;
	u16 u16I = 0x00;
	u16 u16J = 0x00,u16K = 0x00;
	u16 u16Station = 0x00,u16Direction = 0x00;
	char s[20] ;
	u32 u32ReturnFlag = 0x01;
	u32 u32ButtonValue = 0x00;
	#if (LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG !=	DISABLED)
		u32 u32ActionType = 0x00,u32GeJiaNumber = 0x00;
	#endif
	
	/**所有模式下的通用处理**/
	LcdClear();
	//显示信号强度
	LogicRunInfApi(LOGIC_GET_RFID_SIGNAL_STATUS, &u16I);
	ShowSymbol(OLED_DISPLAY_NORMAL,c_u16InterfaceRfidSignalSymbol[u16I],0,4);

	LogicRunInfApi(LOGIC_GET_BATTERY_COULOM_STATUS,&u8I);
	ShowSymbol(OLED_DISPLAY_NORMAL,c_u16InterfaceBatterSymbol[u8I],0,112);
     //无线测试
	LogicParamApi(LOGIC_GET_WL_TEST_ENABLE, &u8I);
    if(u8I == DISABLE)	
    {
        LogicParamApi(LOGIC_GET_YKQ_NUMBER,&u8I);
        LogicParamApi(LOGIC_GET_CTRL_MODE,&u16J);
        if(u16J == MANUAL_CTRL)
            sprintf(s,"MC-%d",u8I);
        else if(u16J == AUTO_CTRL)
            sprintf(s,"AC-%d",u8I);
        else
            sprintf(s,"WL-%d",u8I);
        ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 0, 30);
    }

	LogicRunInfApi(LOGIC_GET_RUN_STATUS,&u8I);
	LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16J);
	#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG != DISABLED)
		if((s_stInterfaceControlMode == INTERFACE_MODE_IDLE)||(u8I == LOGIC_STATUS_IDLE)\
			||(u8I == LOGIC_STATUS_ININT)||(u8I == LOGIC_STATUS_RFID_UNCOLIBRATE)\
			||(u8I == LOGIC_STATUS_SHUTUP)||(u8I == LOGIC_STATUS_RFID_TRANS_PRG)\
			||(u8I == LOGIC_STATUS_RFID_RECV_PRG)||(u8I == LOGIC_STATUS_RFID_TRANS_WL_PRG)\
			||(u8I == LOGIC_STATUS_USART_TRANS_WL_PRG)||(u8I == LOGIC_STATUS_RFID_CXB_RECV_PRG)\
			||(u8I == LOGIC_STATUS_RFID_CXB_RECV_WL_PRG)||(u8I == LOGIC_STATUS_RFID_RECV_WL_PRG)\
		  ||(u8I == LOGIC_STATUS_UNCONNECT_WL))	
	#else
		if((s_stInterfaceControlMode == INTERFACE_MODE_IDLE)||(u8I == LOGIC_STATUS_IDLE)\
			||(u8I == LOGIC_STATUS_ININT)||(u8I == LOGIC_STATUS_RFID_UNCOLIBRATE)\
			||(u8I == LOGIC_STATUS_SHUTUP))
	#endif		
	{
		sprintf(s,"!000");
	}
	else
	{
		LogicRunInfApi(LOGIC_GET_SC_NUMB_ADD_DIRCT,&u8I);
		if(u8I == SC_DIRCT_RIGHT)
			sprintf(s,">%d",u16J);
		else if(u8I == SC_DIRCT_LEFT)
			sprintf(s,"<%d",u16J);
		else
			sprintf(s,"?%d",u16J);
	}
	ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 0, 76);

	/**区分模式的不同处理**/
	/**空闲模式**/
	if(s_stInterfaceControlMode == INTERFACE_MODE_IDLE||s_stInterfaceControlMode == INTERFACE_MODE_WL_UNCONNECT)
	{
		#if (INTERFACE_DISPLAY_SPECIAL_PRODUCTLOG_FLAG != DISABLED)
			memcpy(s,u8RunInfoMsg[INTERFACE_NOTE_ZHONGJIANJIA_YKQ],sizeof(u8RunInfoMsg[INTERFACE_NOTE_ZHONGJIANJIA_YKQ]));
		#else
			memcpy(s,u8RunInfoMsg[INTERFACE_NOTE_HUANING_YKQ],sizeof(u8RunInfoMsg[INTERFACE_NOTE_HUANING_YKQ]));
		#endif
		u16Station = strlen(s);
		u16Station = (128-u16Station*8)/2;		
		ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 3, u16Station);		

	}
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
		else if(s_stInterfaceControlMode == INTERFACE_MODE_SC_UNCERTIFIED)
		{
			LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16J);
			sprintf(s,"%d#,本架控制",u16J);
			u16Station = strlen(s);
			u16Station = (128-u16Station*8)/2;	
			ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 2, u16Station);
			
		}
	#endif
	else if(s_stInterfaceControlMode == INTERFACE_MODE_SC_SELF_CONTROL)
	{
		LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16J);
		#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
			sprintf(s,"%d#,本架控制",u16J);
		#else
			sprintf(s,"%d#,本架控制",u16J);
		#endif
		
		u16Station = strlen(s);
		u16Station = (128-u16Station*8)/2;	
		ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 2, u16Station);
		
	}	
	else if(s_stInterfaceControlMode == INTERFACE_MODE_SC_SET)
	{
		LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16J);
		sprintf(s,"%d#,菜单设置",u16J);
		u16Station = strlen(s);
		u16Station = (128-u16Station*8)/2;			
		ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 2, u16Station);		
	}
	#if (LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG !=	DISABLED)
		else if(s_stInterfaceControlMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)	
		{
			LogicRunInfApi(LOGIC_GET_SC_ACTION_TYPE,&u32ActionType);
			LogicRunInfApi(LOGIC_GET_SC_GEJIA_NUMBER,&u32GeJiaNumber);
			LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16J);
			LogicRunInfApi(LOGIC_GET_CONTRL_SC_NUMB,&u16I);  //jhy
			if(u32GeJiaNumber== 0x00)//邻架操作
			{
				if(u32ActionType  == SC_ACTION_LEFT_CTRL)
				{
					sprintf(s,"%d#,%s%d#",u16J,"控制",u16I);//jhy
				}
				else if(u32ActionType  == SC_ACTION_RIGHT_CTRL)
				{
//					sprintf(s,"%d#,%s",u16J,"右邻架");
					sprintf(s,"%d#,%s%d#",u16J,"控制",u16I);//jhy
				}
				else
				{
					sprintf(s,"%d#,%s",u16J,"本架控制");
				}
			}
			else
			{
				if(u32ActionType  == SC_ACTION_LEFT_CTRL)
				{
					sprintf(s,"%d#,%s%d#",u16J,"控制",u16I);//jhy
//					sprintf(s,"%d#,左隔%d架",u16J,u32GeJiaNumber);
				}
				else if(u32ActionType  == SC_ACTION_RIGHT_CTRL)
				{
					sprintf(s,"%d#,%s%d#",u16J,"控制",u16I);//jhy
//					sprintf(s,"%d#,右隔%d架",u16J,u32GeJiaNumber);
				}
				else
					;
			}
			u16Station = strlen(s);
			u16Station = (128-u16Station*8)/2;	
			ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 2, u16Station);	
			u8I = 0x00;
			sprintf(s,"%s",u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
			u8I++;
			if(s_stInterfaceActionName[u8I] != INTERFACE_ACTION_NONE)
			{
				for(;u8I < INTERFACE_KEY_TOGETHER_MAX;u8I++)
				{
					if(s_stInterfaceActionName[u8I] != INTERFACE_ACTION_NONE)
						strcat(s,(char *)&u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
				}
				
				u16Station = strlen(s);
				if(u16Station*8 < 128)
				{
					u8I = 0x00;
					sprintf(s,"%s",u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
					u8I++;
					for(;u8I < INTERFACE_KEY_TOGETHER_MAX;u8I++)
					{
						strcat(s,",");
						strcat(s,(char *)&u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
					}			
				}
			}
			u16Station = strlen(s);
			if(u16Station > 16)
			{	
				u16Station= 16;//防止刷新至下一行
				s[16] = 0x00;
			}
			u16Station = (128-u16Station*8)/2;	
			ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 4, u16Station);			
			
		}	
	#endif	
	#if (GRP_CTRL_ENABLE_FLAG != DISABLED)	
	else if(s_stInterfaceControlMode == INTERFACE_MODE_SC_GRP_CONTROL)
	{
		LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u32ButtonValue);//近端架号
		if(((u32ButtonValue&RFID_STATUS_START) != RFID_STATUS_START)&&((u32ButtonValue&RFID_STATUS_NEIGHBOR_LEFT) != RFID_STATUS_NEIGHBOR_LEFT)&&((u32ButtonValue&RFID_STATUS_NEIGHBOR_RIGHT) != RFID_STATUS_NEIGHBOR_RIGHT))
		{
			LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16J);
			u8I = 0x00;
			sprintf(s,"%s",u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
			u8I++;
			if(s_stInterfaceActionName[u8I] != INTERFACE_ACTION_NONE)
			{
				for(;u8I < INTERFACE_KEY_TOGETHER_MAX;u8I++)
				{
					if(s_stInterfaceActionName[u8I] != INTERFACE_ACTION_NONE)
						strcat(s,(char *)&u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
				}
				
				u16Station = strlen(s);
				if(u16Station*8 < 128)
				{
					u8I = 0x00;
					sprintf(s,"%s",u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
					u8I++;
					for(;u8I < INTERFACE_KEY_TOGETHER_MAX;u8I++)
					{
						strcat(s,",");
						strcat(s,(char *)&u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
					}			
				}
			}
		
			u16Station = strlen(s);
			if(u16Station > 16)
			{	
				u16Station= 16;//防止刷新至下一行
				s[16] = 0x00;
			}
			u16Station = (128-u16Station*8)/2;
			ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 2, u16Station);			
		}
		LcdAreaClear(OLED_DISPLAY_NORMAL,4,0,5,111);
		LogicRunInfApi(LOGIC_GET_GRP_ACTION_DIRECTION,&u16Direction);
		
	
		if((u16Direction == SC_GRP_ACTION_NEAR_REMOTE) || (u16Direction == SC_GRP_ACTION_REMOTE_NEAR))
		{
			LogicRunInfApi(LOGIC_GET_GRP_NEAR_SC_NUMB,&u16I);//近端架号
			LogicRunInfApi(LOGIC_GET_GRP_REMOTE_SC_NUMB,&u16J);//远端架号
			LogicRunInfApi(LOGIC_GET_SC_NUMB_ADD_DIRCT, &u16K);//发送方向
			LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB, &u16Station);//对码架号

			if(u16K == SC_DIRCT_RIGHT)
			{
				if(u16I <= u16Station)
				{
					if(u16Direction == SC_GRP_ACTION_REMOTE_NEAR)
					{
						if((u32ButtonValue&RFID_STATUS_START) != RFID_STATUS_START)
							sprintf(s,"%d -> %d", u16J,u16I);
						else
							sprintf(s,"%d->%d", u16J,u16I);
						if((u16I == 0x1FF)&&(u16J == 0x1FF))
							sprintf(s,"*** -> ***");
						
					}
					else
					{
						if((u32ButtonValue&RFID_STATUS_START) != RFID_STATUS_START)
							sprintf(s,"%d <- %d",  u16J,u16I);
						else
							sprintf(s,"%d<-%d",  u16J,u16I);
						if((u16I == 0x1FF)&&(u16J == 0x1FF))
							sprintf(s,"*** <- **");
						
					}
				}
				else
				{
					if(u16Direction == SC_GRP_ACTION_REMOTE_NEAR)
					{
						if((u32ButtonValue&RFID_STATUS_START) != RFID_STATUS_START)
							sprintf(s,"%d <- %d", u16I,u16J);
						else
							sprintf(s,"%d<-%d", u16I,u16J);
						if((u16I == 0x1FF)&&(u16J == 0x1FF))
							sprintf(s,"*** <- ***");
						
					}
					else
					{
						if((u32ButtonValue&RFID_STATUS_START) != RFID_STATUS_START)
							sprintf(s,"%d -> %d",  u16I,u16J);
						else
							sprintf(s,"%d->%d",  u16I,u16J);
						if((u16I == 0x1FF)&&(u16J == 0x1FF))
							sprintf(s,"*** -> ***");
						
					}
					
				}				
			}
			else
			{
				if(u16I >= u16Station)
				{
					if(u16Direction == SC_GRP_ACTION_REMOTE_NEAR)
					{
						if((u32ButtonValue&RFID_STATUS_START) != RFID_STATUS_START)
							sprintf(s,"%d -> %d", u16J,u16I);
						else
							sprintf(s,"%d->%d", u16J,u16I);
						if((u16I == 0x1FF)&&(u16J == 0x1FF))
							sprintf(s,"*** -> ***");
						
					}
					else
					{
						if((u32ButtonValue&RFID_STATUS_START) != RFID_STATUS_START)
							sprintf(s,"%d <- %d",  u16J,u16I);
						else
							sprintf(s,"%d<-%d",  u16J,u16I);
						if((u16I == 0x1FF)&&(u16J == 0x1FF))
							sprintf(s,"*** <- ***");
						
					}
					
				}
				else
				{

					if(u16Direction == SC_GRP_ACTION_REMOTE_NEAR)
					{
						if((u32ButtonValue&RFID_STATUS_START) != RFID_STATUS_START)
							sprintf(s,"%d <- %d", u16I,u16J);
						else
							sprintf(s,"%d<-%d", u16I,u16J);
						if((u16I == 0x1FF)&&(u16J == 0x1FF))
							sprintf(s,"*** <- ***");
					}
					else
					{
						if((u32ButtonValue&RFID_STATUS_START) != RFID_STATUS_START)
							sprintf(s,"%d -> %d", u16I,u16J);
						else
							sprintf(s,"%d->%d", u16I,u16J);
						if((u16I == 0x1FF)&&(u16J == 0x1FF))
							sprintf(s,"***->***");
						
					}				
				}				
			
			}	

			if((u32ButtonValue&RFID_STATUS_START) == RFID_STATUS_START)
				strcat(s," ,启动键");
			u16Station = strlen(s);
			if(u16Station > 16)
			{	
				u16Station= 16;//防止刷新至下一行
				s[16] = 0x00;
			}
			u16Station = (128-u16Station*8)/2;			
			ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 4, u16Station);	
		}
	}
	#endif
	else if(s_stInterfaceControlMode == INTERFACE_MODE_SHUTSOWN)
	{
		#if (INTERFACE_DISPLAY_SPECIAL_PRODUCTLOG_FLAG != DISABLED)
			memcpy(s,u8RunInfoMsg[INTERFACE_NOTE_ZHONGJIANJIA_YKQ],sizeof(u8RunInfoMsg[INTERFACE_NOTE_ZHONGJIANJIA_YKQ]));
		#else
			memcpy(s,u8RunInfoMsg[INTERFACE_NOTE_HUANING_YKQ],sizeof(u8RunInfoMsg[INTERFACE_NOTE_HUANING_YKQ]));
		#endif

		u16Station = strlen(s);
		u16Station = (128-u16Station*8)/2;		
		ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 3, u16Station);	
	}
	#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
		else if((s_stInterfaceControlMode == INTERFACE_MODE_TRANS_PRG)\
				||(s_stInterfaceControlMode == INTERFACE_MODE_RECV_PRG_ING)\
				||(s_stInterfaceControlMode == INTERFACE_MODE_RECV_PRG_START)
				|| (s_stInterfaceControlMode == INTERFACE_MODE_ACTIONINFI_SYN))
		{
			memset(s,0x00,sizeof(s));
			LcdAreaClear(OLED_DISPLAY_NORMAL,2,0,5,127);
			if(s_stInterfaceControlMode == INTERFACE_MODE_TRANS_PRG)
				sprintf(s,"正在传输程序");
			else if(s_stInterfaceControlMode == INTERFACE_MODE_ACTIONINFI_SYN)
				sprintf(s,"配置文件同步");
			else
				sprintf(s,"正在接收程序");			
			u16Station = strlen(s);
			u16Station = (128-u16Station*8)/2;	
			ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 2, u16Station);	

			memset(s,0x00,sizeof(s));
			ShowStr(0, "约剩余", 4, 8);	
			FormatRemainTimer(s_u32DownloadRemainTimer,(u8 *)s);
			ShowStr(OLED_DISPLAY_NORMAL,(const u8 *)s,4,56);
			ShowStr(OLED_DISPLAY_NORMAL,"分",4,72);
			ShowStr(OLED_DISPLAY_NORMAL,((const u8 *)s)+2,4,88);
			ShowStr(OLED_DISPLAY_NORMAL,"秒",4,104);
		}	
	#endif
	#if (ZIBAO_CTRL_ENABLE_FLAG != DISABLED)
		else if(s_stInterfaceControlMode == INTERFACE_MODE_SC_ZIBAO_CONTROL)
		{
			LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16J);
			sprintf(s,"%d#,本架自保控制",u16J);
			u16Station = strlen(s);
			u16Station = (128-u16Station*8)/2;	
			ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 2, u16Station);	
		}
	#endif
	else
		;
	
	if(s_u16NoteItemWrite == 0x00)
		s_u16NoteItemRead = INTERFACE_NOTE_ENUM_MAX-0x01;
	else
		s_u16NoteItemRead = s_u16NoteItemWrite-0x01;

	memcpy(s,s_u8NoteItem[s_u16NoteItemRead],sizeof(s_u8NoteItem[s_u16NoteItemRead]));
	u16Station = strlen(s);
	u16Station = (128-u16Station*8)/2;
	ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 6, u16Station);

	if(s_u8NoteItem[s_u16NoteItemRead] != u8RunInfoMsg[INTERFACE_NOTE_SYSTEM_IDLE])//确定提示信息的显示时间
	{
		LogicParamApi(LOGIC_GET_NOTE_SHOW_TIMER,&s_u32NoteShowTimer);
		if(s_u32NoteShowTimer == 0x00)
			s_u32NoteShowTimer = INTERFACE_NOTE_TIMER_STOP;			
	}	
	return u32ReturnFlag;
}

/*********************************************************************************
** 函数名称:ShowMainBattery
** 功能描述：更新电池信息
** 函数输入：none
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32  ShowMainBattery(u32 u32Type)
{
	u8 u8I;
	u32 u32ReturnFlag = 0x01;
	
	LcdAreaClear(OLED_DISPLAY_NORMAL,0,112,1,127);
	LogicRunInfApi(LOGIC_GET_BATTERY_COULOM_STATUS,&u8I);
	ShowSymbol(OLED_DISPLAY_NORMAL,c_u16InterfaceBatterSymbol[u8I],0,112);	

	return u32ReturnFlag;
}

/*********************************************************************************
** 函数名称:ShowMainRfid
** 功能描述：更新无线信号信息 
** 函数输入：none
** 函数输出：None
** 注意事项：
**********************************************************************************/
extern s8 s8RssiSignalInftest;
extern s8 s8ScRssiSignal;
extern u32 RfidTestTimer;
u32  ShowMainRfid(u32 u32Type)
{
	u16 u16I;
	u32 u32ReturnFlag = 0x01;
    u8 u8I, s[20] ;
    LogicParamApi(LOGIC_GET_WL_TEST_ENABLE, &u8I);
    u16I = (u16)(InterfaceGetCtrlMode());
               
    if((u8I == ENABLE) && (u16I == INTERFACE_MODE_SC_UNCERTIFIED))	
    {
        LcdAreaClear(OLED_DISPLAY_NORMAL,0,0,1,64);
        if(RfidTestTimer != 0xffff)
        {
            //显示信号强度
            LogicRunInfApi(LOGIC_GET_RFID_SIGNAL_STATUS, &u16I);
        //	ShowSymbol(OLED_DISPLAY_NORMAL,c_u16InterfaceRfidSignalSymbol[u16I],0,4);		
            sprintf(s,"%d,%d",s8RssiSignalInftest,s8ScRssiSignal);
            ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)&s, 0, 0);	        
        }
        else
        {
            LcdAreaClear(OLED_DISPLAY_NORMAL,0,4,1,22);
            //显示信号强度
            LogicRunInfApi(LOGIC_GET_RFID_SIGNAL_STATUS, &u16I);
            ShowSymbol(OLED_DISPLAY_NORMAL,c_u16InterfaceRfidSignalSymbol[u16I],0,4);	        
        }
    }	
	else
    {
        LcdAreaClear(OLED_DISPLAY_NORMAL,0,4,1,22);
        //显示信号强度
        LogicRunInfApi(LOGIC_GET_RFID_SIGNAL_STATUS, &u16I);
        ShowSymbol(OLED_DISPLAY_NORMAL,c_u16InterfaceRfidSignalSymbol[u16I],0,4);		
    }
	return u32ReturnFlag;
}

/*********************************************************************************
** 函数名称:ShowMainScColibrate
** 功能描述：更新支架控制器对码架信息
** 函数输入：none
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32  ShowMainScColibrate(u32 u32Type)
{
	u8 u8I;
	u16 u16J;
	char s[20] ;
	u32 u32ReturnFlag = 0x01;
	
	LcdAreaClear(OLED_DISPLAY_NORMAL,0,72,1,111);
	LogicRunInfApi(LOGIC_GET_RUN_STATUS,&u8I);
	LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16J);
	if(u8I == LOGIC_STATUS_IDLE)
	{
		sprintf(s,"!000");
	}
	else
	{
		LogicRunInfApi(LOGIC_GET_SC_NUMB_ADD_DIRCT,&u8I);
		if(u8I == SC_DIRCT_RIGHT)
			sprintf(s,">%d",u16J);
		else if(u8I == SC_DIRCT_LEFT)
			sprintf(s,"<%d",u16J);
		else
			sprintf(s,"?%d",u16J);
	}
	ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)&s, 0, 76);		

	return u32ReturnFlag;
}

/*********************************************************************************
** 函数名称:ShowMainScUnColibrate
** 功能描述：更新支架控制器对码架信息
** 函数输入：none
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32  ShowMainScUnColibrate(u32 u32Type)
{
	u32 u32ReturnFlag = 0x01;

	/**判断是否有正在执行的动作***/
	if((RfidGetDataLimitlessSendFlag()))//正在进行数据的无限次发送
	{
		RfidSetDataLimitlessSendEnd();
	}	
	
	/**确实为解除对码***/
	InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
	StateLed(0x00,KEY_SHIFT_LED);
	StateLed(0x00,ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED);
	#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
		KeyLedOnOff(0x00, KEY_LED_ALL);
	#endif
	#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
		BuzzerOn(BEEP_UNCOLIBRAT);
	#endif
	
	s_u64InterfaceRfidDataBackup = 0x00;
	s_u32StartKeyFlag= KEY_UP;
	s_u32ShiftKeyEnableFlag = DISABLED;
	#if (SPECIAL_FX_ENABLE_FLAG != DISABLED)
		#if (SPECIAL_F1_ENABLE_FLAG != DISABLED)
			s_u32SpecialFxKeyFlag1= KEY_UP;
		#endif
		#if (SPECIAL_F2_ENABLE_FLAG != DISABLED)
			s_u32SpecialFxKeyFlag2= KEY_UP;
		#endif
		#if (SPECIAL_F3_ENABLE_FLAG != DISABLED)
			s_u32SpecialFxKeyFlag3= KEY_UP;
		#endif
		#if (SPECIAL_F4_ENABLE_FLAG != DISABLED)
			s_u32SpecialFxKeyFlag4= KEY_UP;
		#endif
	#endif	

	#if (ZIBAO_CTRL_ENABLE_FLAG != DISABLED)
		s_u32ZibaoActionKeyBackup= 0x00;
	#endif

	#if (GRP_LALIU_BUTTONUP_STOP_FLAG != DISABLED)
		s_u32GrpLaliuKeyFlag = KEY_UP;
	#endif

	u32ReturnFlag = 0x00;
	LogicRunInfApi(LOGIC_SET_SC_NUMB_ADD_DIRCT, &u32ReturnFlag);
	LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB, &u32ReturnFlag);	
	
	s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
	InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);
	OSSemPost(g_stSemInterface);

	return u32ReturnFlag;	
}
/*********************************************************************************
** 函数名称:ShowMainNote
** 功能描述：更新提示信息
** 函数输入：none
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32  ShowMainNote(u32 u32Type)
{
	u32 u32ReturnFlag = 0x01;
	u16 u16Station = 0x00;
	char s[20] ;

	if(s_u16NoteItemWrite == 0x00)
		s_u16NoteItemRead = INTERFACE_NOTE_ENUM_MAX-0x01;
	else
		s_u16NoteItemRead = s_u16NoteItemWrite-0x01;
	memcpy(s,s_u8NoteItem[s_u16NoteItemRead],sizeof(s_u8NoteItem[s_u16NoteItemRead]));
	u16Station = strlen(s);
	u16Station = (128-u16Station*8)/2;	
	if(s_stInterfaceControlMode != INTERFACE_MODE_INCHARGE_DISPLAY)
	{
		LcdAreaClear(OLED_DISPLAY_NORMAL,6,0,7,127);
		ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 6, u16Station);	
	}
	else
	{
		LcdAreaClear(OLED_DISPLAY_NORMAL,5,0,6,127);
		ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 5, u16Station);	
	}
	if(s_u8NoteItem[s_u16NoteItemRead] != u8RunInfoMsg[INTERFACE_NOTE_SYSTEM_IDLE])//确定提示信息的显示时间
	{
		LogicParamApi(LOGIC_GET_NOTE_SHOW_TIMER,&s_u32NoteShowTimer);
		if(s_u32NoteShowTimer == 0x00)
			s_u32NoteShowTimer = INTERFACE_NOTE_TIMER_STOP;			
	}

	return u32ReturnFlag;
}

/*********************************************************************************
** 函数名称:ShowMainNoteDirectly
** 功能描述：更新提示信息
** 函数输入：none
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32  ShowMainNoteDirectly(char *s)
{
	u32 u32ReturnFlag = 0x01;
	u16 u16Station = 0x00;
		
	LcdAreaClear(OLED_DISPLAY_NORMAL,6,0,7,127);

	u16Station = strlen(s);
	u16Station = (128-u16Station*8)/2;	
	ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 6, u16Station);	

	return u32ReturnFlag;
}
/*********************************************************************************
** 函数名称:ShowMainYkqId
** 功能描述：更新遥控器ID
** 函数输入：none
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32  ShowMainYkqId(u32 u32Type)
{
	u8 u8I;
	char s[20] ;
	u32 u32ReturnFlag = 0x01;

	if((u32Type & INTERFACE_SHOW_YKQ_ID_NORMAL) == INTERFACE_SHOW_YKQ_ID_NORMAL)
	{
		LogicParamApi(LOGIC_GET_YKQ_NUMBER,&u8I);
		LcdAreaClear(OLED_DISPLAY_NORMAL,0,30,1,67);
		sprintf(s,"WL-%d",u8I);
		ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)&s, 0, 30);	
	}
	
	else if((u32Type & INTERFACE_SHOW_YKQ_ID_INTERVAL) == INTERFACE_SHOW_YKQ_ID_INTERVAL)
	{
		LogicParamApi(LOGIC_GET_YKQ_NUMBER,&u8I);
		sprintf(s,"WL-");
		ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)&s, 0, 30);
		LcdAreaClear(OLED_DISPLAY_INTERVAL,0,54,1,67);
		sprintf(s,"%d",u8I);
		ShowStr(OLED_DISPLAY_INTERVAL, (const u8 *)&s, 0, 54);			
	}
	else
		;

	return u32ReturnFlag;
}

/*********************************************************************************
** 函数名称:ShowMainCtrlMode
** 功能描述：更新遥控器控制模式
** 函数输入：none
** 函数输出：None
** 注意事项：
**********************************************************************************/
//zzw
u32  ShowMainCtrlMode(u32 u32Type)
{
	u8 u8I;
	char s[20] ;
	u32 u32ReturnFlag = 0x01;
	INTERFACE_CONTROL_MODE_TYPE sMode;
	
	sMode = InterfaceGetCtrlMode();
	if(sMode == INTERFACE_MODE_SELF_SET)
		return 0;
	if((u32Type & INTERFACE_SHOW_CTRL_MODE) == INTERFACE_SHOW_CTRL_MODE)
	{
        LogicParamApi(LOGIC_GET_WL_TEST_ENABLE, &u8I);
        if(u8I == DISABLE)
        {
            LogicParamApi(LOGIC_GET_CTRL_MODE,&u8I);
            LcdAreaClear(OLED_DISPLAY_NORMAL,0,30,1,44);
            if(u8I == AUTO_CTRL)
                sprintf(s,"AC");
            else if (u8I == MANUAL_CTRL)
                sprintf(s,"MC");
            else
                sprintf(s,"WL");
            ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)&s, 0, 30);	        
        }            
	}
	
	return u32ReturnFlag;
}

/*********************************************************************************
** 函数名称:ShowScSet
** 功能描述：更新处于参数设置界面下的显示 
** 函数输入：none
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32  ShowScSet(u32 u32Type)
{
	u8 u8I = 0x00;
	char s[20] ;
	u32 u32ReturnFlag = 0x01;
	u16 u16Station = 0x00;

	LcdAreaClear(OLED_DISPLAY_NORMAL,4,0,5,127);
	u8I = 0x00;
	sprintf(s,"%s",u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
	u8I++;
	if(s_stInterfaceActionName[u8I] != INTERFACE_ACTION_NONE)
	{
		for(;u8I < INTERFACE_KEY_TOGETHER_MAX;u8I++)
		{
			if(s_stInterfaceActionName[u8I] != INTERFACE_ACTION_NONE)
				strcat(s,(char *)&u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
		}
		
		u16Station = strlen(s);
		if(u16Station*8 < 128)
		{
			u8I = 0x00;
			sprintf(s,"%s",u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
			u8I++;
			for(;u8I < INTERFACE_KEY_TOGETHER_MAX;u8I++)
			{
				strcat(s,",");
				strcat(s,(char *)&u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
			}			
		}
	}		
		
	u16Station = strlen(s);
	if(u16Station > 16)
	{	
		u16Station= 16;//防止刷新至下一行	
		s[16] = 0x00;
	}
	u16Station = (128-u16Station*8)/2;		
	ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 4, u16Station);		
		
	
	return u32ReturnFlag;
}

/*********************************************************************************
** 函数名称:ShowScUncertifiedCtrl
** 功能描述：更新处于本架非动作下的显示
** 函数输入：none
** 函数输出：None
** 注意事项：
**********************************************************************************/
#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
u32  ShowScUncertifiedCtrl(u32 u32Type)
{
	u8 u8I;
	char s[20] ;
	u32 u32ReturnFlag = 0x01;
	u16 u16Station = 0x00,u16J;
	
	LcdAreaClear(OLED_DISPLAY_NORMAL,2,0,5,127);
	LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16J);
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
		sprintf(s,"%d#,本架控制",u16J);				//zzw
	#endif
	u16Station = strlen(s);
	u16Station = (128-u16Station*8)/2;	
	ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 2, u16Station);	
	if(RuningActionCnt < INTERFACE_DISPLAY_ACTION_MAX)
	{
		u8I = 0x00;
		//zzw
		sprintf(s,"%s",ActionInfoMsg[u8I].ActionName);
		u8I++;	
		if(u8I < RuningActionCnt)
		{
			for(;u8I < RuningActionCnt;u8I++)
			{
				u16Station = strlen(s);
				if(u16Station*8 < 128)
				{
					strcat(s,",");
					strcat(s,(char *)&ActionInfoMsg[u8I].ActionName);
				}
			}
		}		
	}
	else
	{
		memset(s,0x00,sizeof(s));
		for(u8I = 0x00;u8I < INTERFACE_DISPLAY_ACTION_MAX;u8I++)
		{
			if(u8I== 0x00)
			{
				sprintf(s,"%s",ActionInfoMsg[u8I].ActionName);
				s[(u8I+0x01)*4+u8I] =  ',';
				s[(u8I+0x01)*4+u8I+0x01] = 0x00;
			}
			else
			{
				strcat(s,(char *)&ActionInfoMsg[u8I].ActionName);
				if(u8I != (INTERFACE_DISPLAY_ACTION_MAX-0x01))
				{
					s[(u8I+0x01)*4+u8I] =  ',';
					s[(u8I+0x01)*4+u8I+0x01] = 0x00;
				}
				else
				{
					s[(u8I+0x01)*4+u8I] = 0x00;					
				}
			}						
		}	
		if(RuningActionCnt > INTERFACE_DISPLAY_ACTION_MAX)
			strcat(s,"..");	
	}
	u16Station = strlen(s);
	if(u16Station > 16)
	{	
		u16Station= 16;//防止刷新至下一行
		s[16] = 0x00;
	}
	u16Station = (128-u16Station*8)/2;	
	ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 4, u16Station);		
	return u32ReturnFlag;
}
#endif
/*********************************************************************************
** 函数名称:ShowScSelfCtrl
** 功能描述：更新处于本架控制下的显示
** 函数输入：none
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32  ShowScSelfCtrl(u32 u32Type)
{
	u8 u8I;
	char s[20] ;
	u32 u32ReturnFlag = 0x01;
	u16 u16Station = 0x00,u16J;
	
	LcdAreaClear(OLED_DISPLAY_NORMAL,2,0,5,127);
	LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16J);
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
		sprintf(s,"%d#,本架控制",u16J);
	#else
		sprintf(s,"%d#,本架控制",u16J);
	#endif
	u16Station = strlen(s);
	u16Station = (128-u16Station*8)/2;	
	ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 2, u16Station);	

	for(u16J = 0x00,u8I = 0x00;u8I < INTERFACE_KEY_TOGETHER_MAX;u8I++)
	{
		if(s_stInterfaceActionName[u8I] == INTERFACE_ACTION_NONE)
			break;
		else
			u16J++;
	}
	if(u16J < INTERFACE_DISPLAY_ACTION_MAX)
	{
		u8I = 0x00;
		sprintf(s,"%s",u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
		u8I++;	
		if(s_stInterfaceActionName[u8I] != INTERFACE_ACTION_NONE)
		{
			for(;u8I < INTERFACE_KEY_TOGETHER_MAX;u8I++)
			{
				if(s_stInterfaceActionName[u8I] != INTERFACE_ACTION_NONE)
					strcat(s,(char *)&u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
			}
			
			u16Station = strlen(s);
			if(u16Station*8 < 128)
			{
				u8I = 0x00;
				sprintf(s,"%s",u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
				u8I++;
				for(;u8I < u16J;u8I++)
				{
					strcat(s,",");
					strcat(s,(char *)&u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
				}			
			}
		}
	}	
	else
	{
		memset(s,0x00,sizeof(s));
		for(u8I = 0x00;u8I < INTERFACE_DISPLAY_ACTION_MAX;u8I++)
		{
			if(u8I== 0x00)
			{
				sprintf(s,"%s",u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
				s[(u8I+0x01)*4+u8I] =  ',';
				s[(u8I+0x01)*4+u8I+0x01] = 0x00;
			}
			else
			{
				strcat(s,(char *)&u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
				if(u8I != (INTERFACE_DISPLAY_ACTION_MAX-0x01))
				{
					s[(u8I+0x01)*4+u8I] =  ',';
					s[(u8I+0x01)*4+u8I+0x01] = 0x00;
				}
				else
				{
					s[(u8I+0x01)*4+u8I] = 0x00;					
				}
			}						
		}	
		if(u16J> INTERFACE_DISPLAY_ACTION_MAX)
			strcat(s,"..");
	}
	
	u16Station = strlen(s);
	if(u16Station > 16)
	{
		u16Station= 16;//防止刷新至下一行	
		s[16] = 0x00;
	}
	u16Station = (128-u16Station*8)/2;	
	ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 4, u16Station);			
	
	return u32ReturnFlag;
}

/*********************************************************************************
** 函数名称:ShowScNearCtrl
** 功能描述：更新处于邻架控制界面下的显示
** 函数输入：none
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32  ShowScNearCtrl(u32 u32Type)
{
	u8 u8I = 0x00;
	u16 u16I = 0;
	u32 u32GeJiaNumber,u32ActionType;
	char s[20] ;
	u32 u32ReturnFlag = 0x01;
	u16 u16Station = 0x00,u16J = 0x00;
	
	LcdAreaClear(OLED_DISPLAY_NORMAL,2,0,5,127);
	LogicRunInfApi(LOGIC_GET_SC_ACTION_TYPE,&u32ActionType);
	LogicRunInfApi(LOGIC_GET_SC_GEJIA_NUMBER,&u32GeJiaNumber);
	LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16J);
	
	LogicRunInfApi(LOGIC_GET_CONTRL_SC_NUMB,&u16I);  //jhy
	
	if(u32GeJiaNumber== 0x00)//邻架操作
	{
		if(u32ActionType  == SC_ACTION_LEFT_CTRL)
		{
			sprintf(s,"%d#,%s%d#",u16J,"控制",u16I);//jhy
//			sprintf(s,"%d#,%s",u16J,"左邻架");
		}
		else if(u32ActionType  == SC_ACTION_RIGHT_CTRL)
		{
			sprintf(s,"%d#,%s%d#",u16J,"控制",u16I);//jhy
//			sprintf(s,"%d#,%s",u16J,"右邻架");
		}
		else
		{
			sprintf(s,"%d#,%s",u16J,"本架控制");
		}
	}
	else
	{
		if(u32ActionType  == SC_ACTION_LEFT_CTRL)
		{
			sprintf(s,"%d#,%s%d#",u16J,"控制",u16I);//jhy
//			sprintf(s,"%d#,左隔%d架",u16J,u32GeJiaNumber);
		}
		else if(u32ActionType  == SC_ACTION_RIGHT_CTRL)
		{
			sprintf(s,"%d#,%s%d#",u16J,"控制",u16I);//jhy
//			sprintf(s,"%d#,右隔%d架",u16J,u32GeJiaNumber);
		}
		else
			;
	}

	u16Station = strlen(s);
	u16Station = (128-u16Station*8)/2;	
	ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 2, u16Station);	

	if(RuningActionCnt < INTERFACE_DISPLAY_ACTION_MAX)
	{
		u8I = 0x00;
		//zzw
		sprintf(s,"%s",ActionInfoMsg[u8I].ActionName);
		u8I++;	
		if(u8I < RuningActionCnt)
		{
			for(;u8I < RuningActionCnt;u8I++)
			{
				u16Station = strlen(s);
				if(u16Station*8 < 128)
				{
					strcat(s,",");
					strcat(s,(char *)&ActionInfoMsg[u8I].ActionName);
				}
			}
		}		
	}
	else
	{
		memset(s,0x00,sizeof(s));
		for(u8I = 0x00;u8I < INTERFACE_DISPLAY_ACTION_MAX;u8I++)
		{
			if(u8I== 0x00)
			{
				sprintf(s,"%s",ActionInfoMsg[u8I].ActionName);
				s[(u8I+0x01)*4+u8I] =  ',';
				s[(u8I+0x01)*4+u8I+0x01] = 0x00;
			}
			else
			{
				strcat(s,(char *)&ActionInfoMsg[u8I].ActionName);
				if(u8I != (INTERFACE_DISPLAY_ACTION_MAX-0x01))
				{
					s[(u8I+0x01)*4+u8I] =  ',';
					s[(u8I+0x01)*4+u8I+0x01] = 0x00;
				}
				else
				{
					s[(u8I+0x01)*4+u8I] = 0x00;					
				}
			}						
		}	
		if(RuningActionCnt> INTERFACE_DISPLAY_ACTION_MAX)
			strcat(s,"..");	
	}	
	u16Station = strlen(s);
	if(u16Station > 16)
	{
		u16Station= 16;//防止刷新至下一行	
		s[16] = 0x00;
	}
	u16Station = (128-u16Station*8)/2;	
	ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 4, u16Station);	
	
	return u32ReturnFlag;
}

/*********************************************************************************
** 函数名称:ShowScGrpCtrl
** 功能描述：更新处于成组控制界面下的显示 
** 函数输入：none
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32  ShowScGrpCtrl(u32 u32Type)
{
	u8 u8I = 0x00;
	u16 u16I = 0x00;
	u16 u16J = 0x00;
	u16 u16K = 0x00;
	u16 u16Station = 0x00;
	u16 u16Direction = 0x00;
	char s[20] ;
	u32 u32ReturnFlag = 0x01;
	u32 u32ButtonValue;
	
	LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u32ButtonValue);//近端架号
	if(((u32ButtonValue&RFID_STATUS_START) != RFID_STATUS_START)
//		&&((u32ButtonValue&RFID_STATUS_NEIGHBOR_LEFT) != RFID_STATUS_NEIGHBOR_LEFT)
//		&&((u32ButtonValue&RFID_STATUS_NEIGHBOR_RIGHT) != RFID_STATUS_NEIGHBOR_RIGHT)
    )
	{	
		LcdAreaClear(OLED_DISPLAY_NORMAL,2,0,3,127);
		LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16J);
		u8I = 0x00;
		//zzw
		sprintf(s,"%s",ActionInfoMsg[u8I].ActionName);
		#if 0
		u8I++;	
		if(u8I < RuningActionCnt)
		{
			for(;u8I < RuningActionCnt;u8I++)
			{
				u16Station = strlen(s);
				if(u16Station*8 < 128)
				{
					strcat(s,",");
					strcat(s,(char *)&ActionInfoMsg[u8I].ActionName);
				}
			}
		}
		#endif
		u16Station = strlen(s);
		if(u16Station > 16)
		{	
			u16Station= 16;//防止刷新至下一行	
			s[16] = 0x00;
		}
		u16Station = (128-u16Station*8)/2;
		ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 2, u16Station);			
    }
	LcdAreaClear(OLED_DISPLAY_NORMAL,4,0,5,111);
	LogicRunInfApi(LOGIC_GET_GRP_ACTION_DIRECTION,&u16Direction);
	if((u16Direction == SC_GRP_ACTION_NEAR_REMOTE) || (u16Direction == SC_GRP_ACTION_REMOTE_NEAR))
	{
		LogicRunInfApi(LOGIC_GET_GRP_NEAR_SC_NUMB,&u16I);
		LogicRunInfApi(LOGIC_GET_GRP_REMOTE_SC_NUMB,&u16J);
		LogicRunInfApi(LOGIC_GET_SC_NUMB_ADD_DIRCT, &u16K);
		LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB, &u16Station);
		
		if(u16K == SC_DIRCT_RIGHT)
		{
			if(u16I <= u16Station)
			{
				if(u16Direction == SC_GRP_ACTION_REMOTE_NEAR)
				{
					if((u32ButtonValue&RFID_STATUS_START) != RFID_STATUS_START)
						sprintf(s,"%d -> %d", u16J,u16I);
					else
						sprintf(s,"%d->%d", u16J,u16I);
					if((u16I == 0x1FF)&&(u16J == 0x1FF))
						sprintf(s,"*** -> ***");
					
				}
				else
				{
					if((u32ButtonValue&RFID_STATUS_START) != RFID_STATUS_START)
						sprintf(s,"%d <- %d",  u16J,u16I);
					else
						sprintf(s,"%d<-%d",  u16J,u16I);
					if((u16I == 0x1FF)&&(u16J == 0x1FF))
						sprintf(s,"*** <- **");
					
				}
			}
			else
			{
				if(u16Direction == SC_GRP_ACTION_REMOTE_NEAR)
				{
					if((u32ButtonValue&RFID_STATUS_START) != RFID_STATUS_START)
						sprintf(s,"%d <- %d", u16I,u16J);
					else
						sprintf(s,"%d<-%d", u16I,u16J);
					if((u16I == 0x1FF)&&(u16J == 0x1FF))
						sprintf(s,"*** <- ***");
					
				}
				else
				{
					if((u32ButtonValue&RFID_STATUS_START) != RFID_STATUS_START)
						sprintf(s,"%d -> %d",  u16I,u16J);
					else
						sprintf(s,"%d->%d",  u16I,u16J);
					if((u16I == 0x1FF)&&(u16J == 0x1FF))
						sprintf(s,"*** -> ***");
					
				}
				
			}				
		}
		else
		{
			if(u16I >= u16Station)
			{
				if(u16Direction == SC_GRP_ACTION_REMOTE_NEAR)
				{
					if((u32ButtonValue&RFID_STATUS_START) != RFID_STATUS_START)
						sprintf(s,"%d -> %d", u16J,u16I);
					else
						sprintf(s,"%d->%d", u16J,u16I);
					if((u16I == 0x1FF)&&(u16J == 0x1FF))
						sprintf(s,"*** -> ***");
					
				}
				else
				{
					if((u32ButtonValue&RFID_STATUS_START) != RFID_STATUS_START)
						sprintf(s,"%d <- %d",  u16J,u16I);
					else
						sprintf(s,"%d<-%d",  u16J,u16I);
					if((u16I == 0x1FF)&&(u16J == 0x1FF))
						sprintf(s,"*** <- ***");
					
				}
				
			}
			else
			{

				if(u16Direction == SC_GRP_ACTION_REMOTE_NEAR)
				{
					if((u32ButtonValue&RFID_STATUS_START) != RFID_STATUS_START)
						sprintf(s,"%d <- %d", u16I,u16J);
					else
						sprintf(s,"%d<-%d", u16I,u16J);
					if((u16I == 0x1FF)&&(u16J == 0x1FF))
						sprintf(s,"*** <- ***");
				}
				else
				{
					if((u32ButtonValue&RFID_STATUS_START) != RFID_STATUS_START)
						sprintf(s,"%d -> %d", u16I,u16J);
					else
						sprintf(s,"%d->%d", u16I,u16J);
					if((u16I == 0x1FF)&&(u16J == 0x1FF))
						sprintf(s,"***->***");
					
				}				
			}				
		
		}	
		
		if((u32ButtonValue&RFID_STATUS_START) == RFID_STATUS_START)
			strcat(s," ,启动键");		
		u16Station = strlen(s);
		if(u16Station > 16)
		{	
			u16Station= 16;//防止刷新至下一行	
			s[16] = 0x00;
		}
		u16Station = (128-u16Station*8)/2;			
		ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 4, u16Station);	
	}

	return u32ReturnFlag;	
}

/*********************************************************************************
** 函数名称:ShowYkqSelfCtrl
** 功能描述：更新处于本架控制下的显示
** 函数输入：none
** 函数输出：None
** 注意事项：
**********************************************************************************/
#if (SET_YKQ_PRGPT_USE_FLAG != DISABLED)
	u32  ShowMainConnetYkqSelfCtrl(u32 u32Type)
	{
		u8 u8I;
		char s[20] ;
		u32 u32ReturnFlag = 0x01;
		u16 u16Station = 0x00,u16J,u16I;
		
		memset(s,0x00,sizeof(s));
		LcdAreaClear(OLED_DISPLAY_NORMAL,2,0,5,127);
		LogicParamApi(LOGIC_GET_COLIBRATE_YKQ_TYPE,&u16J);
		LogicParamApi(LOGIC_GET_COLIBRATE_YKQ_ID,&u16I);
		if (u16J == YKQ_ALL_FUNCTION)
			sprintf(s,"%d#,全功能遥控器",u16I);
		else
			sprintf(s,"%d#,微型遥控器",u16I);
		u16Station = strlen(s);
		u16Station = (128-u16Station*8)/2;	
		ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 2, u16Station);	

		memset(s,0x00,sizeof(s));
		LogicParamApi(LOGIC_GET_COLIBRATE_YKQ_VERSION,s);
		u16Station = strlen(s);
		if(u16Station > 16)
		{	
			u16Station= 16;//防止刷新至下一行
			s[16] = 0x00;
		}
		u16Station = (128-u16Station*8)/2;	
		ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 4, u16Station);			
			
		return u32ReturnFlag;
	}
#endif

/*********************************************************************************
** 函数名称:ShowScZibaoCtrl
** 功能描述：更新处于本架自保控制下的显示
** 函数输入：none
** 函数输出：None
** 注意事项：
**********************************************************************************/
#if (ZIBAO_CTRL_ENABLE_FLAG != DISABLED)
u32  ShowScZibaoCtrl(u32 u32Type)
{
	u8 u8I;
	char s[20] ;
	u32 u32ReturnFlag = 0x01;
	u16 u16Station = 0x00,u16J;
	
	LcdAreaClear(OLED_DISPLAY_NORMAL,2,0,5,127);
	LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16J);
	
	sprintf(s,"%d#,本架自保",u16J);

	u16Station = strlen(s);
	u16Station = (128-u16Station*8)/2;	
	ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 2, u16Station);	


	for(u16J = 0x00,u8I = 0x00;u8I < INTERFACE_KEY_TOGETHER_MAX;u8I++)
	{
		if(s_stInterfaceActionName[u8I] == INTERFACE_ACTION_NONE)
			break;
		else
			u16J++;
	}
	if(u16J < INTERFACE_DISPLAY_ACTION_MAX)
	{
		u8I = 0x00;
		sprintf(s,"%s",u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
		u8I++;	
		if(s_stInterfaceActionName[u8I] != INTERFACE_ACTION_NONE)
		{
			for(;u8I < INTERFACE_KEY_TOGETHER_MAX;u8I++)
			{
				if(s_stInterfaceActionName[u8I] != INTERFACE_ACTION_NONE)
					strcat(s,(char *)&u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
			}
			
			u16Station = strlen(s);
			if(u16Station*8 < 128)
			{
				u8I = 0x00;
				sprintf(s,"%s",u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
				u8I++;
				for(;u8I < u16J;u8I++)
				{
					strcat(s,",");
					strcat(s,(char *)&u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
				}			
			}
		}
	}	
	else
	{
		memset(s,0x00,sizeof(s));
		for(u8I = 0x00;u8I < INTERFACE_DISPLAY_ACTION_MAX;u8I++)
		{
			if(u8I== 0x00)
			{
				sprintf(s,"%s",u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
				s[(u8I+0x01)*4+u8I] =  ',';
				s[(u8I+0x01)*4+u8I+0x01] = 0x00;
			}
			else
			{
				strcat(s,(char *)&u8OperatInfoMsg[s_stInterfaceActionName[u8I]]);
				if(u8I != (INTERFACE_DISPLAY_ACTION_MAX-0x01))
				{
					s[(u8I+0x01)*4+u8I] =  ',';
					s[(u8I+0x01)*4+u8I+0x01] = 0x00;
				}
				else
				{
					s[(u8I+0x01)*4+u8I] = 0x00;					
				}
			}						
		}	
		if(u16J> INTERFACE_DISPLAY_ACTION_MAX)
			strcat(s,"..");
	}
	
	u16Station = strlen(s);
	if(u16Station > 16)
	{
		u16Station= 16;//防止刷新至下一行	
		s[16] = 0x00;
	}
	u16Station = (128-u16Station*8)/2;
	ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 4, u16Station);		

	return u32ReturnFlag;
}
#endif
/*********************************************************************************
** 函数名称:ShowMainInchargeDisplayCtrl
** 功能描述：更新处于关机充电时的显示
** 函数输入：none
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32  ShowMainInchargeDisplayCtrl(u32 u32Type)
{
	u8 u8I = 0x00;
	u16 u16Station = 0x00;
	char s[20] ;
	u32 u32ReturnFlag = 0x00;
	
	LcdClear();
	LogicRunInfApi(LOGIC_GET_BATTERY_COULOM_STATUS,&u8I);
	ShowSymbol(OLED_DISPLAY_NORMAL,c_u16InterfaceBatterSymbol[u8I],2,60);	

	if(s_u16NoteItemWrite == 0x00)
		s_u16NoteItemRead = INTERFACE_NOTE_ENUM_MAX-0x01;
	else
		s_u16NoteItemRead = s_u16NoteItemWrite-0x01;

	memcpy(s,s_u8NoteItem[s_u16NoteItemRead],sizeof(s_u8NoteItem[s_u16NoteItemRead]));
	u16Station = strlen(s);
	u16Station = (128-u16Station*8)/2;
	ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 5, u16Station);
	
	#if 0
		BatteryGetInchargeVoltBackup(&u32ReturnFlag);
		sprintf(s,"%d",u32ReturnFlag);
		ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 3, 0x00);
	#endif
	
	return u32ReturnFlag;
}

/*********************************************************************************
** 函数名称:ShowMainTransRecvPrgCtrl
** 功能描述：传输或接收程序时对主界面进行更新。
** 函数输入：u32Type,进行主界面更新的方式。
** 函数输出：None
** 注意事项：
**********************************************************************************/
#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
	u32  ShowMainTransRecvPrgCtrl(u32 u32Type)
	{
		char s[20] ;
		u32 u32ReturnFlag = 0x00;
		
		LcdAreaClear(OLED_DISPLAY_NORMAL,4,0,5,127);

		memset(s,0x00,sizeof(s));
		ShowStr(0, "约剩余", 4, 8);	
		FormatRemainTimer(s_u32DownloadRemainTimer,(u8 *)s);
		ShowStr(OLED_DISPLAY_NORMAL,(const u8 *)s,4,56);
		ShowStr(OLED_DISPLAY_NORMAL,"分",4,72);
		ShowStr(OLED_DISPLAY_NORMAL,((const u8 *)s)+2,4,88);
		ShowStr(OLED_DISPLAY_NORMAL,"秒",4,104);	
		
		return u32ReturnFlag;
	}
#endif

/*********************************************************************************
** 函数名称:ShowMain
** 功能描述：依据参数对主界面进行更新。
** 函数输入：*u32Type,进行主界面更新的方式。
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32  ShowMain(u32 *u32Type)
{
	u32 u32ReturnFlag = 0x00;
	
	if(*u32Type == INTERFACE_SHOW_MAIN_NONE)
		return 0x00;


	if((*u32Type & INTERFACE_SHOW_MAIN_ALL) == INTERFACE_SHOW_MAIN_ALL)
	{
		u32ReturnFlag |= ShowMainAll(*u32Type);
		(*u32Type) &= ~INTERFACE_SHOW_MAIN_ALL;
	}
	
	if((*u32Type & INTERFACE_SHOW_MAIN_BATTERY) == INTERFACE_SHOW_MAIN_BATTERY)
	{
		u32ReturnFlag |= ShowMainBattery(*u32Type);
		(*u32Type) &= ~INTERFACE_SHOW_MAIN_BATTERY;
	}
	
	if((*u32Type & INTERFACE_SHOW_MAIN_RFIDSIGNAL) == INTERFACE_SHOW_MAIN_RFIDSIGNAL)
	{	
		u32ReturnFlag |= ShowMainRfid(*u32Type);
		(*u32Type) &= ~INTERFACE_SHOW_MAIN_RFIDSIGNAL;
	}
	if((*u32Type & INTERFACE_SHOW_MAIN_SCCOLIBRATE) == INTERFACE_SHOW_MAIN_SCCOLIBRATE)
	{
		u32ReturnFlag |= ShowMainScColibrate(*u32Type);
		(*u32Type) &= ~INTERFACE_SHOW_MAIN_SCCOLIBRATE;
	}
	if((*u32Type & INTERFACE_SHOW_MAIN_NOTE) == INTERFACE_SHOW_MAIN_NOTE)
	{	
		u32ReturnFlag |= ShowMainNote(*u32Type);
		(*u32Type) &= ~INTERFACE_SHOW_MAIN_NOTE;
	}
	if((*u32Type & INTERFACE_SHOW_MAIN_PROTECED) == INTERFACE_SHOW_MAIN_PROTECED)
	{	
		u32ReturnFlag |= SetScrProtected();//休屏
		(*u32Type) &= ~INTERFACE_SHOW_MAIN_PROTECED;
	}
	if((*u32Type & INTERFACE_SHOW_MAIN_AWAKE) == INTERFACE_SHOW_MAIN_AWAKE)
	{
		u32ReturnFlag |= SetScrAwake();//唤醒屏幕
		(*u32Type) &= ~INTERFACE_SHOW_MAIN_AWAKE;
	}
	if((*u32Type & INTERFACE_SHOW_YKQ_ID) == INTERFACE_SHOW_YKQ_ID)
	{
		u32ReturnFlag |= ShowMainYkqId(*u32Type);
		(*u32Type) &= ~INTERFACE_SHOW_YKQ_ID;
		(*u32Type) &= ~INTERFACE_SHOW_YKQ_ID_NORMAL;
		(*u32Type) &= ~INTERFACE_SHOW_YKQ_ID_INTERVAL;
	}
	if((*u32Type & INTERFACE_SHOW_CTRL_MODE) == INTERFACE_SHOW_CTRL_MODE)		//zzw
	{
		u32ReturnFlag |= ShowMainCtrlMode(*u32Type);
		(*u32Type) &= ~INTERFACE_SHOW_CTRL_MODE;
	}
	if((*u32Type & INTERFACE_SHOW_SC_SET) == INTERFACE_SHOW_SC_SET)//菜单设置
	{
		u32ReturnFlag |= ShowScSet(*u32Type);
		(*u32Type) &= ~INTERFACE_SHOW_SC_SET;
	}
	if((*u32Type & INTERFACE_SHOW_SC_SELF_CONTROL) == INTERFACE_SHOW_SC_SELF_CONTROL)//菜单设置
	{
		u32ReturnFlag |= ShowScSelfCtrl(*u32Type);
		(*u32Type) &= ~INTERFACE_SHOW_SC_SELF_CONTROL;
	}
	
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
		if((*u32Type & INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL) == INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL)//本架非动作状态
		{
			u32ReturnFlag |= ShowScUncertifiedCtrl(*u32Type);
			(*u32Type) &= ~INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL;
		}	
	#endif
	
	if((*u32Type & INTERFACE_SHOW_SC_NEAR_CONTROL) == INTERFACE_SHOW_SC_NEAR_CONTROL)//菜单设置
	{
		u32ReturnFlag |= ShowScNearCtrl(*u32Type);
		(*u32Type) &= ~INTERFACE_SHOW_SC_NEAR_CONTROL;
	}
	if((*u32Type & INTERFACE_SHOW_SC_GRP_CONTROL) == INTERFACE_SHOW_SC_GRP_CONTROL)//菜单设置
	{
		u32ReturnFlag |= ShowScGrpCtrl(*u32Type);
		(*u32Type) &= ~INTERFACE_SHOW_SC_GRP_CONTROL;
	}
	if((*u32Type & INTERFACE_SHOW_MAIN_SC_UNCOLIBRATE) == INTERFACE_SHOW_MAIN_SC_UNCOLIBRATE)//菜单设置
	{
		u32ReturnFlag |= ShowMainScUnColibrate(*u32Type);
		(*u32Type) &= ~INTERFACE_SHOW_MAIN_SC_UNCOLIBRATE;
	}

	#if (SET_YKQ_PRGPT_USE_FLAG != DISABLED)
		if((*u32Type & INTERFACE_SHOW_YKQ_SELF_CONTROL) == INTERFACE_SHOW_YKQ_SELF_CONTROL)//菜单设置
		{
			u32ReturnFlag |= ShowMainConnetYkqSelfCtrl(*u32Type);
			(*u32Type) &= ~INTERFACE_SHOW_YKQ_SELF_CONTROL;
		}		
	#endif	
	#if 0		//自保		zzw	
	#if (ZIBAO_CTRL_ENABLE_FLAG != DISABLED)
		if((*u32Type & INTERFACE_SHOW_SC_ZIBAO_CONTROL) == INTERFACE_SHOW_SC_ZIBAO_CONTROL)//菜单设置
		{
			u32ReturnFlag |= ShowScZibaoCtrl(*u32Type);
			(*u32Type) &= ~INTERFACE_SHOW_SC_ZIBAO_CONTROL;
		}	
	#endif
	#endif
	if((*u32Type & INTERFACE_SHOW_INCHARGE_DISPLAY) == INTERFACE_SHOW_INCHARGE_DISPLAY)//菜单设置
	{
		u32ReturnFlag |= ShowMainInchargeDisplayCtrl(*u32Type);
		(*u32Type) &= ~INTERFACE_SHOW_INCHARGE_DISPLAY;
	}
	
	#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
		if((*u32Type & INTERFACE_SHOW_SELF_TRANS_PRG) == INTERFACE_SHOW_SELF_TRANS_PRG)
		{
			u32ReturnFlag |= ShowMainTransRecvPrgCtrl(*u32Type);
			(*u32Type) &= ~INTERFACE_SHOW_SELF_TRANS_PRG;	
		}	
	
		if((*u32Type & INTERFACE_SHOW_SELF_RECV_PRG) == INTERFACE_SHOW_SELF_RECV_PRG)
		{
			u32ReturnFlag |= ShowMainTransRecvPrgCtrl(*u32Type);		
			(*u32Type) &= ~INTERFACE_SHOW_SELF_RECV_PRG;	
		}
	#endif
	if((*u32Type & INTERFACE_SHOW_SELF_RECV_PRG) == INTERFACE_SHOW_SELF_RECV_PRG)
		{
			u32ReturnFlag |= ShowMainTransRecvPrgCtrl(*u32Type);		
			(*u32Type) &= ~INTERFACE_SHOW_SELF_RECV_PRG;	
		}		
		
	return u32ReturnFlag;		
}

/*********************************************************************************
** 函数名称:ShowKeyLedProc
** 功能描述：依据操作模式对主界面按键指示灯进行更新。
** 函数输入：sMod,进行主界面更新的方式。
** 函数输出：None
** 注意事项：
**********************************************************************************/
#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE == ENABLED)
u32 ShowScSetKeyLedProc(u32 sNewKey,u32 u32KeyStatus,u32 u32ShiftKeyStatus)
{
	u32 u32Y = 0x00;
	u32 u32ButtonValue = 0x00;
	
	switch(sNewKey)
	{
		case KEY_STOP_SHUTDOWN:
			if(u32KeyStatus == KEY_DOWN)
			{
				#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)				
					#if (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);						
					#elif (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);	
					#elif (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);
					#elif (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)	
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);	
					#elif (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);					
					#endif
				#endif
			}
			break;			
		
		default:
			if(u32KeyStatus == KEY_DOWN)
			{
				u32ButtonValue = 0x00;
				u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
				if(u32Y != 0x00)
				{	
					#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)	
						#if (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
							KeyLedOnOff(u32ButtonValue,~u32ButtonValue);
						#elif (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)
							KeyLedOnOff(0x00,u32ButtonValue);						
						#elif (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
							KeyLedOnOff(0x00,u32ButtonValue);
						#elif (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)
							KeyLedOnOff(u32ButtonValue,0x00);	
						#elif (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)
							KeyLedOnOff(u32ButtonValue,0x00)
						#elif (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)	
							u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
							KeyLedOnOff(u32Y,0x00);	
							KeyLedToggle(u32ButtonValue,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);	
						#elif (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
							u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
							KeyLedOnOff(u32Y,0x00);	
							KeyLedToggle(u32ButtonValue,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);	
						#elif (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)	
							KeyLedOnOff(0x00,u32ButtonValue);					
						#endif	
					#endif	
				}		
			}
			else if(u32KeyStatus == KEY_UP)
			{
				#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)				
					#if (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);						
					#elif (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);	
					#elif (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);
					#elif (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);	
					#elif (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_SET_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)	
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);					
					#endif
				#endif					
			}
			else
				;
		}	
		
		return 0x01;
}
#endif

/*********************************************************************************
** 函数名称:ShowSlfSetKeyLedProc
** 功能描述：依据操作模式对主界面按键指示灯进行更新。
** 函数输入：sMod,进行主界面更新的方式。
** 函数输出：None
** 注意事项：
**********************************************************************************/
#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE == ENABLED)
u32 ShowSelfSetKeyLedProc(u32 sNewKey,u32 u32KeyStatus,u32 u32ShiftKeyStatus)
{
	u32 u32Y = 0x00;
	u32 u32ButtonValue = 0x00;
	
	switch(sNewKey)
	{
		case KEY_STOP_SHUTDOWN:
			if(u32KeyStatus == KEY_DOWN)
			{
				#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)				
					#if (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);						
					#elif (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);	
					#elif (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);
					#elif (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)	
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);	
					#elif (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);					
					#endif
				#endif
			}
			break;			
		
		default:
			if(u32KeyStatus == KEY_DOWN)
			{
				u32ButtonValue = 0x00;
				u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
				if(u32Y != 0x00)
				{	
					#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)	
						#if (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
							KeyLedOnOff(u32ButtonValue,~u32ButtonValue);
						#elif (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)
							KeyLedOnOff(0x00,u32ButtonValue);						
						#elif (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
							KeyLedOnOff(0x00,u32ButtonValue);
						#elif (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)
							KeyLedOnOff(u32ButtonValue,0x00);	
						#elif (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)
							KeyLedOnOff(u32ButtonValue,0x00)
						#elif (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)	
							u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
							KeyLedOnOff(u32Y,0x00);	
							KeyLedToggle(u32ButtonValue,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);	
						#elif (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
							u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
							KeyLedOnOff(u32Y,0x00);	
							KeyLedToggle(u32ButtonValue,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);	
						#elif (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)	
							KeyLedOnOff(0x00,u32ButtonValue);					
						#endif	
					#endif	
				}		
			}
			else if(u32KeyStatus == KEY_UP)
			{
				#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)				
					#if (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);						
					#elif (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);	
					#elif (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);
					#elif (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);	
					#elif (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SELF_SET_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)	
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);					
					#endif
				#endif					
			}
			else
				;
		}	
		
		return 0x01;
}
#endif

/*********************************************************************************
** 函数名称:ShowKeyLedProc
** 功能描述：依据操作模式对主界面按键指示灯进行更新。
** 函数输入：sMod,进行主界面更新的方式。
** 函数输出：None
** 注意事项：
**********************************************************************************/
#if ((YKQ_HARDWARE_KEYLED_EXIST_DEFINE == ENABLED)&&(INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED))
u32 ShowScUncertifiedKeyLedProc(u32 sNewKey,u32 u32KeyStatus,u32 u32ShiftKeyStatus)
{
	u32 u32Y = 0x00;
	u32 u32ButtonValue = 0x00;
	
	LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);
	if(u32Y == IN_SC_SS_NUMB)//服务器时，无任何可以动作的按键
	{
		KeyLedOnOff(0x00,KEY_LED_ALL);
		return 0x01; 
	}
	
	switch(sNewKey)
	{
		case KEY_STOP_SHUTDOWN:
		case KEY_INVALID:
			if(u32KeyStatus == KEY_DOWN)
			{
				#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)				
					#if (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);						
					#elif (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);	
					#elif (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);
					#elif (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)	
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);	
					#elif (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);					
					#endif
				#endif
			}
			else if(u32KeyStatus == KEY_UP)
			{
				KeyLedOnOff(0x00,KEY_LED_ALL);
				u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
				KeyLedOnOff(u32Y,0x00);				
			}
				
			break;			
		
		default:
			if((u32KeyStatus == KEY_DOWN)&&(u32ShiftKeyStatus == DISABLED))
			{
				#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)				
					#if (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);						
					#elif (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);	
					#elif (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);
					#elif (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);	
						u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
						if(u32Y != 0x00)
							KeyLedToggle(u32ButtonValue,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);
					#elif (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)	
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);					
					#endif
				#endif			
			}
			else if((u32KeyStatus == KEY_UP)&&(u32ShiftKeyStatus == DISABLED))
			{
				#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)				
					#if (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);						
					#elif (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);	
					#elif (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);
					#elif (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);	
					#elif (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_UNCERTIFIED_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)	
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);					
					#endif
				#endif					
			}
			else if((u32KeyStatus == KEY_DOWN)&&(u32ShiftKeyStatus == ENABLED))
			{
				#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)				
					#if (KEYLED_SC_UNCERTIFIED_SECOND_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_UNCERTIFIED_SECOND_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);						
					#elif (KEYLED_SC_UNCERTIFIED_SECOND_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_UNCERTIFIED_SECOND_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);	
					#elif (KEYLED_SC_UNCERTIFIED_SECOND_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);
					#elif (KEYLED_SC_UNCERTIFIED_SECOND_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)	
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,~u32Y);	 //qinxi
						u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
						if(u32Y != 0x00)
							KeyLedToggle(u32ButtonValue,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);	
					#elif (KEYLED_SC_UNCERTIFIED_SECOND_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_UNCERTIFIED_SECOND_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)	
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);					
					#endif
				#endif			
			}
			else if((u32KeyStatus == KEY_UP))
			{
				#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)				
					#if (KEYLED_SC_UNCERTIFIED_SECOND_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_UNCERTIFIED_SECOND_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);						
					#elif (KEYLED_SC_UNCERTIFIED_SECOND_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_UNCERTIFIED_SECOND_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);	
					#elif (KEYLED_SC_UNCERTIFIED_SECOND_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);
					#elif (KEYLED_SC_UNCERTIFIED_SECOND_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);	
					#elif (KEYLED_SC_UNCERTIFIED_SECOND_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_UNCERTIFIED_SECOND_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)	
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);					
					#endif
				#endif					
			}
			
			else
				;
		}
		
		return 0x01;
}
#endif

/*********************************************************************************
** 函数名称:ShowKeyLedProc
** 功能描述：依据操作模式对主界面按键指示灯进行更新。
** 函数输入：sMod,进行主界面更新的方式。
** 函数输出：None
** 注意事项：
**********************************************************************************/
#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE == ENABLED)
u32 ShowScSelfKeyLedProc(u32 sNewKey,u32 u32KeyStatus,u32 u32ShiftKeyStatus)
{
	u32 u32Y = 0x00;
	u32 u32ButtonValue = 0x00;

	/***判断出是否有功能按键与其他的按键同时按下***/	
	/**2018.02.26，新增加***/	
	if((sNewKey&KEY_SHIFT) == KEY_SHIFT)
	{
		if((sNewKey^KEY_SHIFT) != 0)
		{
			sNewKey = KEY_INVALID;
		}
	}
	if((sNewKey&KEY_INCREASE) == KEY_INCREASE)
	{
		if((sNewKey^KEY_INCREASE) != 0)
		{
			sNewKey = KEY_INVALID;
		}
	}	
	if((sNewKey&KEY_DECREASE) == KEY_DECREASE)
	{
		if((sNewKey^KEY_DECREASE) != 0)
		{
			sNewKey = KEY_INVALID;
		}
	}	
	if((sNewKey&KEY_LEFT_GRP) == KEY_LEFT_GRP)
	{
		if((sNewKey^KEY_LEFT_GRP) != 0)
		{
			sNewKey = KEY_INVALID;
		}
	}	
	if((sNewKey&KEY_RIGHT_GRP) == KEY_RIGHT_GRP)
	{
		if((sNewKey^KEY_RIGHT_GRP) != 0)
		{
			sNewKey = KEY_INVALID;
		}
	}	
	if((sNewKey&KEY_CERTIFY) == KEY_CERTIFY)
	{
		if((sNewKey^KEY_CERTIFY) != 0)
		{
			sNewKey = KEY_INVALID;
		}
	}
	if((sNewKey&KEY_MENU_START) == KEY_MENU_START)
	{
		if((sNewKey^KEY_MENU_START) != 0)
		{
			sNewKey = KEY_INVALID;
		}
	}	
	if((sNewKey&KEY_STOP_SHUTDOWN) == KEY_STOP_SHUTDOWN)//停止键优先级最高
	{
		if((sNewKey^KEY_STOP_SHUTDOWN) != 0)
		{
			sNewKey = KEY_INVALID;
		}
	}	
	
	switch(sNewKey)
	{
		case KEY_INVALID:
			#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)				
				#if (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
					KeyLedOnOff(0x00,KEY_LED_ALL);
				#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)
					KeyLedOnOff(0x00,KEY_LED_ALL);
					u32Y = InterfaceGetKeyLedDefaultValue(INTERFACE_MODE_SC_SELF_CONTROL,s_u32ShiftKeyEnableFlag);
					KeyLedOnOff(u32Y,0x00);						
				#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
					KeyLedOnOff(0x00,KEY_LED_ALL);
				#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)
					KeyLedOnOff(0x00,KEY_LED_ALL);
					u32Y = InterfaceGetKeyLedDefaultValue(INTERFACE_MODE_SC_SELF_CONTROL,s_u32ShiftKeyEnableFlag);
					KeyLedOnOff(u32Y,0x00);	
				#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)
					KeyLedOnOff(0x00,KEY_LED_ALL);
					u32Y = InterfaceGetKeyLedDefaultValue(INTERFACE_MODE_SC_SELF_CONTROL,s_u32ShiftKeyEnableFlag);
					KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);
				#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)	
					KeyLedOnOff(0x00,KEY_LED_ALL);
					u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
					KeyLedOnOff(u32Y,0x00);	
				#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
					KeyLedOnOff(0x00,KEY_LED_ALL);
				#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)	
					KeyLedOnOff(0x00,KEY_LED_ALL);
					u32Y = InterfaceGetKeyLedDefaultValue(INTERFACE_MODE_SC_SELF_CONTROL,s_u32ShiftKeyEnableFlag);
					KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);					
				#endif
			#endif
			break;
			
// 			#if (SPECIAL_FX_ENABLE_FLAG != DIASBLED)
// 				#if (SPECIAL_F1_ENABLE_FLAG != DIASBLED)
// 					case KEY_SHORTCUT_F1:
// 				#endif
// 				#if (SPECIAL_F2_ENABLE_FLAG != DIASBLED)		
// 					case KEY_SHORTCUT_F2:	
// 				#endif
// 				#if (SPECIAL_F3_ENABLE_FLAG != DIASBLED)
// 					case KEY_SHORTCUT_F3:
// 				#endif
// 				#if (SPECIAL_F4_ENABLE_FLAG != DIASBLED)		
// 					case KEY_SHORTCUT_F4:	
// 				#endif					
// 					if(u32KeyStatus == KEY_DOWN)
// 					{
// 						#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)	
// 							#if (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
// 								KeyLedOnOff(0x00,KEY_LED_ALL);
// 							#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)
// 								KeyLedOnOff(0x00,KEY_LED_ALL);				
// 							#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
// 								KeyLedOnOff(0x00,KEY_LED_ALL);
// 							#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)
// 								KeyLedOnOff(0x00,KEY_LED_ALL);
// 							#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)
// 								KeyLedOnOff(0x00,KEY_LED_ALL);
// 							#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)	
// 								KeyLedOnOff(0x00,KEY_LED_ALL);	
// 							#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
// 								KeyLedOnOff(0x00,KEY_LED_ALL);
// 							#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)	
// 								KeyLedOnOff(0x00,KEY_LED_ALL);					
// 							#endif	
// 						#endif					
// 					}
// 					break;
// 		#endif			
					
		default:
			 if(((u32KeyStatus == KEY_DOWN)||(u32KeyStatus == KEY_LONGDOWN))&&(u32ShiftKeyStatus == DISABLED))
			{
				u32ButtonValue = 0x00;
				u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
				if(u32Y != 0x00)
				{	
					#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)	
						#if (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
							KeyLedOnOff(u32ButtonValue,~u32ButtonValue);
						#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)
							KeyLedOnOff(0x00,u32ButtonValue);						
						#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
							KeyLedOnOff(0x00,u32ButtonValue);
						#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)
							KeyLedOnOff(u32ButtonValue,0x00);	
						#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)
							KeyLedOnOff(u32ButtonValue,0x00)
						#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)	
							u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
							KeyLedOnOff(u32Y,~u32Y);
//							if((sNewKey != KEY_SHORTCUT_F1)&&(sNewKey != KEY_SHORTCUT_F2)\
//								&&(sNewKey != KEY_SHORTCUT_F3)&&(sNewKey != KEY_SHORTCUT_F4)\
//								&&(sNewKey != KEY_MENU_START))
//								KeyLedToggle(u32ButtonValue&u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);	
						#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
							u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
							KeyLedOnOff(u32Y,0x00);	
							if((sNewKey != KEY_SHORTCUT_F1)&&(sNewKey != KEY_SHORTCUT_F2)\
								&&(sNewKey != KEY_SHORTCUT_F3)&&(sNewKey != KEY_SHORTCUT_F4)\
								&&(sNewKey != KEY_MENU_START))
								KeyLedToggle(u32ButtonValue&u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);	
						#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)	
							KeyLedOnOff(0x00,u32ButtonValue);					
						#endif	
					#endif	
				}				
			}
			else if(((u32KeyStatus == KEY_DOWN)||(u32KeyStatus == KEY_LONGDOWN))&&(u32ShiftKeyStatus == ENABLED))
			{
				u32ButtonValue = 0x00;
				u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
				if(u32Y != 0x00)
				{	
					#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)	
						#if (KEYLED_SC_SELF_SECOND_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
							KeyLedOnOff(u32ButtonValue,~u32ButtonValue);
						#elif (KEYLED_SC_SELF_SECOND_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)
							KeyLedOnOff(0x00,u32ButtonValue);						
						#elif (KEYLED_SC_SELF_SECOND_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
							KeyLedOnOff(0x00,u32ButtonValue);
						#elif (KEYLED_SC_SELF_SECOND_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)
							KeyLedOnOff(u32ButtonValue,0x00);	
						#elif (KEYLED_SC_SELF_SECOND_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)
							KeyLedOnOff(u32ButtonValue,0x00)
						#elif (KEYLED_SC_SELF_SECOND_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)	
							u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
							KeyLedOnOff(u32Y,~u32Y);	
							if(sNewKey != KEY_MENU_START)
								KeyLedToggle(u32ButtonValue&u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);	
						#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
							u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
							KeyLedOnOff(u32Y,0x00);	
							if((sNewKey != KEY_SHORTCUT_F1)&&(sNewKey != KEY_SHORTCUT_F2)\
								&&(sNewKey != KEY_SHORTCUT_F3)&&(sNewKey != KEY_SHORTCUT_F4)\
								&&(sNewKey != KEY_MENU_START))
								KeyLedToggle(u32ButtonValue&u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);	
						#elif (KEYLED_SC_SELF_SECOND_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)	
							KeyLedOnOff(0x00,u32ButtonValue);					
						#endif	
					#endif	
				}				
			}			 
			else if((u32KeyStatus == KEY_UP)&&(u32ShiftKeyStatus == DISABLED))
			{			
				#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)				
					#if (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32ButtonValue = 0x00;
						u32Y = KeyCorrespendLed(sNewKey.u32Key,&u32ButtonValue);
						KeyLedOnOff(u32ButtonValue,0x00);
					#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)
						KeyLedOnOff(0x00,KEY_LED_ALL);						
					#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32ButtonValue = 0x00;
						u32Y = KeyCorrespendLed(sNewKey.u32Key,&u32ButtonValue);
						KeyLedOnOff(u32ButtonValue,0x00);
					#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32ButtonValue = 0x00;
						u32Y = KeyCorrespendLed(sNewKey.u32Key,&u32ButtonValue);
						KeyLedOnOff(u32ButtonValue,0x00);
					#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)	
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32ButtonValue = 0x00;
						u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
						KeyLedToggle(u32ButtonValue,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);	
					#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
						KeyLedOnOff(0x00,KEY_LED_ALL);
						u32ButtonValue = 0x00;
						u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
						KeyLedToggle(u32ButtonValue,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);	
					#elif (KEYLED_SC_SELF_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)	
						KeyLedOnOff(0x00,KEY_LED_ALL);					
					#endif
				#endif	
		}
		else if ((u32KeyStatus == KEY_UP)&&(u32ShiftKeyStatus == ENABLED))
		{			
			#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)				
				#if (KEYLED_SC_SELF_SECOND_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
					KeyLedOnOff(0x00,KEY_LED_ALL);
					u32ButtonValue = 0x00;
					u32Y = KeyCorrespendLed(sNewKey.u32Key,&u32ButtonValue);
					KeyLedOnOff(u32ButtonValue,0x00);
				#elif (KEYLED_SC_SELF_SECOND_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)
					KeyLedOnOff(0x00,KEY_LED_ALL);						
				#elif (KEYLED_SC_SELF_SECOND_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
					KeyLedOnOff(0x00,KEY_LED_ALL);
				#elif (KEYLED_SC_SELF_SECOND_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)
					KeyLedOnOff(0x00,KEY_LED_ALL);
					u32ButtonValue = 0x00;
					u32Y = KeyCorrespendLed(sNewKey.u32Key,&u32ButtonValue);
					KeyLedOnOff(u32ButtonValue,0x00);
				#elif (KEYLED_SC_SELF_SECOND_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)
					KeyLedOnOff(0x00,KEY_LED_ALL);
					u32ButtonValue = 0x00;
					u32Y = KeyCorrespendLed(sNewKey.u32Key,&u32ButtonValue);
					KeyLedOnOff(u32ButtonValue,0x00);
				#elif (KEYLED_SC_SELF_SECOND_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)	
					KeyLedOnOff(0x00,KEY_LED_ALL);
					u32ButtonValue = 0x00;
					u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
					KeyLedToggle(u32ButtonValue,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);	
				#elif (KEYLED_SC_SELF_SECOND_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
					KeyLedOnOff(0x00,KEY_LED_ALL);
					u32ButtonValue = 0x00;
					u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
					KeyLedToggle(u32ButtonValue,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);	
				#elif (KEYLED_SC_SELF_SECOND_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)	
					KeyLedOnOff(0x00,KEY_LED_ALL);					
				#endif
			#endif	
		} 
	}
	
	return 0x01;
}
#endif

/*********************************************************************************
** 函数名称:ShowKeyLedProc
** 功能描述：依据操作模式对主界面按键指示灯进行更新。
** 函数输入：sMod,进行主界面更新的方式。
** 函数输出：None
** 注意事项：
**********************************************************************************/
#if ((YKQ_HARDWARE_KEYLED_EXIST_DEFINE == ENABLED)&&(QUICK_KEY_CTRL_ENABLE_FLAG	!= DISABLED))
u32 ShowScSelfQuickKeyLedProc(u32 sNewKey,u32 u32KeyStatus,u32 u32ShiftKeyStatus)
{

	return 0x01;
}
#endif

/*********************************************************************************
** 函数名称:ShowKeyLedProc
** 功能描述：依据操作模式对主界面按键指示灯进行更新。
** 函数输入：sMod,进行主界面更新的方式。
** 函数输出：None
** 注意事项：
**********************************************************************************/
#if ((YKQ_HARDWARE_KEYLED_EXIST_DEFINE == ENABLED)&&(GRP_CTRL_ENABLE_FLAG != DISABLED))
u32 ShowScZibaoKeyLedProc(u32 sNewKey,u32 u32KeyStatus,u32 u32ShiftKeyStatus)
{
	u32 u32Y = 0x00;
	u32 u32ButtonValue = 0x00;
	
	switch(sNewKey)
	{
		case KEY_INVALID:
		{
			#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)				
				#if (KEYLED_SC_ZIBAO_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
					KeyLedOnOff(0x00,KEY_LED_ALL);
				#elif (KEYLED_SC_ZIBAO_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)
					u32Y = InterfaceGetKeyLedDefaultValue(INTERFACE_MODE_SC_ZIBAO_CONTROL,s_u32ShiftKeyEnableFlag);
					KeyLedOnOff(u32Y,0x00);						
				#elif (KEYLED_SC_ZIBAO_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
					KeyLedOnOff(0x00,KEY_LED_ALL);
				#elif (KEYLED_SC_ZIBAO_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)
					KeyLedOnOff(0x00,KEY_LED_ALL);
					u32Y = InterfaceGetKeyLedDefaultValue(INTERFACE_MODE_SC_ZIBAO_CONTROL,s_u32ShiftKeyEnableFlag);
					KeyLedOnOff(u32Y,0x00);	
				#elif (KEYLED_SC_ZIBAO_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)
					KeyLedOnOff(0x00,KEY_LED_ALL);
					u32Y = InterfaceGetKeyLedDefaultValue(INTERFACE_MODE_SC_ZIBAO_CONTROL,s_u32ShiftKeyEnableFlag);
					KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);
				#elif (KEYLED_SC_ZIBAO_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)
					KeyLedOnOff(0x00,KEY_LED_ALL);	
//					u32Y = InterfaceGetKeyLedDefaultValue(INTERFACE_MODE_SC_ZIBAO_CONTROL,s_u32ShiftKeyEnableFlag);
					KeyLedOnOff(u32Y,0x00);	
				#elif (KEYLED_SC_ZIBAO_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
					KeyLedOnOff(0x00,KEY_LED_ALL);
				#elif (KEYLED_SC_ZIBAO_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)	
					KeyLedOnOff(0x00,KEY_LED_ALL);
					u32Y = InterfaceGetKeyLedDefaultValue(INTERFACE_MODE_SC_ZIBAO_CONTROL,s_u32ShiftKeyEnableFlag);
					KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);					
				#endif
			#endif
			break;
		}
		
		default:
			if(u32KeyStatus == KEY_DOWN)
			{
				#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
					u32ButtonValue = 0x00;
					u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
					if(u32Y != 0x00)
					{	
						#if (KEYLED_SC_ZIBAO_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
							KeyLedOnOff(u32ButtonValue,~u32ButtonValue);
						#elif (KEYLED_SC_ZIBAO_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)
							KeyLedOnOff(0x00,u32ButtonValue);						
						#elif (KEYLED_SC_ZIBAO_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
							KeyLedOnOff(0x00,u32ButtonValue);
						#elif (KEYLED_SC_ZIBAO_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)
							KeyLedOnOff(u32ButtonValue,0x00);	
						#elif (KEYLED_SC_ZIBAO_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)
							KeyLedOnOff(u32ButtonValue,0x00)
						#elif (KEYLED_SC_ZIBAO_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)					
//							u32Y = InterfaceGetKeyLedDefaultValue(INTERFACE_MODE_SC_ZIBAO_CONTROL,s_u32ShiftKeyEnableFlag);
							KeyLedOnOff(u32Y,0x00);	
							KeyLedToggle(u32ButtonValue,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);	
						#elif (KEYLED_SC_ZIBAO_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
							KeyLedToggle(u32ButtonValue,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);	
						#elif (KEYLED_SC_ZIBAO_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)	
							KeyLedOnOff(0x00,u32ButtonValue);					
						#endif
					}
				#endif					
			}
			break;

	}
	
	return 0x01;

}
#endif

/*********************************************************************************
** 函数名称:ShowKeyLedProc
** 功能描述：依据操作模式对主界面按键指示灯进行更新。
** 函数输入：sMod,进行主界面更新的方式。
** 函数输出：None
** 注意事项：
**********************************************************************************/
#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE == ENABLED)
u32 ShowScGrpKeyLedProc(u32 sNewKey,u32 u32KeyStatus,u32 u32ShiftKeyStatus)
{
	u32 u32Y = 0x00;
	u32 u32ButtonValue = 0x00;
	
	switch(sNewKey)
	{
		case KEY_INVALID:
		{
			#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)	
				#if (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
					KeyLedOnOff(0x00,KEY_LED_ALL);
				#elif (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)		
					KeyLedOnOff(0x00,KEY_LED_ALL);
					u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
					KeyLedOnOff(u32Y,0x00);						
				#elif (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
					KeyLedOnOff(0x00,KEY_LED_ALL);
				#elif (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)	
					KeyLedOnOff(0x00,KEY_LED_ALL);
					u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
					KeyLedOnOff(u32Y,0x00);	
				#elif (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)	
					KeyLedOnOff(0x00,KEY_LED_ALL);
					u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
					KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);
				#elif (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)	
					KeyLedOnOff(0x00,KEY_LED_ALL);
					u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
					KeyLedOnOff(u32Y,0x00);	;
				#elif (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
					KeyLedOnOff(0x00,KEY_LED_ALL);
				#elif (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)	
					KeyLedOnOff(0x00,KEY_LED_ALL);
					u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
					KeyLedOnOff(u32Y,0x00);						
				#endif	
			#endif
			break;
		}

		default:
			 if((u32KeyStatus == KEY_DOWN)&&(u32ShiftKeyStatus == DISABLED))
			{
				#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)	
					#if (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
						u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
						if(u32Y != 0x00)
						{
							KeyLedOnOff(u32ButtonValue,0x00);
						}
					#elif (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)
						u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
						if(u32Y != 0x00)
						{
							KeyLedOnOff(0x00,u32ButtonValue);						
						}
					#elif (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
						u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
						if(u32Y != 0x00)
						{
							KeyLedOnOff(0x00,u32ButtonValue);
						}
					#elif (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)
						u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
						if(u32Y != 0x00)
						{
							KeyLedOnOff(u32ButtonValue,0x00);	
						}
					#elif (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)
						u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
						if(u32Y != 0x00)
						{
							KeyLedOnOff(u32ButtonValue,0x00)
						}
					#elif (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)	
						u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
						if((u32Y != 0x00))//&&(status == SC_BUTTON_ACK_ACCEPT) && (u32ButtonAckValue == u32ButtonValue))
						{
 							KeyLedOnOff(0x00,KEY_LED_ALL);	//shift时，同时按住两个有效键，抬起其中之一，抬起的键仍一直闪烁 2020.03.04 parry
							KeyLedToggle(u32ButtonValue,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);	
						}
					#elif (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
						u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
						if((u32Y != 0x00)&&(status == SC_BUTTON_ACK_ACCEPT) && (u32ButtonAckValue == u32ButtonValue))
						{
							KeyLedToggle(u32ButtonValue,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);	
						}
					#elif (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)
						u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
						if(u32Y != 0x00)
						{
							KeyLedOnOff(0x00,u32ButtonValue);
						}
					#endif	
				#endif							
			}
			else if((u32KeyStatus == KEY_DOWN)&&(u32ShiftKeyStatus == ENABLED))
			{
				#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)	
					#if (KEYLED_SC_GRP_SECONDCTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
						u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
						if(u32Y != 0x00)
						{
							KeyLedOnOff(u32ButtonValue,0x00);
						}
					#elif (KEYLED_SC_GRP_SECONDCTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)
						u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
						if(u32Y != 0x00)
						{
							KeyLedOnOff(0x00,u32ButtonValue);						
						}
					#elif (KEYLED_SC_GRP_SECONDCTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
						u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
						if(u32Y != 0x00)
						{
							KeyLedOnOff(0x00,u32ButtonValue);
						}
					#elif (KEYLED_SC_GRP_SECONDCTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)
						u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
						if(u32Y != 0x00)
						{
							KeyLedOnOff(u32ButtonValue,0x00);	
						}
					#elif (KEYLED_SC_GRP_SECONDCTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)
						u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
						if(u32Y != 0x00)
						{
							KeyLedOnOff(u32ButtonValue,0x00)
						}
					#elif (KEYLED_SC_GRP_SECONDCTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)	
						u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
						if((u32Y != 0x00))//&&(status == SC_BUTTON_ACK_ACCEPT) && (u32ButtonAckValue == u32ButtonValue))
						{
 							KeyLedOnOff(0x00,KEY_LED_ALL);	//同时按住有效键和无效键，先抬起有效键再抬起无效键时，灯异常 2020.03.04 parry
							KeyLedToggle(u32ButtonValue,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);	
						}
					#elif (KEYLED_SC_GRP_SECONDCTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
						u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
						if((u32Y != 0x00))//&&(status == SC_BUTTON_ACK_ACCEPT) && (u32ButtonAckValue == u32ButtonValue))
						{
							KeyLedToggle(u32ButtonValue,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);	
						}
					#elif (KEYLED_SC_GRP_SECONDCTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)
						u32Y = KeyCorrespendLed(sNewKey,&u32ButtonValue);
						if(u32Y != 0x00)
						{
							KeyLedOnOff(0x00,u32ButtonValue);
						}
					#endif	
				#endif							
			}
			else if((u32KeyStatus == KEY_UP)&&(u32ShiftKeyStatus == DISABLED))
			{
				#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)	
					#if (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)		
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);						
					#elif (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)	
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);	
					#elif (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)	
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);
					#elif (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)	
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(0x00,KEY_LED_ALL);
						KeyLedOnOff(u32Y,0x00);
					#elif (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_GRP_CTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)							
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);						
					#endif	
				#endif				
			}
			else if((u32KeyStatus == KEY_UP)&&(u32ShiftKeyStatus == ENABLED))
			{
				#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)	
					#if (KEYLED_SC_GRP_SECONDCTRL_METHOD == KEY_DOWN_LEDON_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_GRP_SECONDCTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDON)		
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);						
					#elif (KEYLED_SC_GRP_SECONDCTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDOFF)
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_GRP_SECONDCTRL_METHOD == KEY_DOWN_LEDON_UP_LEDON)	
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);	
					#elif (KEYLED_SC_GRP_SECONDCTRL_METHOD == KEY_DOWN_LEDON_UP_LEDTOGGLE)	
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedToggle(u32Y,KEY_DOWN_LEDON_TIME_SET,KEY_DOWN_LEDOFF_TIME_SET);
					#elif (KEYLED_SC_GRP_SECONDCTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDON)	
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);	;
					#elif (KEYLED_SC_GRP_SECONDCTRL_METHOD == KEY_DOWN_LEDTOGGLE_UP_LEDOFF)	
						KeyLedOnOff(0x00,KEY_LED_ALL);
					#elif (KEYLED_SC_GRP_SECONDCTRL_METHOD == KEY_DOWN_LEDOFF_UP_LEDTOGGLE)							
						u32Y = InterfaceGetKeyLedDefaultValue(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag);
						KeyLedOnOff(u32Y,0x00);						
					#endif	
				#endif				
			}	
			else
				;
	}

	return 0x01;
}
#endif

/*********************************************************************************
** 函数名称:ShowKeyLedProc
** 功能描述：依据操作模式对主界面按键指示灯进行更新。
** 函数输入：sMod,进行主界面更新的方式。
** 函数输出：None
** 注意事项：
**********************************************************************************/
#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
	u32 ShowOnlineUpdateKeyLedProc(u32 sNewKey,u32 u32KeyStatus,u32 u32ShiftKeyStatus)
	{
		u32 u32KeyLedValue = 0x00;
		u32 u32I = 0x00;
		
		u32KeyLedValue = KEY_MENU_OK|KEY_MENU_STOP;  
	
		switch(sNewKey)
		{
			case KEY_STOP_SHUTDOWN:
				KeyLedOnOff(0x00,KEY_LED_ALL);
				break;
			
			default:
				if((u32KeyStatus == KEY_DOWN)||(u32KeyStatus == KEY_LONGDOWN))
				{	
					KeyLedOnOff(0x00,KEY_LED_ALL);
					KeyLedOnOff(u32KeyLedValue,0x00);
				}
				else	
				{
					KeyLedOnOff(0x00,u32KeyLedValue);
				}
		}
		return 0x01;
	}
#endif
/*******************************************************************************************
**函数名称：InterfaceModeInintProc
**函数作用：显示层处于初始化状态下，按键的处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
static u32 InterfaceModeInintProc(stKEY	sNewKey)
{
	u32 u32ReturnFlag = 0x00;
	
	return u32ReturnFlag;
}

static u8 calcNumProc(u32 num)
{
	u8 res = 1;
	switch(num)
	{
		case 0x00000040:
			res = 1;
			break;
		case 0x00000080:
			res = 2;
			break;
		case 0x00000400:
			res = 3;
			break;
		case 0x00000800:
			res = 4;
			break;
		case 0x00004000:
			res = 5;
			break;
		case 0x00008000:
			res = 6;
			break;
		case 0x00000010:
			res = 7;
			break;
		case 0x00000020:
			res = 8;
			break;
		case 0x00000100:
			res = 9;
			break;
		case 0x00000200:
			res = 0;
			break;
		default:
			res = 0xff;
			break;
		
	}
	return res;
}

/*******************************************************************************************
**函数名称：InterfaceModeIdleProc
**函数作用：显示层处于空闲状态下,按键的处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
static u32 InterfaceModeIdleProc(stKEY	sNewKey)
{
	u32 u32ReturnFlag = 0x00;
	u8 u8I = 0x00;
	INT8U err,I8errKeyFlag;
	u32 u32Y = 0x00;
	u16 u16DelayTime = 0x00;
	u16 u16SendTimes = 0x00;
	u8 u8J = 0;		//zzw
	char s[30];
	u32 fastColiearateFlag = 0;
	static u16 u16StopKeyPressFlag = 0x00;
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
		 OS_CPU_SR  cpu_sr = 0;
	#endif
	u8J = NO_CTRL;
	LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);
	LogicParamApi(LOGIC_GET_FASTCOLIBARATE_ENABLE,&fastColiearateFlag);	 //jhy
	InterfaceSetUpdateMainMethod(INTERFACE_SHOW_CTRL_MODE);			//zzw
	if((sNewKey.u32Status == KEY_UP) )//按键抬起，不予以识别
	{
		if(sNewKey.u32Key == KEY_SHIFT)
		{
			StateLed(0x00, KEY_SHIFT_LED);
			u32ReturnFlag = 0x01;
			if(g_u32ID == B_type)
			{
				IrUsartChipPwrOff();
			}
			else
			{
				GPIO_SetBits(GPIOC,GPIO_Pin_6);
			}
		}
	}
	
	if((sNewKey.u32Key != KEY_STOP_SHUTDOWN) &&((sNewKey.u32Status == KEY_DOWN) ||((sNewKey.u32Status == KEY_LONGDOWN))))//停止键按下，唤醒屏幕
	{
		StateLed(0x00, KEY_SHIFT_LED);//qinxi
		InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_AWAKE);
		OSSemPost(g_stSemInterface);
		u32ReturnFlag = 0x01;
	}

	if((sNewKey.u32Key == KEY_SHIFT)&& ((sNewKey.u32Status == KEY_DOWN) || (sNewKey.u32Status == KEY_LONGDOWN)))
	{
		KeyLedOnOff(0x00, KEY_LED_ALL);//qinxi
		if(sNewKey.u32Status == KEY_DOWN)
		{
			if(g_u32ID == B_type)
			{
				IrUsartChipPwrOn();//开启供电
			}
			else
			{
				WirelessChipPwrOn();
				GPIO_ResetBits(GPIOC, GPIO_Pin_6);//接收使能
			}
			if(g_u32ID != B_type)
				SendSetupToWl(RFID_ININT_SYNC1);
		}
		StateLed(KEY_SHIFT_LED,0x00);
		u32ReturnFlag = 0x01;
	}

	/**处理空闲模式下，可以识别的按键**/
	if((sNewKey.u32Key == KEY_MENU_START) && (sNewKey.u32Status == KEY_DOWN))//KEY_LONGDOWN
	{
		fastColiearateScNum = 0;
		u8FastColiearateScNumFlag = 0;
		InterfaceSetCtrlMode(INTERFACE_MODE_SELF_SET);
		LogicParamApi(LOGIC_GET_YKQ_NUMBER, &u8I);
		LogicParamApi(LOGIC_SET_YKQ_NUMBER_BACKUP, &u8I);
		
		ParEdit(sNewKey,&s_u32SelfSetIntfState);
		ShowScSetKeyLedProc(KEY_INVALID,KEY_UP,s_u32ShiftKeyEnableFlag);
		u32ReturnFlag = 0x01;
	}


	else if((sNewKey.u32Key == KEY_SHIFT)&& (sNewKey.u32Status == KEY_LONGDOWN))		//对码键
	{
		if(WLUnconnectedFlag)
		{		
			InterfaceInsertNote(WL_UNCONNECTED);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);
			ShowMain(&s_u32MainUpdateMethod);
			OSSemPost(g_stSemInterface);			
			return 0x01;
		}
#if 0			//对码成功zzw	。test测试用
		{
			s_u64InterfaceRfidDataBackup = 0x00;
			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
				BuzzerOn(BEEP_KEY_PRESSED);
			#endif
			StateLed(ADJUST_CODE_SUCCEED_LED,ADJUST_CODE_ING_LED);
			
			u8J = MANUAL_CTRL;
			LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);
			
			LogicRunInfApi(LOGIG_GET_ACTIONINFO_CRC_IDENCAL, &u8J);
			if(u8J == 0)	
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);//本架控制
			else
			{			
				InterfaceSetCtrlMode(INTERFACE_MODE_ACTIONINFI_SYN);//配置文件同步
			}
#if (HEART == 1)				
				//心跳		
				Heart.ActionType = NO_ACTION;
				Heart.SendMode = NO_ACTION;
				Heart.ActionValue = 0;
#endif
			//zzw		测试用，将对码架号设置为20
			u8J = 20;
			LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB, &u8J);
	
			InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_SUCCEED);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
			ShowMain(&s_u32MainUpdateMethod);	
			LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
			s_u32InterfaceColibrateTimer = u32Y;	
			u32Y = LOGIC_COLIBRATE_SC_IGNORE;
			LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);	
			if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_SELF_CONTROL)
				ShowScSelfKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);
			else	if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_UNCERTIFIED)
			{
				ShowScUncertifiedKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);
			}
			else
				;	
		}
#else	
		#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
			BuzzerOn(BEEP_COLIBRATE_START);
		#endif
		g_stLogicSem->OSEventCnt = 0;
		g_stKeySem->OSEventCnt = 0;
		u32Y = LOGIC_COLIBRATE_SC_SHIFT_BUTTON;
		LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);		
		u32Y = LOGIC_STATUS_IR_COLIBARAE_START;
		LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
		InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_START);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
		ShowMain(&s_u32MainUpdateMethod);
		u32ReturnFlag = 0x01;	
		LogicParamApi(LOGIC_GET_IR_SENDTIMES, &u16SendTimes);
		LogicParamApi(LOGIC_GET_IR_TINMEOUT, &u16DelayTime);
		//预估时间，包括了ir及RFID应答时间
		u32Y = (u32)(u16DelayTime*u16SendTimes);
		LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
		LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
		u32Y += (u32)(u16DelayTime*u16SendTimes);
		u32Y += 1000;//1000		
		while((u32Y > 0) && (u32Y < (u32)(0xffff))) //对码过程中有按键按下，会导致u32Y变为0xffffffff从而令原while(u32Y)无法执行完毕 2019.8.6修改 Parry
		{
			OSSemPend(g_stKeySem,INTERFACE_TASK_TIME_SET/TICK_TIME,&I8errKeyFlag);				
			if(I8errKeyFlag == OS_ERR_NONE)
			{
				u32Y = LOGIC_STATUS_IDLE;
				LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);	
				GetKey(&sNewKey);
				if((sNewKey.u32Status == KEY_UP)&&(sNewKey.u32Key == KEY_SHIFT))
					break;					
			}
			
			OSSemPend(g_stLogicSem,INTERFACE_TASK_TIME_SET/TICK_TIME,&err);	
			if(err == OS_ERR_NONE)
				break;			
			u32Y -= INTERFACE_TASK_TIME_SET*2;
		}

		if(I8errKeyFlag == OS_ERR_NONE)//有按键抬起动作
		{
			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
				BuzzerOn(BEEP_KEY_PRESSED);
			#endif
			StateLed(0x00,ADJUST_CODE_ING_LED);
			InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_STOP);//空闲
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
			InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
			u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);		
			u16DelayTime = RFID_SIGNAL_TOO_SMALL;
			LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DelayTime);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_RFIDSIGNAL);
			ShowMain(&s_u32MainUpdateMethod);	
			u32Y = LOGIC_COLIBRATE_SC_NONE;
			LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);				
		
		}
		else if(err == OS_ERR_TIMEOUT)//无应答
		{
			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
				BuzzerOn(BEEP_KEY_PRESSED);
			#endif
			InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_FAIL);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
			InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);	
			LogicRunInfApi(LOGIC_GET_RUN_STATUS,&u32Y);	
			if((u32Y >= LOGIC_STATUS_RFID_COLIBARAE_START) &&(u32Y <= LOGIC_STATUS_RFID_COLIBARAE_FAIL))
			{
				u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;
				LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);	
				OSTimeDly(200/TICK_TIME);
			}
			u16DelayTime = RFID_SIGNAL_TOO_SMALL;
			LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DelayTime);			
			ShowMain(&s_u32MainUpdateMethod);	
			StateLed(0x00,ADJUST_CODE_ING_LED);	
			u32Y = LOGIC_COLIBRATE_SC_NONE;
			LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);			
		}
		else if(err == OS_ERR_NONE)//有应答
		{
			/**关于设置的一些特殊模式切换的处理*/
			if(InterfaceGetCtrlMode() == INTERFACE_MODE_COLIBRATE_CODE_FAIL)
			{
				#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
					BuzzerOn(BEEP_KEY_PRESSED);
				#endif
				StateLed(0x00,ADJUST_CODE_ING_LED);
				InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_FAIL);//空闲
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
				InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
				LogicRunInfApi(LOGIC_GET_RUN_STATUS,&u32Y);	
				if((u32Y >= LOGIC_STATUS_RFID_COLIBARAE_START) &&(u32Y <= LOGIC_STATUS_RFID_COLIBARAE_FAIL))
				{
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);	
					OSTimeDly(200/TICK_TIME);
				}
				u16DelayTime = RFID_SIGNAL_TOO_SMALL;
				LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DelayTime);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_RFIDSIGNAL);
				ShowMain(&s_u32MainUpdateMethod);	
				u32Y = LOGIC_COLIBRATE_SC_NONE;
				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);				
			}
			else if(InterfaceGetCtrlMode() == INTERFACE_MODE_COLIBRATE_CODE_SUCCESS)		//对码成功
			{
				s_u64InterfaceRfidDataBackup = 0x00;
				#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
					BuzzerOn(BEEP_KEY_PRESSED);
				#endif
				StateLed(ADJUST_CODE_SUCCEED_LED,ADJUST_CODE_ING_LED);
				u8J = MANUAL_CTRL;
				LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);
				LogicRunInfApi(LOGIG_GET_ACTIONINFO_CRC_IDENCAL, &u8J);
				if(u8J == 1)	
				{
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);//本架控制
				#if (HEART == 1)				
					//心跳		
					SIGNAL_ACTION_HEART Heart = {0};
					Heart.ActionType = NO_ACTION;
					Heart.SendMode = NO_ACTION;
					Heart.ActionValue = 0;
					LogicRunInfApi(LOGIC_SET_HEART, &Heart);
				#endif
				}
				else
				{			
					InterfaceSetCtrlMode(INTERFACE_MODE_ACTIONINFI_SYN);//配置文件同步
				}	
				InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_SUCCEED);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
				ShowMain(&s_u32MainUpdateMethod);	
				LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
				s_u32InterfaceColibrateTimer = u32Y;	
				u32Y = LOGIC_COLIBRATE_SC_IGNORE;
				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);	
				if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_SELF_CONTROL)
					ShowScSelfKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);
				else	if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_UNCERTIFIED)
				{
					ShowScUncertifiedKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);
				}
				else
					;
			}
			else if(InterfaceGetCtrlMode() == INTERFACE_MODE_RELIEVE_COLIBRATE)
			{
			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
					BuzzerOn(BEEP_KEY_PRESSED);
				#endif
				StateLed(0x00,ADJUST_CODE_ING_LED);
				InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_FAIL);//空闲
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
				InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
				LogicRunInfApi(LOGIC_GET_RUN_STATUS,&u32Y);	
				u8J = NO_CTRL;
				LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);
				if((u32Y >= LOGIC_STATUS_RFID_COLIBARAE_START) &&(u32Y <= LOGIC_STATUS_RFID_COLIBARAE_FAIL))
				{
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);	
					OSTimeDly(200/TICK_TIME);
				}
				u16DelayTime = RFID_SIGNAL_TOO_SMALL;
				LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DelayTime);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_RFIDSIGNAL);
				ShowMain(&s_u32MainUpdateMethod);	
				u32Y = LOGIC_COLIBRATE_SC_NONE;
				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);	
			}
		}
		else
			;
#endif
		StateLed(0x00, KEY_SHIFT_LED);
		s_u32ShiftKeyEnableFlag = DISABLED;	
	}
	else if((sNewKey.u32Key == KEY_SHIFT)&& (sNewKey.u32Status == KEY_UP))
	{
		//InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_IDLE);
		//InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
		//OSSemPost(g_stSemInterface);		
	}
	else if(((sNewKey.u32Key&KEY_STOP_SHUTDOWN) == KEY_STOP_SHUTDOWN)&& (sNewKey.u32Status == KEY_DOWN))//KEY_DOWN
	{

		KeyLedOnOff(0x00, KEY_LED_ALL);  //qinxi
		if(s_stScrAwake == INTERFACE_SCR_PROTECED)
		{
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_AWAKE);
			OSSemPost(g_stSemInterface);			
		}
		else
		{
			u16StopKeyPressFlag = 0x01;			
		}		
	}		
	else if(((sNewKey.u32Key&KEY_STOP_SHUTDOWN) == KEY_STOP_SHUTDOWN)&& (sNewKey.u32Status == KEY_UP)&&(u16StopKeyPressFlag == 0x01))//KEY_DOWN
	{
//		timestest = 0;
		
		u16StopKeyPressFlag = 0x00;
		if(s_stScrAwake == INTERFACE_SCR_AWAKE && (!fastColiearateFlag||u8FastColiearateScNumFlag == 0))
		{
			
//			if(g_u32ID != B_type&&fastColiearateFlag == 0)
//			{
//				WirelessChipPwrOff();
//			}
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_PROTECED);
			KeyLedOnOff(0x00,KEY_LED_ALL);
			OSSemPost(g_stSemInterface);		
		}
		else if(s_stScrAwake == INTERFACE_SCR_AWAKE && fastColiearateFlag && u8FastColiearateScNumFlag == 1)
		{
			u8FastColiearateScNumFlag = 0;
			fastColiearateScNum = 0;
			InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_IDLE);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);			
			ShowMain(&s_u32MainUpdateMethod);
		}
	}
	else if(((sNewKey.u32Key&KEY_STOP_SHUTDOWN) == KEY_STOP_SHUTDOWN)&& (sNewKey.u32Status == KEY_LONGDOWN))
	{
		u16StopKeyPressFlag = 0x00;
		#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
			BuzzerOn(BEEP_SHUTDOWN_NOTE);
		#endif
		OSTaskSuspend(SCAN_KEY_TASK_PRIO);//挂起按键扫描任务
		InterfaceSetCtrlMode(INTERFACE_MODE_SHUTSOWN);		
		InterfaceInsertNote(INTERFACE_NOTE_SHUTDOWN);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL|INTERFACE_SHOW_MAIN_AWAKE);	
		u32Y = LOGIC_STATUS_SHUTUP;
		LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);			
		ShowMain(&s_u32MainUpdateMethod);	
		#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
			KeyLedOnOff(0x00,KEY_LED_ALL);
		#endif			
		u32ReturnFlag = 0x01;	
	}
	#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
		/**判断是否为设置参数的按键**/
		if((sNewKey.u32Key == (KEY_PRGPT_SET_KEY1|KEY_PRGPT_SET_KEY2))&&(sNewKey.u32Status == KEY_LONGDOWN))		//程序接收界面
		{
			#if (YKQ_PRG_BEEP_USE_FLAG == 0x01)
				BuzzerOn(BEEP_COLIBRATE_START);
			#endif				
			KeyLedOnOff(0x00,KEY_LED_MAX);
			if(RfidGetDataLimitlessSendFlag())
			{
				RfidSetDataLimitlessSendEnd();
			}				
			InterfaceSetCtrlMode(INTERFACE_MODE_RECV_PRG_START);
			
			u32Y = LOGIC_STATUS_RFID_RECV_PRG;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);	
			/*计算总共需要多少时间*/
			InterfaceSetDownloadRemainTimer(0x0000);
			InterfaceInsertNote(INTERFACE_NOTE_PLEASE_DONOT_MOVE);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);
			OSSemPost(g_stSemInterface);

//			OS_ENTER_CRITICAL();
			if(g_u32ID != B_type)
				OSTaskSuspend(USART_TASK_PRIO);

			OSTaskSuspend(IRMNG_TASK_PRIO);
			OSTaskDel(RFIDMNG_TASK_PRIO);
	
			/**无线更新处理任务**/
			if(g_u32ID == B_type)
			{			
				RFID_Init(RFID_PRG_ININT_SYNC0,RFID_PRG_ININT_SYNC1);
				RfidMngSetRecvCallBack();
			}
//			else
//			{
//				SendSetupToWl(RFID_PRG_ININT_SYNC1);				//马上切同步码可能导致无线模块收到到
//			}

			/**无线更新程序处理任务**/
			#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
				RfidUpdateMngInint();
				OSTaskCreate(RfidMng_Update_Task, (void *)0, &RfidMng_task_stk[RFIDMNG_UPDATE_TASK_STK_SIZE - 1], RFIDMNG_UPDATE_TASK_PRIO); 
			#endif
//			OS_EXIT_CRITICAL();	
			
			StateLed(ADJUST_CODE_ING_LED,ADJUST_CODE_SUCCEED_LED);
			return u32ReturnFlag;
		}
	#endif
	
//		else if(((sNewKey.u32Key == KEY_SHIFT) || (sNewKey.u32Key == KEY_INCREASE)\
//		|| (sNewKey.u32Key == KEY_DECREASE)|| (sNewKey.u32Key == KEY_INTERFACE_N) \
//		|| (sNewKey.u32Key == KEY_INTERFACE_Q)|| (sNewKey.u32Key == KEY_LEFT_GRP)\
//		|| (sNewKey.u32Key == KEY_RIGHT_GRP)|| (sNewKey.u32Key == KEY_LEFT_NEIGHBOR)\
//		|| (sNewKey.u32Key == KEY_RIGHT_NEIGHBOR))&&(sNewKey.u32Status == KEY_DOWN))
//		{
//			timestest	++;
//		}
	else if((sNewKey.u32Key != KEY_SHIFT) && (sNewKey.u32Key != KEY_MENU_START)\
		&& (sNewKey.u32Key != KEY_STOP_SHUTDOWN) && (sNewKey.u32Key != KEY_INTERFACE_N) \
		&& (sNewKey.u32Key != KEY_INTERFACE_Q) && (sNewKey.u32Key != KEY_LEFT_GRP)\
		&& (sNewKey.u32Key != KEY_RIGHT_GRP) && (sNewKey.u32Key != KEY_LEFT_NEIGHBOR)\
		&& (sNewKey.u32Key != KEY_RIGHT_NEIGHBOR))
	{
		if(!fastColiearateFlag)//jhy   快速对码
		{
			if((sNewKey.u32Status == KEY_DOWN) ||((sNewKey.u32Status == KEY_LONGDOWN)))
			{
				InterfaceInsertNote(INTERFACE_NOTE_PRESSED_BUTTON_INVAILD);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
				OSSemPost(g_stSemInterface);
			}
			else
			{
				KeyLedOnOff(0x00, KEY_LED_ALL);
			}
		}
		else
		{
			if((sNewKey.u32Status == KEY_DOWN)&&((sNewKey.u32Key & (u32)0xcff0) != (u32)0))
			{
				if(g_u32ID != B_type&&u8FastColiearateScNumFlag == 0)
				{
					WirelessChipPwrOn();
					SendSetupToWl(RFID_ININT_SYNC1);
				}
				
//			timestest	++;
				
				InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_IDLE);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);			
				ShowMain(&s_u32MainUpdateMethod);
				
				u8FastColiearateScNumFlag = 1;
				if(g_u32ID != B_type)
					KeyLedOnOff(KEY_LED_NUMBER, 0x00);  
				else
					KeyLedOnOff(KEY_LED_ALL, 0x00);
				s_u32NoteShowTimer = INTERFACE_NOTE_TIMER_STOP;
				
				u8I = calcNumProc(sNewKey.u32Key);
				if(u8I != 0xff)
				{
					fastColiearateScNum = (fastColiearateScNum*10 +u8I)%1000;
				}
				sprintf(s,"对码架号:%3d",fastColiearateScNum);
				ShowStr(0, (const u8 *)s, 0x06, 16);
					
			}
			if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_CERTIFY)&& fastColiearateScNum != 0)
			{
				
				if(WLUnconnectedFlag)
				{
					InterfaceInsertNote(WL_UNCONNECTED);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);
					ShowMain(&s_u32MainUpdateMethod);
					OSSemPost(g_stSemInterface);
					return 0x01;					
				}
				
				u8FastColiearateScNumFlag = 0;
				g_stLogicSem->OSEventCnt = 0;
				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&fastColiearateScNum);	
				u32Y = LOGIC_COLIBRATE_SC_INCREASE_DECREASE;
				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);
				u32Y = LOGIC_STATUS_IR_COLIBARAE_SUCCESS;
				LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
				InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_START);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
				ShowMain(&s_u32MainUpdateMethod);
				u32ReturnFlag = 0x01;				
				//预估时间，包括了RFID应答时间
				u32Y = 0x00;
				LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
				LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
				u32Y += (u32)(u16DelayTime*u16SendTimes);
				u32Y += 3000;//3000
					
				OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
				if(err == OS_ERR_TIMEOUT)//无应答
				{
					u32Y = LOGIC_STATUS_IDLE;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
					#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)				
						BuzzerOn(BEEP_UNCOLIBRAT);
					#endif
					u16DelayTime = RFID_SIGNAL_TOO_SMALL;
					LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DelayTime);	
					
					StateLed(0x00,ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED|KEY_SHIFT_LED);
					InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_FAIL);//空闲
					InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);				
					ShowMain(&s_u32MainUpdateMethod);		
					KeyLedOnOff(0x00, KEY_LED_ALL);
					s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
					
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//先解除对码
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
					OSTimeDly(200/TICK_TIME);
					
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//后解除对码
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
									
					u32Y = LOGIC_COLIBRATE_SC_NONE;
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);
					
					SendSetupToWl(RFID_ININT_SYNC1);//jhyjhy

				}
				else if(err == OS_ERR_NONE)//有应答
				{
					/**关于设置的一些特殊模式切换的处理*/
					if(InterfaceGetCtrlMode() == INTERFACE_MODE_COLIBRATE_CODE_FAIL)
					{
						u32Y = LOGIC_STATUS_IDLE;
						LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);	
						#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)	
							BuzzerOn(BEEP_UNCOLIBRAT);
						#endif
						u16DelayTime = RFID_SIGNAL_TOO_SMALL;
						LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DelayTime);	
						StateLed(0x00,ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED|KEY_SHIFT_LED);
						InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_FAIL);//空闲
						InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);					
						ShowMain(&s_u32MainUpdateMethod);	
						KeyLedOnOff(0x00, KEY_LED_ALL);
						s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
						
//						LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&fastColiearateScNum);
//						u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//先解除对码
//						LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
//						OSTimeDly(200/TICK_TIME);
						
						LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&fastColiearateScNum);	
						u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//后解除对码
						LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
										
						u32Y = LOGIC_COLIBRATE_SC_NONE;
						LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);					
					}
					else if(InterfaceGetCtrlMode() == INTERFACE_MODE_COLIBRATE_CODE_SUCCESS)
					{
						LogicRunInfApi(LOGIC_SET_CONLIBRATE_SUCCESS_SC_NUMB,&fastColiearateScNum);
						#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
							BuzzerOn(BEEP_KEY_PRESSED);
						#endif
						StateLed(ADJUST_CODE_SUCCEED_LED,ADJUST_CODE_ING_LED);
						/**在使用加减键的情况下，直接进入动作状态***/
						InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);//本架控制模式	
						u8J = 0;
						LogicRunInfApi(LOGIG_GET_ACTIONINFO_CRC_IDENCAL, &u8J);
						if(u8J == 1)
						{							
							InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);//本架控制
						#if (HEART == 1)
							//心跳		
							Heart.ActionType = NO_ACTION;
							Heart.SendMode = NO_ACTION;
							Heart.ActionValue = 0;
							LogicRunInfApi(LOGIC_SET_HEART, &Heart);
						#endif
						}
						else
						{			
							InterfaceSetCtrlMode(INTERFACE_MODE_ACTIONINFI_SYN);//配置文件同步
						}							
						u8J = MANUAL_CTRL;
						LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);		//zzw
						InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_SUCCEED);
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
						ShowMain(&s_u32MainUpdateMethod);		
						ShowScSelfKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);				
					}
					else if(InterfaceGetCtrlMode() == INTERFACE_MODE_RELIEVE_COLIBRATE)
					{
						u32Y = LOGIC_STATUS_IDLE;
						LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);	
						#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)					
							BuzzerOn(BEEP_UNCOLIBRAT);
						#endif
						u8J = NO_CTRL;
						LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);		//zzw
						u16DelayTime = RFID_SIGNAL_TOO_SMALL;
						LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DelayTime);	
						StateLed(0x00,ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED|KEY_SHIFT_LED);
						InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_FAIL);//空闲
						InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);
						u8J = NO_CTRL;
						LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);						
						ShowMain(&s_u32MainUpdateMethod);	
						KeyLedOnOff(0x00, KEY_LED_ALL);
						s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
						
						LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&fastColiearateScNum);
						u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//先解除对码
						LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
						OSTimeDly(200/TICK_TIME);
									
										
						u32Y = LOGIC_COLIBRATE_SC_NONE;
						LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);									
					}
				}
				else
					;
				fastColiearateScNum = 0;	
				}
			else
			;
			
		}
		u32ReturnFlag = 0x01;
	}
	else if((sNewKey.u32Key == KEY_INTERFACE_N)||(sNewKey.u32Key == KEY_INTERFACE_Q)\
			||(sNewKey.u32Key == KEY_LEFT_GRP)||(sNewKey.u32Key == KEY_RIGHT_GRP)\
			||(sNewKey.u32Key == KEY_LEFT_NEIGHBOR)||(sNewKey.u32Key == KEY_RIGHT_NEIGHBOR))
	{
		if(sNewKey.u32Status == KEY_UP)
			KeyLedOnOff(0x00, KEY_LED_ALL);
	}
	else
		;
	return u32ReturnFlag;
}

/*******************************************************************************************
**函数名称：InterfaceModeSelfSetProc
**函数作用：显示层处于设置自身参数模式下，按键的处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
static u32 InterfaceModeSelfSetProc(stKEY	sNewKey)
{
	u32 u32ReturnFlag = 0x00;
	u32 u32Y = 0x00;
	
	InterfaceSetCtrlMode(INTERFACE_MODE_SELF_SET);
	InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_AWAKE);
	
	if((sNewKey.u32Status == KEY_UP) && (sNewKey.u32Key == KEY_MENU_START))
	{
		return 0x00;
	}

	/**在任何模式下，均识别关机键*/
	if(((sNewKey.u32Key&KEY_STOP_SHUTDOWN) == KEY_STOP_SHUTDOWN)&& (sNewKey.u32Status == KEY_LONGDOWN))
	{
		#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
			BuzzerOn(BEEP_SHUTDOWN_NOTE);
		#endif
		OSTaskSuspend(SCAN_KEY_TASK_PRIO);//挂起按键扫描任务
		InterfaceSetCtrlMode(INTERFACE_MODE_SHUTSOWN);
		
		InterfaceInsertNote(INTERFACE_NOTE_SHUTDOWN);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);	
		u32ReturnFlag = LOGIC_STATUS_SHUTUP;
		LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32ReturnFlag);			
		ShowMain(&s_u32MainUpdateMethod);
		u32ReturnFlag = 0x01;	
		ShowSelfSetKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);			
		return u32ReturnFlag;
	}
 	else
		;
	if((sNewKey.u32Status == KEY_DOWN) )
	{	
		u32Y = ParEdit(sNewKey,&s_u32SelfSetIntfState);
		if(u32Y == 0x01)
		{
			InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_IDLE);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);
			InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
			ShowMain(&s_u32MainUpdateMethod);			
			u32ReturnFlag = 0x01;
			ShowSelfSetKeyLedProc(KEY_STOP_SHUTDOWN, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);				

		} 
		else if(u32Y == 0x02)//
		{
			;
		}
		else
		{
			ShowSelfSetKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);
		}		
	}
	else if(sNewKey.u32Status == KEY_UP)
	{
		ShowSelfSetKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);
	}
	else
		;
	
	return u32ReturnFlag;
}

/*******************************************************************************************
**函数名称：InterfaceModeScSetProc
**函数作用：显示层处于支架控制器参数设置模式下，按键的处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
static u32 InterfaceModeScSetProc(stKEY	sNewKey)
{
	u32 u32ReturnFlag = 0x00;
	INT8U err;
	static u64 u64ButtonValue;
	static u64 u64ButtonAckValue;
	static u64 u64stRfidType;
	SC_BUTTON_TYPE status;
	u32 u32Y = 0x00;
	static u64 u64Y;
	static u64 u64RfidData;
	RFID_FRAME_TYPE stRfidType;
	static u32 u32StopKeyFlag = KEY_UP;//
	u16 u16DelayTime = 0x00;
	u16 u16SendTimes = 0x00;
	
	u64Y = 0x00;
	u64RfidData = RFID_ACTION_NONE;

	s_u32AllKeyStatus = sNewKey.u32Status;
	LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
	s_u32InterfaceColibrateTimer = u32Y;	
	InterfaceSetCtrlMode(INTERFACE_MODE_SC_SET);
	InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_AWAKE);
	if((sNewKey.u32Key&KEY_STOP_SHUTDOWN) == KEY_STOP_SHUTDOWN)
	{	
		sNewKey.u32Key = KEY_STOP_SHUTDOWN;
		s_u32ShiftKeyEnableFlag = DISABLED;
		StateLed(0x00, KEY_SHIFT_LED);
	}
	u64RfidData = InterfaceGetKeyRfidDataShowName(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag,sNewKey.u32Key,&(s_stInterfaceActionName[0x00]),&stRfidType);
	u64stRfidType = (u64)stRfidType;
	/*按键无效时的处理**/
	if(u64RfidData == RFID_ACTION_NONE)
	{
		InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
		ShowMain(&s_u32MainUpdateMethod);
		if(RfidGetDataLimitlessSendFlag())
		{
			RfidSetDataLimitlessSendEnd();
		}	
		return u32ReturnFlag;
	}
	
	if((sNewKey.u32Key == KEY_SHIFT)&& (sNewKey.u32Status == KEY_DOWN))
	{
		InterfaceInsertNote(INTERFACE_NOTE_BUTTON_SHIFT_DOWN);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
		OSSemPost(g_stSemInterface);	
	}		
	else if((sNewKey.u32Key == KEY_SHIFT)&& (sNewKey.u32Status == KEY_UP))
	{

	}	
	/**识别解除对码键(长按SHIFT，解除对码)**/
	else if((sNewKey.u32Key == KEY_SHIFT)&&(sNewKey.u32Status == KEY_LONGDOWN))
	{
		#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
			BuzzerOn(BEEP_UNCOLIBRAT);
		#endif
		s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
		InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
		StateLed(0x00,KEY_SHIFT_LED);
		StateLed(0x00,ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED);
		u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;
		LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
		u16DelayTime = RFID_SIGNAL_TOO_SMALL;
		LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DelayTime);		
		InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_UNCOLIBRATE);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);				
		ShowMain(&s_u32MainUpdateMethod);

		KeyLedOnOff(0x00, KEY_LED_ALL);
		OSTimeDly(200/TICK_TIME);	
		u32ReturnFlag = 0x01;		
	}	
	else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_LONGDOWN))
	{
		u32StopKeyFlag = KEY_LONGDOWN;
		#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
			BuzzerOn(BEEP_SHUTDOWN_NOTE);
		#endif
		OSTaskSuspend(SCAN_KEY_TASK_PRIO);//挂起按键扫描任务
		#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
			KeyLedOnOff(0x00,KEY_LED_ALL);
		#endif			
		s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
		InterfaceInsertNote(INTERFACE_NOTE_SHUTDOWN);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
		ShowMain(&s_u32MainUpdateMethod);
		u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;
		LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);				
		OSTimeDly(200/TICK_TIME);			
		
		u32Y = LOGIC_STATUS_SHUTUP;
		LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);			
		OSTimeDly(1000/TICK_TIME);
		u32ReturnFlag = 0x01;		
	}
	else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_DOWN))
	{
		u32StopKeyFlag = KEY_DOWN;
	}	
	else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_UP))//对停止键做特殊处理
	{
		if(u32StopKeyFlag == KEY_LONGDOWN)/**在任何模式下，均识别关机键*/
		{
			return u32ReturnFlag;			
	
		}
		else if(u32StopKeyFlag == KEY_DOWN)//停止键
		{
			/**键值显示**/
			LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
			LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//发送的数据
			g_stLogicSem->OSEventCnt = 0;
			u32Y = SC_BUTTON_ACK_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//将应答设为无效	
			u64Y = RFID_ACTION_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u64Y);//将应答动作字设为无效			
			u64Y = (u64)(u64RfidData|(u64stRfidType<<62));
			LogicRunInfApi(LOGIC_SET_BUTTON_VALUE,&u64Y);
			InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL);				
			ShowMain(&s_u32MainUpdateMethod);	
			LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
			LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
			u32Y = (u32)(u16DelayTime*u16SendTimes);	
			u32Y += 30;
			
			OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);			
			if(err == OS_ERR_TIMEOUT)//无应答
			{
				InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);		
				ShowMain(&s_u32MainUpdateMethod);							
			}
			else if(err == OS_ERR_NONE)//有应答
			{
				/**关于设置的一些特殊模式切换的处理*/
				LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
				LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u64ButtonValue);
				LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&u64ButtonAckValue);
				if((status == SC_BUTTON_ACK_ACCEPT) && (u64ButtonAckValue == u64ButtonValue))
				{
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
					#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
						InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);					
					#else
						InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
					#endif
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
					u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
					u32Y = 0x00;
					LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);	
					LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32Y);
					ShowMain(&s_u32MainUpdateMethod);							
						
				}
				else if(status == SC_BUTTON_ACK_REJECT)
				{
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
					#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
						InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);					
					#else
						InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
					#endif
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
					u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
					ShowMain(&s_u32MainUpdateMethod);							
				}				
				else
				{
					InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
					#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
						InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);					
					#else
						InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
					#endif
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息					
					u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
					ShowMain(&s_u32MainUpdateMethod);					
				}
			}
			else
				;
		
			u32ReturnFlag = 0x01;	
			
			
			if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_SELF_CONTROL)
				ShowScSelfKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);
			else if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_UNCERTIFIED)
				ShowScUncertifiedKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);
			else
				;
		}
		u32StopKeyFlag = KEY_UP;			
	}
	else if((sNewKey.u32Status == KEY_DOWN))
	{
		/**键值显示**/
		LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
		LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//发送的数据		
		g_stLogicSem->OSEventCnt = 0;
		u32Y = SC_BUTTON_ACK_NONE;
		LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//将应答设为无效	
		u64Y = RFID_ACTION_NONE;
		LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u64Y);//将应答动作字设为无效
		u64Y = (u64)(u64RfidData|(u64stRfidType<<62));
		LogicRunInfApi(LOGIC_SET_BUTTON_VALUE,&u64Y);
		InterfaceInsertNote(INTERFACE_NOTE_BUTTON_DOWN);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SET|INTERFACE_SHOW_MAIN_NOTE);
		ShowMain(&s_u32MainUpdateMethod);
		LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
		LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
		u32Y = (u32)(u16DelayTime*u16SendTimes);	
		u32Y += 30;
		
		OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
		if(err == OS_ERR_TIMEOUT)//无应答
		{
			InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);					
			if(sNewKey.u32Key != KEY_STOP_SHUTDOWN)
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_SET);
			else
			{
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);	
			}
			ShowMain(&s_u32MainUpdateMethod);		
		}
		else if(err == OS_ERR_NONE)//有应答
		{
			/**关于设置的一些特殊模式切换的处理*/
			LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
			LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u64ButtonValue);
			LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&u64ButtonAckValue);
			if((status == SC_BUTTON_ACK_ACCEPT) && (u64ButtonAckValue == u64ButtonValue))//按键接受
			{
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
			}

			else  if(status == SC_BUTTON_ACK_REJECT)
			{
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
			}
			else
			{
				InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
			}			
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
			if(sNewKey.u32Key != KEY_STOP_SHUTDOWN)
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_SET);
			else
			{
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);	
			}
			u32Y = LOGIC_STATUS_RFID_SC_SET;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);				
			ShowMain(&s_u32MainUpdateMethod);			
			
		}
		else
			;
		
		u32ReturnFlag = 0x01;					
		
		ShowScSetKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);
	}
	else if((sNewKey.u32Status == KEY_UP)&&(sNewKey.u32Key != KEY_STOP_SHUTDOWN)&&(sNewKey.u32Key != KEY_SHIFT))
	{
		for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
			s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示		
		InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SET);//显示主屏所有信息
		ShowMain(&s_u32MainUpdateMethod);	
		ShowScSetKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);
	}
	else
		;
	
	return u32ReturnFlag;
}

//清除动作信息
void ClearActionInfo(void)
{
	RuningActionCnt = 0;
	memset(ActionInfoMsg,0,sizeof(ActionInfoMsg));	
}
//确认键处理（模式切换）zzw
u8 InterfaceKeyCertifyPro(stKEY	sNewKey,u32 LogicRunStatus,INTERFACE_CONTROL_MODE_TYPE stNewMode,void *pDate)
{
	u8 u8J = 0xff;
	if((sNewKey.u32Key == KEY_CERTIFY)&& (sNewKey.u32Status == KEY_DOWN))		//对确认键做特殊处理
	{
		LogicParamApi(LOGIC_GET_CTRL_MODE,&u8J);
		u8J = (u8J == MANUAL_CTRL?AUTO_CTRL:MANUAL_CTRL);
		LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);
		if(u8J == MANUAL_CTRL)
			InterfaceInsertNote(INTERFACE_NOTE_AC_TO_MC);
		else
			InterfaceInsertNote(INTERFACE_NOTE_MC_TO_AC);
		if(ActionInfoMsg[0].ActionType ==COMBINATION_ACTION)
			ClearActionInfo();
		switch(LogicRunStatus)
		{
			case LOGIC_STATUS_RFID_SC_SELF_CONTROL:		//本架控制
			{
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_CTRL_MODE);//显示主屏所有信息
			}break;
			case LOGIC_STATUS_RFID_SC_NEIGHBORL:		//邻架控制
			{
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_NEAR_CONTROL|INTERFACE_SHOW_CTRL_MODE);//显示主屏所有信息
			}break;
			case LOGIC_STATUS_RFID_SC_GRP:				//成组控制
			{
//				GetActionInfoByID(1,INTERFACE_MODE_SC_GRP_CONTROL);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_GRP_CONTROL|INTERFACE_SHOW_CTRL_MODE);//显示主屏所有信息
			}break;
			default:
				break;
		}
		InterfaceSetCtrlMode(stNewMode);
		LogicRunInfApi(LOGIC_SET_RUN_STATUS, &LogicRunStatus);				
		ShowMain(&s_u32MainUpdateMethod);	
	}

	if((sNewKey.u32Key == KEY_CERTIFY)&& (sNewKey.u32Status == KEY_UP))		//对确认键做特殊处理
	{
		u8 err;
		for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
			s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示		
		ClearUpdateMainMethod();	
		LogicParamApi(LOGIC_GET_CTRL_MODE,&u8J);
		if(u8J == MANUAL_CTRL)
			InterfaceInsertNote(INTERFACE_NOTE_AC_TO_MC);
		else
			InterfaceInsertNote(INTERFACE_NOTE_MC_TO_AC);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
		switch(LogicRunStatus)
		{
			case LOGIC_STATUS_RFID_SC_SELF_CONTROL:		//本架控制
			{
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_CTRL_MODE);//显示主屏所有信息
			}break;
			case LOGIC_STATUS_RFID_SC_NEIGHBORL:		//邻架控制
			{
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_NEAR_CONTROL|INTERFACE_SHOW_CTRL_MODE);//显示主屏所有信息
			}break;
			case LOGIC_STATUS_RFID_SC_GRP:				//成组控制
			{
				;//InterfaceSetUpdateMainMethod(INTERFACE_SHOW_CTRL_MODE);//显示主屏所有信息
			}break;
			default:
				break;
		}
		ShowMain(&s_u32MainUpdateMethod);			
	}
	//更新指示灯显示
	if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
		ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
	else if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_UNCERTIFIED)
		ShowScUncertifiedKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
	else if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_GRP_CONTROL)
		ShowScGrpKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
	else
		;	
	return 0x01;
}

//邻架键处理
u8 InterfaceKeyNeighborPro(stKEY sNewKey,u32 LogicRunStatus,INTERFACE_CONTROL_MODE_TYPE stNewMode,void *pDate)
{
	u32 u32Y = 0,u32I = 0,u32K = 0,u32ActionType = 0;
	u16 u16SendTimes,u16DelayTime;
	u8 err;
	//将左右邻架键改为成组键 zzw2023.12.25
	if(((sNewKey.u32Key == KEY_LEFT_GRP) || (sNewKey.u32Key == KEY_RIGHT_GRP)) && (sNewKey.u32Status == KEY_DOWN))		//邻架键		
	{
		if(stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
		{
			u32Y = 0x00;
			LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);//对码架架号
			LogicRunInfApi(LOGIC_GET_GRP_NEAR_SC_NUMB,&u32I);//近端架号
			LogicRunInfApi(LOGIC_GET_GRP_ACTION_DIRECTION,&u32K);//动作执行方向
			LogicRunInfApi(LOGIC_GET_SC_NUMB_ADD_DIRCT,&u32ActionType);//架号增向			
			if(sNewKey.u32Key == KEY_LEFT_NEIGHBOR)//左邻架键
			{	
				if(u32ActionType == SC_DIRCT_RIGHT)
				{
					if(u32I <= u32Y)//近端架号\对码架架号
					{
						if(u32K == SC_GRP_ACTION_REMOTE_NEAR)//动作执行方向
						{
							u32K = SC_GRP_ACTION_NEAR_REMOTE;
							LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION,&u32K);//动作执行方向								
						}
					}
					else
					{
						if(u32K == SC_GRP_ACTION_NEAR_REMOTE)//动作执行方向
						{
							u32K = SC_GRP_ACTION_REMOTE_NEAR;
							LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION,&u32K);//动作执行方向								
						}
					}				
				}
				else
				{
					if(u32I >= u32Y)//近端架号\对码架架号
					{
						if(u32K == SC_GRP_ACTION_REMOTE_NEAR)//动作执行方向
						{
							u32K = SC_GRP_ACTION_NEAR_REMOTE;
							LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION,&u32K);//动作执行方向
						}							
					}
					else
					{
		
						if(u32K == SC_GRP_ACTION_NEAR_REMOTE)//动作执行方向
						{
							u32K = SC_GRP_ACTION_REMOTE_NEAR;
							LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION,&u32K);//动作执行方向
						}				
					}				
				
				}						
			}
			else//方向右键
			{
				if(u32ActionType == SC_DIRCT_RIGHT)
				{
					if(u32I <= u32Y)//近端架号\对码架架号
					{
						if(u32K == SC_GRP_ACTION_NEAR_REMOTE)//动作执行方向
						{
							u32K = SC_GRP_ACTION_REMOTE_NEAR;
							LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION,&u32K);//动作执行方向
						}
					}
					else
					{
						if(u32K == SC_GRP_ACTION_REMOTE_NEAR)//动作执行方向
						{
							u32K = SC_GRP_ACTION_NEAR_REMOTE;
							LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION,&u32K);//动作执行方向
						}							
					}				
				}
				else
				{
					if(u32I >= u32Y)//近端架号\对码架架号
					{
						if(u32K == SC_GRP_ACTION_NEAR_REMOTE)
						{
							u32K = SC_GRP_ACTION_REMOTE_NEAR;
							LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION,&u32K);//动作执行方向
						}
						
					}
					else
					{
		
						if(u32K == SC_GRP_ACTION_REMOTE_NEAR)
						{
							u32K = SC_GRP_ACTION_NEAR_REMOTE;
							LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION,&u32K);//动作执行方向
						}
					}	
				}				
			}
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);				
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_GRP_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
			ShowMain(&s_u32MainUpdateMethod);		
		}
		else
		{
			LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32K);
			LogicRunInfApi(LOGIC_GET_SC_NUMB_ADD_DIRCT,&u32Y);
			LogicRunInfApi(LOGIC_GET_CONTRL_SC_NUMB,&u32I);  //jhy
			//防止架号设1和250极端，再按下左右邻架   jhy
			u32Y = u32Y&0xff;
			u32I = u32I&0xff;
			u32K = u32K&0xff;
			
			if(u32K == SC_NUM_MIN)
			{
				if(u32Y == SC_DIRCT_RIGHT && u32I <= SC_NUM_MIN+1 && sNewKey.u32Key == KEY_LEFT_GRP)
				{
					return 0;
				}
				else if(u32Y == SC_DIRCT_LEFT && u32I <= SC_NUM_MIN+1 && sNewKey.u32Key == KEY_RIGHT_GRP)
				{
					return 0;
				}
			}
			else
			{
				if(u32Y == SC_DIRCT_RIGHT && u32I <= SC_NUM_MIN && sNewKey.u32Key == KEY_LEFT_GRP)
				{
					return 0;
				}
				else if(u32Y == SC_DIRCT_LEFT && u32I <= SC_NUM_MIN && sNewKey.u32Key == KEY_RIGHT_GRP)
				{
					return 0;
				}
			}
			
			LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32K);
			u32K = u32K&0xff;
			if(u32K == SC_NUM_MAX)
			{
				if(u32Y == SC_DIRCT_RIGHT && u32I >= SC_NUM_MAX-1 && sNewKey.u32Key == KEY_RIGHT_GRP)
				{
					return 0;
				}
				else if(u32Y == SC_DIRCT_LEFT && u32I >= SC_NUM_MAX-1 && sNewKey.u32Key == KEY_LEFT_GRP)
				{
					return 0;
				}
			}
			else
			{
				if(u32Y == SC_DIRCT_RIGHT && u32I >= SC_NUM_MAX && sNewKey.u32Key == KEY_RIGHT_GRP)
				{
					return 0;
				}
				else if(u32Y == SC_DIRCT_LEFT && u32I >= SC_NUM_MAX && sNewKey.u32Key == KEY_LEFT_GRP)
				{
					return 0;
				}
			}
			//设置动作类型
			if(sNewKey.u32Key == KEY_LEFT_GRP)
			{
				u32ActionType = SC_ACTION_LEFT_CTRL;
				LogicRunInfApi(LOGIC_SET_SC_ACTION_TYPE,&u32ActionType);
			}
			else
			{
				u32ActionType = SC_ACTION_RIGHT_CTRL;
				LogicRunInfApi(LOGIC_SET_SC_ACTION_TYPE,&u32ActionType);		
			}
			LogicRunInfApi(LOGIC_GET_CONTRL_SC_NUMB,&u32I);
			if((sNewKey.u32Key == KEY_LEFT_GRP && u32Y == SC_DIRCT_RIGHT)
				||(sNewKey.u32Key == KEY_RIGHT_GRP && u32Y == SC_DIRCT_LEFT))
			{
				u32I--;
				if(u32I == u32K)		//解决能邻架控制能切到本架问题，目的是与D版靠近实际没什么用
					u32I--;
			}
			else
			{
				u32I++;	
				if(u32I == u32K)		
					u32I++;
			}
					//清除心跳
			if(RfidGetDataLimitlessSendFlag())
			{
				RfidSetDataLimitlessSendEnd();
			}
			//更新显示
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_DOWN);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
			ShowMain(&s_u32MainUpdateMethod);
					//清除心跳
			if(RfidGetDataLimitlessSendFlag())
			{
				RfidSetDataLimitlessSendEnd();
			}
			//下发邻架控制请求
			g_stLogicSem->OSEventCnt = 0;
			LogicRunInfApi(LOGIC_SET_NEAR_CTRL_REQ, &u32I);
			//刷新等待时间
			LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
			LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
			u32Y = (u32)(u16DelayTime*u16SendTimes);	
			u32Y += 30;

			OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
			if(err == OS_ERR_TIMEOUT)//无应答
			{
				InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
				InterfaceSetCtrlMode(stNewMode);
				ShowMain(&s_u32MainUpdateMethod);	
				ShowScUncertifiedKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);	
			}
			else if(err == OS_ERR_NONE)//有应答
			{
//					LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32I);	
				/*为了修改控制的架号**/
				u32Y = LOGIC_STATUS_RFID_SC_NEIGHBORL;
				LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);					
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_NEAR_CONTROL|INTERFACE_SHOW_MAIN_NOTE);
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_NEIGHBOR_CONTROL);	
			}
			else
				;				
		}
		
		if(((sNewKey.u32Key == KEY_LEFT_GRP) || (sNewKey.u32Key == KEY_RIGHT_GRP)) && (sNewKey.u32Status == KEY_UP))			//邻架键
		{
			InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);
			InterfaceSetCtrlMode(INTERFACE_MODE_SC_NEIGHBOR_CONTROL);	
			
		}
		ShowMain(&s_u32MainUpdateMethod);
		#if (HEART == 1)		//心跳帧			
		{			
			Heart.ActionType = SIGNAL_ACTION;
			Heart.SendMode = 0;
			LogicRunInfApi(LOGIC_SET_HEART, &Heart);
		}	
		#endif	
	}
	return 1;
}

//成组键处理		//zzw
u8 InterfaceKeyGrpCtrlPro(stKEY sNewKey,u32 LogicRunStatus,INTERFACE_CONTROL_MODE_TYPE stNewMode,void *pDate)
{
	u8 CtrlMode;
	u16 u16GrpRemoSc,u16GrpNearSc,u16AddDirct,GrpDirct;
	u32 u32Y;
	
	if(((sNewKey.u32Key == KEY_LEFT_GRP)|| (sNewKey.u32Key == KEY_RIGHT_GRP)) && ((sNewKey.u32Status == KEY_DOWN)))
	{
		if((stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)
			||(stNewMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL))		//本架/邻架控制按成组键读取ID1的动作信息
		{
			//读动作信息
			if(GetActionInfoByID(1,INTERFACE_MODE_SC_GRP_CONTROL))
			{
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);
			}
			else
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);	
		}
		CtrlMode = AUTO_CTRL;
		LogicParamApi(LOGIC_SET_CTRL_MODE,&CtrlMode);
		InterfaceSetCtrlMode(INTERFACE_MODE_SC_GRP_CONTROL);
		u32Y = LOGIC_STATUS_RFID_SC_GRP;
		LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);	
		
		LogicRunInfApi(LOGIC_GET_GRP_REMOTE_SC_NUMB,&u16GrpRemoSc);//远端架号
		LogicRunInfApi(LOGIC_GET_GRP_NEAR_SC_NUMB,&u16GrpNearSc);//近端架号
		LogicRunInfApi(LOGIC_GET_SC_NUMB_ADD_DIRCT,&u16AddDirct);//架号增向			
		if(sNewKey.u32Key == KEY_RIGHT_GRP)//右成组
		{
			if(u16AddDirct == SC_DIRCT_LEFT)
			{
				if((LogicRunStatus == LOGIC_STATUS_RFID_SC_SELF_CONTROL)
					||(LogicRunStatus == LOGIC_STATUS_RFID_SC_NEIGHBORL))
				{
					//本架控制时，需要设置默认范围
					LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);
					u16GrpNearSc = u32Y > GRP_CTRL_MIN_SPACE?u32Y - GRP_CTRL_MIN_SPACE:1;
					u16GrpRemoSc = u32Y > GRP_CTRL_INIT_ARE?u32Y - GRP_CTRL_INIT_ARE:1;
					if((u16GrpNearSc==1)&&(u16GrpRemoSc == 1))
					{
						u16GrpNearSc = 0;
						u16GrpRemoSc = 0;
					}
					GrpDirct = SC_GRP_ACTION_NEAR_REMOTE;
					LogicRunInfApi(LOGIC_SET_GRP_REMOTE_SC_NUMB,&u16GrpRemoSc);//远端架号
					LogicRunInfApi(LOGIC_SET_GRP_NEAR_SC_NUMB,&u16GrpNearSc);//近端架号
					LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION, &GrpDirct);
				}
				else
				{
					u16GrpRemoSc--;
					if((u16GrpRemoSc < u16GrpNearSc?(u16GrpNearSc-u16GrpRemoSc >= GRP_CTRL_MIN_SPACE?TRUE:FALSE)\
						:(u16GrpRemoSc-u16GrpNearSc >= GRP_CTRL_MIN_SPACE?TRUE:FALSE))
						&&(u16GrpRemoSc > 0 && u16GrpRemoSc < 251))
					{
						LogicRunInfApi(LOGIC_SET_GRP_REMOTE_SC_NUMB, &u16GrpRemoSc);						
					}
					else
					{
						InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
						InterfaceSetCtrlMode(INTERFACE_MODE_SC_GRP_CONTROL);
					}				
				}	
			}
			else
			{
				if((LogicRunStatus == LOGIC_STATUS_RFID_SC_SELF_CONTROL)
					||(LogicRunStatus == LOGIC_STATUS_RFID_SC_NEIGHBORL))
				{
					//本架控制时，需要设置默认范围
					LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);
					u16GrpNearSc = u32Y + GRP_CTRL_MIN_SPACE>250?250:u32Y + GRP_CTRL_MIN_SPACE;
					u16GrpRemoSc = u32Y + GRP_CTRL_INIT_ARE>250?250:u32Y + GRP_CTRL_INIT_ARE;
					if((u16GrpNearSc==250)&&(u16GrpRemoSc == 250))
					{
						u16GrpNearSc = 0;
						u16GrpRemoSc = 0;
					}
					GrpDirct = SC_GRP_ACTION_NEAR_REMOTE;
					LogicRunInfApi(LOGIC_SET_GRP_REMOTE_SC_NUMB,&u16GrpRemoSc);//远端架号
					LogicRunInfApi(LOGIC_SET_GRP_NEAR_SC_NUMB,&u16GrpNearSc);//近端架号
					LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION, &GrpDirct);
				}
				else
				{
					u16GrpRemoSc++;
					if((u16GrpRemoSc < u16GrpNearSc?(u16GrpNearSc-u16GrpRemoSc >= GRP_CTRL_MIN_SPACE?TRUE:FALSE)\
						:(u16GrpRemoSc-u16GrpNearSc >= GRP_CTRL_MIN_SPACE?TRUE:FALSE))
						&&(u16GrpRemoSc > 0 && u16GrpRemoSc < 251))
					{
						LogicRunInfApi(LOGIC_SET_GRP_REMOTE_SC_NUMB, &u16GrpRemoSc);
					}
					else
					{
						InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
						InterfaceSetCtrlMode(INTERFACE_MODE_SC_GRP_CONTROL);	
					}				
				}
			}					
		}
		else//方向右键
		{
			if(u16AddDirct == SC_DIRCT_LEFT)
			{
				if((LogicRunStatus == LOGIC_STATUS_RFID_SC_SELF_CONTROL)
					||(LogicRunStatus == LOGIC_STATUS_RFID_SC_NEIGHBORL))
				{
					//本架控制时，需要设置默认范围
					LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);
					u16GrpNearSc = u32Y + GRP_CTRL_MIN_SPACE>250?250:u32Y + GRP_CTRL_MIN_SPACE;
					u16GrpRemoSc = u32Y + GRP_CTRL_INIT_ARE>250?250:u32Y + GRP_CTRL_INIT_ARE;
					if((u16GrpNearSc==250)&&(u16GrpRemoSc == 250))
					{
						u16GrpNearSc = 0;
						u16GrpRemoSc = 0;
					}
					GrpDirct = SC_GRP_ACTION_NEAR_REMOTE;
					LogicRunInfApi(LOGIC_SET_GRP_REMOTE_SC_NUMB,&u16GrpRemoSc);//远端架号
					LogicRunInfApi(LOGIC_SET_GRP_NEAR_SC_NUMB,&u16GrpNearSc);//近端架号	
					LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION, &GrpDirct);
				}
				else
				{
					u16GrpRemoSc++;
					if((u16GrpRemoSc < u16GrpNearSc?(u16GrpNearSc-u16GrpRemoSc >= GRP_CTRL_MIN_SPACE?TRUE:FALSE)\
						:(u16GrpRemoSc-u16GrpNearSc >= GRP_CTRL_MIN_SPACE?TRUE:FALSE))
						&&(u16GrpRemoSc > 0 && u16GrpRemoSc < 251))
					{
						LogicRunInfApi(LOGIC_SET_GRP_REMOTE_SC_NUMB, &u16GrpRemoSc);						
					}
					else
					{
						InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
						InterfaceSetCtrlMode(INTERFACE_MODE_SC_GRP_CONTROL);
					}				
				}
			}
			else
			{
				if((LogicRunStatus == LOGIC_STATUS_RFID_SC_SELF_CONTROL)
					||(LogicRunStatus == LOGIC_STATUS_RFID_SC_NEIGHBORL))
				{
					//本架控制时，需要设置默认范围
					LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);
					u16GrpNearSc = u32Y > GRP_CTRL_MIN_SPACE?u32Y - GRP_CTRL_MIN_SPACE:1;
					u16GrpRemoSc = u32Y > GRP_CTRL_INIT_ARE?u32Y - GRP_CTRL_INIT_ARE:1;
					if((u16GrpNearSc==1)&&(u16GrpRemoSc == 1))
					{
						u16GrpNearSc = 0;
						u16GrpRemoSc = 0;
					}
					GrpDirct = SC_GRP_ACTION_NEAR_REMOTE;
					LogicRunInfApi(LOGIC_SET_GRP_REMOTE_SC_NUMB,&u16GrpRemoSc);//远端架号
					LogicRunInfApi(LOGIC_SET_GRP_NEAR_SC_NUMB,&u16GrpNearSc);//近端架号	
					LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION, &GrpDirct);
				}
				else
				{
					u16GrpRemoSc--;
					if((u16GrpRemoSc < u16GrpNearSc?(u16GrpNearSc-u16GrpRemoSc >= GRP_CTRL_MIN_SPACE?TRUE:FALSE)\
						:(u16GrpRemoSc-u16GrpNearSc >= GRP_CTRL_MIN_SPACE?TRUE:FALSE))
						&&(u16GrpRemoSc > 0 && u16GrpRemoSc < 251))
					{
						LogicRunInfApi(LOGIC_SET_GRP_REMOTE_SC_NUMB, &u16GrpRemoSc);
					}
					else
					{
						InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
						InterfaceSetCtrlMode(INTERFACE_MODE_SC_GRP_CONTROL);
					}
				}
			}				
		}
		InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);				
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_GRP_CONTROL);//显示主屏所有信息
		InterfaceSetCtrlMode(INTERFACE_MODE_SC_GRP_CONTROL);	
	}
	if(((sNewKey.u32Key == KEY_LEFT_GRP)|| (sNewKey.u32Key == KEY_RIGHT_GRP)) && ((sNewKey.u32Status == KEY_UP)))
	{
	
	}
	ShowMain(&s_u32MainUpdateMethod);	
	return 1;
}

//停止键处理		//zzw
u8 InterfaceKeyShuntDownPro(u32 LogicRunStatus,void *pDate)
{
	u32 u32Y;
	u16 u16SendTimes,u16DelayTime,u16I;
	u32 u32ReturnFlag,status;
	COMMAND_ACTION ActionTemp = {0};
	u8 err,u8J;
	
	if((LogicRunStatus != LOGIC_STATUS_RFID_SC_SELF_CONTROL)			//其他界面按停止回到本架控制界面
		|| (ActionInfoMsg[0].ActionType == COMBINATION_ACTION))
	{
//		if(LogicRunStatus == LOGIC_STATUS_RFID_SC_NEIGHBORL)
		{
					//清除心跳
			if(RfidGetDataLimitlessSendFlag())
			{
				RfidSetDataLimitlessSendEnd();
			}
			//下发邻架控制请求
			g_stLogicSem->OSEventCnt = 0;
			LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16I);	
			LogicRunInfApi(LOGIC_SET_NEAR_CTRL_REQ, &u16I);
			//刷新等待时间
			LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
			LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
			u32Y = (u32)(u16DelayTime*u16SendTimes);	
			u32Y += 30;

			OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
			if(err == OS_ERR_TIMEOUT)//无应答
			{
				InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
				ShowMain(&s_u32MainUpdateMethod);	
			}
			else if(err == OS_ERR_NONE)//有应答
			{
				/*为了修改控制的架号**/
				u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
				LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);					
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);

			}
			else
				;		
			#if (HEART == 1)	
				//心跳
				Heart.ActionType = SIGNAL_ACTION;
				Heart.SendMode = 0;
				Heart.ActionValue = 0;
				LogicRunInfApi(LOGIC_SET_HEART, &Heart);
			#endif			
		}
//		if(LogicRunStatus == LOGIC_STATUS_RFID_SC_GRP)
		{
			u8J = MANUAL_CTRL;
			LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);
		}
		s_u32ShiftKeyEnableFlag = DISABLE;
		ClearUpdateMainMethod();
		InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
		InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);					
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息				
		u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
		LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
		u32Y = 0x00;
		LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);	
		LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32Y);	
		ShowMain(&s_u32MainUpdateMethod);	
		ClearActionInfo();	
		
		ShowScUncertifiedKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
	}
	else		//本架控制界面按停止需下发停止命令
	{
		//zzw	清除心跳
		if(RfidGetDataLimitlessSendFlag())
		{
			RfidSetDataLimitlessSendEnd();
		}
		/**键值显示**/
		g_stLogicSem->OSEventCnt = 0;
		u32Y = SC_BUTTON_ACK_NONE;
		LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//将应答设为无效
		//停止命令下发
		ActionTemp.ActionType.Type.ActionType = 4;
		ActionTemp.OrderActionOrResult = ACTION_STOP;
		ActionTemp.ActionID = 0;
		LogicRunInfApi(LOGIC_SET_BUTTON_VALUE, &ActionTemp);
		//更新显示界面
		InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
		InterfaceInsertNote(INTERFACE_NOTE_BUTTON_DOWN);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);			
		ShowMain(&s_u32MainUpdateMethod);	
		LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
		LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
		u32Y = (u32)(u16DelayTime*u16SendTimes);	
		u32Y += 30;			
		
		OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);			
		if(err == OS_ERR_TIMEOUT)//无应答
		{
			InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL);//显示主屏所有信息	
			ShowMain(&s_u32MainUpdateMethod);						
		}
		else if(err == OS_ERR_NONE)//有应答
		{
			/**关于设置的一些特殊模式切换的处理*/
			status = 0;
			LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
			LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&ActionTemp);
			if((status == SC_BUTTON_ACK_ACCEPT)&& (ActionTemp.OrderActionOrResult == 1))		//按键接收且无异常
			{
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
				u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
				LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
				u32Y = 0x00;
				LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);	
				LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32Y);	
				for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示							
				ShowMain(&s_u32MainUpdateMethod);						
			}
			else if(status == SC_BUTTON_ACK_REJECT)
			{
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
				u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
				LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
				for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示						
				ShowMain(&s_u32MainUpdateMethod);							
			}				
			else
			{
				InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息					
				u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
				LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
				for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示						
				ShowMain(&s_u32MainUpdateMethod);			
			}
		}
		else
			;	
		#if (HEART == 1)	
		//心跳
		Heart.ActionType = SIGNAL_ACTION;
		Heart.SendMode = 0;
		Heart.ActionValue = 0;
		LogicRunInfApi(LOGIC_SET_HEART, &Heart);
		#endif
	}
	return 1;
}

//动作键按下处理	zzw
u64 HeartTemp = 0x01;
u8 InterfaceActionKeyDownPro(stKEY sNewKey,u32 LogicRunStatus,INTERFACE_CONTROL_MODE_TYPE stNewMode,void *pDate)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
		OS_CPU_SR  cpu_sr = 0;
	#endif
	u8 i,j,err,CtrlMode,key[INTERFACE_KEY_TOGETHER_MAX] = {0};
	u16 u16SendTimes,u16DelayTime;
	u32 u32ReturnFlag,u32Y,status;
	COMMAND_ACTION ActionTemp;
	ActionMess ActionOldTemp;
//	u64 u64TempValue;
//	u64TempValue = 0x01;	
	
	LogicParamApi(LOGIC_GET_CTRL_MODE,&CtrlMode);
	//从配置文件中读数
	i = ActionKeyValueTurnAround(sNewKey.u32Key,s_u32ShiftKeyEnableFlag,key);
//	InterfaceInsertNote(INTERFACE_NOTE_BUTTON_DOWN);
//	InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
//	ShowMain(&s_u32MainUpdateMethod);
	if(stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
	{
		ActionOldTemp = ActionInfoMsg[0];
		ClearActionInfo();	
	}
	for(j = 0;j < i;j++)
	{
		GetActionIDByKey(stNewMode,key[j]);
	}
	//未找到动作或控制方式不允许，提示
	if(RuningActionCnt == 0)
	{
		ActionInfoMsg[0] = ActionOldTemp;
		InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);		
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
		ShowMain(&s_u32MainUpdateMethod);					
	}
	else
	{
		if((stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
			|| (ActionInfoMsg[0].ActionType == COMBINATION_ACTION))
		{
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_DOWN);
			if(stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_GRP_CONTROL);//显示主屏所有信息
			else if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL);//显示主屏所有信息
			else if(stNewMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_NEAR_CONTROL);
//			InterfaceSetCtrlMode(stNewMode);
			ShowMain(&s_u32MainUpdateMethod);				
			LogicRunInfApi(LOGIC_SET_RUN_STATUS,&LogicRunStatus);		
			u32ReturnFlag = 0x01;

			return u32ReturnFlag;
		}
		else
		{
			//zzw	清除心跳
			if(RfidGetDataLimitlessSendFlag())
			{
				RfidSetDataLimitlessSendEnd();
			}
			for(i = 0;i < RuningActionCnt;i++)
			{
				//更新主界面
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_DOWN);
				if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL);//显示主屏所有信息
				else if(stNewMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_NEAR_CONTROL);//显示主屏所有信息
				else if(stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_GRP_CONTROL);//显示主屏所有信息
				else
					;
				ShowMain(&s_u32MainUpdateMethod);
				//之前就有的动作可以不发，但是这样会增加遥控器按下了，但控制器不执行的风险。
//				if(((Heart.ActionValue >> (ActionInfoMsg[i].ActionID - 1))&0x0001) == 0x0001)
//				{
//					ActionTemp.ActionType.Type.ActionType = ActionInfoMsg[i].ActionType;
//					
//					if(CtrlMode == MANUAL_CTRL)
//						ActionTemp.ActionType.Type.CtrlMode = MANUAL_CTRL;
//					else
//						ActionTemp.ActionType.Type.CtrlMode = AUTO_CTRL;
//					continue;
//				}
				OS_ENTER_CRITICAL();
				if(ActionInfoMsg[i].ActionID < 64)
					Heart.ActionValue |= ((u64)(HeartTemp << (ActionInfoMsg[i].ActionID - 1)));
					
				g_stLogicSem->OSEventCnt = 0;
				u32Y = SC_BUTTON_ACK_NONE;
				LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//将应答状态设为无效	
				ActionTemp.ActionType.type = 0;
				ActionTemp.OrderActionOrResult = 0;
				ActionTemp.ActionID = 0;
				LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&ActionTemp);//将应答动作字设为无效	
				
//				//命令下发
				ActionTemp.ActionID = ActionInfoMsg[i].ActionID;
				ActionTemp.ActionType.Type.ActionType = ActionInfoMsg[i].ActionType;
				
				if(CtrlMode == MANUAL_CTRL)
					ActionTemp.ActionType.Type.CtrlMode = MANUAL_CTRL;
				else
					ActionTemp.ActionType.Type.CtrlMode = AUTO_CTRL;
				ActionTemp.OrderActionOrResult = ACTION_INNEDIATELY_START;
				ActionTemp.ActionID = ActionInfoMsg[i].ActionID;
				LogicRunInfApi(LOGIC_SET_BUTTON_VALUE, &ActionTemp);
				OS_EXIT_CRITICAL();
				//刷新等待时间
				LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
				LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
				u32Y = (u32)(u16DelayTime*u16SendTimes);	
				u32Y += 50;
				OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
				if(err == OS_ERR_TIMEOUT)//无应答
				{
					#if 1		//test
					InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
//					InterfaceSetCtrlMode(stNewMode);
					ShowMain(&s_u32MainUpdateMethod);	
					ShowScUncertifiedKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);	
					#else
					//zzw
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
					if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//显示主屏所有信息
					else if(stNewMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_NEAR_CONTROL);//显示主屏所有信息
					else if(stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_GRP_CONTROL);//显示主屏所有信息
					else
						;
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
					ShowMain(&s_u32MainUpdateMethod);				
					u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
					#endif
				}
				else if(err == OS_ERR_NONE)//有应答
				{
					/**关于设置的一些特殊模式切换的处理*/
					status = 0;
					LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
					LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&ActionTemp);
					if((status == SC_BUTTON_ACK_ACCEPT)&& (ActionTemp.OrderActionOrResult == 1))		//按键接收且无异常
					{
						InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
						if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)	//显示主屏所有信息
							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL);
						else if(stNewMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_NEAR_CONTROL);
						else if(stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_GRP_CONTROL);
						else
							;
//						InterfaceSetCtrlMode(stNewMode);
						ShowMain(&s_u32MainUpdateMethod);				
						LogicRunInfApi(LOGIC_SET_RUN_STATUS,&LogicRunStatus);
					}
					else if(status == SC_BUTTON_ACK_REJECT)
					{
						InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
						if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)	//显示主屏所有信息
							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL);
						else if(stNewMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_NEAR_CONTROL);
						else if(stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_GRP_CONTROL);
						else
							;
//						InterfaceSetCtrlMode(stNewMode);
						ShowMain(&s_u32MainUpdateMethod);									
						LogicRunInfApi(LOGIC_SET_RUN_STATUS,&LogicRunStatus);
					}
					else
					{
						InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
						if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)	//显示主屏所有信息
							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL);
						else if(stNewMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_NEAR_CONTROL);
						else if(stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_GRP_CONTROL);
						else
							;
//						InterfaceSetCtrlMode(stNewMode);	
						ShowMain(&s_u32MainUpdateMethod);	
						LogicRunInfApi(LOGIC_SET_RUN_STATUS,&LogicRunStatus);				
					}

				}
				else
					;
				
				u32ReturnFlag = 0x01;			
			}
					
			ShowScUncertifiedKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);
		}	
		#if (HEART == 1)
			//心跳帧，不停心跳别的数据发不出去
			if((ActionTemp.ActionType.Type.ActionType == SIGNAL_ACTION)
				&&(ActionTemp.ActionType.Type.CtrlMode == MANUAL_CTRL))
			{			
				Heart.ActionType = SIGNAL_ACTION;
				Heart.SendMode = 1;
			}
			else
			{
				Heart.ActionType = SIGNAL_ACTION;
				Heart.SendMode = 0;
				Heart.ActionValue = 0;				
			}
			LogicRunInfApi(LOGIC_SET_HEART, &Heart);
		#endif		
	}
	u32ReturnFlag = 0x01;

	return u32ReturnFlag;
}
//动作键抬起处理	zzw
u8 InterfaceActionKeyUpPro(stKEY sNewKey,u32 LogicRunStatus,INTERFACE_CONTROL_MODE_TYPE stNewMode,void *pDate)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
		OS_CPU_SR  cpu_sr = 0;
	#endif
	u8 i,j,err = 0,CtrlMode,key[INTERFACE_KEY_TOGETHER_MAX] = {0};
	u16 u16SendTimes,u16DelayTime;
	u32 u32ReturnFlag,u32Y,status;
	COMMAND_ACTION ActionTemp;
//	u64 u64TempValue;
//	u64TempValue = 0;
	LogicParamApi(LOGIC_GET_CTRL_MODE,&CtrlMode);
	//从配置文件中读数
	i = ActionKeyValueTurnAround(sNewKey.u32Key,s_u32ShiftKeyEnableFlag,key);
//	InterfaceInsertNote(INTERFACE_NOTE_BUTTON_UP);
//	InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
//	ShowMain(&s_u32MainUpdateMethod);
	for(j = 0;j < i;j++)
	{
		GetActionIDByKey(stNewMode,key[j]);
	}
	//未找到动作或控制方式不允许，提示
	if(RuningActionCnt == 0)
	{
		InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);		
		if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL);//显示主屏所有信息
		else if(stNewMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_NEAR_CONTROL);//显示主屏所有信息
		else if(stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_GRP_CONTROL);//显示主屏所有信息
		else
			;		
//		InterfaceSetCtrlMode(stNewMode);
		ShowMain(&s_u32MainUpdateMethod);					
	}
	else
	{
		if((stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
			|| (ActionInfoMsg[0].ActionType == COMBINATION_ACTION))
		{
			InterfaceInsertNote(INTERFACE_NOTE_START_ACTION);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
			ShowMain(&s_u32MainUpdateMethod);
			u32ReturnFlag = 0x01;	
			
			return u32ReturnFlag;		
		}		
		if(CtrlMode == AUTO_CTRL)//当控制模式为自动时，按键抬起清除数据		zzw
		{
			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
				BuzzerOff();
			#endif
			ClearActionInfo();	
			InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);
			if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)	//显示主屏所有信息
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL);
			else if(stNewMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_NEAR_CONTROL);
			else if(stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_GRP_CONTROL);
			else
				;
//			InterfaceSetCtrlMode(stNewMode);
			ShowMain(&s_u32MainUpdateMethod);	
			ShowScUncertifiedKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);
		}
		else	//控制模式为手动时，抬起下发动作停止
		{
			if(RfidGetDataLimitlessSendFlag())				//zzw		清除心跳
			{
				RfidSetDataLimitlessSendEnd();
			}
			for(i = 0;i < RuningActionCnt;i++)
			{
				OS_ENTER_CRITICAL();	
				if(ActionInfoMsg[i].ActionID < 64)
					Heart.ActionValue &= (~(HeartTemp << (ActionInfoMsg[i].ActionID-1)));
				
				g_stLogicSem->OSEventCnt = 0;
				u32Y = SC_BUTTON_ACK_NONE;
				LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//将应答状态设为无效	
				ActionTemp.ActionType.type = 0;
				ActionTemp.OrderActionOrResult = 0;
				ActionTemp.ActionID = 0;
				LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&ActionTemp);//将应答动作字设为无效	
				//命令下发
				ActionTemp.ActionID = ActionInfoMsg[i].ActionID;
				ActionTemp.ActionType.Type.ActionType = ActionInfoMsg[i].ActionType;
				
				if(CtrlMode == MANUAL_CTRL)
					ActionTemp.ActionType.Type.CtrlMode = MANUAL_CTRL;
				else
					ActionTemp.ActionType.Type.CtrlMode = AUTO_CTRL;
				ActionTemp.OrderActionOrResult = ACTION_STOP;
				LogicRunInfApi(LOGIC_SET_BUTTON_VALUE, &ActionTemp);
				OS_EXIT_CRITICAL();

				if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)	//显示主屏所有信息
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL);
				else if(stNewMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_NEAR_CONTROL);
				else if(stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_GRP_CONTROL);
				else
					;
//				InterfaceSetCtrlMode(stNewMode);
				ShowMain(&s_u32MainUpdateMethod);	
				ShowScUncertifiedKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);				
				
				u32Y = 560+30;//此时间与发送的时间有关jhyjhy
				OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
				if(err == OS_ERR_TIMEOUT)//无应答
				{
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_UP);
					if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//显示主屏所有信息
					else if(stNewMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_NEAR_CONTROL);//显示主屏所有信息
					else if(stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_GRP_CONTROL);//显示主屏所有信息
					else
						;		
//					InterfaceSetCtrlMode(stNewMode);
				
					ShowMain(&s_u32MainUpdateMethod);	
				}
				else if(err == OS_ERR_NONE)//有应答
				{
					/**关于设置的一些特殊模式切换的处理*/
					status = 0;
					LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
					LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&ActionTemp);
					if((status == SC_BUTTON_ACK_ACCEPT)&& (ActionTemp.OrderActionOrResult == 0x01))		//按键接收且无异常
					{
						InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
						if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)	//显示主屏所有信息
							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);
						else if(stNewMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_NEAR_CONTROL);
						else if(stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_GRP_CONTROL);
						else
							;				
//						InterfaceSetCtrlMode(stNewMode);
						LogicRunInfApi(LOGIC_SET_RUN_STATUS, &LogicRunStatus);
						
						ShowMain(&s_u32MainUpdateMethod);	
					}
					else if(status == SC_BUTTON_ACK_REJECT)
					{
						InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
//						InterfaceSetCtrlMode(stNewMode);
						LogicRunInfApi(LOGIC_SET_RUN_STATUS,&LogicRunStatus);				
						ShowMain(&s_u32MainUpdateMethod);	
					}
					else
					{
						InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);

//						InterfaceSetCtrlMode(stNewMode);
						LogicRunInfApi(LOGIC_SET_RUN_STATUS,&LogicRunStatus);
						ShowMain(&s_u32MainUpdateMethod);	
					}						
				}
				else
					;
			}	
			#if (HEART == 1)		//心跳帧			
			{			
				Heart.ActionType = SIGNAL_ACTION;
				Heart.SendMode = 0;
				LogicRunInfApi(LOGIC_SET_HEART, &Heart);
			}	
			#endif	
		}	
			
		ClearActionInfo();	
		if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)	//显示主屏所有信息
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL);
		else if(stNewMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_NEAR_CONTROL);
		else if(stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_GRP_CONTROL);
		else
			;
		ShowMain(&s_u32MainUpdateMethod);			
		ShowScSelfKeyLedProc(KEY_INVALID,KEY_UP,s_u32ShiftKeyEnableFlag);				
	}
	u32ReturnFlag = 0x01;	
	
	return u32ReturnFlag;
}


//解除对码	zzw
u8 InterfaceShiftKeyLongDownPro(void)
{
	u8 u8J;
	u32 u32Y;
	u16 u16DelayTime;
	
	//zzw	清除心跳
	if(RfidGetDataLimitlessSendFlag())
	{
		RfidSetDataLimitlessSendEnd();
	}	
	u8J = NO_CTRL;
	LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);
	s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
	InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
	StateLed(0x00,KEY_SHIFT_LED);
	StateLed(0x00,ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED);
	KeyLedOnOff(0x00,KEY_LED_ALL);
	u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;
	LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
	u16DelayTime = RFID_SIGNAL_TOO_SMALL;
	LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DelayTime);		
	InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_UNCOLIBRATE);
	InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);	
	ShowMain(&s_u32MainUpdateMethod);
	OSTimeDly(200/TICK_TIME);		
	InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);		
	ShowMain(&s_u32MainUpdateMethod);
	KeyLedOnOff(0x00, KEY_LED_ALL);
//	if(RfidGetDataLimitlessSendFlag())
//	{
//		RfidSetDataLimitlessSendEnd();
//	}
	s_u32ShiftKeyEnableFlag = DISABLE;
	
	return 1;
}

//长按停止键	zzw
u8 InterfaceStopKeyLongDownPro(void)
{
	u32 u32Y;
	#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
		BuzzerOn(BEEP_SHUTDOWN_NOTE);
	#endif
	//先解除对码
	InterfaceShiftKeyLongDownPro();
	OSTimeDly(200/TICK_TIME);			//不加延时数据发不出去		zzw
	OSTaskSuspend(SCAN_KEY_TASK_PRIO);//挂起按键扫描任务
	s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
	InterfaceInsertNote(INTERFACE_NOTE_SHUTDOWN);
	InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
	ShowMain(&s_u32MainUpdateMethod);
	#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
		KeyLedOnOff(0x00,KEY_LED_ALL);
	#endif				
	u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;
	LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);				
	OSTimeDly(200/TICK_TIME);
	
	u32Y = LOGIC_STATUS_SHUTUP;
	LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);			
	OSTimeDly(1000/TICK_TIME);
	
	return 1;
}

//成组启动按键处理
u8 InterfaceGrpActionStartPro(void)
{
	u8 u8J,CtrlMode,err,status;
	u16 u16SendTimes = 0,u16DelayTime = 0,LogicRunStatus;
	u32 u32Y = 0,u32ReturnFlag = 0x01;	
	COMMAND_ACTION ActionTemp;
	
	if(ActionInfoMsg[0].ActionID == 0)	//无动作时按下启动键
	{
		InterfaceInsertNote(INTERFACE_NOTE_PRESSED_BUTTON_INVAILD);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_MAIN_AWAKE);
		ShowMain(&s_u32MainUpdateMethod);
		
		return u32ReturnFlag;
	}
	//zzw	清除心跳
	if(RfidGetDataLimitlessSendFlag())
	{
		RfidSetDataLimitlessSendEnd();
	}	
	LogicParamApi(LOGIC_GET_CTRL_MODE,&CtrlMode);
	//命令下发
	ActionTemp.ActionID = ActionInfoMsg[0].ActionID;
	ActionTemp.ActionType.Type.ActionType = ActionInfoMsg[0].ActionType;
	if(CtrlMode == MANUAL_CTRL)
		ActionTemp.ActionType.Type.CtrlMode = MANUAL_CTRL;
	else
		ActionTemp.ActionType.Type.CtrlMode = AUTO_CTRL;
	ActionTemp.OrderActionOrResult = ACTION_INNEDIATELY_START;

	LogicRunInfApi(LOGIC_SET_BUTTON_VALUE, &ActionTemp);
				
	InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);								
	ClearUpdateMainMethod();	
	InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL|INTERFACE_SHOW_MAIN_AWAKE);//显示主屏所有信息

	//回本架控制并把控制方式切换为手动
	InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
	u8J = MANUAL_CTRL;
	LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);
	u8J  = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
	LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u8J);	
	s_u32ShiftKeyEnableFlag = DISABLED;	
	StateLed(0x00,KEY_SHIFT_LED);

	ShowMain(&s_u32MainUpdateMethod);
	//刷新等待时间
	LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
	LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
	u32Y = (u32)(u16DelayTime*u16SendTimes);	
	u32Y += 30;
	LogicRunStatus = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
	OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
	if(err == OS_ERR_TIMEOUT)//无应答
	{
		InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
		InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
		ShowMain(&s_u32MainUpdateMethod);	
		LogicRunInfApi(LOGIC_SET_RUN_STATUS,&LogicRunStatus);
	}
	else if(err == OS_ERR_NONE)//有应答
	{
		/**关于设置的一些特殊模式切换的处理*/
		status = 0;
		LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
		LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&ActionTemp);
		if((status == SC_BUTTON_ACK_ACCEPT)&& (ActionTemp.OrderActionOrResult == 1))		//按键接收且无异常
		{
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);
			InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
			ShowMain(&s_u32MainUpdateMethod);				
			LogicRunInfApi(LOGIC_SET_RUN_STATUS,&LogicRunStatus);
		}
		else if(status == SC_BUTTON_ACK_REJECT)
		{
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);
			InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
			ShowMain(&s_u32MainUpdateMethod);									
			LogicRunInfApi(LOGIC_SET_RUN_STATUS,&LogicRunStatus);
		}
		else
		{
			InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);
			InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);	
			ShowMain(&s_u32MainUpdateMethod);	
			LogicRunInfApi(LOGIC_SET_RUN_STATUS,&LogicRunStatus);				
		}
	}
	else
		;
	u32ReturnFlag = 0x01;			
	//清除动作数据
	ClearActionInfo();
	{
		u16 u16I;
				//清除心跳
		if(RfidGetDataLimitlessSendFlag())
		{
			RfidSetDataLimitlessSendEnd();
		}
		//下发邻架控制请求
		g_stLogicSem->OSEventCnt = 0;
		LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16I);	
		LogicRunInfApi(LOGIC_SET_NEAR_CTRL_REQ, &u16I);
		//刷新等待时间
		LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
		LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
		u32Y = (u32)(u16DelayTime*u16SendTimes);	
		u32Y += 30;

		OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
		if(err == OS_ERR_TIMEOUT)//无应答
		{
			InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
			ShowMain(&s_u32MainUpdateMethod);	
		}
		else if(err == OS_ERR_NONE)//有应答
		{
			/*为了修改控制的架号**/
			u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);					
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);

		}
		s_u32ShiftKeyEnableFlag = DISABLE;
		ClearUpdateMainMethod();
		InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
		InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);					
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息				
		u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
		LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
		u32Y = 0x00;
		LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);	
		LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32Y);	
		ShowMain(&s_u32MainUpdateMethod);			
	}	
	//心跳
	Heart.ActionType = SIGNAL_ACTION;
	Heart.SendMode = 0;
	Heart.ActionValue = 0;
	LogicRunInfApi(LOGIC_SET_HEART, &Heart);	
	return u32ReturnFlag;
}

/*******************************************************************************************
**函数名称：InterfaceModeScUncertifiedProc
**函数作用：显示层处于本架非动作模式下,按键的处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
	static u32 InterfaceModeScUncertifiedProc(stKEY	sNewKey)
	{
		u32 u32ReturnFlag = 0x00;
		INT8U err;
		static u64 u64ButtonValue;
		static u64 u64ButtonAckValue;
		static u64 u64stRfidType;
		SC_BUTTON_TYPE status;
		u32 u32Y = 0x00;
		u32 u32I = 0x00;
		u32 u32K = 0x00;
		u8 u8tmp = 0;
		static u32 u32StopKeyFlag = KEY_UP;//
		static u64 u64RfidData;
		static u64 u64Y;
		RFID_FRAME_TYPE stRfidType;
		u16 u16DelayTime = 0x00;
		u16 u16SendTimes = 0x00;
		u16 u16ScNumbOldBackup;
		u16 u16ScNumbNewBackup;
		u16 u16temptest;
		u8 u8J = 0;
		u32 u32ActionType;
		
		u64Y = 0x00;
		u64RfidData = RFID_ACTION_NONE;
		s_u32AllKeyStatus = sNewKey.u32Status;
		
		LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
		s_u32InterfaceColibrateTimer = u32Y;		
		InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_AWAKE);
		LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);	
		LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32Y);	
//		memset(ActionInfoMsg,0,sizeof(ActionInfoMsg));		//zzw
		
		if(sNewKey.u32Status == KEY_DOWN)
			s_u8pleaseNearFlag = 0;
		
		if((sNewKey.u32Key&KEY_STOP_SHUTDOWN) == KEY_STOP_SHUTDOWN)
		{	
			sNewKey.u32Key = KEY_STOP_SHUTDOWN;
			s_u32ShiftKeyEnableFlag = DISABLED;
			StateLed(0x00, KEY_SHIFT_LED);
			ShowScUncertifiedKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);
		}
		//shift键短按
		if((sNewKey.u32Key == KEY_SHIFT)&& (sNewKey.u32Status == KEY_DOWN))		
		{
			if(s_u32ShiftKeyEnableFlag == DISABLED)
			{
				StateLed(KEY_SHIFT_LED,0x00);
				u32ReturnFlag = 0x01;
				s_u32ShiftKeyEnableFlag = ENABLED;
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_SHIFT_DOWN);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
				OSSemPost(g_stSemInterface);			
			}
			else
			{
				StateLed(0x00,KEY_SHIFT_LED);
				u32ReturnFlag = 0x01;
				s_u32ShiftKeyEnableFlag = DISABLED;	
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_SHIFT_UP);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
				OSSemPost(g_stSemInterface);			
			}
			ShowScUncertifiedKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);
		}	
		else if((sNewKey.u32Key == KEY_SHIFT)&&(sNewKey.u32Status == KEY_UP))
		{

		}	

		/**识别解除对码键(长按SHIFT，解除对码)**/
		else if((sNewKey.u32Key == KEY_SHIFT)&&(sNewKey.u32Status == KEY_LONGDOWN))				
		{
			InterfaceShiftKeyLongDownPro();
			u32ReturnFlag = 0x01;		
		}	
		else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_LONGDOWN))
		{
			u32StopKeyFlag = KEY_LONGDOWN;
			InterfaceStopKeyLongDownPro();
		}
		else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_DOWN))
		{
			u32StopKeyFlag = KEY_DOWN;
		}	
		else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_UP))//对停止键做特殊处理
		{
			if(u32StopKeyFlag == KEY_LONGDOWN)/**在任何模式下，均识别关机键*/
			{
				return u32ReturnFlag;			
		
			}
			else if(u32StopKeyFlag == KEY_DOWN)//停止键
			{
				InterfaceKeyShuntDownPro(LOGIC_STATUS_RFID_SC_SELF_CONTROL,NULL);
			
				u32ReturnFlag = 0x01;
					
			}
			u32StopKeyFlag = KEY_UP;
			ShowScUncertifiedKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);
		}
		//对确认键做特殊处理	zzw
		else if((sNewKey.u32Key == KEY_CERTIFY)&& (sNewKey.u32Status == KEY_DOWN || sNewKey.u32Status == KEY_UP))		
		{
			u32ReturnFlag = InterfaceKeyCertifyPro(sNewKey,LOGIC_STATUS_RFID_SC_SELF_CONTROL,INTERFACE_MODE_SC_UNCERTIFIED,NULL);
		}		
		//对成组键做特殊处理，成组键当邻架键使用 zzw 2023.12.24
		else if(((sNewKey.u32Key == KEY_LEFT_GRP)|| (sNewKey.u32Key == KEY_RIGHT_GRP))&& ((sNewKey.u32Status == KEY_DOWN)))		
		{
//			InterfaceKeyGrpCtrlPro(sNewKey,LOGIC_STATUS_RFID_SC_SELF_CONTROL,INTERFACE_MODE_SC_UNCERTIFIED,NULL);
//			u32ReturnFlag = 0x01;	
//			//键盘指示灯
//			if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_GRP_CONTROL)   
//				ShowScGrpKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);
//			else if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_UNCERTIFIED)
//				ShowScUncertifiedKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);
//			else
//				;
			if(!InterfaceKeyNeighborPro(sNewKey,LOGIC_STATUS_RFID_SC_SELF_CONTROL,INTERFACE_MODE_SC_UNCERTIFIED,NULL))
					return 0;
			u32ReturnFlag = 0x01;	
		}	
		else if(((sNewKey.u32Key == KEY_INCREASE)|| (sNewKey.u32Key == KEY_DECREASE))&& ((sNewKey.u32Status == KEY_DOWN)))	//对+-键做特殊处理
		{
			//zzw	清除心跳
			if(RfidGetDataLimitlessSendFlag())
			{
				RfidSetDataLimitlessSendEnd();
			}	
			if(s_u32ShiftKeyEnableFlag == ENABLED)
			{
				StateLed(0x00,KEY_SHIFT_LED);
				u32ReturnFlag = 0x01;
				s_u32ShiftKeyEnableFlag = DISABLED;	
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_SHIFT_UP);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
				OSSemPost(g_stSemInterface);			
			}
			ShowScUncertifiedKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);

			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
				BuzzerOn(BEEP_COLIBRATE_START);
			#endif
			LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16DelayTime);	
			u16ScNumbOldBackup = u16DelayTime;
			if(sNewKey.u32Key == KEY_INCREASE)
			{
				if(u16DelayTime < (SYSTEM_SS_ID-0x01))//(SYSTEM_SS_ID-YKQ_NUMBER_MAX)
					u16DelayTime++;
				else
				{
					InterfaceInsertNote(INTERFACE_NOTE_SET_YKQ_ID_OUTSCOPE);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
					ShowMain(&s_u32MainUpdateMethod);				
					return	u32ReturnFlag;
				}
			}
			else
			{
				if(u16DelayTime >= 0x02)
					u16DelayTime--;
				else
				{
					InterfaceInsertNote(INTERFACE_NOTE_SET_YKQ_ID_OUTSCOPE);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
					ShowMain(&s_u32MainUpdateMethod);				
					return u32ReturnFlag;
				}
			}
							
			u32ReturnFlag = 0x00;
			g_stLogicSem->OSEventCnt = 0;
			LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16DelayTime);	
			u16ScNumbNewBackup = u16DelayTime;
			u32Y = LOGIC_COLIBRATE_SC_INCREASE_DECREASE;
			LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);
			u32Y = LOGIC_STATUS_IR_COLIBARAE_SUCCESS;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
			InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_START);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
			ShowMain(&s_u32MainUpdateMethod);
			u32ReturnFlag = 0x01;				
			//预估时间，包括了RFID应答时间
			u32Y = 0x00;
			LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
			LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
			u32Y += (u32)(u16DelayTime*u16SendTimes);

			u32Y+= 3000;	//修改
			g_stLogicSem->OSEventCnt = 0x00;
			u8tmp = getColiScNumStatus();
			u8tmp++;
			setColiScNumStatus(u8tmp);
			
			OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);
			setColiScNumStatus(0);
			StateLed(ADJUST_CODE_ING_LED,ADJUST_CODE_SUCCEED_LED);	
			if(err == OS_ERR_TIMEOUT)//无应答
			{
				s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
				
				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
				u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//先解除对码
				LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
				OSTimeDly(200/TICK_TIME);
				
				//jhyjhy
				LogicRunInfApi(LOGIC_GET_CONLIBRATE_SUCCESS_SC_NUMB,&u16ScNumbOldBackup); 
				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);
				
				InterfaceInsertNote(INTERFACE_NOTE_PLEASE_NEAR);

				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
				u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
				LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
				u32Y = 0x00;
				LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);	
				LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32Y);	
				ShowMain(&s_u32MainUpdateMethod);
				ShowStr(0, (const u8 *)"操作失败!", 0x04, 32);
				s_u8pleaseNearFlag = 1;
				OSTimeDly(100/TICK_TIME);
					
				ShowScUncertifiedKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);					

			}
			else if(err == OS_ERR_NONE)//有应答
			{
				/**关于设置的一些特殊模式切换的处理*/
				if(InterfaceGetCtrlMode() == INTERFACE_MODE_COLIBRATE_CODE_FAIL)
				{
					s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
					
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//先解除对码
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
					OSTimeDly(200/TICK_TIME);
					
					//jhyjhy
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);
					
					InterfaceInsertNote(INTERFACE_NOTE_PLEASE_NEAR);
					#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
						InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
					#else
						InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
					#endif
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
					u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
					u32Y = 0x00;
					LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);	
					LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32Y);	
					ShowMain(&s_u32MainUpdateMethod);
					ShowStr(0, (const u8 *)"操作失败!", 0x04, 32);
					s_u8pleaseNearFlag = 1;
					OSTimeDly(100/TICK_TIME);
					ShowScUncertifiedKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);
				}
				else if(InterfaceGetCtrlMode() == INTERFACE_MODE_COLIBRATE_CODE_SUCCESS)
				{				
					#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
						BuzzerOn(BEEP_KEY_PRESSED);
					#endif
					StateLed(ADJUST_CODE_SUCCEED_LED,ADJUST_CODE_ING_LED);
					
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);//本架控制模式	
 
					InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_SUCCEED);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
					ShowMain(&s_u32MainUpdateMethod);	
					ShowScSelfKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);						
					LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
					s_u32InterfaceColibrateTimer = u32Y;	
					
//					LogicRunInfApi(LOGIC_GET_CONLIBRATE_SUCCESS_SC_NUMB,&u16temptest);
//					if(u16ScNumbNewBackup>u16temptest)
//					{
//						if(u16ScNumbNewBackup-u16temptest>6)
//							u32K = 6;
//						else
//							u32K = u16ScNumbNewBackup-u16temptest;
//						for(u32Y = 0; u32Y<u32K; u32Y++)
//						{
//							LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16temptest);
//							u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//先解除对码
//							LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
//							OSTimeDly(150/TICK_TIME);
//							u16temptest++;
//						}
//					}
//					else
//					{
//						if(u16temptest-u16ScNumbNewBackup>6)
//							u32K = 6;
//						else
//							u32K = u16temptest-u16ScNumbNewBackup;
//						for(u32Y = 0; u32Y<u32K; u32Y++)
//						{
//							LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16temptest);
//							u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//先解除对码
//							LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);	
//							OSTimeDly(150/TICK_TIME);
//							u16temptest--;
//						}
//					}
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);
					
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//先解除对码
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
					OSTimeDly(200/TICK_TIME);
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
					LogicRunInfApi(LOGIC_SET_CONLIBRATE_SUCCESS_SC_NUMB,&u16ScNumbNewBackup);
					u32Y = LOGIC_COLIBRATE_SC_IGNORE;
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);	
					ShowScSelfKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);			
				
				}
				else if(InterfaceGetCtrlMode() == INTERFACE_MODE_SP)
				{	
	
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);//本架控制模式						
					LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
					s_u32InterfaceColibrateTimer = u32Y;
					u32Y = LOGIC_COLIBRATE_SC_IGNORE;
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);
					//解决+-键快速按问题		zzw
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//先解除对码
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
					OSTimeDly(200/TICK_TIME);
					
					//jhyjhy
					LogicRunInfApi(LOGIC_GET_CONLIBRATE_SUCCESS_SC_NUMB,&u16ScNumbOldBackup); 
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);					
					if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_SELF_CONTROL)
					ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
					else if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_UNCERTIFIED)
						ShowScUncertifiedKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
					else
						;					
					u32ReturnFlag = 0x01;			
				}
				else if(InterfaceGetCtrlMode() == INTERFACE_MODE_RELIEVE_COLIBRATE)
				{
					u32Y = LOGIC_STATUS_IDLE;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);	
					#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)					
						BuzzerOn(BEEP_UNCOLIBRAT);
					#endif
					u16DelayTime = RFID_SIGNAL_TOO_SMALL;
					LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DelayTime);	
					StateLed(0x00,ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED|KEY_SHIFT_LED);
					InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_FAIL);//空闲
					InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);
					u8J = NO_CTRL;
					LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);
					ShowMain(&s_u32MainUpdateMethod);	
					KeyLedOnOff(0x00, KEY_LED_ALL);
					s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
					
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//先解除对码
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
					OSTimeDly(200/TICK_TIME);
					
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);	
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//后解除对码
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
									
					u32Y = LOGIC_COLIBRATE_SC_NONE;
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);									
				}				
				else
					;
			}
			else
				;

			StateLed(0x00, KEY_SHIFT_LED);
			s_u32ShiftKeyEnableFlag = DISABLED;	
			#if (HEART == 1)		//心跳帧			
			{			
				Heart.ActionType = SIGNAL_ACTION;
				Heart.SendMode = 0;
			}	
			#endif	
		}	
		
		#if (LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG != DISABLED)	//邻架键当动作键使用
//		else if(((sNewKey.u32Key == KEY_LEFT_NEIGHBOR)|| (sNewKey.u32Key == KEY_RIGHT_NEIGHBOR))&& ((sNewKey.u32Status == KEY_DOWN)))	//对邻架键做特殊处理
//		{
//			if(!InterfaceKeyNeighborPro(sNewKey,LOGIC_STATUS_RFID_SC_SELF_CONTROL,INTERFACE_MODE_SC_UNCERTIFIED,NULL))
//					return 0;
//			u32ReturnFlag = 0x01;	
//			
//		}	
		#endif
		else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_MENU_START))//按了启动键
		{
			InterfaceGrpActionStartPro();	
			ShowScUncertifiedKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);	
		}	
		//动作键按下
		else if(sNewKey.u32Status == KEY_DOWN)		//对剩余按键
		{
			u32ReturnFlag = 0x01;					
			//清除数据		zzw
			ClearActionInfo();				
			InterfaceActionKeyDownPro(sNewKey,LOGIC_STATUS_RFID_SC_SELF_CONTROL,INTERFACE_MODE_SC_UNCERTIFIED,NULL);
			ShowScUncertifiedKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);
		}
		//动作键抬起		//zzw2023.12.25 去掉左右邻架，作为动作键
		else if(sNewKey.u32Status == KEY_UP&&(sNewKey.u32Key != KEY_SHIFT)&&(sNewKey.u32Key != KEY_CERTIFY)\
			&&(sNewKey.u32Key != KEY_INCREASE)&&(sNewKey.u32Key != KEY_DECREASE)\
			&&(sNewKey.u32Key != KEY_LEFT_GRP)&&(sNewKey.u32Key != KEY_RIGHT_GRP)\
			/*&&(sNewKey.u32Key != KEY_LEFT_NEIGHBOR)&&(sNewKey.u32Key != KEY_RIGHT_NEIGHBOR)*/\
			&&(sNewKey.u32Key != KEY_MENU_START))
		{
			//清除数据		zzw
			ClearActionInfo();			
			InterfaceActionKeyUpPro(sNewKey,LOGIC_STATUS_RFID_SC_SELF_CONTROL,INTERFACE_MODE_SC_UNCERTIFIED,NULL);	
			ShowScUncertifiedKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);
			u32ReturnFlag = 0x01;
		}	
		else
			;	
        //无线测试使能，发送无线测试帧
        LogicParamApi(LOGIC_GET_WL_TEST_ENABLE, &u32Y);
        if((u8)(u32Y &0x01) == ENABLE)
        {
            //zzw	清除心跳
            if(RfidGetDataLimitlessSendFlag())
            {
                RfidSetDataLimitlessSendEnd();
            }	
            LogicRunInfApi(LOGIC_WL_TEST_SEND_PROC,&u32Y);
        }
        
		return u32ReturnFlag;

	}
#endif

/*******************************************************************************************
**函数名称：InterfaceModeScSelfQuickProc
**函数作用：显示层处于本架快捷操作模式下,按键的处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
#if (QUICK_KEY_CTRL_ENABLE_FLAG	!= DISABLED)
	static u32 InterfaceModeScSelfQuickProc(stKEY	sNewKey)
	{
		u32 u32ReturnFlag = 0x00;
		u32 u32Y = 0x00;
		u32 u32RfidData = RFID_ACTION_NONE;
		RFID_FRAME_TYPE stRfidType;
		
		s_u32AllKeyStatus = sNewKey.u32Status;
		LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
		s_u32InterfaceColibrateTimer = u32Y;		
		InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_QUICK);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_AWAKE);
		u32RfidData = InterfaceGetKeyRfidDataShowName(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag,sNewKey.u32Key,&(s_stInterfaceActionName[0x00]),&stRfidType);
		/*按键无效时的处理**/
		if(u32RfidData == RFID_ACTION_NONE)
		{
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
			ShowMain(&s_u32MainUpdateMethod);
			if(RfidGetDataLimitlessSendFlag())
			{
				RfidSetDataLimitlessSendEnd();
				
			}
			u32ReturnFlag = LogicRunInfApi(LOGIC_GET_SC_QUICK_TYPE, &u32Y);
			if(u32Y != SC_ACTION_QUICK_DONE)
				InterfaceInsertNote(INTERFACE_NOTE_ACTION_JIANGYI_STOP);			
			u32Y = SC_ACTION_QUICK_STOP;
			u32ReturnFlag = LogicRunInfApi(LOGIC_SET_SC_QUICK_TYPE, &u32Y);
			InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息	
			ShowMain(&s_u32MainUpdateMethod);			
			return u32ReturnFlag;
		}
		
		s_u64InterfaceRfidDataBackup = (u32RfidData|(stRfidType<<30));
		
		if((sNewKey.u32Key == KEY_QUICK_CTRL_1) && (sNewKey.u32Status == KEY_UP))//此时停止所有动作的发送
		{
			u32ReturnFlag = LogicRunInfApi(LOGIC_GET_SC_QUICK_TYPE, &u32Y);
			if(u32Y != SC_ACTION_QUICK_DONE)
				InterfaceInsertNote(INTERFACE_NOTE_ACTION_JIANGYI_STOP);
			u32Y = SC_ACTION_QUICK_STOP;
			u32ReturnFlag = LogicRunInfApi(LOGIC_SET_SC_QUICK_TYPE, &u32Y);				
			for(u32Y = 0x00; u32Y < INTERFACE_KEY_TOGETHER_MAX;u32Y++)
				s_stInterfaceActionName[u32Y] = INTERFACE_ACTION_NONE;//默认屏幕显示			
			InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息	
			ShowMain(&s_u32MainUpdateMethod);			
		}
		else
			;
			
		return u32ReturnFlag;

		
	}
#endif
/*******************************************************************************************
**函数名称：InterfaceModeKeyProcFuncs
**函数作用：显示层处于本架控制模式下,按键的处理(无用代码zzw)
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
static u32 InterfaceModeScSelfControlProc(stKEY	sNewKeyInput)
{
	u32 u32ReturnFlag = 0x00;
	INT8U err;
	static u64 u64ButtonValue;
	static u64 u64ButtonAckValue;
	static u64 u64stRfidType;
	SC_BUTTON_TYPE status;
	u32 u32Y = 0x00;
	u32 u32K = 0x00;
	u8 u8tmp;
	static u32 u32StopKeyFlag = KEY_UP;//
	static u64 u64Y;
	static u64 u64RfidData;
	static RFID_FRAME_TYPE stRfidType;
	static stKEY sNewKey;
	u8 u8J = 0;
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)	
		u16 u16ScNumbOldBackup;
		u16 u16ScNumbNewBackup;		
		u16 u16temptest;
	#endif
	
	u16 u16DelayTime = 0x00;
	u16 u16SendTimes = 0x00;
	stRfidType = RFID_FRAME_INVAILD;
	sNewKey = sNewKeyInput;
	u64Y = 0x00;
	u64RfidData = RFID_ACTION_NONE;
	
	s_u32AllKeyStatus = sNewKey.u32Status;
	LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
	s_u32InterfaceColibrateTimer = u32Y;
	InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
	InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_AWAKE|INTERFACE_SHOW_CTRL_MODE);	//zzw
	ShowMain(&s_u32MainUpdateMethod);	
	if((sNewKey.u32Key&KEY_STOP_SHUTDOWN) == KEY_STOP_SHUTDOWN)
	{	
		if(RfidGetDataLimitlessSendFlag())
		{
			RfidSetDataLimitlessSendEnd();
			stRfidType = RFID_FRAME_ACTION;;
			LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
			LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//发送的数据			
			LogicRunInfApi( LOGIC_SET_BUTTON_UP,&s_u64InterfaceRfidDataBackup); 
			for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
				s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示				
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
			ShowMain(&s_u32MainUpdateMethod);				
		}		
		if(sNewKey.u32Key == KEY_STOP_SHUTDOWN)
		{
			#if	((SPECIAL_F1_ENABLE_FLAG != DISABLED)&&(SPECIAL_F2_ENABLE_FLAG != DISABLED)&&(SPECIAL_F3_ENABLE_FLAG != DISABLED)&&(SPECIAL_F4_ENABLE_FLAG != DISABLED))
				if((s_u32StartKeyFlag != KEY_DOWN)&&(s_u32SpecialFxKeyFlag1 != KEY_DOWN) \
					&&(s_u32SpecialFxKeyFlag2 != KEY_DOWN) &&(s_u32SpecialFxKeyFlag3 != KEY_DOWN) \
					&&(s_u32SpecialFxKeyFlag4 != KEY_DOWN))
			#elif	((SPECIAL_F1_ENABLE_FLAG != DISABLED)&&(SPECIAL_F2_ENABLE_FLAG != DISABLED)&&(SPECIAL_F3_ENABLE_FLAG != DISABLED))	
				if((s_u32StartKeyFlag != KEY_DOWN)&&(s_u32SpecialFxKeyFlag1 != KEY_DOWN) \
					&&(s_u32SpecialFxKeyFlag2 != KEY_DOWN) &&(s_u32SpecialFxKeyFlag3 != KEY_DOWN))	
			#elif	((SPECIAL_F1_ENABLE_FLAG != DISABLED)&&(SPECIAL_F2_ENABLE_FLAG != DISABLED))	
				if((s_u32StartKeyFlag != KEY_DOWN)&&(s_u32SpecialFxKeyFlag1 != KEY_DOWN) \
					&&(s_u32SpecialFxKeyFlag2 != KEY_DOWN))						
			#elif (SPECIAL_F1_ENABLE_FLAG != DISABLED)	
				if((s_u32StartKeyFlag != KEY_DOWN)&&(s_u32SpecialFxKeyFlag1 != KEY_DOWN))
			#else
				if((s_u32StartKeyFlag != KEY_DOWN))
			#endif
			{
				s_u32ShiftKeyEnableFlag = DISABLED;
				StateLed(0x00, KEY_SHIFT_LED);
			}
		}
		sNewKey.u32Key = KEY_STOP_SHUTDOWN;
	}	
	u64RfidData = InterfaceGetKeyRfidDataShowName(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag,sNewKey.u32Key,&(s_stInterfaceActionName[0x00]),&stRfidType);
	u64stRfidType = (u64)stRfidType;
	/*按键无效时的处理**/
	u64ButtonAckValue = ((u64)(RFID_FRAME_ACTION))<<62;
	u64ButtonAckValue |= ((u64)(RFID_FRAME_STATUS))<<62;
	u64ButtonValue = u64RfidData;
	u64ButtonValue |= ((u64)(u64stRfidType))<<62;
    if(((u64RfidData == RFID_ACTION_NONE)||(u64ButtonValue > u64ButtonAckValue))||  \
		(((sNewKey.u32Key & INVALID_ACTION) != 0) && (s_u32ShiftKeyEnableFlag == ENABLED) && (sNewKey.u32Status == KEY_DOWN)))
	{
		if((sNewKey.u32Status == KEY_DOWN)||(sNewKey.u32Status == KEY_UP)||(sNewKey.u32Status == KEY_LONGDOWN))
		{
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);
			for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
				s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示				
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
			ShowMain(&s_u32MainUpdateMethod);	
			if(RfidGetDataLimitlessSendFlag())
			{
				RfidSetDataLimitlessSendEnd();
				stRfidType = RFID_FRAME_ACTION;;
				LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
				LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//发送的数据			
				LogicRunInfApi( LOGIC_SET_BUTTON_UP,&s_u64InterfaceRfidDataBackup); 			
			}
		}
		s_u32StartKeyFlag= KEY_UP;
		#if (SPECIAL_FX_ENABLE_FLAG != DISABLED)
			#if (SPECIAL_F1_ENABLE_FLAG != DISABLED)
				s_u32SpecialFxKeyFlag1= KEY_UP;//
			#endif
			#if (SPECIAL_F2_ENABLE_FLAG != DISABLED)
				s_u32SpecialFxKeyFlag2= KEY_UP;//
			#endif
			#if (SPECIAL_F3_ENABLE_FLAG != DISABLED)
				s_u32SpecialFxKeyFlag3= KEY_UP;//
			#endif
			#if (SPECIAL_F4_ENABLE_FLAG != DISABLED)
				s_u32SpecialFxKeyFlag4= KEY_UP;//
			#endif
		#endif			
		s_u64InterfaceRfidDataBackup = 0x00;	
		if(sNewKey.u32Status == KEY_DOWN)
			ShowScSelfKeyLedProc(sNewKey.u32Key,KEY_DOWN,s_u32ShiftKeyEnableFlag);
		else	
			ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
		return u32ReturnFlag;
	}
	#if 0 	//自保键	zzw
	#if (ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)
		if(sNewKey.u32Status == KEY_DOWN)
			s_u32ZibaoActionKeyBackup = sNewKey.u32Key;
		else if (sNewKey.u32Status == KEY_UP)
			s_u32ZibaoActionKeyBackup &= ~sNewKey.u32Key;
		else
			;
		if(((sNewKey.u32Key&KEY_ZI_BAOCHI) == KEY_ZI_BAOCHI) && (sNewKey.u32Status == KEY_DOWN)&&(s_u32ShiftKeyEnableFlag == DISABLED))
		{
			s_u32StartKeyFlag= KEY_UP;
			#if (SPECIAL_FX_ENABLE_FLAG != DISABLED)
					#if (SPECIAL_F1_ENABLE_FLAG != DISABLED)
						s_u32SpecialFxKeyFlag1= KEY_UP;//
					#endif
					#if (SPECIAL_F2_ENABLE_FLAG != DISABLED)
						s_u32SpecialFxKeyFlag2= KEY_UP;//
					#endif
					#if (SPECIAL_F3_ENABLE_FLAG != DISABLED)
						s_u32SpecialFxKeyFlag3= KEY_UP;//
					#endif
					#if (SPECIAL_F4_ENABLE_FLAG != DISABLED)
						s_u32SpecialFxKeyFlag4= KEY_UP;//
					#endif
			#endif
			s_u32ZibaoActionKeyBackup &= ~KEY_ZI_BAOCHI;
			LogicParamApi(LOGIC_GET_ZIBAO_ENABLE_FLAG,&u16DelayTime);
			if(u16DelayTime == ENABLED)
			{
				s_u64InterfaceRfidDataBackup = 0x00;
				
				s_u32ShiftKeyEnableFlag = DISABLED;	
				StateLed(0x00,KEY_SHIFT_LED);	
				for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示					
				if(s_u32ZibaoActionKeyBackup == 0x00)
				{	
					s_u32ZibaoActionKeyBackup = 0x00;
					u32Y = LOGIC_STATUS_RFID_SC_ZIBAO_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);				
					InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_ZIBAO_CONTROL);	
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_ZIBAO_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息	
					ShowMain(&s_u32MainUpdateMethod);					
				}
				else
				{
					s_u32ZibaoActionKeyBackup = 0x00;
					u32Y = LOGIC_STATUS_RFID_SC_ZIBAO_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);				
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_ZIBAO_CONTROL);					
					(*InterfaceModeKeyProcFuncs[s_stInterfaceControlMode])(sNewKey);
				}
				ShowScZibaoKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);				
				return 0x01;
			}
			else
			{
				for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示					
				InterfaceInsertNote(INTERFACE_NOTE_ZIBAO_DISABLED);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//显示主屏所有信息	
				ShowMain(&s_u32MainUpdateMethod);
				if(RfidGetDataLimitlessSendFlag())
				{
					RfidSetDataLimitlessSendEnd();
					stRfidType = RFID_FRAME_ACTION;;
					LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
					LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//发送的数据			
					LogicRunInfApi( LOGIC_SET_BUTTON_UP,&s_u64InterfaceRfidDataBackup); 			
				}
				ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
				return 0x00;
			}
		}
		else if(((sNewKey.u32Key&KEY_ZI_BAOCHI) == KEY_ZI_BAOCHI) && (sNewKey.u32Status == KEY_UP))
		{
			InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息	
			ShowMain(&s_u32MainUpdateMethod);	
//			KeyLedOnOff	(0x00,KEY_LED_ALL);
			ShowScSelfKeyLedProc(KEY_INVALID,KEY_UP,s_u32ShiftKeyEnableFlag);
		}
		else if(((sNewKey.u32Key&KEY_ZI_BAOCHI) == KEY_ZI_BAOCHI) && (sNewKey.u32Status == KEY_DOWN)&&(s_u32ShiftKeyEnableFlag == ENABLED))
		{
			if(RfidGetDataLimitlessSendFlag())
			{
				RfidSetDataLimitlessSendEnd();
				stRfidType = RFID_FRAME_ACTION;
				LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
				LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//发送的数据			
				LogicRunInfApi( LOGIC_SET_BUTTON_UP,&s_u64InterfaceRfidDataBackup); 	
			}
			for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
				s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示		
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息	
			ShowMain(&s_u32MainUpdateMethod);
			s_u64InterfaceRfidDataBackup = 0x00;	
			return 0x00;
		}
	#endif
	#endif	
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)	
		/**启动键确认中，只识别确认和停止**/
		if((s_u32StartKeyFlag == KEY_DOWN))
		{
			if((sNewKey.u32Key != KEY_CERTIFY) && (sNewKey.u32Key != KEY_STOP_SHUTDOWN))
			{
				if(sNewKey.u32Status == KEY_UP)
					KeyLedOnOff	(0x00,KEY_LED_ALL);
				else if(sNewKey.u32Status == KEY_DOWN)
				{
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
					ShowMain(&s_u32MainUpdateMethod);
				}
				return u32ReturnFlag;
			}
		}
		#if (SPECIAL_FX_ENABLE_FLAG != DISABLED)	//需要确认的按键，已经按下过，而且已弹出过确认画面，其标签为KEY_DOWN，此时按下无效键(非确认键和停止键)时，在此处处理
			#if	((SPECIAL_F1_ENABLE_FLAG != DISABLED)&&(SPECIAL_F2_ENABLE_FLAG != DISABLED)&&(SPECIAL_F3_ENABLE_FLAG != DISABLED)&&(SPECIAL_F4_ENABLE_FLAG != DISABLED))
				if((s_u32SpecialFxKeyFlag1 == KEY_DOWN) || (s_u32SpecialFxKeyFlag2 == KEY_DOWN) ||(s_u32SpecialFxKeyFlag3 == KEY_DOWN) ||(s_u32SpecialFxKeyFlag4 == KEY_DOWN) )
			#elif ((SPECIAL_F1_ENABLE_FLAG != DISABLED)&&(SPECIAL_F2_ENABLE_FLAG != DISABLED)&&(SPECIAL_F3_ENABLE_FLAG != DISABLED))
				if((s_u32SpecialFxKeyFlag1 == KEY_DOWN) || (s_u32SpecialFxKeyFlag2 == KEY_DOWN) ||(s_u32SpecialFxKeyFlag3 == KEY_DOWN) )
			#elif	((SPECIAL_F1_ENABLE_FLAG != DISABLED)&&(SPECIAL_F2_ENABLE_FLAG != DISABLED))
				if((s_u32SpecialFxKeyFlag1 == KEY_DOWN) || (s_u32SpecialFxKeyFlag2 == KEY_DOWN) )
			#elif	((SPECIAL_F1_ENABLE_FLAG != DISABLED))
				if((s_u32SpecialFxKeyFlag1 == KEY_DOWN) )
			#endif
				{
					if((sNewKey.u32Key != KEY_CERTIFY) && (sNewKey.u32Key != KEY_STOP_SHUTDOWN))
					{
						if(sNewKey.u32Status == KEY_UP)
							KeyLedOnOff	(0x00,KEY_LED_ALL);
						else if(sNewKey.u32Status == KEY_DOWN)
						{
							InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);
							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
							ShowMain(&s_u32MainUpdateMethod);
						}						
						return u32ReturnFlag;
					}					
				}					
		#endif
	#endif

	#if (SHIFT_MULTIPLE_CTRL_ENABLE_FLAG != DISABLED)  //对包含shift的按键，在此处加上shift
		#if  (KEY_AUTO_PRESS_PROC_FLAG	!=	DISABLED)
			if((s_u32ShiftKeyEnableFlag ==  ENABLED)&&((KEY_AUTO_PRESS_SPECIAL&KEY_MULRIPLE_VALUE) == KEY_MULRIPLE_VALUE))
			{
				if(sNewKey.u32Key == (KEY_AUTO_PRESS_SPECIAL&(~KEY_MULRIPLE_VALUE)))
					sNewKey.u32Key |= KEY_MULRIPLE_VALUE;
			}
		#endif
		#if  (SPECIAL_FX_ENABLE_FLAG	!=	DISABLED)
			#if (SPECIAL_F1_ENABLE_FLAG	!= DISABLED)
//				if((s_u32ShiftKeyEnableFlag ==  ENABLED)&&((KEY_SHORTCUT_F1&KEY_MULRIPLE_VALUE) == KEY_MULRIPLE_VALUE))
//				{
//					if(sNewKey.u32Key == (KEY_SHORTCUT_F1&(~KEY_MULRIPLE_VALUE)))
//						sNewKey.u32Key |= KEY_MULRIPLE_VALUE;
//				}
			#endif
			#if (SPECIAL_F2_ENABLE_FLAG	!= DISABLED)
//				if((s_u32ShiftKeyEnableFlag ==  ENABLED)&&((KEY_SHORTCUT_F2&KEY_MULRIPLE_VALUE) == KEY_MULRIPLE_VALUE))
//				{
//					if(sNewKey.u32Key == (KEY_SHORTCUT_F2&(~KEY_MULRIPLE_VALUE)))
//						sNewKey.u32Key |= KEY_MULRIPLE_VALUE;
//				}
			#endif	
			#if (SPECIAL_F3_ENABLE_FLAG	!= DISABLED)
				if((s_u32ShiftKeyEnableFlag ==  ENABLED)&&((KEY_SHORTCUT_F3&KEY_MULRIPLE_VALUE) == KEY_MULRIPLE_VALUE))
				{
					if(sNewKey.u32Key == (KEY_SHORTCUT_F3&(~KEY_MULRIPLE_VALUE)))
						sNewKey.u32Key |= KEY_MULRIPLE_VALUE;
				}
			#endif
			#if (SPECIAL_F4_ENABLE_FLAG	!= DISABLED)
				if((s_u32ShiftKeyEnableFlag ==  ENABLED)&&((KEY_SHORTCUT_F4&KEY_MULRIPLE_VALUE) == KEY_MULRIPLE_VALUE))
				{
					if(sNewKey.u32Key == (KEY_SHORTCUT_F4&(~KEY_MULRIPLE_VALUE)))
						sNewKey.u32Key |= KEY_MULRIPLE_VALUE;
				}
			#endif				
		#endif			
		if((sNewKey.u32Key == KEY_SHIFT)&& (sNewKey.u32Status == KEY_DOWN))		//按下shift键
		{
			if(RfidGetDataLimitlessSendFlag())
			{
				RfidSetDataLimitlessSendEnd();
			}				
			if(s_u32ShiftKeyEnableFlag == DISABLED)
			{
				StateLed(KEY_SHIFT_LED,0x00);
				u32ReturnFlag = 0x01;
				s_u32ShiftKeyEnableFlag = ENABLED;
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_SHIFT_DOWN);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
				OSSemPost(g_stSemInterface);			
			}
			else
			{
				StateLed(0x00,KEY_SHIFT_LED);
				u32ReturnFlag = 0x01;
				s_u32ShiftKeyEnableFlag = DISABLED;	
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_SHIFT_UP);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
				OSSemPost(g_stSemInterface);			
			}			
		}	
		else if((sNewKey.u32Key == KEY_SHIFT)&& (sNewKey.u32Status == KEY_UP))	//抬起shift键
		{
			ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
		}		
	#else
		if((sNewKey.u32Key == KEY_SHIFT)&& (sNewKey.u32Status == KEY_DOWN))
		{
			s_u32ShiftKeyEnableFlag = ENABLED;
		}
		else if((sNewKey.u32Key == KEY_SHIFT)&&(s_u32ShiftKeyEnableFlag == ENABLED)&&(sNewKey.u32Status == KEY_UP))
		{
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
			ShowMain(&s_u32MainUpdateMethod);	
			s_u32ShiftKeyEnableFlag = DISABLED;
		}	
	#endif
	/**识别解除对码键(长按SHIFT，解除对码)**/
	else if((sNewKey.u32Key == KEY_SHIFT)&&(sNewKey.u32Status == KEY_LONGDOWN))	 
	{
		#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
			BuzzerOn(BEEP_UNCOLIBRAT);
		#endif
		s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
		InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
		s_u32ShiftKeyEnableFlag = DISABLED;
		StateLed(0x00,KEY_SHIFT_LED);
		StateLed(0x00,ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED);
		u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;
		LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
		u16DelayTime = RFID_SIGNAL_TOO_SMALL;
		LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DelayTime);		
		InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_UNCOLIBRATE);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);		
		ShowMain(&s_u32MainUpdateMethod);	
		KeyLedOnOff(0x00, KEY_LED_ALL);
		u32ReturnFlag = 0x01;		
	}	
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG	!= DISABLED)
		/**在鲍店矿中，识别加减键操作***/
		else if(((sNewKey.u32Key == KEY_INCREASE)|| (sNewKey.u32Key == KEY_DECREASE))&& ((sNewKey.u32Status == KEY_DOWN)))
		{
			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
				BuzzerOn(BEEP_COLIBRATE_START);
			#endif
			LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16DelayTime);	
			u16ScNumbOldBackup = u16DelayTime;
			if(sNewKey.u32Key == KEY_INCREASE)
			{
				if(u16DelayTime < (SYSTEM_SS_ID-0x01))
					u16DelayTime++;
				else
				{
					InterfaceInsertNote(INTERFACE_NOTE_SET_YKQ_ID_OUTSCOPE);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
					ShowMain(&s_u32MainUpdateMethod);				
					return	u32ReturnFlag;
				}
			}
			else
			{
				if(u16DelayTime >= 0x02)
					u16DelayTime--;
				else
				{
					InterfaceInsertNote(INTERFACE_NOTE_SET_YKQ_ID_OUTSCOPE);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
					ShowMain(&s_u32MainUpdateMethod);				
					return u32ReturnFlag;
				}
			}
			StateLed(ADJUST_CODE_ING_LED,ADJUST_CODE_SUCCEED_LED);
			u32ReturnFlag = 0x00;
			g_stLogicSem->OSEventCnt = 0;		
			LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16DelayTime);	
			u16ScNumbNewBackup = u16DelayTime;
			u32Y = LOGIC_COLIBRATE_SC_INCREASE_DECREASE;
			LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);
			u32Y = LOGIC_STATUS_IR_COLIBARAE_SUCCESS;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
			InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_START);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
			ShowMain(&s_u32MainUpdateMethod);
			u32ReturnFlag = 0x01;				
			//预估时间，包括了RFID应答时间
			u32Y = 0x00;
			LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
			LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
			u32Y += (u32)(u16DelayTime*u16SendTimes);

			u32Y += 3000;
			g_stLogicSem->OSEventCnt = 0x00;
			u8tmp = getColiScNumStatus();
			u8tmp++;
			setColiScNumStatus(u8tmp);
			OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);
			setColiScNumStatus(0);
			#if 0
				err =OS_ERR_NONE;
				InterfaceSetCtrlMode( INTERFACE_MODE_COLIBRATE_CODE_SUCCESS);
			#endif
			StateLed(ADJUST_CODE_ING_LED,ADJUST_CODE_SUCCEED_LED);	
			if(err == OS_ERR_TIMEOUT)//无应答
			{
				StateLed(ADJUST_CODE_SUCCEED_LED,ADJUST_CODE_ING_LED);
				s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
				
				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
				u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//先解除对码
				LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
				OSTimeDly(200/TICK_TIME);
				LogicRunInfApi(LOGIC_GET_CONLIBRATE_SUCCESS_SC_NUMB,&u16ScNumbOldBackup); 
				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);
				
				/**键值显示**/   //jhy发送停止
				
				s_u32StartKeyFlag= KEY_UP;
				u32StopKeyFlag = KEY_UP;
				stRfidType = RFID_FRAME_STATUS;
				u64RfidData = 0x60;
				LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
				LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//发送的数据
				g_stLogicSem->OSEventCnt = 0;
				u32Y = SC_BUTTON_ACK_NONE;
				LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//将应答设为无效	
				u64Y = RFID_ACTION_NONE;
				LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u64Y);//将应答动作字设为无效						
				u64Y = (u64)(u64RfidData|(u64stRfidType<<62));
				LogicRunInfApi(LOGIC_SET_BUTTON_VALUE,&u64Y);
				LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
				LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
				u32Y = (u32)(u16DelayTime*u16SendTimes);	
				u32Y += 230;			
				
				OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);			
				{
					InterfaceInsertNote(INTERFACE_NOTE_PLEASE_NEAR);
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
					u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
					u32Y = 0x00;
					LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);	
					LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32Y);	
					ShowMain(&s_u32MainUpdateMethod);
					ShowStr(0, (const u8 *)"操作失败!", 0x04, 32);
					s_u8pleaseNearFlag = 1;
					OSTimeDly(100/TICK_TIME);					
				}
				u32ReturnFlag = 0x01;
				
				if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_SELF_CONTROL)
				ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
				else if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_UNCERTIFIED)
					ShowScUncertifiedKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
				else
					;		
				u32StopKeyFlag = KEY_UP;			
			}
			else if(err == OS_ERR_NONE)//有应答
			{
				/**关于设置的一些特殊模式切换的处理*/
				if(InterfaceGetCtrlMode() == INTERFACE_MODE_COLIBRATE_CODE_FAIL)
				{	
					StateLed(ADJUST_CODE_SUCCEED_LED,ADJUST_CODE_ING_LED);
					s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
					
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//先解除对码
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
					OSTimeDly(200/TICK_TIME);
					LogicRunInfApi(LOGIC_GET_CONLIBRATE_SUCCESS_SC_NUMB,&u16ScNumbOldBackup); 
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);
					
					/**键值显示**/   //jhy发送停止
					stRfidType = RFID_FRAME_STATUS;
					u64RfidData = 0x60;
					LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
					LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//发送的数据
					g_stLogicSem->OSEventCnt = 0;
					u32Y = SC_BUTTON_ACK_NONE;
					LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//将应答设为无效	
					u64Y = RFID_ACTION_NONE;
					LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u64Y);//将应答动作字设为无效						
					u64Y = (u64)(u64RfidData|(u64stRfidType<<62));
					LogicRunInfApi(LOGIC_SET_BUTTON_VALUE,&u64Y);
					LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
					LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
					u32Y = (u32)(u16DelayTime*u16SendTimes);	
					u32Y += 230;			
					
					OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);			
					{
						InterfaceInsertNote(INTERFACE_NOTE_PLEASE_NEAR);
						#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
							InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
						#else
							InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
						#endif
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
						u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
						LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
						u32Y = 0x00;
						LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);	
						LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32Y);	
						ShowMain(&s_u32MainUpdateMethod);
						ShowStr(0, (const u8 *)"操作失败!", 0x04, 32);
						s_u8pleaseNearFlag = 1;
						OSTimeDly(100/TICK_TIME);					
					}
					u32ReturnFlag = 0x01;
					
					if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_SELF_CONTROL)
					ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
					else if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_UNCERTIFIED)
						ShowScUncertifiedKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
					else
						;		

				}
				else if(InterfaceGetCtrlMode() == INTERFACE_MODE_COLIBRATE_CODE_SUCCESS)
				{
					#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
						BuzzerOn(BEEP_KEY_PRESSED);
					#endif
					StateLed(ADJUST_CODE_SUCCEED_LED,ADJUST_CODE_ING_LED);
					/**在使用加减键的情况下，直接进入动作状态***/
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);//本架控制模式	
					
					InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_SUCCEED);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
					ShowMain(&s_u32MainUpdateMethod);	
					LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
					s_u32InterfaceColibrateTimer = u32Y;	
					
//					LogicRunInfApi(LOGIC_GET_CONLIBRATE_SUCCESS_SC_NUMB,&u16temptest);
//					
//					if(u16ScNumbNewBackup>u16temptest)
//					{
//						if(u16ScNumbNewBackup-u16temptest>6)
//							u32K = 6;
//						else
//							u32K = u16ScNumbNewBackup-u16temptest;
//						for(u32Y = 0; u32Y<u32K; u32Y++)
//						{
//							LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16temptest);
//							u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//先解除对码
//							LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);	
//							OSTimeDly(150/TICK_TIME);
//							u16temptest++;
//						}
//					}
//					else
//					{
//						if(u16temptest-u16ScNumbNewBackup>6)
//							u32K = 6;
//						else
//							u32K = u16temptest-u16ScNumbNewBackup;
//						for(u32Y = 0; u32Y<u32K; u32Y++)
//						{
//							LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16temptest);
//							u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//先解除对码
//							LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);	
//							OSTimeDly(150/TICK_TIME);
//							u16temptest--;
//						}
//					}
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);
					
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//先解除对码
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
					OSTimeDly(200/TICK_TIME);
					
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
					
					LogicRunInfApi(LOGIC_SET_CONLIBRATE_SUCCESS_SC_NUMB,&u16ScNumbNewBackup);
					
					u32Y = LOGIC_COLIBRATE_SC_IGNORE;
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);					
					
				}
				else if(InterfaceGetCtrlMode() == INTERFACE_MODE_SP)
				{	
					StateLed(ADJUST_CODE_ING_LED,ADJUST_CODE_SUCCEED_LED);
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);//本架控制模式						
					LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
					s_u32InterfaceColibrateTimer = u32Y;
//					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
//					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//先解除对码
//					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						

//					OSTimeDly(200/TICK_TIME);
//					
//					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);
					u32Y = LOGIC_COLIBRATE_SC_IGNORE;
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);
					
					if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_SELF_CONTROL)
					ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
					else if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_UNCERTIFIED)
						ShowScUncertifiedKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
					else
						;
					u32ReturnFlag = 0x01;	
				}
				else if(InterfaceGetCtrlMode() == INTERFACE_MODE_RELIEVE_COLIBRATE)
				{
					u32Y = LOGIC_STATUS_IDLE;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);	
					#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)					
						BuzzerOn(BEEP_UNCOLIBRAT);
					#endif
					u16DelayTime = RFID_SIGNAL_TOO_SMALL;
					LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DelayTime);	
					StateLed(0x00,ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED|KEY_SHIFT_LED);
					InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_FAIL);//空闲
					InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);
					u8J = NO_CTRL;
					LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);
					ShowMain(&s_u32MainUpdateMethod);	
					KeyLedOnOff(0x00, KEY_LED_ALL);
					s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
					
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//先解除对码
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
					OSTimeDly(200/TICK_TIME);
					
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);	
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//后解除对码
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
									
					u32Y = LOGIC_COLIBRATE_SC_NONE;
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);									
				}
				else
					;
			}
			else
				;
			StateLed(0x00, KEY_SHIFT_LED);
			s_u32ShiftKeyEnableFlag = DISABLED;	
			if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_SELF_CONTROL)
				ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
			else if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_UNCERTIFIED)
				ShowScUncertifiedKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
			else
				;	
		}		
	#endif	

	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
#if 0 		//自动移架		zzw
		else if((sNewKey.u32Status == KEY_DOWN) && (sNewKey.u32Key == KEY_MENU_START) 
			&&(stRfidType == RFID_FRAME_STATUS)&&((u32)u64RfidData == RFID_STATUS_START)) //本架动作模式下，按下启动键，表示自动拉架，在此处弹出确认菜单，并为标签赋值
		{
			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
				BuzzerOn(BEEP_ACTION);
			#endif
			if(sNewKey.u32Key == KEY_MENU_START)
//			if((sNewKey.u32Key == KEY_MENU_START)|| (sNewKey.u32Key == (KEY_MENU_START | KEY_MULRIPLE_VALUE)))
			{
				s_u32StartKeyFlag= KEY_DOWN;			
			}
			else
			{
				;					
			}
			InterfaceInsertNote(INTERFACE_NOTE_START_CERTIFY);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);
			ShowMain(&s_u32MainUpdateMethod);
			#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE == ENABLED)
				KeyLedOnOff(0x00,KEY_LED_ALL);
			#endif
		}	
#endif
	#else
		/**识别其他操作按键**/
		else if((sNewKey.u32Key == KEY_MENU_START)&& ((sNewKey.u32Status == KEY_DOWN)))
		{
			/**键值显示**/
			LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
			LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u32RfidData);//发送的数据
			g_stLogicSem->OSEventCnt = 0;
			u32Y = SC_BUTTON_ACK_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//将应答设为无效	
			u32Y = RFID_ACTION_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u32Y);//将应答动作字设为无效			
			u32Y = (u32)(u32RfidData|(stRfidType<<30));	
			LogicRunInfApi(LOGIC_SET_BUTTON_VALUE,&u32Y);
			InterfaceInsertNote(INTERFACE_NOTE_SC_SET);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
			ShowMain(&s_u32MainUpdateMethod);		
			LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
			LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
			u32Y = (u32)(u16DelayTime*u16SendTimes);	
			u32Y += 30;
			
			OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
			if(err == OS_ERR_TIMEOUT)//无应答
			{
				InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
				ShowMain(&s_u32MainUpdateMethod);		
			}
			else if(err == OS_ERR_NONE)//有应答
			{
				/**关于设置的一些特殊模式切换的处理*/
				LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
				LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u32ButtonValue);
				LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&u32ButtonAckValue);
				if((status == SC_BUTTON_ACK_ACCEPT) && (u32ButtonAckValue == u32ButtonValue))
				{
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_SET);
					u32Y = LOGIC_STATUS_RFID_SC_SET;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
					ShowMain(&s_u32MainUpdateMethod);		
				}
				else if((status == SC_BUTTON_ACK_REJECT))
				{
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);									
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
					ShowMain(&s_u32MainUpdateMethod);	
				}
				else
				{
					InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);										
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
					ShowMain(&s_u32MainUpdateMethod);	
				}			
			}
			else
				;
			
			u32ReturnFlag = 0x01;
				
		}
	#endif
	#if ((INCREASE_DECREASE_CTRL_ENABLE_FLAG	!= DISABLED)&&(QUICK_KEY_CTRL_ENABLE_FLAG	!= DISABLED))
		#if (QUICK_KEY_1_ENABLE_FLAG != DISABLED)
			else if((sNewKey.u32Status == KEY_DOWN) && ((sNewKey.u32Key == KEY_QUICK_CTRL_1)))
			{
				sNewKey.u32Key = KEY_QUICK_CTRL_1;//需重新计算需要发送的键值
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_QUICK);	

				s_stInterfaceActionName[0x00] = SC_ACTION_QUICK_1;
				s_stInterfaceActionName[0x01] = INTERFACE_ACTION_NONE;				
				u32ReturnFlag = LogicRunInfApi(LOGIC_SET_SC_QUICK_TYPE, &s_stInterfaceActionName);
				if(s_stInterfaceActionName[0x00])	
				{
					u32Y = LOGIC_STATUS_RFID_SC_QUICK_CTRL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_QUICK);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL);//显示主屏所有信息
					ShowMain(&s_u32MainUpdateMethod);
					if((s_stInterfaceActionName[0x00] != 0x00) && (u32ReturnFlag != 0x00))//有需要立即执行的动作
					{
						if((u32ReturnFlag == INTERFACE_NOTE_ACTION_ALARAM)||(u32ReturnFlag == INTERFACE_NOTE_ACTION_ING))
						{
							InterfaceInsertNote((INTERFACE_NOTE_TYPE)(u32ReturnFlag));
							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
							ShowMain(&s_u32MainUpdateMethod);	
						}
						return 0x00;
					}
					else
					{
						return 0x00;	
					}
				}
				else
				{
					InterfaceInsertNote(INTERFACE_NOTE_QUICK_ACTIONNUMB_NONE);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
					ShowMain(&s_u32MainUpdateMethod);	
					return 0x00;
				}
				
			}
		#endif
	#endif	
	#if (LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG != DISABLED)
		else if(((sNewKey.u32Key == KEY_LEFT_NEIGHBOR)|| (sNewKey.u32Key == KEY_RIGHT_NEIGHBOR))&& ((sNewKey.u32Status == KEY_DOWN)))
		{
			/**键值显示**/
			LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
			LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//发送的数据
			
			g_stLogicSem->OSEventCnt = 0;
			u32Y = LOGIC_STATUS_RFID_SC_NEIGHBORL;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);			
			u32Y = SC_BUTTON_ACK_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//将应答设为无效	
			u32Y = RFID_ACTION_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u32Y);//将应答动作字设为无效			
			u32Y = (u32)(u64RfidData|(stRfidType<<30));
			LogicRunInfApi(LOGIC_SET_BUTTON_VALUE,&u32Y);
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_DOWN);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);	
			ShowMain(&s_u32MainUpdateMethod);
			LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
			LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
			u32Y = (u32)(u16DelayTime*u16SendTimes);
			u32Y += 50;  //jhy
//			u32Y += 480;
			OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
			if(err == OS_ERR_TIMEOUT)//无应答
			{
			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
				BuzzerOn(BEEP_UNCOLIBRAT);
			#endif
				//jhy不解除对码
				s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
				StateLed(0x00,KEY_SHIFT_LED);
				StateLed(0x00,ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED);			
				u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//2015.12.29,改为邻架键无应答，解除对码
				LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
				u16DelayTime = RFID_SIGNAL_TOO_SMALL;
				LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DelayTime);			
				InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
				for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示

				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息	
				ShowMain(&s_u32MainUpdateMethod);	
				InterfaceInsertNote(INTERFACE_NOTE_NOACK_UNCOLIBRATE);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示提示信息
				ShowMain(&s_u32MainUpdateMethod);	
//				KeyLedOnOff(0x00, KEY_LED_ALL);
			}
			else if(err == OS_ERR_NONE)//有应答
			{
				/**关于设置的一些特殊模式切换的处理*/
				LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
				LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u64ButtonValue);
				LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&u64ButtonAckValue);
				if((status == SC_BUTTON_ACK_ACCEPT) && (u64ButtonAckValue == u64ButtonValue))
				{
					LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32Y);		
					for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
						s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示

					InterfaceSetCtrlMode(INTERFACE_MODE_SC_NEIGHBOR_CONTROL);
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
					ShowMain(&s_u32MainUpdateMethod);		
				}
				else if(status == SC_BUTTON_ACK_REJECT)
				{
					u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);		
					for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
						s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示
				
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
					ShowMain(&s_u32MainUpdateMethod);	
				}
				else
				{
				//jhy不解除对码
				#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
					BuzzerOn(BEEP_UNCOLIBRAT);
				#endif
					s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
					for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
						s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示
				
					StateLed(0x00,KEY_SHIFT_LED);
					StateLed(0x00,ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED);			
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//2015.12.29,改为邻架键无应答，解除对码
					LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
					u16DelayTime = RFID_SIGNAL_TOO_SMALL;
					LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DelayTime);				
					InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
					KeyLedOnOff(0x00, KEY_LED_ALL);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息	
					ShowMain(&s_u32MainUpdateMethod);	
					InterfaceInsertNote(INTERFACE_NOTE_NOACK_UNCOLIBRATE);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示提示信息		
					ShowMain(&s_u32MainUpdateMethod);	
				}
			}
			else
				;
			
			u32ReturnFlag = 0x01;	
			
		}	
	#endif
	#if (GRP_CTRL_ENABLE_FLAG != DISABLED)
		else if(((sNewKey.u32Key == KEY_LEFT_GRP)|| (sNewKey.u32Key == KEY_RIGHT_GRP))&& ((sNewKey.u32Status == KEY_DOWN)))  //按下左右成组键
		{
			/**键值显示**/
			u32Y  = (RFID_STATUS_GRP_LEFT|RFID_STATUS_GRP_RIGHT);
			u32Y |= ((u32)(RFID_FRAME_STATUS))<<30;
			if((s_u64InterfaceRfidDataBackup^u32Y) != 0x00)			
			{
				s_u64InterfaceRfidDataBackup &= ~u32Y;
				if(s_u64InterfaceRfidDataBackup != 0x00)
				{
					g_stLogicSem->OSEventCnt = 0;
					u32Y = (s_u64InterfaceRfidDataBackup>>30);
					LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &u32Y);//发送的数据
					LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//发送的数据		
					LogicRunInfApi(LOGIC_SET_BUTTON_UP,&s_u64InterfaceRfidDataBackup); 
					LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
					LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
					u32Y = (u32)(u16DelayTime*u16SendTimes);	
					u32Y += 50;
					OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);					
				}
			}
			/**键值显示**/
			LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
			LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//发送的数据		
			g_stLogicSem->OSEventCnt = 0;
			u32Y = SC_GRP_ACTION_DIRCTION_MAX;//将成组方向设置为无效
			LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION,&u32Y);//将方向设为无效
			u32Y = SC_BUTTON_ACK_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//将应答设为无效	
			u64Y = RFID_ACTION_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u64Y);//将应答动作字设为无效			
			u64Y = (u64)(u64RfidData|(u64stRfidType<<62));
			LogicRunInfApi(LOGIC_SET_BUTTON_VALUE,&u64Y);		
			u32Y = LOGIC_STATUS_RFID_SC_GRP;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);			
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL);
			ShowMain(&s_u32MainUpdateMethod);
			LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
			LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
			u32Y = (u32)(u16DelayTime*u16SendTimes);	
			u32Y += 250;
			
			OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
			if(err == OS_ERR_TIMEOUT)//无应答
			{
				InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);			
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);;
				ShowMain(&s_u32MainUpdateMethod);	
			}
			else if(err == OS_ERR_NONE)//有应答
			{
				/**关于设置的一些特殊模式切换的处理*/
				LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
				LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u64ButtonValue);
				LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&u64ButtonAckValue);
				if((status == SC_BUTTON_ACK_ACCEPT) && (u64ButtonAckValue == u64ButtonValue))
				{
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_GRP_CONTROL);
					u32Y = LOGIC_STATUS_RFID_SC_GRP;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);			
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
//					s_stInterfaceActionName[0x00] = INTERFACE_GRP_ACTION_DEFAULT;
					for(err = 0x01; err < INTERFACE_KEY_TOGETHER_MAX;err++)
						s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示
					ShowMain(&s_u32MainUpdateMethod);
				}
				else if(status == SC_BUTTON_ACK_REJECT)
				{
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);	
					u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
					ShowMain(&s_u32MainUpdateMethod);		
				}
				else
				{
					InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);	
					u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
					ShowMain(&s_u32MainUpdateMethod);		
				}
			}
			else
				;
			
			u32ReturnFlag = 0x01;	
				
			KeyLedOnOff(0x00,KEY_LED_ALL);
			if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_GRP_CONTROL)
				ShowScGrpKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
			else if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_SELF_CONTROL)
				ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
			else
				;
		}		
	#endif
	else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_LONGDOWN))		//按住停止键
	{
		u32StopKeyFlag = KEY_LONGDOWN;
		#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
			BuzzerOn(BEEP_SHUTDOWN_NOTE);
		#endif
		OSTaskSuspend(SCAN_KEY_TASK_PRIO);//挂起按键扫描任务
		s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
		InterfaceInsertNote(INTERFACE_NOTE_SHUTDOWN);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
		ShowMain(&s_u32MainUpdateMethod);
		u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;
		LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);				
		OSTimeDly(200/TICK_TIME);
		
		u32Y = LOGIC_STATUS_SHUTUP;
		LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);			
		OSTimeDly(1000/TICK_TIME);
	}
	else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_DOWN))		//按下停止键
	{
		u32StopKeyFlag = KEY_DOWN;
	}
	//确认键进行模式切换zzw
	else if((sNewKey.u32Key == KEY_CERTIFY) && (sNewKey.u32Status == KEY_DOWN))		//按下确认键
	{
		//zzw
		InterfaceKeyCertifyPro(sNewKey,LOGIC_STATUS_RFID_SC_SELF_CONTROL,INTERFACE_MODE_SC_UNCERTIFIED,NULL);
		ShowScUncertifiedKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);	
	}
	else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_UP))//对停止键做特殊处理
	{
		if(u32StopKeyFlag == KEY_LONGDOWN)/**在任何模式下，均识别关机键*/
		{
			return u32ReturnFlag;			
	
		}
		else if(u32StopKeyFlag == KEY_DOWN)//停止键
		{
			#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
				#if (SPECIAL_FX_ENABLE_FLAG != DISABLED)
					#if ((SPECIAL_F1_ENABLE_FLAG != DISABLED)&& (SPECIAL_F2_ENABLE_FLAG != DISABLED)&&(SPECIAL_F3_ENABLE_FLAG != DISABLED)&& (SPECIAL_F4_ENABLE_FLAG != DISABLED))
						if((s_u32StartKeyFlag == KEY_DOWN) || (s_u32SpecialFxKeyFlag1 == KEY_DOWN) || (s_u32SpecialFxKeyFlag2 == KEY_DOWN) \
							|| (s_u32SpecialFxKeyFlag3 == KEY_DOWN) || (s_u32SpecialFxKeyFlag4 == KEY_DOWN))
					#elif ((SPECIAL_F1_ENABLE_FLAG != DISABLED)&&(SPECIAL_F2_ENABLE_FLAG != DISABLED)&&(SPECIAL_F3_ENABLE_FLAG != DISABLED))
						if((s_u32StartKeyFlag == KEY_DOWN) || (s_u32SpecialFxKeyFlag1 == KEY_DOWN) || (s_u32SpecialFxKeyFlag2 == KEY_DOWN) \
							|| (s_u32SpecialFxKeyFlag3 == KEY_DOWN))						
					#elif ((SPECIAL_F1_ENABLE_FLAG != DISABLED)&&(SPECIAL_F2_ENABLE_FLAG != DISABLED))
						if((s_u32StartKeyFlag == KEY_DOWN) || (s_u32SpecialFxKeyFlag1 == KEY_DOWN) || (s_u32SpecialFxKeyFlag2 == KEY_DOWN))						
					#elif ((SPECIAL_F1_ENABLE_FLAG != DISABLED))
						if((s_u32StartKeyFlag == KEY_DOWN) || (s_u32SpecialFxKeyFlag1 == KEY_DOWN) )			
					#endif
				#else
					if((s_u32StartKeyFlag == KEY_DOWN) )
				#endif		
					{
						s_u32StartKeyFlag= KEY_UP;
						u32StopKeyFlag = KEY_UP;
						#if (SPECIAL_FX_ENABLE_FLAG != DISABLED)
							#if (SPECIAL_F1_ENABLE_FLAG != DISABLED)
								s_u32SpecialFxKeyFlag1 = KEY_UP;
							#endif
							#if (SPECIAL_F2_ENABLE_FLAG != DISABLED)
								s_u32SpecialFxKeyFlag2 = KEY_UP;
							#endif	
							#if (SPECIAL_F3_ENABLE_FLAG != DISABLED)
								s_u32SpecialFxKeyFlag3 = KEY_UP;
							#endif	
							#if (SPECIAL_F4_ENABLE_FLAG != DISABLED)
								s_u32SpecialFxKeyFlag4 = KEY_UP;
							#endif	
						#endif
						u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
						LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);				
						for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
							s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示
			
						InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//显示主屏所有信息
						InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);	
						ShowMain(&s_u32MainUpdateMethod);
						ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag); 
						return u32ReturnFlag;
					}
			#endif
					#if (SPECIAL_FX_ENABLE_FLAG != DISABLED)
						#if (QUICK_KEY_1_ENABLE_FLAG != DISABLED)
							s_u32SpecialFxKeyFlag1= KEY_UP;
						#endif
						#if (QUICK_KEY_2_ENABLE_FLAG != DISABLED)
							s_u32SpecialFxKeyFlag2= KEY_UP;
						#endif
						#if (QUICK_KEY_3_ENABLE_FLAG != DISABLED)
							s_u32SpecialFxKeyFlag3= KEY_UP;
						#endif	
						#if (QUICK_KEY_4_ENABLE_FLAG != DISABLED)
							s_u32SpecialFxKeyFlag4= KEY_UP;
						#endif					
					#endif			
			/**键值显示**/
					
			LogicRunInfApi(LOGIC_GET_CONLIBRATE_SUCCESS_SC_NUMB,&u16ScNumbOldBackup); 
			LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);
			StateLed(ADJUST_CODE_SUCCEED_LED,ADJUST_CODE_ING_LED);
					
			LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
			LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//发送的数据
			g_stLogicSem->OSEventCnt = 0;
			u32Y = SC_BUTTON_ACK_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//将应答设为无效	
			u64Y = RFID_ACTION_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u64Y);//将应答动作字设为无效						
			u64Y = (u64)(u64RfidData|(u64stRfidType<<62));
			LogicRunInfApi(LOGIC_SET_BUTTON_VALUE,&u64Y);
			LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
			LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
			u32Y = (u32)(u16DelayTime*u16SendTimes);	
			u32Y += 30;			
			
			OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);			
			if(err == OS_ERR_TIMEOUT)//无应答
			{
				InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息	
				ShowMain(&s_u32MainUpdateMethod);						
			}
			else if(err == OS_ERR_NONE)//有应答
			{
				/**关于设置的一些特殊模式切换的处理*/
				LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
				LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u64ButtonValue);
				LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&u64ButtonAckValue);
				if((status == SC_BUTTON_ACK_ACCEPT) && (u64ButtonAckValue == u64ButtonValue))
				{
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
					#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
						InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
					#else
						InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
					#endif
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
					u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
					u32Y = 0x00;
					LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);	
					LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32Y);	
					ShowMain(&s_u32MainUpdateMethod);
				}
				else if(status == SC_BUTTON_ACK_REJECT)
				{
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
					#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
						InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
					#else
						InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
					#endif
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
					u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
					ShowMain(&s_u32MainUpdateMethod);							
				}				
				else
				{
					InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
					#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
						InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
					#else
						InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
					#endif
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息					
					u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
					ShowMain(&s_u32MainUpdateMethod);			
				}
			}
			else
				;
			u32ReturnFlag = 0x01;
				
			if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_SELF_CONTROL)
				ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
			else if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_UNCERTIFIED)
				ShowScUncertifiedKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
			else
				;			
		}
		u32StopKeyFlag = KEY_UP;
		
	}
	#if  (KEY_AUTO_PRESS_PROC_FLAG	!=	DISABLED)
		else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_AUTO_PRESS_SPECIAL))
		{
			s_u32AutoPressKeyFlag = KEY_DOWN;
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_DOWN);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);
			ShowMain(&s_u32MainUpdateMethod);			
		}
		else if(((sNewKey.u32Status == KEY_LONGDOWN)||(sNewKey.u32Status == KEY_UP))\
			&&(s_u32AutoPressKeyFlag == KEY_DOWN)&&(sNewKey.u32Key == KEY_AUTO_PRESS_SPECIAL))
		{
			s_u32AutoPressKeyFlag = KEY_UP;
			if(sNewKey.u32Status == KEY_LONGDOWN)
			{
				u32RfidData =DISABLED;	
				InterfaceInsertNote(INTERFACE_NOTE_AUTO_PRESS_DISABLED);
			}
			else
			{
				u32RfidData = ENABLED;	
				InterfaceInsertNote(INTERFACE_NOTE_AUTO_PRESS_ENABLED);
			}
			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
				BuzzerOn(BEEP_ACTION);
			#endif
			/**键值显示**/
			g_stLogicSem->OSEventCnt = 0;
			LogicRunInfApi(LOGIC_SET_AUTOPRESS_ENABLE_FLAG,&u64RfidData);//将应答设为无效	
			u64Y = RFID_ACTION_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u64Y);//将应答动作字设为无效			
	
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);
			ShowMain(&s_u32MainUpdateMethod);
			LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
			LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
			u32Y = (u32)(u16DelayTime*u16SendTimes);	
			u32Y += 30;		
			
			OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
			if(err == OS_ERR_TIMEOUT)//无应答
			{
				InterfaceInsertNote(INTERFACE_NOTE_AUTO_PRESS_FAIL);
			}
			else if(err == OS_ERR_NONE)//有应答
			{
				/**关于设置的一些特殊模式切换的处理*/
				LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&u32ButtonAckValue);
				if(u32ButtonAckValue == ENABLED)
				{
					InterfaceInsertNote(INTERFACE_NOTE_AUTO_PRESS_SUCCESSED);					
				}
				else
				{
					InterfaceInsertNote(INTERFACE_NOTE_AUTO_PRESS_FAIL);					
				}
			}
			else	
				InterfaceInsertNote(INTERFACE_NOTE_AUTO_PRESS_FAIL);
			InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
			u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
			for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
				s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//显示主屏所有信息
			ShowMain(&s_u32MainUpdateMethod);	
			s_u64InterfaceRfidDataBackup = 0x00;
			s_u32ZibaoActionKeyBackup= 0x00;
			ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
			u32ReturnFlag = 0x01;					
			
			s_u64InterfaceRfidDataBackup = u32RfidData;			
		}
	#endif
//	
//	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)	//对需要确认的按键，已经出现确认画面，此时按下确认键时，在此处处理
//		#if (SPECIAL_FX_ENABLE_FLAG == DISABLED)
//			else if((sNewKey.u32Status == KEY_DOWN)\
//				&& ((sNewKey.u32Key != KEY_MENU_START)||((sNewKey.u32Key == KEY_MENU_START)&&(u64RfidData != RFID_STATUS_START))))	
//		#elif ((SPECIAL_F1_ENABLE_FLAG != DISABLED)&&(SPECIAL_F2_ENABLE_FLAG != DISABLED)&&(SPECIAL_F3_ENABLE_FLAG != DISABLED)&&(SPECIAL_F4_ENABLE_FLAG != DISABLED))
//			else if((sNewKey.u32Status == KEY_DOWN)\
//					&&((sNewKey.u32Key != KEY_MENU_START)||((sNewKey.u32Key == KEY_MENU_START)&&(u64RfidData != RFID_STATUS_START)))\
//					&&((sNewKey.u32Key != KEY_SHORTCUT_F3)||((sNewKey.u32Key == KEY_SHORTCUT_F3)&&(u64RfidData != RFID_STATUS_QUICK3_KEY)))\
//					&&((sNewKey.u32Key != KEY_SHORTCUT_F4)||((sNewKey.u32Key == KEY_SHORTCUT_F4)&&(u64RfidData != RFID_STATUS_QUICK4_KEY))))			
//		#elif ((SPECIAL_F1_ENABLE_FLAG != DISABLED)&&(SPECIAL_F2_ENABLE_FLAG != DISABLED)&&(SPECIAL_F3_ENABLE_FLAG != DISABLED))
//			else if((sNewKey.u32Status == KEY_DOWN) \
//					&& ((sNewKey.u32Key != KEY_MENU_START)||((sNewKey.u32Key == KEY_MENU_START)&&(u32RfidData != RFID_STATUS_START)))\
//					&&((sNewKey.u32Key != KEY_SHORTCUT_F1)||((sNewKey.u32Key == KEY_SHORTCUT_F1)&&(u32RfidData != RFID_STATUS_QUICK1_KEY)))\
//					&&((sNewKey.u32Key != KEY_SHORTCUT_F2)||((sNewKey.u32Key == KEY_SHORTCUT_F2)&&(u32RfidData != RFID_STATUS_QUICK2_KEY)))\
//					&&((sNewKey.u32Key != KEY_SHORTCUT_F3)||((sNewKey.u32Key == KEY_SHORTCUT_F3)&&(u32RfidData != RFID_STATUS_QUICK3_KEY))))			
//		#elif ((SPECIAL_F1_ENABLE_FLAG != DISABLED)&&(SPECIAL_F2_ENABLE_FLAG != DISABLED))
//				else if((sNewKey.u32Status == KEY_DOWN) \
//					&& ((sNewKey.u32Key != KEY_MENU_START)||((sNewKey.u32Key == KEY_MENU_START)&&(u32RfidData != RFID_STATUS_START)))\
//					&&((sNewKey.u32Key != KEY_SHORTCUT_F1)||((sNewKey.u32Key == KEY_SHORTCUT_F1)&&(u32RfidData != RFID_STATUS_QUICK1_KEY)))\
//					&&((sNewKey.u32Key != KEY_SHORTCUT_F2)||((sNewKey.u32Key == KEY_SHORTCUT_F2)&&(u32RfidData != RFID_STATUS_QUICK2_KEY))))			
//		#elif ((SPECIAL_F1_ENABLE_FLAG != DISABLED))
//				else if((sNewKey.u32Status == KEY_DOWN) \
//					&& ((sNewKey.u32Key != KEY_MENU_START)||((sNewKey.u32Key == KEY_MENU_START)&&((u32)u64RfidData != RFID_STATUS_START)))\
//					&&((sNewKey.u32Key != KEY_SHORTCUT_F1)||((sNewKey.u32Key == KEY_SHORTCUT_F1)&&((u32)u64RfidData != RFID_STATUS_QUICK1_KEY))))		
//		#endif
//	#else
//		else if((sNewKey.u32Status == KEY_DOWN) )		
//	#endif
//		{
//			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
//				BuzzerOn(BEEP_ACTION);
//			#endif
//			#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
//				if((s_u32StartKeyFlag ==  KEY_DOWN) && (sNewKey.u32Key == KEY_CERTIFY) && (sNewKey.u32Status == KEY_DOWN))	//自动移架动作，按下确认后的处理
//				{
//					s_u32StartKeyFlag = KEY_UP;//降移升联动，之后点击了确认
//					#if (SPECIAL_FX_ENABLE_FLAG != DISABLED)
//						#if (SPECIAL_F1_ENABLE_FLAG != DISABLED)
//							s_u32SpecialFxKeyFlag1 = KEY_UP;
//						#endif
//						#if (SPECIAL_F2_ENABLE_FLAG != DISABLED)
//							s_u32SpecialFxKeyFlag2 = KEY_UP;
//						#endif
//						#if (SPECIAL_F3_ENABLE_FLAG != DISABLED)
//							s_u32SpecialFxKeyFlag3 = KEY_UP;
//						#endif
//						#if (SPECIAL_F4_ENABLE_FLAG != DISABLED)
//							s_u32SpecialFxKeyFlag4 = KEY_UP;
//						#endif				
//					#endif					
//					sNewKey.u32Key = KEY_MENU_START;//需重新计算需要发送的键值
//					u64RfidData = InterfaceGetKeyRfidDataShowName(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag,sNewKey.u32Key,&(s_stInterfaceActionName[0x00]),&stRfidType);
//					u64stRfidType = (u64)stRfidType;
//				}
//				#if (SPECIAL_FX_ENABLE_FLAG != DISABLED)		//4个特殊键已出现确认画面，并按下确认键后的处理
//					#if ((SPECIAL_F1_ENABLE_FLAG != DISABLED)&&(SPECIAL_F2_ENABLE_FLAG != DISABLED)&&(SPECIAL_F3_ENABLE_FLAG != DISABLED)&&(SPECIAL_F4_ENABLE_FLAG != DISABLED))
//					else if(((s_u32SpecialFxKeyFlag1 ==  KEY_DOWN) || (s_u32SpecialFxKeyFlag2 ==  KEY_DOWN)|| (s_u32SpecialFxKeyFlag3 ==  KEY_DOWN)||(s_u32SpecialFxKeyFlag4 ==  KEY_DOWN)) \
//						&& (sNewKey.u32Key == KEY_CERTIFY) && (sNewKey.u32Status == KEY_DOWN))
//					#elif ((SPECIAL_F1_ENABLE_FLAG != DISABLED)&&(SPECIAL_F2_ENABLE_FLAG != DISABLED)&&(SPECIAL_F3_ENABLE_FLAG != DISABLED))
//					else if(((s_u32SpecialFxKeyFlag1 ==  KEY_DOWN) || (s_u32SpecialFxKeyFlag2 ==  KEY_DOWN)|| (s_u32SpecialFxKeyFlag3 ==  KEY_DOWN) ) \
//						&& (sNewKey.u32Key == KEY_CERTIFY) && (sNewKey.u32Status == KEY_DOWN))						
//					#elif ((SPECIAL_F1_ENABLE_FLAG != DISABLED)&&(SPECIAL_F2_ENABLE_FLAG != DISABLED))
//					else if(((s_u32SpecialFxKeyFlag1 ==  KEY_DOWN) || (s_u32SpecialFxKeyFlag2 ==  KEY_DOWN) ) \
//						&& (sNewKey.u32Key == KEY_CERTIFY) && (sNewKey.u32Status == KEY_DOWN))				
//					#elif (SPECIAL_F1_ENABLE_FLAG != DISABLED)
//					else if(((s_u32SpecialFxKeyFlag1 ==  KEY_DOWN)) \
//						&& (sNewKey.u32Key == KEY_CERTIFY) && (sNewKey.u32Status == KEY_DOWN))
//					#endif
//					{
//						if(s_u32SpecialFxKeyFlag1 ==  KEY_DOWN)	//之前按下的是特殊键F1
//						{
//							s_u32SpecialFxKeyFlag1 = KEY_UP;							
//							s_u32StartKeyFlag = KEY_UP;
//							sNewKey.u32Key = KEY_SHORTCUT_F1;//需重新计算需要发送的键值
//							u64RfidData = InterfaceGetKeyRfidDataShowName(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag,sNewKey.u32Key,&(s_stInterfaceActionName[0x00]),&stRfidType);
//							u64stRfidType = (u64)stRfidType;
//						}
//						#if (SPECIAL_F2_ENABLE_FLAG != DISABLED)
//							else if(s_u32SpecialFxKeyFlag2 ==  KEY_DOWN)	//之前按下的是特殊键F2
//							{
//								s_u32SpecialFxKeyFlag2 = KEY_UP;
//								s_u32SpecialFxKeyFlag1 =  KEY_UP;
//								s_u32StartKeyFlag = KEY_UP;
//								sNewKey.u32Key = KEY_SHORTCUT_F2;//需重新计算需要发送的键值
//								u64RfidData = InterfaceGetKeyRfidDataShowName(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag,sNewKey.u32Key,&(s_stInterfaceActionName[0x00]),&stRfidType);
//								u64stRfidType = (u64)stRfidType;
//							}
//						#endif
//							
//						#if (SPECIAL_F3_ENABLE_FLAG != DISABLED)	
//							else if(s_u32SpecialFxKeyFlag3 ==  KEY_DOWN)	//之前按下的是特殊键F3
//							{
//								s_u32SpecialFxKeyFlag3 = KEY_UP;
//								s_u32SpecialFxKeyFlag2 = KEY_UP;
//								s_u32SpecialFxKeyFlag1 =  KEY_UP;
//								s_u32StartKeyFlag = KEY_UP;
//								sNewKey.u32Key = KEY_SHORTCUT_F3;//需重新计算需要发送的键值
//								u64RfidData = InterfaceGetKeyRfidDataShowName(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag,sNewKey.u32Key,&(s_stInterfaceActionName[0x00]),&stRfidType);
//								u64stRfidType = (u64)stRfidType;
//							}	
//						#endif	
//						#if (SPECIAL_F4_ENABLE_FLAG != DISABLED)	
//							else if(s_u32SpecialFxKeyFlag4 ==  KEY_DOWN)	//之前按下的是特殊键F4
//							{
//								s_u32SpecialFxKeyFlag4 = KEY_UP;
//								s_u32SpecialFxKeyFlag3 = KEY_UP;
//								s_u32SpecialFxKeyFlag2 = KEY_UP;
//								s_u32SpecialFxKeyFlag1 =  KEY_UP;
//								s_u32StartKeyFlag = KEY_UP;
//								sNewKey.u32Key = KEY_SHORTCUT_F4;//需重新计算需要发送的键值
//								u64RfidData = InterfaceGetKeyRfidDataShowName(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag,sNewKey.u32Key,&(s_stInterfaceActionName[0x00]),&stRfidType);
//								u64stRfidType = (u64)stRfidType;
//							}	
//						#endif	
//						else
//							;
//					}
//				#endif				
//				else if (sNewKey.u32Key == KEY_CERTIFY)//再无任何操作的情况下，直接点击确认键无效
//				{
//					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_CTRL_MODE);//显示主屏所有信息
//					ShowMain(&s_u32MainUpdateMethod);
//					if(RfidGetDataLimitlessSendFlag())
//					{
//						RfidSetDataLimitlessSendEnd();	
//					}						
//					ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);	
////					s_u32ZibaoActionKeyBackup = 0x00;					
//					return u32ReturnFlag;
//				}
//				else
//					;	
//			#endif
//			
//			/**键值显示**/
//			LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
//			LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//发送的数据
//			g_stLogicSem->OSEventCnt = 0;
//			u32Y = SC_BUTTON_ACK_NONE;
//			LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//将应答设为无效	
//			u64Y = RFID_ACTION_NONE;
//			LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u64Y);//将应答动作字设为无效			
//			u64Y = (u64)(u64RfidData|(u64stRfidType<<62));
//			LogicRunInfApi(LOGIC_SET_BUTTON_VALUE,&u64Y);
//			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_DOWN);
////			timestest++;

//			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);
//			ShowMain(&s_u32MainUpdateMethod);
//			LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
//			LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
//			u32Y = (u32)(u16DelayTime*u16SendTimes);	
//			u32Y += 230;
////			u32Y += 150;//jhyjhy

//			OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
//			if(err == OS_ERR_TIMEOUT)//无应答
//			{
//				for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
//					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示				
//				InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
//				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);
//				#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
//					if(sNewKey.u32Key != KEY_CERTIFY)
//						InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);	
//					else
//						InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
//				#else
//					InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);	
//				#endif
//				ShowMain(&s_u32MainUpdateMethod);	
////				timesnoack++;					
//			}
//			else if(err == OS_ERR_NONE)//有应答
//			{
//				/**关于设置的一些特殊模式切换的处理*/
//				LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
//				LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u64ButtonValue);
//				LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&u64ButtonAckValue);
//				if((status == SC_BUTTON_ACK_ACCEPT) && (u64ButtonAckValue == u64ButtonValue))
//				{
//					#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
//						/*某些特定按键接受	情况下允许连续发送**/
//						#if (QUICK_KEY_CTRL_ENABLE_FLAG	!=	DISABLED)
//							if((sNewKey.u32Key != KEY_CERTIFY)&&(sNewKey.u32Key != KEY_MENU_START))
//						#elif ((SPECIAL_F1_ENABLE_FLAG != DISABLED) && (SPECIAL_F2_ENABLE_FLAG != DISABLED) && (SPECIAL_F3_ENABLE_FLAG != DISABLED) && (SPECIAL_F4_ENABLE_FLAG != DISABLED))
//							if((sNewKey.u32Key != KEY_CERTIFY)&&(sNewKey.u32Key != KEY_MENU_START)\
//								&&(sNewKey.u32Key != KEY_SHORTCUT_F1)&&(sNewKey.u32Key != KEY_SHORTCUT_F2)\
//								&&(sNewKey.u32Key != KEY_SHORTCUT_F3)&&(sNewKey.u32Key != KEY_SHORTCUT_F4))
//						#elif ((SPECIAL_F1_ENABLE_FLAG != DISABLED) && (SPECIAL_F2_ENABLE_FLAG != DISABLED) && (SPECIAL_F3_ENABLE_FLAG != DISABLED))
//							if((sNewKey.u32Key != KEY_CERTIFY)&&(sNewKey.u32Key != KEY_MENU_START)\
//								&&(sNewKey.u32Key != KEY_SHORTCUT_F1)&&(sNewKey.u32Key != KEY_SHORTCUT_F2)\
//								&&(sNewKey.u32Key != KEY_SHORTCUT_F3))
//						#elif ((SPECIAL_F1_ENABLE_FLAG != DISABLED) && (SPECIAL_F2_ENABLE_FLAG != DISABLED) )
//							if((sNewKey.u32Key != KEY_CERTIFY)&&(sNewKey.u32Key != KEY_MENU_START)\
//								&&(sNewKey.u32Key != KEY_SHORTCUT_F1)&&(sNewKey.u32Key != KEY_SHORTCUT_F2))
//						#elif ((SPECIAL_F1_ENABLE_FLAG != DISABLED) )
//							if((sNewKey.u32Key != KEY_CERTIFY)&&(sNewKey.u32Key != KEY_MENU_START)\
//								&&(sNewKey.u32Key != KEY_SHORTCUT_F1))					
//						#else
//							if((sNewKey.u32Key != KEY_CERTIFY)&&(sNewKey.u32Key != KEY_MENU_START))						
//						#endif
//						{
////							OSSemPost(s_stKeyTest);
//							u64Y = (u64)(u64RfidData|(u64stRfidType<<62));
//							LogicRunInfApi(LOGIC_SET_BUTTON_LONG_VALUE,&u64Y);	
//							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//显示主屏所有信息				
//							InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
//							u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;							
//						}
//						else
//						{
//							for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
//								s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示
//							#if (FX_START_RETURN_SELF_FLAG != DISABLED)
//								InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//显示主屏所有信息				
//								InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
//								u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;								
//							#else
//								InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL);//显示主屏所有信息				
//								InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
//								u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;								
//							#endif
//							s_u32ShiftKeyEnableFlag = DISABLED;
//							StateLed(0x00,KEY_SHIFT_LED);
//						}
//						LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);						
//						ShowMain(&s_u32MainUpdateMethod);	
//					#else						
//						InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
//						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
//						InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
//						u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
//						LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
//						ShowMain(&s_u32MainUpdateMethod);
//					#endif
//				}
//				else if(status == SC_BUTTON_ACK_REJECT)
//				{
//					for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
//						s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示						
//					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
//					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//显示主屏所有信息
//					#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
//						if(sNewKey.u32Key != KEY_CERTIFY)
//							InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
//						else
//							InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);//INTERFACE_MODE_SC_UNCERTIFIED
//					#else
//						InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
//					#endif
//					u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
//					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
//					ShowMain(&s_u32MainUpdateMethod);
//				}
//				else
//				{	
////					timesnoack++;
//					for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
//						s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//??è??á????ê?					
//					InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
//					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//??ê??÷?á?ùóDD??￠
//					#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
//						if(sNewKey.u32Key != KEY_CERTIFY)
//							InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
//						else
//							InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);//INTERFACE_MODE_SC_UNCERTIFIED
//					#else
//						InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);	
//					#endif		
//					u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
//					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
//					ShowMain(&s_u32MainUpdateMethod);	
//				}
//			}
//			else
//				;
//			u32ReturnFlag = 0x01;					
//			
//			if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_SELF_CONTROL)
//			{
//				if((sNewKey.u32Key != KEY_CERTIFY)&&(sNewKey.u32Key != KEY_MENU_START)\
//				&&(sNewKey.u32Key != KEY_SHORTCUT_F1)&&(sNewKey.u32Key != KEY_SHORTCUT_F2)\
//				&&(sNewKey.u32Key != KEY_SHORTCUT_F3)&&(sNewKey.u32Key != KEY_SHORTCUT_F4))
//					ShowScSelfKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);	
//				else
//					ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);	
//			}
//			else if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_UNCERTIFIED)
//			{
//				if((sNewKey.u32Key != KEY_CERTIFY)&&(sNewKey.u32Key != KEY_MENU_START)\
//				&&(sNewKey.u32Key != KEY_SHORTCUT_F1)&&(sNewKey.u32Key != KEY_SHORTCUT_F2)\
//				&&(sNewKey.u32Key != KEY_SHORTCUT_F3)&&(sNewKey.u32Key != KEY_SHORTCUT_F4))
//					ShowScUncertifiedKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);	
//				else
//					ShowScUncertifiedKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);	
//			}
//			else
//				;			
//		}
//	#if ((INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)&&(QUICK_KEY_CTRL_ENABLE_FLAG	== DISABLED)\
//		&&(ZIBAO_CTRL_ENABLE_FLAG == DISABLED))
//		else if((sNewKey.u32Status == KEY_UP)&&(sNewKey.u32Key != KEY_SHIFT)\
//			&&(sNewKey.u32Key != KEY_INCREASE)&&(sNewKey.u32Key != KEY_DECREASE))
//	#elif ((INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)&&(QUICK_KEY_CTRL_ENABLE_FLAG == DISABLED)\
//		&&(ZIBAO_CTRL_ENABLE_FLAG != DISABLED))
//		else if((sNewKey.u32Status == KEY_UP)&&(sNewKey.u32Key != KEY_SHIFT)\
//			&&(sNewKey.u32Key != KEY_INCREASE)&&(sNewKey.u32Key != KEY_DECREASE)\
//			&&(sNewKey.u32Key != KEY_ZI_BAOCHI))		
//	#elif 	((INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)&&(ZIBAO_CTRL_ENABLE_FLAG == DISABLED)\
//		&&(QUICK_KEY_1_ENABLE_FLAG != DISABLED)&&(QUICK_KEY_2_ENABLE_FLAG != DISABLED)\
//		&&(QUICK_KEY_3_ENABLE_FLAG != DISABLED)&&(QUICK_KEY_4_ENABLE_FLAG != DISABLED))	
//		else if((sNewKey.u32Status == KEY_UP)&&(sNewKey.u32Key != KEY_SHIFT)\
//			&&(sNewKey.u32Key != KEY_INCREASE)&&(sNewKey.u32Key != KEY_DECREASE)\
//			&&(sNewKey.u32Key != KEY_QUICK_CTRL_1)&&(sNewKey.u32Key != KEY_QUICK_CTRL_2)\
//			&&(sNewKey.u32Key != KEY_QUICK_CTRL_3)&&(sNewKey.u32Key != KEY_QUICK_CTRL_4))			
//	#elif 	((INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)&&(ZIBAO_CTRL_ENABLE_FLAG == DISABLED)\
//		&&(QUICK_KEY_1_ENABLE_FLAG != DISABLED)&&(QUICK_KEY_2_ENABLE_FLAG != DISABLED)\
//		&&(QUICK_KEY_3_ENABLE_FLAG != DISABLED))	
//		else if((sNewKey.u32Status == KEY_UP)&&(sNewKey.u32Key != KEY_SHIFT)
//			&&(sNewKey.u32Key != KEY_INCREASE)&&(sNewKey.u32Key != KEY_DECREASE)\
//			&&(sNewKey.u32Key != KEY_QUICK_CTRL_1)&&(sNewKey.u32Key != KEY_QUICK_CTRL_2)\
//			&&(sNewKey.u32Key != KEY_QUICK_CTRL_3))			
//	#elif 	((INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)&&(ZIBAO_CTRL_ENABLE_FLAG == DISABLED)\
//		&&(QUICK_KEY_1_ENABLE_FLAG != DISABLED)&&(QUICK_KEY_2_ENABLE_FLAG != DISABLED))	
//		else if((sNewKey.u32Status == KEY_UP)&&(sNewKey.u32Key != KEY_SHIFT)\
//			&&(sNewKey.u32Key != KEY_INCREASE)&&(sNewKey.u32Key != KEY_DECREASE)\
//			&&(sNewKey.u32Key != KEY_QUICK_CTRL_1)&&(sNewKey.u32Key != KEY_QUICK_CTRL_2))			
//	#elif 	((INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)&&(ZIBAO_CTRL_ENABLE_FLAG == DISABLED)\
//		&&(QUICK_KEY_1_ENABLE_FLAG != DISABLED))	
//		else if((sNewKey.u32Status == KEY_UP)&&(sNewKey.u32Key != KEY_SHIFT)\
//			&&(sNewKey.u32Key != KEY_INCREASE)&&(sNewKey.u32Key != KEY_DECREASE)\
//			&&(sNewKey.u32Key != KEY_QUICK_CTRL_1))
//	#elif     ((INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)&&(ZIBAO_CTRL_ENABLE_FLAG != DISABLED))
//		else if(sNewKey.u32Status == KEY_UP&&(sNewKey.u32Key != KEY_SHIFT)\
//			&&(sNewKey.u32Key != KEY_CERTIFY)&&((sNewKey.u32Key != KEY_ZI_BAOCHI)))	
//	#else
//		else if(sNewKey.u32Status == KEY_UP&&(sNewKey.u32Key != KEY_SHIFT)&&(sNewKey.u32Key != KEY_CERTIFY))	
//	#endif
//		{
//			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
//				BuzzerOff();
//			#endif
//			#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
//				if((sNewKey.u32Key != KEY_GRP_LEFT_DIRECTION)||(sNewKey.u32Key != KEY_GRP_RIGHT_DIRECTION))
//			#else
//				if((sNewKey.u32Key != KEY_LEFT_NEIGHBOR)||(sNewKey.u32Key != KEY_RIGHT_NEIGHBOR))
//			#endif	
//			{	
//				for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
//					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示
//			}
//			#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
//				if(sNewKey.u32Key != KEY_CERTIFY)

//				{
//					if(RfidGetDataLimitlessSendFlag())
//					{
//						RfidSetDataLimitlessSendEnd();
//					}		
//					#if (KEY_AUTO_PRESS_PROC_FLAG != DISABLED)
//						if(~((sNewKey.u32Key == KEY_24)&&(s_u32ShiftKeyEnableFlag == ENABLED)))
//						{
//							LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
//							LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&u32ButtonAckValue);	
//							u32Y = (u32)(u32RfidData|(stRfidType<<30));
//							if(status != SC_BUTTON_ACK_ACCEPT)//防止在按键按下拒绝或无应答时，仍发送按键抬起。
//							{
//								ShowScSelfKeyLedProc(KEY_INVALID,KEY_UP,s_u32ShiftKeyEnableFlag);
//								return 0x00;
//							}
//							LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
//							LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u32RfidData);//发送的数据
//							
//							LogicRunInfApi(LOGIC_SET_BUTTON_UP, &u32Y);	
//						}							
//						InterfaceInsertNote(INTERFACE_NOTE_BUTTON_UP);
//						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
//						ShowMain(&s_u32MainUpdateMethod);	
//					#else
//						LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
//						if(status != SC_BUTTON_ACK_ACCEPT)//防止在按键按下拒绝或无应答时，仍发送按键抬起。
//						{	
//							ShowScSelfKeyLedProc(KEY_INVALID,KEY_UP,s_u32ShiftKeyEnableFlag);
//							return 0x00;						
//						}
////						if(g_u32ID != B_type)//jhyjhy防止刚按下直接受到应答，马上发送长按数据
////							OSTimeDly(10/TICK_TIME);
//						LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
//						LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//发送的数据
//						u64Y = (u64)(u64RfidData|(u64stRfidType<<62));
//						LogicRunInfApi(LOGIC_SET_BUTTON_UP, &u64Y);	
//						InterfaceInsertNote(INTERFACE_NOTE_BUTTON_UP);
//						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
//						ShowMain(&s_u32MainUpdateMethod);						
//					#endif		
//				}			
//				/**键值显示**/
//				#if (QUICK_KEY_CTRL_ENABLE_FLAG	!= DISABLED)
//					if((sNewKey.u32Key == KEY_MENU_START)  ||  (sNewKey.u32Key == KEY_CERTIFY) \
//						||(sNewKey.u32Key == KEY_QUICK_CTRL_1) )
//				#else
//					if((sNewKey.u32Key == KEY_MENU_START)  ||  (sNewKey.u32Key == KEY_CERTIFY))				
//				#endif
//				{
//					ClearUpdateMainMethod();	
//					InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);
//					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//显示主屏所有信息
//					ShowMain(&s_u32MainUpdateMethod);		
//				}
//				else
//				{
//					#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
//						if((sNewKey.u32Key != KEY_GRP_LEFT_DIRECTION)||(sNewKey.u32Key != KEY_GRP_RIGHT_DIRECTION))
//					#else
//						if((sNewKey.u32Key != KEY_LEFT_NEIGHBOR)&&(sNewKey.u32Key != KEY_RIGHT_NEIGHBOR)&& (sNewKey.u32Key != KEY_CERTIFY))
//					#endif	
//					{
//						
//						g_stLogicSem->OSEventCnt = 0x00;
//						u32Y = 560+30;//此时间与发送的时间有关jhyjhy
//						OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
//						if(err == OS_ERR_TIMEOUT)//无应答
//						{
//							InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
//							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);			
//							InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);;
//							for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
//								s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示					
//							ShowMain(&s_u32MainUpdateMethod);	
//						}
//						else if(err == OS_ERR_NONE)//有应答
//						{
//							/**关于设置的一些特殊模式切换的处理*/
//							LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
//							LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u64ButtonValue);
//							LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&u64ButtonAckValue);
//							if((status == SC_BUTTON_ACK_ACCEPT) && (u64ButtonAckValue == u64ButtonValue))
//							{
//								InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
//								InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//显示主屏所有信息				
//								InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
//								u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
//								LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
//								for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
//									s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示						
//								ShowMain(&s_u32MainUpdateMethod);	
//							}
//							else if(status == SC_BUTTON_ACK_REJECT)
//							{
//								InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
//								InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//显示主屏所有信息
//								InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
//								u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
//								LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
//								for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
//									s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示						
//								ShowMain(&s_u32MainUpdateMethod);	
//							}
//							else
//							{
//								InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
//								InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//显示主屏所有信息
//								InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
//								u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
//								for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
//									s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示						LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
//								ShowMain(&s_u32MainUpdateMethod);	
//							}						
//						}
//						else
//							;	
//						
//						u32ReturnFlag = 0x01;									
//						for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
//							s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示
//						ShowScSelfKeyLedProc(KEY_INVALID,KEY_UP,s_u32ShiftKeyEnableFlag);	
// 	
//					}
//				}						
//			#else	
//				{		
//					/**键值显示**/
//					if(RfidGetDataLimitlessSendFlag())
//					{
//						RfidSetDataLimitlessSendEnd();
//					}					
//					LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
//					LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u32RfidData);//发送的数据		
//					u32Y = (u32)(u32RfidData|(stRfidType<<30));
//					LogicRunInfApi( LOGIC_SET_BUTTON_UP,&u32Y); 
//					LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
//					LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u32RfidData);//发送的数据
//					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL);
//					InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);
//					for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
//						s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示
//					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//显示主屏所有信息
//					ShowMain(&s_u32MainUpdateMethod);
//				}	
//			#endif
//		}
//#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG	!= DISABLED)		//需要显示确认的按键，按下后且无线码返回正确时，在此处显示确认画面，并为该按键标签赋值
//	#if (SPECIAL_FX_ENABLE_FLAG != DISABLED)
//			#if ((SPECIAL_F1_ENABLE_FLAG != DISABLED) && (SPECIAL_F2_ENABLE_FLAG != DISABLED) && (SPECIAL_F3_ENABLE_FLAG != DISABLED) && (SPECIAL_F4_ENABLE_FLAG != DISABLED))
//				else if((sNewKey.u32Status == KEY_DOWN) && ((sNewKey.u32Key == KEY_MENU_START) \
//					||(sNewKey.u32Key == KEY_SHORTCUT_F1)||(sNewKey.u32Key == KEY_SHORTCUT_F2)
//					||(sNewKey.u32Key == KEY_SHORTCUT_F3)||(sNewKey.u32Key == KEY_SHORTCUT_F4))\
//					&&((u64RfidData == RFID_STATUS_START)||(u64RfidData == RFID_STATUS_QUICK1_KEY)\
//					||(u64RfidData == RFID_STATUS_QUICK2_KEY)||(u64RfidData == RFID_STATUS_QUICK3_KEY)||(u64RfidData == RFID_STATUS_QUICK4_KEY)))
//			#elif ((SPECIAL_F1_ENABLE_FLAG != DISABLED) && (SPECIAL_F2_ENABLE_FLAG != DISABLED) && (SPECIAL_F3_ENABLE_FLAG != DISABLED))
//				else if((sNewKey.u32Status == KEY_DOWN) && ((sNewKey.u32Key == KEY_MENU_START) \
//					||(sNewKey.u32Key == KEY_SHORTCUT_F1)||(sNewKey.u32Key == KEY_SHORTCUT_F2)
//					||(sNewKey.u32Key == KEY_SHORTCUT_F3))\
//					&&((u64RfidData == RFID_STATUS_START)||(u64RfidData == RFID_STATUS_QUICK1_KEY)\
//					||(u64RfidData == RFID_STATUS_QUICK2_KEY)||(u64RfidData == RFID_STATUS_QUICK3_KEY)))
//			#elif ((SPECIAL_F1_ENABLE_FLAG != DISABLED) && (SPECIAL_F2_ENABLE_FLAG != DISABLED) )
//				else if((sNewKey.u32Status == KEY_DOWN) && ((sNewKey.u32Key == KEY_MENU_START) \
//					||(sNewKey.u32Key == KEY_SHORTCUT_F1)||(sNewKey.u32Key == KEY_SHORTCUT_F2))\
//					&&((u64RfidData == RFID_STATUS_START)||(u64RfidData == RFID_STATUS_QUICK1_KEY)||(u64RfidData == RFID_STATUS_QUICK2_KEY)))
//			#elif ((SPECIAL_F1_ENABLE_FLAG != DISABLED))
//				else if((sNewKey.u32Status == KEY_DOWN) && ((sNewKey.u32Key == KEY_MENU_START) \
//					||(sNewKey.u32Key == KEY_SHORTCUT_F1))\
//					&&(((u32)u64RfidData == RFID_STATUS_START)||((u32)u64RfidData == RFID_STATUS_QUICK1_KEY)))
//			#endif
//	#else	
//			else if((sNewKey.u32Status == KEY_DOWN) && ((sNewKey.u32Key == KEY_MENU_START)))
//	#endif					
//			{
//				#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
//					BuzzerOn(BEEP_ACTION);
//				#endif
//				if(sNewKey.u32Key == KEY_MENU_START)
//				{
//					s_u32StartKeyFlag= KEY_DOWN;			
//				}
//				#if (SPECIAL_F1_ENABLE_FLAG != DISABLED)
//					else if(sNewKey.u32Key == KEY_SHORTCUT_F1)
//					{
//						#if ((SPECIAL_F1_ENABLE_FLAG != DISABLED))
//							s_u32SpecialFxKeyFlag1 = KEY_DOWN;
//						#endif	
//					
//						#if ((SPECIAL_F2_ENABLE_FLAG != DISABLED))
//							s_u32SpecialFxKeyFlag2 = KEY_UP;
//						#endif
//					
//						#if ((SPECIAL_F3_ENABLE_FLAG != DISABLED))
//							s_u32SpecialFxKeyFlag3 = KEY_UP;
//						#endif
//					
//						#if ((SPECIAL_F4_ENABLE_FLAG != DISABLED))
//							s_u32SpecialFxKeyFlag4 = KEY_UP;
//						#endif
//						
//					}
//				#endif	
//				#if (SPECIAL_F2_ENABLE_FLAG != DISABLED)
//					else if(sNewKey.u32Key == KEY_SHORTCUT_F2)
//					{
//							#if ((SPECIAL_F1_ENABLE_FLAG != DISABLED))
//								s_u32SpecialFxKeyFlag1 = KEY_UP;
//							#endif	
//						
//							#if ((SPECIAL_F2_ENABLE_FLAG != DISABLED))
//								s_u32SpecialFxKeyFlag2 = KEY_DOWN;
//							#endif
//						
//							#if ((SPECIAL_F3_ENABLE_FLAG != DISABLED))
//								s_u32SpecialFxKeyFlag3 = KEY_UP;
//							#endif
//						
//							#if ((SPECIAL_F4_ENABLE_FLAG != DISABLED))
//								s_u32SpecialFxKeyFlag4 = KEY_UP;
//							#endif
//						
//					}	
//				#endif	
//				#if (SPECIAL_F3_ENABLE_FLAG != DISABLED)		
//					else if(sNewKey.u32Key == KEY_SHORTCUT_F3)
//					{
//							#if ((SPECIAL_F1_ENABLE_FLAG != DISABLED))
//								s_u32SpecialFxKeyFlag1 = KEY_UP;
//							#endif	
//						
//							#if ((SPECIAL_F2_ENABLE_FLAG != DISABLED))
//								s_u32SpecialFxKeyFlag2 = KEY_UP;
//							#endif
//		
//							if(u16KeyFilmNameRam != 0x01)		//一键放煤键在综采键盘膜(0x01)时不能使?2020.01.15 parry?
//							{
//								#if ((SPECIAL_F3_ENABLE_FLAG != DISABLED))
//									s_u32SpecialFxKeyFlag3 = KEY_DOWN;
//								#endif
//							}
//							else
//							{
//								return u32ReturnFlag;
//							}
//							#if ((SPECIAL_F4_ENABLE_FLAG != DISABLED))
//								s_u32SpecialFxKeyFlag4 = KEY_UP;
//							#endif
//						
//					}
//				#endif		
//				#if (SPECIAL_F4_ENABLE_FLAG != DISABLED)
//					else if(sNewKey.u32Key == KEY_SHORTCUT_F4)
//					{
//							#if ((SPECIAL_F1_ENABLE_FLAG != DISABLED))
//								s_u32SpecialFxKeyFlag1 = KEY_UP;
//							#endif	
//						
//							#if ((SPECIAL_F2_ENABLE_FLAG != DISABLED))
//								s_u32SpecialFxKeyFlag2 = KEY_UP;
//							#endif
//						
//							#if ((SPECIAL_F3_ENABLE_FLAG != DISABLED))
//								s_u32SpecialFxKeyFlag3 = KEY_UP;
//							#endif
//						
//							#if ((SPECIAL_F4_ENABLE_FLAG != DISABLED))
//								s_u32SpecialFxKeyFlag4 = KEY_DOWN;
//							#endif
//						
//					}					
//				#endif	
//				InterfaceInsertNote(INTERFACE_NOTE_START_CERTIFY);
//				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);
//				ShowMain(&s_u32MainUpdateMethod);
//				ShowScSelfKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);
//			}	
//#endif
//	else
//		;
//		
//	if(sNewKey.u32Status == KEY_DOWN)
//		s_u64InterfaceRfidDataBackup = (u64RfidData|(u64stRfidType<<62));
//	else if(sNewKey.u32Status == KEY_UP)
//		s_u64InterfaceRfidDataBackup = 0x00;
//	else
//		;
	
	return u32ReturnFlag;
}


/*******************************************************************************************
**函数名称：InterfaceModeScNeighborControlProc
**函数作用：显示层处于邻隔架控制模式下,按键的处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
#if (LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG != DISABLED)
	static u32 InterfaceModeScNeighborControlProc(stKEY	sNewKey)
	{
		u32 u32ReturnFlag = 0x00;
		INT8U err;
		static u64 u64ButtonValue;
		static u64 u64ButtonAckValue;
		SC_BUTTON_TYPE status;
		u32 u32Y = 0x00;
		u32 u32I = 0x00;
		u32 u32K = 0x00;
		u8 u8tmp = 0;
		static u64 u64RfidData;
		static u64 u64Y;
		static u32 u32StopKeyFlag = KEY_UP;//
		static u64 u64stRfidType;
		RFID_FRAME_TYPE stRfidType;
		u16 u16DelayTime = 0x00;
		u16 u16SendTimes = 0x00;
		u16 u16ScNumbOldBackup;
		u16 u16ScNumbNewBackup;		
		u8 u8J = 0;
		u64RfidData = RFID_ACTION_NONE;
		
		s_u32AllKeyStatus = sNewKey.u32Status;
		LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
		s_u32InterfaceColibrateTimer = u32Y;		
		InterfaceSetCtrlMode(INTERFACE_MODE_SC_NEIGHBOR_CONTROL);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_AWAKE);
		
		if((sNewKey.u32Key&KEY_STOP_SHUTDOWN) == KEY_STOP_SHUTDOWN)
		{	
			if(RfidGetDataLimitlessSendFlag())
			{
				RfidSetDataLimitlessSendEnd();
				stRfidType = RFID_FRAME_ACTION;;
				LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
				LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//发送的数据			
				LogicRunInfApi( LOGIC_SET_BUTTON_UP,&s_u64InterfaceRfidDataBackup); 
				for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示				
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
				ShowMain(&s_u32MainUpdateMethod);				
			}		
			if(sNewKey.u32Key == KEY_STOP_SHUTDOWN)
			{
				#if	((SPECIAL_F1_ENABLE_FLAG != DISABLED)&&(SPECIAL_F2_ENABLE_FLAG != DISABLED)&&(SPECIAL_F3_ENABLE_FLAG != DISABLED)&&(SPECIAL_F4_ENABLE_FLAG != DISABLED))
					if((s_u32StartKeyFlag != KEY_DOWN)&&(s_u32SpecialFxKeyFlag1 != KEY_DOWN) \
						&&(s_u32SpecialFxKeyFlag2 != KEY_DOWN) &&(s_u32SpecialFxKeyFlag3 != KEY_DOWN) \
						&&(s_u32SpecialFxKeyFlag4 != KEY_DOWN))
				#elif	((SPECIAL_F1_ENABLE_FLAG != DISABLED)&&(SPECIAL_F2_ENABLE_FLAG != DISABLED)&&(SPECIAL_F3_ENABLE_FLAG != DISABLED))	
					if((s_u32StartKeyFlag != KEY_DOWN)&&(s_u32SpecialFxKeyFlag1 != KEY_DOWN) \
						&&(s_u32SpecialFxKeyFlag2 != KEY_DOWN) &&(s_u32SpecialFxKeyFlag3 != KEY_DOWN))	
				#elif	((SPECIAL_F1_ENABLE_FLAG != DISABLED)&&(SPECIAL_F2_ENABLE_FLAG != DISABLED))	
					if((s_u32StartKeyFlag != KEY_DOWN)&&(s_u32SpecialFxKeyFlag1 != KEY_DOWN) \
						&&(s_u32SpecialFxKeyFlag2 != KEY_DOWN))						
				#elif (SPECIAL_F1_ENABLE_FLAG != DISABLED)	
					if((s_u32StartKeyFlag != KEY_DOWN)&&(s_u32SpecialFxKeyFlag1 != KEY_DOWN))
				#else
					if((s_u32StartKeyFlag != KEY_DOWN))
				#endif
				{
					s_u32ShiftKeyEnableFlag = DISABLED;
					StateLed(0x00, KEY_SHIFT_LED);
				}
			}
			sNewKey.u32Key = KEY_STOP_SHUTDOWN;
		}	
		//shift键短按
		if((sNewKey.u32Key == KEY_SHIFT)&& (sNewKey.u32Status == KEY_DOWN))			
		{
			if(s_u32ShiftKeyEnableFlag == DISABLED)
			{
				StateLed(KEY_SHIFT_LED,0x00);
				u32ReturnFlag = 0x01;
				s_u32ShiftKeyEnableFlag = ENABLED;
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_SHIFT_DOWN);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
				OSSemPost(g_stSemInterface);			
			}
			else
			{
				StateLed(0x00,KEY_SHIFT_LED);
				u32ReturnFlag = 0x01;
				s_u32ShiftKeyEnableFlag = DISABLED;	
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_SHIFT_UP);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
				OSSemPost(g_stSemInterface);			
			}
			ShowScUncertifiedKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);
		}	
		else if((sNewKey.u32Key == KEY_SHIFT)&& (sNewKey.u32Status == KEY_UP))
		{

		}		
		/**识别解除对码键(长按SHIFT，解除对码)**/
		else if((sNewKey.u32Key == KEY_SHIFT)&&(sNewKey.u32Status == KEY_LONGDOWN))			
		{
			InterfaceShiftKeyLongDownPro();
			u32ReturnFlag = 0x01;		
		}	
		//确认键
		else if((sNewKey.u32Key == KEY_CERTIFY)&& (sNewKey.u32Status == KEY_DOWN || sNewKey.u32Status == KEY_UP))		//对确认键做特殊处理
		{
			u32ReturnFlag = InterfaceKeyCertifyPro(sNewKey,LOGIC_STATUS_RFID_SC_NEIGHBORL,INTERFACE_MODE_SC_NEIGHBOR_CONTROL,NULL);
		}			
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG	!= DISABLED)
		/**加减键操作***/
		else if(((sNewKey.u32Key == KEY_INCREASE)|| (sNewKey.u32Key == KEY_DECREASE))&& ((sNewKey.u32Status == KEY_DOWN)))
		{
			//zzw	清除心跳
			if(RfidGetDataLimitlessSendFlag())
			{
				RfidSetDataLimitlessSendEnd();
			}
			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
				BuzzerOn(BEEP_COLIBRATE_START);
			#endif
			LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16DelayTime);	
			u16ScNumbOldBackup = u16DelayTime;
			if(sNewKey.u32Key == KEY_INCREASE)
			{
				if(u16DelayTime < (SYSTEM_SS_ID-0x01))
					u16DelayTime++;
				else
				{
					InterfaceInsertNote(INTERFACE_NOTE_SET_YKQ_ID_OUTSCOPE);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
					ShowMain(&s_u32MainUpdateMethod);				
					return	u32ReturnFlag;
				}
			}
			else
			{
				if(u16DelayTime >= 0x02)
					u16DelayTime--;
				else
				{
					InterfaceInsertNote(INTERFACE_NOTE_SET_YKQ_ID_OUTSCOPE);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
					ShowMain(&s_u32MainUpdateMethod);				
					return u32ReturnFlag;
				}
			}
			u32ReturnFlag = 0x00;
			g_stLogicSem->OSEventCnt = 0;		
			LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16DelayTime);	
			u16ScNumbNewBackup = u16DelayTime;
			u32Y = LOGIC_COLIBRATE_SC_INCREASE_DECREASE;
			LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);
			u32Y = LOGIC_STATUS_IR_COLIBARAE_SUCCESS;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
			InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_START);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
			ShowMain(&s_u32MainUpdateMethod);
			u32ReturnFlag = 0x01;				
			//预估时间，包括了RFID应答时间
			u32Y = 0x00;
			LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
			LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
			u32Y += (u32)(u16DelayTime*u16SendTimes);
//			u32Y += 1000;//1000
			u32Y += 3000;
			g_stLogicSem->OSEventCnt = 0x00;
			u8tmp = getColiScNumStatus();
			u8tmp++;
			setColiScNumStatus(u8tmp);
			OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);
			setColiScNumStatus(0);
			#if 0
				err =OS_ERR_NONE;
				InterfaceSetCtrlMode( INTERFACE_MODE_COLIBRATE_CODE_SUCCESS);
			#endif
			StateLed(ADJUST_CODE_ING_LED,ADJUST_CODE_SUCCEED_LED);	
			if(err == OS_ERR_TIMEOUT)//无应答
			{
//				u32Y = LOGIC_STATUS_IDLE;
//				LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);	
//				#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)				
//					BuzzerOn(BEEP_UNCOLIBRAT);
//				#endif
//				u16DelayTime = RFID_SIGNAL_TOO_SMALL;
//				LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DelayTime);	
//				StateLed(0x00,ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED|KEY_SHIFT_LED);
//				InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_FAIL);//空闲
//				InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
//				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);
//				ShowMain(&s_u32MainUpdateMethod);	
//				KeyLedOnOff(0x00, KEY_LED_ALL);
				s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
				
				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
				u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//先解除对码
				LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
				OSTimeDly(200/TICK_TIME);
				
				//jhyjhy
				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);
				
				/**键值显示**/   //jhy发送停止
				stRfidType = RFID_FRAME_STATUS;
				u64RfidData = 0x60;
				LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
				LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//发送的数据
				g_stLogicSem->OSEventCnt = 0;
				u32Y = SC_BUTTON_ACK_NONE;
				LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//将应答设为无效	
				u64Y = RFID_ACTION_NONE;
				LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u64Y);//将应答动作字设为无效						
				u64Y = (u64)(u64RfidData|(u64stRfidType<<62));
				LogicRunInfApi(LOGIC_SET_BUTTON_VALUE,&u64Y);
				LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
				LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
				u32Y = (u32)(u16DelayTime*u16SendTimes);
				u32Y += 230;			
				
				OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);			
				if(err == OS_ERR_TIMEOUT)//无应答
				{
					InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息	
					ShowMain(&s_u32MainUpdateMethod);						
				}
				else if(err == OS_ERR_NONE)//有应答
				{
					/**关于设置的一些特殊模式切换的处理*/
					LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
					LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u64ButtonValue);
					LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&u64ButtonAckValue);
					if((status == SC_BUTTON_ACK_ACCEPT) && (u64ButtonAckValue == u64ButtonValue))
					{
						InterfaceInsertNote(INTERFACE_NOTE_PLEASE_NEAR);
						#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
							InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
						#else
							InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
						#endif
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
						u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
						LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
						u32Y = 0x00;
						LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);	
						LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32Y);	
						ShowMain(&s_u32MainUpdateMethod);
						ShowStr(0, (const u8 *)"操作失败!", 0x04, 32);
						s_u8pleaseNearFlag = 1;
						OSTimeDly(100/TICK_TIME);
					}
					else if(status == SC_BUTTON_ACK_REJECT)
					{
						InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
						#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
							InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
						#else
							InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
						#endif
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
						u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
						LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
						ShowMain(&s_u32MainUpdateMethod);							
					}				
					else
					{
						InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
						#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
							InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
						#else
							InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
						#endif
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息					
						u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
						LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
						ShowMain(&s_u32MainUpdateMethod);			
					}
				}
				else
					;
				u32ReturnFlag = 0x01;
				
				if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
				ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
				else if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_UNCERTIFIED)
					ShowScUncertifiedKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
				else
					;		
				
				
//        jhyjhy
//				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);	
//				u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//后解除对码
//				LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
								
//				u32Y = LOGIC_COLIBRATE_SC_NONE;
//				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);					
			}
			else if(err == OS_ERR_NONE)//有应答
			{
				/**关于设置的一些特殊模式切换的处理*/
				if(InterfaceGetCtrlMode() == INTERFACE_MODE_COLIBRATE_CODE_FAIL)
				{
					s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
					
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//先解除对码
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
					OSTimeDly(200/TICK_TIME);
					
					//jhyjhy
					LogicRunInfApi(LOGIC_GET_CONLIBRATE_SUCCESS_SC_NUMB,&u16ScNumbOldBackup); 
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);
					
					/**键值显示**/   //jhy发送停止
					stRfidType = RFID_FRAME_STATUS;
					u64RfidData = 0x60;
					LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
					LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//发送的数据
					g_stLogicSem->OSEventCnt = 0;
					u32Y = SC_BUTTON_ACK_NONE;
					LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//将应答设为无效	
					u64Y = RFID_ACTION_NONE;
					LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u64Y);//将应答动作字设为无效						
					u64Y = (u64)(u64RfidData|(u64stRfidType<<62));
					LogicRunInfApi(LOGIC_SET_BUTTON_VALUE,&u64Y);
					LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
					LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
					u32Y = (u32)(u16DelayTime*u16SendTimes);	
					u32Y += 230;			
					
					OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);			
					if(err == OS_ERR_TIMEOUT)//无应答
					{
						InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息	
						ShowMain(&s_u32MainUpdateMethod);						
					}
					else if(err == OS_ERR_NONE)//有应答
					{
						/**关于设置的一些特殊模式切换的处理*/
						LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
						LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u64ButtonValue);
						LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&u64ButtonAckValue);
						if((status == SC_BUTTON_ACK_ACCEPT) && (u64ButtonAckValue == u64ButtonValue))
						{
							InterfaceInsertNote(INTERFACE_NOTE_PLEASE_NEAR);
							#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
								InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
							#else
								InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
							#endif
							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
							u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
							LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
							u32Y = 0x00;
							LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);	
							LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32Y);	
							ShowMain(&s_u32MainUpdateMethod);
							ShowStr(0, (const u8 *)"操作失败!", 0x04, 32);
							s_u8pleaseNearFlag = 1;
							OSTimeDly(100/TICK_TIME);
						}
						else if(status == SC_BUTTON_ACK_REJECT)
						{
							InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
							#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
								InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
							#else
								InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
							#endif
							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
							u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
							LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
							ShowMain(&s_u32MainUpdateMethod);							
						}				
						else
						{
							InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
							#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
								InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
							#else
								InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
							#endif
							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息					
							u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
							LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
							ShowMain(&s_u32MainUpdateMethod);			
						}
					}
					else
						;
					u32ReturnFlag = 0x01;
					
					if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
						ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
					else if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_UNCERTIFIED)
						ShowScUncertifiedKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
					else
						;		
				}
				else if(InterfaceGetCtrlMode() == INTERFACE_MODE_COLIBRATE_CODE_SUCCESS)
				{
					#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
						BuzzerOn(BEEP_KEY_PRESSED);
					#endif
					StateLed(ADJUST_CODE_SUCCEED_LED,ADJUST_CODE_ING_LED);
					/**在使用加减键的情况下，直接进入动作状态***/
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);//本架控制模式	
					
					InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_SUCCEED);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
					ShowMain(&s_u32MainUpdateMethod);	
					LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
					s_u32InterfaceColibrateTimer = u32Y;	
					
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);						
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//先解除对码
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);	
					OSTimeDly(200/TICK_TIME);
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
					LogicRunInfApi(LOGIC_SET_CONLIBRATE_SUCCESS_SC_NUMB,&u16ScNumbNewBackup);
					u32Y = LOGIC_COLIBRATE_SC_IGNORE;
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);					
					
				}
				else if(InterfaceGetCtrlMode() == INTERFACE_MODE_SP)
				{		
					StateLed(ADJUST_CODE_ING_LED,ADJUST_CODE_SUCCEED_LED);					
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);//本架控制模式						
					LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
					s_u32InterfaceColibrateTimer = u32Y;
//					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
//					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//先解除对码
//					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						

//					OSTimeDly(200/TICK_TIME);		  
//					
//					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);
					u32Y = LOGIC_COLIBRATE_SC_IGNORE;
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);
					
					if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_SELF_CONTROL)
					ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
					else if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_UNCERTIFIED)
						ShowScUncertifiedKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
					else
						;		
					u32ReturnFlag = 0x01;
						
				}
				else if(InterfaceGetCtrlMode() == INTERFACE_MODE_RELIEVE_COLIBRATE)
				{
					u32Y = LOGIC_STATUS_IDLE;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);	
					#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)					
						BuzzerOn(BEEP_UNCOLIBRAT);
					#endif
					u16DelayTime = RFID_SIGNAL_TOO_SMALL;
					LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DelayTime);	
					StateLed(0x00,ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED|KEY_SHIFT_LED);
					InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_FAIL);//空闲
					InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);	
					u8J = NO_CTRL;
					LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);
					ShowMain(&s_u32MainUpdateMethod);	
					KeyLedOnOff(0x00, KEY_LED_ALL);
					s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
					
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//先解除对码
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
					OSTimeDly(200/TICK_TIME);
					
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);	
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//后解除对码
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
									
					u32Y = LOGIC_COLIBRATE_SC_NONE;
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);									
				}
				else
					;
			}
			else
				;
			StateLed(0x00, KEY_SHIFT_LED);
			s_u32ShiftKeyEnableFlag = DISABLED;	
			
			if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
				ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
			else if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_UNCERTIFIED)
				ShowScUncertifiedKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
			else
				;	
			#if (HEART == 1)		//心跳帧			
			{			
				Heart.ActionType = SIGNAL_ACTION;
				Heart.SendMode = 0;
			}	
			#endif	
		}		
	#endif
		//停止键长按
		else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_LONGDOWN))
		{
			u32StopKeyFlag = KEY_LONGDOWN;
			InterfaceStopKeyLongDownPro();
		}
		else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_DOWN))
		{
			u32StopKeyFlag = KEY_DOWN;
		}
		//对停止键短按
		else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_UP))
		{
			if(u32StopKeyFlag == KEY_LONGDOWN)/**在任何模式下，均识别关机键*/
			{
				return u32ReturnFlag;			
		
			}
			else if(u32StopKeyFlag == KEY_DOWN)//停止键
			{
				InterfaceKeyShuntDownPro(LOGIC_STATUS_RFID_SC_NEIGHBORL,NULL);		//zzw
			}
			u32StopKeyFlag = KEY_UP;		
		}
		//邻架键
//		#if (LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG != DISABLED)
//			else if(((sNewKey.u32Key == KEY_LEFT_NEIGHBOR) || (sNewKey.u32Key == KEY_RIGHT_NEIGHBOR)) 
//				&& (sNewKey.u32Status == KEY_DOWN || sNewKey.u32Status == KEY_UP))		
//			{
//				if(!InterfaceKeyNeighborPro(sNewKey,LOGIC_STATUS_RFID_SC_NEIGHBORL,INTERFACE_MODE_SC_NEIGHBOR_CONTROL,NULL))
//					return 0;
//			}
//		#endif	
		//按下左右成组键	//成组键改邻架键	zzw2023.12.25
		else if(((sNewKey.u32Key == KEY_LEFT_GRP)|| (sNewKey.u32Key == KEY_RIGHT_GRP))&& ((sNewKey.u32Status == KEY_DOWN)))  
		{
//			InterfaceKeyGrpCtrlPro(sNewKey,LOGIC_STATUS_RFID_SC_NEIGHBORL,INTERFACE_MODE_SC_NEIGHBOR_CONTROL,NULL);
				if(!InterfaceKeyNeighborPro(sNewKey,LOGIC_STATUS_RFID_SC_NEIGHBORL,INTERFACE_MODE_SC_NEIGHBOR_CONTROL,NULL))
					return 0;
		}	
		//按了启动键		
		else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_MENU_START))
		{
			InterfaceGrpActionStartPro();	
			ShowScUncertifiedKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);			
		}
		//动作键按下
		else if(sNewKey.u32Status == KEY_DOWN)		//对剩余按键
		{
			u32ReturnFlag = 0x01;					
			//清除数据		zzw
			ClearActionInfo();				
			InterfaceActionKeyDownPro(sNewKey,LOGIC_STATUS_RFID_SC_NEIGHBORL,INTERFACE_MODE_SC_NEIGHBOR_CONTROL,NULL);
			ShowScUncertifiedKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);
		}
		//动作键抬起
		else if(sNewKey.u32Status == KEY_UP&&((sNewKey.u32Key&KEY_SHIFT) != KEY_SHIFT)&&((sNewKey.u32Key&KEY_CERTIFY) != KEY_CERTIFY)\
			&&((sNewKey.u32Key&KEY_INCREASE) != KEY_INCREASE)&&((sNewKey.u32Key&KEY_DECREASE) != KEY_DECREASE)\
			/*&&(sNewKey.u32Key != KEY_LEFT_NEIGHBOR)&&(sNewKey.u32Key != KEY_RIGHT_NEIGHBOR)\*/
			&&((sNewKey.u32Key&KEY_LEFT_GRP) != KEY_LEFT_GRP)&&((sNewKey.u32Key&KEY_RIGHT_GRP) != KEY_RIGHT_GRP))
		{
			//清除数据		zzw
			ClearActionInfo();			
			InterfaceActionKeyUpPro(sNewKey,LOGIC_STATUS_RFID_SC_NEIGHBORL,INTERFACE_MODE_SC_NEIGHBOR_CONTROL,NULL);
		}		
		else
			;	
		return u32ReturnFlag;
		
	}
#endif
/*******************************************************************************************
**函数名称：InterfaceModeScGrpControlProc
**函数作用：显示层处于成组控制模式下，按键的处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
#if (GRP_CTRL_ENABLE_FLAG != DISABLED)
	static u32 InterfaceModeScGrpControlProc(stKEY	sNewKey)
	{
		u32 u32ReturnFlag = 0x00;
		INT8U err;
		static u64 u64ButtonValue;
		static u64 u64ButtonAckValue;
		static u64 u64stRfidType;
		SC_BUTTON_TYPE status;
		u32 u32Y = 0x00;
		static u32 u32StopKeyFlag = KEY_UP;//
		static u64 u64Y;
		static u64 u64RfidData;
		RFID_FRAME_TYPE stRfidType;
		u16 u16DelayTime = 0x00;
		u16 u16SendTimes = 0x00;
		u8 u8J = 0xff;	//zzw
		u16 u16GrpRemoSc = 0;
		u16 u16GrpNearSc = 0;
		
		u64Y = 0x00;
		u64RfidData = RFID_ACTION_NONE;
		s_u32AllKeyStatus = sNewKey.u32Status;
		LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
		s_u32InterfaceColibrateTimer = u32Y;		
		InterfaceSetCtrlMode(INTERFACE_MODE_SC_GRP_CONTROL);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_AWAKE);
		if((sNewKey.u32Key&KEY_STOP_SHUTDOWN) == KEY_STOP_SHUTDOWN)
		{	
			sNewKey.u32Key = KEY_STOP_SHUTDOWN;
			s_u32ShiftKeyEnableFlag = DISABLED;
		}
		//shift键短按
		#if (SHIFT_MULTIPLE_CTRL_ENABLE_FLAG != DISABLED)
			if((sNewKey.u32Key == KEY_SHIFT)&& (sNewKey.u32Status == KEY_DOWN))
			{
				if(s_u32ShiftKeyEnableFlag == DISABLED)
				{
					StateLed(KEY_SHIFT_LED,0x00);
					u32ReturnFlag = 0x01;
					s_u32ShiftKeyEnableFlag = ENABLED;
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_SHIFT_DOWN);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
					ShowMain(&s_u32MainUpdateMethod);			
				}
				else
				{
					StateLed(0x00,KEY_SHIFT_LED);
					u32ReturnFlag = 0x01;
					s_u32ShiftKeyEnableFlag = DISABLED;	
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_SHIFT_UP);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
					ShowMain(&s_u32MainUpdateMethod);			
				}
				ShowScGrpKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);		
			}	
			else if((sNewKey.u32Key == KEY_SHIFT)&& (sNewKey.u32Status == KEY_UP))
			{

			}	
		#else
			if((sNewKey.u32Key == KEY_SHIFT)&& (sNewKey.u32Status == KEY_DOWN))
			{
				s_u32ShiftKeyEnableFlag = ENABLED;
			}
			else if((sNewKey.u32Key == KEY_SHIFT)&&(s_u32ShiftKeyEnableFlag == ENABLED)&&(sNewKey.u32Status == KEY_UP))
			{
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
				ShowMain(&s_u32MainUpdateMethod);		
				s_u32ShiftKeyEnableFlag = DISABLED;
			}
		#endif
		//zzw		确认键
		else if((sNewKey.u32Key == KEY_CERTIFY)&& (sNewKey.u32Status == KEY_DOWN || sNewKey.u32Status == KEY_UP))		//对确认键做特殊处理
		{
			u32ReturnFlag = InterfaceKeyCertifyPro(sNewKey,LOGIC_STATUS_RFID_SC_GRP,INTERFACE_MODE_SC_GRP_CONTROL,NULL);
		}	
		/**识别解除对码键(长按SHIFT，解除对码)**/
		else if((sNewKey.u32Key == KEY_SHIFT)&&(sNewKey.u32Status == KEY_LONGDOWN))		
		{
			InterfaceShiftKeyLongDownPro();
			u32ReturnFlag = 0x01;		
		}	
		//停止键长按
		else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_LONGDOWN))
		{
			u32StopKeyFlag = KEY_LONGDOWN;
			InterfaceStopKeyLongDownPro();
		}
		else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_DOWN))
		{
			#if (GRP_LALIU_BUTTONUP_STOP_FLAG != DISABLED)
				s_u32GrpLaliuKeyFlag = KEY_UP;
			#endif			
			u32StopKeyFlag = KEY_DOWN;
		}	
		else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_UP))//对停止键做特殊处理
		{
			if(u32StopKeyFlag == KEY_LONGDOWN)/**在任何模式下，均识别关机键*/
			{
				return u32ReturnFlag;				
			}
			else if(u32StopKeyFlag == KEY_DOWN)//停止键
			{
				InterfaceKeyShuntDownPro(LOGIC_STATUS_RFID_SC_GRP,NULL);		//zzw
			}
			u32StopKeyFlag = KEY_UP;			
		}	
		/**用于换方向**/
		else if(((sNewKey.u32Key == KEY_LEFT_NEIGHBOR) || (sNewKey.u32Key == KEY_RIGHT_NEIGHBOR)) && (sNewKey.u32Status == KEY_DOWN))		//邻架键	
		{
			if(!InterfaceKeyNeighborPro(sNewKey,LOGIC_STATUS_RFID_SC_GRP,INTERFACE_MODE_SC_GRP_CONTROL,NULL))
					return 0;
			u32ReturnFlag = 0x01;					
		}
		//成组范围调整
		else if(((sNewKey.u32Key == KEY_LEFT_GRP)|| (sNewKey.u32Key == KEY_RIGHT_GRP)) && ((sNewKey.u32Status == KEY_DOWN)))
		{
			InterfaceKeyGrpCtrlPro(sNewKey,LOGIC_STATUS_RFID_SC_GRP,INTERFACE_MODE_SC_GRP_CONTROL,NULL);	
			u32ReturnFlag = 0x01;			
		}	
		//动作键按下
		else if((sNewKey.u32Status == KEY_DOWN)
				&& (((sNewKey.u32Key&KEY_LEFT_GRP) != KEY_LEFT_GRP) && ((sNewKey.u32Key&KEY_RIGHT_GRP) != KEY_RIGHT_GRP)
				&& ((sNewKey.u32Key&KEY_LEFT_NEIGHBOR) != KEY_LEFT_NEIGHBOR) && ((sNewKey.u32Key&KEY_RIGHT_NEIGHBOR) != KEY_RIGHT_NEIGHBOR)
				&& ((sNewKey.u32Key&KEY_CERTIFY) != KEY_CERTIFY) && ((sNewKey.u32Key&KEY_SHIFT) != KEY_SHIFT) 
				&& ((sNewKey.u32Key&KEY_MENU_START) != KEY_MENU_START)))//成组动作控制中，不识别长按
		{
			InterfaceActionKeyDownPro(sNewKey,LOGIC_STATUS_RFID_SC_GRP,INTERFACE_MODE_SC_GRP_CONTROL,NULL);

			u32ReturnFlag = 0x01;	
		}
		else if((sNewKey.u32Status == KEY_UP)&&(sNewKey.u32Key != KEY_SHIFT))
		{
			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
				BuzzerOff();
			#endif
//			if(RfidGetDataLimitlessSendFlag())
//			{
//				RfidSetDataLimitlessSendEnd();
//			}		
			
			if(sNewKey.u32Key == KEY_MENU_START)		//按了启动键
			{
				InterfaceGrpActionStartPro();			
			}
			else
			{
				if((sNewKey.u32Key != KEY_LEFT_GRP)&&(sNewKey.u32Key != KEY_RIGHT_GRP))
				{
					InterfaceInsertNote(INTERFACE_NOTE_START_ACTION);	
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_GRP_CONTROL);
					ShowMain(&s_u32MainUpdateMethod);
				}
			}
			if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_GRP_CONTROL)
				ShowScGrpKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);		
			else if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_SELF_CONTROL)
				ShowScSelfKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);	
			else if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
				ShowScSelfKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);				
			else if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_UNCERTIFIED)
				ShowScUncertifiedKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);	
			else
				;
		}
		else
			;
		
		return u32ReturnFlag;
	}
#endif

/*******************************************************************************************
**函数名称：InterfaceModeYkqSelfControlProc
**函数作用：显示层处于控制其他遥控器模式下,按键的处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
#if (SET_YKQ_PRGPT_USE_FLAG != DISABLED)
	static u32 InterfaceModeYkqSelfControlProc(stKEY	sNewKey)
	{
		u32 u32ReturnFlag = 0x00;
		INT8U err;
		u32 u32ButtonValue;
		u32 u32ButtonAckValue;
		SC_BUTTON_TYPE status;
		u32 u32Y = 0x00;
		u32 u32RfidData = RFID_ACTION_NONE;
		RFID_FRAME_TYPE stRfidType;
		static u32 u32StopKeyFlag = KEY_UP;//
		u16 u16DelayTime = 0x00;
		u16 u16SendTimes = 0x00;

		s_u32AllKeyStatus = sNewKey.u32Status;
		InterfaceSetCtrlMode(INTERFACE_MODE_YKQ_SELF_CONTROL);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_AWAKE);
		u32RfidData = InterfaceGetKeyRfidDataShowName(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag,sNewKey.u32Key,&(s_stInterfaceActionName[0x00]),&stRfidType);
		/*按键无效时的处理**/
		if(u32RfidData == RFID_ACTION_NONE)
		{
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
			ShowMain(&s_u32MainUpdateMethod);
			return u32ReturnFlag;
		}
		

		/**识别解除对码键(长按SHIFT，解除对码)**/
		if((sNewKey.u32Key == KEY_CERTIFY)&&(sNewKey.u32Status == KEY_LONGDOWN))
		{
			BuzzerOn(BEEP_UNCOLIBRAT);
			s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
			InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
			InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_IDLE);
			StateLed(0x00,ADJUST_CODE_ING_LED);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);				
			ShowMain(&s_u32MainUpdateMethod);
			KeyLedOnOff(0x00, KEY_LED_ALL);
			u32ReturnFlag = 0x01;	
			ParEditSetSearchDevFlagProc(DISABLED);
			if(g_u32ID == B_type)
			{
				IrUsartChipPwrOff();
			}
			else
			{
				GPIO_SetBits(GPIOC,GPIO_Pin_6);
			}
			u32Y = LOGIC_STATUS_IDLE;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);				
		}	
		else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_LONGDOWN))
		{
			u32StopKeyFlag = KEY_LONGDOWN;
			BuzzerOn(BEEP_SHUTDOWN_NOTE);
			OSTaskSuspend(SCAN_KEY_TASK_PRIO);//挂起按键扫描任务
			s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
			InterfaceInsertNote(INTERFACE_NOTE_SHUTDOWN);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
			ShowMain(&s_u32MainUpdateMethod);
				
			u32Y = LOGIC_STATUS_SHUTUP;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);			
			OSTimeDly(1000/TICK_TIME);
			u32ReturnFlag = 0x01;		
		}
		else if((sNewKey.u32Key == KEY_MENU_START)&& ((sNewKey.u32Status == KEY_DOWN)))
		{
			InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);		
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
			ShowMain(&s_u32MainUpdateMethod);
			u32Y = LOGIC_STATUS_IR_SEARCH_YQK_ING;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
			/**设置特殊标记*****/
			ParEditSetSearchDevFlagProc(ENABLED);
			s_u32SelfSetIntfState = INTERFACEMENU_STATE_MENU_START;
			ParEdit(sNewKey,&s_u32SelfSetIntfState);
			InterfaceSetCtrlMode(INTERFACE_MODE_YKQ_SET_CONTROL);
		}		
		else if((sNewKey.u32Status == KEY_UP)&&(sNewKey.u32Key != KEY_CERTIFY))
		{
			for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
				s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示		
			InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_YKQ_SELF_CONTROL);//显示主屏所有信息
			ShowMain(&s_u32MainUpdateMethod);				
		}
		else
			;
		
		return u32ReturnFlag;

	}
#endif

/*******************************************************************************************
**函数名称：InterfaceModeShutdownProc
**函数作用：显示层处于关机模式下,按键的处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
#if (SET_YKQ_PRGPT_USE_FLAG != DISABLED)
	static u32 InterfaceModeYkqSetControlProc(stKEY	sNewKey)
	{
		u32 u32ReturnFlag = 0x00;
		INT8U err;
		u32 u32ButtonValue;
		u32 u32ButtonAckValue;
		SC_BUTTON_TYPE status;
		u32 u32Y = 0x00;
		u32 u32RfidData = RFID_ACTION_NONE;
		RFID_FRAME_TYPE stRfidType;
		static u32 u32StopKeyFlag = KEY_UP;//
		u16 u16DelayTime = 0x00;
		u16 u16SendTimes = 0x00;

		s_u32AllKeyStatus = sNewKey.u32Status;
		InterfaceSetCtrlMode(INTERFACE_MODE_YKQ_SET_CONTROL);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_AWAKE);
		u32RfidData = InterfaceGetKeyRfidDataShowName(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag,sNewKey.u32Key,&(s_stInterfaceActionName[0x00]),&stRfidType);
		/*按键无效时的处理**/
		if(u32RfidData == RFID_ACTION_NONE)
		{
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
			ShowMain(&s_u32MainUpdateMethod);
			return u32ReturnFlag;
		}
		/**识别解除对码键(长按SHIFT，解除对码)**/
		if((sNewKey.u32Key == KEY_CERTIFY)&&(sNewKey.u32Status == KEY_LONGDOWN))
		{
			BuzzerOn(BEEP_UNCOLIBRAT);
			s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
			InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
			InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_IDLE);
			StateLed(0x00,ADJUST_CODE_ING_LED);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);				
			ShowMain(&s_u32MainUpdateMethod);
			KeyLedOnOff(0x00, KEY_LED_ALL);
			u32ReturnFlag = 0x01;		
			ParEditSetSearchDevFlagProc(DISABLED);
			if(g_u32ID == B_type)
			{
				IrUsartChipPwrOff();
			}
			else
			{
				GPIO_SetBits(GPIOC,GPIO_Pin_6);
			}
			u32Y = LOGIC_STATUS_IDLE;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);				
		}		
		else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_LONGDOWN))
		{
			u32StopKeyFlag = KEY_LONGDOWN;
// 			BuzzerOn(BEEP_SHUTDOWN_NOTE);
			OSTaskSuspend(SCAN_KEY_TASK_PRIO);//挂起按键扫描任务
			s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
			InterfaceInsertNote(INTERFACE_NOTE_SHUTDOWN);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
			ShowMain(&s_u32MainUpdateMethod);
			u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);				
			OSTimeDly(200/TICK_TIME);
			
			u32Y = LOGIC_STATUS_SHUTUP;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);			
			OSTimeDly(1000/TICK_TIME);
			u32ReturnFlag = 0x01;		
		}
		else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_DOWN))
		{
			InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);		
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);
			InterfaceSetCtrlMode(INTERFACE_MODE_YKQ_SELF_CONTROL);
			ShowMain(&s_u32MainUpdateMethod);
			u32Y = LOGIC_STATUS_IR_SEARCH_YQK_ING;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
		}	
		else if((sNewKey.u32Status == KEY_DOWN))
		{
			if(ParEdit(sNewKey,&s_u32SelfSetIntfState))
			{
				InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_YKQ_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);
				InterfaceSetCtrlMode(INTERFACE_MODE_YKQ_SELF_CONTROL);
				ShowMain(&s_u32MainUpdateMethod);			
				u32ReturnFlag = 0x01;
			} 	
		}
		else
			;
		
		return u32ReturnFlag;

	}
#endif	

/*******************************************************************************************
**函数名称：InterfaceModeScZibaoControlProc
**函数作用：显示层处于本架自保模式下,按键的处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
#if ((ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)&&(ZIBAO_CTRL_METHOD_TYPE == ZIBAO_MANUAL_CTRL_METHOD))
	static u32 InterfaceModeScZibaoControlProc(stKEY	sNewKey)
	{
		u32 u32ReturnFlag = 0x00;
		INT8U err;
		u32 u32ButtonValue;
		static u64 u64ButtonValue;
		static u64 u64ButtonAckValue;
		static u64 u64stRfidType;
		SC_BUTTON_TYPE status;
		u32 u32Y = 0x00;		
		static u64 u64Y;
		static u64 u64RfidData;
		RFID_FRAME_TYPE stRfidType;
		u16 u16DelayTime = 0x00;
		u16 u16SendTimes = 0x00;
		static INTERFACE_ACTION_TYPE temp[INTERFACE_KEY_TOGETHER_MAX] ;

		u64Y = 0x00;		
		u64RfidData = RFID_ACTION_NONE;
		memset(temp,INTERFACE_ACTION_NONE,INTERFACE_KEY_TOGETHER_MAX);
		
		s_u32AllKeyStatus = sNewKey.u32Status;
		LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
		s_u32InterfaceColibrateTimer = u32Y;		
		InterfaceSetCtrlMode(INTERFACE_MODE_SC_ZIBAO_CONTROL);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_AWAKE);

		if(((sNewKey.u32Status == KEY_UP) && ((sNewKey.u32Key&KEY_ZI_BAOCHI) == KEY_ZI_BAOCHI))\
			||((sNewKey.u32Status == KEY_DOWN)&&((sNewKey.u32Key&KEY_STOP_SHUTDOWN) == KEY_STOP_SHUTDOWN)))
		{
			if(RfidGetDataLimitlessSendFlag())
			{
				RfidSetDataLimitlessSendEnd();
				stRfidType = RFID_FRAME_ACTION;;
				LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
				LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//发送的数据			
				LogicRunInfApi( LOGIC_SET_BUTTON_UP,&s_u64InterfaceRfidDataBackup); 			
			}	
			u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);				
			InterfaceInsertNote(INTERFACE_NOTE_ZIBAO_END);
			for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
				s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示	
			InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);	
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
			ShowMain(&s_u32MainUpdateMethod);	

			s_u64InterfaceRfidDataBackup = 0x00;
			s_u32ZibaoActionKeyBackup= 0x00;
			ShowScSelfKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);
			StateLed(ADJUST_CODE_SUCCEED_LED, 0x00);		
			
			return u32ReturnFlag;
		}
		else if((sNewKey.u32Status == KEY_DOWN) && (sNewKey.u32Key == KEY_ZI_BAOCHI))
		{
			ShowScZibaoKeyLedProc(s_u32ZibaoActionKeyBackup, KEY_DOWN, s_u32ShiftKeyEnableFlag);
			return u32ReturnFlag;
		}
		else if((sNewKey.u32Status == KEY_LONGDOWN)&& (sNewKey.u32Key == KEY_ZI_BAOCHI))
		{
			ShowScZibaoKeyLedProc(s_u32ZibaoActionKeyBackup, KEY_DOWN, s_u32ShiftKeyEnableFlag);			
			return u32ReturnFlag;
		}
		else if(((sNewKey.u32Key&KEY_STOP_SHUTDOWN) == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_LONGDOWN))
		{
			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
				BuzzerOn(BEEP_SHUTDOWN_NOTE);
			#endif
			OSTaskSuspend(SCAN_KEY_TASK_PRIO);//挂起按键扫描任务
			s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
			InterfaceInsertNote(INTERFACE_NOTE_SHUTDOWN);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
			ShowMain(&s_u32MainUpdateMethod);
			#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
				KeyLedOnOff(0x00,KEY_LED_ALL);
			#endif				
			u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);				
			OSTimeDly(200/TICK_TIME);
			
			u32Y = LOGIC_STATUS_SHUTUP;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);			
			OSTimeDly(1000/TICK_TIME);
		}		
		else 
		{
			if((sNewKey.u32Status == KEY_DOWN)&&((sNewKey.u32Key&KEY_ZI_BAOCHI) != KEY_ZI_BAOCHI)&&(((sNewKey.u32Key&KEY_STOP_SHUTDOWN) != KEY_STOP_SHUTDOWN)))
			{
				if(RfidGetDataLimitlessSendFlag())
				{
					RfidSetDataLimitlessSendEnd();
					stRfidType = RFID_FRAME_ACTION;;
					LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
					LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//发送的数据			
					LogicRunInfApi( LOGIC_SET_BUTTON_UP,&s_u64InterfaceRfidDataBackup); 			
				}	
				u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
				LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);				
				InterfaceInsertNote(INTERFACE_NOTE_ZIBAO_END);
				for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示	
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);	
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
				ShowMain(&s_u32MainUpdateMethod);	

				s_u64InterfaceRfidDataBackup = 0x00;
				s_u32ZibaoActionKeyBackup= 0x00;
				ShowScSelfKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);
				return u32ReturnFlag;
			}
		}

		
		sNewKey.u32Key &= ~KEY_ZI_BAOCHI;
		u64RfidData = InterfaceGetKeyRfidDataShowName(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag,sNewKey.u32Key,&(temp[0x00]),&stRfidType);
		u64stRfidType = (u64)stRfidType;
		if(u64RfidData != 0x00)
		{
			if((sNewKey.u32Status == KEY_DOWN))
			{
				if((s_u32ZibaoActionKeyBackup&sNewKey.u32Key) == sNewKey.u32Key)
				{
					s_u32ZibaoActionKeyBackup &= ~(sNewKey.u32Key);							
				}
				else
				{
					s_u32ZibaoActionKeyBackup |= sNewKey.u32Key;
				}
			}
			InterfaceTakeOutReverseKey(InterfaceGetCtrlMode(),&s_u32ZibaoActionKeyBackup);
			u64RfidData = InterfaceGetKeyRfidDataShowName(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag,s_u32ZibaoActionKeyBackup,&(temp[0x00]),&stRfidType);			
			u64stRfidType = (u64)stRfidType;
		}
		else
		{
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
			ShowMain(&s_u32MainUpdateMethod);
			if(s_u32ZibaoActionKeyBackup !=  0x00)		
			{
				u32ButtonValue = 0x00;
 				u32Y = KeyCorrespendLed(sNewKey.u32Key,&u32ButtonValue);
				u64ButtonValue = (u64)u32ButtonValue;
				if(u32Y != 0x00)
					KeyLedOnOff(0x00,u32ButtonValue);		
			}
			else
				ShowScZibaoKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);
			return u32ReturnFlag;
		}

		/*按键无效时的处理**/
		if((u64RfidData == RFID_ACTION_NONE))
		{
			if(s_u64InterfaceRfidDataBackup != 0x00)
			{
				if(RfidGetDataLimitlessSendFlag())
				{
					RfidSetDataLimitlessSendEnd();	
					stRfidType = RFID_FRAME_ACTION;						
					LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
					LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//发送的数据		
					u64Y = (u64)(s_u64InterfaceRfidDataBackup|(u64stRfidType<<62));
					LogicRunInfApi( LOGIC_SET_BUTTON_UP,&u64Y); 
				}				
			}
			for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
				s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示	
			if(s_u32ZibaoActionKeyBackup != 0x00)
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_ZIBAO_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
			ShowMain(&s_u32MainUpdateMethod);
			s_u32ZibaoActionKeyBackup = 0x00;
			s_u64InterfaceRfidDataBackup = 0x00;
			ShowScZibaoKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);		
			return u32ReturnFlag;		
		}
		else
		{		
			
//			u32ReturnFlag = u64RfidData;
//			u32ReturnFlag &= ~(stRfidType<<30);
			
			u64stRfidType = (u64)u64RfidData;
			u64stRfidType &= ~(stRfidType<<62);
			for(err = 0x00,u16DelayTime = 0x00;err < 56;err++)	
			{
				if((u64stRfidType&0x01) == 0x01)
					u16DelayTime++;
				u64stRfidType >>= 1;				
			}
			LogicParamApi(LOGIC_GET_ZIBAO_ACTION_MAX, &u16SendTimes);
			if(u16DelayTime > u16SendTimes)
			{
				if((sNewKey.u32Status == KEY_DOWN)||(sNewKey.u32Status == KEY_LONGDOWN))
					InterfaceInsertNote(INTERFACE_NOTE_ZIBAO_TROPPO);
				else
					InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
				ShowMain(&s_u32MainUpdateMethod);
				u32ReturnFlag = 0x00;
				s_u32ZibaoActionKeyBackup &= ~(sNewKey.u32Key);									
				return u32ReturnFlag;		
			}
		}
		
		if((sNewKey.u32Status == KEY_DOWN)||(sNewKey.u32Status == KEY_LONGDOWN))
		{
			if(u64RfidData < s_u64InterfaceRfidDataBackup )//第二次按下，需停止
			{
				for(err = 0x00;err < INTERFACE_KEY_TOGETHER_MAX;err++)
				{
					if(s_stInterfaceActionName[err] == INTERFACE_ACTION_NONE)
						break;
					else
					{
						for(u16DelayTime=0x00;u16DelayTime < INTERFACE_KEY_TOGETHER_MAX;u16DelayTime++)
						{
							if(temp[u16DelayTime] == s_stInterfaceActionName[err])
								break;
						}
						if(u16DelayTime == INTERFACE_KEY_TOGETHER_MAX)
						{
							for(u16DelayTime=err;u16DelayTime < (INTERFACE_KEY_TOGETHER_MAX-0x01);u16DelayTime++)
							{
								s_stInterfaceActionName[u16DelayTime] = s_stInterfaceActionName[u16DelayTime+0x01];
								if(s_stInterfaceActionName[u16DelayTime+0x01] == INTERFACE_ACTION_NONE)
									break;
							}						
						}
					}
				}				
			}
			else
			{
				for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
				{
					if(temp[err] == INTERFACE_ACTION_NONE)
					{
						break;
					}
					else
					{
						for(u32Y = 0x00; u32Y < INTERFACE_KEY_TOGETHER_MAX;u32Y++)
						{
							if(s_stInterfaceActionName[u32Y] == temp[err])
								break;
							if(s_stInterfaceActionName[u32Y] == INTERFACE_ACTION_NONE)
							{
								s_stInterfaceActionName[u32Y] = temp[err];
								break;
							}
						}
					}
				}				
			}
			

		}
		else
		{
			return u32ReturnFlag;		
		}

		if(u64RfidData&&(sNewKey.u32Status == KEY_DOWN))
		{
			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
				BuzzerOn(BEEP_ACTION);
			#endif
			/**键值显示**/
			LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
			LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//发送的数据
			g_stLogicSem->OSEventCnt = 0;
			u32Y = SC_BUTTON_ACK_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//将应答设为无效	
			u64Y = RFID_ACTION_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u64Y);//将应答动作字设为无效			
			u64Y = (u64)(u64RfidData|(u64stRfidType<<62));
			LogicRunInfApi(LOGIC_SET_BUTTON_VALUE,&u64Y);
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_DOWN);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_ZIBAO_CONTROL|INTERFACE_SHOW_MAIN_NOTE);
			ShowMain(&s_u32MainUpdateMethod);
			LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
			LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
			u32Y = (u32)(u16DelayTime*u16SendTimes);	
			u32Y += 30;		
			
			OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
			if(err == OS_ERR_TIMEOUT)//无应答
			{
				InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_ZIBAO_CONTROL);	
				for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_ZIBAO_CONTROL);//显示主屏所有信息
				ShowMain(&s_u32MainUpdateMethod);	
				s_u64InterfaceRfidDataBackup = 0x00;
				s_u32ZibaoActionKeyBackup= 0x00;
				ShowScZibaoKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);
			}
			else if(err == OS_ERR_NONE)//有应答
			{
				/**关于设置的一些特殊模式切换的处理*/
				LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
				LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u64ButtonValue);
				LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&u64ButtonAckValue);
				if((status == SC_BUTTON_ACK_ACCEPT) && (u64ButtonAckValue == u64RfidData))
				{
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_ZIBAO_CONTROL);
					LogicRunInfApi(LOGIC_SET_BUTTON_LONG_VALUE,&u64RfidData);					
					u32Y = LOGIC_STATUS_RFID_SC_ZIBAO_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
					ShowMain(&s_u32MainUpdateMethod);
					ShowScZibaoKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);
					ShowScZibaoKeyLedProc(s_u32ZibaoActionKeyBackup, KEY_DOWN, s_u32ShiftKeyEnableFlag);
				}
				else if(status == SC_BUTTON_ACK_REJECT)
				{
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_ZIBAO_CONTROL);
					u32Y = LOGIC_STATUS_RFID_SC_ZIBAO_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
					for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
						s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_ZIBAO_CONTROL);//显示主屏所有信息
					ShowMain(&s_u32MainUpdateMethod);	
					s_u64InterfaceRfidDataBackup = 0x00;
					s_u32ZibaoActionKeyBackup= 0x00;
					ShowScZibaoKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);		
				}
				else
				{
					InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_ZIBAO_CONTROL);	
					u32Y = LOGIC_STATUS_RFID_SC_ZIBAO_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
					for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
						s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_ZIBAO_CONTROL);//显示主屏所有信息					
					ShowMain(&s_u32MainUpdateMethod);	
					s_u64InterfaceRfidDataBackup = 0x00;
					s_u32ZibaoActionKeyBackup= 0x00;
					ShowScZibaoKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);	
				}
			}
			else	
				;
			
			u32ReturnFlag = 0x01;					
			
			s_u64InterfaceRfidDataBackup = (u64RfidData|(u64stRfidType<<62));
		}
		else if(u64RfidData&&(sNewKey.u32Status == KEY_LONGDOWN))
		{
			ShowScZibaoKeyLedProc(s_u32ZibaoActionKeyBackup, KEY_DOWN, s_u32ShiftKeyEnableFlag);
		}
		else
			;
		
		return 0x01;
	}
/*******************************************************************************************
**函数名称：InterfaceModeScZibaoControlProc
**函数作用：显示层处于本架自保模式下,按键的处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
#elif ((ZIBAO_CTRL_ENABLE_FLAG != DISABLED)&&(ZIBAO_CTRL_METHOD_TYPE == ZIBAO_PARAM_CTRL_METHOD))
	static u32 InterfaceModeScZibaoControlProc(stKEY	sNewKey)	
	{
		u32 u32ReturnFlag = 0x00;
		INT8U err;
		u32 u32ButtonValue;
		u32 u32ButtonAckValue;
		SC_BUTTON_TYPE status;
		u32 u32Y = 0x00;
		u32 u32RfidData = RFID_ACTION_NONE;
		RFID_FRAME_TYPE stRfidType;
		u16 u16DelayTime = 0x00;
		u16 u16SendTimes = 0x00;
		INTERFACE_ACTION_TYPE temp[INTERFACE_KEY_TOGETHER_MAX] ;
		
		s_u32AllKeyStatus = sNewKey.u32Status;
		LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
		s_u32InterfaceColibrateTimer = u32Y;		
		/**在自保按键按下时，开始计时，抬起和长按无效。其他键均有效***/
		if(sNewKey.u32Key == KEY_ZI_BAOCHI)
		{	
			if(sNewKey.u32Status == KEY_DOWN)
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_ZIBAO_CONTROL);
		}
		else
		{
			InterfaceSetCtrlMode(INTERFACE_MODE_SC_ZIBAO_CONTROL);
		}
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_AWAKE);

		if(((sNewKey.u32Status == KEY_DOWN) && ((sNewKey.u32Key&KEY_ZI_BAOCHI) == KEY_ZI_BAOCHI))\
			||((sNewKey.u32Status == KEY_DOWN)&&((sNewKey.u32Key&KEY_STOP_SHUTDOWN) == KEY_STOP_SHUTDOWN)))
		{
			if(RfidGetDataLimitlessSendFlag())
			{
				RfidSetDataLimitlessSendEnd();
				stRfidType = RFID_FRAME_ACTION;;
				LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
				LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//发送的数据			
				LogicRunInfApi( LOGIC_SET_BUTTON_UP,&s_u64InterfaceRfidDataBackup); 			
			}	
			u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);					
			InterfaceInsertNote(INTERFACE_NOTE_ZIBAO_END);
			for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
				s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示	
			InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);	
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
			ShowMain(&s_u32MainUpdateMethod);	

			s_u64InterfaceRfidDataBackup = 0x00;
			s_u32ZibaoActionKeyBackup= 0x00;
			#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
				KeyLedOnOff(0x00,KEY_LED_ALL);
			#endif			
			StateLed(ADJUST_CODE_SUCCEED_LED, 0x00);		
			ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
			
			return u32ReturnFlag;
		}
		else if((sNewKey.u32Status == KEY_UP) && (sNewKey.u32Key == KEY_ZI_BAOCHI))
		{
			return u32ReturnFlag;
		}
		else if((sNewKey.u32Status == KEY_LONGDOWN)&& (sNewKey.u32Key == KEY_ZI_BAOCHI))
		{
			return u32ReturnFlag;
		}
		else if(((sNewKey.u32Key&KEY_STOP_SHUTDOWN) == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_LONGDOWN))
		{
			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
				BuzzerOn(BEEP_SHUTDOWN_NOTE);
			#endif
			if(RfidGetDataLimitlessSendFlag())
			{
				RfidSetDataLimitlessSendEnd();
				stRfidType = RFID_FRAME_ACTION;;
				LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
				LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//发送的数据			
				LogicRunInfApi( LOGIC_SET_BUTTON_UP,&s_u64InterfaceRfidDataBackup); 			
			}	
			#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
				KeyLedOnOff(0x00,KEY_LED_ALL);
			#endif			
			InterfaceInsertNote(INTERFACE_NOTE_ZIBAO_END);
			for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
				s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示	
			InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);	
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
			ShowMain(&s_u32MainUpdateMethod);	
			#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
				ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
			#endif				
			
			s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
			InterfaceInsertNote(INTERFACE_NOTE_SHUTDOWN);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
			ShowMain(&s_u32MainUpdateMethod);
			u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);				
			OSTimeDly(200/TICK_TIME);
			
			u32Y = LOGIC_STATUS_SHUTUP;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);			
			OSTimeDly(1000/TICK_TIME);
		}		
		else if(sNewKey.u32Status == KEY_UP)
		{
			if(s_u32ZibaoActionKeyBackup != 0x00)
				ShowScZibaoKeyLedProc(s_u32ZibaoActionKeyBackup,KEY_DOWN,s_u32ShiftKeyEnableFlag);
			else
				ShowScZibaoKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);			
			return 	u32ReturnFlag;			
		}
		else
			;

		sNewKey.u32Key &= ~KEY_ZI_BAOCHI;
		u32RfidData = InterfaceGetKeyRfidDataShowName(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag,sNewKey.u32Key,&(temp[0x00]),&stRfidType);
		if((u32RfidData != 0x00)&&(stRfidType == RFID_FRAME_ACTION)\
			&&(u32RfidData >= RFID_ACTION_SHENGZHU)&&(u32RfidData <= RFID_ACTION_QIANLIANG_XIABAI))
		{
			/**判断此动作是否允许**/
			u32ReturnFlag = u32RfidData;
			u32ReturnFlag &= 0x3FFFFFFF;
			for(u16DelayTime = 0x00;u16DelayTime < 32; u16DelayTime++)
			{	
				u32Y = u32ReturnFlag;
				u32Y &= (1<<u16DelayTime);
				if(u32Y != 0x00)
				{
					break;
				}					
			}
			u32ReturnFlag = 0x00;
			if(u16DelayTime < 32)
			{
				LogicParamApi((LOGIC_PARAM_MSG_TYPE)(LOGIC_GET_SHENGZHU_ENABLE_FLAG+u16DelayTime), &u32ReturnFlag);
			}
			if(u32ReturnFlag == DISABLED)
			{
				InterfaceInsertNote(INTERFACE_NOTE_ZIBAO_ACTION_DISABLED);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
				ShowMain(&s_u32MainUpdateMethod);					
				return u32ReturnFlag;
			}
			if((sNewKey.u32Status == KEY_DOWN))
			{
				if((s_u32ZibaoActionKeyBackup&sNewKey.u32Key) == sNewKey.u32Key)
				{
					s_u32ZibaoActionKeyBackup &= ~(sNewKey.u32Key);							
				}
				else
				{
					s_u32ZibaoActionKeyBackup |= sNewKey.u32Key;
				}
			}
			InterfaceTakeOutReverseKey(InterfaceGetCtrlMode(),&s_u32ZibaoActionKeyBackup);
			u32RfidData = InterfaceGetKeyRfidDataShowName(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag,s_u32ZibaoActionKeyBackup,&(temp[0x00]),&stRfidType);			
		}
		else
		{
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
			ShowMain(&s_u32MainUpdateMethod);				
			return u32ReturnFlag;
		}

		/*按键无效时的处理**/
		if((u32RfidData == RFID_ACTION_NONE))
		{
			if(s_u64InterfaceRfidDataBackup != 0x00)
			{
				if(RfidGetDataLimitlessSendFlag())
				{
					RfidSetDataLimitlessSendEnd();	
					stRfidType = RFID_FRAME_ACTION;						
					LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
					LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//发送的数据		
					u32Y = (u32)(s_u64InterfaceRfidDataBackup|(stRfidType<<30));
					LogicRunInfApi( LOGIC_SET_BUTTON_UP,&u32Y); 
				}				
			}
			for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
				s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示	
			if(s_u32ZibaoActionKeyBackup != 0x00)
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);
			else
				InterfaceInsertNote(INTERFACE_NOTE_ZIBAO_ACTION_STOP);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_ZIBAO_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
			ShowMain(&s_u32MainUpdateMethod);
			s_u32ZibaoActionKeyBackup = 0x00;
			s_u64InterfaceRfidDataBackup = 0x00;
			ShowScZibaoKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);			
			return u32ReturnFlag;		
		}
		else
		{
			u32ReturnFlag = u32RfidData;
			u32ReturnFlag &= ~(stRfidType<<30);
			for(err = 0x00,u16DelayTime = 0x00;err < 32;err++)	
			{
				if((u32ReturnFlag&0x01) == 0x01)
					u16DelayTime++;
				u32ReturnFlag >>= 1;				
			}
			LogicParamApi(LOGIC_GET_ZIBAO_ACTION_MAX, &u16SendTimes);
			if(u16DelayTime > u16SendTimes)
			{
				if((sNewKey.u32Status == KEY_DOWN)||(sNewKey.u32Status == KEY_LONGDOWN))
					InterfaceInsertNote(INTERFACE_NOTE_ZIBAO_TROPPO);
				else
					InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
				ShowMain(&s_u32MainUpdateMethod);
				u32ReturnFlag = 0x00;
				s_u32ZibaoActionKeyBackup &= ~(sNewKey.u32Key);	
				ShowScZibaoKeyLedProc(s_u32ZibaoActionKeyBackup,KEY_DOWN,s_u32ShiftKeyEnableFlag);
				return u32ReturnFlag;		
			}
		}
		
		if((sNewKey.u32Status == KEY_DOWN)||(sNewKey.u32Status == KEY_LONGDOWN))
		{
			if(u32RfidData < s_u64InterfaceRfidDataBackup )//第二次按下，需停止
			{
				for(err = 0x00;err < INTERFACE_KEY_TOGETHER_MAX;err++)
				{
					if(s_stInterfaceActionName[err] == INTERFACE_ACTION_NONE)
						break;
					else
					{
						for(u16DelayTime=0x00;u16DelayTime < INTERFACE_KEY_TOGETHER_MAX;u16DelayTime++)
						{
							if(temp[u16DelayTime] == s_stInterfaceActionName[err])
								break;
						}
						if(u16DelayTime == INTERFACE_KEY_TOGETHER_MAX)
						{
							for(u16DelayTime=err;u16DelayTime < (INTERFACE_KEY_TOGETHER_MAX-0x01);u16DelayTime++)
							{
								s_stInterfaceActionName[u16DelayTime] = s_stInterfaceActionName[u16DelayTime+0x01];
								if(s_stInterfaceActionName[u16DelayTime+0x01] == INTERFACE_ACTION_NONE)
									break;
							}						
						}
					}
				}				
			}
			else
			{
				for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
				{
					if(temp[err] == INTERFACE_ACTION_NONE)
					{
						break;
					}
					else
					{
						for(u32Y = 0x00; u32Y < INTERFACE_KEY_TOGETHER_MAX;u32Y++)
						{
							if(s_stInterfaceActionName[u32Y] == temp[err])
								break;
							if(s_stInterfaceActionName[u32Y] == INTERFACE_ACTION_NONE)
							{
								s_stInterfaceActionName[u32Y] = temp[err];
								break;
							}
						}
					}
				}				
			}
			

		}
		else
		{
			return u32ReturnFlag;		
		}

		if(u32RfidData&&(sNewKey.u32Status == KEY_DOWN))
		{
			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
				BuzzerOn(BEEP_ACTION);
			#endif
			/**键值显示**/
			LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//发送的数据
			LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u32RfidData);//发送的数据
			g_stLogicSem->OSEventCnt = 0;
			u32Y = SC_BUTTON_ACK_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//将应答设为无效	
			u32Y = RFID_ACTION_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u32Y);//将应答动作字设为无效			
			u32Y = (u32)(u32RfidData|(stRfidType<<30));
			LogicRunInfApi(LOGIC_SET_BUTTON_VALUE,&u32Y);
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_DOWN);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_ZIBAO_CONTROL|INTERFACE_SHOW_MAIN_NOTE);
			ShowMain(&s_u32MainUpdateMethod);
			LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
			LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
			u32Y = (u32)(u16DelayTime*u16SendTimes);	
			u32Y += 30;		
			
			OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
			if(err == OS_ERR_TIMEOUT)//无应答
			{
				InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_ZIBAO_CONTROL);	
				for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_ZIBAO_CONTROL);//显示主屏所有信息
				ShowMain(&s_u32MainUpdateMethod);	
				s_u64InterfaceRfidDataBackup = 0x00;
				s_u32ZibaoActionKeyBackup= 0x00;
				ShowScZibaoKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);				
			}
			else if(err == OS_ERR_NONE)//有应答
			{
				/**关于设置的一些特殊模式切换的处理*/
				LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
				LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u32ButtonValue);
				LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&u32ButtonAckValue);
				if((status == SC_BUTTON_ACK_ACCEPT) && (u32ButtonAckValue == u32RfidData))
				{
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_ZIBAO_CONTROL);
					LogicRunInfApi(LOGIC_SET_BUTTON_LONG_VALUE,&u32RfidData);					
					u32Y = LOGIC_STATUS_RFID_SC_ZIBAO_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
					ShowMain(&s_u32MainUpdateMethod);
					ShowScZibaoKeyLedProc(s_u32ZibaoActionKeyBackup,KEY_DOWN,s_u32ShiftKeyEnableFlag);			
				}
				else if(status == SC_BUTTON_ACK_REJECT)
				{
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_ZIBAO_CONTROL);
					u32Y = LOGIC_STATUS_RFID_SC_ZIBAO_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
					for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
						s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_ZIBAO_CONTROL);//显示主屏所有信息
					ShowMain(&s_u32MainUpdateMethod);	
					s_u64InterfaceRfidDataBackup = 0x00;
					s_u32ZibaoActionKeyBackup= 0x00;
					ShowScZibaoKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);				
				}
				else
				{
					InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_ZIBAO_CONTROL);	
					u32Y = LOGIC_STATUS_RFID_SC_ZIBAO_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
					for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
						s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//默认屏幕显示
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_ZIBAO_CONTROL);//显示主屏所有信息					
					ShowMain(&s_u32MainUpdateMethod);	
					s_u64InterfaceRfidDataBackup = 0x00;
					s_u32ZibaoActionKeyBackup= 0x00;
					ShowScZibaoKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);				
				}
			}
			else	
				;
			
			u32ReturnFlag = 0x01;					
			
			s_u64InterfaceRfidDataBackup = (u64RfidData|(u64stRfidType<<62));
		}
		else if(u32RfidData&&(sNewKey.u32Status == KEY_LONGDOWN))
		{
			ShowScZibaoKeyLedProc(s_u32ZibaoActionKeyBackup,KEY_DOWN,s_u32ShiftKeyEnableFlag);		
		}
		else
			;
		
		return 0x01;
	}
#endif
/*******************************************************************************************
**函数名称：InterfaceModeShutdownProc
**函数作用：显示层处于关机模式下,按键的处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/

static u32 InterfaceModeShutdownProc(stKEY	sNewKey)
{
	u32 u32ReturnFlag = 0x00;
	
	//在此模式下，对按键的操作不予理解
	return u32ReturnFlag;
}
/*******************************************************************************************
**函数名称：InterfaceModeShutdownProc
**函数作用：显示层处于关机模式下,按键的处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
static u32 InterfaceModeInchargeDisplayProc(stKEY	sNewKey)
{
	u32 u32ReturnFlag;
	static u16 u16StopKeyPressFlag = 0x00;
	
	if((sNewKey.u32Key != KEY_STOP_SHUTDOWN) &&((sNewKey.u32Status == KEY_DOWN) ||((sNewKey.u32Status == KEY_LONGDOWN))))//有按键按下，唤醒屏幕
	{
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_AWAKE);
		OSSemPost(g_stSemInterface);
		u32ReturnFlag = 0x01;
	}
	else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_DOWN))//KEY_DOWN
	{
		if(s_stScrAwake == INTERFACE_SCR_PROTECED)
		{
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_AWAKE);
			OSSemPost(g_stSemInterface);			
		}
		else
		{
			u16StopKeyPressFlag = 0x01;			
		}		
	}		
	else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN)&& (sNewKey.u32Status == KEY_UP)&&(u16StopKeyPressFlag == 0x01))//KEY_DOWN
	{
		u16StopKeyPressFlag = 0x00;
		if(s_stScrAwake == INTERFACE_SCR_AWAKE)
		{
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_PROTECED);
			OSSemPost(g_stSemInterface);		
		}
		
	}
	else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN)&& (sNewKey.u32Status == KEY_LONGDOWN))
	{
		#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
			BuzzerOn(BEEP_SHUTDOWN_NOTE);
		#endif
		OSTaskSuspend(SCAN_KEY_TASK_PRIO);//挂起按键扫描任务
		u32ReturnFlag = LOGIC_STATUS_SHUTUP;
		LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32ReturnFlag);			
		ShowMain(&s_u32MainUpdateMethod);	
		u32ReturnFlag = 0x01;	
	}	
	else
		;
	return u32ReturnFlag;
}

/*******************************************************************************************
**函数名称：InterfaceModeShutdownProc
**函数作用：显示层处于程序更新,按键的处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
static u32 InterfaceModeSelfRecvTransPrgProc(stKEY	sNewKey)
{
	u32 u32ReturnFlag = 0x01;
	u32 u32SendLength;
	u32 u32Temp;
	u8  u8Buf[0x01];
	u16 u16i;
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
		OS_CPU_SR  cpu_sr = 0;
	#endif
	
	if((sNewKey.u32Key == KEY_STOP_SHUTDOWN)&&(sNewKey.u32Status == KEY_LONGDOWN))
	{
		StateLed(0x00,ADJUST_CODE_ING_LED);
		if(InterfaceGetCtrlMode() == INTERFACE_MODE_TRANS_PRG)
		{	
			OSTaskSuspend(LOGIC_TASK_PRIO);
			InterfaceInsertNote(INTERFACE_NOTE_TRANS_STOP);
			u8Buf[0x00] = 0x01;
			u32SendLength = 0x01;
			RfidUpdateSendPrgData(0x00,RFID_FT_UPDATE_END,0x00,u8Buf,u32SendLength);					
		}
		else
		{
			InterfaceInsertNote(INTERFACE_NOTE_RECV_STOP);
		}
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
		ShowMain(&s_u32MainUpdateMethod);	
		
		u32SendLength = LOGIC_STATUS_IDLE;
		LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32SendLength);
		
//		OS_ENTER_CRITICAL();
		if(g_u32ID == B_type)
		{	
			RFID_Init(RFID_ININT_SYNC0,RFID_ININT_SYNC1);
			RfidMngSetRecvCallBack();	
		}
		else
		{

			OSTaskResume(USART_TASK_PRIO);
			SendSetupToWl(RFID_ININT_SYNC1);
		}

		OSTaskResume(IRMNG_TASK_PRIO);	
		OSTaskDel(RFIDMNG_UPDATE_TASK_PRIO);
		OSTaskCreate(RfidMng_Task, (void *)0, &RfidMng_task_stk[RFIDMNG_TASK_STK_SIZE - 1], RFIDMNG_TASK_PRIO); 
		OSTaskResume(LOGIC_TASK_PRIO);	
//		OS_EXIT_CRITICAL();	
		
		OSTimeDly(150/TICK_TIME);			
		StateLed(0x00,ADJUST_CODE_ING_LED);
		
		u16i = RFID_SIGNAL_TOO_SMALL;
		LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16i);

		
		InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL|INTERFACE_SHOW_MAIN_RFIDSIGNAL);
		OSSemPost(g_stSemInterface);	
		
		
			u32Temp = (Version[8]-0x30)|(((Version[6]-0x30)+(Version[5]-0x30)*10)<<8)|((Version[3]-0x30)<<16)|((Version[1]-0x30)<<24);			
		
		#if (BSP_PRG_STORAGE_FLASH_FLAG	==	BSP_PRG_STORAGE_INFLASH_USE)
				__disable_fault_irq();
				FLASH_SetLatency(THE_DEV_FLASH_Latency);//120 MHz< HCLK <= 168 MHz
				FLASH_Unlock();			
		#endif	
		
		IapEraserSector(APP_VERSION_ADDRESS);
		IapWriteBuf((u8 *)&u32Temp, APP_VERSION_ADDRESS, 0x04);	
		IapWriteBuf((u8 *)&u32Temp, APP_VERSION_ADDRESS, 0x04);
		
		#ifdef IWDG_ENABLED
			IWDG_Feed();				//喂狗
		#endif
		
	#if (BSP_PRG_STORAGE_FLASH_FLAG	==	BSP_PRG_STORAGE_INFLASH_USE)
		FLASH_Lock();
		__enable_fault_irq();			
	#endif
	
	}
	else
	{
		ShowOnlineUpdateKeyLedProc(sNewKey.u32Key,sNewKey.u32Status,s_u32ShiftKeyEnableFlag);
	}
	
	

	
	return u32ReturnFlag;
}
#endif	




/*******************************************************************************************
**函数名称：InterfaceModeShutdownProc
**函数作用：显示层处于关机模式下,按键的处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
static u32 InterfaceModeWLUnConnectProc(stKEY	sNewKey)
{
	u32 u32ReturnFlag = 0x00;
	u32 u32Y = 0x00;
	static u16 u16StopKeyPressFlag = 0x00;
	
	if((sNewKey.u32Key != KEY_STOP_SHUTDOWN) &&((sNewKey.u32Status == KEY_DOWN) ||((sNewKey.u32Status == KEY_LONGDOWN))))//有按键按下，唤醒屏幕
	{
		StateLed(0x00, KEY_SHIFT_LED);//qinxi
		InterfaceSetCtrlMode(INTERFACE_MODE_WL_UNCONNECT);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_AWAKE);
		OSSemPost(g_stSemInterface);
		u32ReturnFlag = 0x01;
	}
	else if(((sNewKey.u32Key&KEY_STOP_SHUTDOWN) == KEY_STOP_SHUTDOWN)&& (sNewKey.u32Status == KEY_DOWN))//KEY_DOWN
	{

		KeyLedOnOff(0x00, KEY_LED_ALL);  //qinxi
		if(s_stScrAwake == INTERFACE_SCR_PROTECED)
		{
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_AWAKE);
			OSSemPost(g_stSemInterface);			
		}
		else
		{
			u16StopKeyPressFlag = 0x01;			
		}		
	}		
	else if(((sNewKey.u32Key&KEY_STOP_SHUTDOWN) == KEY_STOP_SHUTDOWN)&& (sNewKey.u32Status == KEY_UP)&&(u16StopKeyPressFlag == 0x01))//KEY_DOWN
	{
		
		u16StopKeyPressFlag = 0x00;
		if(s_stScrAwake == INTERFACE_SCR_AWAKE)
		{
			
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_PROTECED);
			KeyLedOnOff(0x00,KEY_LED_ALL);
			OSSemPost(g_stSemInterface);		
		}
		else if(s_stScrAwake == INTERFACE_SCR_AWAKE )
		{
			u8FastColiearateScNumFlag = 0;
			fastColiearateScNum = 0;
			InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_IDLE);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);			
			ShowMain(&s_u32MainUpdateMethod);
		}
	}
	else if(((sNewKey.u32Key&KEY_STOP_SHUTDOWN) == KEY_STOP_SHUTDOWN)&& (sNewKey.u32Status == KEY_LONGDOWN))
	{
		u16StopKeyPressFlag = 0x00;
		#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
			BuzzerOn(BEEP_SHUTDOWN_NOTE);
		#endif
		OSTaskSuspend(SCAN_KEY_TASK_PRIO);//挂起按键扫描任务
		InterfaceSetCtrlMode(INTERFACE_MODE_SHUTSOWN);		
		InterfaceInsertNote(INTERFACE_NOTE_SHUTDOWN);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL|INTERFACE_SHOW_MAIN_AWAKE);	
		u32Y = LOGIC_STATUS_SHUTUP;
		LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);			
		ShowMain(&s_u32MainUpdateMethod);	
		#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
			KeyLedOnOff(0x00,KEY_LED_ALL);
		#endif			
		u32ReturnFlag = 0x01;	

	}
	else
		;
	return u32ReturnFlag;
}
//zzw
/*******************************************************************************************
**函数名称：InterfaceModeActionInfoSyn
**函数作用：配置文件同步界面
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
static u32 InterfaceModeActionInfoSyn(stKEY	sNewKey)
{
	u32 u32ReturnFlag = 0x00;
	u32 u32Y;
	u16 u16DelayTime;
	u8 u8J;
	LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
	s_u32InterfaceColibrateTimer = u32Y;	
	InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
	InterfaceInsertNote(INTERFACE_NOTE_START_CERTIFY);
	ShowMain(&s_u32MainUpdateMethod);	
	//确认键开始同步	
	if(((sNewKey.u32Key&KEY_CERTIFY) == KEY_CERTIFY) && (sNewKey.u32Status == KEY_DOWN))//有按键按下，唤醒屏幕
	{
		LogicRunInfApi(LOGIC_SYNCHRO_ACTION_INFO, NULL);	//请求同步
		u32Y = LOGIC_STATUS_RFID_RECV_PRG;
		LogicRunInfApi(LOGIC_GET_RUN_STATUS,&u32Y);
//		if(1)
//		{
//			
//		}
//		else	//同步完成
//		{
//			u8J = MANUAL_CTRL;
//			LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);
//			InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);//支架控制器未确认状态(非动作状态)	
//			InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_SUCCEED);
//			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
//			ShowMain(&s_u32MainUpdateMethod);	
//			LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
//			s_u32InterfaceColibrateTimer = u32Y;	
//			u32Y = LOGIC_COLIBRATE_SC_IGNORE;
//			LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);	
//			if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_SELF_CONTROL)
//				ShowScSelfKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);
//			else	if(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_UNCERTIFIED)
//			{
//				ShowScUncertifiedKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);
//			}
//			else
//				;		
//		}

	}
	//停止键解除对码，回到空闲
	else if(((sNewKey.u32Key&KEY_STOP_SHUTDOWN) == KEY_STOP_SHUTDOWN)&& (sNewKey.u32Status == KEY_DOWN))
	{
		InterfaceShiftKeyLongDownPro();
		u32ReturnFlag = 0x01;		
	}
	else
		;
	return u32ReturnFlag;
}
/*******************************************************************************************
**函数名称：InterfacePeriodicProc
**函数作用：显示层周期性处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void InterfacePeriodicProc(u32 u32DelayTimer)//支架控制器周期性处理工作
{
	LOGIC_RUN_STATUS_TYPE temp = LOGIC_STATUS_IDLE;
	u8 u8BatteryStatus = 0x00;
	INTERFACE_CONTROL_MODE_TYPE sMode;
	u32 u32Y;
	u32 fastColiearateFlag = 0;
	u16 u16i;
	u8 u8J;
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
		 OS_CPU_SR  cpu_sr = 0;
	#endif
	
	sMode = InterfaceGetCtrlMode();
	LogicRunInfApi(LOGIC_GET_BATTERY_STATUS, &u8BatteryStatus);
	LogicParamApi(LOGIC_GET_FASTCOLIBARATE_ENABLE,&fastColiearateFlag);	 //jhy

	/**关于提示信息的显示管理*/
	#if ((QUICK_KEY_CTRL_ENABLE_FLAG != DISABLED)&&(ONLINE_UPDATE_SELF_PRG_PT_FLAG != DISABLED))
		if((sMode != INTERFACE_MODE_IDLE)&&(sMode != INTERFACE_MODE_SHUTSOWN)\
			&&(sMode != INTERFACE_MODE_SELF_SET)&&(sMode != INTERFACE_MODE_SC_SELF_QUICK)\
			&&(sMode != INTERFACE_MODE_TRANS_PRG)&&(sMode != INTERFACE_MODE_RECV_PRG))
	#elif	 (QUICK_KEY_CTRL_ENABLE_FLAG != DISABLED)
		if((sMode != INTERFACE_MODE_IDLE)&&(sMode != INTERFACE_MODE_SHUTSOWN)\
			&&(sMode != INTERFACE_MODE_SELF_SET)&&((sMode != INTERFACE_MODE_SC_SELF_QUICK)))
	#elif	(ONLINE_UPDATE_SELF_PRG_PT_FLAG != DISABLED)
		if((sMode != INTERFACE_MODE_IDLE)&&(sMode != INTERFACE_MODE_SHUTSOWN)\
			&&(sMode != INTERFACE_MODE_SELF_SET)&&(sMode != INTERFACE_MODE_TRANS_PRG)\
			&&(sMode != INTERFACE_MODE_RECV_PRG_START)&&(sMode != INTERFACE_MODE_RECV_PRG_ING))		
	#else
		if((sMode != INTERFACE_MODE_IDLE)&&(sMode != INTERFACE_MODE_SHUTSOWN)&&(sMode != INTERFACE_MODE_SELF_SET))
	#endif	
		{
			if((u8BatteryStatus != BATTERY_START_CHARGED)&&(u8BatteryStatus != BATTERY_BEING_CHARGED)\
				&&(u8BatteryStatus != BATTERY_CHARGE_GONNA_OVER)&&(u8BatteryStatus != BATTERY_CHARGE_OVER))
			{
				if(s_u32NoteShowTimer && (s_u32NoteShowTimer != INTERFACE_NOTE_TIMER_STOP))
				{
					s_u32NoteShowTimer -= u32DelayTimer;
					if(s_u32NoteShowTimer == 0x00)
					{
						if(s_u8pleaseNearFlag)
							InterfaceInsertNote(INTERFACE_NOTE_PLEASE_NEAR);//请靠近
						else
							InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);//空闲
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//提示信息
						OSSemPost(g_stSemInterface);
						s_u32NoteShowTimer = INTERFACE_NOTE_TIMER_STOP;
					}
				}
			}
		}
		else if(sMode== INTERFACE_MODE_IDLE)
		{
			if((u8BatteryStatus != BATTERY_START_CHARGED)&&(u8BatteryStatus != BATTERY_BEING_CHARGED)\
				&&(u8BatteryStatus != BATTERY_CHARGE_GONNA_OVER)&&(u8BatteryStatus != BATTERY_CHARGE_OVER))
			{
				if(s_u32NoteShowTimer && (s_u32NoteShowTimer != INTERFACE_NOTE_TIMER_STOP))
				{
					s_u32NoteShowTimer -= u32DelayTimer;
					if(s_u32NoteShowTimer == 0x00)
					{
						fastColiearateScNum = 0;
						KeyLedOnOff(0x00, KEY_LED_ALL);
						InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_IDLE);//空闲
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//提示信息
						OSSemPost(g_stSemInterface);
						s_u32NoteShowTimer = INTERFACE_NOTE_TIMER_STOP;
					}
				}
			}			
		}
		else
			;
	/*关于返回本架控制模式下的处理**/
	#if	((ZIBAO_CTRL_ENABLE_FLAG	==	ENABLED)&&(ZIBAO_CTRL_METHOD_TYPE == ZIBAO_PARAM_CTRL_METHOD))	
		if(s_u32ScCtrlKeyTimer && (s_u32ScCtrlKeyTimer  != INTERFACE_CTRL_TIMER_STOP)\
			&&(((s_u32AllKeyStatus == KEY_UP)&&(sMode != INTERFACE_MODE_SC_ZIBAO_CONTROL))||(sMode == INTERFACE_MODE_SC_ZIBAO_CONTROL))	)	
	#else
		if(s_u32ScCtrlKeyTimer && (s_u32ScCtrlKeyTimer  != INTERFACE_CTRL_TIMER_STOP) && (s_u32AllKeyStatus == KEY_UP))
	#endif
		{
			s_u32ScCtrlKeyTimer  -= u32DelayTimer;
			if(s_u32ScCtrlKeyTimer == 0x00)
			{
				#if ((LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG != DISABLED)&&(GRP_CTRL_ENABLE_FLAG != DISABLED)\
				&&((ZIBAO_CTRL_ENABLE_FLAG	==	ENABLED)&&(ZIBAO_CTRL_METHOD_TYPE == ZIBAO_PARAM_CTRL_METHOD)))
					if((sMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)||(sMode == INTERFACE_MODE_SC_SET)\
					   ||(sMode == INTERFACE_MODE_SC_GRP_CONTROL)||(sMode == INTERFACE_MODE_SC_ZIBAO_CONTROL))
				#elif (GRP_CTRL_ENABLE_FLAG != DISABLED) \
					&&((ZIBAO_CTRL_ENABLE_FLAG	==	ENABLED)&&(ZIBAO_CTRL_METHOD_TYPE == ZIBAO_PARAM_CTRL_METHOD))	
					if((sMode == INTERFACE_MODE_SC_SET)||(sMode == INTERFACE_MODE_SC_GRP_CONTROL)\
					  ||(sMode == INTERFACE_MODE_SC_ZIBAO_CONTROL))	
				#elif ((LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG != DISABLED)\
					&&((ZIBAO_CTRL_ENABLE_FLAG	==	ENABLED)&&(ZIBAO_CTRL_METHOD_TYPE == ZIBAO_PARAM_CTRL_METHOD)))
					if((sMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)||(sMode == INTERFACE_MODE_SC_SET)\
					  ||(sMode == INTERFACE_MODE_SC_ZIBAO_CONTROL))
				#elif ((GRP_CTRL_ENABLE_FLAG != DISABLED)&&(LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG != DISABLED))
					if((sMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)||(sMode == INTERFACE_MODE_SC_SET)\
					   ||(sMode == INTERFACE_MODE_SC_GRP_CONTROL))			
				#elif (GRP_CTRL_ENABLE_FLAG != DISABLED) 	  
					if((sMode == INTERFACE_MODE_SC_SET)||(sMode == INTERFACE_MODE_SC_GRP_CONTROL))
				#elif (LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG != DISABLED)
					if((sMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)||(sMode == INTERFACE_MODE_SC_SET))
				#elif ((ZIBAO_CTRL_ENABLE_FLAG ==	ENABLED)&&(ZIBAO_CTRL_METHOD_TYPE == ZIBAO_PARAM_CTRL_METHOD))
					if((sMode == INTERFACE_MODE_SC_SET)||(sMode == INTERFACE_MODE_SC_ZIBAO_CONTROL))
				#else
					if(sMode == INTERFACE_MODE_SC_SET)	
				#endif		
					{
						s_u32StartKeyFlag = KEY_UP ;
//						s_u32SpecialFxKeyFlag3 = KEY_UP;
//						s_u32SpecialFxKeyFlag1 = KEY_UP;
//						s_u32SpecialFxKeyFlag2 = KEY_UP;
						LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);	
						InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);//空闲
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL|INTERFACE_SHOW_MAIN_AWAKE);//显示主屏所有信息
						#if ((ZIBAO_CTRL_ENABLE_FLAG ==	ENABLED)&&(ZIBAO_CTRL_METHOD_TYPE == ZIBAO_PARAM_CTRL_METHOD))
							if(sMode == INTERFACE_MODE_SC_ZIBAO_CONTROL)
							{	
								InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
								ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
							}
							else
							{
								#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
									InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
									ShowScUncertifiedKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
								#else
									InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
									ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
								#endif							
							}
						#else
							#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
								InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
								ShowScUncertifiedKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
							#else
								InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
								ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
							#endif
						#endif
						OSSemPost(g_stSemInterface);
						s_u32ShiftKeyEnableFlag = DISABLED;
						StateLed(0x00, KEY_SHIFT_LED);	
					}				
				s_u32ScCtrlKeyTimer = INTERFACE_CTRL_TIMER_STOP;
			}
		}
	
	/**关于退出主控模式的处理**/
	#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
		if(s_u32InterfaceControlTimer && (s_u32InterfaceControlTimer  != INTERFACE_CTRL_TIMER_STOP)\
			&&((sMode == INTERFACE_MODE_IDLE)||(sMode == INTERFACE_MODE_SELF_SET)||(sMode == INTERFACE_MODE_TRANS_PRG)\
		    ||(sMode == INTERFACE_MODE_RECV_PRG_ING)||(sMode == INTERFACE_MODE_RECV_PRG_START)))		
	#else	
		if(s_u32InterfaceControlTimer && (s_u32InterfaceControlTimer  != INTERFACE_CTRL_TIMER_STOP)\
			&&((sMode == INTERFACE_MODE_IDLE)||(sMode == INTERFACE_MODE_SELF_SET)))
	#endif
		{
			s_u32InterfaceControlTimer  -= u32DelayTimer;
			if(s_u32InterfaceControlTimer == 0x00)
			{
				ClearUpdateMainMethod();
				LogicRunInfApi(LOGIC_GET_BATTERY_STATUS, &u8BatteryStatus);
				if(sMode == INTERFACE_MODE_SELF_SET)
				{
					s_u32SelfSetIntfState = INTERFACEMENU_STATE_MENU_START;
					InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//显示主屏所有信息
					OSSemPost(g_stSemInterface);
					KeyLedOnOff(0x00,KEY_LED_ALL);
				}	
				#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
					else if((sMode == INTERFACE_MODE_TRANS_PRG)||(sMode == INTERFACE_MODE_RECV_PRG_ING)||(sMode == INTERFACE_MODE_RECV_PRG_START))
					{
						if(sMode == INTERFACE_MODE_TRANS_PRG)
							InterfaceInsertNote(INTERFACE_NOTE_TRANS_STOP);
						else
							InterfaceInsertNote(INTERFACE_NOTE_RECV_STOP);
						
						u32Y = LOGIC_STATUS_IDLE;
						LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);			
						
						u16i = RFID_SIGNAL_TOO_SMALL;
						LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16i);

						InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL|INTERFACE_SHOW_MAIN_RFIDSIGNAL);//显示主屏所有信息
						OSSemPost(g_stSemInterface);
						KeyLedOnOff(0x00,KEY_LED_ALL);
						StateLed(0x00,ADJUST_CODE_ING_LED);					

						IapReadBuf((u8 *)&fastColiearateFlag,YKQ_DEV_PROGRAM_BASE_ADDRESS|PROG_VERSION_OFFSET_ADDRESS,0x04);	
						IapWriteBuf((u8 *)&fastColiearateFlag, APP_VERSION_ADDRESS, 0x04);	
						IapWriteBuf((u8 *)&fastColiearateFlag, APP_VERSION_ADDRESS, 0x04);
						
//						OS_ENTER_CRITICAL();		
						if(g_u32ID == B_type)
						{			
							RFID_Init(RFID_ININT_SYNC0,RFID_ININT_SYNC1);
							RfidMngSetRecvCallBack();
						}
						else
						{
							OSTaskResume(USART_TASK_PRIO);
							SendSetupToWl(RFID_ININT_SYNC1);							
						}

										
						OSTaskDel(RFIDMNG_UPDATE_TASK_PRIO);						
						OSTaskResume(IRMNG_TASK_PRIO);
						OSTaskCreate(RfidMng_Task, (void *)0, &RfidMng_task_stk[RFIDMNG_TASK_STK_SIZE - 1], RFIDMNG_TASK_PRIO); 
//						OS_EXIT_CRITICAL();
						
					}
				#endif
				else if((u8BatteryStatus == BATTERY_START_CHARGED)||(u8BatteryStatus == BATTERY_BEING_CHARGED)\
					||(u8BatteryStatus == BATTERY_CHARGE_GONNA_OVER))
				{
					InterfaceInsertNote(INTERFACE_NOTE_BATTERY_INCHARGE);//正在充电
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
					OSSemPost(g_stSemInterface);
					s_u32InterfaceControlTimer  = INTERFACE_CTRL_TIMER_STOP;
				}
				else if(u8BatteryStatus == BATTERY_CHARGE_OVER)//充电已经完成
				{
					InterfaceInsertNote(INTERFACE_NOTE_BATTERY_INCHARGE_OVER);//正在充电
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
					OSSemPost(g_stSemInterface);
					s_u32InterfaceControlTimer  = INTERFACE_CTRL_TIMER_STOP;							
				}
				else
				{
					InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_IDLE);//空闲
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息
					OSSemPost(g_stSemInterface);
					s_u32InterfaceControlTimer  = INTERFACE_CTRL_TIMER_STOP;
				}				
			}
		}

	/*关于解除对码的处理**/
	#if (ZIBAO_CTRL_ENABLE_FLAG != DISABLED)
		if(s_u32InterfaceColibrateTimer && (s_u32InterfaceColibrateTimer  != INTERFACE_COLIBRATE_TIMER_STOP)\
			&&((s_u32AllKeyStatus == KEY_UP)\
			||((s_u32ZibaoActionKeyBackup == KEY_ZI_BAOCHI)&&(sMode == INTERFACE_MODE_SC_ZIBAO_CONTROL)&&(RfidGetDataLimitlessSendFlag()==0x00))))
	#else
		if(s_u32InterfaceColibrateTimer && (s_u32InterfaceColibrateTimer  != INTERFACE_COLIBRATE_TIMER_STOP)&&(s_u32AllKeyStatus == KEY_UP))	
	#endif
		{
			s_u32InterfaceColibrateTimer -= u32DelayTimer;
			if(s_u32InterfaceColibrateTimer == 0x00)
			{
				#if (SET_YKQ_PRGPT_USE_FLAG	!= DISABLED)	
					if((sMode != INTERFACE_MODE_YKQ_SELF_CONTROL)&&(sMode != INTERFACE_MODE_YKQ_SET_CONTROL))
				#else
				#endif
					{
						#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
							BuzzerOn(BEEP_UNCOLIBRAT);
						#endif
						#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
							KeyLedOnOff(0x00, KEY_LED_ALL);
						#endif
						if(RfidGetDataLimitlessSendFlag())
						{
							RfidSetDataLimitlessSendEnd();
						}
						u8J = NO_CTRL;
						LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_CTRL_MODE);			//zzw
						ShowMain(&s_u32MainUpdateMethod);
						/*解除对码**/
						InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
						StateLed(0x00,KEY_SHIFT_LED|ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED);
						temp = LOGIC_STATUS_RFID_UNCOLIBRATE;
						LogicRunInfApi(LOGIC_SET_RUN_STATUS,&temp);
						u32Y = RFID_SIGNAL_TOO_SMALL;
						LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u32Y);			
						InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_UNCOLIBRATE);
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);		
						OSSemPost(g_stSemInterface);
						KeyLedOnOff(0x00, KEY_LED_ALL);
						s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
						
						s_u64InterfaceRfidDataBackup = 0x00;
						s_u32StartKeyFlag= KEY_UP;//
						s_u32ShiftKeyEnableFlag = DISABLED;
						#if (SPECIAL_FX_ENABLE_FLAG != DISABLED)
							#if (SPECIAL_F1_ENABLE_FLAG != DISABLED)
								s_u32SpecialFxKeyFlag1= KEY_UP;//
							#endif
							#if (SPECIAL_F2_ENABLE_FLAG != DISABLED)
								s_u32SpecialFxKeyFlag2= KEY_UP;//
							#endif
							#if (SPECIAL_F3_ENABLE_FLAG != DISABLED)
								s_u32SpecialFxKeyFlag3= KEY_UP;//
							#endif
							#if (SPECIAL_F4_ENABLE_FLAG != DISABLED)
								s_u32SpecialFxKeyFlag4= KEY_UP;//
							#endif
						#endif
						#if (ZIBAO_CTRL_ENABLE_FLAG != DISABLED)
							s_u32ZibaoActionKeyBackup = 0x00;
						#endif

						#if (GRP_LALIU_BUTTONUP_STOP_FLAG != DISABLED)
							s_u32GrpLaliuKeyFlag = KEY_UP;
						#endif	
					}
				#if (SET_YKQ_PRGPT_USE_FLAG	!= DISABLED)	
					if((sMode == INTERFACE_MODE_YKQ_SELF_CONTROL)||(sMode != INTERFACE_MODE_YKQ_SET_CONTROL))
					{
						BuzzerOn(BEEP_UNCOLIBRAT);	
						/*解除对码**/
						InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
						StateLed(0x00,KEY_SHIFT_LED|ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED);
						KeyLedOnOff(0x00, KEY_LED_ALL);
						temp = LOGIC_STATUS_IDLE;
						LogicRunInfApi(LOGIC_SET_RUN_STATUS,&temp);
						InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_IDLE);
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);		
						OSSemPost(g_stSemInterface);			
						s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
						ParEditSetSearchDevFlagProc(DISABLED);
						if(g_u32ID == B_type)
						{
							IrUsartChipPwrOff();
						}
						else
						{
							GPIO_SetBits(GPIOC,GPIO_Pin_6);
						}				
					}
				#endif	
			}
		}

//	/**关于休屏的管理**/
	if(s_u32ScrPrtectedTimer && (s_u32ScrPrtectedTimer != INTERFACE_SCR_TIMER_STOP) && ((sMode == INTERFACE_MODE_IDLE)||(sMode == INTERFACE_MODE_INCHARGE_DISPLAY)))
	{
		s_u32ScrPrtectedTimer -= u32DelayTimer;
		if(s_u32ScrPrtectedTimer == 0x00)
		{
//			if(g_u32ID != B_type&&fastColiearateFlag == 0)
//				WirelessChipPwrOff();
			ClearUpdateMainMethod();
			KeyLedOnOff(0x00,KEY_LED_ALL);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_PROTECED);//休屏
			OSSemPost(g_stSemInterface);
		}
	}	
	
	/**自动关机的管理***/
	if(s_u32AutoShutdownTimer && (s_u32AutoShutdownTimer != INTERFACE_AUTOSHUTDOWN_TIMER_STOP) \
		&& ((sMode == INTERFACE_MODE_IDLE)||(sMode == INTERFACE_MODE_INCHARGE_DISPLAY)))
	{
		s_u32AutoShutdownTimer -= u32DelayTimer;  //jhyjhy
		if(s_u32AutoShutdownTimer == 0x00)
		{
			if((u8BatteryStatus != BATTERY_START_CHARGED)&&(u8BatteryStatus != BATTERY_BEING_CHARGED)\
				&&(u8BatteryStatus != BATTERY_CHARGE_GONNA_OVER))//&&(u32Y == ENABLED))
			{				
				InterfaceInsertNote(INTERFACE_NOTE_AUTO_SHUTDOWN);
				if(sMode == INTERFACE_MODE_IDLE)
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_MAIN_AWAKE);
				else
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_INCHARGE_DISPLAY|INTERFACE_SHOW_MAIN_AWAKE);
				ShowMain(&s_u32MainUpdateMethod);
				#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
					BuzzerOn(BEEP_SHUTDOWN_NOTE);
				#endif
				OSTimeDly(1000/TICK_TIME);	//showing for 1s
				/*进行关机操作**/
				LogicShutdownProc();             //jhyjhy
			}
			else
			{
				LogicParamApi(LOGIC_GET_OLED_AUTOSHUTDOWN_TIMER,&s_u32AutoShutdownTimer);
				if(s_u32AutoShutdownTimer == 0x00)
					s_u32AutoShutdownTimer = INTERFACE_AUTOSHUTDOWN_TIMER_STOP;				
			}
		}
	}
	/***传输或接收程序时，时间的显示**/
	#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)	
		if((sMode == INTERFACE_MODE_TRANS_PRG)||(sMode == INTERFACE_MODE_RECV_PRG_ING))
		{
			/*小于十秒每秒刷新一次,大于10秒每10秒刷新一次*/
			if(s_u32DownloadRemainTimer >= u32DelayTimer)
				s_u32DownloadRemainTimer -= u32DelayTimer;
			else
				s_u32DownloadRemainTimer = 0x00;
			if(((s_u32DownloadRemainTimer <= 10000) && ((s_u32DownloadRemainTimerBackup - s_u32DownloadRemainTimer) >= 1000 ))
				|| ((s_u32DownloadRemainTimer > 10000) && ((s_u32DownloadRemainTimerBackup - s_u32DownloadRemainTimer) >= 10000)))
			{
				
				s_u32DownloadRemainTimerBackup = s_u32DownloadRemainTimer;
				if(s_u32DownloadRemainTimerBackup > 10000)//为保证以10秒的倍数进行显示，需对剩余时间的备份进行特殊处理
				{
					s_u32DownloadRemainTimer /= 1000;//秒
					if(s_u32DownloadRemainTimer % 10000)
					{
						s_u32DownloadRemainTimer /= 10;
						s_u32DownloadRemainTimer ++;	
						s_u32DownloadRemainTimer *= 10;
					}
					s_u32DownloadRemainTimer *= 1000;
				}
				InterfaceSetUpdateMainMethod((sMode==INTERFACE_MODE_TRANS_PRG)?INTERFACE_SHOW_SELF_TRANS_PRG:INTERFACE_SHOW_SELF_RECV_PRG);
				ShowMain(&s_u32MainUpdateMethod);
			}
		}
		//配置文件同步	zzw
		if(sMode == INTERFACE_MODE_ACTIONINFI_SYN)
		{
			/*小于十秒每秒刷新一次,大于10秒每10秒刷新一次*/
			if(s_u32DownloadRemainTimer >= u32DelayTimer)
				s_u32DownloadRemainTimer -= u32DelayTimer;
			else
				s_u32DownloadRemainTimer = 0x00;
			if(((s_u32DownloadRemainTimer <= 10000) && ((s_u32DownloadRemainTimerBackup - s_u32DownloadRemainTimer) >= 1000 ))
				|| ((s_u32DownloadRemainTimer > 10000) && ((s_u32DownloadRemainTimerBackup - s_u32DownloadRemainTimer) >= 10000)))
			{
				
				s_u32DownloadRemainTimerBackup = s_u32DownloadRemainTimer;
				if(s_u32DownloadRemainTimerBackup > 10000)//为保证以10秒的倍数进行显示，需对剩余时间的备份进行特殊处理
				{
					s_u32DownloadRemainTimer /= 1000;//秒
					if(s_u32DownloadRemainTimer % 10000)
					{
						s_u32DownloadRemainTimer /= 10;
						s_u32DownloadRemainTimer ++;	
						s_u32DownloadRemainTimer *= 10;
					}
					s_u32DownloadRemainTimer *= 1000;
				}
				InterfaceSetUpdateMainMethod(sMode);
				ShowMain(&s_u32MainUpdateMethod);
			}
		}
	#endif	
//	if(u16KeyFilmNameRam == 0x02)		//选择自定义键盘膜时，因各键动作未定义，导致键盘灯都熄灭，在此处点亮	2020.04.27 parry
//	{
//		switch(InterfaceGetCtrlMode())	
//		{
//			case 	INTERFACE_MODE_SELF_SET:
//				if(g_u32ID == B_type)
//				{
//					if(stEdit.u16MenuGrpNo > MENU_YKQ_MACHINE_NO)
//						KeyLedOnOff(KEY_LED_ALL,0x00);				
//				}
//				else
//				{
//					if(stEdit.u16MenuGrpNo > MENU_YKQ_MACHINE_NO)
//						KeyLedOnOff(KEY_LED_SET,0x00);			
//				}

//				break;
//			case	INTERFACE_MODE_SC_SET:
//				if(g_u32ID == B_type)
//				{
//					if(s_u32SelfSetIntfState > INTERFACEMENU_STATE_NONE)
//						KeyLedOnOff(KEY_LED_ALL,0x00);
//				}
//				else
//				{
//					if(s_u32SelfSetIntfState > INTERFACEMENU_STATE_NONE)
//						KeyLedOnOff(KEY_LED_SET,0x00);			
//				}
//				break;
//			default:
//				break;
//		}
//	}
}

/**********************************************************************************************
**函数名称：InterfaceTaskStartProc
**函数作用：本模块是开机之后,界面处理任务进入循环之前的操作
**函数参数：无
**函数输出：无
**注意事项：无
***********************************************************************************************/
#ifdef	IWDG_ENABLED
	extern OS_STK  Iwdg_task_stk[TASK_STK_SIZE_64];				//开辟任务堆栈
#endif
void InterfaceTaskStartProc(void)
{
	u32 u32DelayTime = 0x00;
	u32 u32PwrCsr;
	u32 u32ShutDownFlag;
	u32 u32Temp = 0x00;
	u32 u32Temp2 = 0x00;
	u8 CtrlMode = NO_CTRL;
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
		OSTimeDly(3000/TICK_TIME);	//showing for 1s	
	#endif

	if(((u32ShutDownFlag&YKQ_SHUTDOWN_INCHARGE_TURNON) == YKQ_SHUTDOWN_INCHARGE_TURNON)\
		||((u32ShutDownFlag&YKQ_SHUTDOWN_WORK_TURNON) == YKQ_SHUTDOWN_WORK_TURNON))
	{
		LogicParamApi(LOGIC_GET_BATTERY_COULDATA_BACKUP,&u32DelayTime);
		if((u32DelayTime >= BATTERY_VOLTAGE_FULL_PERCENT)&&((u32ShutDownFlag&YKQ_SHUTDOWN_INCHARGE_TURNON) == YKQ_SHUTDOWN_INCHARGE_TURNON))
		{
			OSTaskSuspend(BATTERYMNG_TASK_PRIO);
			InterfaceSetCtrlMode(INTERFACE_MODE_INCHARGE_DISPLAY);
			u32DelayTime = BATTERY_CHARGE_OVER;
			LogicRunInfApi(LOGIC_SET_BATTERY_STATUS, &u32DelayTime);
			LogicShutdownProc();
		}
		else
			;//直接开机
	}		
	else if((u32ShutDownFlag&YKQ_SHUTDOWN_INCHARGE_TURNDOWN) == YKQ_SHUTDOWN_INCHARGE_TURNDOWN)
	{
		if(GPIO_ReadInputDataBit(INPUT_WAKEUP1_PORT,INPUT_WAKEUP1_PIN) == Bit_SET)
			u32ShutDownFlag = YKQ_SHUTDOWN_INCHARGE_TURNON;
		else
		{
			OSTaskSuspend(BATTERYMNG_TASK_PRIO);
			LogicShutdownProc();
		}
	}
	else if(((u32ShutDownFlag&YKQ_SHUTDOWN_SLEEP_WAKEUP1) == YKQ_SHUTDOWN_SLEEP_WAKEUP1)|| (u32ShutDownFlag&YKQ_SHUTDOWN_SLEEP_WAKEUP2) == YKQ_SHUTDOWN_SLEEP_WAKEUP2)
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
							u32ShutDownFlag = YKQ_SHUTDOWN_INCHARGE_TURNON;
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
								u32ShutDownFlag = YKQ_SHUTDOWN_INCHARGE_TURNON;
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

		/*看门狗任务***/
	#ifdef	IWDG_ENABLED
		OSTaskCreateExt(IWDG_Task, 
						(void *)0, 
						&Iwdg_task_stk[IWDG_TASK_STK_SIZE - 1], 
						IWDG_TASK_PRIO,
						IWDG_TASK_PRIO,
						&Iwdg_task_stk[0],
						IWDG_TASK_STK_SIZE,
						(void* )0,
						(uint16_t)(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
	#endif
	
	//初始化
	if(s_stInterfaceControlMode == INTERFACE_MODE_ININT)
	{
			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
				BUZZER_ON();
			#endif
			StateLed(ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED|KEY_SHIFT_LED,0x00);
			#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
				KeyLedOnOff(KEY_LED_ALL,0x00);	
			#endif
			LcdClear();
			ShowStr(OLED_DISPLAY_NORMAL,Version,4, 24);
			OSTimeDly(1000/TICK_TIME);	//showing for 1s	
			LogicParamApi(LOGIC_SET_CTRL_MODE,&CtrlMode);	//zzw
			if((GPIO_ReadInputDataBit(INPUT_WAKEUP1_PORT,INPUT_WAKEUP1_PIN) == Bit_SET)\
				&&((u32ShutDownFlag&YKQ_SHUTDOWN_INCHARGE_TURNON) == YKQ_SHUTDOWN_INCHARGE_TURNON))//充电唤醒的情况下
			{
				s_stInterfaceControlMode = INTERFACE_MODE_INCHARGE_DISPLAY;
				InterfaceInsertNote(INTERFACE_NOTE_BATTERY_INCHARGE);	
				
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_INCHARGE_DISPLAY);	
				if(g_u32ID != B_type)
					OSTaskSuspend(USART_TASK_PRIO);

				OSTaskSuspend(IRMNG_TASK_PRIO);			
				OSTaskSuspend(RFIDMNG_TASK_PRIO);
								
				#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
					OSTaskSuspend(BUZZERMNG_TASK_PRIO);
				#endif
//				WirelessChipPwrOff();//jhyjhy
				if(g_u32ID == B_type)
				{
					IrUsartChipPwrOff();
				}
				else
				{
					GPIO_SetBits(GPIOC,GPIO_Pin_6);
				}
			}
			else
			{
				s_stInterfaceControlMode = INTERFACE_MODE_IDLE;				
				InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_IDLE);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);	
			}

			ShowMain(&s_u32MainUpdateMethod);
			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
				BUZZER_OFF();
			#endif
			StateLed(0x00, ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED|KEY_SHIFT_LED);
			#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
				KeyLedOnOff(0x00,KEY_LED_ALL);
			#endif			

			#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_ADC_MNG)//在硬件中，使用AD方式计算电量
				OSTimeDly(300/TICK_TIME);	//showing for 1s	
				LogicParamApi(LOGIC_GET_BATTERY_VOLTAGE_BACKUP,&u32DelayTime);
				BatteryGetVoltBackup(&u32Temp);
				LogicParamApi(LOGIC_GET_BATTERY_INCHARGE_TIMER,&u32Temp2);
			
			#if 0
				LcdClear();
				sprintf(s2,"%d,",u32DelayTime);
				ShowStr(OLED_DISPLAY_NORMAL,s2,0, 0);	
				sprintf(s2,"%d,",u32Temp);
				ShowStr(OLED_DISPLAY_NORMAL,s2,0, 48);	
				
				sprintf(s2,"%x,",u32Temp2);
				ShowStr(OLED_DISPLAY_NORMAL,s2,2, 0);				
				
				sprintf(s2,"%d",u32Temp-u32DelayTime);
				ShowStr(OLED_DISPLAY_NORMAL,s2,2, 72);
						
			#endif
			
				if(u32Temp >=  u32DelayTime)
					u32Temp -= u32DelayTime;
				else
				{	
					u32DelayTime -= u32Temp;
					u32Temp = u32DelayTime;
				}				
				if(((u32Temp2 == 0xFFFFFFFF)&&(u32Temp <= BATTERY_TURNOFF_TURNON_VOLTAGE_DIFFERENCE))\
					||((u32Temp2 != 0xFFFFFFFF)&&(u32Temp <= BATTERY_TURNOFF_TURNON_VOLTAGE_DIFFERENCE2))\
					||((GPIO_ReadInputDataBit(INPUT_WAKEUP1_PORT,INPUT_WAKEUP1_PIN) == Bit_SET)&&(u32Temp <= BATTERY_TURNOFF_TURNON_VOLTAGE_DIFFERENCE2)))
				{
					u32DelayTime = 0x00;
					LogicParamApi(LOGIC_GET_BATTERY_ACR_BACKUP,&u32DelayTime);
					if(g_u32ID == B_type)
					{
						if(u32DelayTime <= BATTERY_CURRENT_MAX)
							BatterySetTotalAcr(u32DelayTime);
					}
					else
					{
						if(u32DelayTime <= BATTERY_CURRENT_MAX_D)
							BatterySetTotalAcr(u32DelayTime);
					}
					
					#if 0
						sprintf(s2,"%d,",u32DelayTime);
						ShowStr(OLED_DISPLAY_NORMAL,s2,4, 0);				
					#endif
					
					u32DelayTime = 0x00;
					LogicParamApi(LOGIC_GET_BATTERY_COULDATA_BACKUP,&u32DelayTime);
					if(u32DelayTime <= BATTERY_VOLTAGE_FULL_PERCENT)				
						BatterySetCoulomeData(u32DelayTime);
					
					#if 0
						sprintf(s2,"%d",u32DelayTime);
						ShowStr(OLED_DISPLAY_NORMAL,s2,4, 64);	
		
						u32DelayTime = 0x00;	
						BatteryGetCoulomeData((u16 *)&u32Temp);
						sprintf(s2,"%d",u32Temp);
						ShowStr(OLED_DISPLAY_NORMAL,s2,6, 0);
					#endif					
				}		
				u32DelayTime = 0xFFFFFFFF;
				LogicParamApi(LOGIC_SET_BATTERY_ACR_BACKUP,&u32DelayTime);
				LogicParamApi(LOGIC_SET_BATTERY_COULDATA_BACKUP,&u32DelayTime);
				LogicParamApi(LOGIC_SET_BATTERY_VOLTAGE_BACKUP,&u32DelayTime);
				OSSemPost(g_stLogicSem);	
			#endif
	}
}	

/**********************************************************************************************
**函数名称：Interface_task
**函数作用：界面显示主任务
**函数参数：无
**函数输出：无
**注意事项：无
**********************************************************************************************/
void Interface_task(void *p_arg) 
{ 
	u32 u32Temp;
	INT8U	err;
	stKEY	sNewKey;	
	u16 ScNum,u16I;
    u8 u8I = 0;
//	char s[30];
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
		OS_CPU_SR  cpu_sr = 0;
	#endif
	ActionInfoInit();		//配置文件初始化	
	InterfaceTaskStartProc();
	//任务运行循环
	while(1) 
	{
		#ifdef	IWDG_ENABLED	
			OS_ENTER_CRITICAL();
			Iwdg_Interface = 0;
			OS_EXIT_CRITICAL();
		#endif

//		OSSemPend(g_stKeySem,INTERFACE_TASK_TIME_SET/TICK_TIME, &err);
//		if(err == OS_ERR_NONE)
		OSTimeDly(INTERFACE_TASK_TIME_SET/TICK_TIME);
		if(GetKeyNum() > 0)
		{
			GetKey(&sNewKey);
			if(InterfaceModeKeyProcFuncs[s_stInterfaceControlMode])
			{
				(*InterfaceModeKeyProcFuncs[s_stInterfaceControlMode])(sNewKey);
			}
			else
			{
				u32Temp = LOGIC_STATUS_IDLE;
				LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Temp);

				InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_IDLE);	
				InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);
				OSSemPost(g_stSemInterface);
				
				StateLed(0x00,ADJUST_CODE_ING_LED);
			}
		}
//		else if(err == OS_ERR_TIMEOUT)
		{
			OSSemPend(g_stSemInterface,INTERFACE_TASK_TIME_SET/TICK_TIME, &err);	
			if(err == OS_ERR_NONE)	
			{
				ShowMain(&s_u32MainUpdateMethod);
			}
//			if((InterfaceGetCtrlMode() == INTERFACE_MODE_SC_UNCERTIFIED)
//				||(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
//				||(InterfaceGetCtrlMode() == INTERFACE_MODE_SC_GRP_CONTROL))
//				LogicRunInfApi(LOGIC_SET_HEART, &Heart);
//			else
//			{
//				if(RfidGetDataLimitlessSendFlag())	//zzw	清除心跳
//				{
//					RfidSetDataLimitlessSendEnd();
//				}
//			}
		}
//		else
//			;
		InterfacePeriodicProc(INTERFACE_TASK_TIME_SET+INTERFACE_TASK_TIME_SET);//支架控制器周期性处理工作
		
//		sprintf(s,"1:%6d 2:%3d",timestest,timesnoack);
//			ShowStr(0, (const u8 *)s, 0x00, 00);
	}
}
	
#endif
