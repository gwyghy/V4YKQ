/********************************************************************************
* �ļ�����	Interface.h
* ���ߣ�	������   
* �汾��   	V1.0
* ���ڣ�    2015.05.15  
* ��������: ����Interface.hͷ�ļ���
* 			
*			
* �޸�˵����   
*
*       >>>>  �ڹ����е�λ��  <<<<
*             3-Ӧ�ò�
*         ��   2-Э���
*             1-Ӳ��������
*********************************************************************************
* @copy
* <h2><center>&copy; COPYRIGHT ������������޹�˾ �з����� �����</center></h2>
*********************************************************************************/
#ifndef  __INTERFACE_H__
#define  __INTERFACE_H__

/********************************************************************************
* .hͷ�ļ�
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
* #define�궨��
*********************************************************************************/
#define 	SC_NUM_MAX 	250
#define		SC_NUM_MIN	1

/**��ʹ�õĽӿں�������***/
#define		LcdClear()					LcdClr(OLED_DISPLAY_NORMAL)
#define		LcdAreaClear				LcdAreaClr
#define		ShowStr						ShowStr16b//��ʾ�ַ���(�������ּ�ASCII�ַ�)
#define		ShowChar					ShowChar_ASCII16b//��ʾ��׼ASCII�ַ�
#define		ShowSymbol					ShowChar_Special//��ʾ�ض������ַ�	


/**��ʹ�õ�ָʾ�ƶ���****/
#define		ADJUST_CODE_ING_LED			LED_1
#define		ADJUST_CODE_SUCCEED_LED		LED_2
#define		KEY_SHIFT_LED				LED_3
#ifdef GENERAL_APP		//�������̵��ڳ���ʱ��סĳ���ٰ��������ᵼ�µ��쳣������  2019.08.29 parry
// 	#define   KEY_SC_GRP_OFF_LED   KEY_LED_2|KEY_LED_3|KEY_LED_4|KEY_LED_12			
// 	#define   KEY_SC_GRP_SHIFT_OFF_LED   KEY_LED_3|KEY_LED_4|KEY_LED_5|KEY_LED_6|KEY_LED_7|KEY_LED_8|KEY_LED_9|KEY_LED_10|KEY_LED_11
#endif

#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
	#define		FLASH_BULK_ERASER_TIME				(1800)//�����ʱ��
	#define 	TRANS_PROG_TIMES					0x01//ÿһ֡���ݷ��͵Ĵ���
	#define 	TRANS_RECV_PRG_HUNDRED_BYTES_TIME   522//ÿ���䡢����100�ֽڳ�����ʱ��352����	
	#define 	TRANS_RECV_PRG_WRITE_TIME   		25//ÿ����һ������д��ʱ��		
#endif
#define 	TRANS_RECV_ACTION_INFO_BYTES_TIME   252//ÿ���䡢����100�ֽڳ�����ʱ��352����	

/**INTERFCE����״̬ö��***/
typedef enum
{
	INTERFACE_MODE_NONE = 0x00, //��Ч
	
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
		INTERFACE_MODE_SC_UNCERTIFIED,//֧�ܿ�����δȷ��״̬(�Ƕ���״̬)1
	#endif	
	
	INTERFACE_MODE_SC_SET,//֧�ܿ��������ò���ģʽ		2
	INTERFACE_MODE_SC_SELF_CONTROL,//֧�ܿ������������ģʽ	3
	
	#if (ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)
		INTERFACE_MODE_SC_ZIBAO_CONTROL,//��������ң����ģʽ,������ʾ	4	
	#endif
	
	#if (LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG !=	DISABLED)
		INTERFACE_MODE_SC_NEIGHBOR_CONTROL,//֧�ܿ������ڼܿ���ģʽ5
	#endif
	
	#if (QUICK_KEY_CTRL_ENABLE_FLAG	!= DISABLED)
		INTERFACE_MODE_SC_SELF_QUICK,//���ܿ�ݲ���
	#endif

	#if (GRP_CTRL_ENABLE_FLAG != DISABLED)
		INTERFACE_MODE_SC_GRP_CONTROL,//֧�ܿ������������ģʽ	6
	#endif
	
	#if (SET_YKQ_PRGPT_USE_FLAG != DISABLED)
		INTERFACE_MODE_YKQ_SELF_CONTROL,//��������ң����ģʽ,������ʾ
		INTERFACE_MODE_YKQ_SET_CONTROL,//��������ң����ģʽ����������
	#endif
	
	INTERFACE_MODE_SELF_SET,//ң�����������ò���ģʽ7

	INTERFACE_MODE_SHUTSOWN,//�ػ�״̬(����͹���˯��ģʽ)8	

	INTERFACE_MODE_COLIBRATE_CODE_SUCCESS,//����ɹ�9
	INTERFACE_MODE_COLIBRATE_CODE_FAIL,//����ʧ��(������״̬��������д���ֻ�ǵ�������)a

	INTERFACE_MODE_INCHARGE_DISPLAY,//�����ʾ����b

	INTERFACE_MODE_IDLE ,//����(�޲���״̬)c
	INTERFACE_MODE_ININT ,//��ʼ��
	
	INTERFACE_MODE_SP ,//��ʼ��
	
	#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
		INTERFACE_MODE_TRANS_PRG,//���ͳ������
		INTERFACE_MODE_RECV_PRG_ING,//���ճ������
		INTERFACE_MODE_RECV_PRG_START,//��ʼ���ճ���,δ����
	#endif
	//�����ļ�ͬ��	zzw
	INTERFACE_MODE_ACTIONINFI_SYN,
	INTERFACE_MODE_WL_UNCONNECT,
	INTERFACE_MODE_RELIEVE_COLIBRATE,//�������

	INTERFACE_MODE_MAX
	
}INTERFACE_CONTROL_MODE_TYPE;

/**������ʾ��ʽ***/
typedef enum
{
	INTERFACE_SHOW_MAIN_NONE = 0x0000,//��Ч�Ĵ���ʽ
	INTERFACE_SHOW_MAIN_ALL = 0x0001,//��������ȫ��
	INTERFACE_SHOW_MAIN_BATTERY = 0x0002,//���µ��
	INTERFACE_SHOW_MAIN_RFIDSIGNAL= 0x0004,//�����ź�ǿ��
	INTERFACE_SHOW_MAIN_SCCOLIBRATE = 0x0008,//���¶���ܼܺ�(���ܺ�����)
	INTERFACE_SHOW_MAIN_NOTE = 0x0010,//����������ʾ��Ϣ
	
	INTERFACE_SHOW_MAIN_PROTECED = 0x0020,//����(����ʾ�䰵)
	INTERFACE_SHOW_MAIN_AWAKE = 0x0040,//����(��������)
	INTERFACE_SHOW_MAIN_SHUTDOWN = 0x0080,//�����ػ�

	INTERFACE_SHOW_YKQ_ID = 0x0100,//����ң�������
	INTERFACE_SHOW_YKQ_ID_NORMAL = 0x1100,//������ʾYKQ���
	INTERFACE_SHOW_YKQ_ID_INTERVAL = 0x2100,//������ʾYKQ���

	INTERFACE_SHOW_SC_SET = 0x0200,//�˵�����
	
	INTERFACE_SHOW_SC_SELF_CONTROL = 0x0400,//���ܿ���
	
	INTERFACE_SHOW_SC_NEAR_CONTROL = 0x0800,//�ڼܿ���
	
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
		INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL = 0x1000,//�ڼܿ�ݼ�����		
	#endif
	
	INTERFACE_SHOW_SC_GRP_CONTROL = 0x4000,//�������

	INTERFACE_SHOW_MAIN_SC_UNCOLIBRATE = 0x8000,//֧�ܿ������������

	
	#if (SET_YKQ_PRGPT_USE_FLAG != DISABLED)
		INTERFACE_SHOW_YKQ_SELF_CONTROL = 0x10000,//ң��������	
	#endif

	#if (ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)
		INTERFACE_SHOW_SC_ZIBAO_CONTROL = 0x20000,//ң��������			
	#endif
	INTERFACE_SHOW_INCHARGE_DISPLAY = 0x40000,//���ʱ��ʾ	

	#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
		INTERFACE_SHOW_SELF_TRANS_PRG = 0x80000,//���ͳ������
		INTERFACE_SHOW_SELF_RECV_PRG = 0x100000,//���ճ������
	#endif
	INTERFACE_SHOW_CTRL_MODE = 0x200000,		//����ģʽ		zzw
	INTERFACE_SHOW_MAIN_METHOD_MAX 
}INTERFACE_SHOW_MAIN_TYPE;

/**������Ϣö��**/
typedef enum
{
	INTERFACE_ACTION_NONE = 0,
	/**����Ϊ������Ϣ**/
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

/**������Ϣö��**/
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
		INTERFACE_NOTE_SYS_RESTART,//��������
		
		INTERFACE_NOTE_TRANS_RECV_HAVE_DONE,
		INTERFACE_NOTE_RECV_ERR,
		
		INTERFACE_NOTE_TRANS_STOP,		
		INTERFACE_NOTE_RECV_STOP,
	#endif
	
	WL_UNCONNECTED,  //����ģ��ͨ�Ŵ���
	
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

//�����ļ�����
__packed typedef struct
{
	u8 ActionType;				//��������
	u8 ActionID;				//����ID
	u8 ActionName[12];			//������
	u8 MannulCtrlMode;			//�ֶ�����
	u8 AutoCtrlMode;			//�Զ�����
	u8 ActionKey;				//����ֵ
	u8 ForbidActionType;		//��ֹ��������
	u8 ForbidActionID;			//��ֹ����id
}ActionMess;
//�����ļ�ͷ��
__packed typedef struct
{
	u8 Veron[4];		//�汾��Ϣ
	u8 ApplyScene;		//Ӧ�ó���
	u8 TotalAction;		//��������
	u16 Verify;			//У��
	u8 Nouse[24];		//Ԥ��
}ActionInfoHead;
//�����ļ���Ϣ
__packed typedef struct
{
	ActionInfoHead Head;
	ActionMess	Body[ACTION_NUM_MAX];
}ACTION_INFO;
/********************************************************************************
* ��������
*********************************************************************************/
#define NOKEYDOWN  0
#define FIRSTDOWN  1
#define SECONDDOWN 2

/********************************************************************************
* ����ȫ�ֲ���
*********************************************************************************/
extern OS_EVENT    *g_stSemInterface;//������ʾ�ź���

/********************************************************************************
* ��������
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
//�������ļ�
u8 ReadActionInfo(void);
//д�����ļ�
u8 WriteActionInfo(void);
//�������
u8 InterfaceShiftKeyLongDownPro(void);
#endif
