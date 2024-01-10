/********************************************************************************
* �ļ����ƣ�RfidMng.h
* ��	�ߣ�������   
* ��ǰ�汾��V1.0
* ������ڣ�2015.05.18
* ��������: ����RfidMng.hͷ�ļ�
* ��ʷ��Ϣ��   
*           	�汾��Ϣ     ���ʱ��      ԭ����        ע��
*
*       >>>>  �ڹ����е�λ��  <<<<
*             3-Ӧ�ò�
*          ��  2-Э���
*          	  1-Ӳ��������
*********************************************************************************
* Copyright (c) 2014,������������޹�˾ All rights reserved.
*********************************************************************************/
#ifndef __RFIDMNG_H__
#define __RFIDMNG_H__

/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/
#include "includes.h"
#include "logic.h"
/********************************************************************************
* #define�궨��
*********************************************************************************/
/**ͨѶЭ������������豸����****/
#define NONE_DEV_TYPE					(0x00000000)//�豸������Ч
#define SC_DEV_TYPE						(0x00100A00)//֧�ܿ�����
#define EMVD_DEV_TYPE					(0x00100A01)//��ŷ�
#define HUB_DEV_TYPE					(0x00100A02)//HUB
#define WL_DEV_TYPE 					(0x00100A03)//����ģ��
#define CXB_DEV_TYPE					(0x00100A04)//�����
#define ANGLE_DEV_TYPE					(0x00100A05)//�Ƕ�
#define SS_DEV_TYPE						(0x00100A06)//֧�ܷ�����
#define YKQ_DEV_TYPE 					(0x00100A08)//ң����
#define YKQ_WL_DEV_TYPE 				(0x00100A0A)//ң����WL
#define DYK_DEV_TYPE_MAX				(0x00100A1F)//�豸���͵����ֵ

/**�����붨��**/	//30������������56������ 2019.12.24 parry
#define	RFID_ACTION_NONE  (0x0000000000000000)
#define	RFID_ACTION_SHENGZHU  (0x4000000000000001)
#define	RFID_ACTION_JIANGZHU  (0x4000000000000002)
#define	RFID_ACTION_TUILIU  (0x4000000000000004)
#define	RFID_ACTION_YIJIA   (0x4000000000000008)//�����Ƽ�(����)

#define	RFID_ACTION_SHEN_SHENSUOLIANG  (0x4000000000000010)
#define	RFID_ACTION_SHOU_SHENSUOLIANG  (0x4000000000000020)
#define	RFID_ACTION_SHEN_BAILIANG  (0x4000000000000040)
#define	RFID_ACTION_SHOU_BAILIANG  (0x4000000000000080)	

#define	RFID_ACTION_SHEN_HUBANG  (0x4000000000000100)
#define	RFID_ACTION_SHOU_HUBANG  (0x4000000000000200)
#define	RFID_ACTION_SHEN_CEHU  (0x4000000000000400)
#define	RFID_ACTION_SHOU_CEHU  (0x4000000000000800)

#define	RFID_ACTION_TAI_DIZUO  (0x4000000000001000)
#define	RFID_ACTION_JIANG_DIZUO  (0x4000000000002000)
#define	RFID_ACTION_SHEN_HUBANG2  (0x4000000000004000)
#define	RFID_ACTION_SHOU_HUBANG2  (0x4000000000008000)	

#define	RFID_ACTION_WEILIANG_SHANGBAI  (0x4000000000010000)
#define	RFID_ACTION_WEILIANG_XIABAI  (0x4000000000020000)
#define	RFID_ACTION_SHEN_CHABAN  (0x4000000000040000)
#define	RFID_ACTION_SHOU_CHABAN  (0x4000000000080000)

#define	RFID_ACTION_SHENG_HOUZHU  (0x4000000000100000)
#define	RFID_ACTION_JIANG_HOUZHU  (0x4000000000200000)
#define	RFID_ACTION_DINGLIANG_PENWU  (0x4000000000400000)//�ڱ���8301�����У��Դ˴�������		
#define	RFID_ACTION_FANGMEI_PENWU  (0x4000000000800000)

#define	RFID_ACTION_SONG_HOULIU  (0x4000000001000000)//�ڱ����6303&8301����ú���У��Դ˴����ɺ���
#define	RFID_ACTION_LA_HOULIU  (0x4000000002000000)
#define	RFID_ACTION_SHEN_DITIAO  (0x4000000004000000)//�˷�����ΪԤ�����ܣ��ڴ˰汾ң�������޶��塣
#define	RFID_ACTION_SHOU_DITIAO  (0x4000000008000000)//�˷�����ΪԤ�����ܣ��ڴ˰汾ң�������޶��塣

#define	RFID_ACTION_QIANLIANG_SHANGBAI  (0x4000000010000000)
#define	RFID_ACTION_QIANLIANG_XIABAI  (0x4000000020000000)	
#define	RFID_ACTION_FANCHONG1  (0x4000000040000000)
#define	RFID_ACTION_FANCHONG2  (0x4000000080000000)	

#define	RFID_ACTION_SHEN_YANCE  (0x4000000100000000)
#define	RFID_ACTION_SHOU_YANCE  (0x4000000200000000)	
#define	RFID_ACTION_SHEN_HUBANG3  (0x4000000400000000)
#define	RFID_ACTION_SHOU_HUBANG3  (0x4000000800000000)	

#define	RFID_ACTION_SHEN_PINGHENG  (0x4000001000000000)
#define	RFID_ACTION_SHOU_PINGHENG  (0x4000002000000000)	
#define	RFID_ACTION_SHENG_TIAOLIU  (0x4000004000000000)
#define	RFID_ACTION_JIANG_TIAOLIU  (0x4000008000000000)	

/**����Ŀ��ʹ����չ֡��ʽ***/
__packed typedef struct
{
	__packed union
	{
		__packed struct
		{
			unsigned int	RxID:3;				//���շ�ID
			unsigned int	TxID:3;				//���ͷ�ID
			unsigned int	FrameType:10;		//֡���
			/**�������ˮ�ŵ�λ�ý������޸�,SUM��ǰ(4),LS�ں�(5)***/
			/**����֮ǰ����Ŀ��LS��ǰ(4)��SUM�ں�(5)*/
			unsigned int	Sum:4;		//��֡��Ż�	
			unsigned int	LiuShuiNumb:5;		//��ˮ���кţ�0x0-0xfѭ����ACK=1Ӧ��֡����ˮ�����ñ�Ӧ��֡����ˮ		
			
			unsigned int	Sub:1;		//���֡/��֡��0�����֡����֡��1����֡
			unsigned int	ACK:1;				//Ӧ���־��1:Ӧ��0:����Ӧ��			
			unsigned int	Reservd:2;			//Ԥ���������ڴ�����:01,����:00	
			unsigned int	NoUsed:3;			//����������
		} ID;//֡ID
		u32 u32Id;//֡ID
	} u32ID;	//֡ID
	u8	u8DT[11];			//֡����		//�������ӵ�11 zzw2023.12.16
	u8	u8DLC;				//֡����
} stFRAME;

/**֡���Ͷ���**/
enum
{
	RFID_FT_HEART = 0,
	RFID_FT_COLIBARATE = 6,
	RFID_FT_COLIBARATE_ACK = 7,	
	RFID_FT_GRP_ACTION_INF = 8,	
	RFID_FT_GRP_YALI_INF = 9,	
	RFID_FT_KEY_DOWN  = 10,			//��������
	RFID_FT_KEY_DOWN_ACK  = 11,		//������Ӧ��
	RFID_FT_KEY_LONGDOWN  = 12,
	RFID_FT_KEY_UP  = 13,
	RFID_FT_SC_UNCOLIBATARE  = 14,
	RFID_FT_WL_UNCOLIBATARE  = 15,
	RFID_FT_RESET_PARAM  = 16,		//�����趨����
	RFID_FT_KEY_UP_ACK  = 17,		//����̧��Ӧ��
	RFID_FT_ACTION_INFO_REQ = 18,	//�����ļ�ͬ������
	RFID_FT_NEAR_CTRL_REQ = 19,		//�ڼܿ�������
	RFID_FT_NEAR_CTRL_CONFIRM = 20,	//�ڼܿ���Ӧ��		
	RFID_FT_WL_TEST_SEND = 21,      //���߲���֡
    RFID_FT_WL_TEST_SEND_ACK = 22,  //���߲���Ӧ��
	#if (KEY_AUTO_PRESS_PROC_FLAG != DISABLED)
		RFID_FT_AUTOPRESS_ENABLED = 18,//�Զ���ѹ����
	
	
	
		RFID_FT_AUTOPRESS_ENABLED_ACK = 19,//�Զ���ѹ����Ӧ��
		RFID_FT_AUTOPRESS_DISABLED = 20,//�Զ���ѹ�ر�
		RFID_FT_AUTOPRESS_DISABLED_ACK = 21,//�Զ���ѹ����Ӧ��		
	#endif
};

/**�Ƿ�Ӧ����***/
enum
{
	NO_ACK = 0,
	ACK = 1
};

/**RFIDЭ���г��鶯����ö�ٶ���**/
typedef enum
{
	/**���鶯����**/
	RFID_GRP_YIJIA = 0x00000001,
	RFID_GRP_LALIU = 0x00000002,	
	RFID_GRP_TUILIU = 0x00000003,
	RFID_GRP_LA_HOULIU = 0x00000004,
	RFID_GRP_SHOU_HUBANG = 0x00000005,

	RFID_GRP_SHEN_HUBANG = 0x00000006,	
	RFID_GRP_SHOU_HUBANG2 = 0x00000007,	
	RFID_GRP_SHEN_HUBANG2 = 0x00000008,
	RFID_GRP_FANGMEI_PENWU = 0x00000009,
	RFID_GRP_SHOUHUBANG_UNION = 0x0000000A,//�ջ���������������
	
	RFID_GRP_SHENHUBANG_UNION = 0x0000000B,	//�컥��������������	
	RFID_GRP_SHOU_SHENSUOLIANG = 0x0000000C,
	RFID_GRP_SHEN_SHENSUOLIANG = 0x0000000D,	
	RFID_GRP_DINGLIANG_PENWU = 0x0000000E,
//	RFID_GRP_FANGDINGMEI = 0x0000000F,
	RFID_GRP_BUYA = 0x00000010,
	RFID_GRP_SHENG_TIAOLIU = 0x00000011,
	RFID_GRP_JIANG_TIAOLIU = 0x00000012,
	RFID_GRP_SHEN_HUBANG3 = 0x00000013,
	RFID_GRP_SHOU_HUBANG3 = 0x00000014,

	RFID_GRP_ACTION_MAX = 0x00000015
}RFID_GRP_ACTION_TYPE;

/**RFID�������Ͷ���**/
typedef enum
{
	RFID_FRAME_INVAILD = 0x00,//��Ч��ֵ
	RFID_FRAME_ACTION = 0x01,//������
	RFID_FRAME_STATUS = 0x02,//״̬��
}RFID_FRAME_TYPE;

/**���������뼰״̬����**/
enum
{
	/**״̬�붨��**///�����ݷ���ʱ����Ҫ����0x80000000
	RFID_STATUS_NUMBER_1 = 0x00000001,
	RFID_STATUS_NUMBER_2 = 0x00000002,
	RFID_STATUS_NUMBER_3 = 0x00000003,	
	RFID_STATUS_NUMBER_4 = 0x00000004,
	RFID_STATUS_NUMBER_5 = 0x00000005,
	RFID_STATUS_NUMBER_6 = 0x00000006,
	RFID_STATUS_NUMBER_7 = 0x00000007,	
	RFID_STATUS_NUMBER_8 = 0x00000008,	
	RFID_STATUS_NUMBER_9 = 0x00000009,
	RFID_STATUS_NUMBER_0 = 0x0000000A,

	RFID_STATUS_GRP_LEFT = 0x00000010,
	RFID_STATUS_GRP_RIGHT = 0x00000020,	
	RFID_STATUS_NEIGHBOR_LEFT = 0x00000030,
	RFID_STATUS_NEIGHBOR_RIGHT = 0x00000040,	
	RFID_STATUS_CERTIUFY = 0x00000050,	
	RFID_STATUS_STOP = 0x00000060,	
	RFID_STATUS_START = 0x00000070,
	RFID_STATUS_MENU_LEFT = 0x00000080,		
	RFID_STATUS_MENU_RIGHT = 0x00000090,	
	RFID_STATUS_MENU_UP = 0x000000A0,		
	RFID_STATUS_MENU_DOWN = 0x000000B0,		
	RFID_STATUS_SELECT_UP = 0x000000C0,		
	RFID_STATUS_SELECT_DOWN = 0x000000D0,

	RFID_STATUS_GRP_SHENHUBANG_UNION = 0x00000100,	//�����ջ���������������	
	RFID_STATUS_GRP_SHOUHUBANG_UNION = 0x00000200,//�����커��������������

	RFID_STATUS_QUICK1_KEY = 0x00000300,//��ݼ�F1
	RFID_STATUS_QUICK2_KEY = 0x00000400,//��ݼ�F2
	RFID_STATUS_QUICK3_KEY = 0x00000500,//һ����ú
	RFID_STATUS_QUICK4_KEY = 0x00000600,//�Զ�����ϴ
	RFID_STATUS_INVALID = 0x0FFFFFFF,//���κ�����İ���	
};

/**�ź�ǿ��ö��**/
enum
{
	RFID_SIGNAL_NONE,
	RFID_SIGNAL_TOO_SMALL,//̫�����ź�
	RFID_SIGNAL_NEGATIVE_80,//-80dbm����
	RFID_SIGNAL_NEGATIVE_60,//-60dbm����
	RFID_SIGNAL_NEGATIVE_50,//-50dbm����
	RFID_SIGNAL_NEGATIVE_40,//-40dbm����
	RFID_SIGNAL_NEGATIVE_30,//-30dbm����
	RFID_SIGNAL_NO_CONNEST,//δ����
};


/********************************************************************************
* ���ڶ���
*********************************************************************************/




#define  SendHeartTimerMAX     10000


//���ڼ�ʱ���ļ���״̬
#define TIMER_EXPIRED	(u32)0x0				//��ʱ����
#define TIMER_DONE		(u32)0xfffffffe			//��ʱ�����Ѵ���
#define TIMER_CLOSED	(u32)0xffffffff			//��ʱ���ر�


/**Ӧ����**/
enum
{
	NOACK=0,
};
/**֡���ö��**/
enum
{
	HEARTBEAT,//����
	WIRELESSDATA=1,//��������
	STARTHINT,//
	RESERVED,
	PARAMETERSETTING,//
	UPDATAPROGRAM,//gnegxin
	RESTORATIONWL//
};




/********************************************************************************
* ��������
*********************************************************************************/

/********************************************************************************
* ȫ�ֱ�������
*********************************************************************************/

/********************************************************************************
* ��������
*********************************************************************************/

/********************************************************************************
* ��������
*********************************************************************************/
u32 RfidRecvSemCreate(void);
u32 RfidMngSetRecvCallBack(void);
u32 RfidGetDataLimitlessSendFlag(void);
u32 RfidSetDataLimitlessSendEnd(void);
u32 RfidMngSendDataColibarate(void);
u32 RfidMngSendDataUnColibarate(void);
u32 RfidMngSendDataKeyDown(u8 ActionType,u8 ActionID,u8 ActionOrder);
u32 RfidMngSendDataKeyLongDown(RFID_FRAME_TYPE stType, u64 u64ActionType);
u32 RfidMngSendDataKeyUp(RFID_FRAME_TYPE stType, u64 u64ActionType);
void SendSetupToWl(u8 type);
void UsartSendDataProc(void);
u8 InsUsartTrsQueue(u8 *pFrame,u8 dlc,u8 FrameType , u8 ack,u8 isSend);
//����֡
u32 RfidMngSendHeart(u64 u64FrameData,u8 ActionType,u8 Sendmode);
//�ڼܿ�������
u32 RfidMngNearCtrlReq(u8 NearScNum);
u8 RfidMngWLTestSend(void);     //���߲���
u32 RfidMngSendActionInfoReq(void);
#if (KEY_AUTO_PRESS_PROC_FLAG != DISABLED)
	u32 RfidMngSendAutoPressEnabledDisabledFlag(u32 u32FrameData);
#endif
void SendResponseData(u8 Serial , u8 Data,u8 ft);
u32 UsartMngSetRecvCallBack(void);
void RfidMng_Task(void *p_arg);
void Usart_Task(void *p_arg);


extern u8 WLUnconnectedFlag;
#endif

