/********************************************************************************
* �ļ�����	interface.c
* ���ߣ�	������   
* �汾��   	V1.0
* ���ڣ�    2015.05.15  
* ��������: ��ģ����ң�����˻������������ģ�顣��Ҫ�У�����������������������
* 			ʾ������������ȡ�
*			 
* �޸�˵����   
*
*       >>>>  �ڹ����е�λ��  <<<<
*             3-Ӧ�ò�
*         ��  2-Э���
*             1-Ӳ��������
*********************************************************************************
* @copy
* <h2><center>&copy; COPYRIGHT ������������޹�˾ �з����� �����</center></h2>
*********************************************************************************/
/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/
#ifndef __INTERFACE_C__
#define __INTERFACE_C__

/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/
#include "includes.h"
#include "InterfaceApp.h"

/********************************************************************************
* #define�궨��
*********************************************************************************/
/**�����ص������ź�ǿ�ȵķ�������***/
enum
{
	//��ص�������Ӧ���������
	INTERFACE_SYMBOL_NONE	= 0,//���κ��ַ�
	INETRFACE_BATTERY_SYMBOL_0 , 
	INETRFACE_BATTERY_SYMBOL_20, 
	INETRFACE_BATTERY_SYMBOL_40, 
	INETRFACE_BATTERY_SYMBOL_60, 
	INETRFACE_BATTERY_SYMBOL_80, 
	INETRFACE_BATTERY_SYMBOL_100, 
	INETRFACE_BATTERY_SYMBOL_WRONG,

	/**����Ϊ��������Ӧ���������**/
	INETRFACE_RFIDSIGNAL_SYMBOL_0 , 
	INETRFACE_RFIDSIGNAL_SYMBOL_20, 
	INETRFACE_RFIDSIGNAL_SYMBOL_40, 
	INETRFACE_RFIDSIGNAL_SYMBOL_60, 
	INETRFACE_RFIDSIGNAL_SYMBOL_80, 
	INETRFACE_RFIDSIGNAL_SYMBOL_100, 
	INETRFACE_RFIDSIGNAL_SYMBOL_WRONG	
};

/**������Ļˢ������***/
typedef enum
{
	INTERFACE_SCR_IDLE = 0,
	INTERFACE_SCR_AWAKE,
	INTERFACE_SCR_PROTECED,
	INTERFACE_SCR_CLOSED,
	INTERFACE_SCR_MAX
}INTERFACE_SCR_TYPE;

/********************************************************************************
* ��������
*********************************************************************************/

#define INVALID_ACTION    (0)
#define	INTERFACE_TASK_TIME_SET				10//����ִ������

#define	INTERFACE_PWR_TIME_SET				2000//������ʱ
#define	INTERFACE_INTERCTRL_TIMER_MAX		30000
#define	INTERFACE_SCR_TIMER_STOP			0xFFFFFFFF//������ʱ��ֹͣ����
#define	INTERFACE_NOTE_TIMER_STOP			0xFFFFFFFF//��ʾ��ʾ��Ϣ�����ʱ��
#define	INTERFACE_CTRL_TIMER_STOP			0xFFFFFFFF//���ؼ�ʱֹͣ����
#define	INTERFACE_COLIBRATE_TIMER_STOP		0xFFFFFFFF//�����ʱֹͣ����
#define	INTERFACE_AUTOSHUTDOWN_TIMER_STOP	0xFFFFFFFF//�Զ��ػ���ʱ��ֹͣ����

#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
	#define	INTERFACE_PRGSET_TIMER_SET		120000//���в��������Լ�������ĳ�ʱʱ��
#endif

/**���鶯����Ĭ�϶�����**/
#if (INTERFACE_DISPLAY_GRP_LAJIA_FLAG	== ENABLED)
	#define	INTERFACE_GRP_ACTION_DEFAULT		INTERFACE_ACTION_GRP_LA_JIA
#else
	#define	INTERFACE_GRP_ACTION_DEFAULT		INTERFACE_ACTION_GRP_YI_JIA
#endif

/**������ʾ�������Ƶ����ֵ**/
#define	INTERFACE_DISPLAY_ACTION_MAX		3

SIGNAL_ACTION_HEART Heart = {0};
/**��������Ӧ�ĵ�ص�����ʾ���Ŷ���(���ص���״̬һһ��Ӧ)**/
static const u16 c_u16InterfaceBatterSymbol[] = 
{
	INTERFACE_SYMBOL_NONE,//���κ��ַ�
	INETRFACE_BATTERY_SYMBOL_0, 
	INETRFACE_BATTERY_SYMBOL_20,
	INETRFACE_BATTERY_SYMBOL_40, 
	INETRFACE_BATTERY_SYMBOL_60, 
	INETRFACE_BATTERY_SYMBOL_80, 
	INETRFACE_BATTERY_SYMBOL_100,
	INETRFACE_BATTERY_SYMBOL_WRONG
};

/**�ź�ǿ������Ӧ���ź�ǿ����ʾ���Ŷ���(���ź�ǿ��״̬һһ��Ӧ)**/
static const u16 c_u16InterfaceRfidSignalSymbol[] = 
{
	INTERFACE_SYMBOL_NONE,//���κ��ַ�
	INETRFACE_RFIDSIGNAL_SYMBOL_0  , 
	INETRFACE_RFIDSIGNAL_SYMBOL_20, 
	INETRFACE_RFIDSIGNAL_SYMBOL_40, 
	INETRFACE_RFIDSIGNAL_SYMBOL_60, 
	INETRFACE_RFIDSIGNAL_SYMBOL_80, 
	INETRFACE_RFIDSIGNAL_SYMBOL_100, 
	INETRFACE_RFIDSIGNAL_SYMBOL_WRONG	
};

const u8 u8OperateTittle[13]="������Ϣ";
static const u8  u8OperatInfoMsg[INTERFACE_ACTION_ENUM_MAX][17] =
{
	/*��Ч*/
	"",
	/**����Ϊ������Ϣ**/
	"���ڼܼ�",
	"���ڼܼ�",	
	"������",
	"�ҳ����",	
	"�˵����ü�",
	"������",
	"ȷ�ϼ�",
	"ֹͣ��",	
	"�˵��ϼ�",
	"�˵��¼�",		
	"�˵����",
	"�˵��Ҽ�" ,
	"�˵�N/C��",
	"�˵�Q/D��",		
	"SHIFT��",	
	
	//"�˵��˵��˵��˵�"
	"1��",
	"2��",
	"3��",
	"4��",
	"5��",
	"6��",
	"7��",
	"8��",
	"9��",
	"0��",
	
	#if  (KEY_AUTO_PRESS_PROC_FLAG	!=	DISABLED)
		"�Զ���ѹ��",
	#endif
};
	
/**������Ϣ**/
const u8 u8NoteTittle[]="����״̬";
static const u8 u8RunInfoMsg[INTERFACE_NOTE_ENUM_MAX][17] =
{
	/*��Ч*/
 	"��Ч",	
	"δ����",
	"����WL���",
	"����Χ",
	"ֹͣ����",
	"������óɹ�",
	
	"���ڶ���",
	"����ʧ��",
	"����ֹͣ",
	"����ɹ�",
	"�������",
	"�����������",
	
	"�밴������",
	"������Ч",

	#if (BOARD_BATTERY_TEST == BOARD_BATTERY_COULOME_NUMBER)
		"�����",//��Я����ص����İٷֱȲ���
	#else
		"�����",
	#endif
	
	"������,����!",
	"��������,10s�ر�",
	"��������,9s�ر�",
	"��������,8s�ر�",
	"��������,7s�ر�",
	"��������,6s�ر�",
	"��������,5s�ر�",
	"��������,4s�ر�",
	"��������,3s�ر�",
	"��������,2s�ر�",
	"��������,1s�ر�",	
	"��ع���",
	"��������",
	"��������ѶϿ�",
	"�޲���,�Զ��ػ�",
	
	"֧�ܲ˵�����",
	"֧�ܵ��ܿ���",
	"֧���ڼܿ���",	
	"֧�ܳ������",	

	"��Ӧ��",
	"��������",
	"�����ܾ�",
	"������Ч",
	"����̧��",
	"��������",
	"��������",
	"��Ӧ��,�������",
	"��ȷ��",

	"shift����",
	"shifţ��",
	
	"�����ػ�",
	"�������ݷ���ʧ��",

	"δʹ��",
	"���κζ���",
	"��ִ��ʱ��",

	"Ԥ��",
	"������",
	"���������",
	"������ֹͣ",
	
	"�м���ֳ��ն�",//"�����ֳ��ն�"	
	"�����ֳ��ն�",

	#if (ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)
		"�Ա�����ֹ",
		"��������ֹ",
		"�Ա�ֹͣ",
		"��������Χ",
		"ĳ����ֹͣ",		
	#endif

	#if (GRP_LALIU_BUTTONUP_STOP_FLAG != DISABLED)
		"������������",
		"��������ֹͣ",
	#endif

	#if (KEY_AUTO_PRESS_PROC_FLAG != DISABLED)
		"�Զ���ѹ����",
		"�Զ���ѹ��ֹ",
		"����ʧ��",
		"���óɹ�",
	#endif	
	
	#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
		"��ʼ�����豸",
		"����ʧ��",
		"����ֹͣ",
		"�����ɹ�",
		
		"����",
		"��������",
		"�����",
		"����ʧ��",
		
		"ֹͣ����",
		"ֹͣ����",
		"����ģ���쳣",
		"�뿿��",
		"�ֶ�ģʽ",
		"�Զ�ģʽ",
		"������",
	#endif
};
//zzw
//
ActionMess ActionInfoMsg[INTERFACE_KEY_TOGETHER_MAX] = {0};
u8 RuningActionCnt = 0;
extern ACTION_INFO ActionInformation ;

/********************************************************************************
* ��������
*********************************************************************************/
OS_EVENT    *g_stSemInterface;//������ʾ�ź���
static INTERFACE_CONTROL_MODE_TYPE	s_stInterfaceControlMode;//�������ģʽ
static u32 s_u32MainUpdateMethod;//������ʾ��ʽ
static INTERFACE_ACTION_TYPE s_stInterfaceActionName[INTERFACE_KEY_TOGETHER_MAX];
static u64 s_u64InterfaceRfidDataBackup;//��һ�η������ݵı���
	
static u8 s_u8NoteItem[INTERFACE_NOTE_ENUM_MAX][17];//������Ϣ��ʾ��Ŀ
static u16 s_u16NoteItemWrite;//������Ϣ��ʾ��Ŀ����ָ��
static u16 s_u16NoteItemRead;//������Ϣ��ʾ��Ŀ��ȡָ��

static INTERFACE_SCR_TYPE s_stScrAwake;//������Ļ��־��ENABLE/DISABLE��
static u32 s_u32InterfaceColibrateTimer;//�����ʱ.0xFFFFFF:ֹͣ����
static u32 s_u32InterfaceControlTimer;//��Ļ�������߼�ʱ��.0xFFFFFF:ֹͣ����
static u32 s_u32ScrPrtectedTimer;//��Ļ�������߼�ʱ��.0xFFFFFF:ֹͣ����
static u32 s_u32NoteShowTimer;//��Ļ��ʾ��ʾ��Ϣʱ�ļ�ʱ

static u32 s_u32ShiftKeyEnableFlag;//����ʹ�ܱ�־
static u32 s_u32AllKeyStatus;//���а�����״̬�����»�̧��

static u32 s_u32ScCtrlKeyTimer;//֧�ܿ���������״̬�£�������ʱ��ʾ��ʱ�䡣������ʱ�䣬���˻ر��ܿ���״̬
static u32 s_u32SelfSetIntfState = INTERFACEMENU_STATE_MENU_START;//���ò���״̬����

static u32 s_u32AutoShutdownTimer;//�Զ��ػ���ʱ
u32 s_u32StartKeyFlag= KEY_UP;

static u32 s_u32ScGrpKeyValue = 0x00;		//�������̵��ڳ���ʱ��סĳ���ٰ��������ᵼ�µ��쳣������  2019.08.29 parry
static u32 s_u32ScGrpShiftKeyValue = 0x00;
static u8  s_u8ScGrpInvalidActionKeyNum = 0x00;


static u8 s_u8pleaseNearFlag = 0;

//static u32 KEY_SHENG_ZHU;		//������Ҫ����u16KeySetActionNameRam�еİ�����������ֵ	2020.01.15 parry			
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

u8 u8SwitchColiScNumStatus = NOKEYDOWN;//���¼Ӽ����л������ʱ��Ϊ���ܿ�����Ӧ�ٴΰ��¼Ӽ�����־λ

//extern u16 u16KeySetActionNameRam[LOGIC_KEYSET_ACTIONNUMB_MAX];
//extern u16 u16KeyFilmNameRam;
extern stEDIT_PAR stEdit;

u16 KEY_SC_GRP_OFF_LED; 	//����ģʽ�µ�һ�����в��ܳ���ļ��ĵ�ֵ	2020.04.27  parry
u16 KEY_SC_GRP_SHIFT_OFF_LED; 	//����ģʽ�µڶ������в��ܳ���ļ��ĵ�ֵ

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
	
	extern OS_STK  RfidMng_task_stk[RFIDMNG_TASK_STK_SIZE];			//���������ջ
	
#endif		


//u32 timestest = 0;
//u32  timesnoack = 0;
//extern OS_EVENT *s_stKeyTest ;   //���ڽ����ź���

/********************************************************************************
* ��������
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
/*INTERFACE״̬ģʽ�°���������ָ������*/
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
	//�����ļ�ͬ��
	InterfaceModeActionInfoSyn,	
	InterfaceModeWLUnConnectProc,

	NULL,
};

//zzw
/*******************************************************************************************
**�������ƣ�ReadActionInfo
**�������ã����ڲ�flash�ж������ļ�
**������������
**���������1������Ҫ����0���ɹ�
**ע�������
*******************************************************************************************/
u8 ReadActionInfo(void)
{
	//�������
	u8 temp[4];
	u32 Version;
	IapReadBuf(temp,ACTION_ALLOCATION_INFO_ADDRESS,0x04);
	Version = ((u32)temp[0]<<24)|((u32)temp[1]<<16)|((u32)temp[2]<<8)|((u32)temp[3]);
	if(Version == 0)
		return 1;
	//���flash��û�У����ö���ʹ���ڴ���Ĭ�ϵ�
	memset(&ActionInformation,0,sizeof(ActionInformation));
	IapReadBuf((u8 *)&ActionInformation,ACTION_ALLOCATION_INFO_ADDRESS,sizeof(ActionInformation));
	return 0;
}
/*******************************************************************************************
**�������ƣ�WriteActionInfo
**�������ã��ڲ�FLASH��д�������ļ�
**������������
**���������1��crc��ͬ������Ҫд�롣0���ɹ�
**ע�������
*******************************************************************************************/
u8 WriteActionInfo(void)
{
	u8 temp[4];
	u16 OldCRC = 0;
	//��ȡrom�������ļ�crc��Ϣ
	IapReadBuf(temp,ACTION_ALLOCATION_INFO_ADDRESS+4,0x04);
	OldCRC = (((u16)temp[2])<<8) |((u16)temp[3]);
	
	if(OldCRC == ActionInformation.Head.Verify)
		return 1;
	//д������
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
**�������ƣ�ActionKeyValueTurnAround
**�������ã�������ֵת��Ϊ�����ļ��еļ�ֵ
**����������stKey������ֵ��ShiftKeyFlag��shift����pKey��ת����İ���ֵ
**�����������������
**ע�������
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
									{KEY_55,0x00},{KEY_61,0x00},{KEY_62,0x00},{KEY_63,0x00},	//zzw 2023.12.24 �������ڼܼ���ɶ�����
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
	for(i =0;i<32;i++)			//ͳ��ͬʱ���µĸ���
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
//	if(ShiftKeyFlag == DISABLE)		//shif��δ����
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
//	else		//shif��δ����
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
**�������ƣ�ActionKeyValueTurnAround
**�������ã��������ļ��ļ�ֵת��Ϊʵ�ʼ�ֵ
**����������stKey������ֵ��ShiftKeyFlag��shift����pKey��ת����İ���ֵ
**�����������������
**ע�������
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
**�������ƣ�GetActionIDByKey
**�������ã�������ȡ�����ṹ��
**����������InterfaceCtrlMode���������档Key�������������ļ��У���Action����������
**�����������������
**ע�������
*******************************************************************************************/
u8 GetActionIDByKey(INTERFACE_CONTROL_MODE_TYPE InterfaceCtrlMode,u8 Key)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
		 OS_CPU_SR  cpu_sr = 0;
	#endif
	u8 i,CtrlMode;
	LogicParamApi(LOGIC_GET_CTRL_MODE,&CtrlMode);
	//���ܿ��ơ��ڼܿ���
	OS_ENTER_CRITICAL();
	if((InterfaceCtrlMode == INTERFACE_MODE_SC_UNCERTIFIED)
		||(InterfaceCtrlMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL))
	{
		for(i=0; i<ActionInformation.Head.TotalAction&&i<ACTION_NUM_MAX; i++)
		{
			if((ActionInformation.Body[i].ActionKey == Key)&&(ActionInformation.Body[i].ActionType != GROUP_ACTION))
			{
				//����ģʽ����
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
	//�������
	else if(InterfaceCtrlMode == INTERFACE_MODE_SC_GRP_CONTROL)
	{
		for(i=0; i<ACTION_NUM_MAX; i++)
		{
			if((ActionInformation.Body[i].ActionKey == Key)
				&&(ActionInformation.Body[i].ActionType == GROUP_ACTION))
			{
				//����ģʽ����
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
	//���ܿ��ơ��ڼܿ���
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
	//�������
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
**�������ƣ�Interface_Init
**�������ã������������µı������ź����ĳ�ʼ��
**������������
**�����������
**ע�������
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
	s_u32AutoShutdownTimer = 0x00;//�Զ��ػ���ʱ
	
	LogicParamApi(LOGIC_GET_OLED_SCR_LIGHT,&u16I);	
	OledContrastAdj(u16I);	//���Ȳ���			
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
	
	g_stSemInterface = OSSemCreate(0);	//������Ϣ��ʾ�ź���
}

/*********************************************************************************
** ��������:InterfaceGetKeyRfidData
** ������������ȡ����RFID�Ķ������Լ���ʾ����
** �������룺stMode:ģʽ��u32ShiftKeyStatus,shift״̬�u32KeyValue:����İ���
** ���������RFID_DATA_TYPE�����֣�*pInterAction:��ʾ������
** ע�����
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
		
	/***�жϳ��Ƿ��й��ܰ����������İ���ͬʱ����***/	
	/**2018.02.26��������***/	
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
	if((u32KeyValueBackup&KEY_STOP_SHUTDOWN) == KEY_STOP_SHUTDOWN)//ֹͣ�����ȼ����
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
		
	/**�ж��Ƿ����෴�Ķ���**/
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
		(stMode == INTERFACE_MODE_SC_UNCERTIFIED)||(stMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)) && ((0xCFFC & u32KeyValueBackup) == u32KeyValueBackup))	//jhy //��������ģʽ�µĶ����������෴�����ж� 0xCFFC������12�����������ļ�ֵ�� 
	{
		InterfaceTakeOutReverseKey(stMode,&u32KeyValueBackup);
	}
	
	for(u16K = 0; u16K < 32; u16K++)	//���ж����ֵʱ����ֿ�����������Ϊÿ����ֵ��Ӧһ��������ţ�ֻ�ж�����Ų��ܲ�ѯ�����߶����� 2020.01.15 parry
	{
		u32ShiftBit = (u32)(0x01 << u16K);
		if((u32KeyValueBackup & u32ShiftBit) == 0x00) 
			continue;
		else
			u32OneKeyOfKeyValue = u32ShiftBit;
		if(((stMode == INTERFACE_MODE_SC_SELF_CONTROL)  || (stMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)|| \
			(stMode == INTERFACE_MODE_SC_GRP_CONTROL) || (stMode == INTERFACE_MODE_SC_UNCERTIFIED)) && ((0xCFFC & u32OneKeyOfKeyValue) == u32OneKeyOfKeyValue))//jhy	//��������ģʽ�µĶ���������ת�� 0xCFFC������12�����������ļ�ֵ�� 
		{
			for(u16I = 0x00; u16I < 12; u16I++)		//����ֵת��Ϊ�������(0~11)	2020.01.15 parry
			{
				if(s_stInterfaceKeySNTable[u16I] == u32OneKeyOfKeyValue)
					break;
			}
//			if(u16I < 12)		//��12��������
//			{
//				if(u32KeyShiftBackup > 0)		//ͨ��������ź�shiftֵ�ó��������
//					u32OneKeyOfKeyValue = u16KeySetActionNameRam[u16I + 12];
//				else
//					u32OneKeyOfKeyValue = u16KeySetActionNameRam[u16I];
//			}
		}
		else if((stMode == INTERFACE_MODE_SC_SELF_CONTROL) && ((u32KeyValueBackup == KEY_MENU_START) | (u32KeyValueBackup == KEY_SHORTCUT_F3)) && (u32ShiftKeyStatus == ENABLED)) //һ����ú����12���������뵥������	2020.01.15 parry
		{
			u32OneKeyOfKeyValue = KEY_SHORTCUT_F3;
			u32KeyValueBackup = 0x00;
		}

		/**����������ִ�еĶ���**/
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
** ��������:InterfaceGetKeyLedValue
** ������������ȡ����ĳ��ģʽĬ�ϵ����İ���ָʾ��
** �������룺stMode:ģʽ��u32ShiftKeyStatus,shift״̬
** ���������RFID_DATA_TYPE�����֣�*pInterAction:��ʾ������
** ע�����
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
	
	/***���ֲ�ͬģʽ���д���***/
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
		else if(stMode == INTERFACE_MODE_SC_ZIBAO_CONTROL)//�Ա�ʱ��ȥ��ݼ�	
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
** ��������:InterfaceGetKeyRfidData
** ������������ȡ����RFID�Ķ������Լ���ʾ����
** �������룺stMode:ģʽ��u32ShiftKeyStatus,shift״̬�u32KeyValue:����İ���
** ���������RFID_DATA_TYPE�����֣�*pInterAction:��ʾ������
** ע�����
**********************************************************************************/
u64 InterfaceTakeOutReverseKey(INTERFACE_CONTROL_MODE_TYPE stMode,u32 *pKeyValue)  //ȥ���෴����	2020.01.15 parry
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
		
	/**�ж��Ƿ����෴�Ķ���**/
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
			for(u16K = 0; u16K < 32; u16K++)	//���ж����ֵʱ����ֿ�����������Ϊÿ����ֵ��Ӧһ��������ţ�ֻ�ж�����Ų��ܲ�ѯ�����߶�����
			{
				u32ShiftBit = (u32)(0x01 << u16K);
				if((u32KeyValueBackup & u32ShiftBit) == 0x00) 
					continue;
				else
					u32OneKeyOfKeyValue = u32ShiftBit;
				for(u16I = 0x00; u16I < 12; u16I++)		//����ֵת��Ϊ�������(0~11)	
				{
					if(s_stInterfaceKeySNTable[u16I] == u32OneKeyOfKeyValue)
						break;
				}
//				if(u16I < 12)		//��12��������
//				{
//					if(s_u32ShiftKeyEnableFlag > 0)		//ͨ��������ź�shiftֵ�ó��������
//						u32OneKeyOfKeyValue = (u32)u16KeySetActionNameRam[u16I + 12];
//					else
//						u32OneKeyOfKeyValue = (u32)u16KeySetActionNameRam[u16I];
//				}

				/**����������ִ�еĶ���**/
				for(u16I = 0x00;;u16I++)
				{
					if(p[u16I].stInterfaceCtrlMode == INTERFACE_MODE_NONE)
						break;

					if((p[u16I].stInterfaceCtrlMode == stMode)&&(u32OneKeyOfKeyValue == p[u16I].u32InterfaceKeyValue))
					{
						MultiKeyActionTable[u16J].u32KeyValueSingle = u32ShiftBit;	//��ÿ�����������д������ս�����������ֵ�붯��������Ĺ�ϵ��
						MultiKeyActionTable[u16J].u64RfidSendActionDataSingle = p[u16I].u64RfidSendActionData;
						u16J++;
					 
						u32KeyValueBackup ^= u32ShiftBit;
						break;
					}
				}
				if(u32KeyValueBackup == 0x00)	
					break;
			}
			
			if(u16J < 2)	//�����෴����
				return u64RfidData;
			else
			{
				u32KeyValueBackup = *pKeyValue;
				for(u16I = 0x00; u16I < (u16J - 1); u16I++)  //�ԡ�������ֵ�붯��������Ĺ�ϵ�������жϣ��Ƿ����s_stRfidReverseActionTable�е�Լ������
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
								return u64RfidData;		//����ͬʱ���ڶ���෴����
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
** ��������:SetScrAwake
** ����������������Ļ
** �������룺
** ���������None
** ע�����
**********************************************************************************/
u32 SetScrAwake(void)
{
	u16 u16Brightness;
	
	if ((s_stScrAwake == INTERFACE_SCR_PROTECED) || (s_stScrAwake == INTERFACE_SCR_CLOSED))
	{
		#if 1
			OledShutOn();
			LogicParamApi(LOGIC_GET_OLED_SCR_LIGHT,&u16Brightness);	
			OledContrastAdj(u16Brightness);	//���Ȳ���		
		#else
			LogicParamApi(LOGIC_GET_OLED_SCR_LIGHT,&u16Brightness);	
			OledContrastAdj(u16Brightness);	//���Ȳ���
		#endif
		
		s_stScrAwake = INTERFACE_SCR_AWAKE;
	}
	
	LogicParamApi(LOGIC_GET_OLED_PROTECTED_TIMER,&s_u32ScrPrtectedTimer);
	if(s_u32ScrPrtectedTimer == 0x00)
		s_u32ScrPrtectedTimer = INTERFACE_SCR_TIMER_STOP;
	
	s_u32AutoShutdownTimer = INTERFACE_AUTOSHUTDOWN_TIMER_STOP;//�Զ��ػ�ֹͣ����
	
	return 0x01;
}

/*********************************************************************************
** ��������:SetScrClosed
** �����������ر���Ļ
** �������룺
** ���������None
** ע�����
**********************************************************************************/
u32 SetScrClosed(void)
{
	LcdClear();
	
	OledContrastAdj(0x00);	//���Ȳ���
	
	s_stScrAwake = INTERFACE_SCR_CLOSED;
	
	return 0x01;
}
/*********************************************************************************
** ��������:SetScrProtected
** ������������Ļ����
** �������룺
** ���������None
** ע�����
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
			OledContrastAdj(u16Brightness);	//���Ȳ���
		
		s_stScrAwake = INTERFACE_SCR_PROTECED;
		s_u32ScrPrtectedTimer = INTERFACE_SCR_TIMER_STOP;

		LogicParamApi(LOGIC_GET_OLED_AUTOSHUTDOWN_TIMER,&s_u32AutoShutdownTimer);
		if(s_u32AutoShutdownTimer == 0x00)//��Ϊ0ʱ�������ػ�
			s_u32AutoShutdownTimer = INTERFACE_TASK_TIME_SET*2;	
	}

	return 0x01;	
}

/*********************************************************************************
** ��������:InterfaceInsertNote
** ����������������ʾ��Ϣ(�ڲ����ⲿ����ʹ��)
** �������룺
** ���������None
** ע�����
**********************************************************************************/
u32 InterfaceInsertNote(INTERFACE_NOTE_TYPE stNoteType)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
			OS_CPU_SR  cpu_sr = 0;
	#endif
	#if (BOARD_BATTERY_TEST == BOARD_BATTERY_COULOME_NUMBER)//�˱�־�����ڵ�includes��
		char s[20];
	#endif
	#if (BOARD_BATTERY_TEST == BOARD_BATTERY_COULOME_NUMBER)//�˱�־�����ڵ�includes��
		u16 u16Temp;
	#endif
	
	OS_ENTER_CRITICAL(); 

	s_u8NoteItem[s_u16NoteItemWrite][0x00] = 0x00;
	strcat((char *)&s_u8NoteItem[s_u16NoteItemWrite],(char *)&u8RunInfoMsg[stNoteType]);
	#if (BOARD_BATTERY_TEST == BOARD_BATTERY_COULOME_NUMBER)//�˱�־�����ڵ�includes��
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
** ��������:InterfaceSetCtrlMode
** ���������������������Ʒ�ʽ(�ڲ����ⲿ����ʹ��)
** �������룺stNewMode����״̬
** ���������None
** ע�����
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
				||(stNewMode == INTERFACE_MODE_ACTIONINFI_SYN))		//�����ļ�ͬ��	zzw
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
			LogicParamApi(LOGIC_GET_ZIBAO_TIMEOUT,&s_u32ScCtrlKeyTimer);//�˴���ȡ�Ա���ʱʱ��
			if(!s_u32ScCtrlKeyTimer)
				s_u32ScCtrlKeyTimer = INTERFACE_TASK_TIME_SET;	

			LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &s_u32InterfaceColibrateTimer);			
		}
	#endif
	
	OS_EXIT_CRITICAL();
	
	return 0x01;
}

/*********************************************************************************
** ��������:InterfaceGetCtrlMode
** ������������ȡ�������Ʒ�ʽ(�ڲ����ⲿ����ʹ��)
** �������룺stNewMode����״̬
** ���������None
** ע�����
**********************************************************************************/
INTERFACE_CONTROL_MODE_TYPE InterfaceGetCtrlMode(void)
{
  	return s_stInterfaceControlMode;
}

/*********************************************************************************
** ��������:InterfaceGetActionNameNumb
** ������������ȡ������ʾ���������к�(�ڲ����ⲿ����ʹ��)
** �������룺stNewMode����״̬
** ���������None
** ע�����
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
** ��������:InterfaceSetActionName
** �������������ø��������ķ�ʽ(�ڲ����ⲿ����ʹ��)
** �������룺u16All��ȫ����ʾ���ǲ�����ʾ
** ���������None
** ע�����
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
			s_stInterfaceActionName[u16I] = *pName;//Ĭ����Ļ��ʾ
			pName++;
		}

		OS_EXIT_CRITICAL();
		
		return 0x01;
	}
#endif

/*********************************************************************************
** ��������:InterfaceGetActionName
** �������������ø��������ķ�ʽ(�ڲ����ⲿ����ʹ��)
** �������룺u16All��ȫ����ʾ���ǲ�����ʾ
** ���������None
** ע�����
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
			*pName = s_stInterfaceActionName[u16I];//Ĭ����Ļ��ʾ
			pName++;
		}

		OS_EXIT_CRITICAL();
		
		return 0x01;
	}
#endif
/*********************************************************************************
** ��������:InterfaceGetZibaoActionKey
** �����������ض��Ա�ģʽ�İ����洢ֵ
** �������룺pData������ָ��
** ���������None
** ע�����
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
** ��������:InterfaceSetZibaoActionKey
** ���������������Ա�ģʽ�İ����洢ֵ
** �������룺pData������ָ��
** ���������None
** ע�����
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
** ��������:InterfaceSetZibaoActionKey
** ���������������Ա�ģʽ�İ����洢ֵ
** �������룺pData������ָ��
** ���������None
** ע�����
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
** ��������:InterfaceSetZibaoKeyLedStatus
** ���������������Ա�ģʽʱ��
** �������룺pData������ָ��
** ���������None
** ע�����
**********************************************************************************/
#if ((ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)&&(ZIBAO_CTRL_METHOD_TYPE == ZIBAO_PARAM_CTRL_METHOD))
static u32 ShowScZibaoKeyLedProc(u32 sNewKey,u32 u32KeyStatus,u32 u32ShiftKeyStatus);
u32 InterfaceSetZibaoKeyLedStatus(u32 sNewKey,u32 u32KeyStatus)
{
	return (ShowScZibaoKeyLedProc(sNewKey,u32KeyStatus,s_u32ShiftKeyEnableFlag));
}
#endif

/*******************************************************************************************
**�������ƣ�InterfaceSetDownloadRemainTimer
**�������ã�����s_u32DownloadRemainTimer��ֵ
**������������
**ע�������
*******************************************************************************************/
#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
	void InterfaceSetDownloadRemainTimer(u32 u32value)
	{
		#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
				OS_CPU_SR  cpu_sr = 0;
		#endif
		
		OS_ENTER_CRITICAL(); 
		
		s_u32DownloadRemainTimer = u32value;
		/**ֻ��ʾ10���������**/
		s_u32DownloadRemainTimerBackup = (u32value/1000);//��
		s_u32DownloadRemainTimerBackup /= 10;
		s_u32DownloadRemainTimerBackup ++;
		s_u32DownloadRemainTimerBackup ++;//ȷ��ÿ������ʱ�������ȷ��ʾ
		s_u32DownloadRemainTimerBackup *= 10;
		s_u32DownloadRemainTimerBackup *= 1000;		
		
		OS_EXIT_CRITICAL();		
	}
#endif

/*****************************************************************************************
**�������ƣ�InterfaceDecDownloadRemainTimer
**�������ã�����s_u32DownloadRemainTimer��ֵ
**������������
**ע�������
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
** ������   ��FormatRemainTimer()
** �������� ����ʽ��ʱ������
** ������� :	u16 u16Prs��ѹ������, u8 *u8dst
** ����ֵ   ����
** ����     ��
** ����     ��2013-7-24
** ��ע     ��
****************************************************************************************************************************************/
#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
	void FormatRemainTimer(u32 u32time, u8 *u8dst)
	{
		if(u32time)
		{
			u8dst[0] = (u8)((u32time /1000/ 60 /10) + 0x30);//���ӵĸ�λ
			u8dst[1] = (u8)((u32time /1000/ 60)%10 + 0x30);//���ӵĸ�λ
			u8dst[2] = (u8)((((u32time/1000) % 60)/10)+ 0x30);//���ʮλ
			u8dst[3] = (u8)((((u32time/1000) % 60)%10)+ 0x30);//��ĸ�λ
		}
		else
		{
			u8dst[0] = 0x30;//���ӵĸ�λ
			u8dst[1] = 0x30;//���ӵĸ�λ
			u8dst[2] = 0x30;//���ʮλ
			u8dst[3] = 0x30;//��ĸ�λ		
		}
	}
#endif

/*********************************************************************************
** ��������:SetUpdateMainMethod
** �������������ø��������ķ�ʽ(�ڲ����ⲿ����ʹ��)
** �������룺u16All��ȫ����ʾ���ǲ�����ʾ
** ���������None
** ע�����
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
** ��������:ClearUpdateMainMethod
** �������������ø��������ķ�ʽΪ��Ч(�ڲ�ˢ����Ļ֮���ʹ��)
** �������룺u16All��ȫ����ʾ���ǲ�����ʾ
** ���������None
** ע�����
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
** ��������:ShowMainAll
** ������������ʾ��Ļ��������Ϣ(���ݹ���ģʽ������Ļ�����и���)
** �������룺none
** ���������None
** ע�����
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
	
	/**����ģʽ�µ�ͨ�ô���**/
	LcdClear();
	//��ʾ�ź�ǿ��
	LogicRunInfApi(LOGIC_GET_RFID_SIGNAL_STATUS, &u16I);
	ShowSymbol(OLED_DISPLAY_NORMAL,c_u16InterfaceRfidSignalSymbol[u16I],0,4);

	LogicRunInfApi(LOGIC_GET_BATTERY_COULOM_STATUS,&u8I);
	ShowSymbol(OLED_DISPLAY_NORMAL,c_u16InterfaceBatterSymbol[u8I],0,112);
     //���߲���
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

	/**����ģʽ�Ĳ�ͬ����**/
	/**����ģʽ**/
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
			sprintf(s,"%d#,���ܿ���",u16J);
			u16Station = strlen(s);
			u16Station = (128-u16Station*8)/2;	
			ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 2, u16Station);
			
		}
	#endif
	else if(s_stInterfaceControlMode == INTERFACE_MODE_SC_SELF_CONTROL)
	{
		LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16J);
		#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
			sprintf(s,"%d#,���ܿ���",u16J);
		#else
			sprintf(s,"%d#,���ܿ���",u16J);
		#endif
		
		u16Station = strlen(s);
		u16Station = (128-u16Station*8)/2;	
		ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 2, u16Station);
		
	}	
	else if(s_stInterfaceControlMode == INTERFACE_MODE_SC_SET)
	{
		LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16J);
		sprintf(s,"%d#,�˵�����",u16J);
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
			if(u32GeJiaNumber== 0x00)//�ڼܲ���
			{
				if(u32ActionType  == SC_ACTION_LEFT_CTRL)
				{
					sprintf(s,"%d#,%s%d#",u16J,"����",u16I);//jhy
				}
				else if(u32ActionType  == SC_ACTION_RIGHT_CTRL)
				{
//					sprintf(s,"%d#,%s",u16J,"���ڼ�");
					sprintf(s,"%d#,%s%d#",u16J,"����",u16I);//jhy
				}
				else
				{
					sprintf(s,"%d#,%s",u16J,"���ܿ���");
				}
			}
			else
			{
				if(u32ActionType  == SC_ACTION_LEFT_CTRL)
				{
					sprintf(s,"%d#,%s%d#",u16J,"����",u16I);//jhy
//					sprintf(s,"%d#,���%d��",u16J,u32GeJiaNumber);
				}
				else if(u32ActionType  == SC_ACTION_RIGHT_CTRL)
				{
					sprintf(s,"%d#,%s%d#",u16J,"����",u16I);//jhy
//					sprintf(s,"%d#,�Ҹ�%d��",u16J,u32GeJiaNumber);
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
				u16Station= 16;//��ֹˢ������һ��
				s[16] = 0x00;
			}
			u16Station = (128-u16Station*8)/2;	
			ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 4, u16Station);			
			
		}	
	#endif	
	#if (GRP_CTRL_ENABLE_FLAG != DISABLED)	
	else if(s_stInterfaceControlMode == INTERFACE_MODE_SC_GRP_CONTROL)
	{
		LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u32ButtonValue);//���˼ܺ�
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
				u16Station= 16;//��ֹˢ������һ��
				s[16] = 0x00;
			}
			u16Station = (128-u16Station*8)/2;
			ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 2, u16Station);			
		}
		LcdAreaClear(OLED_DISPLAY_NORMAL,4,0,5,111);
		LogicRunInfApi(LOGIC_GET_GRP_ACTION_DIRECTION,&u16Direction);
		
	
		if((u16Direction == SC_GRP_ACTION_NEAR_REMOTE) || (u16Direction == SC_GRP_ACTION_REMOTE_NEAR))
		{
			LogicRunInfApi(LOGIC_GET_GRP_NEAR_SC_NUMB,&u16I);//���˼ܺ�
			LogicRunInfApi(LOGIC_GET_GRP_REMOTE_SC_NUMB,&u16J);//Զ�˼ܺ�
			LogicRunInfApi(LOGIC_GET_SC_NUMB_ADD_DIRCT, &u16K);//���ͷ���
			LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB, &u16Station);//����ܺ�

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
				strcat(s," ,������");
			u16Station = strlen(s);
			if(u16Station > 16)
			{	
				u16Station= 16;//��ֹˢ������һ��
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
				sprintf(s,"���ڴ������");
			else if(s_stInterfaceControlMode == INTERFACE_MODE_ACTIONINFI_SYN)
				sprintf(s,"�����ļ�ͬ��");
			else
				sprintf(s,"���ڽ��ճ���");			
			u16Station = strlen(s);
			u16Station = (128-u16Station*8)/2;	
			ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 2, u16Station);	

			memset(s,0x00,sizeof(s));
			ShowStr(0, "Լʣ��", 4, 8);	
			FormatRemainTimer(s_u32DownloadRemainTimer,(u8 *)s);
			ShowStr(OLED_DISPLAY_NORMAL,(const u8 *)s,4,56);
			ShowStr(OLED_DISPLAY_NORMAL,"��",4,72);
			ShowStr(OLED_DISPLAY_NORMAL,((const u8 *)s)+2,4,88);
			ShowStr(OLED_DISPLAY_NORMAL,"��",4,104);
		}	
	#endif
	#if (ZIBAO_CTRL_ENABLE_FLAG != DISABLED)
		else if(s_stInterfaceControlMode == INTERFACE_MODE_SC_ZIBAO_CONTROL)
		{
			LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16J);
			sprintf(s,"%d#,�����Ա�����",u16J);
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

	if(s_u8NoteItem[s_u16NoteItemRead] != u8RunInfoMsg[INTERFACE_NOTE_SYSTEM_IDLE])//ȷ����ʾ��Ϣ����ʾʱ��
	{
		LogicParamApi(LOGIC_GET_NOTE_SHOW_TIMER,&s_u32NoteShowTimer);
		if(s_u32NoteShowTimer == 0x00)
			s_u32NoteShowTimer = INTERFACE_NOTE_TIMER_STOP;			
	}	
	return u32ReturnFlag;
}

/*********************************************************************************
** ��������:ShowMainBattery
** �������������µ����Ϣ
** �������룺none
** ���������None
** ע�����
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
** ��������:ShowMainRfid
** �������������������ź���Ϣ 
** �������룺none
** ���������None
** ע�����
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
            //��ʾ�ź�ǿ��
            LogicRunInfApi(LOGIC_GET_RFID_SIGNAL_STATUS, &u16I);
        //	ShowSymbol(OLED_DISPLAY_NORMAL,c_u16InterfaceRfidSignalSymbol[u16I],0,4);		
            sprintf(s,"%d,%d",s8RssiSignalInftest,s8ScRssiSignal);
            ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)&s, 0, 0);	        
        }
        else
        {
            LcdAreaClear(OLED_DISPLAY_NORMAL,0,4,1,22);
            //��ʾ�ź�ǿ��
            LogicRunInfApi(LOGIC_GET_RFID_SIGNAL_STATUS, &u16I);
            ShowSymbol(OLED_DISPLAY_NORMAL,c_u16InterfaceRfidSignalSymbol[u16I],0,4);	        
        }
    }	
	else
    {
        LcdAreaClear(OLED_DISPLAY_NORMAL,0,4,1,22);
        //��ʾ�ź�ǿ��
        LogicRunInfApi(LOGIC_GET_RFID_SIGNAL_STATUS, &u16I);
        ShowSymbol(OLED_DISPLAY_NORMAL,c_u16InterfaceRfidSignalSymbol[u16I],0,4);		
    }
	return u32ReturnFlag;
}

/*********************************************************************************
** ��������:ShowMainScColibrate
** ��������������֧�ܿ������������Ϣ
** �������룺none
** ���������None
** ע�����
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
** ��������:ShowMainScUnColibrate
** ��������������֧�ܿ������������Ϣ
** �������룺none
** ���������None
** ע�����
**********************************************************************************/
u32  ShowMainScUnColibrate(u32 u32Type)
{
	u32 u32ReturnFlag = 0x01;

	/**�ж��Ƿ�������ִ�еĶ���***/
	if((RfidGetDataLimitlessSendFlag()))//���ڽ������ݵ����޴η���
	{
		RfidSetDataLimitlessSendEnd();
	}	
	
	/**ȷʵΪ�������***/
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
** ��������:ShowMainNote
** ����������������ʾ��Ϣ
** �������룺none
** ���������None
** ע�����
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
	if(s_u8NoteItem[s_u16NoteItemRead] != u8RunInfoMsg[INTERFACE_NOTE_SYSTEM_IDLE])//ȷ����ʾ��Ϣ����ʾʱ��
	{
		LogicParamApi(LOGIC_GET_NOTE_SHOW_TIMER,&s_u32NoteShowTimer);
		if(s_u32NoteShowTimer == 0x00)
			s_u32NoteShowTimer = INTERFACE_NOTE_TIMER_STOP;			
	}

	return u32ReturnFlag;
}

/*********************************************************************************
** ��������:ShowMainNoteDirectly
** ����������������ʾ��Ϣ
** �������룺none
** ���������None
** ע�����
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
** ��������:ShowMainYkqId
** ��������������ң����ID
** �������룺none
** ���������None
** ע�����
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
** ��������:ShowMainCtrlMode
** ��������������ң��������ģʽ
** �������룺none
** ���������None
** ע�����
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
** ��������:ShowScSet
** �������������´��ڲ������ý����µ���ʾ 
** �������룺none
** ���������None
** ע�����
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
		u16Station= 16;//��ֹˢ������һ��	
		s[16] = 0x00;
	}
	u16Station = (128-u16Station*8)/2;		
	ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 4, u16Station);		
		
	
	return u32ReturnFlag;
}

/*********************************************************************************
** ��������:ShowScUncertifiedCtrl
** �������������´��ڱ��ܷǶ����µ���ʾ
** �������룺none
** ���������None
** ע�����
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
		sprintf(s,"%d#,���ܿ���",u16J);				//zzw
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
		u16Station= 16;//��ֹˢ������һ��
		s[16] = 0x00;
	}
	u16Station = (128-u16Station*8)/2;	
	ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 4, u16Station);		
	return u32ReturnFlag;
}
#endif
/*********************************************************************************
** ��������:ShowScSelfCtrl
** �������������´��ڱ��ܿ����µ���ʾ
** �������룺none
** ���������None
** ע�����
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
		sprintf(s,"%d#,���ܿ���",u16J);
	#else
		sprintf(s,"%d#,���ܿ���",u16J);
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
		u16Station= 16;//��ֹˢ������һ��	
		s[16] = 0x00;
	}
	u16Station = (128-u16Station*8)/2;	
	ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 4, u16Station);			
	
	return u32ReturnFlag;
}

/*********************************************************************************
** ��������:ShowScNearCtrl
** �������������´����ڼܿ��ƽ����µ���ʾ
** �������룺none
** ���������None
** ע�����
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
	
	if(u32GeJiaNumber== 0x00)//�ڼܲ���
	{
		if(u32ActionType  == SC_ACTION_LEFT_CTRL)
		{
			sprintf(s,"%d#,%s%d#",u16J,"����",u16I);//jhy
//			sprintf(s,"%d#,%s",u16J,"���ڼ�");
		}
		else if(u32ActionType  == SC_ACTION_RIGHT_CTRL)
		{
			sprintf(s,"%d#,%s%d#",u16J,"����",u16I);//jhy
//			sprintf(s,"%d#,%s",u16J,"���ڼ�");
		}
		else
		{
			sprintf(s,"%d#,%s",u16J,"���ܿ���");
		}
	}
	else
	{
		if(u32ActionType  == SC_ACTION_LEFT_CTRL)
		{
			sprintf(s,"%d#,%s%d#",u16J,"����",u16I);//jhy
//			sprintf(s,"%d#,���%d��",u16J,u32GeJiaNumber);
		}
		else if(u32ActionType  == SC_ACTION_RIGHT_CTRL)
		{
			sprintf(s,"%d#,%s%d#",u16J,"����",u16I);//jhy
//			sprintf(s,"%d#,�Ҹ�%d��",u16J,u32GeJiaNumber);
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
		u16Station= 16;//��ֹˢ������һ��	
		s[16] = 0x00;
	}
	u16Station = (128-u16Station*8)/2;	
	ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 4, u16Station);	
	
	return u32ReturnFlag;
}

/*********************************************************************************
** ��������:ShowScGrpCtrl
** �������������´��ڳ�����ƽ����µ���ʾ 
** �������룺none
** ���������None
** ע�����
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
	
	LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u32ButtonValue);//���˼ܺ�
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
			u16Station= 16;//��ֹˢ������һ��	
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
			strcat(s," ,������");		
		u16Station = strlen(s);
		if(u16Station > 16)
		{	
			u16Station= 16;//��ֹˢ������һ��	
			s[16] = 0x00;
		}
		u16Station = (128-u16Station*8)/2;			
		ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 4, u16Station);	
	}

	return u32ReturnFlag;	
}

/*********************************************************************************
** ��������:ShowYkqSelfCtrl
** �������������´��ڱ��ܿ����µ���ʾ
** �������룺none
** ���������None
** ע�����
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
			sprintf(s,"%d#,ȫ����ң����",u16I);
		else
			sprintf(s,"%d#,΢��ң����",u16I);
		u16Station = strlen(s);
		u16Station = (128-u16Station*8)/2;	
		ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 2, u16Station);	

		memset(s,0x00,sizeof(s));
		LogicParamApi(LOGIC_GET_COLIBRATE_YKQ_VERSION,s);
		u16Station = strlen(s);
		if(u16Station > 16)
		{	
			u16Station= 16;//��ֹˢ������һ��
			s[16] = 0x00;
		}
		u16Station = (128-u16Station*8)/2;	
		ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 4, u16Station);			
			
		return u32ReturnFlag;
	}
#endif

/*********************************************************************************
** ��������:ShowScZibaoCtrl
** �������������´��ڱ����Ա������µ���ʾ
** �������룺none
** ���������None
** ע�����
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
	
	sprintf(s,"%d#,�����Ա�",u16J);

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
		u16Station= 16;//��ֹˢ������һ��	
		s[16] = 0x00;
	}
	u16Station = (128-u16Station*8)/2;
	ShowStr(OLED_DISPLAY_NORMAL, (const u8 *)s, 4, u16Station);		

	return u32ReturnFlag;
}
#endif
/*********************************************************************************
** ��������:ShowMainInchargeDisplayCtrl
** �������������´��ڹػ����ʱ����ʾ
** �������룺none
** ���������None
** ע�����
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
** ��������:ShowMainTransRecvPrgCtrl
** �����������������ճ���ʱ����������и��¡�
** �������룺u32Type,������������µķ�ʽ��
** ���������None
** ע�����
**********************************************************************************/
#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
	u32  ShowMainTransRecvPrgCtrl(u32 u32Type)
	{
		char s[20] ;
		u32 u32ReturnFlag = 0x00;
		
		LcdAreaClear(OLED_DISPLAY_NORMAL,4,0,5,127);

		memset(s,0x00,sizeof(s));
		ShowStr(0, "Լʣ��", 4, 8);	
		FormatRemainTimer(s_u32DownloadRemainTimer,(u8 *)s);
		ShowStr(OLED_DISPLAY_NORMAL,(const u8 *)s,4,56);
		ShowStr(OLED_DISPLAY_NORMAL,"��",4,72);
		ShowStr(OLED_DISPLAY_NORMAL,((const u8 *)s)+2,4,88);
		ShowStr(OLED_DISPLAY_NORMAL,"��",4,104);	
		
		return u32ReturnFlag;
	}
#endif

/*********************************************************************************
** ��������:ShowMain
** �������������ݲ�������������и��¡�
** �������룺*u32Type,������������µķ�ʽ��
** ���������None
** ע�����
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
		u32ReturnFlag |= SetScrProtected();//����
		(*u32Type) &= ~INTERFACE_SHOW_MAIN_PROTECED;
	}
	if((*u32Type & INTERFACE_SHOW_MAIN_AWAKE) == INTERFACE_SHOW_MAIN_AWAKE)
	{
		u32ReturnFlag |= SetScrAwake();//������Ļ
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
	if((*u32Type & INTERFACE_SHOW_SC_SET) == INTERFACE_SHOW_SC_SET)//�˵�����
	{
		u32ReturnFlag |= ShowScSet(*u32Type);
		(*u32Type) &= ~INTERFACE_SHOW_SC_SET;
	}
	if((*u32Type & INTERFACE_SHOW_SC_SELF_CONTROL) == INTERFACE_SHOW_SC_SELF_CONTROL)//�˵�����
	{
		u32ReturnFlag |= ShowScSelfCtrl(*u32Type);
		(*u32Type) &= ~INTERFACE_SHOW_SC_SELF_CONTROL;
	}
	
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
		if((*u32Type & INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL) == INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL)//���ܷǶ���״̬
		{
			u32ReturnFlag |= ShowScUncertifiedCtrl(*u32Type);
			(*u32Type) &= ~INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL;
		}	
	#endif
	
	if((*u32Type & INTERFACE_SHOW_SC_NEAR_CONTROL) == INTERFACE_SHOW_SC_NEAR_CONTROL)//�˵�����
	{
		u32ReturnFlag |= ShowScNearCtrl(*u32Type);
		(*u32Type) &= ~INTERFACE_SHOW_SC_NEAR_CONTROL;
	}
	if((*u32Type & INTERFACE_SHOW_SC_GRP_CONTROL) == INTERFACE_SHOW_SC_GRP_CONTROL)//�˵�����
	{
		u32ReturnFlag |= ShowScGrpCtrl(*u32Type);
		(*u32Type) &= ~INTERFACE_SHOW_SC_GRP_CONTROL;
	}
	if((*u32Type & INTERFACE_SHOW_MAIN_SC_UNCOLIBRATE) == INTERFACE_SHOW_MAIN_SC_UNCOLIBRATE)//�˵�����
	{
		u32ReturnFlag |= ShowMainScUnColibrate(*u32Type);
		(*u32Type) &= ~INTERFACE_SHOW_MAIN_SC_UNCOLIBRATE;
	}

	#if (SET_YKQ_PRGPT_USE_FLAG != DISABLED)
		if((*u32Type & INTERFACE_SHOW_YKQ_SELF_CONTROL) == INTERFACE_SHOW_YKQ_SELF_CONTROL)//�˵�����
		{
			u32ReturnFlag |= ShowMainConnetYkqSelfCtrl(*u32Type);
			(*u32Type) &= ~INTERFACE_SHOW_YKQ_SELF_CONTROL;
		}		
	#endif	
	#if 0		//�Ա�		zzw	
	#if (ZIBAO_CTRL_ENABLE_FLAG != DISABLED)
		if((*u32Type & INTERFACE_SHOW_SC_ZIBAO_CONTROL) == INTERFACE_SHOW_SC_ZIBAO_CONTROL)//�˵�����
		{
			u32ReturnFlag |= ShowScZibaoCtrl(*u32Type);
			(*u32Type) &= ~INTERFACE_SHOW_SC_ZIBAO_CONTROL;
		}	
	#endif
	#endif
	if((*u32Type & INTERFACE_SHOW_INCHARGE_DISPLAY) == INTERFACE_SHOW_INCHARGE_DISPLAY)//�˵�����
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
** ��������:ShowKeyLedProc
** �������������ݲ���ģʽ�������水��ָʾ�ƽ��и��¡�
** �������룺sMod,������������µķ�ʽ��
** ���������None
** ע�����
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
** ��������:ShowSlfSetKeyLedProc
** �������������ݲ���ģʽ�������水��ָʾ�ƽ��и��¡�
** �������룺sMod,������������µķ�ʽ��
** ���������None
** ע�����
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
** ��������:ShowKeyLedProc
** �������������ݲ���ģʽ�������水��ָʾ�ƽ��и��¡�
** �������룺sMod,������������µķ�ʽ��
** ���������None
** ע�����
**********************************************************************************/
#if ((YKQ_HARDWARE_KEYLED_EXIST_DEFINE == ENABLED)&&(INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED))
u32 ShowScUncertifiedKeyLedProc(u32 sNewKey,u32 u32KeyStatus,u32 u32ShiftKeyStatus)
{
	u32 u32Y = 0x00;
	u32 u32ButtonValue = 0x00;
	
	LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);
	if(u32Y == IN_SC_SS_NUMB)//������ʱ�����κο��Զ����İ���
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
** ��������:ShowKeyLedProc
** �������������ݲ���ģʽ�������水��ָʾ�ƽ��и��¡�
** �������룺sMod,������������µķ�ʽ��
** ���������None
** ע�����
**********************************************************************************/
#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE == ENABLED)
u32 ShowScSelfKeyLedProc(u32 sNewKey,u32 u32KeyStatus,u32 u32ShiftKeyStatus)
{
	u32 u32Y = 0x00;
	u32 u32ButtonValue = 0x00;

	/***�жϳ��Ƿ��й��ܰ����������İ���ͬʱ����***/	
	/**2018.02.26��������***/	
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
	if((sNewKey&KEY_STOP_SHUTDOWN) == KEY_STOP_SHUTDOWN)//ֹͣ�����ȼ����
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
** ��������:ShowKeyLedProc
** �������������ݲ���ģʽ�������水��ָʾ�ƽ��и��¡�
** �������룺sMod,������������µķ�ʽ��
** ���������None
** ע�����
**********************************************************************************/
#if ((YKQ_HARDWARE_KEYLED_EXIST_DEFINE == ENABLED)&&(QUICK_KEY_CTRL_ENABLE_FLAG	!= DISABLED))
u32 ShowScSelfQuickKeyLedProc(u32 sNewKey,u32 u32KeyStatus,u32 u32ShiftKeyStatus)
{

	return 0x01;
}
#endif

/*********************************************************************************
** ��������:ShowKeyLedProc
** �������������ݲ���ģʽ�������水��ָʾ�ƽ��и��¡�
** �������룺sMod,������������µķ�ʽ��
** ���������None
** ע�����
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
** ��������:ShowKeyLedProc
** �������������ݲ���ģʽ�������水��ָʾ�ƽ��и��¡�
** �������룺sMod,������������µķ�ʽ��
** ���������None
** ע�����
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
 							KeyLedOnOff(0x00,KEY_LED_ALL);	//shiftʱ��ͬʱ��ס������Ч����̧������֮һ��̧��ļ���һֱ��˸ 2020.03.04 parry
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
 							KeyLedOnOff(0x00,KEY_LED_ALL);	//ͬʱ��ס��Ч������Ч������̧����Ч����̧����Ч��ʱ�����쳣 2020.03.04 parry
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
** ��������:ShowKeyLedProc
** �������������ݲ���ģʽ�������水��ָʾ�ƽ��и��¡�
** �������룺sMod,������������µķ�ʽ��
** ���������None
** ע�����
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
**�������ƣ�InterfaceModeInintProc
**�������ã���ʾ�㴦�ڳ�ʼ��״̬�£������Ĵ���
**������������
**�����������
**ע�������
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
**�������ƣ�InterfaceModeIdleProc
**�������ã���ʾ�㴦�ڿ���״̬��,�����Ĵ���
**������������
**�����������
**ע�������
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
	if((sNewKey.u32Status == KEY_UP) )//����̧�𣬲�����ʶ��
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
	
	if((sNewKey.u32Key != KEY_STOP_SHUTDOWN) &&((sNewKey.u32Status == KEY_DOWN) ||((sNewKey.u32Status == KEY_LONGDOWN))))//ֹͣ�����£�������Ļ
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
				IrUsartChipPwrOn();//��������
			}
			else
			{
				WirelessChipPwrOn();
				GPIO_ResetBits(GPIOC, GPIO_Pin_6);//����ʹ��
			}
			if(g_u32ID != B_type)
				SendSetupToWl(RFID_ININT_SYNC1);
		}
		StateLed(KEY_SHIFT_LED,0x00);
		u32ReturnFlag = 0x01;
	}

	/**�������ģʽ�£�����ʶ��İ���**/
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


	else if((sNewKey.u32Key == KEY_SHIFT)&& (sNewKey.u32Status == KEY_LONGDOWN))		//�����
	{
		if(WLUnconnectedFlag)
		{		
			InterfaceInsertNote(WL_UNCONNECTED);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);
			ShowMain(&s_u32MainUpdateMethod);
			OSSemPost(g_stSemInterface);			
			return 0x01;
		}
#if 0			//����ɹ�zzw	��test������
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
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);//���ܿ���
			else
			{			
				InterfaceSetCtrlMode(INTERFACE_MODE_ACTIONINFI_SYN);//�����ļ�ͬ��
			}
#if (HEART == 1)				
				//����		
				Heart.ActionType = NO_ACTION;
				Heart.SendMode = NO_ACTION;
				Heart.ActionValue = 0;
#endif
			//zzw		�����ã�������ܺ�����Ϊ20
			u8J = 20;
			LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB, &u8J);
	
			InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_SUCCEED);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
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
		//Ԥ��ʱ�䣬������ir��RFIDӦ��ʱ��
		u32Y = (u32)(u16DelayTime*u16SendTimes);
		LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
		LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
		u32Y += (u32)(u16DelayTime*u16SendTimes);
		u32Y += 1000;//1000		
		while((u32Y > 0) && (u32Y < (u32)(0xffff))) //����������а������£��ᵼ��u32Y��Ϊ0xffffffff�Ӷ���ԭwhile(u32Y)�޷�ִ����� 2019.8.6�޸� Parry
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

		if(I8errKeyFlag == OS_ERR_NONE)//�а���̧����
		{
			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
				BuzzerOn(BEEP_KEY_PRESSED);
			#endif
			StateLed(0x00,ADJUST_CODE_ING_LED);
			InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_STOP);//����
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
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
		else if(err == OS_ERR_TIMEOUT)//��Ӧ��
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
		else if(err == OS_ERR_NONE)//��Ӧ��
		{
			/**�������õ�һЩ����ģʽ�л��Ĵ���*/
			if(InterfaceGetCtrlMode() == INTERFACE_MODE_COLIBRATE_CODE_FAIL)
			{
				#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
					BuzzerOn(BEEP_KEY_PRESSED);
				#endif
				StateLed(0x00,ADJUST_CODE_ING_LED);
				InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_FAIL);//����
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
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
			else if(InterfaceGetCtrlMode() == INTERFACE_MODE_COLIBRATE_CODE_SUCCESS)		//����ɹ�
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
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);//���ܿ���
				#if (HEART == 1)				
					//����		
					SIGNAL_ACTION_HEART Heart = {0};
					Heart.ActionType = NO_ACTION;
					Heart.SendMode = NO_ACTION;
					Heart.ActionValue = 0;
					LogicRunInfApi(LOGIC_SET_HEART, &Heart);
				#endif
				}
				else
				{			
					InterfaceSetCtrlMode(INTERFACE_MODE_ACTIONINFI_SYN);//�����ļ�ͬ��
				}	
				InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_SUCCEED);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
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
				InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_FAIL);//����
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
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
		OSTaskSuspend(SCAN_KEY_TASK_PRIO);//���𰴼�ɨ������
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
		/**�ж��Ƿ�Ϊ���ò����İ���**/
		if((sNewKey.u32Key == (KEY_PRGPT_SET_KEY1|KEY_PRGPT_SET_KEY2))&&(sNewKey.u32Status == KEY_LONGDOWN))		//������ս���
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
			/*�����ܹ���Ҫ����ʱ��*/
			InterfaceSetDownloadRemainTimer(0x0000);
			InterfaceInsertNote(INTERFACE_NOTE_PLEASE_DONOT_MOVE);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);
			OSSemPost(g_stSemInterface);

//			OS_ENTER_CRITICAL();
			if(g_u32ID != B_type)
				OSTaskSuspend(USART_TASK_PRIO);

			OSTaskSuspend(IRMNG_TASK_PRIO);
			OSTaskDel(RFIDMNG_TASK_PRIO);
	
			/**���߸��´�������**/
			if(g_u32ID == B_type)
			{			
				RFID_Init(RFID_PRG_ININT_SYNC0,RFID_PRG_ININT_SYNC1);
				RfidMngSetRecvCallBack();
			}
//			else
//			{
//				SendSetupToWl(RFID_PRG_ININT_SYNC1);				//������ͬ������ܵ�������ģ���յ���
//			}

			/**���߸��³���������**/
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
		if(!fastColiearateFlag)//jhy   ���ٶ���
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
				sprintf(s,"����ܺ�:%3d",fastColiearateScNum);
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
				//Ԥ��ʱ�䣬������RFIDӦ��ʱ��
				u32Y = 0x00;
				LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
				LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
				u32Y += (u32)(u16DelayTime*u16SendTimes);
				u32Y += 3000;//3000
					
				OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
				if(err == OS_ERR_TIMEOUT)//��Ӧ��
				{
					u32Y = LOGIC_STATUS_IDLE;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
					#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)				
						BuzzerOn(BEEP_UNCOLIBRAT);
					#endif
					u16DelayTime = RFID_SIGNAL_TOO_SMALL;
					LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DelayTime);	
					
					StateLed(0x00,ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED|KEY_SHIFT_LED);
					InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_FAIL);//����
					InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);				
					ShowMain(&s_u32MainUpdateMethod);		
					KeyLedOnOff(0x00, KEY_LED_ALL);
					s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
					
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//�Ƚ������
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
					OSTimeDly(200/TICK_TIME);
					
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//��������
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
									
					u32Y = LOGIC_COLIBRATE_SC_NONE;
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);
					
					SendSetupToWl(RFID_ININT_SYNC1);//jhyjhy

				}
				else if(err == OS_ERR_NONE)//��Ӧ��
				{
					/**�������õ�һЩ����ģʽ�л��Ĵ���*/
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
						InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_FAIL);//����
						InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);					
						ShowMain(&s_u32MainUpdateMethod);	
						KeyLedOnOff(0x00, KEY_LED_ALL);
						s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
						
//						LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&fastColiearateScNum);
//						u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//�Ƚ������
//						LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
//						OSTimeDly(200/TICK_TIME);
						
						LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&fastColiearateScNum);	
						u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//��������
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
						/**��ʹ�üӼ���������£�ֱ�ӽ��붯��״̬***/
						InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);//���ܿ���ģʽ	
						u8J = 0;
						LogicRunInfApi(LOGIG_GET_ACTIONINFO_CRC_IDENCAL, &u8J);
						if(u8J == 1)
						{							
							InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);//���ܿ���
						#if (HEART == 1)
							//����		
							Heart.ActionType = NO_ACTION;
							Heart.SendMode = NO_ACTION;
							Heart.ActionValue = 0;
							LogicRunInfApi(LOGIC_SET_HEART, &Heart);
						#endif
						}
						else
						{			
							InterfaceSetCtrlMode(INTERFACE_MODE_ACTIONINFI_SYN);//�����ļ�ͬ��
						}							
						u8J = MANUAL_CTRL;
						LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);		//zzw
						InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_SUCCEED);
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
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
						InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_FAIL);//����
						InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);
						u8J = NO_CTRL;
						LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);						
						ShowMain(&s_u32MainUpdateMethod);	
						KeyLedOnOff(0x00, KEY_LED_ALL);
						s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
						
						LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&fastColiearateScNum);
						u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//�Ƚ������
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
**�������ƣ�InterfaceModeSelfSetProc
**�������ã���ʾ�㴦�������������ģʽ�£������Ĵ���
**������������
**�����������
**ע�������
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

	/**���κ�ģʽ�£���ʶ��ػ���*/
	if(((sNewKey.u32Key&KEY_STOP_SHUTDOWN) == KEY_STOP_SHUTDOWN)&& (sNewKey.u32Status == KEY_LONGDOWN))
	{
		#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
			BuzzerOn(BEEP_SHUTDOWN_NOTE);
		#endif
		OSTaskSuspend(SCAN_KEY_TASK_PRIO);//���𰴼�ɨ������
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
**�������ƣ�InterfaceModeScSetProc
**�������ã���ʾ�㴦��֧�ܿ�������������ģʽ�£������Ĵ���
**������������
**�����������
**ע�������
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
	/*������Чʱ�Ĵ���**/
	if(u64RfidData == RFID_ACTION_NONE)
	{
		InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
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
	/**ʶ���������(����SHIFT���������)**/
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
		OSTaskSuspend(SCAN_KEY_TASK_PRIO);//���𰴼�ɨ������
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
	else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_UP))//��ֹͣ�������⴦��
	{
		if(u32StopKeyFlag == KEY_LONGDOWN)/**���κ�ģʽ�£���ʶ��ػ���*/
		{
			return u32ReturnFlag;			
	
		}
		else if(u32StopKeyFlag == KEY_DOWN)//ֹͣ��
		{
			/**��ֵ��ʾ**/
			LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
			LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//���͵�����
			g_stLogicSem->OSEventCnt = 0;
			u32Y = SC_BUTTON_ACK_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//��Ӧ����Ϊ��Ч	
			u64Y = RFID_ACTION_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u64Y);//��Ӧ��������Ϊ��Ч			
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
			if(err == OS_ERR_TIMEOUT)//��Ӧ��
			{
				InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);		
				ShowMain(&s_u32MainUpdateMethod);							
			}
			else if(err == OS_ERR_NONE)//��Ӧ��
			{
				/**�������õ�һЩ����ģʽ�л��Ĵ���*/
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
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
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
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
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
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ					
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
		/**��ֵ��ʾ**/
		LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
		LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//���͵�����		
		g_stLogicSem->OSEventCnt = 0;
		u32Y = SC_BUTTON_ACK_NONE;
		LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//��Ӧ����Ϊ��Ч	
		u64Y = RFID_ACTION_NONE;
		LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u64Y);//��Ӧ��������Ϊ��Ч
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
		if(err == OS_ERR_TIMEOUT)//��Ӧ��
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
		else if(err == OS_ERR_NONE)//��Ӧ��
		{
			/**�������õ�һЩ����ģʽ�л��Ĵ���*/
			LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
			LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u64ButtonValue);
			LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&u64ButtonAckValue);
			if((status == SC_BUTTON_ACK_ACCEPT) && (u64ButtonAckValue == u64ButtonValue))//��������
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
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
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
			s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ		
		InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SET);//��ʾ����������Ϣ
		ShowMain(&s_u32MainUpdateMethod);	
		ShowScSetKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);
	}
	else
		;
	
	return u32ReturnFlag;
}

//���������Ϣ
void ClearActionInfo(void)
{
	RuningActionCnt = 0;
	memset(ActionInfoMsg,0,sizeof(ActionInfoMsg));	
}
//ȷ�ϼ�����ģʽ�л���zzw
u8 InterfaceKeyCertifyPro(stKEY	sNewKey,u32 LogicRunStatus,INTERFACE_CONTROL_MODE_TYPE stNewMode,void *pDate)
{
	u8 u8J = 0xff;
	if((sNewKey.u32Key == KEY_CERTIFY)&& (sNewKey.u32Status == KEY_DOWN))		//��ȷ�ϼ������⴦��
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
			case LOGIC_STATUS_RFID_SC_SELF_CONTROL:		//���ܿ���
			{
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_CTRL_MODE);//��ʾ����������Ϣ
			}break;
			case LOGIC_STATUS_RFID_SC_NEIGHBORL:		//�ڼܿ���
			{
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_NEAR_CONTROL|INTERFACE_SHOW_CTRL_MODE);//��ʾ����������Ϣ
			}break;
			case LOGIC_STATUS_RFID_SC_GRP:				//�������
			{
//				GetActionInfoByID(1,INTERFACE_MODE_SC_GRP_CONTROL);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_GRP_CONTROL|INTERFACE_SHOW_CTRL_MODE);//��ʾ����������Ϣ
			}break;
			default:
				break;
		}
		InterfaceSetCtrlMode(stNewMode);
		LogicRunInfApi(LOGIC_SET_RUN_STATUS, &LogicRunStatus);				
		ShowMain(&s_u32MainUpdateMethod);	
	}

	if((sNewKey.u32Key == KEY_CERTIFY)&& (sNewKey.u32Status == KEY_UP))		//��ȷ�ϼ������⴦��
	{
		u8 err;
		for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
			s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ		
		ClearUpdateMainMethod();	
		LogicParamApi(LOGIC_GET_CTRL_MODE,&u8J);
		if(u8J == MANUAL_CTRL)
			InterfaceInsertNote(INTERFACE_NOTE_AC_TO_MC);
		else
			InterfaceInsertNote(INTERFACE_NOTE_MC_TO_AC);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
		switch(LogicRunStatus)
		{
			case LOGIC_STATUS_RFID_SC_SELF_CONTROL:		//���ܿ���
			{
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_CTRL_MODE);//��ʾ����������Ϣ
			}break;
			case LOGIC_STATUS_RFID_SC_NEIGHBORL:		//�ڼܿ���
			{
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_NEAR_CONTROL|INTERFACE_SHOW_CTRL_MODE);//��ʾ����������Ϣ
			}break;
			case LOGIC_STATUS_RFID_SC_GRP:				//�������
			{
				;//InterfaceSetUpdateMainMethod(INTERFACE_SHOW_CTRL_MODE);//��ʾ����������Ϣ
			}break;
			default:
				break;
		}
		ShowMain(&s_u32MainUpdateMethod);			
	}
	//����ָʾ����ʾ
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

//�ڼܼ�����
u8 InterfaceKeyNeighborPro(stKEY sNewKey,u32 LogicRunStatus,INTERFACE_CONTROL_MODE_TYPE stNewMode,void *pDate)
{
	u32 u32Y = 0,u32I = 0,u32K = 0,u32ActionType = 0;
	u16 u16SendTimes,u16DelayTime;
	u8 err;
	//�������ڼܼ���Ϊ����� zzw2023.12.25
	if(((sNewKey.u32Key == KEY_LEFT_GRP) || (sNewKey.u32Key == KEY_RIGHT_GRP)) && (sNewKey.u32Status == KEY_DOWN))		//�ڼܼ�		
	{
		if(stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
		{
			u32Y = 0x00;
			LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);//����ܼܺ�
			LogicRunInfApi(LOGIC_GET_GRP_NEAR_SC_NUMB,&u32I);//���˼ܺ�
			LogicRunInfApi(LOGIC_GET_GRP_ACTION_DIRECTION,&u32K);//����ִ�з���
			LogicRunInfApi(LOGIC_GET_SC_NUMB_ADD_DIRCT,&u32ActionType);//�ܺ�����			
			if(sNewKey.u32Key == KEY_LEFT_NEIGHBOR)//���ڼܼ�
			{	
				if(u32ActionType == SC_DIRCT_RIGHT)
				{
					if(u32I <= u32Y)//���˼ܺ�\����ܼܺ�
					{
						if(u32K == SC_GRP_ACTION_REMOTE_NEAR)//����ִ�з���
						{
							u32K = SC_GRP_ACTION_NEAR_REMOTE;
							LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION,&u32K);//����ִ�з���								
						}
					}
					else
					{
						if(u32K == SC_GRP_ACTION_NEAR_REMOTE)//����ִ�з���
						{
							u32K = SC_GRP_ACTION_REMOTE_NEAR;
							LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION,&u32K);//����ִ�з���								
						}
					}				
				}
				else
				{
					if(u32I >= u32Y)//���˼ܺ�\����ܼܺ�
					{
						if(u32K == SC_GRP_ACTION_REMOTE_NEAR)//����ִ�з���
						{
							u32K = SC_GRP_ACTION_NEAR_REMOTE;
							LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION,&u32K);//����ִ�з���
						}							
					}
					else
					{
		
						if(u32K == SC_GRP_ACTION_NEAR_REMOTE)//����ִ�з���
						{
							u32K = SC_GRP_ACTION_REMOTE_NEAR;
							LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION,&u32K);//����ִ�з���
						}				
					}				
				
				}						
			}
			else//�����Ҽ�
			{
				if(u32ActionType == SC_DIRCT_RIGHT)
				{
					if(u32I <= u32Y)//���˼ܺ�\����ܼܺ�
					{
						if(u32K == SC_GRP_ACTION_NEAR_REMOTE)//����ִ�з���
						{
							u32K = SC_GRP_ACTION_REMOTE_NEAR;
							LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION,&u32K);//����ִ�з���
						}
					}
					else
					{
						if(u32K == SC_GRP_ACTION_REMOTE_NEAR)//����ִ�з���
						{
							u32K = SC_GRP_ACTION_NEAR_REMOTE;
							LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION,&u32K);//����ִ�з���
						}							
					}				
				}
				else
				{
					if(u32I >= u32Y)//���˼ܺ�\����ܼܺ�
					{
						if(u32K == SC_GRP_ACTION_NEAR_REMOTE)
						{
							u32K = SC_GRP_ACTION_REMOTE_NEAR;
							LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION,&u32K);//����ִ�з���
						}
						
					}
					else
					{
		
						if(u32K == SC_GRP_ACTION_REMOTE_NEAR)
						{
							u32K = SC_GRP_ACTION_NEAR_REMOTE;
							LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION,&u32K);//����ִ�з���
						}
					}	
				}				
			}
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);				
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_GRP_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
			ShowMain(&s_u32MainUpdateMethod);		
		}
		else
		{
			LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32K);
			LogicRunInfApi(LOGIC_GET_SC_NUMB_ADD_DIRCT,&u32Y);
			LogicRunInfApi(LOGIC_GET_CONTRL_SC_NUMB,&u32I);  //jhy
			//��ֹ�ܺ���1��250���ˣ��ٰ��������ڼ�   jhy
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
			//���ö�������
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
				if(u32I == u32K)		//������ڼܿ������е��������⣬Ŀ������D�濿��ʵ��ûʲô��
					u32I--;
			}
			else
			{
				u32I++;	
				if(u32I == u32K)		
					u32I++;
			}
					//�������
			if(RfidGetDataLimitlessSendFlag())
			{
				RfidSetDataLimitlessSendEnd();
			}
			//������ʾ
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_DOWN);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
			ShowMain(&s_u32MainUpdateMethod);
					//�������
			if(RfidGetDataLimitlessSendFlag())
			{
				RfidSetDataLimitlessSendEnd();
			}
			//�·��ڼܿ�������
			g_stLogicSem->OSEventCnt = 0;
			LogicRunInfApi(LOGIC_SET_NEAR_CTRL_REQ, &u32I);
			//ˢ�µȴ�ʱ��
			LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
			LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
			u32Y = (u32)(u16DelayTime*u16SendTimes);	
			u32Y += 30;

			OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
			if(err == OS_ERR_TIMEOUT)//��Ӧ��
			{
				InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
				InterfaceSetCtrlMode(stNewMode);
				ShowMain(&s_u32MainUpdateMethod);	
				ShowScUncertifiedKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);	
			}
			else if(err == OS_ERR_NONE)//��Ӧ��
			{
//					LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32I);	
				/*Ϊ���޸Ŀ��Ƶļܺ�**/
				u32Y = LOGIC_STATUS_RFID_SC_NEIGHBORL;
				LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);					
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_NEAR_CONTROL|INTERFACE_SHOW_MAIN_NOTE);
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_NEIGHBOR_CONTROL);	
			}
			else
				;				
		}
		
		if(((sNewKey.u32Key == KEY_LEFT_GRP) || (sNewKey.u32Key == KEY_RIGHT_GRP)) && (sNewKey.u32Status == KEY_UP))			//�ڼܼ�
		{
			InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);
			InterfaceSetCtrlMode(INTERFACE_MODE_SC_NEIGHBOR_CONTROL);	
			
		}
		ShowMain(&s_u32MainUpdateMethod);
		#if (HEART == 1)		//����֡			
		{			
			Heart.ActionType = SIGNAL_ACTION;
			Heart.SendMode = 0;
			LogicRunInfApi(LOGIC_SET_HEART, &Heart);
		}	
		#endif	
	}
	return 1;
}

//���������		//zzw
u8 InterfaceKeyGrpCtrlPro(stKEY sNewKey,u32 LogicRunStatus,INTERFACE_CONTROL_MODE_TYPE stNewMode,void *pDate)
{
	u8 CtrlMode;
	u16 u16GrpRemoSc,u16GrpNearSc,u16AddDirct,GrpDirct;
	u32 u32Y;
	
	if(((sNewKey.u32Key == KEY_LEFT_GRP)|| (sNewKey.u32Key == KEY_RIGHT_GRP)) && ((sNewKey.u32Status == KEY_DOWN)))
	{
		if((stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)
			||(stNewMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL))		//����/�ڼܿ��ư��������ȡID1�Ķ�����Ϣ
		{
			//��������Ϣ
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
		
		LogicRunInfApi(LOGIC_GET_GRP_REMOTE_SC_NUMB,&u16GrpRemoSc);//Զ�˼ܺ�
		LogicRunInfApi(LOGIC_GET_GRP_NEAR_SC_NUMB,&u16GrpNearSc);//���˼ܺ�
		LogicRunInfApi(LOGIC_GET_SC_NUMB_ADD_DIRCT,&u16AddDirct);//�ܺ�����			
		if(sNewKey.u32Key == KEY_RIGHT_GRP)//�ҳ���
		{
			if(u16AddDirct == SC_DIRCT_LEFT)
			{
				if((LogicRunStatus == LOGIC_STATUS_RFID_SC_SELF_CONTROL)
					||(LogicRunStatus == LOGIC_STATUS_RFID_SC_NEIGHBORL))
				{
					//���ܿ���ʱ����Ҫ����Ĭ�Ϸ�Χ
					LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);
					u16GrpNearSc = u32Y > GRP_CTRL_MIN_SPACE?u32Y - GRP_CTRL_MIN_SPACE:1;
					u16GrpRemoSc = u32Y > GRP_CTRL_INIT_ARE?u32Y - GRP_CTRL_INIT_ARE:1;
					if((u16GrpNearSc==1)&&(u16GrpRemoSc == 1))
					{
						u16GrpNearSc = 0;
						u16GrpRemoSc = 0;
					}
					GrpDirct = SC_GRP_ACTION_NEAR_REMOTE;
					LogicRunInfApi(LOGIC_SET_GRP_REMOTE_SC_NUMB,&u16GrpRemoSc);//Զ�˼ܺ�
					LogicRunInfApi(LOGIC_SET_GRP_NEAR_SC_NUMB,&u16GrpNearSc);//���˼ܺ�
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
					//���ܿ���ʱ����Ҫ����Ĭ�Ϸ�Χ
					LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);
					u16GrpNearSc = u32Y + GRP_CTRL_MIN_SPACE>250?250:u32Y + GRP_CTRL_MIN_SPACE;
					u16GrpRemoSc = u32Y + GRP_CTRL_INIT_ARE>250?250:u32Y + GRP_CTRL_INIT_ARE;
					if((u16GrpNearSc==250)&&(u16GrpRemoSc == 250))
					{
						u16GrpNearSc = 0;
						u16GrpRemoSc = 0;
					}
					GrpDirct = SC_GRP_ACTION_NEAR_REMOTE;
					LogicRunInfApi(LOGIC_SET_GRP_REMOTE_SC_NUMB,&u16GrpRemoSc);//Զ�˼ܺ�
					LogicRunInfApi(LOGIC_SET_GRP_NEAR_SC_NUMB,&u16GrpNearSc);//���˼ܺ�
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
		else//�����Ҽ�
		{
			if(u16AddDirct == SC_DIRCT_LEFT)
			{
				if((LogicRunStatus == LOGIC_STATUS_RFID_SC_SELF_CONTROL)
					||(LogicRunStatus == LOGIC_STATUS_RFID_SC_NEIGHBORL))
				{
					//���ܿ���ʱ����Ҫ����Ĭ�Ϸ�Χ
					LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);
					u16GrpNearSc = u32Y + GRP_CTRL_MIN_SPACE>250?250:u32Y + GRP_CTRL_MIN_SPACE;
					u16GrpRemoSc = u32Y + GRP_CTRL_INIT_ARE>250?250:u32Y + GRP_CTRL_INIT_ARE;
					if((u16GrpNearSc==250)&&(u16GrpRemoSc == 250))
					{
						u16GrpNearSc = 0;
						u16GrpRemoSc = 0;
					}
					GrpDirct = SC_GRP_ACTION_NEAR_REMOTE;
					LogicRunInfApi(LOGIC_SET_GRP_REMOTE_SC_NUMB,&u16GrpRemoSc);//Զ�˼ܺ�
					LogicRunInfApi(LOGIC_SET_GRP_NEAR_SC_NUMB,&u16GrpNearSc);//���˼ܺ�	
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
					//���ܿ���ʱ����Ҫ����Ĭ�Ϸ�Χ
					LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);
					u16GrpNearSc = u32Y > GRP_CTRL_MIN_SPACE?u32Y - GRP_CTRL_MIN_SPACE:1;
					u16GrpRemoSc = u32Y > GRP_CTRL_INIT_ARE?u32Y - GRP_CTRL_INIT_ARE:1;
					if((u16GrpNearSc==1)&&(u16GrpRemoSc == 1))
					{
						u16GrpNearSc = 0;
						u16GrpRemoSc = 0;
					}
					GrpDirct = SC_GRP_ACTION_NEAR_REMOTE;
					LogicRunInfApi(LOGIC_SET_GRP_REMOTE_SC_NUMB,&u16GrpRemoSc);//Զ�˼ܺ�
					LogicRunInfApi(LOGIC_SET_GRP_NEAR_SC_NUMB,&u16GrpNearSc);//���˼ܺ�	
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
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_GRP_CONTROL);//��ʾ����������Ϣ
		InterfaceSetCtrlMode(INTERFACE_MODE_SC_GRP_CONTROL);	
	}
	if(((sNewKey.u32Key == KEY_LEFT_GRP)|| (sNewKey.u32Key == KEY_RIGHT_GRP)) && ((sNewKey.u32Status == KEY_UP)))
	{
	
	}
	ShowMain(&s_u32MainUpdateMethod);	
	return 1;
}

//ֹͣ������		//zzw
u8 InterfaceKeyShuntDownPro(u32 LogicRunStatus,void *pDate)
{
	u32 u32Y;
	u16 u16SendTimes,u16DelayTime,u16I;
	u32 u32ReturnFlag,status;
	COMMAND_ACTION ActionTemp = {0};
	u8 err,u8J;
	
	if((LogicRunStatus != LOGIC_STATUS_RFID_SC_SELF_CONTROL)			//�������水ֹͣ�ص����ܿ��ƽ���
		|| (ActionInfoMsg[0].ActionType == COMBINATION_ACTION))
	{
//		if(LogicRunStatus == LOGIC_STATUS_RFID_SC_NEIGHBORL)
		{
					//�������
			if(RfidGetDataLimitlessSendFlag())
			{
				RfidSetDataLimitlessSendEnd();
			}
			//�·��ڼܿ�������
			g_stLogicSem->OSEventCnt = 0;
			LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16I);	
			LogicRunInfApi(LOGIC_SET_NEAR_CTRL_REQ, &u16I);
			//ˢ�µȴ�ʱ��
			LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
			LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
			u32Y = (u32)(u16DelayTime*u16SendTimes);	
			u32Y += 30;

			OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
			if(err == OS_ERR_TIMEOUT)//��Ӧ��
			{
				InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
				ShowMain(&s_u32MainUpdateMethod);	
			}
			else if(err == OS_ERR_NONE)//��Ӧ��
			{
				/*Ϊ���޸Ŀ��Ƶļܺ�**/
				u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
				LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);					
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);

			}
			else
				;		
			#if (HEART == 1)	
				//����
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
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ				
		u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
		LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
		u32Y = 0x00;
		LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);	
		LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32Y);	
		ShowMain(&s_u32MainUpdateMethod);	
		ClearActionInfo();	
		
		ShowScUncertifiedKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
	}
	else		//���ܿ��ƽ��水ֹͣ���·�ֹͣ����
	{
		//zzw	�������
		if(RfidGetDataLimitlessSendFlag())
		{
			RfidSetDataLimitlessSendEnd();
		}
		/**��ֵ��ʾ**/
		g_stLogicSem->OSEventCnt = 0;
		u32Y = SC_BUTTON_ACK_NONE;
		LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//��Ӧ����Ϊ��Ч
		//ֹͣ�����·�
		ActionTemp.ActionType.Type.ActionType = 4;
		ActionTemp.OrderActionOrResult = ACTION_STOP;
		ActionTemp.ActionID = 0;
		LogicRunInfApi(LOGIC_SET_BUTTON_VALUE, &ActionTemp);
		//������ʾ����
		InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
		InterfaceInsertNote(INTERFACE_NOTE_BUTTON_DOWN);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);			
		ShowMain(&s_u32MainUpdateMethod);	
		LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
		LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
		u32Y = (u32)(u16DelayTime*u16SendTimes);	
		u32Y += 30;			
		
		OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);			
		if(err == OS_ERR_TIMEOUT)//��Ӧ��
		{
			InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL);//��ʾ����������Ϣ	
			ShowMain(&s_u32MainUpdateMethod);						
		}
		else if(err == OS_ERR_NONE)//��Ӧ��
		{
			/**�������õ�һЩ����ģʽ�л��Ĵ���*/
			status = 0;
			LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
			LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&ActionTemp);
			if((status == SC_BUTTON_ACK_ACCEPT)&& (ActionTemp.OrderActionOrResult == 1))		//�������������쳣
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
					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ							
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
					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ						
				ShowMain(&s_u32MainUpdateMethod);							
			}				
			else
			{
				InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ					
				u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
				LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
				for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ						
				ShowMain(&s_u32MainUpdateMethod);			
			}
		}
		else
			;	
		#if (HEART == 1)	
		//����
		Heart.ActionType = SIGNAL_ACTION;
		Heart.SendMode = 0;
		Heart.ActionValue = 0;
		LogicRunInfApi(LOGIC_SET_HEART, &Heart);
		#endif
	}
	return 1;
}

//���������´���	zzw
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
	//�������ļ��ж���
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
	//δ�ҵ���������Ʒ�ʽ��������ʾ
	if(RuningActionCnt == 0)
	{
		ActionInfoMsg[0] = ActionOldTemp;
		InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);		
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
		ShowMain(&s_u32MainUpdateMethod);					
	}
	else
	{
		if((stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
			|| (ActionInfoMsg[0].ActionType == COMBINATION_ACTION))
		{
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_DOWN);
			if(stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_GRP_CONTROL);//��ʾ����������Ϣ
			else if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL);//��ʾ����������Ϣ
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
			//zzw	�������
			if(RfidGetDataLimitlessSendFlag())
			{
				RfidSetDataLimitlessSendEnd();
			}
			for(i = 0;i < RuningActionCnt;i++)
			{
				//����������
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_DOWN);
				if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL);//��ʾ����������Ϣ
				else if(stNewMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_NEAR_CONTROL);//��ʾ����������Ϣ
				else if(stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_GRP_CONTROL);//��ʾ����������Ϣ
				else
					;
				ShowMain(&s_u32MainUpdateMethod);
				//֮ǰ���еĶ������Բ�������������������ң���������ˣ�����������ִ�еķ��ա�
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
				LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//��Ӧ��״̬��Ϊ��Ч	
				ActionTemp.ActionType.type = 0;
				ActionTemp.OrderActionOrResult = 0;
				ActionTemp.ActionID = 0;
				LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&ActionTemp);//��Ӧ��������Ϊ��Ч	
				
//				//�����·�
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
				//ˢ�µȴ�ʱ��
				LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
				LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
				u32Y = (u32)(u16DelayTime*u16SendTimes);	
				u32Y += 50;
				OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
				if(err == OS_ERR_TIMEOUT)//��Ӧ��
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
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//��ʾ����������Ϣ
					else if(stNewMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_NEAR_CONTROL);//��ʾ����������Ϣ
					else if(stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_GRP_CONTROL);//��ʾ����������Ϣ
					else
						;
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
					ShowMain(&s_u32MainUpdateMethod);				
					u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
					#endif
				}
				else if(err == OS_ERR_NONE)//��Ӧ��
				{
					/**�������õ�һЩ����ģʽ�л��Ĵ���*/
					status = 0;
					LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
					LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&ActionTemp);
					if((status == SC_BUTTON_ACK_ACCEPT)&& (ActionTemp.OrderActionOrResult == 1))		//�������������쳣
					{
						InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
						if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)	//��ʾ����������Ϣ
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
						if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)	//��ʾ����������Ϣ
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
						if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)	//��ʾ����������Ϣ
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
			//����֡����ͣ����������ݷ�����ȥ
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
//������̧����	zzw
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
	//�������ļ��ж���
	i = ActionKeyValueTurnAround(sNewKey.u32Key,s_u32ShiftKeyEnableFlag,key);
//	InterfaceInsertNote(INTERFACE_NOTE_BUTTON_UP);
//	InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
//	ShowMain(&s_u32MainUpdateMethod);
	for(j = 0;j < i;j++)
	{
		GetActionIDByKey(stNewMode,key[j]);
	}
	//δ�ҵ���������Ʒ�ʽ��������ʾ
	if(RuningActionCnt == 0)
	{
		InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);		
		if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL);//��ʾ����������Ϣ
		else if(stNewMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_NEAR_CONTROL);//��ʾ����������Ϣ
		else if(stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_GRP_CONTROL);//��ʾ����������Ϣ
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
		if(CtrlMode == AUTO_CTRL)//������ģʽΪ�Զ�ʱ������̧���������		zzw
		{
			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
				BuzzerOff();
			#endif
			ClearActionInfo();	
			InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);
			if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)	//��ʾ����������Ϣ
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
		else	//����ģʽΪ�ֶ�ʱ��̧���·�����ֹͣ
		{
			if(RfidGetDataLimitlessSendFlag())				//zzw		�������
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
				LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//��Ӧ��״̬��Ϊ��Ч	
				ActionTemp.ActionType.type = 0;
				ActionTemp.OrderActionOrResult = 0;
				ActionTemp.ActionID = 0;
				LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&ActionTemp);//��Ӧ��������Ϊ��Ч	
				//�����·�
				ActionTemp.ActionID = ActionInfoMsg[i].ActionID;
				ActionTemp.ActionType.Type.ActionType = ActionInfoMsg[i].ActionType;
				
				if(CtrlMode == MANUAL_CTRL)
					ActionTemp.ActionType.Type.CtrlMode = MANUAL_CTRL;
				else
					ActionTemp.ActionType.Type.CtrlMode = AUTO_CTRL;
				ActionTemp.OrderActionOrResult = ACTION_STOP;
				LogicRunInfApi(LOGIC_SET_BUTTON_VALUE, &ActionTemp);
				OS_EXIT_CRITICAL();

				if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)	//��ʾ����������Ϣ
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
				
				u32Y = 560+30;//��ʱ���뷢�͵�ʱ���й�jhyjhy
				OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
				if(err == OS_ERR_TIMEOUT)//��Ӧ��
				{
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_UP);
					if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//��ʾ����������Ϣ
					else if(stNewMode == INTERFACE_MODE_SC_NEIGHBOR_CONTROL)
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_NEAR_CONTROL);//��ʾ����������Ϣ
					else if(stNewMode == INTERFACE_MODE_SC_GRP_CONTROL)
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_GRP_CONTROL);//��ʾ����������Ϣ
					else
						;		
//					InterfaceSetCtrlMode(stNewMode);
				
					ShowMain(&s_u32MainUpdateMethod);	
				}
				else if(err == OS_ERR_NONE)//��Ӧ��
				{
					/**�������õ�һЩ����ģʽ�л��Ĵ���*/
					status = 0;
					LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
					LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&ActionTemp);
					if((status == SC_BUTTON_ACK_ACCEPT)&& (ActionTemp.OrderActionOrResult == 0x01))		//�������������쳣
					{
						InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
						if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)	//��ʾ����������Ϣ
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
			#if (HEART == 1)		//����֡			
			{			
				Heart.ActionType = SIGNAL_ACTION;
				Heart.SendMode = 0;
				LogicRunInfApi(LOGIC_SET_HEART, &Heart);
			}	
			#endif	
		}	
			
		ClearActionInfo();	
		if(stNewMode == INTERFACE_MODE_SC_UNCERTIFIED)	//��ʾ����������Ϣ
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


//�������	zzw
u8 InterfaceShiftKeyLongDownPro(void)
{
	u8 u8J;
	u32 u32Y;
	u16 u16DelayTime;
	
	//zzw	�������
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

//����ֹͣ��	zzw
u8 InterfaceStopKeyLongDownPro(void)
{
	u32 u32Y;
	#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
		BuzzerOn(BEEP_SHUTDOWN_NOTE);
	#endif
	//�Ƚ������
	InterfaceShiftKeyLongDownPro();
	OSTimeDly(200/TICK_TIME);			//������ʱ���ݷ�����ȥ		zzw
	OSTaskSuspend(SCAN_KEY_TASK_PRIO);//���𰴼�ɨ������
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

//����������������
u8 InterfaceGrpActionStartPro(void)
{
	u8 u8J,CtrlMode,err,status;
	u16 u16SendTimes = 0,u16DelayTime = 0,LogicRunStatus;
	u32 u32Y = 0,u32ReturnFlag = 0x01;	
	COMMAND_ACTION ActionTemp;
	
	if(ActionInfoMsg[0].ActionID == 0)	//�޶���ʱ����������
	{
		InterfaceInsertNote(INTERFACE_NOTE_PRESSED_BUTTON_INVAILD);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_MAIN_AWAKE);
		ShowMain(&s_u32MainUpdateMethod);
		
		return u32ReturnFlag;
	}
	//zzw	�������
	if(RfidGetDataLimitlessSendFlag())
	{
		RfidSetDataLimitlessSendEnd();
	}	
	LogicParamApi(LOGIC_GET_CTRL_MODE,&CtrlMode);
	//�����·�
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
	InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL|INTERFACE_SHOW_MAIN_AWAKE);//��ʾ����������Ϣ

	//�ر��ܿ��Ʋ��ѿ��Ʒ�ʽ�л�Ϊ�ֶ�
	InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
	u8J = MANUAL_CTRL;
	LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);
	u8J  = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
	LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u8J);	
	s_u32ShiftKeyEnableFlag = DISABLED;	
	StateLed(0x00,KEY_SHIFT_LED);

	ShowMain(&s_u32MainUpdateMethod);
	//ˢ�µȴ�ʱ��
	LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
	LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
	u32Y = (u32)(u16DelayTime*u16SendTimes);	
	u32Y += 30;
	LogicRunStatus = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
	OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
	if(err == OS_ERR_TIMEOUT)//��Ӧ��
	{
		InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
		InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);
		ShowMain(&s_u32MainUpdateMethod);	
		LogicRunInfApi(LOGIC_SET_RUN_STATUS,&LogicRunStatus);
	}
	else if(err == OS_ERR_NONE)//��Ӧ��
	{
		/**�������õ�һЩ����ģʽ�л��Ĵ���*/
		status = 0;
		LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
		LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&ActionTemp);
		if((status == SC_BUTTON_ACK_ACCEPT)&& (ActionTemp.OrderActionOrResult == 1))		//�������������쳣
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
	//�����������
	ClearActionInfo();
	{
		u16 u16I;
				//�������
		if(RfidGetDataLimitlessSendFlag())
		{
			RfidSetDataLimitlessSendEnd();
		}
		//�·��ڼܿ�������
		g_stLogicSem->OSEventCnt = 0;
		LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u16I);	
		LogicRunInfApi(LOGIC_SET_NEAR_CTRL_REQ, &u16I);
		//ˢ�µȴ�ʱ��
		LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
		LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
		u32Y = (u32)(u16DelayTime*u16SendTimes);	
		u32Y += 30;

		OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
		if(err == OS_ERR_TIMEOUT)//��Ӧ��
		{
			InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
			ShowMain(&s_u32MainUpdateMethod);	
		}
		else if(err == OS_ERR_NONE)//��Ӧ��
		{
			/*Ϊ���޸Ŀ��Ƶļܺ�**/
			u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);					
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);

		}
		s_u32ShiftKeyEnableFlag = DISABLE;
		ClearUpdateMainMethod();
		InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
		InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);					
		InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ				
		u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
		LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
		u32Y = 0x00;
		LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);	
		LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32Y);	
		ShowMain(&s_u32MainUpdateMethod);			
	}	
	//����
	Heart.ActionType = SIGNAL_ACTION;
	Heart.SendMode = 0;
	Heart.ActionValue = 0;
	LogicRunInfApi(LOGIC_SET_HEART, &Heart);	
	return u32ReturnFlag;
}

/*******************************************************************************************
**�������ƣ�InterfaceModeScUncertifiedProc
**�������ã���ʾ�㴦�ڱ��ܷǶ���ģʽ��,�����Ĵ���
**������������
**�����������
**ע�������
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
		//shift���̰�
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

		/**ʶ���������(����SHIFT���������)**/
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
		else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_UP))//��ֹͣ�������⴦��
		{
			if(u32StopKeyFlag == KEY_LONGDOWN)/**���κ�ģʽ�£���ʶ��ػ���*/
			{
				return u32ReturnFlag;			
		
			}
			else if(u32StopKeyFlag == KEY_DOWN)//ֹͣ��
			{
				InterfaceKeyShuntDownPro(LOGIC_STATUS_RFID_SC_SELF_CONTROL,NULL);
			
				u32ReturnFlag = 0x01;
					
			}
			u32StopKeyFlag = KEY_UP;
			ShowScUncertifiedKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);
		}
		//��ȷ�ϼ������⴦��	zzw
		else if((sNewKey.u32Key == KEY_CERTIFY)&& (sNewKey.u32Status == KEY_DOWN || sNewKey.u32Status == KEY_UP))		
		{
			u32ReturnFlag = InterfaceKeyCertifyPro(sNewKey,LOGIC_STATUS_RFID_SC_SELF_CONTROL,INTERFACE_MODE_SC_UNCERTIFIED,NULL);
		}		
		//�Գ���������⴦����������ڼܼ�ʹ�� zzw 2023.12.24
		else if(((sNewKey.u32Key == KEY_LEFT_GRP)|| (sNewKey.u32Key == KEY_RIGHT_GRP))&& ((sNewKey.u32Status == KEY_DOWN)))		
		{
//			InterfaceKeyGrpCtrlPro(sNewKey,LOGIC_STATUS_RFID_SC_SELF_CONTROL,INTERFACE_MODE_SC_UNCERTIFIED,NULL);
//			u32ReturnFlag = 0x01;	
//			//����ָʾ��
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
		else if(((sNewKey.u32Key == KEY_INCREASE)|| (sNewKey.u32Key == KEY_DECREASE))&& ((sNewKey.u32Status == KEY_DOWN)))	//��+-�������⴦��
		{
			//zzw	�������
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
			//Ԥ��ʱ�䣬������RFIDӦ��ʱ��
			u32Y = 0x00;
			LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
			LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
			u32Y += (u32)(u16DelayTime*u16SendTimes);

			u32Y+= 3000;	//�޸�
			g_stLogicSem->OSEventCnt = 0x00;
			u8tmp = getColiScNumStatus();
			u8tmp++;
			setColiScNumStatus(u8tmp);
			
			OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);
			setColiScNumStatus(0);
			StateLed(ADJUST_CODE_ING_LED,ADJUST_CODE_SUCCEED_LED);	
			if(err == OS_ERR_TIMEOUT)//��Ӧ��
			{
				s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
				
				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
				u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//�Ƚ������
				LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
				OSTimeDly(200/TICK_TIME);
				
				//jhyjhy
				LogicRunInfApi(LOGIC_GET_CONLIBRATE_SUCCESS_SC_NUMB,&u16ScNumbOldBackup); 
				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);
				
				InterfaceInsertNote(INTERFACE_NOTE_PLEASE_NEAR);

				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
				u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
				LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
				u32Y = 0x00;
				LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);	
				LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32Y);	
				ShowMain(&s_u32MainUpdateMethod);
				ShowStr(0, (const u8 *)"����ʧ��!", 0x04, 32);
				s_u8pleaseNearFlag = 1;
				OSTimeDly(100/TICK_TIME);
					
				ShowScUncertifiedKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);					

			}
			else if(err == OS_ERR_NONE)//��Ӧ��
			{
				/**�������õ�һЩ����ģʽ�л��Ĵ���*/
				if(InterfaceGetCtrlMode() == INTERFACE_MODE_COLIBRATE_CODE_FAIL)
				{
					s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
					
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//�Ƚ������
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
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
					u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
					u32Y = 0x00;
					LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);	
					LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32Y);	
					ShowMain(&s_u32MainUpdateMethod);
					ShowStr(0, (const u8 *)"����ʧ��!", 0x04, 32);
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
					
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);//���ܿ���ģʽ	
 
					InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_SUCCEED);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
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
//							u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//�Ƚ������
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
//							u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//�Ƚ������
//							LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);	
//							OSTimeDly(150/TICK_TIME);
//							u16temptest--;
//						}
//					}
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);
					
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//�Ƚ������
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
	
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);//���ܿ���ģʽ						
					LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
					s_u32InterfaceColibrateTimer = u32Y;
					u32Y = LOGIC_COLIBRATE_SC_IGNORE;
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);
					//���+-�����ٰ�����		zzw
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//�Ƚ������
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
					InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_FAIL);//����
					InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);
					u8J = NO_CTRL;
					LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);
					ShowMain(&s_u32MainUpdateMethod);	
					KeyLedOnOff(0x00, KEY_LED_ALL);
					s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
					
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//�Ƚ������
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
					OSTimeDly(200/TICK_TIME);
					
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);	
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//��������
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
			#if (HEART == 1)		//����֡			
			{			
				Heart.ActionType = SIGNAL_ACTION;
				Heart.SendMode = 0;
			}	
			#endif	
		}	
		
		#if (LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG != DISABLED)	//�ڼܼ���������ʹ��
//		else if(((sNewKey.u32Key == KEY_LEFT_NEIGHBOR)|| (sNewKey.u32Key == KEY_RIGHT_NEIGHBOR))&& ((sNewKey.u32Status == KEY_DOWN)))	//���ڼܼ������⴦��
//		{
//			if(!InterfaceKeyNeighborPro(sNewKey,LOGIC_STATUS_RFID_SC_SELF_CONTROL,INTERFACE_MODE_SC_UNCERTIFIED,NULL))
//					return 0;
//			u32ReturnFlag = 0x01;	
//			
//		}	
		#endif
		else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_MENU_START))//����������
		{
			InterfaceGrpActionStartPro();	
			ShowScUncertifiedKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);	
		}	
		//����������
		else if(sNewKey.u32Status == KEY_DOWN)		//��ʣ�ఴ��
		{
			u32ReturnFlag = 0x01;					
			//�������		zzw
			ClearActionInfo();				
			InterfaceActionKeyDownPro(sNewKey,LOGIC_STATUS_RFID_SC_SELF_CONTROL,INTERFACE_MODE_SC_UNCERTIFIED,NULL);
			ShowScUncertifiedKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);
		}
		//������̧��		//zzw2023.12.25 ȥ�������ڼܣ���Ϊ������
		else if(sNewKey.u32Status == KEY_UP&&(sNewKey.u32Key != KEY_SHIFT)&&(sNewKey.u32Key != KEY_CERTIFY)\
			&&(sNewKey.u32Key != KEY_INCREASE)&&(sNewKey.u32Key != KEY_DECREASE)\
			&&(sNewKey.u32Key != KEY_LEFT_GRP)&&(sNewKey.u32Key != KEY_RIGHT_GRP)\
			/*&&(sNewKey.u32Key != KEY_LEFT_NEIGHBOR)&&(sNewKey.u32Key != KEY_RIGHT_NEIGHBOR)*/\
			&&(sNewKey.u32Key != KEY_MENU_START))
		{
			//�������		zzw
			ClearActionInfo();			
			InterfaceActionKeyUpPro(sNewKey,LOGIC_STATUS_RFID_SC_SELF_CONTROL,INTERFACE_MODE_SC_UNCERTIFIED,NULL);	
			ShowScUncertifiedKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);
			u32ReturnFlag = 0x01;
		}	
		else
			;	
        //���߲���ʹ�ܣ��������߲���֡
        LogicParamApi(LOGIC_GET_WL_TEST_ENABLE, &u32Y);
        if((u8)(u32Y &0x01) == ENABLE)
        {
            //zzw	�������
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
**�������ƣ�InterfaceModeScSelfQuickProc
**�������ã���ʾ�㴦�ڱ��ܿ�ݲ���ģʽ��,�����Ĵ���
**������������
**�����������
**ע�������
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
		/*������Чʱ�Ĵ���**/
		if(u32RfidData == RFID_ACTION_NONE)
		{
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
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
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ	
			ShowMain(&s_u32MainUpdateMethod);			
			return u32ReturnFlag;
		}
		
		s_u64InterfaceRfidDataBackup = (u32RfidData|(stRfidType<<30));
		
		if((sNewKey.u32Key == KEY_QUICK_CTRL_1) && (sNewKey.u32Status == KEY_UP))//��ʱֹͣ���ж����ķ���
		{
			u32ReturnFlag = LogicRunInfApi(LOGIC_GET_SC_QUICK_TYPE, &u32Y);
			if(u32Y != SC_ACTION_QUICK_DONE)
				InterfaceInsertNote(INTERFACE_NOTE_ACTION_JIANGYI_STOP);
			u32Y = SC_ACTION_QUICK_STOP;
			u32ReturnFlag = LogicRunInfApi(LOGIC_SET_SC_QUICK_TYPE, &u32Y);				
			for(u32Y = 0x00; u32Y < INTERFACE_KEY_TOGETHER_MAX;u32Y++)
				s_stInterfaceActionName[u32Y] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ			
			InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ	
			ShowMain(&s_u32MainUpdateMethod);			
		}
		else
			;
			
		return u32ReturnFlag;

		
	}
#endif
/*******************************************************************************************
**�������ƣ�InterfaceModeKeyProcFuncs
**�������ã���ʾ�㴦�ڱ��ܿ���ģʽ��,�����Ĵ���(���ô���zzw)
**������������
**�����������
**ע�������
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
			LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
			LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//���͵�����			
			LogicRunInfApi( LOGIC_SET_BUTTON_UP,&s_u64InterfaceRfidDataBackup); 
			for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
				s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ				
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
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
	/*������Чʱ�Ĵ���**/
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
				s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ				
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
			ShowMain(&s_u32MainUpdateMethod);	
			if(RfidGetDataLimitlessSendFlag())
			{
				RfidSetDataLimitlessSendEnd();
				stRfidType = RFID_FRAME_ACTION;;
				LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
				LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//���͵�����			
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
	#if 0 	//�Ա���	zzw
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
					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ					
				if(s_u32ZibaoActionKeyBackup == 0x00)
				{	
					s_u32ZibaoActionKeyBackup = 0x00;
					u32Y = LOGIC_STATUS_RFID_SC_ZIBAO_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);				
					InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_ZIBAO_CONTROL);	
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_ZIBAO_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ	
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
					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ					
				InterfaceInsertNote(INTERFACE_NOTE_ZIBAO_DISABLED);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//��ʾ����������Ϣ	
				ShowMain(&s_u32MainUpdateMethod);
				if(RfidGetDataLimitlessSendFlag())
				{
					RfidSetDataLimitlessSendEnd();
					stRfidType = RFID_FRAME_ACTION;;
					LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
					LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//���͵�����			
					LogicRunInfApi( LOGIC_SET_BUTTON_UP,&s_u64InterfaceRfidDataBackup); 			
				}
				ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
				return 0x00;
			}
		}
		else if(((sNewKey.u32Key&KEY_ZI_BAOCHI) == KEY_ZI_BAOCHI) && (sNewKey.u32Status == KEY_UP))
		{
			InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ	
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
				LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
				LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//���͵�����			
				LogicRunInfApi( LOGIC_SET_BUTTON_UP,&s_u64InterfaceRfidDataBackup); 	
			}
			for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
				s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ		
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ	
			ShowMain(&s_u32MainUpdateMethod);
			s_u64InterfaceRfidDataBackup = 0x00;	
			return 0x00;
		}
	#endif
	#endif	
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)	
		/**������ȷ���У�ֻʶ��ȷ�Ϻ�ֹͣ**/
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
		#if (SPECIAL_FX_ENABLE_FLAG != DISABLED)	//��Ҫȷ�ϵİ������Ѿ����¹��������ѵ�����ȷ�ϻ��棬���ǩΪKEY_DOWN����ʱ������Ч��(��ȷ�ϼ���ֹͣ��)ʱ���ڴ˴�����
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

	#if (SHIFT_MULTIPLE_CTRL_ENABLE_FLAG != DISABLED)  //�԰���shift�İ������ڴ˴�����shift
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
		if((sNewKey.u32Key == KEY_SHIFT)&& (sNewKey.u32Status == KEY_DOWN))		//����shift��
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
		else if((sNewKey.u32Key == KEY_SHIFT)&& (sNewKey.u32Status == KEY_UP))	//̧��shift��
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
	/**ʶ���������(����SHIFT���������)**/
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
		/**�ڱ�����У�ʶ��Ӽ�������***/
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
			//Ԥ��ʱ�䣬������RFIDӦ��ʱ��
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
			if(err == OS_ERR_TIMEOUT)//��Ӧ��
			{
				StateLed(ADJUST_CODE_SUCCEED_LED,ADJUST_CODE_ING_LED);
				s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
				
				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
				u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//�Ƚ������
				LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
				OSTimeDly(200/TICK_TIME);
				LogicRunInfApi(LOGIC_GET_CONLIBRATE_SUCCESS_SC_NUMB,&u16ScNumbOldBackup); 
				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);
				
				/**��ֵ��ʾ**/   //jhy����ֹͣ
				
				s_u32StartKeyFlag= KEY_UP;
				u32StopKeyFlag = KEY_UP;
				stRfidType = RFID_FRAME_STATUS;
				u64RfidData = 0x60;
				LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
				LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//���͵�����
				g_stLogicSem->OSEventCnt = 0;
				u32Y = SC_BUTTON_ACK_NONE;
				LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//��Ӧ����Ϊ��Ч	
				u64Y = RFID_ACTION_NONE;
				LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u64Y);//��Ӧ��������Ϊ��Ч						
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
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
					u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
					u32Y = 0x00;
					LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);	
					LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32Y);	
					ShowMain(&s_u32MainUpdateMethod);
					ShowStr(0, (const u8 *)"����ʧ��!", 0x04, 32);
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
			else if(err == OS_ERR_NONE)//��Ӧ��
			{
				/**�������õ�һЩ����ģʽ�л��Ĵ���*/
				if(InterfaceGetCtrlMode() == INTERFACE_MODE_COLIBRATE_CODE_FAIL)
				{	
					StateLed(ADJUST_CODE_SUCCEED_LED,ADJUST_CODE_ING_LED);
					s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
					
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//�Ƚ������
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
					OSTimeDly(200/TICK_TIME);
					LogicRunInfApi(LOGIC_GET_CONLIBRATE_SUCCESS_SC_NUMB,&u16ScNumbOldBackup); 
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);
					
					/**��ֵ��ʾ**/   //jhy����ֹͣ
					stRfidType = RFID_FRAME_STATUS;
					u64RfidData = 0x60;
					LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
					LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//���͵�����
					g_stLogicSem->OSEventCnt = 0;
					u32Y = SC_BUTTON_ACK_NONE;
					LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//��Ӧ����Ϊ��Ч	
					u64Y = RFID_ACTION_NONE;
					LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u64Y);//��Ӧ��������Ϊ��Ч						
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
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
						u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
						LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
						u32Y = 0x00;
						LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);	
						LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32Y);	
						ShowMain(&s_u32MainUpdateMethod);
						ShowStr(0, (const u8 *)"����ʧ��!", 0x04, 32);
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
					/**��ʹ�üӼ���������£�ֱ�ӽ��붯��״̬***/
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);//���ܿ���ģʽ	
					
					InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_SUCCEED);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
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
//							u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//�Ƚ������
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
//							u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//�Ƚ������
//							LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);	
//							OSTimeDly(150/TICK_TIME);
//							u16temptest--;
//						}
//					}
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);
					
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//�Ƚ������
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
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);//���ܿ���ģʽ						
					LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
					s_u32InterfaceColibrateTimer = u32Y;
//					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
//					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//�Ƚ������
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
					InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_FAIL);//����
					InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);
					u8J = NO_CTRL;
					LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);
					ShowMain(&s_u32MainUpdateMethod);	
					KeyLedOnOff(0x00, KEY_LED_ALL);
					s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
					
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//�Ƚ������
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
					OSTimeDly(200/TICK_TIME);
					
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);	
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//��������
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
#if 0 		//�Զ��Ƽ�		zzw
		else if((sNewKey.u32Status == KEY_DOWN) && (sNewKey.u32Key == KEY_MENU_START) 
			&&(stRfidType == RFID_FRAME_STATUS)&&((u32)u64RfidData == RFID_STATUS_START)) //���ܶ���ģʽ�£���������������ʾ�Զ����ܣ��ڴ˴�����ȷ�ϲ˵�����Ϊ��ǩ��ֵ
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
		/**ʶ��������������**/
		else if((sNewKey.u32Key == KEY_MENU_START)&& ((sNewKey.u32Status == KEY_DOWN)))
		{
			/**��ֵ��ʾ**/
			LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
			LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u32RfidData);//���͵�����
			g_stLogicSem->OSEventCnt = 0;
			u32Y = SC_BUTTON_ACK_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//��Ӧ����Ϊ��Ч	
			u32Y = RFID_ACTION_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u32Y);//��Ӧ��������Ϊ��Ч			
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
			if(err == OS_ERR_TIMEOUT)//��Ӧ��
			{
				InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);		
				ShowMain(&s_u32MainUpdateMethod);		
			}
			else if(err == OS_ERR_NONE)//��Ӧ��
			{
				/**�������õ�һЩ����ģʽ�л��Ĵ���*/
				LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
				LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u32ButtonValue);
				LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&u32ButtonAckValue);
				if((status == SC_BUTTON_ACK_ACCEPT) && (u32ButtonAckValue == u32ButtonValue))
				{
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_SET);
					u32Y = LOGIC_STATUS_RFID_SC_SET;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
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
				sNewKey.u32Key = KEY_QUICK_CTRL_1;//�����¼�����Ҫ���͵ļ�ֵ
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_QUICK);	

				s_stInterfaceActionName[0x00] = SC_ACTION_QUICK_1;
				s_stInterfaceActionName[0x01] = INTERFACE_ACTION_NONE;				
				u32ReturnFlag = LogicRunInfApi(LOGIC_SET_SC_QUICK_TYPE, &s_stInterfaceActionName);
				if(s_stInterfaceActionName[0x00])	
				{
					u32Y = LOGIC_STATUS_RFID_SC_QUICK_CTRL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_QUICK);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL);//��ʾ����������Ϣ
					ShowMain(&s_u32MainUpdateMethod);
					if((s_stInterfaceActionName[0x00] != 0x00) && (u32ReturnFlag != 0x00))//����Ҫ����ִ�еĶ���
					{
						if((u32ReturnFlag == INTERFACE_NOTE_ACTION_ALARAM)||(u32ReturnFlag == INTERFACE_NOTE_ACTION_ING))
						{
							InterfaceInsertNote((INTERFACE_NOTE_TYPE)(u32ReturnFlag));
							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
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
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
					ShowMain(&s_u32MainUpdateMethod);	
					return 0x00;
				}
				
			}
		#endif
	#endif	
	#if (LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG != DISABLED)
		else if(((sNewKey.u32Key == KEY_LEFT_NEIGHBOR)|| (sNewKey.u32Key == KEY_RIGHT_NEIGHBOR))&& ((sNewKey.u32Status == KEY_DOWN)))
		{
			/**��ֵ��ʾ**/
			LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
			LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//���͵�����
			
			g_stLogicSem->OSEventCnt = 0;
			u32Y = LOGIC_STATUS_RFID_SC_NEIGHBORL;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);			
			u32Y = SC_BUTTON_ACK_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//��Ӧ����Ϊ��Ч	
			u32Y = RFID_ACTION_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u32Y);//��Ӧ��������Ϊ��Ч			
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
			if(err == OS_ERR_TIMEOUT)//��Ӧ��
			{
			#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
				BuzzerOn(BEEP_UNCOLIBRAT);
			#endif
				//jhy���������
				s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
				StateLed(0x00,KEY_SHIFT_LED);
				StateLed(0x00,ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED);			
				u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//2015.12.29,��Ϊ�ڼܼ���Ӧ�𣬽������
				LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
				u16DelayTime = RFID_SIGNAL_TOO_SMALL;
				LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DelayTime);			
				InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
				for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ

				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ	
				ShowMain(&s_u32MainUpdateMethod);	
				InterfaceInsertNote(INTERFACE_NOTE_NOACK_UNCOLIBRATE);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ��ʾ��Ϣ
				ShowMain(&s_u32MainUpdateMethod);	
//				KeyLedOnOff(0x00, KEY_LED_ALL);
			}
			else if(err == OS_ERR_NONE)//��Ӧ��
			{
				/**�������õ�һЩ����ģʽ�л��Ĵ���*/
				LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
				LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u64ButtonValue);
				LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&u64ButtonAckValue);
				if((status == SC_BUTTON_ACK_ACCEPT) && (u64ButtonAckValue == u64ButtonValue))
				{
					LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32Y);		
					for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
						s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ

					InterfaceSetCtrlMode(INTERFACE_MODE_SC_NEIGHBOR_CONTROL);
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
					ShowMain(&s_u32MainUpdateMethod);		
				}
				else if(status == SC_BUTTON_ACK_REJECT)
				{
					u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);		
					for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
						s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ
				
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
					ShowMain(&s_u32MainUpdateMethod);	
				}
				else
				{
				//jhy���������
				#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
					BuzzerOn(BEEP_UNCOLIBRAT);
				#endif
					s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
					for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
						s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ
				
					StateLed(0x00,KEY_SHIFT_LED);
					StateLed(0x00,ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED);			
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//2015.12.29,��Ϊ�ڼܼ���Ӧ�𣬽������
					LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
					u16DelayTime = RFID_SIGNAL_TOO_SMALL;
					LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DelayTime);				
					InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
					KeyLedOnOff(0x00, KEY_LED_ALL);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ	
					ShowMain(&s_u32MainUpdateMethod);	
					InterfaceInsertNote(INTERFACE_NOTE_NOACK_UNCOLIBRATE);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ��ʾ��Ϣ		
					ShowMain(&s_u32MainUpdateMethod);	
				}
			}
			else
				;
			
			u32ReturnFlag = 0x01;	
			
		}	
	#endif
	#if (GRP_CTRL_ENABLE_FLAG != DISABLED)
		else if(((sNewKey.u32Key == KEY_LEFT_GRP)|| (sNewKey.u32Key == KEY_RIGHT_GRP))&& ((sNewKey.u32Status == KEY_DOWN)))  //�������ҳ����
		{
			/**��ֵ��ʾ**/
			u32Y  = (RFID_STATUS_GRP_LEFT|RFID_STATUS_GRP_RIGHT);
			u32Y |= ((u32)(RFID_FRAME_STATUS))<<30;
			if((s_u64InterfaceRfidDataBackup^u32Y) != 0x00)			
			{
				s_u64InterfaceRfidDataBackup &= ~u32Y;
				if(s_u64InterfaceRfidDataBackup != 0x00)
				{
					g_stLogicSem->OSEventCnt = 0;
					u32Y = (s_u64InterfaceRfidDataBackup>>30);
					LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &u32Y);//���͵�����
					LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//���͵�����		
					LogicRunInfApi(LOGIC_SET_BUTTON_UP,&s_u64InterfaceRfidDataBackup); 
					LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
					LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
					u32Y = (u32)(u16DelayTime*u16SendTimes);	
					u32Y += 50;
					OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);					
				}
			}
			/**��ֵ��ʾ**/
			LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
			LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//���͵�����		
			g_stLogicSem->OSEventCnt = 0;
			u32Y = SC_GRP_ACTION_DIRCTION_MAX;//�����鷽������Ϊ��Ч
			LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION,&u32Y);//��������Ϊ��Ч
			u32Y = SC_BUTTON_ACK_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//��Ӧ����Ϊ��Ч	
			u64Y = RFID_ACTION_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u64Y);//��Ӧ��������Ϊ��Ч			
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
			if(err == OS_ERR_TIMEOUT)//��Ӧ��
			{
				InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);			
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);;
				ShowMain(&s_u32MainUpdateMethod);	
			}
			else if(err == OS_ERR_NONE)//��Ӧ��
			{
				/**�������õ�һЩ����ģʽ�л��Ĵ���*/
				LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
				LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u64ButtonValue);
				LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&u64ButtonAckValue);
				if((status == SC_BUTTON_ACK_ACCEPT) && (u64ButtonAckValue == u64ButtonValue))
				{
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_GRP_CONTROL);
					u32Y = LOGIC_STATUS_RFID_SC_GRP;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);			
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
//					s_stInterfaceActionName[0x00] = INTERFACE_GRP_ACTION_DEFAULT;
					for(err = 0x01; err < INTERFACE_KEY_TOGETHER_MAX;err++)
						s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ
					ShowMain(&s_u32MainUpdateMethod);
				}
				else if(status == SC_BUTTON_ACK_REJECT)
				{
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);	
					u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
					ShowMain(&s_u32MainUpdateMethod);		
				}
				else
				{
					InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
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
	else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_LONGDOWN))		//��סֹͣ��
	{
		u32StopKeyFlag = KEY_LONGDOWN;
		#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
			BuzzerOn(BEEP_SHUTDOWN_NOTE);
		#endif
		OSTaskSuspend(SCAN_KEY_TASK_PRIO);//���𰴼�ɨ������
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
	else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_DOWN))		//����ֹͣ��
	{
		u32StopKeyFlag = KEY_DOWN;
	}
	//ȷ�ϼ�����ģʽ�л�zzw
	else if((sNewKey.u32Key == KEY_CERTIFY) && (sNewKey.u32Status == KEY_DOWN))		//����ȷ�ϼ�
	{
		//zzw
		InterfaceKeyCertifyPro(sNewKey,LOGIC_STATUS_RFID_SC_SELF_CONTROL,INTERFACE_MODE_SC_UNCERTIFIED,NULL);
		ShowScUncertifiedKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);	
	}
	else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_UP))//��ֹͣ�������⴦��
	{
		if(u32StopKeyFlag == KEY_LONGDOWN)/**���κ�ģʽ�£���ʶ��ػ���*/
		{
			return u32ReturnFlag;			
	
		}
		else if(u32StopKeyFlag == KEY_DOWN)//ֹͣ��
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
							s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ
			
						InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//��ʾ����������Ϣ
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
			/**��ֵ��ʾ**/
					
			LogicRunInfApi(LOGIC_GET_CONLIBRATE_SUCCESS_SC_NUMB,&u16ScNumbOldBackup); 
			LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);
			StateLed(ADJUST_CODE_SUCCEED_LED,ADJUST_CODE_ING_LED);
					
			LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
			LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//���͵�����
			g_stLogicSem->OSEventCnt = 0;
			u32Y = SC_BUTTON_ACK_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//��Ӧ����Ϊ��Ч	
			u64Y = RFID_ACTION_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u64Y);//��Ӧ��������Ϊ��Ч						
			u64Y = (u64)(u64RfidData|(u64stRfidType<<62));
			LogicRunInfApi(LOGIC_SET_BUTTON_VALUE,&u64Y);
			LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
			LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
			u32Y = (u32)(u16DelayTime*u16SendTimes);	
			u32Y += 30;			
			
			OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);			
			if(err == OS_ERR_TIMEOUT)//��Ӧ��
			{
				InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ	
				ShowMain(&s_u32MainUpdateMethod);						
			}
			else if(err == OS_ERR_NONE)//��Ӧ��
			{
				/**�������õ�һЩ����ģʽ�л��Ĵ���*/
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
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
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
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
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
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ					
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
			/**��ֵ��ʾ**/
			g_stLogicSem->OSEventCnt = 0;
			LogicRunInfApi(LOGIC_SET_AUTOPRESS_ENABLE_FLAG,&u64RfidData);//��Ӧ����Ϊ��Ч	
			u64Y = RFID_ACTION_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u64Y);//��Ӧ��������Ϊ��Ч			
	
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);
			ShowMain(&s_u32MainUpdateMethod);
			LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
			LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
			u32Y = (u32)(u16DelayTime*u16SendTimes);	
			u32Y += 30;		
			
			OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
			if(err == OS_ERR_TIMEOUT)//��Ӧ��
			{
				InterfaceInsertNote(INTERFACE_NOTE_AUTO_PRESS_FAIL);
			}
			else if(err == OS_ERR_NONE)//��Ӧ��
			{
				/**�������õ�һЩ����ģʽ�л��Ĵ���*/
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
				s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//��ʾ����������Ϣ
			ShowMain(&s_u32MainUpdateMethod);	
			s_u64InterfaceRfidDataBackup = 0x00;
			s_u32ZibaoActionKeyBackup= 0x00;
			ShowScSelfKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);
			u32ReturnFlag = 0x01;					
			
			s_u64InterfaceRfidDataBackup = u32RfidData;			
		}
	#endif
//	
//	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)	//����Ҫȷ�ϵİ������Ѿ�����ȷ�ϻ��棬��ʱ����ȷ�ϼ�ʱ���ڴ˴�����
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
//				if((s_u32StartKeyFlag ==  KEY_DOWN) && (sNewKey.u32Key == KEY_CERTIFY) && (sNewKey.u32Status == KEY_DOWN))	//�Զ��Ƽܶ���������ȷ�Ϻ�Ĵ���
//				{
//					s_u32StartKeyFlag = KEY_UP;//������������֮������ȷ��
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
//					sNewKey.u32Key = KEY_MENU_START;//�����¼�����Ҫ���͵ļ�ֵ
//					u64RfidData = InterfaceGetKeyRfidDataShowName(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag,sNewKey.u32Key,&(s_stInterfaceActionName[0x00]),&stRfidType);
//					u64stRfidType = (u64)stRfidType;
//				}
//				#if (SPECIAL_FX_ENABLE_FLAG != DISABLED)		//4��������ѳ���ȷ�ϻ��棬������ȷ�ϼ���Ĵ���
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
//						if(s_u32SpecialFxKeyFlag1 ==  KEY_DOWN)	//֮ǰ���µ��������F1
//						{
//							s_u32SpecialFxKeyFlag1 = KEY_UP;							
//							s_u32StartKeyFlag = KEY_UP;
//							sNewKey.u32Key = KEY_SHORTCUT_F1;//�����¼�����Ҫ���͵ļ�ֵ
//							u64RfidData = InterfaceGetKeyRfidDataShowName(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag,sNewKey.u32Key,&(s_stInterfaceActionName[0x00]),&stRfidType);
//							u64stRfidType = (u64)stRfidType;
//						}
//						#if (SPECIAL_F2_ENABLE_FLAG != DISABLED)
//							else if(s_u32SpecialFxKeyFlag2 ==  KEY_DOWN)	//֮ǰ���µ��������F2
//							{
//								s_u32SpecialFxKeyFlag2 = KEY_UP;
//								s_u32SpecialFxKeyFlag1 =  KEY_UP;
//								s_u32StartKeyFlag = KEY_UP;
//								sNewKey.u32Key = KEY_SHORTCUT_F2;//�����¼�����Ҫ���͵ļ�ֵ
//								u64RfidData = InterfaceGetKeyRfidDataShowName(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag,sNewKey.u32Key,&(s_stInterfaceActionName[0x00]),&stRfidType);
//								u64stRfidType = (u64)stRfidType;
//							}
//						#endif
//							
//						#if (SPECIAL_F3_ENABLE_FLAG != DISABLED)	
//							else if(s_u32SpecialFxKeyFlag3 ==  KEY_DOWN)	//֮ǰ���µ��������F3
//							{
//								s_u32SpecialFxKeyFlag3 = KEY_UP;
//								s_u32SpecialFxKeyFlag2 = KEY_UP;
//								s_u32SpecialFxKeyFlag1 =  KEY_UP;
//								s_u32StartKeyFlag = KEY_UP;
//								sNewKey.u32Key = KEY_SHORTCUT_F3;//�����¼�����Ҫ���͵ļ�ֵ
//								u64RfidData = InterfaceGetKeyRfidDataShowName(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag,sNewKey.u32Key,&(s_stInterfaceActionName[0x00]),&stRfidType);
//								u64stRfidType = (u64)stRfidType;
//							}	
//						#endif	
//						#if (SPECIAL_F4_ENABLE_FLAG != DISABLED)	
//							else if(s_u32SpecialFxKeyFlag4 ==  KEY_DOWN)	//֮ǰ���µ��������F4
//							{
//								s_u32SpecialFxKeyFlag4 = KEY_UP;
//								s_u32SpecialFxKeyFlag3 = KEY_UP;
//								s_u32SpecialFxKeyFlag2 = KEY_UP;
//								s_u32SpecialFxKeyFlag1 =  KEY_UP;
//								s_u32StartKeyFlag = KEY_UP;
//								sNewKey.u32Key = KEY_SHORTCUT_F4;//�����¼�����Ҫ���͵ļ�ֵ
//								u64RfidData = InterfaceGetKeyRfidDataShowName(InterfaceGetCtrlMode(),s_u32ShiftKeyEnableFlag,sNewKey.u32Key,&(s_stInterfaceActionName[0x00]),&stRfidType);
//								u64stRfidType = (u64)stRfidType;
//							}	
//						#endif	
//						else
//							;
//					}
//				#endif				
//				else if (sNewKey.u32Key == KEY_CERTIFY)//�����κβ���������£�ֱ�ӵ��ȷ�ϼ���Ч
//				{
//					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_CTRL_MODE);//��ʾ����������Ϣ
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
//			/**��ֵ��ʾ**/
//			LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
//			LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//���͵�����
//			g_stLogicSem->OSEventCnt = 0;
//			u32Y = SC_BUTTON_ACK_NONE;
//			LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//��Ӧ����Ϊ��Ч	
//			u64Y = RFID_ACTION_NONE;
//			LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u64Y);//��Ӧ��������Ϊ��Ч			
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
//			if(err == OS_ERR_TIMEOUT)//��Ӧ��
//			{
//				for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
//					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ				
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
//			else if(err == OS_ERR_NONE)//��Ӧ��
//			{
//				/**�������õ�һЩ����ģʽ�л��Ĵ���*/
//				LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
//				LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u64ButtonValue);
//				LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&u64ButtonAckValue);
//				if((status == SC_BUTTON_ACK_ACCEPT) && (u64ButtonAckValue == u64ButtonValue))
//				{
//					#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
//						/*ĳЩ�ض���������	�����������������**/
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
//							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//��ʾ����������Ϣ				
//							InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
//							u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;							
//						}
//						else
//						{
//							for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
//								s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ
//							#if (FX_START_RETURN_SELF_FLAG != DISABLED)
//								InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//��ʾ����������Ϣ				
//								InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
//								u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;								
//							#else
//								InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_UNCERTIFIED_CONTROL);//��ʾ����������Ϣ				
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
//						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
//						InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
//						u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
//						LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
//						ShowMain(&s_u32MainUpdateMethod);
//					#endif
//				}
//				else if(status == SC_BUTTON_ACK_REJECT)
//				{
//					for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
//						s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ						
//					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
//					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//��ʾ����������Ϣ
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
//						s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//??��??��????��?					
//					InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
//					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//??��??��?��?����DD??��
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
//					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ
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
//							if(status != SC_BUTTON_ACK_ACCEPT)//��ֹ�ڰ������¾ܾ�����Ӧ��ʱ���Է��Ͱ���̧��
//							{
//								ShowScSelfKeyLedProc(KEY_INVALID,KEY_UP,s_u32ShiftKeyEnableFlag);
//								return 0x00;
//							}
//							LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
//							LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u32RfidData);//���͵�����
//							
//							LogicRunInfApi(LOGIC_SET_BUTTON_UP, &u32Y);	
//						}							
//						InterfaceInsertNote(INTERFACE_NOTE_BUTTON_UP);
//						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
//						ShowMain(&s_u32MainUpdateMethod);	
//					#else
//						LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
//						if(status != SC_BUTTON_ACK_ACCEPT)//��ֹ�ڰ������¾ܾ�����Ӧ��ʱ���Է��Ͱ���̧��
//						{	
//							ShowScSelfKeyLedProc(KEY_INVALID,KEY_UP,s_u32ShiftKeyEnableFlag);
//							return 0x00;						
//						}
////						if(g_u32ID != B_type)//jhyjhy��ֹ�հ���ֱ���ܵ�Ӧ�����Ϸ��ͳ�������
////							OSTimeDly(10/TICK_TIME);
//						LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
//						LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//���͵�����
//						u64Y = (u64)(u64RfidData|(u64stRfidType<<62));
//						LogicRunInfApi(LOGIC_SET_BUTTON_UP, &u64Y);	
//						InterfaceInsertNote(INTERFACE_NOTE_BUTTON_UP);
//						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
//						ShowMain(&s_u32MainUpdateMethod);						
//					#endif		
//				}			
//				/**��ֵ��ʾ**/
//				#if (QUICK_KEY_CTRL_ENABLE_FLAG	!= DISABLED)
//					if((sNewKey.u32Key == KEY_MENU_START)  ||  (sNewKey.u32Key == KEY_CERTIFY) \
//						||(sNewKey.u32Key == KEY_QUICK_CTRL_1) )
//				#else
//					if((sNewKey.u32Key == KEY_MENU_START)  ||  (sNewKey.u32Key == KEY_CERTIFY))				
//				#endif
//				{
//					ClearUpdateMainMethod();	
//					InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);
//					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//��ʾ����������Ϣ
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
//						u32Y = 560+30;//��ʱ���뷢�͵�ʱ���й�jhyjhy
//						OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);		
//						if(err == OS_ERR_TIMEOUT)//��Ӧ��
//						{
//							InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
//							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);			
//							InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);;
//							for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
//								s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ					
//							ShowMain(&s_u32MainUpdateMethod);	
//						}
//						else if(err == OS_ERR_NONE)//��Ӧ��
//						{
//							/**�������õ�һЩ����ģʽ�л��Ĵ���*/
//							LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
//							LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u64ButtonValue);
//							LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&u64ButtonAckValue);
//							if((status == SC_BUTTON_ACK_ACCEPT) && (u64ButtonAckValue == u64ButtonValue))
//							{
//								InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
//								InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//��ʾ����������Ϣ				
//								InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
//								u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
//								LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
//								for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
//									s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ						
//								ShowMain(&s_u32MainUpdateMethod);	
//							}
//							else if(status == SC_BUTTON_ACK_REJECT)
//							{
//								InterfaceInsertNote(INTERFACE_NOTE_BUTTON_REJECT);
//								InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//��ʾ����������Ϣ
//								InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
//								u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
//								LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
//								for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
//									s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ						
//								ShowMain(&s_u32MainUpdateMethod);	
//							}
//							else
//							{
//								InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
//								InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//��ʾ����������Ϣ
//								InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);
//								u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
//								for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
//									s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ						LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);
//								ShowMain(&s_u32MainUpdateMethod);	
//							}						
//						}
//						else
//							;	
//						
//						u32ReturnFlag = 0x01;									
//						for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
//							s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ
//						ShowScSelfKeyLedProc(KEY_INVALID,KEY_UP,s_u32ShiftKeyEnableFlag);	
// 	
//					}
//				}						
//			#else	
//				{		
//					/**��ֵ��ʾ**/
//					if(RfidGetDataLimitlessSendFlag())
//					{
//						RfidSetDataLimitlessSendEnd();
//					}					
//					LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
//					LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u32RfidData);//���͵�����		
//					u32Y = (u32)(u32RfidData|(stRfidType<<30));
//					LogicRunInfApi( LOGIC_SET_BUTTON_UP,&u32Y); 
//					LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
//					LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u32RfidData);//���͵�����
//					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL);
//					InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);
//					for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
//						s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ
//					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_SELF_CONTROL);//��ʾ����������Ϣ
//					ShowMain(&s_u32MainUpdateMethod);
//				}	
//			#endif
//		}
//#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG	!= DISABLED)		//��Ҫ��ʾȷ�ϵİ��������º��������뷵����ȷʱ���ڴ˴���ʾȷ�ϻ��棬��Ϊ�ð�����ǩ��ֵ
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
//							if(u16KeyFilmNameRam != 0x01)		//һ����ú�����۲ɼ���Ĥ(0x01)ʱ����ʹ?2020.01.15 parry?
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
**�������ƣ�InterfaceModeScNeighborControlProc
**�������ã���ʾ�㴦���ڸ��ܿ���ģʽ��,�����Ĵ���
**������������
**�����������
**ע�������
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
				LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
				LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//���͵�����			
				LogicRunInfApi( LOGIC_SET_BUTTON_UP,&s_u64InterfaceRfidDataBackup); 
				for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ				
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
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
		//shift���̰�
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
		/**ʶ���������(����SHIFT���������)**/
		else if((sNewKey.u32Key == KEY_SHIFT)&&(sNewKey.u32Status == KEY_LONGDOWN))			
		{
			InterfaceShiftKeyLongDownPro();
			u32ReturnFlag = 0x01;		
		}	
		//ȷ�ϼ�
		else if((sNewKey.u32Key == KEY_CERTIFY)&& (sNewKey.u32Status == KEY_DOWN || sNewKey.u32Status == KEY_UP))		//��ȷ�ϼ������⴦��
		{
			u32ReturnFlag = InterfaceKeyCertifyPro(sNewKey,LOGIC_STATUS_RFID_SC_NEIGHBORL,INTERFACE_MODE_SC_NEIGHBOR_CONTROL,NULL);
		}			
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG	!= DISABLED)
		/**�Ӽ�������***/
		else if(((sNewKey.u32Key == KEY_INCREASE)|| (sNewKey.u32Key == KEY_DECREASE))&& ((sNewKey.u32Status == KEY_DOWN)))
		{
			//zzw	�������
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
			//Ԥ��ʱ�䣬������RFIDӦ��ʱ��
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
			if(err == OS_ERR_TIMEOUT)//��Ӧ��
			{
//				u32Y = LOGIC_STATUS_IDLE;
//				LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);	
//				#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)				
//					BuzzerOn(BEEP_UNCOLIBRAT);
//				#endif
//				u16DelayTime = RFID_SIGNAL_TOO_SMALL;
//				LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DelayTime);	
//				StateLed(0x00,ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED|KEY_SHIFT_LED);
//				InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_FAIL);//����
//				InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
//				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);
//				ShowMain(&s_u32MainUpdateMethod);	
//				KeyLedOnOff(0x00, KEY_LED_ALL);
				s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
				
				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
				u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//�Ƚ������
				LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
				OSTimeDly(200/TICK_TIME);
				
				//jhyjhy
				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);
				
				/**��ֵ��ʾ**/   //jhy����ֹͣ
				stRfidType = RFID_FRAME_STATUS;
				u64RfidData = 0x60;
				LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
				LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//���͵�����
				g_stLogicSem->OSEventCnt = 0;
				u32Y = SC_BUTTON_ACK_NONE;
				LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//��Ӧ����Ϊ��Ч	
				u64Y = RFID_ACTION_NONE;
				LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u64Y);//��Ӧ��������Ϊ��Ч						
				u64Y = (u64)(u64RfidData|(u64stRfidType<<62));
				LogicRunInfApi(LOGIC_SET_BUTTON_VALUE,&u64Y);
				LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
				LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
				u32Y = (u32)(u16DelayTime*u16SendTimes);
				u32Y += 230;			
				
				OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);			
				if(err == OS_ERR_TIMEOUT)//��Ӧ��
				{
					InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ	
					ShowMain(&s_u32MainUpdateMethod);						
				}
				else if(err == OS_ERR_NONE)//��Ӧ��
				{
					/**�������õ�һЩ����ģʽ�л��Ĵ���*/
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
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
						u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
						LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
						u32Y = 0x00;
						LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);	
						LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32Y);	
						ShowMain(&s_u32MainUpdateMethod);
						ShowStr(0, (const u8 *)"����ʧ��!", 0x04, 32);
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
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
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
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ					
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
//				u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//��������
//				LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
								
//				u32Y = LOGIC_COLIBRATE_SC_NONE;
//				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_TYPE,&u32Y);					
			}
			else if(err == OS_ERR_NONE)//��Ӧ��
			{
				/**�������õ�һЩ����ģʽ�л��Ĵ���*/
				if(InterfaceGetCtrlMode() == INTERFACE_MODE_COLIBRATE_CODE_FAIL)
				{
					s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
					
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//�Ƚ������
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
					OSTimeDly(200/TICK_TIME);
					
					//jhyjhy
					LogicRunInfApi(LOGIC_GET_CONLIBRATE_SUCCESS_SC_NUMB,&u16ScNumbOldBackup); 
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);
					
					/**��ֵ��ʾ**/   //jhy����ֹͣ
					stRfidType = RFID_FRAME_STATUS;
					u64RfidData = 0x60;
					LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
					LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//���͵�����
					g_stLogicSem->OSEventCnt = 0;
					u32Y = SC_BUTTON_ACK_NONE;
					LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//��Ӧ����Ϊ��Ч	
					u64Y = RFID_ACTION_NONE;
					LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u64Y);//��Ӧ��������Ϊ��Ч						
					u64Y = (u64)(u64RfidData|(u64stRfidType<<62));
					LogicRunInfApi(LOGIC_SET_BUTTON_VALUE,&u64Y);
					LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
					LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);	
					u32Y = (u32)(u16DelayTime*u16SendTimes);	
					u32Y += 230;			
					
					OSSemPend(g_stLogicSem,u32Y/TICK_TIME,&err);			
					if(err == OS_ERR_TIMEOUT)//��Ӧ��
					{
						InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ	
						ShowMain(&s_u32MainUpdateMethod);						
					}
					else if(err == OS_ERR_NONE)//��Ӧ��
					{
						/**�������õ�һЩ����ģʽ�л��Ĵ���*/
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
							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
							u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
							LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u32Y);
							u32Y = 0x00;
							LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB,&u32Y);	
							LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u32Y);	
							ShowMain(&s_u32MainUpdateMethod);
							ShowStr(0, (const u8 *)"����ʧ��!", 0x04, 32);
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
							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
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
							InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ					
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
					/**��ʹ�üӼ���������£�ֱ�ӽ��붯��״̬***/
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);//���ܿ���ģʽ	
					
					InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_SUCCEED);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
					ShowMain(&s_u32MainUpdateMethod);	
					LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
					s_u32InterfaceColibrateTimer = u32Y;	
					
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);						
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//�Ƚ������
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
					InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);//���ܿ���ģʽ						
					LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
					s_u32InterfaceColibrateTimer = u32Y;
//					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
//					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//�Ƚ������
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
					InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_FAIL);//����
					InterfaceSetCtrlMode(INTERFACE_MODE_IDLE);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);	
					u8J = NO_CTRL;
					LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);
					ShowMain(&s_u32MainUpdateMethod);	
					KeyLedOnOff(0x00, KEY_LED_ALL);
					s_u32InterfaceColibrateTimer = INTERFACE_COLIBRATE_TIMER_STOP;
					
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbNewBackup);
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//�Ƚ������
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);						
					OSTimeDly(200/TICK_TIME);
					
					LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB,&u16ScNumbOldBackup);	
					u32Y = LOGIC_STATUS_RFID_UNCOLIBRATE;//��������
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
			#if (HEART == 1)		//����֡			
			{			
				Heart.ActionType = SIGNAL_ACTION;
				Heart.SendMode = 0;
			}	
			#endif	
		}		
	#endif
		//ֹͣ������
		else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_LONGDOWN))
		{
			u32StopKeyFlag = KEY_LONGDOWN;
			InterfaceStopKeyLongDownPro();
		}
		else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_DOWN))
		{
			u32StopKeyFlag = KEY_DOWN;
		}
		//��ֹͣ���̰�
		else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_UP))
		{
			if(u32StopKeyFlag == KEY_LONGDOWN)/**���κ�ģʽ�£���ʶ��ػ���*/
			{
				return u32ReturnFlag;			
		
			}
			else if(u32StopKeyFlag == KEY_DOWN)//ֹͣ��
			{
				InterfaceKeyShuntDownPro(LOGIC_STATUS_RFID_SC_NEIGHBORL,NULL);		//zzw
			}
			u32StopKeyFlag = KEY_UP;		
		}
		//�ڼܼ�
//		#if (LEFTRIGHT_NEIGHBOR_CTRL_ENABLE_FLAG != DISABLED)
//			else if(((sNewKey.u32Key == KEY_LEFT_NEIGHBOR) || (sNewKey.u32Key == KEY_RIGHT_NEIGHBOR)) 
//				&& (sNewKey.u32Status == KEY_DOWN || sNewKey.u32Status == KEY_UP))		
//			{
//				if(!InterfaceKeyNeighborPro(sNewKey,LOGIC_STATUS_RFID_SC_NEIGHBORL,INTERFACE_MODE_SC_NEIGHBOR_CONTROL,NULL))
//					return 0;
//			}
//		#endif	
		//�������ҳ����	//��������ڼܼ�	zzw2023.12.25
		else if(((sNewKey.u32Key == KEY_LEFT_GRP)|| (sNewKey.u32Key == KEY_RIGHT_GRP))&& ((sNewKey.u32Status == KEY_DOWN)))  
		{
//			InterfaceKeyGrpCtrlPro(sNewKey,LOGIC_STATUS_RFID_SC_NEIGHBORL,INTERFACE_MODE_SC_NEIGHBOR_CONTROL,NULL);
				if(!InterfaceKeyNeighborPro(sNewKey,LOGIC_STATUS_RFID_SC_NEIGHBORL,INTERFACE_MODE_SC_NEIGHBOR_CONTROL,NULL))
					return 0;
		}	
		//����������		
		else if((sNewKey.u32Status == KEY_DOWN)&&(sNewKey.u32Key == KEY_MENU_START))
		{
			InterfaceGrpActionStartPro();	
			ShowScUncertifiedKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);			
		}
		//����������
		else if(sNewKey.u32Status == KEY_DOWN)		//��ʣ�ఴ��
		{
			u32ReturnFlag = 0x01;					
			//�������		zzw
			ClearActionInfo();				
			InterfaceActionKeyDownPro(sNewKey,LOGIC_STATUS_RFID_SC_NEIGHBORL,INTERFACE_MODE_SC_NEIGHBOR_CONTROL,NULL);
			ShowScUncertifiedKeyLedProc(sNewKey.u32Key, sNewKey.u32Status, s_u32ShiftKeyEnableFlag);
		}
		//������̧��
		else if(sNewKey.u32Status == KEY_UP&&((sNewKey.u32Key&KEY_SHIFT) != KEY_SHIFT)&&((sNewKey.u32Key&KEY_CERTIFY) != KEY_CERTIFY)\
			&&((sNewKey.u32Key&KEY_INCREASE) != KEY_INCREASE)&&((sNewKey.u32Key&KEY_DECREASE) != KEY_DECREASE)\
			/*&&(sNewKey.u32Key != KEY_LEFT_NEIGHBOR)&&(sNewKey.u32Key != KEY_RIGHT_NEIGHBOR)\*/
			&&((sNewKey.u32Key&KEY_LEFT_GRP) != KEY_LEFT_GRP)&&((sNewKey.u32Key&KEY_RIGHT_GRP) != KEY_RIGHT_GRP))
		{
			//�������		zzw
			ClearActionInfo();			
			InterfaceActionKeyUpPro(sNewKey,LOGIC_STATUS_RFID_SC_NEIGHBORL,INTERFACE_MODE_SC_NEIGHBOR_CONTROL,NULL);
		}		
		else
			;	
		return u32ReturnFlag;
		
	}
#endif
/*******************************************************************************************
**�������ƣ�InterfaceModeScGrpControlProc
**�������ã���ʾ�㴦�ڳ������ģʽ�£������Ĵ���
**������������
**�����������
**ע�������
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
		//shift���̰�
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
		//zzw		ȷ�ϼ�
		else if((sNewKey.u32Key == KEY_CERTIFY)&& (sNewKey.u32Status == KEY_DOWN || sNewKey.u32Status == KEY_UP))		//��ȷ�ϼ������⴦��
		{
			u32ReturnFlag = InterfaceKeyCertifyPro(sNewKey,LOGIC_STATUS_RFID_SC_GRP,INTERFACE_MODE_SC_GRP_CONTROL,NULL);
		}	
		/**ʶ���������(����SHIFT���������)**/
		else if((sNewKey.u32Key == KEY_SHIFT)&&(sNewKey.u32Status == KEY_LONGDOWN))		
		{
			InterfaceShiftKeyLongDownPro();
			u32ReturnFlag = 0x01;		
		}	
		//ֹͣ������
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
		else if((sNewKey.u32Key == KEY_STOP_SHUTDOWN) && (sNewKey.u32Status == KEY_UP))//��ֹͣ�������⴦��
		{
			if(u32StopKeyFlag == KEY_LONGDOWN)/**���κ�ģʽ�£���ʶ��ػ���*/
			{
				return u32ReturnFlag;				
			}
			else if(u32StopKeyFlag == KEY_DOWN)//ֹͣ��
			{
				InterfaceKeyShuntDownPro(LOGIC_STATUS_RFID_SC_GRP,NULL);		//zzw
			}
			u32StopKeyFlag = KEY_UP;			
		}	
		/**���ڻ�����**/
		else if(((sNewKey.u32Key == KEY_LEFT_NEIGHBOR) || (sNewKey.u32Key == KEY_RIGHT_NEIGHBOR)) && (sNewKey.u32Status == KEY_DOWN))		//�ڼܼ�	
		{
			if(!InterfaceKeyNeighborPro(sNewKey,LOGIC_STATUS_RFID_SC_GRP,INTERFACE_MODE_SC_GRP_CONTROL,NULL))
					return 0;
			u32ReturnFlag = 0x01;					
		}
		//���鷶Χ����
		else if(((sNewKey.u32Key == KEY_LEFT_GRP)|| (sNewKey.u32Key == KEY_RIGHT_GRP)) && ((sNewKey.u32Status == KEY_DOWN)))
		{
			InterfaceKeyGrpCtrlPro(sNewKey,LOGIC_STATUS_RFID_SC_GRP,INTERFACE_MODE_SC_GRP_CONTROL,NULL);	
			u32ReturnFlag = 0x01;			
		}	
		//����������
		else if((sNewKey.u32Status == KEY_DOWN)
				&& (((sNewKey.u32Key&KEY_LEFT_GRP) != KEY_LEFT_GRP) && ((sNewKey.u32Key&KEY_RIGHT_GRP) != KEY_RIGHT_GRP)
				&& ((sNewKey.u32Key&KEY_LEFT_NEIGHBOR) != KEY_LEFT_NEIGHBOR) && ((sNewKey.u32Key&KEY_RIGHT_NEIGHBOR) != KEY_RIGHT_NEIGHBOR)
				&& ((sNewKey.u32Key&KEY_CERTIFY) != KEY_CERTIFY) && ((sNewKey.u32Key&KEY_SHIFT) != KEY_SHIFT) 
				&& ((sNewKey.u32Key&KEY_MENU_START) != KEY_MENU_START)))//���鶯�������У���ʶ�𳤰�
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
			
			if(sNewKey.u32Key == KEY_MENU_START)		//����������
			{
				InterfaceGrpActionStartPro();			
			}
			else
			{
				if((sNewKey.u32Key != KEY_LEFT_GRP)&&(sNewKey.u32Key != KEY_RIGHT_GRP))
				{
					InterfaceInsertNote(INTERFACE_NOTE_START_ACTION);	
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
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
**�������ƣ�InterfaceModeYkqSelfControlProc
**�������ã���ʾ�㴦�ڿ�������ң����ģʽ��,�����Ĵ���
**������������
**�����������
**ע�������
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
		/*������Чʱ�Ĵ���**/
		if(u32RfidData == RFID_ACTION_NONE)
		{
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
			ShowMain(&s_u32MainUpdateMethod);
			return u32ReturnFlag;
		}
		

		/**ʶ���������(����SHIFT���������)**/
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
			OSTaskSuspend(SCAN_KEY_TASK_PRIO);//���𰴼�ɨ������
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
			/**����������*****/
			ParEditSetSearchDevFlagProc(ENABLED);
			s_u32SelfSetIntfState = INTERFACEMENU_STATE_MENU_START;
			ParEdit(sNewKey,&s_u32SelfSetIntfState);
			InterfaceSetCtrlMode(INTERFACE_MODE_YKQ_SET_CONTROL);
		}		
		else if((sNewKey.u32Status == KEY_UP)&&(sNewKey.u32Key != KEY_CERTIFY))
		{
			for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
				s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ		
			InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_YKQ_SELF_CONTROL);//��ʾ����������Ϣ
			ShowMain(&s_u32MainUpdateMethod);				
		}
		else
			;
		
		return u32ReturnFlag;

	}
#endif

/*******************************************************************************************
**�������ƣ�InterfaceModeShutdownProc
**�������ã���ʾ�㴦�ڹػ�ģʽ��,�����Ĵ���
**������������
**�����������
**ע�������
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
		/*������Чʱ�Ĵ���**/
		if(u32RfidData == RFID_ACTION_NONE)
		{
			InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
			ShowMain(&s_u32MainUpdateMethod);
			return u32ReturnFlag;
		}
		/**ʶ���������(����SHIFT���������)**/
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
			OSTaskSuspend(SCAN_KEY_TASK_PRIO);//���𰴼�ɨ������
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
**�������ƣ�InterfaceModeScZibaoControlProc
**�������ã���ʾ�㴦�ڱ����Ա�ģʽ��,�����Ĵ���
**������������
**�����������
**ע�������
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
				LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
				LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//���͵�����			
				LogicRunInfApi( LOGIC_SET_BUTTON_UP,&s_u64InterfaceRfidDataBackup); 			
			}	
			u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);				
			InterfaceInsertNote(INTERFACE_NOTE_ZIBAO_END);
			for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
				s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ	
			InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);	
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
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
			OSTaskSuspend(SCAN_KEY_TASK_PRIO);//���𰴼�ɨ������
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
					LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
					LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//���͵�����			
					LogicRunInfApi( LOGIC_SET_BUTTON_UP,&s_u64InterfaceRfidDataBackup); 			
				}	
				u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
				LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);				
				InterfaceInsertNote(INTERFACE_NOTE_ZIBAO_END);
				for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ	
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);	
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
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
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
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

		/*������Чʱ�Ĵ���**/
		if((u64RfidData == RFID_ACTION_NONE))
		{
			if(s_u64InterfaceRfidDataBackup != 0x00)
			{
				if(RfidGetDataLimitlessSendFlag())
				{
					RfidSetDataLimitlessSendEnd();	
					stRfidType = RFID_FRAME_ACTION;						
					LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
					LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//���͵�����		
					u64Y = (u64)(s_u64InterfaceRfidDataBackup|(u64stRfidType<<62));
					LogicRunInfApi( LOGIC_SET_BUTTON_UP,&u64Y); 
				}				
			}
			for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
				s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ	
			if(s_u32ZibaoActionKeyBackup != 0x00)
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_ZIBAO_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
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
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
				ShowMain(&s_u32MainUpdateMethod);
				u32ReturnFlag = 0x00;
				s_u32ZibaoActionKeyBackup &= ~(sNewKey.u32Key);									
				return u32ReturnFlag;		
			}
		}
		
		if((sNewKey.u32Status == KEY_DOWN)||(sNewKey.u32Status == KEY_LONGDOWN))
		{
			if(u64RfidData < s_u64InterfaceRfidDataBackup )//�ڶ��ΰ��£���ֹͣ
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
			/**��ֵ��ʾ**/
			LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
			LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u64RfidData);//���͵�����
			g_stLogicSem->OSEventCnt = 0;
			u32Y = SC_BUTTON_ACK_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//��Ӧ����Ϊ��Ч	
			u64Y = RFID_ACTION_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u64Y);//��Ӧ��������Ϊ��Ч			
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
			if(err == OS_ERR_TIMEOUT)//��Ӧ��
			{
				InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_ZIBAO_CONTROL);	
				for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_ZIBAO_CONTROL);//��ʾ����������Ϣ
				ShowMain(&s_u32MainUpdateMethod);	
				s_u64InterfaceRfidDataBackup = 0x00;
				s_u32ZibaoActionKeyBackup= 0x00;
				ShowScZibaoKeyLedProc(KEY_INVALID, KEY_DOWN, s_u32ShiftKeyEnableFlag);
			}
			else if(err == OS_ERR_NONE)//��Ӧ��
			{
				/**�������õ�һЩ����ģʽ�л��Ĵ���*/
				LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
				LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u64ButtonValue);
				LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&u64ButtonAckValue);
				if((status == SC_BUTTON_ACK_ACCEPT) && (u64ButtonAckValue == u64RfidData))
				{
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
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
						s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_ZIBAO_CONTROL);//��ʾ����������Ϣ
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
						s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_ZIBAO_CONTROL);//��ʾ����������Ϣ					
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
**�������ƣ�InterfaceModeScZibaoControlProc
**�������ã���ʾ�㴦�ڱ����Ա�ģʽ��,�����Ĵ���
**������������
**�����������
**ע�������
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
		/**���Ա���������ʱ����ʼ��ʱ��̧��ͳ�����Ч������������Ч***/
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
				LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
				LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//���͵�����			
				LogicRunInfApi( LOGIC_SET_BUTTON_UP,&s_u64InterfaceRfidDataBackup); 			
			}	
			u32Y = LOGIC_STATUS_RFID_SC_SELF_CONTROL;
			LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Y);					
			InterfaceInsertNote(INTERFACE_NOTE_ZIBAO_END);
			for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
				s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ	
			InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);	
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
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
				LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
				LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//���͵�����			
				LogicRunInfApi( LOGIC_SET_BUTTON_UP,&s_u64InterfaceRfidDataBackup); 			
			}	
			#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
				KeyLedOnOff(0x00,KEY_LED_ALL);
			#endif			
			InterfaceInsertNote(INTERFACE_NOTE_ZIBAO_END);
			for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
				s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ	
			InterfaceSetCtrlMode(INTERFACE_MODE_SC_SELF_CONTROL);	
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_SELF_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
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
			/**�жϴ˶����Ƿ�����**/
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
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
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
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
			ShowMain(&s_u32MainUpdateMethod);				
			return u32ReturnFlag;
		}

		/*������Чʱ�Ĵ���**/
		if((u32RfidData == RFID_ACTION_NONE))
		{
			if(s_u64InterfaceRfidDataBackup != 0x00)
			{
				if(RfidGetDataLimitlessSendFlag())
				{
					RfidSetDataLimitlessSendEnd();	
					stRfidType = RFID_FRAME_ACTION;						
					LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
					LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &s_u64InterfaceRfidDataBackup);//���͵�����		
					u32Y = (u32)(s_u64InterfaceRfidDataBackup|(stRfidType<<30));
					LogicRunInfApi( LOGIC_SET_BUTTON_UP,&u32Y); 
				}				
			}
			for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
				s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ	
			if(s_u32ZibaoActionKeyBackup != 0x00)
				InterfaceInsertNote(INTERFACE_NOTE_BUTTON_INVALID);
			else
				InterfaceInsertNote(INTERFACE_NOTE_ZIBAO_ACTION_STOP);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_SC_ZIBAO_CONTROL|INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
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
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
				ShowMain(&s_u32MainUpdateMethod);
				u32ReturnFlag = 0x00;
				s_u32ZibaoActionKeyBackup &= ~(sNewKey.u32Key);	
				ShowScZibaoKeyLedProc(s_u32ZibaoActionKeyBackup,KEY_DOWN,s_u32ShiftKeyEnableFlag);
				return u32ReturnFlag;		
			}
		}
		
		if((sNewKey.u32Status == KEY_DOWN)||(sNewKey.u32Status == KEY_LONGDOWN))
		{
			if(u32RfidData < s_u64InterfaceRfidDataBackup )//�ڶ��ΰ��£���ֹͣ
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
			/**��ֵ��ʾ**/
			LogicRunInfApi(LOGIC_SET_RFID_FRAME_TYPE, &stRfidType);//���͵�����
			LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u32RfidData);//���͵�����
			g_stLogicSem->OSEventCnt = 0;
			u32Y = SC_BUTTON_ACK_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS,&u32Y);//��Ӧ����Ϊ��Ч	
			u32Y = RFID_ACTION_NONE;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE,&u32Y);//��Ӧ��������Ϊ��Ч			
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
			if(err == OS_ERR_TIMEOUT)//��Ӧ��
			{
				InterfaceInsertNote(INTERFACE_NOTE_NO_ACK);
				InterfaceSetCtrlMode(INTERFACE_MODE_SC_ZIBAO_CONTROL);	
				for(err = 0x00; err < INTERFACE_KEY_TOGETHER_MAX;err++)
					s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_ZIBAO_CONTROL);//��ʾ����������Ϣ
				ShowMain(&s_u32MainUpdateMethod);	
				s_u64InterfaceRfidDataBackup = 0x00;
				s_u32ZibaoActionKeyBackup= 0x00;
				ShowScZibaoKeyLedProc(KEY_INVALID,KEY_DOWN,s_u32ShiftKeyEnableFlag);				
			}
			else if(err == OS_ERR_NONE)//��Ӧ��
			{
				/**�������õ�һЩ����ģʽ�л��Ĵ���*/
				LogicRunInfApi(LOGIC_GET_BUTTON_ACCEPT_STATUS, &status);
				LogicRunInfApi(LOGIC_GET_BUTTON_VALUE,&u32ButtonValue);
				LogicRunInfApi(LOGIC_GET_BUTTON_ACK_VALUE,&u32ButtonAckValue);
				if((status == SC_BUTTON_ACK_ACCEPT) && (u32ButtonAckValue == u32RfidData))
				{
					InterfaceInsertNote(INTERFACE_NOTE_BUTTON_VALID);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
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
						s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_ZIBAO_CONTROL);//��ʾ����������Ϣ
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
						s_stInterfaceActionName[err] = INTERFACE_ACTION_NONE;//Ĭ����Ļ��ʾ
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE|INTERFACE_SHOW_SC_ZIBAO_CONTROL);//��ʾ����������Ϣ					
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
**�������ƣ�InterfaceModeShutdownProc
**�������ã���ʾ�㴦�ڹػ�ģʽ��,�����Ĵ���
**������������
**�����������
**ע�������
*******************************************************************************************/

static u32 InterfaceModeShutdownProc(stKEY	sNewKey)
{
	u32 u32ReturnFlag = 0x00;
	
	//�ڴ�ģʽ�£��԰����Ĳ����������
	return u32ReturnFlag;
}
/*******************************************************************************************
**�������ƣ�InterfaceModeShutdownProc
**�������ã���ʾ�㴦�ڹػ�ģʽ��,�����Ĵ���
**������������
**�����������
**ע�������
*******************************************************************************************/
static u32 InterfaceModeInchargeDisplayProc(stKEY	sNewKey)
{
	u32 u32ReturnFlag;
	static u16 u16StopKeyPressFlag = 0x00;
	
	if((sNewKey.u32Key != KEY_STOP_SHUTDOWN) &&((sNewKey.u32Status == KEY_DOWN) ||((sNewKey.u32Status == KEY_LONGDOWN))))//�а������£�������Ļ
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
		OSTaskSuspend(SCAN_KEY_TASK_PRIO);//���𰴼�ɨ������
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
**�������ƣ�InterfaceModeShutdownProc
**�������ã���ʾ�㴦�ڳ������,�����Ĵ���
**������������
**�����������
**ע�������
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
			IWDG_Feed();				//ι��
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
**�������ƣ�InterfaceModeShutdownProc
**�������ã���ʾ�㴦�ڹػ�ģʽ��,�����Ĵ���
**������������
**�����������
**ע�������
*******************************************************************************************/
static u32 InterfaceModeWLUnConnectProc(stKEY	sNewKey)
{
	u32 u32ReturnFlag = 0x00;
	u32 u32Y = 0x00;
	static u16 u16StopKeyPressFlag = 0x00;
	
	if((sNewKey.u32Key != KEY_STOP_SHUTDOWN) &&((sNewKey.u32Status == KEY_DOWN) ||((sNewKey.u32Status == KEY_LONGDOWN))))//�а������£�������Ļ
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
		OSTaskSuspend(SCAN_KEY_TASK_PRIO);//���𰴼�ɨ������
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
**�������ƣ�InterfaceModeActionInfoSyn
**�������ã������ļ�ͬ������
**������������
**�����������
**ע�������
*******************************************************************************************/
static u32 InterfaceModeActionInfoSyn(stKEY	sNewKey)
{
	u32 u32ReturnFlag = 0x00;
	u32 u32Y;
	u16 u16DelayTime;
	u8 u8J;
	LogicParamApi(LOGIC_GET_COLIBARETE_TIMEOUT, &u32Y);
	s_u32InterfaceColibrateTimer = u32Y;	
	InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
	InterfaceInsertNote(INTERFACE_NOTE_START_CERTIFY);
	ShowMain(&s_u32MainUpdateMethod);	
	//ȷ�ϼ���ʼͬ��	
	if(((sNewKey.u32Key&KEY_CERTIFY) == KEY_CERTIFY) && (sNewKey.u32Status == KEY_DOWN))//�а������£�������Ļ
	{
		LogicRunInfApi(LOGIC_SYNCHRO_ACTION_INFO, NULL);	//����ͬ��
		u32Y = LOGIC_STATUS_RFID_RECV_PRG;
		LogicRunInfApi(LOGIC_GET_RUN_STATUS,&u32Y);
//		if(1)
//		{
//			
//		}
//		else	//ͬ�����
//		{
//			u8J = MANUAL_CTRL;
//			LogicParamApi(LOGIC_SET_CTRL_MODE,&u8J);
//			InterfaceSetCtrlMode(INTERFACE_MODE_SC_UNCERTIFIED);//֧�ܿ�����δȷ��״̬(�Ƕ���״̬)	
//			InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_COLIBRATE_SUCCEED);
//			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
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
	//ֹͣ��������룬�ص�����
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
**�������ƣ�InterfacePeriodicProc
**�������ã���ʾ�������Դ���
**������������
**�����������
**ע�������
*******************************************************************************************/
void InterfacePeriodicProc(u32 u32DelayTimer)//֧�ܿ����������Դ�����
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

	/**������ʾ��Ϣ����ʾ����*/
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
							InterfaceInsertNote(INTERFACE_NOTE_PLEASE_NEAR);//�뿿��
						else
							InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);//����
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ��Ϣ
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
						InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_IDLE);//����
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ��Ϣ
						OSSemPost(g_stSemInterface);
						s_u32NoteShowTimer = INTERFACE_NOTE_TIMER_STOP;
					}
				}
			}			
		}
		else
			;
	/*���ڷ��ر��ܿ���ģʽ�µĴ���**/
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
						InterfaceInsertNote(INTERFACE_NOTE_PLEASE_PRESS_BUTTON);//����
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL|INTERFACE_SHOW_MAIN_AWAKE);//��ʾ����������Ϣ
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
	
	/**�����˳�����ģʽ�Ĵ���**/
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
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//��ʾ����������Ϣ
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
						InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL|INTERFACE_SHOW_MAIN_RFIDSIGNAL);//��ʾ����������Ϣ
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
					InterfaceInsertNote(INTERFACE_NOTE_BATTERY_INCHARGE);//���ڳ��
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
					OSSemPost(g_stSemInterface);
					s_u32InterfaceControlTimer  = INTERFACE_CTRL_TIMER_STOP;
				}
				else if(u8BatteryStatus == BATTERY_CHARGE_OVER)//����Ѿ����
				{
					InterfaceInsertNote(INTERFACE_NOTE_BATTERY_INCHARGE_OVER);//���ڳ��
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
					OSSemPost(g_stSemInterface);
					s_u32InterfaceControlTimer  = INTERFACE_CTRL_TIMER_STOP;							
				}
				else
				{
					InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_IDLE);//����
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ
					OSSemPost(g_stSemInterface);
					s_u32InterfaceControlTimer  = INTERFACE_CTRL_TIMER_STOP;
				}				
			}
		}

	/*���ڽ������Ĵ���**/
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
						/*�������**/
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
						/*�������**/
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

//	/**���������Ĺ���**/
	if(s_u32ScrPrtectedTimer && (s_u32ScrPrtectedTimer != INTERFACE_SCR_TIMER_STOP) && ((sMode == INTERFACE_MODE_IDLE)||(sMode == INTERFACE_MODE_INCHARGE_DISPLAY)))
	{
		s_u32ScrPrtectedTimer -= u32DelayTimer;
		if(s_u32ScrPrtectedTimer == 0x00)
		{
//			if(g_u32ID != B_type&&fastColiearateFlag == 0)
//				WirelessChipPwrOff();
			ClearUpdateMainMethod();
			KeyLedOnOff(0x00,KEY_LED_ALL);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_PROTECED);//����
			OSSemPost(g_stSemInterface);
		}
	}	
	
	/**�Զ��ػ��Ĺ���***/
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
				/*���йػ�����**/
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
	/***�������ճ���ʱ��ʱ�����ʾ**/
	#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)	
		if((sMode == INTERFACE_MODE_TRANS_PRG)||(sMode == INTERFACE_MODE_RECV_PRG_ING))
		{
			/*С��ʮ��ÿ��ˢ��һ��,����10��ÿ10��ˢ��һ��*/
			if(s_u32DownloadRemainTimer >= u32DelayTimer)
				s_u32DownloadRemainTimer -= u32DelayTimer;
			else
				s_u32DownloadRemainTimer = 0x00;
			if(((s_u32DownloadRemainTimer <= 10000) && ((s_u32DownloadRemainTimerBackup - s_u32DownloadRemainTimer) >= 1000 ))
				|| ((s_u32DownloadRemainTimer > 10000) && ((s_u32DownloadRemainTimerBackup - s_u32DownloadRemainTimer) >= 10000)))
			{
				
				s_u32DownloadRemainTimerBackup = s_u32DownloadRemainTimer;
				if(s_u32DownloadRemainTimerBackup > 10000)//Ϊ��֤��10��ı���������ʾ�����ʣ��ʱ��ı��ݽ������⴦��
				{
					s_u32DownloadRemainTimer /= 1000;//��
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
		//�����ļ�ͬ��	zzw
		if(sMode == INTERFACE_MODE_ACTIONINFI_SYN)
		{
			/*С��ʮ��ÿ��ˢ��һ��,����10��ÿ10��ˢ��һ��*/
			if(s_u32DownloadRemainTimer >= u32DelayTimer)
				s_u32DownloadRemainTimer -= u32DelayTimer;
			else
				s_u32DownloadRemainTimer = 0x00;
			if(((s_u32DownloadRemainTimer <= 10000) && ((s_u32DownloadRemainTimerBackup - s_u32DownloadRemainTimer) >= 1000 ))
				|| ((s_u32DownloadRemainTimer > 10000) && ((s_u32DownloadRemainTimerBackup - s_u32DownloadRemainTimer) >= 10000)))
			{
				
				s_u32DownloadRemainTimerBackup = s_u32DownloadRemainTimer;
				if(s_u32DownloadRemainTimerBackup > 10000)//Ϊ��֤��10��ı���������ʾ�����ʣ��ʱ��ı��ݽ������⴦��
				{
					s_u32DownloadRemainTimer /= 1000;//��
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
//	if(u16KeyFilmNameRam == 0x02)		//ѡ���Զ������Ĥʱ�����������δ���壬���¼��̵ƶ�Ϩ���ڴ˴�����	2020.04.27 parry
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
**�������ƣ�InterfaceTaskStartProc
**�������ã���ģ���ǿ���֮��,���洦���������ѭ��֮ǰ�Ĳ���
**������������
**�����������
**ע�������
***********************************************************************************************/
#ifdef	IWDG_ENABLED
	extern OS_STK  Iwdg_task_stk[TASK_STK_SIZE_64];				//���������ջ
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
			;//ֱ�ӿ���
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

		/*���Ź�����***/
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
	
	//��ʼ��
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
				&&((u32ShutDownFlag&YKQ_SHUTDOWN_INCHARGE_TURNON) == YKQ_SHUTDOWN_INCHARGE_TURNON))//��绽�ѵ������
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

			#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_ADC_MNG)//��Ӳ���У�ʹ��AD��ʽ�������
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
**�������ƣ�Interface_task
**�������ã�������ʾ������
**������������
**�����������
**ע�������
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
	ActionInfoInit();		//�����ļ���ʼ��	
	InterfaceTaskStartProc();
	//��������ѭ��
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
//				if(RfidGetDataLimitlessSendFlag())	//zzw	�������
//				{
//					RfidSetDataLimitlessSendEnd();
//				}
//			}
		}
//		else
//			;
		InterfacePeriodicProc(INTERFACE_TASK_TIME_SET+INTERFACE_TASK_TIME_SET);//֧�ܿ����������Դ�����
		
//		sprintf(s,"1:%6d 2:%3d",timestest,timesnoack);
//			ShowStr(0, (const u8 *)s, 0x00, 00);
	}
}
	
#endif
