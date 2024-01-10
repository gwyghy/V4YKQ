/********************************************************************************
* 文件名：	Interface.h
* 作者：	马如意   
* 版本：   	V1.0
* 日期：    2015.05.15  
* 功能描述: 定义Interface.h头文件。
* 			
*			
* 修改说明：   
*
*       >>>>  在工程中的位置  <<<<
*             3-应用层
*         √   2-协议层
*             1-硬件驱动层
*********************************************************************************
* @copy
* <h2><center>&copy; COPYRIGHT 天津华宁电子有限公司 研发中心 软件部</center></h2>
*********************************************************************************/
#ifndef  __INTERFACE_H__
#define  __INTERFACE_H__

/********************************************************************************
* .h头文件
*********************************************************************************/
#include "BeepApp.h"
#include "KeyApp.h"
#include "Battery.h"
#include "InterfaceMenu.h"
#include "iwdg.h"
#ifdef HARDWARE_TEST
	#include "hardware_test.h"
#endif

/********************************************************************************
* #define宏定义
*********************************************************************************/
#define 	SC_NUM_MAX 	250
#define		SC_NUM_MIN	1

/**所使用的接口函数定义***/
#define		LcdClear()					LcdClr(OLED_DISPLAY_NORMAL)
#define		LcdAreaClear				LcdAreaClr
#define		ShowStr						ShowStr16b//显示字符串(包括汉字及ASCII字符)
#define		ShowChar					ShowChar_ASCII16b//显示标准ASCII字符
#define		ShowSymbol					ShowChar_Special//显示特定自制字符	


/**所使用的指示灯定义****/
#define		ADJUST_CODE_ING_LED			LED_1
#define		ADJUST_CODE_SUCCEED_LED		LED_2
#define		KEY_SHIFT_LED				LED_3
#ifdef GENERAL_APP		//消除键盘灯在成组时按住某键再按其他键会导致灯异常的现象  2019.08.29 parry
// 	#define   KEY_SC_GRP_OFF_LED   KEY_LED_2|KEY_LED_3|KEY_LED_4|KEY_LED_12			
// 	#define   KEY_SC_GRP_SHIFT_OFF_LED   KEY_LED_3|KEY_LED_4|KEY_LED_5|KEY_LED_6|KEY_LED_7|KEY_LED_8|KEY_LED_9|KEY_LED_10|KEY_LED_11
#endif

#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
	#define		FLASH_BULK_ERASER_TIME				(1800)//块擦除时间
	#define 	TRANS_PROG_TIMES					0x01//每一帧数据发送的次数
	#define 	TRANS_RECV_PRG_HUNDRED_BYTES_TIME   522//每传输、接收100字节程序用时：352毫秒	
	#define 	TRANS_RECV_PRG_WRITE_TIME   		25//每接收一包程序写入时间		
#endif
#define 	TRANS_RECV_ACTION_INFO_BYTES_TIME   252//每传输、接收100字节程序用时：352毫秒	

/**INTERFCE控制状态枚举***/
typedef enum
{
	INTERFACE_MODE_NONE = 0x00, //无效
	
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
		INTERFACE_MODE_SC_UNCERTIFIED,//支架控制器未确认状态(非动作状态)1
	#endif	
	
	INTERFACE_MODE_SC_SET,//支架控制器设置参数模式		2
	INTERFACE_MODE_SC_SELF_CONTROL,//支架控制器自身控制模式	3
	
	#if (ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)
		INTERFACE_MODE_SC_ZIBAO_CONTROL,//控制其他遥控器模式,界面显示	4	
	#endif
	
	#if (LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG !=	DISABLED)
		INTERFACE_MODE_SC_NEIGHBOR_CONTROL,//支架控制器邻架控制模式5
	#endif
	
	#if (QUICK_KEY_CTRL_ENABLE_FLAG	!= DISABLED)
		INTERFACE_MODE_SC_SELF_QUICK,//本架快捷操作
	#endif

	#if (GRP_CTRL_ENABLE_FLAG != DISABLED)
		INTERFACE_MODE_SC_GRP_CONTROL,//支架控制器成组控制模式	6
	#endif
	
	#if (SET_YKQ_PRGPT_USE_FLAG != DISABLED)
		INTERFACE_MODE_YKQ_SELF_CONTROL,//控制其他遥控器模式,界面显示
		INTERFACE_MODE_YKQ_SET_CONTROL,//控制其他遥控器模式，参数设置
	#endif
	
	INTERFACE_MODE_SELF_SET,//遥控器自身设置参数模式7

	INTERFACE_MODE_SHUTSOWN,//关机状态(进入低功耗睡眠模式)8	

	INTERFACE_MODE_COLIBRATE_CODE_SUCCESS,//对码成功9
	INTERFACE_MODE_COLIBRATE_CODE_FAIL,//对码失败(此两种状态，无需进行处理，只是单纯设置)a

	INTERFACE_MODE_INCHARGE_DISPLAY,//充电显示部分b

	INTERFACE_MODE_IDLE ,//空闲(无操作状态)c
	INTERFACE_MODE_ININT ,//初始化
	
	INTERFACE_MODE_SP ,//初始化
	
	#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
		INTERFACE_MODE_TRANS_PRG,//发送程序代码
		INTERFACE_MODE_RECV_PRG_ING,//接收程序代码
		INTERFACE_MODE_RECV_PRG_START,//开始接收程序,未接收
	#endif
	//配置文件同步	zzw
	INTERFACE_MODE_ACTIONINFI_SYN,
	INTERFACE_MODE_WL_UNCONNECT,
	INTERFACE_MODE_RELIEVE_COLIBRATE,//解除对码

	INTERFACE_MODE_MAX
	
}INTERFACE_CONTROL_MODE_TYPE;

/**主屏显示方式***/
typedef enum
{
	INTERFACE_SHOW_MAIN_NONE = 0x0000,//无效的处理方式
	INTERFACE_SHOW_MAIN_ALL = 0x0001,//更新主屏全部
	INTERFACE_SHOW_MAIN_BATTERY = 0x0002,//更新电池
	INTERFACE_SHOW_MAIN_RFIDSIGNAL= 0x0004,//更新信号强度
	INTERFACE_SHOW_MAIN_SCCOLIBRATE = 0x0008,//更新对码架架号(含架号增向)
	INTERFACE_SHOW_MAIN_NOTE = 0x0010,//更新主屏提示信息
	
	INTERFACE_SHOW_MAIN_PROTECED = 0x0020,//休眠(即显示变暗)
	INTERFACE_SHOW_MAIN_AWAKE = 0x0040,//唤醒(正常亮度)
	INTERFACE_SHOW_MAIN_SHUTDOWN = 0x0080,//主屏关机

	INTERFACE_SHOW_YKQ_ID = 0x0100,//设置遥控器编号
	INTERFACE_SHOW_YKQ_ID_NORMAL = 0x1100,//正常显示YKQ编号
	INTERFACE_SHOW_YKQ_ID_INTERVAL = 0x2100,//泛白显示YKQ编号

	INTERFACE_SHOW_SC_SET = 0x0200,//菜单设置
	
	INTERFACE_SHOW_SC_SELF_CONTROL = 0x0400,//单架控制
	
	INTERFACE_SHOW_SC_NEAR_CONTROL = 0x0800,//邻架控制
	
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
		INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL = 0x1000,//邻架快捷键控制		
	#endif
	
	INTERFACE_SHOW_SC_GRP_CONTROL = 0x4000,//成组控制

	INTERFACE_SHOW_MAIN_SC_UNCOLIBRATE = 0x8000,//支架控制器解除对码

	
	#if (SET_YKQ_PRGPT_USE_FLAG != DISABLED)
		INTERFACE_SHOW_YKQ_SELF_CONTROL = 0x10000,//遥控器控制	
	#endif

	#if (ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)
		INTERFACE_SHOW_SC_ZIBAO_CONTROL = 0x20000,//遥控器控制			
	#endif
	INTERFACE_SHOW_INCHARGE_DISPLAY = 0x40000,//充电时显示	

	#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
		INTERFACE_SHOW_SELF_TRANS_PRG = 0x80000,//发送程序代码
		INTERFACE_SHOW_SELF_RECV_PRG = 0x100000,//接收程序代码
	#endif
	INTERFACE_SHOW_CTRL_MODE = 0x200000,		//控制模式		zzw
	INTERFACE_SHOW_MAIN_METHOD_MAX 
}INTERFACE_SHOW_MAIN_TYPE;

/**动作信息枚举**/
typedef enum
{
	INTERFACE_ACTION_NONE = 0,
	/**以下为按键信息**/
	INTERFACE_ACTION_LEFT_NEIGHBOR,
	INTERFACE_ACTION_RIGHT_NEIGHBOR,
	INTERFACE_ACTION_LEFT_GRP,
	INTERFACE_ACTION_RIGHT_GRP,	
	INTERFACE_ACTION_MENU,
	INTERFACE_ACTION_START,	
	INTERFACE_ACTION_CERTIFY,
	INTERFACE_ACTION_STOP,		
	INTERFACE_ACTION_MENU_UP,
	INTERFACE_ACTION_MENU_DOWN,
	INTERFACE_ACTION_MENU_LEFT,
	INTERFACE_ACTION_MENU_RIGHT,
	INTERFACE_ACTION_MENU_N,	
	INTERFACE_ACTION_MENU_Q,	
	INTERFACE_ACTION_SHIFT,
	
	INTERFACE_ACTION_1,
	INTERFACE_ACTION_2,
	INTERFACE_ACTION_3,
	INTERFACE_ACTION_4,
	INTERFACE_ACTION_5,
	INTERFACE_ACTION_6,
	INTERFACE_ACTION_7,
	INTERFACE_ACTION_8,
	INTERFACE_ACTION_9,
	INTERFACE_ACTION_0,

	#if  (KEY_AUTO_PRESS_PROC_FLAG	!=	DISABLED)
		INTERFACE_ACTION_AUTO_PRESS,
	#endif
	
	INTERFACE_ACTION_ENUM_MAX
	
}INTERFACE_ACTION_TYPE;

/**运行信息枚举**/
typedef enum
{
	INTERFACE_NOTE_NONE = 0x00,
	INTERFACE_NOTE_SYSTEM_IDLE ,

	INTERFACE_NOTE_SET_YKQ_ID_START,
	INTERFACE_NOTE_SET_YKQ_ID_OUTSCOPE,
	INTERFACE_NOTE_SET_YKQ_ID_STOP,
	INTERFACE_NOTE_SET_YKQ_ID_SUCCESSED,
	
	INTERFACE_NOTE_SYSTEM_COLIBRATE_START,
	INTERFACE_NOTE_SYSTEM_COLIBRATE_FAIL,
	INTERFACE_NOTE_SYSTEM_COLIBRATE_STOP,
	INTERFACE_NOTE_SYSTEM_COLIBRATE_SUCCEED,
	INTERFACE_NOTE_SYSTEM_UNCOLIBRATE,
	INTERFACE_NOTE_SYSTEM_PASSIVE_UNCOLIBRATE,
	
	INTERFACE_NOTE_PLEASE_PRESS_BUTTON,
	INTERFACE_NOTE_PRESSED_BUTTON_INVAILD,

	INTERFACE_NOTE_BATTERY_INCHARGE,
	INTERFACE_NOTE_BATTERY_TOO_LOW,
	INTERFACE_NOTE_BATTERY_LOW_SHUTDOWN_10,
	INTERFACE_NOTE_BATTERY_LOW_SHUTDOWN_9,
	INTERFACE_NOTE_BATTERY_LOW_SHUTDOWN_8,
	INTERFACE_NOTE_BATTERY_LOW_SHUTDOWN_7,
	INTERFACE_NOTE_BATTERY_LOW_SHUTDOWN_6,
	INTERFACE_NOTE_BATTERY_LOW_SHUTDOWN_5,
	INTERFACE_NOTE_BATTERY_LOW_SHUTDOWN_4,
	INTERFACE_NOTE_BATTERY_LOW_SHUTDOWN_3,
	INTERFACE_NOTE_BATTERY_LOW_SHUTDOWN_2,
	INTERFACE_NOTE_BATTERY_LOW_SHUTDOWN_1,	
	INTERFACE_NOTE_BATTERY_FAULT,
	INTERFACE_NOTE_BATTERY_INCHARGE_OVER,
	INTERFACE_NOTE_BATTERY_LINE_OFF,
	INTERFACE_NOTE_AUTO_SHUTDOWN,
	
	INTERFACE_NOTE_SC_SET,
	INTERFACE_NOTE_SC_SELF_CONTROL,
	INTERFACE_NOTE_SC_NEIGHBOR_CONTROL,
	INTERFACE_NOTE_SC_GRP_CONTROL,

	INTERFACE_NOTE_NO_ACK,
	INTERFACE_NOTE_BUTTON_VALID,
	INTERFACE_NOTE_BUTTON_REJECT,
	INTERFACE_NOTE_BUTTON_INVALID,
	INTERFACE_NOTE_BUTTON_UP,
	INTERFACE_NOTE_BUTTON_DOWN,
	INTERFACE_NOTE_BUTTON_LONGDOWN,
	INTERFACE_NOTE_NOACK_UNCOLIBRATE,
	INTERFACE_NOTE_START_CERTIFY,
	
	INTERFACE_NOTE_BUTTON_SHIFT_DOWN,
	INTERFACE_NOTE_BUTTON_SHIFT_UP,
	
	INTERFACE_NOTE_SHUTDOWN,
	INTERFACE_NOTE_RFID_SEND_FAIL,

	INTERFACE_NOTE_QUICK_DISABLED,
	INTERFACE_NOTE_QUICK_ACTIONNUMB_NONE,
	INTERFACE_NOTE_QUICK_ACTIONTIME_NONE,

	INTERFACE_NOTE_ACTION_ALARAM,
	INTERFACE_NOTE_ACTION_ING,
	INTERFACE_NOTE_ACTION_JIANGYI_END,
	INTERFACE_NOTE_ACTION_JIANGYI_STOP,
	
	INTERFACE_NOTE_ZHONGJIANJIA_YKQ,	
	INTERFACE_NOTE_HUANING_YKQ,
	
	#if (ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)
		INTERFACE_NOTE_ZIBAO_DISABLED,
		INTERFACE_NOTE_ZIBAO_ACTION_DISABLED,
		INTERFACE_NOTE_ZIBAO_END,
		INTERFACE_NOTE_ZIBAO_TROPPO,
		INTERFACE_NOTE_ZIBAO_ACTION_STOP,		
	#endif
	
	#if (GRP_LALIU_BUTTONUP_STOP_FLAG != DISABLED)
		INTERFACE_NOTE_GRP_LALIU_START,
		INTERFACE_NOTE_GRP_LALIU_STOP,
	#endif

	#if (KEY_AUTO_PRESS_PROC_FLAG != DISABLED)
		INTERFACE_NOTE_AUTO_PRESS_ENABLED,
		INTERFACE_NOTE_AUTO_PRESS_DISABLED,
		INTERFACE_NOTE_AUTO_PRESS_FAIL,
		INTERFACE_NOTE_AUTO_PRESS_SUCCESSED,
	#endif
	
	#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
		INTERFACE_NOTE_SEARCH_YKQ_START,
		INTERFACE_NOTE_SEARCH_YKQ_FAIL,
		INTERFACE_NOTE_SEARCH_YKQ_STOP,
		INTERFACE_NOTE_SEARCH_YKQ_SUCCEED,
		
		INTERFACE_NOTE_PLEASE_DONOT_MOVE,
		INTERFACE_NOTE_SYS_RESTART,//即将重启
		
		INTERFACE_NOTE_TRANS_RECV_HAVE_DONE,
		INTERFACE_NOTE_RECV_ERR,
		
		INTERFACE_NOTE_TRANS_STOP,		
		INTERFACE_NOTE_RECV_STOP,
	#endif
	
	WL_UNCONNECTED,  //无线模块通信错误
	
	INTERFACE_NOTE_PLEASE_NEAR,
	
	INTERFACE_NOTE_AC_TO_MC,
	INTERFACE_NOTE_MC_TO_AC,
	INTERFACE_NOTE_START_ACTION,
	
	INTERFACE_NOTE_ENUM_MAX
	
}INTERFACE_NOTE_TYPE;

//zzw
#define ACTION_NUM_MAX		96

typedef enum
{
	NO_ACTION,
	SIGNAL_ACTION,
	COMBINATION_ACTION,
	GROUP_ACTION,
}ACTION_TYPE;

//配置文件主体
__packed typedef struct
{
	u8 ActionType;				//动作类型
	u8 ActionID;				//动作ID
	u8 ActionName[12];			//动作名
	u8 MannulCtrlMode;			//手动控制
	u8 AutoCtrlMode;			//自动动作
	u8 ActionKey;				//按键值
	u8 ForbidActionType;		//禁止动作类型
	u8 ForbidActionID;			//禁止动作id
}ActionMess;
//配置文件头部
__packed typedef struct
{
	u8 Veron[4];		//版本信息
	u8 ApplyScene;		//应用场景
	u8 TotalAction;		//动作个数
	u16 Verify;			//校验
	u8 Nouse[24];		//预留
}ActionInfoHead;
//配置文件信息
__packed typedef struct
{
	ActionInfoHead Head;
	ActionMess	Body[ACTION_NUM_MAX];
}ACTION_INFO;
/********************************************************************************
* 常量定义
*********************************************************************************/
#define NOKEYDOWN  0
#define FIRSTDOWN  1
#define SECONDDOWN 2

/********************************************************************************
* 声明全局参数
*********************************************************************************/
extern OS_EVENT    *g_stSemInterface;//界面显示信号量

/********************************************************************************
* 函数声明
*********************************************************************************/
u32 SetScrClosed(void);
u32 InterfaceSetAction(INTERFACE_ACTION_TYPE stActionType);
u32 InterfaceInsertNote(INTERFACE_NOTE_TYPE stNoteType);
u32 InterfaceSetCtrlMode(INTERFACE_CONTROL_MODE_TYPE stNewMode);
INTERFACE_CONTROL_MODE_TYPE InterfaceGetCtrlMode(void);
#if ((QUICK_KEY_CTRL_ENABLE_FLAG != DISABLED)||((ZIBAO_CTRL_ENABLE_FLAG != DISABLED)&&(ZIBAO_CTRL_METHOD_TYPE == ZIBAO_PARAM_CTRL_METHOD)))
	u32 InterfaceGetActionNameNumb(const u8 *s,u32 *pNumb);
	u32 InterfaceSetActionName(INTERFACE_ACTION_TYPE *pName);
	u32 InterfaceGetActionName(INTERFACE_ACTION_TYPE *pName);
#endif
#if ((ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)&&(ZIBAO_CTRL_METHOD_TYPE == ZIBAO_PARAM_CTRL_METHOD))
	u32 InterfaceGetZibaoActionKey(u32 *pData);
	u32 InterfaceSetZibaoActionKey(u32 *pData);
	u32 InterfaceSetZibaoRfidBackup(u32 *pData);
	u32 InterfaceSetZibaoKeyLedStatus(u32 sNewKey,u32 u32KeyStatus);
#endif
#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
	void InterfaceSetDownloadRemainTimer(u32 u32value);
	void InterfaceDecDownloadRemainTimer(u32 u32value);	
#endif	
u32 InterfaceSetUpdateMainMethod(u32  u32ShowType);
void Interface_Init(void);
void Interface_task(void *p_arg); 

uint8_t getColiScNumStatus(void);
void setColiScNumStatus(uint8_t tmp);
//读配置文件
u8 ReadActionInfo(void);
//写配置文件
u8 WriteActionInfo(void);
//解除对码
u8 InterfaceShiftKeyLongDownPro(void);
#endif
