/********************************************************************************
* 文件名称：RfidMng.h
* 作	者：马如意   
* 当前版本：V1.0
* 完成日期：2015.05.18
* 功能描述: 定义RfidMng.h头文件
* 历史信息：   
*           	版本信息     完成时间      原作者        注释
*
*       >>>>  在工程中的位置  <<<<
*             3-应用层
*          √  2-协议层
*          	  1-硬件驱动层
*********************************************************************************
* Copyright (c) 2014,天津华宁电子有限公司 All rights reserved.
*********************************************************************************/
#ifndef __RFIDMNG_H__
#define __RFIDMNG_H__

/********************************************************************************
* .h头文件
*********************************************************************************/
#include "includes.h"
#include "logic.h"
/********************************************************************************
* #define宏定义
*********************************************************************************/
/**通讯协议中所定义的设备类型****/
#define NONE_DEV_TYPE					(0x00000000)//设备类型无效
#define SC_DEV_TYPE						(0x00100A00)//支架控制器
#define EMVD_DEV_TYPE					(0x00100A01)//电磁阀
#define HUB_DEV_TYPE					(0x00100A02)//HUB
#define WL_DEV_TYPE 					(0x00100A03)//无线模块
#define CXB_DEV_TYPE					(0x00100A04)//程序棒
#define ANGLE_DEV_TYPE					(0x00100A05)//角度
#define SS_DEV_TYPE						(0x00100A06)//支架服务器
#define YKQ_DEV_TYPE 					(0x00100A08)//遥控器
#define YKQ_WL_DEV_TYPE 				(0x00100A0A)//遥控器WL
#define DYK_DEV_TYPE_MAX				(0x00100A1F)//设备类型的最大值

/**动作码定义**/	//30个动作增加至56个动作 2019.12.24 parry
#define	RFID_ACTION_NONE  (0x0000000000000000)
#define	RFID_ACTION_SHENGZHU  (0x4000000000000001)
#define	RFID_ACTION_JIANGZHU  (0x4000000000000002)
#define	RFID_ACTION_TUILIU  (0x4000000000000004)
#define	RFID_ACTION_YIJIA   (0x4000000000000008)//包含移架(拉溜)

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
#define	RFID_ACTION_DINGLIANG_PENWU  (0x4000000000400000)//在鲍店8301改造中，以此代替喷雾		
#define	RFID_ACTION_FANGMEI_PENWU  (0x4000000000800000)

#define	RFID_ACTION_SONG_HOULIU  (0x4000000001000000)//在鲍店矿6303&8301、阳煤矿中，以此代替松后溜
#define	RFID_ACTION_LA_HOULIU  (0x4000000002000000)
#define	RFID_ACTION_SHEN_DITIAO  (0x4000000004000000)//此阀动作为预留功能，在此版本遥控器中无定义。
#define	RFID_ACTION_SHOU_DITIAO  (0x4000000008000000)//此阀动作为预留功能，在此版本遥控器中无定义。

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

/**本项目中使用扩展帧格式***/
__packed typedef struct
{
	__packed union
	{
		__packed struct
		{
			unsigned int	RxID:3;				//接收方ID
			unsigned int	TxID:3;				//发送方ID
			unsigned int	FrameType:10;		//帧类别
			/**鲍店矿将流水号的位置进行了修改,SUM在前(4),LS在后(5)***/
			/**鲍店之前的项目，LS在前(4)，SUM在后(5)*/
			unsigned int	Sum:4;		//子帧序号或	
			unsigned int	LiuShuiNumb:5;		//流水序列号，0x0-0xf循环，ACK=1应答帧的流水，采用被应答帧的流水		
			
			unsigned int	Sub:1;		//组合帧/子帧，0：组合帧总数帧：1：子帧
			unsigned int	ACK:1;				//应答标志，1:应答，0:无需应答			
			unsigned int	Reservd:2;			//预留区域。用于传程序:01,其他:00	
			unsigned int	NoUsed:3;			//不存在区域
		} ID;//帧ID
		u32 u32Id;//帧ID
	} u32ID;	//帧ID
	u8	u8DT[11];			//帧数据		//长度增加到11 zzw2023.12.16
	u8	u8DLC;				//帧长度
} stFRAME;

/**帧类型定义**/
enum
{
	RFID_FT_HEART = 0,
	RFID_FT_COLIBARATE = 6,
	RFID_FT_COLIBARATE_ACK = 7,	
	RFID_FT_GRP_ACTION_INF = 8,	
	RFID_FT_GRP_YALI_INF = 9,	
	RFID_FT_KEY_DOWN  = 10,			//动作控制
	RFID_FT_KEY_DOWN_ACK  = 11,		//动作空应答
	RFID_FT_KEY_LONGDOWN  = 12,
	RFID_FT_KEY_UP  = 13,
	RFID_FT_SC_UNCOLIBATARE  = 14,
	RFID_FT_WL_UNCOLIBATARE  = 15,
	RFID_FT_RESET_PARAM  = 16,		//重新设定参数
	RFID_FT_KEY_UP_ACK  = 17,		//按键抬起应答
	RFID_FT_ACTION_INFO_REQ = 18,	//配置文件同步请求
	RFID_FT_NEAR_CTRL_REQ = 19,		//邻架控制请求
	RFID_FT_NEAR_CTRL_CONFIRM = 20,	//邻架控制应答		
	RFID_FT_WL_TEST_SEND = 21,      //无线测试帧
    RFID_FT_WL_TEST_SEND_ACK = 22,  //无线测试应答
	#if (KEY_AUTO_PRESS_PROC_FLAG != DISABLED)
		RFID_FT_AUTOPRESS_ENABLED = 18,//自动补压开启
	
	
	
		RFID_FT_AUTOPRESS_ENABLED_ACK = 19,//自动补压开启应答
		RFID_FT_AUTOPRESS_DISABLED = 20,//自动补压关闭
		RFID_FT_AUTOPRESS_DISABLED_ACK = 21,//自动补压开启应答		
	#endif
};

/**是否应答定义***/
enum
{
	NO_ACK = 0,
	ACK = 1
};

/**RFID协议中成组动作号枚举定义**/
typedef enum
{
	/**成组动作号**/
	RFID_GRP_YIJIA = 0x00000001,
	RFID_GRP_LALIU = 0x00000002,	
	RFID_GRP_TUILIU = 0x00000003,
	RFID_GRP_LA_HOULIU = 0x00000004,
	RFID_GRP_SHOU_HUBANG = 0x00000005,

	RFID_GRP_SHEN_HUBANG = 0x00000006,	
	RFID_GRP_SHOU_HUBANG2 = 0x00000007,	
	RFID_GRP_SHEN_HUBANG2 = 0x00000008,
	RFID_GRP_FANGMEI_PENWU = 0x00000009,
	RFID_GRP_SHOUHUBANG_UNION = 0x0000000A,//收互帮收伸缩梁联动
	
	RFID_GRP_SHENHUBANG_UNION = 0x0000000B,	//伸互帮伸伸缩梁联动	
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

/**RFID数据类型定义**/
typedef enum
{
	RFID_FRAME_INVAILD = 0x00,//无效数值
	RFID_FRAME_ACTION = 0x01,//动作字
	RFID_FRAME_STATUS = 0x02,//状态字
}RFID_FRAME_TYPE;

/**动作控制码及状态定义**/
enum
{
	/**状态码定义**///在数据发送时，需要或上0x80000000
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

	RFID_STATUS_GRP_SHENHUBANG_UNION = 0x00000100,	//成组收护帮收伸缩梁联动	
	RFID_STATUS_GRP_SHOUHUBANG_UNION = 0x00000200,//成组伸护帮伸伸缩梁联动

	RFID_STATUS_QUICK1_KEY = 0x00000300,//快捷键F1
	RFID_STATUS_QUICK2_KEY = 0x00000400,//快捷键F2
	RFID_STATUS_QUICK3_KEY = 0x00000500,//一键放煤
	RFID_STATUS_QUICK4_KEY = 0x00000600,//自动反冲洗
	RFID_STATUS_INVALID = 0x0FFFFFFF,//无任何意义的按键	
};

/**信号强度枚举**/
enum
{
	RFID_SIGNAL_NONE,
	RFID_SIGNAL_TOO_SMALL,//太弱的信号
	RFID_SIGNAL_NEGATIVE_80,//-80dbm以上
	RFID_SIGNAL_NEGATIVE_60,//-60dbm以上
	RFID_SIGNAL_NEGATIVE_50,//-50dbm以上
	RFID_SIGNAL_NEGATIVE_40,//-40dbm以上
	RFID_SIGNAL_NEGATIVE_30,//-30dbm以上
	RFID_SIGNAL_NO_CONNEST,//未连接
};


/********************************************************************************
* 串口定义
*********************************************************************************/




#define  SendHeartTimerMAX     10000


//关于计时器的几种状态
#define TIMER_EXPIRED	(u32)0x0				//计时结束
#define TIMER_DONE		(u32)0xfffffffe			//计时结束已处理
#define TIMER_CLOSED	(u32)0xffffffff			//计时器关闭


/**应答定义**/
enum
{
	NOACK=0,
};
/**帧类别枚举**/
enum
{
	HEARTBEAT,//心跳
	WIRELESSDATA=1,//无线数据
	STARTHINT,//
	RESERVED,
	PARAMETERSETTING,//
	UPDATAPROGRAM,//gnegxin
	RESTORATIONWL//
};




/********************************************************************************
* 常量定义
*********************************************************************************/

/********************************************************************************
* 全局变量声明
*********************************************************************************/

/********************************************************************************
* 函数声明
*********************************************************************************/

/********************************************************************************
* 函数声明
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
//心跳帧
u32 RfidMngSendHeart(u64 u64FrameData,u8 ActionType,u8 Sendmode);
//邻架控制请求
u32 RfidMngNearCtrlReq(u8 NearScNum);
u8 RfidMngWLTestSend(void);     //无线测试
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

