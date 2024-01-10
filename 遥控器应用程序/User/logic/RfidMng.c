/********************************************************************************
* 文件名称：RfidMng.c
* 作    者：马如意   
* 版    本：V1.0
* 日    期：2015.05.18
* 功能描述: 实现RFID的协议层处理。

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
#include	"includes.h"

/********************************************************************************
* #define宏定义
*********************************************************************************/
__packed typedef struct
{
	u8 u8DestDevID;
	u16 u16SendTimes;	
	u8 u8WaitAckFlag;//是否需要等待对方应答标志
	u8 u8OtherAckFlag;//对方已经应答标志
	
	stFRAME stFrame;
	u32 u32SendLength;
	u16 u16SendInterval;	
	
}RFID_MNG_DATA_TYPE;

/**建立SC应答成组命令字与界面显示的对应关系**/
typedef struct
{
	RFID_GRP_ACTION_TYPE	stGrpActionAckCode;//SC应答的成组动作字
	RFID_FRAME_TYPE			stActionCodeType;//动作码的类型
	u32			u32ActionCode;//RFID中定义的动作码(即实际被控架执行的动作)	
}RFID_GRP_ACK_INTERFACE_TYPE;

u32 RfidTestTimer = 0xffff;      //无线测试超时时间
/********************************************************************************
* 常量定义
*********************************************************************************/
#define	RFIDMNG_TASK_TIME_SET				10//任务延时周期
#define	RFID_SEND_BUF_SIZE					20
#define STFRAME_LS_MAX						255//流水号的最大值

#define	RFID_SEND_INTERVAL_CLOSED			0xFFFFFFFF//等待时间无效(关闭)
#define	RFID_SEND_INTERVAL_DEFAULT			40//默认的等待间隔

#define	RFID_SEND_TIMES_LIMITLESS			0xFFFF//无限次发送

#define	RFID_SEND_ALLDEV_ID					0xFF//群发地址

#define	RFID_TEST							0

#define	RSSI_OFFSET_VALUE					74

#define RFID_KEYUP_SEND_TIMES_MAX			3//底层驱动进行发送的最大次数

#define RFID_NO_ACK_NUMBER    30
/********************************************************************************
* 变量定义
*********************************************************************************/
/**定义SC应答成组动作字与界面显示动作的对应关系型**/
static RFID_GRP_ACK_INTERFACE_TYPE const	s_stGrpActionInterfaceList[RFID_GRP_ACTION_MAX] = 
{
	/*SC应答的成组动作字**/				/**动作码类型**/					/**动作码**/
	{RFID_GRP_YIJIA ,					       RFID_FRAME_ACTION,					RFID_ACTION_YIJIA},
	{RFID_GRP_LALIU ,					       RFID_FRAME_ACTION,					RFID_ACTION_YIJIA},
	{RFID_GRP_TUILIU ,				       RFID_FRAME_ACTION,					RFID_ACTION_TUILIU},
	{RFID_GRP_LA_HOULIU ,			       RFID_FRAME_ACTION,					RFID_ACTION_LA_HOULIU},
	{RFID_GRP_SHOU_HUBANG ,			     RFID_FRAME_ACTION,					RFID_ACTION_SHOU_HUBANG},
															    
	{RFID_GRP_SHEN_HUBANG ,				   RFID_FRAME_ACTION,					RFID_ACTION_SHEN_HUBANG},
	{RFID_GRP_SHOU_HUBANG2 ,			   RFID_FRAME_ACTION,					RFID_ACTION_SHOU_HUBANG2},
	{RFID_GRP_SHEN_HUBANG2 ,			   RFID_FRAME_ACTION,					RFID_ACTION_SHEN_HUBANG2},
	{RFID_GRP_FANGMEI_PENWU ,			   RFID_FRAME_ACTION,					RFID_ACTION_FANGMEI_PENWU},
	{RFID_GRP_SHOUHUBANG_UNION ,	   RFID_FRAME_STATUS,					RFID_STATUS_GRP_SHOUHUBANG_UNION},//成组收护帮收伸缩梁联动
															    
	{RFID_GRP_SHENHUBANG_UNION ,	   RFID_FRAME_STATUS,					RFID_STATUS_GRP_SHENHUBANG_UNION},//伸互帮伸伸缩梁联动	
	{RFID_GRP_SHOU_SHENSUOLIANG ,    RFID_FRAME_ACTION,					RFID_ACTION_SHEN_SHENSUOLIANG},
	{RFID_GRP_SHEN_SHENSUOLIANG ,    RFID_FRAME_ACTION,					RFID_ACTION_SHOU_SHENSUOLIANG},
	{RFID_GRP_DINGLIANG_PENWU ,		   RFID_FRAME_ACTION,					RFID_ACTION_DINGLIANG_PENWU},
//	{RFID_GRP_FANGDINGMEI ,			   RFID_FRAME_ACTION,					RFID_ACTION_DINGLIANG_PENWU},
															    
	{RFID_GRP_BUYA ,							   RFID_FRAME_ACTION,					RFID_ACTION_SHENGZHU},	//  成组模式下,升立柱变成补压 
	{RFID_GRP_SHENG_TIAOLIU ,			   RFID_FRAME_ACTION,					RFID_ACTION_SHENG_TIAOLIU},	
	{RFID_GRP_JIANG_TIAOLIU ,			   RFID_FRAME_ACTION,					RFID_ACTION_JIANG_TIAOLIU},	
	{RFID_GRP_SHEN_HUBANG3 ,			   RFID_FRAME_ACTION,					RFID_ACTION_SHEN_HUBANG3},	
	{RFID_GRP_SHOU_HUBANG3 ,			   RFID_FRAME_ACTION,					RFID_ACTION_SHOU_HUBANG3}	
	
};

static u16 u16LiuShuiNumb;//流水号
static RFID_MNG_DATA_TYPE	s_stRfidSendBuf[RFID_SEND_BUF_SIZE];
u16 s_u16RfidSendBufWrite;
u16 s_u16RfidSendBufRead;
#if RFID_TEST
	stFRAME temp;
#endif	
static u32 u32SendTimer = 0x00;	
static u16 u16ScLsNumbBackup = 0x00;

extern SIGNAL_ACTION_HEART Heart;	

#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
	static u16 u16YkqSendScIdBackup = 0x00;
	static u16 u16YkqSendLsNumbBackup = 0x00;
#endif

#define WLSIZE sizeof(stUSARTDATA)


#define	USART_SEND_BUF_SIZE					30
stUSARTDATA	s_stRfidUsartSendBuf[USART_SEND_BUF_SIZE];
u16 s_u16RfidUsartSendBufWrite;
u16 s_u16RfidUsartSendBufRead;

static u8 u8tmp_Usart[128];

static u8 u8LiuShuiNumb = 0x00;
static u8 u8SendTime = 0x00;

u32 WlHeartTime = SendHeartTimerMAX;

static u8 u8WlVersion[10] = {0};

OS_EVENT *s_stRfidRecvd ;
OS_EVENT *s_stUsartRecvd ;   //串口接收信号量

OS_EVENT *s_stUsartSend ;   //串口接收信号量

u8 ACKFrameType;
u8 ACKData;

u8 WLUnconnectedFlag= 0;

extern ACTION_INFO ActionInformation;
/********************************************************************************
* 函数定义
*********************************************************************************/
/*****************************************************************
** 函数名：MessageForRfidRcved
** 输　入：无
** 输　出：无
** 功能描述：RFID数据接收后的回调函数，发出信号量
******************************************************************/
void RfidMessageForRfidRcved(void)
{
	OSSemPost(s_stRfidRecvd);	//RFID接收到数据
}
/*****************************************************************
** 函数名：RfidMngSetRecvCallBack
** 输　入：无
** 输　出：无
** 功能描述：RFID数据接收后的回调函数，发出信号量
******************************************************************/
u32 RfidMngSetRecvCallBack(void)
{
	return RFID_RcvIntTransMsgRegister((RcvIntTransMsgFunc)RfidMessageForRfidRcved);
}
/*****************************************************************
** 函数名：RfidRecvSemCreate
** 输　入：无
** 输　出：无
** 功能描述：RFID数据接收后的回调函数，发出信号量
******************************************************************/
u32 RfidRecvSemCreate(void)
{
	s_stRfidRecvd  = OSSemCreate(0);	//创建接收数据信号量
	if(g_u32ID != B_type)
	{
		s_stUsartRecvd  = OSSemCreate(0);	//创建接收数据信号量
		s_stUsartSend = OSSemCreate(0);
	}
	return 0x01;
}

/*********************************************************************************
** 函数名称: RfidMngInint
** 功能描述：RFID管理任务初始化
** 函数输入：无
** 函数输出：None
** 注意事项：
**********************************************************************************/
void RfidMngInint(void)
{
	u8 u8I = 0x00;
	u16 u16Status;
	
	for(u8I = 0x00; u8I < RFID_SEND_BUF_SIZE; u8I ++)
	{
		memset(&s_stRfidSendBuf[u8I],0x00,sizeof(RFID_MNG_DATA_TYPE));
	}
	
	s_u16RfidSendBufWrite = 0x00;
	s_u16RfidSendBufRead = 0x00;
	u16LiuShuiNumb = 0x00;	
	u32SendTimer = 0x00;
	u16ScLsNumbBackup = 0xFFFF;
	
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
		u16YkqSendScIdBackup = 0x00;
		u16YkqSendLsNumbBackup = 0x00;
	#endif
	if(g_u32ID == B_type)
	{

		// 设置RFID接收数据后发送信号量
		RfidMngSetRecvCallBack();

		/**设置RFID的默认地址**/
		LogicParamApi(LOGIC_GET_RFID_NUMBER, &u8I);
		SetLocalAddress(u8I);

		u8I = GetRfidCurStatus();//获取芯片状态
		if(u8I == CC1101_STATE_RX_OVERFLOW)
			RfidStrobe(CC1101_SFRX);    //冲洗 RX FIFO buffer.
		else if(u8I == CC1101_STATE_TX_UNDERFLOW)
			RfidStrobe(CC1101_SFTX);    //冲洗 TX FIFO buffer.
		else if(u8I == CC1101_STATE_IDLE)
			SetRfidSRX();
		else
			SetRFChipSleepMode();//让芯片进入休眠模式;
		u16Status = 0x00;
		while((u8I == CC1101_STATE_RX_OVERFLOW) || (u8I == CC1101_STATE_TX_UNDERFLOW) || (u8I == CC1101_STATE_IDLE))
		{						
			OSTimeDly(RFIDMNG_TASK_TIME_SET/TICK_TIME);
			u16Status ++;
			u8I = GetRfidCurStatus();//获取芯片状态
			if((u8I != CC1101_STATE_RX_OVERFLOW) && (u8I != CC1101_STATE_TX_UNDERFLOW) &&(u8I != CC1101_STATE_IDLE))
			{
				SetRFChipSleepMode();//让芯片进入休眠模式;
				break;
			}
			if(u16Status >= 10)
			{
				SetRFChipSleepMode();//让芯片进入休眠模式;
				break;
			}
		}		
	}
}

/*********************************************************************************
** 函数名称: RfidGrpAckGetRfidAction
** 功能描述：查找成组应答字所对应的RFID数据
** 函数输入：无
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32 RfidGrpAckGetRfidAction(RFID_GRP_ACTION_TYPE stScAckCode, RFID_FRAME_TYPE stFrameType, u32 *pData)
{
	u16 u16I;
	u32 u32ReturnValue = 0x00;
	
	for(u16I= 0x00;u16I < RFID_GRP_ACTION_MAX;u16I++)
	{
		if((s_stGrpActionInterfaceList[u16I].stGrpActionAckCode == stScAckCode) && (s_stGrpActionInterfaceList[u16I].stActionCodeType == stFrameType))
		{
			*(u32 *)pData = s_stGrpActionInterfaceList[u16I].u32ActionCode;
			u32ReturnValue = 0x01;
			break;
		}
	}

	return u32ReturnValue;
}

#if RFID_TEST
/*********************************************************************************
** 函数名称: RfidGrpAckGetScAckCode
** 功能描述：查找RFID数据所对应的成组应答字
** 函数输入：无
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32 RfidGrpAckGetScAckCode(RFID_GRP_ACTION_TYPE *stScAckCode, RFID_FRAME_TYPE stFrameType, RFID_DATA_TYPE stRfidData)
{
	u16 u16I;
	u32 u32ReturnValue = 0x00;
	
	for(u16I= 0x00;u16I < RFID_GRP_ACTION_MAX;u16I++)
	{
		if(((s_stGrpActionInterfaceList[u16I].stActionCode&0x3FFFFFFF) == (stRfidData&0x3FFFFFFF)) && (s_stGrpActionInterfaceList[u16I].stActionCodeType == stFrameType) )
		{
			*(RFID_GRP_ACTION_TYPE *)stScAckCode = s_stGrpActionInterfaceList[u16I].stGrpActionAckCode;
			u32ReturnValue = 0x01;
			break;
		}
	}

	return u32ReturnValue;
}
#endif



/********************************************************************************
* 功能描述： 插入usart发送队列
* 入口参数：
* 返回值：无
********************************************************************************/
u8 InsUsartTrsQueue(u8 *pFrame,u8 dlc,u8 FrameType , u8 ack,u8 isSend)
{
	u16 u16CrcTemp = 0x00;
	u8 u8ReturnFlag = 0x01;
	uint8_t  err;

	#if OS_CRITICAL_METHOD == 3
			OS_CPU_SR  cpu_sr = 0;
	#endif
	
	OS_ENTER_CRITICAL();
	memset(&(s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].SendBuf), 0x00  ,USARTSENDBUFSIZE);
	memcpy(&(s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].SendBuf), pFrame, dlc);
	s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].FrameType = FrameType;
	s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].ACK = ack;
	s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].Address = USART_YKQ_ADDRESS;
	s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].Serial = u8LiuShuiNumb;	
	s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].u8DLC = dlc;
	//计算crc
	Crc16UsartSendChick((u8*)&(s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].Address),s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].u8DLC+5,&u16CrcTemp);
	s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].CRCHigh = (u8)((u16CrcTemp&0xFF00)>>8);
	s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].CRCLow = (u8)(u16CrcTemp&0x00FF);
	OS_EXIT_CRITICAL();	
	//数据立即发送
	if(isSend)
	{
		USART_SendData(WL_USART, *((u8*)&s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite]));
		USART_ITConfig(WL_USART, USART_IT_TXE, ENABLE);
		s_stUsartSend->OSEventCnt = 0;
		OSSemPend(s_stUsartSend,20/TICK_TIME,&err);//等待数据接收完成
		u8ReturnFlag = (err == OS_ERR_TIMEOUT) ? 0 : 1;	
	}
	//数据存缓存
	else
	{
		s_u16RfidUsartSendBufWrite ++;
		s_u16RfidUsartSendBufWrite %= USART_SEND_BUF_SIZE;
	}
	u8LiuShuiNumb++;
	u8LiuShuiNumb %= STFRAME_LS_MAX;

	return u8ReturnFlag;
}

/***************************************************************************************
** 功能描述：发送应答数据
** 函数输入：无
** 函数输出：None
** 注意事项：
******************************************************************************************/
void SendResponseData(u8 Serial , u8 Data,u8 ft)
{
	u16 u16CrcTemp = 0;
	
	#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr = 0;
#endif
	OS_ENTER_CRITICAL();
	s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].Address = 2;
	s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].ACK = 0x00;
	s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].Serial = Serial;
	s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].FrameType = ft;
	s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].u8DLC = 0x01;
	s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].SendBuf[0] = Data;
	Crc16UsartSendChick((u8*)&(s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite]),s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].u8DLC+5,&u16CrcTemp);
	s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].CRCHigh = (u8)((u16CrcTemp&0xFF00)>>8);
	s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].CRCLow = (u8)(u16CrcTemp&0x00FF);
	s_u16RfidUsartSendBufWrite ++;
	s_u16RfidUsartSendBufWrite %= USART_SEND_BUF_SIZE;
	OS_EXIT_CRITICAL();
}

/*******************************************************************************************
**函数名称：HubCanTransformTx
**函数作用：HubCan发送数据处理
**函数参数：无
**函数输出：无
**注意事项：u8WaitAckFlag]:1,u16Interval应减去任务延时，u8WaitAckFlag:0，即为实际的发送间隔
*******************************************************************************************/
u8 RfidTransformTx(stFRAME *pFrame,u8 u8DevID,u16 u16SendTimes,u16 u16Interval,u8 u8WaitAckFlag)//本项目中统一使用扩展帧，无标准帧
{
	u16 u16TxBufWriteBackup = 0x00;
	u32 u32ReturnFlag = 0x01;
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
		OS_CPU_SR  cpu_sr = 0;
	#endif

	/**新数据加入时，自动停止之前任何数据的发送**/
	OS_ENTER_CRITICAL(); 
	if(s_u16RfidSendBufRead != s_u16RfidSendBufWrite)
	{
		s_u16RfidSendBufRead = s_u16RfidSendBufWrite;
	}
	OS_EXIT_CRITICAL();	

	/**防止队列满的情况出现**/
	u16TxBufWriteBackup = s_u16RfidSendBufWrite;
	u16TxBufWriteBackup++;
	u16TxBufWriteBackup %= RFID_SEND_BUF_SIZE;		
	if((u16TxBufWriteBackup != s_u16RfidSendBufRead))
	{
		OS_ENTER_CRITICAL(); 

		memcpy(&(s_stRfidSendBuf[s_u16RfidSendBufWrite].stFrame), pFrame, sizeof(stFRAME));
		s_stRfidSendBuf[s_u16RfidSendBufWrite].u8DestDevID = u8DevID;
		s_stRfidSendBuf[s_u16RfidSendBufWrite].u8WaitAckFlag = u8WaitAckFlag;
		s_stRfidSendBuf[s_u16RfidSendBufWrite].u8OtherAckFlag = NO_ACK;
		s_stRfidSendBuf[s_u16RfidSendBufWrite].u16SendInterval  = u16Interval;
		s_stRfidSendBuf[s_u16RfidSendBufWrite].u16SendTimes = u16SendTimes;
	
		s_u16RfidSendBufWrite ++;
		s_u16RfidSendBufWrite %= RFID_SEND_BUF_SIZE;
		
		OS_EXIT_CRITICAL();	
	}
	else
	{
		u32ReturnFlag =  0x00;//缓冲区已满，不能再加入队列中
	}	
	
	return u32ReturnFlag;
}

/*******************************************************************************************
**函数名称：RfidMngSendFrame
**函数作用：发送数据处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
u8 RfidMngSendFrame(RFID_MNG_DATA_TYPE *pData)//本项目中统一使用扩展帧，无标准帧
{
	u8 u8Buf[70] = {0};
	u8 u8Buf1[70] = {0};
	u32 u32SendLength = 0x00;
	u32 u32SendLength1 = 0x00;
	u32 u32ReturnValue = 0x00;
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
		OS_CPU_SR  cpu_sr = 0;
	#endif
	#if 0
		u32 u32SendTimes;
	#endif

	
	if((pData->stFrame.u32ID.ID.FrameType == RFID_FT_KEY_LONGDOWN)
		||(pData->stFrame.u32ID.ID.FrameType == RFID_FT_HEART))//按键连续按下，流水号应该改变
	{
		OS_ENTER_CRITICAL(); 
		pData->stFrame.u32ID.ID.LiuShuiNumb = u16LiuShuiNumb;
		
		u16LiuShuiNumb++;
		u16LiuShuiNumb %= STFRAME_LS_MAX;
		
		u32SendTimer = 0x00;//立即启动下次发送
		OS_EXIT_CRITICAL();	
	}
	else
	{
		pData->stFrame.u32ID.ID.LiuShuiNumb = u16LiuShuiNumb;
	}	
	// zzw 2023.12.26 增加长度超过8的处理
	if(pData->stFrame.u8DLC > 8)	//数据长度大于8
	{
		//第一包
		pData->stFrame.u32ID.ID.Sum = u16LiuShuiNumb; //u16LiuShuiNumb
		pData->stFrame.u32ID.ID.Sub = 0;
//		pData->stFrame.u8DLC = 8;
		pData->stFrame.u32ID.ID.LiuShuiNumb = pData->stFrame.u8DLC/8;; //pData->stFrame.u8DLC/8;
		u16LiuShuiNumb++;
		u16LiuShuiNumb %= STFRAME_LS_MAX;
		
		memcpy(u8Buf,&(pData->stFrame.u32ID),0x04);
		u32SendLength = 0x04;
		u8Buf[u32SendLength++] = 8;	//长度
		memcpy(u8Buf+u32SendLength,&(pData->stFrame.u8DT[0x00]),8);
		u32SendLength += 8;
		//第二包 子帧
		pData->stFrame.u32ID.ID.Sum = u16LiuShuiNumb;
		pData->stFrame.u32ID.ID.Sub = 1;
		pData->stFrame.u32ID.ID.LiuShuiNumb = 0;
		u16LiuShuiNumb++;
		u16LiuShuiNumb %= STFRAME_LS_MAX;
		memcpy(u8Buf1,&(pData->stFrame.u32ID),0x04);
		u32SendLength1 = 0x04;
		u8Buf1[u32SendLength1++] = pData->stFrame.u8DLC - 8;	//长度
		memcpy(u8Buf1+u32SendLength1,&(pData->stFrame.u8DT[0x08]),(pData->stFrame.u8DLC - 8)); //zzw 
		u32SendLength1 += pData->stFrame.u8DLC - 8;
	}
	else
	{
		memcpy(u8Buf,&(pData->stFrame.u32ID),0x04);
		u32SendLength = 0x04;
		u8Buf[u32SendLength++] = pData->stFrame.u8DLC;
		memcpy(u8Buf+u32SendLength,&(pData->stFrame.u8DT[0x00]),pData->stFrame.u8DLC);
		u32SendLength += pData->stFrame.u8DLC;
	}
	
	#if 0//经验证，此方式不行(对码都不能成功)
		RFID_SetRfidSRX();
		u32SendTimes = 0x00;
		u32ReturnValue= 0x00;	
		while(u32SendTimes < RFID_DRIVER_SEND_TIMES_MAX)
		{
			u32SendLength = RFID_GetRfidSendEnable();
			if(u32SendLength == RET_OK)
			{
				RFID_SendData(pData->u8DestDevID,u8Buf,&u32SendLength);
				u32ReturnValue= 0x01;
				break;
			}
			u32SendTimes++;
			if(u32SendTimes >= RFID_DRIVER_SEND_TIMES_MAX)
				break;
			OSTimeDly(RFIDMNG_TASK_TIME_SET/TICK_TIME);//发送间隔	
		}

		if(u32SendTimes >= RFID_DRIVER_SEND_TIMES_MAX)//反馈设备故障
		{
			InterfaceInsertNote(INTERFACE_NOTE_RFID_SEND_FAIL);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//显示主屏所有信息	
			OSSemPost(g_stSemInterface);
		}
	
	#else
	//D版usart发送
	if(g_u32ID != B_type)
	{	if(pData->stFrame.u8DLC > 8)
		{
			//发第一帧
			OS_ENTER_CRITICAL();
			memset(u8tmp_Usart,0x00,u32SendLength+1);
			u8tmp_Usart[0] = pData->u8DestDevID;
			memcpy(&u8tmp_Usart[1],u8Buf,u32SendLength);
			OS_EXIT_CRITICAL();		
			InsUsartTrsQueue(u8tmp_Usart,u32SendLength+1,WIRELESSDATA,NOACK,1);			
			OSTimeDly(10);			
			//发第二帧
			OS_ENTER_CRITICAL();
			memset(u8tmp_Usart,0x00,u32SendLength1+1);
			u8tmp_Usart[0] = pData->u8DestDevID;
			memcpy(&u8tmp_Usart[1],u8Buf1,u32SendLength1);
			OS_EXIT_CRITICAL();		
			InsUsartTrsQueue(u8tmp_Usart,u32SendLength1+1,WIRELESSDATA,NOACK,1);			
		}
		else
		{
			OS_ENTER_CRITICAL();
			memset(u8tmp_Usart,0x00,u32SendLength+1);
			u8tmp_Usart[0] = pData->u8DestDevID;
			memcpy(&u8tmp_Usart[1],u8Buf,u32SendLength);
			OS_EXIT_CRITICAL();		
			InsUsartTrsQueue(u8tmp_Usart,u32SendLength+1,WIRELESSDATA,NOACK,1);
		}
	}
	//B版RFID发送
	else
	{
		RFID_SendData(pData->u8DestDevID,u8Buf,&u32SendLength);
	}
		u32ReturnValue= 0x01;
	#endif
	
	return u32ReturnValue;
}

/***************************************************************************************
** 函数名称: RfidMngSendDatacolibarate
** 功能描述：发送对码数据
** 函数输入：无
** 函数输出：None
** 注意事项：
******************************************************************************************/
u32 RfidMngSendDataColibarate(void)
{
	stFRAME stSendFrame;
	u8 u8YkqID;
	u16 u16ScID;
	u16 u16DelayTime = 0x00;
	u16 u16SendTimes = 0x00;
	u32 u32RData;
	
	LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);
	if(u16DelayTime < LOGIC_SEND_TIMEOUT_MIN)//等待时间为0时，防止出现死等的情况出现
		u16DelayTime = LOGIC_SEND_TIMEOUT_MIN;
	else if(u16DelayTime > LOGIC_SEND_TIMEOUT_MAX)
		u16DelayTime = LOGIC_SEND_TIMEOUT_MAX;
	else
		;
	
	LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
	if(u16SendTimes > LOGIC_SEND_TIMES_MAX)
		u16SendTimes = LOGIC_SEND_TIMES_MAX;
	else if(u16SendTimes <LOGIC_SEND_TIMES_MIN)
		u16SendTimes = LOGIC_SEND_TIMES_MIN;
	else
		;
		
	memset(&stSendFrame,0x00,sizeof(stFRAME));
	
	stSendFrame.u32ID.ID.RxID = (u8)(SC_DEV_TYPE&0xFF);
	stSendFrame.u32ID.ID.TxID = (u8)(WL_DEV_TYPE&0xFF);
	stSendFrame.u32ID.ID.FrameType = RFID_FT_COLIBARATE;
	stSendFrame.u32ID.ID.Sum = 0;
	stSendFrame.u32ID.ID.Sub = 0;
	stSendFrame.u32ID.ID.ACK = NO_ACK;
	stSendFrame.u32ID.ID.Reservd = 0;
	stSendFrame.u32ID.ID.NoUsed = 0;

	stSendFrame.u8DLC = 0x08;

	LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB, &u16ScID);
	
	LogicParamApi(LOGIC_GET_YKQ_NUMBER, &u8YkqID);

	stSendFrame.u8DT[0x02] = (u8)(YKQ_SIMPLE_FUNCTION<<7);
		
	stSendFrame.u8DT[0x03] = 0x00;
	stSendFrame.u8DT[0x04] = u16ScID;
	stSendFrame.u8DT[0x05] = u8YkqID;
	stSendFrame.u8DT[0x06] = (ActionInformation.Head.Verify)&0x00ff;
	stSendFrame.u8DT[0x07] = ((ActionInformation.Head.Verify)&0xff00)>>8;
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
		u32RData = 0x00;
		LogicRunInfApi(LOGIC_GET_COLIBRATE_SC_TYPE,&u32RData);
		if(u32RData == LOGIC_COLIBRATE_SC_INCREASE_DECREASE)	
		{
			stSendFrame.u8DT[0x02] |= (u8)((LOGIC_COLIBRATE_SC_INCREASE_DECREASE-0x01)<<6);//2017.01.12.依据最新协议修改
			/**2016.03.15,依据会议纪要,+-键时，发送8次，间隔150ms**/
//			if(u16ScID != IN_SC_SS_NUMB)
//				RfidTransformTx(&stSendFrame, (u8)u16ScID,u16SendTimes+0x05, u16DelayTime+50-RFIDMNG_TASK_TIME_SET,ACK);//发送次数比参数多两次
//			else
//				RfidTransformTx(&stSendFrame, SYSTEM_SS_ID,u16SendTimes+0x05, u16DelayTime+50-RFIDMNG_TASK_TIME_SET,ACK);//发送次数比参数多两次
			if(u16ScID != IN_SC_SS_NUMB)
				RfidTransformTx(&stSendFrame, (u8)u16ScID,u16SendTimes+0x1b, u16DelayTime-RFIDMNG_TASK_TIME_SET,ACK);//发送次数30次jhyjhy
			else
				RfidTransformTx(&stSendFrame, SYSTEM_SS_ID,u16SendTimes+0x05, u16DelayTime+50-RFIDMNG_TASK_TIME_SET,ACK);//发送次数比参数多两次			
			
		}
		else
		{
			stSendFrame.u8DT[0x02] |= (u8)((LOGIC_COLIBRATE_SC_SHIFT_BUTTON-0x01)<<6);//2017.01.12.依据最新协议修改
			if(u16ScID != IN_SC_SS_NUMB)
				RfidTransformTx(&stSendFrame, (u8)u16ScID,u16SendTimes+0x02, u16DelayTime-RFIDMNG_TASK_TIME_SET,ACK);//发送次数比参数多两次
			else
				RfidTransformTx(&stSendFrame, SYSTEM_SS_ID,u16SendTimes+0x02, u16DelayTime-RFIDMNG_TASK_TIME_SET,ACK);//发送次数比参数多两次			
		}
	#else
		if(u16ScID != IN_SC_SS_NUMB)
			RfidTransformTx(&stSendFrame, (u8)u16ScID,u16SendTimes+0x02, u16DelayTime-RFIDMNG_TASK_TIME_SET,ACK);//发送次数比参数多两次
		else
			RfidTransformTx(&stSendFrame, SYSTEM_SS_ID,u16SendTimes+0x02, u16DelayTime-RFIDMNG_TASK_TIME_SET,ACK);//发送次数比参数多两次
	#endif
	
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
		u16YkqSendScIdBackup = u16ScID;//备份
		u16YkqSendLsNumbBackup = u16LiuShuiNumb;//进行备份
		u16LiuShuiNumb = 0x00;//每次对码从0开始发送
	#endif

	
#if RFID_TEST
	temp.u32ID.ID.RxID = (u8)(WL_DEV_TYPE&0xFF);
	temp.u32ID.ID.TxID = (u8)(SC_DEV_TYPE&0xFF);
	temp.u32ID.ID.FrameType = RFID_FT_COLIBARATE_ACK;
	temp.u32ID.ID.Sum = 0;
	temp.u32ID.ID.Sub = 0;
	temp.u32ID.ID.ACK = ACK;
	temp.u32ID.ID.Reservd = 0;
	temp.u32ID.ID.NoUsed = 0;
	temp.u8DLC = 0x08;
	temp.u8DT[0x00] = 0x00;
	temp.u8DT[0x01] = 0x00;
	temp.u8DT[0x02] = 0x00;
	temp.u8DT[0x03] = (u8ScID&0xFF);
	temp.u8DT[0x04] = 0x00;
	temp.u8DT[0x05] = u8YkqID;
	temp.u8DT[0x06] = 0x00;
	temp.u8DT[0x07] = 0x01;	
#endif		
	return 0x01;
}

/***************************************************************************************
** 函数名称: RfidMngSendDatacolibarate
** 功能描述：发送解码数据
** 函数输入：无
** 函数输出：None
** 注意事项：
******************************************************************************************/
u32 RfidMngSendDataUnColibarate(void)
{
	stFRAME stSendFrame;
	u8 u8YkqID;
	u16 u16ScID;

	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
		OS_CPU_SR  cpu_sr = 0;
	#endif

	OS_ENTER_CRITICAL(); 
	u16ScLsNumbBackup = 0xFFFF;
	OS_EXIT_CRITICAL(); 

	memset(&stSendFrame,0x00,sizeof(stFRAME));
	
	stSendFrame.u32ID.ID.RxID = (u8)(SC_DEV_TYPE&0xFF);
	stSendFrame.u32ID.ID.TxID = (u8)(WL_DEV_TYPE&0xFF);
	stSendFrame.u32ID.ID.FrameType = RFID_FT_WL_UNCOLIBATARE;
	stSendFrame.u32ID.ID.Sum = 0;
	stSendFrame.u32ID.ID.Sub = 0;
	stSendFrame.u32ID.ID.ACK = NO_ACK;
	stSendFrame.u32ID.ID.Reservd = 0;
	stSendFrame.u32ID.ID.NoUsed = 0;

	stSendFrame.u8DLC = 0x08;
	
	LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB, &u16ScID);
	
	LogicParamApi(LOGIC_GET_YKQ_NUMBER, &u8YkqID);

	stSendFrame.u8DT[0x03] = (u8)(u16ScID&0xFF);
	stSendFrame.u8DT[0x04] = u8YkqID;
//	stSendFrame.u8DT[0x05] = u8YkqID;

	if(u16ScID != IN_SC_SS_NUMB)
		RfidTransformTx(&stSendFrame, (u8)u16ScID,0x03, 50,NO_ACK);
	else 
		RfidTransformTx(&stSendFrame, SYSTEM_SS_ID,0x03, 50,NO_ACK);

	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
		if(u16ScID != u16YkqSendScIdBackup)
		{
			u16ScID = u16LiuShuiNumb;
			u16LiuShuiNumb = u16YkqSendLsNumbBackup;//进行恢复	
			u16YkqSendLsNumbBackup = u16ScID;
		}
	#endif
	
	return 0x01;
}

/***************************************************************************************
** 函数名称: ProtocolTransRx
** 功能描述：接收动作字时，原协议为1bit表示1个动作，新协议为1个动作序号表示一个动作。本函数的功能是协议转换
** 函数输入：stFrameType：接收的结构体数据
** 函数输出：动作码
** 注意事项：
******************************************************************************************/
u64 ProtocolTransRx(stFRAME *pFrame)
{
	u8 u8ActionSerialNum[6];	//最多可同时接收6个动作
	u8 u8i = 0;
	u32 u32j = 0;
	static u64 u64k;
	u64k = 0x00;

	u32j |= (u32)(pFrame->u8DT[0x00]) ;		
	u32j |= (u32)(pFrame->u8DT[0x01]) << 8 ;
	u32j |= (u32)(pFrame->u8DT[0x02]) << 16 ;
	u32j |= (u32)(pFrame->u8DT[0x03] & 0x3F) << 24;	
	for(u8i = 0; u8i < 5; u8i++)	//这个30位数据包含了5个有效位为6位的数据，取出这5个数据(每个数据表示1个动作序号)
	{
		u8ActionSerialNum[u8i] = (u8)((u32j & ((u32)0x3F << (u8i * 6))) >> (u8i * 6));
	}
	u8ActionSerialNum[5] = pFrame->u8DT[0x07] >> 0x02;	//第6个动作序号在[0x07]的高6位中
	for(u8i = 0; u8i < 6; u8i++)	//将这6个动作序号转换为动作位
	{
		if(u8ActionSerialNum[u8i] != 0x00)
			u64k |= (u64)0x01 << (u8ActionSerialNum[u8i] - 1);
	}
	u64k |= (u64)(0x01) << 62;	//置动作控制码标识
	return u64k;
}

/***************************************************************************************
** 函数名称: ProtocolTransTx
** 功能描述：发送动作字时，原协议为1bit表示1个动作，新协议为1个动作序号表示一个动作。本函数的功能是协议转换
** 函数输入：stFrameType：要发送的结构体数据；u64FrameData：动作码
** 函数输出：None
** 注意事项：
******************************************************************************************/
u32 ProtocolTransTx(stFRAME *pFrame,u64 u64FrameData)
{
	u8 u8ActionSerialNum[6];	//最多可同时发送6个动作
	u8 u8i,u8j=0;
	static u64 u64k;
	u64k = 0x00;
	memset(u8ActionSerialNum, 0x00, 6);
	for(u8i = 0; u8i < MAX_ACTION_NUM; u8i++)	
	{
		if(u64FrameData & ((u64)0x01 << u8i))		//逐位判断是否含动作标志位
		{
			u8ActionSerialNum[u8j] = u8i + 1;
			u8j++;
			if(u8j > 5) break;
			u64k = (u64)(((u64)0x01 << (u8i + 1)) - 1);
			if(((u64FrameData & 0x00FFFFFFFFFFFFFF) & (u64)(~u64k)) == (u64)0x00) //判断是否还有动作标志?
				break;
		}
	}
	u64k = 0x00;
	//每个动作序号的低6位有效，前5个动作序号取出后组成30bit长字节，再按byte赋值给发送结构体;第6个动作的低6位单独放于u8DT[7]的高6位。
	for(u8i = 0; u8i < 5; u8i++) 
	{
		u64k |= ((u64)(u8ActionSerialNum[u8i] & 0x3F) << (u8i * 6));
	}
	for(u8i = 0; u8i < 4; u8i++)
	{
		pFrame->u8DT[u8i] = (u8)((u64k & ((u64)(0xFF) << (u8i * 8))) >> (u8i * 8));
	}
	pFrame->u8DT[0x07] = (u8)(u8ActionSerialNum[0x05] & 0x3F) << 0x02;
	return 0x01;
}
/***************************************************************************************
** 函数名称: RfidMngSendDatacolibarate
** 功能描述：发送动作控制命令
** 函数输入：无
** 函数输出：None
** 注意事项：
******************************************************************************************/
u32 RfidMngSendDataKeyDown(u8 ActionType,u8 ActionID,u8 ActionOrder)
{
	stFRAME stSendFrame;
	u8 u8YkqID;
	u16 u16ScID,CtrlID;
	u16 u16DelayTime = 0x00;
	u16 u16SendTimes = 0x00;
	u16 u16GrpRemoSc,u16GrpNearSc,u16Direction;
	u16 RunStatus;
	LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);
	if(u16DelayTime < LOGIC_SEND_TIMEOUT_MIN)//等待时间为0时，防止出现死等的情况出现
		u16DelayTime = LOGIC_SEND_TIMEOUT_MIN;
	else if(u16DelayTime > LOGIC_SEND_TIMEOUT_MAX)
		u16DelayTime = LOGIC_SEND_TIMEOUT_MAX;
	else
		;
	
	LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
	if(u16SendTimes > LOGIC_SEND_TIMES_MAX)
		u16SendTimes = LOGIC_SEND_TIMES_MAX;
	else if(u16SendTimes <LOGIC_SEND_TIMES_MIN)
		u16SendTimes = LOGIC_SEND_TIMES_MIN;
	else
		;
	LogicParamApi(LOGIC_GET_YKQ_NUMBER, &u8YkqID);
	LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB, &u16ScID);
	LogicRunInfApi(LOGIC_GET_RUN_STATUS, &RunStatus);
	if(RunStatus == LOGIC_STATUS_RFID_SC_NEIGHBORL)
		LogicRunInfApi(LOGIC_GET_CONTRL_SC_NUMB, &CtrlID);
	else
		LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB, &CtrlID);
	if(RunStatus != LOGIC_STATUS_RFID_SC_GRP)
	{
		u16GrpRemoSc = 0;
		u16GrpNearSc = 0;
		u16Direction = 0;
	}
	else
	{
		LogicRunInfApi(LOGIC_GET_GRP_REMOTE_SC_NUMB,&u16GrpRemoSc);//远端架号
		LogicRunInfApi(LOGIC_GET_GRP_NEAR_SC_NUMB,&u16GrpNearSc);//近端架号	
		LogicRunInfApi(LOGIC_GET_GRP_ACTION_DIRECTION,&u16Direction);//执行方向
	}
	memset(&stSendFrame,0x00,sizeof(stFRAME));
	
	stSendFrame.u32ID.ID.RxID = (u8)(SC_DEV_TYPE&0xFF);
	stSendFrame.u32ID.ID.TxID = (u8)(WL_DEV_TYPE&0xFF);
	stSendFrame.u32ID.ID.FrameType = RFID_FT_KEY_DOWN;
	stSendFrame.u32ID.ID.Sum = 0;
	stSendFrame.u32ID.ID.Sub = 0;
	stSendFrame.u32ID.ID.LiuShuiNumb = u16LiuShuiNumb;
	stSendFrame.u32ID.ID.ACK = NO_ACK;
	stSendFrame.u32ID.ID.Reservd = 0;
	stSendFrame.u32ID.ID.NoUsed = 0;

	stSendFrame.u8DLC = 0x08;
	
	stSendFrame.u8DT[0x00] = u8YkqID;
	stSendFrame.u8DT[0x01] = (u8)(CtrlID&0xFF);
	stSendFrame.u8DT[0x02] = ActionType;
	stSendFrame.u8DT[0x03] = ActionID;
	stSendFrame.u8DT[0x04] = ActionOrder;
	stSendFrame.u8DT[0x05] = (u8)(u16GrpNearSc&0xff);
	stSendFrame.u8DT[0x06] = (u8)(u16GrpRemoSc&0xff);
	stSendFrame.u8DT[0x07] = (u8)(u16Direction&0xff);	
	//按键按下，发5次，	
	if(u16ScID != IN_SC_SS_NUMB)	
		RfidTransformTx(&stSendFrame, (u8)u16ScID,u16SendTimes, u16DelayTime-RFIDMNG_TASK_TIME_SET,ACK);
	else
		RfidTransformTx(&stSendFrame, SYSTEM_SS_ID,u16SendTimes, u16DelayTime-RFIDMNG_TASK_TIME_SET,ACK);
		
	
#if RFID_TEST
	temp.u32ID.ID.RxID = (u8)(WL_DEV_TYPE&0xFF);
	temp.u32ID.ID.TxID = (u8)(SC_DEV_TYPE&0xFF);
	temp.u32ID.ID.FrameType = RFID_FT_KEY_DOWN_ACK;
	temp.u32ID.ID.Sum = 0;
	temp.u32ID.ID.Sub = 0;
	temp.u32ID.ID.ACK = NO_ACK;
	temp.u32ID.ID.Reservd = 0;
	temp.u32ID.ID.NoUsed = 0;
	temp.u8DLC = 0x08;
	temp.u8DT[0x00] = (u8)(u32FrameData&0x000000FF);
	temp.u8DT[0x01] = (u8)((u32FrameData&0x0000FF00)>>8);
	temp.u8DT[0x02] = (u8)((u32FrameData&0x00FF0000)>>16);
	
	temp.u8DT[0x03] =  (u8)(stFrameType);
	temp.u8DT[0x03] <<= 6;
	temp.u8DT[0x03] |= (u8)((u32FrameData&0xFF000000)>>24);
	
	temp.u8DT[0x04] = (u8ScID&0xFF);
	temp.u8DT[0x05] = 0x00;
	temp.u8DT[0x06] = u8YkqID;
	temp.u8DT[0x07] = 0x01;	
#endif	
	return 0x01;
}

/***************************************************************************************
** 函数名称: RfidMngNearCtrlReq
** 功能描述：邻架控制请求
** 函数输入：无
** 函数输出：None
** 注意事项：
******************************************************************************************/
u32 RfidMngNearCtrlReq(u8 NearScNum)
{
	stFRAME stSendFrame;
	u8 u8YkqID;
	u16 u16ScID,CtrlID;
	u16 u16DelayTime = 0x00;
	u16 u16SendTimes = 0x00;
	u16 RunStatus;
	LogicParamApi(LOGIC_GET_RFID_TINMEOUT, &u16DelayTime);
	if(u16DelayTime < LOGIC_SEND_TIMEOUT_MIN)//等待时间为0时，防止出现死等的情况出现
		u16DelayTime = LOGIC_SEND_TIMEOUT_MIN;
	else if(u16DelayTime > LOGIC_SEND_TIMEOUT_MAX)
		u16DelayTime = LOGIC_SEND_TIMEOUT_MAX;
	else
		;
	
	LogicParamApi(LOGIC_GET_RFID_SENDTIMES, &u16SendTimes);
	if(u16SendTimes > LOGIC_SEND_TIMES_MAX)
		u16SendTimes = LOGIC_SEND_TIMES_MAX;
	else if(u16SendTimes <LOGIC_SEND_TIMES_MIN)
		u16SendTimes = LOGIC_SEND_TIMES_MIN;
	else
		;
	LogicParamApi(LOGIC_GET_YKQ_NUMBER, &u8YkqID);
	LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB, &u16ScID);
	LogicRunInfApi(LOGIC_GET_RUN_STATUS, &RunStatus);
	
	LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB, &CtrlID);		//对码架号
	memset(&stSendFrame,0x00,sizeof(stFRAME));
	
	stSendFrame.u32ID.ID.RxID = (u8)(SC_DEV_TYPE&0xFF);
	stSendFrame.u32ID.ID.TxID = (u8)(WL_DEV_TYPE&0xFF);
	stSendFrame.u32ID.ID.FrameType = RFID_FT_NEAR_CTRL_REQ;
	stSendFrame.u32ID.ID.Sum = 0;
	stSendFrame.u32ID.ID.Sub = 0;
	stSendFrame.u32ID.ID.LiuShuiNumb = u16LiuShuiNumb;
	stSendFrame.u32ID.ID.ACK = NO_ACK;
	stSendFrame.u32ID.ID.Reservd = 0;
	stSendFrame.u32ID.ID.NoUsed = 0;

	stSendFrame.u8DLC = 0x03;
	
	stSendFrame.u8DT[0x00] = u8YkqID;
	stSendFrame.u8DT[0x01] = (u8)(CtrlID&0xFF);
	stSendFrame.u8DT[0x02] = NearScNum;
	
	RfidTransformTx(&stSendFrame, (u8)u16ScID,u16SendTimes, u16DelayTime-RFIDMNG_TASK_TIME_SET,ACK);
	
	return 0x01;
}

/***************************************************************************************
** 函数名称: RfidMngSendHeart
** 功能描述：心跳帧
** 函数输入：无
** 函数输出：u64FrameData：40个单动作，每一位代表一个动作id，ActionType：动作类型
** 注意事项：单动作手动控制时sendmode为1，其他情况都为0
******************************************************************************************/
u32 RfidMngSendHeart(u64 u64FrameData,u8 ActionType,u8 Sendmode)
{
	stFRAME stSendFrame;
	u8 u8YkqID;
	u16 u16ScID;
	
	memset(&stSendFrame,0x00,sizeof(stFRAME));
	
	stSendFrame.u32ID.ID.RxID = (u8)(SC_DEV_TYPE&0xFF);
	stSendFrame.u32ID.ID.TxID = (u8)(WL_DEV_TYPE&0xFF);
	stSendFrame.u32ID.ID.FrameType = 0;	//zzw2023.12.25 寸二心跳类型
	stSendFrame.u32ID.ID.Sum = 0;
	stSendFrame.u32ID.ID.Sub = 0;
	stSendFrame.u32ID.ID.ACK = NO_ACK;
	stSendFrame.u32ID.ID.Reservd = 0;
	stSendFrame.u32ID.ID.NoUsed = 0;

	if(Sendmode == 1)		//单动作手动控制时
	{
		stSendFrame.u8DLC = 11;
		
		LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB, &u16ScID);
		
		LogicParamApi(LOGIC_GET_YKQ_NUMBER, &u8YkqID);

		stSendFrame.u8DT[0x00] = u16ScID;
		stSendFrame.u8DT[0x01] = u8YkqID;
		stSendFrame.u8DT[0x02] = ActionType;
		//zzw2023.12.25 心跳支持到64个
		stSendFrame.u8DT[0x03] = (u8)( u64FrameData&0x00000000000000ff);
		stSendFrame.u8DT[0x04] = (u8)((u64FrameData&0x000000000000ff00)>>8);
		stSendFrame.u8DT[0x05] = (u8)((u64FrameData&0x0000000000ff0000)>>16);
		stSendFrame.u8DT[0x06] = (u8)((u64FrameData&0x00000000ff000000)>>24);
		stSendFrame.u8DT[0x07] = (u8)((u64FrameData&0x000000ff00000000)>>32);
		stSendFrame.u8DT[0x08] = (u8)((u64FrameData&0x0000ff0000000000)>>40);
		stSendFrame.u8DT[0x09] = (u8)((u64FrameData&0x00ff000000000000)>>48);
		stSendFrame.u8DT[0x0a] = (u8)((u64FrameData&0xff00000000000000)>>56);	
	}
	else
	{
		stSendFrame.u8DLC = 8;
		
		LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB, &u16ScID);
		
		LogicParamApi(LOGIC_GET_YKQ_NUMBER, &u8YkqID);

		stSendFrame.u8DT[0x00] = u16ScID;
		stSendFrame.u8DT[0x01] = u8YkqID;
		stSendFrame.u8DT[0x02] = ActionType;
		//zzw2023.12.25 心跳支持到48个
		stSendFrame.u8DT[0x03] = (u8)( u64FrameData&0x00000000000000ff);
		stSendFrame.u8DT[0x04] = (u8)((u64FrameData&0x000000000000ff00)>>8);
		stSendFrame.u8DT[0x05] = (u8)((u64FrameData&0x0000000000ff0000)>>16);
		stSendFrame.u8DT[0x06] = (u8)((u64FrameData&0x00000000ff000000)>>24);
		stSendFrame.u8DT[0x07] = (u8)((u64FrameData&0x000000ff00000000)>>32);	
	}

	if(Sendmode == 1)		//单动作手动控制时
	{
		if(u16ScID != IN_SC_SS_NUMB)			
			RfidTransformTx(&stSendFrame, (u8)u16ScID,RFID_SEND_TIMES_LIMITLESS, 70,NO_ACK);
		else
			RfidTransformTx(&stSendFrame, SYSTEM_SS_ID,RFID_SEND_TIMES_LIMITLESS, 70,NO_ACK);		
	}
	else
	{
		if(u16ScID != IN_SC_SS_NUMB)			
			RfidTransformTx(&stSendFrame, (u8)u16ScID,RFID_SEND_TIMES_LIMITLESS, 5000,NO_ACK);
		else
			RfidTransformTx(&stSendFrame, SYSTEM_SS_ID,RFID_SEND_TIMES_LIMITLESS, 5000,NO_ACK);			
	}
	
	return 0x01;
}

/***************************************************************************************
** 函数名称: RfidMngSendDatacolibarate
** 功能描述：发送按键长按数据
** 函数输入：无
** 函数输出：None
** 注意事项：只在单架、邻架、隔架动作时有效。(成组动作无效)
******************************************************************************************/
u32 RfidMngSendDataKeyLongDown(RFID_FRAME_TYPE stFrameType, u64 u64FrameData)
{
	stFRAME stSendFrame;
	u8 u8YkqID;
	u16 u16ScID;
	
	memset(&stSendFrame,0x00,sizeof(stFRAME));
	
	stSendFrame.u32ID.ID.RxID = (u8)(SC_DEV_TYPE&0xFF);
	stSendFrame.u32ID.ID.TxID = (u8)(WL_DEV_TYPE&0xFF);
	stSendFrame.u32ID.ID.FrameType = RFID_FT_KEY_LONGDOWN;
	stSendFrame.u32ID.ID.Sum = 0;
	stSendFrame.u32ID.ID.Sub = 0;
	stSendFrame.u32ID.ID.ACK = NO_ACK;
	stSendFrame.u32ID.ID.Reservd = 0;
	stSendFrame.u32ID.ID.NoUsed = 0;

	stSendFrame.u8DLC = 0x08;
	
	LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB, &u16ScID);
	
	LogicParamApi(LOGIC_GET_YKQ_NUMBER, &u8YkqID);

	if(stFrameType != 0x01)	//发送非动作字时，不进行协议转换；发送动作字时，进行协议转换
	{
		stSendFrame.u8DT[0x00] = (u8)(u64FrameData&0x000000FF);
		stSendFrame.u8DT[0x01] = (u8)((u64FrameData&0x0000FF00)>>8);
		stSendFrame.u8DT[0x02] = (u8)((u64FrameData&0x00FF0000)>>16);

		stSendFrame.u8DT[0x03] = (u8)(stFrameType);
		stSendFrame.u8DT[0x03] <<= 6;
		stSendFrame.u8DT[0x03] |= (u8)((u64FrameData&0xFF000000)>>24);

		stSendFrame.u8DT[0x04] = (u8)(u16ScID&0xFF);
		stSendFrame.u8DT[0x05] = (u8)((u16ScID&0xFF00)>>8);
		stSendFrame.u8DT[0x06] = u8YkqID;

		stSendFrame.u8DT[0x07] = 0x00;
	}
	else	//对动作字，按新协议：30个动作增加至56个动作 2019.12.24 parry
	{
		ProtocolTransTx(&stSendFrame, u64FrameData);
		stSendFrame.u8DT[0x03] |= (stFrameType <<= 6);
		stSendFrame.u8DT[0x04] = (u8)(u16ScID&0xFF);
		stSendFrame.u8DT[0x05] = (u8)((u16ScID&0xFF00)>>8);
		stSendFrame.u8DT[0x06] = u8YkqID;
	}
	if(g_u32ID ==  B_type)
	{
		if(u16ScID != IN_SC_SS_NUMB)			
			RfidTransformTx(&stSendFrame, (u8)u16ScID,RFID_SEND_TIMES_LIMITLESS, 40,NO_ACK);
		else
			RfidTransformTx(&stSendFrame, SYSTEM_SS_ID,RFID_SEND_TIMES_LIMITLESS, 40,NO_ACK);
	}
	else
	{
		if(u16ScID != IN_SC_SS_NUMB)			
			RfidTransformTx(&stSendFrame, (u8)u16ScID,RFID_SEND_TIMES_LIMITLESS, 50,NO_ACK);
		else
			RfidTransformTx(&stSendFrame, SYSTEM_SS_ID,RFID_SEND_TIMES_LIMITLESS, 50,NO_ACK);	
	}

	return 0x01;
}

/***************************************************************************************
** 函数名称: RfidMngSendDatacolibarate
** 功能描述：发送按键抬起对码数据
** 函数输入：无
** 函数输出：None
** 注意事项：
******************************************************************************************/
u32 RfidMngSendDataKeyUp(RFID_FRAME_TYPE stFrameType, u64 u64FrameData)
{
	stFRAME stSendFrame;
	u8 u8YkqID;
	u16 u16ScID;
	u32 u32Y = 0x00;
	static u64 u64ButtonVale;
	u64ButtonVale =  0x00;
	
	memset(&stSendFrame,0x00,sizeof(stFRAME));
	
	stSendFrame.u32ID.ID.RxID = (u8)(SC_DEV_TYPE&0xFF);
	stSendFrame.u32ID.ID.TxID = (u8)(WL_DEV_TYPE&0xFF);
	stSendFrame.u32ID.ID.FrameType = RFID_FT_KEY_UP;
	stSendFrame.u32ID.ID.Sum = 0;
	stSendFrame.u32ID.ID.Sub = 0;
	stSendFrame.u32ID.ID.ACK = NO_ACK;
	stSendFrame.u32ID.ID.Reservd = 0;
	stSendFrame.u32ID.ID.NoUsed = 0;

	stSendFrame.u8DLC = 0x08;
	
	LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB, &u16ScID);
	
	LogicParamApi(LOGIC_GET_YKQ_NUMBER, &u8YkqID);
	
	if(stFrameType != 0x01)	//发送非动作字时，不进行协议转换；发送动作字时，进行协议转换
	{
		stSendFrame.u8DT[0x00] = (u8)(u64FrameData&0x000000FF);
		stSendFrame.u8DT[0x01] = (u8)((u64FrameData&0x0000FF00)>>8);
		stSendFrame.u8DT[0x02] = (u8)((u64FrameData&0x00FF0000)>>16);

		stSendFrame.u8DT[0x03] = (u8)(stFrameType);
		stSendFrame.u8DT[0x03] <<= 6;
		stSendFrame.u8DT[0x03] |= (u8)((u64FrameData&0xFF000000)>>24);

		stSendFrame.u8DT[0x04] = (u8)(u16ScID&0xFF);
		stSendFrame.u8DT[0x05] = (u8)((u16ScID&0xFF00)>>8);
		stSendFrame.u8DT[0x06] = u8YkqID;

		stSendFrame.u8DT[0x07] = 0x00;
	}
	else	//对动作字，按新协议：30个动作增加至56个动作 2019.12.24 parry
	{
		ProtocolTransTx(&stSendFrame, u64FrameData);
		stSendFrame.u8DT[0x03] |= (stFrameType <<= 6);
		stSendFrame.u8DT[0x04] = (u8)(u16ScID&0xFF);
		stSendFrame.u8DT[0x05] = (u8)((u16ScID&0xFF00)>>8);
		stSendFrame.u8DT[0x06] = u8YkqID;
	}
	/*获取当前的控制模式**/	
	LogicRunInfApi(LOGIC_GET_RUN_STATUS, &u32Y);	
	LogicRunInfApi(LOGIC_GET_BUTTON_VALUE, &u64ButtonVale);

#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
	if(u32Y == LOGIC_STATUS_RFID_SC_SELF_CONTROL || u32Y == LOGIC_STATUS_RFID_SC_NEIGHBORL)    //jhy
#endif	
	{
		u64ButtonVale >>= 62;
		u64ButtonVale &= 0x01;
		if(u64ButtonVale ==  (RFID_FRAME_ACTION))
		{
			if(u16ScID != IN_SC_SS_NUMB)	
				RfidTransformTx(&stSendFrame, u16ScID,5, 90,ACK);//100*10 = 1s,支架控制器方超时5s将自动停止
			else
				RfidTransformTx(&stSendFrame, SYSTEM_SS_ID,10, 90,ACK);//100*10 = 1s,支架控制器方超时5s将自动停止
		}
		else
		{
			if(u16ScID != IN_SC_SS_NUMB)	
				RfidTransformTx(&stSendFrame, u16ScID,RFID_KEYUP_SEND_TIMES_MAX, 40,NO_ACK);
			else
				RfidTransformTx(&stSendFrame, SYSTEM_SS_ID,RFID_KEYUP_SEND_TIMES_MAX, 40,NO_ACK);
		}
	}
	else
	{
		if(u16ScID != IN_SC_SS_NUMB)		
			RfidTransformTx(&stSendFrame, (u8)u16ScID,RFID_KEYUP_SEND_TIMES_MAX, 40,NO_ACK);
		else
			RfidTransformTx(&stSendFrame, SYSTEM_SS_ID,RFID_KEYUP_SEND_TIMES_MAX, 40,NO_ACK);
	}
	return 0x01;
}

/***************************************************************************************
** 函数名称: RfidMngWLTestSend
** 功能描述：无线测试发送数据
** 函数输入：无
** 函数输出：None
** 注意事项：
******************************************************************************************/
u8 RfidMngWLTestSend(void)
{
	stFRAME stSendFrame;
	u8 u8YkqID;
	u16 u16ScID;
	
	memset(&stSendFrame,0x00,sizeof(stFRAME));
	
	stSendFrame.u32ID.ID.RxID = (u8)(SC_DEV_TYPE&0xFF);
	stSendFrame.u32ID.ID.TxID = (u8)(WL_DEV_TYPE&0xFF);
	stSendFrame.u32ID.ID.FrameType = RFID_FT_WL_TEST_SEND;
	stSendFrame.u32ID.ID.Sum = 0;
	stSendFrame.u32ID.ID.Sub = 0;
	stSendFrame.u32ID.ID.ACK = NO_ACK;
	stSendFrame.u32ID.ID.Reservd = 0;
	stSendFrame.u32ID.ID.NoUsed = 0;

	stSendFrame.u8DLC = 0x08;
	
	LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB, &u16ScID);
	
	LogicParamApi(LOGIC_GET_YKQ_NUMBER, &u8YkqID);

	stSendFrame.u8DT[0x00] = u8YkqID;
	stSendFrame.u8DT[0x01] = u16ScID;
    memset(&stSendFrame.u8DT[0x02],0,6);
    
    RfidTransformTx(&stSendFrame, (u8)u16ScID,RFID_SEND_TIMES_LIMITLESS, 1000,NOACK);
//    RfidTestTimer = 300;
	return 0x01;
}

/***************************************************************************************
** 函数名称: RfidMngSendAutoPressEnabledUp
** 功能描述：发送自动补压开启数据
** 函数输入：无
** 函数输出：None
** 注意事项：
******************************************************************************************/
#if (KEY_AUTO_PRESS_PROC_FLAG != DISABLED)
u32 RfidMngSendAutoPressEnabledDisabledFlag(u32 u32FrameData)
{
	stFRAME stSendFrame;
	u8 u8YkqID;
	u16 u16ScID;
	
	memset(&stSendFrame,0x00,sizeof(stFRAME));
	
	stSendFrame.u32ID.ID.RxID = (u8)(SC_DEV_TYPE&0xFF);
	stSendFrame.u32ID.ID.TxID = (u8)(WL_DEV_TYPE&0xFF);
	if(u32FrameData == ENABLED)
		stSendFrame.u32ID.ID.FrameType = RFID_FT_AUTOPRESS_ENABLED;
	else if(u32FrameData == DISABLED)
		stSendFrame.u32ID.ID.FrameType = RFID_FT_AUTOPRESS_DISABLED;
	else
		return 0x00;
	
	stSendFrame.u32ID.ID.Sum = 0;
	stSendFrame.u32ID.ID.Sub = 0;
	stSendFrame.u32ID.ID.ACK = NO_ACK;
	stSendFrame.u32ID.ID.Reservd = 0;
	stSendFrame.u32ID.ID.NoUsed = 0;

	stSendFrame.u8DLC = 0x08;
	
	LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB, &u16ScID);
	
	LogicParamApi(LOGIC_GET_YKQ_NUMBER, &u8YkqID);
	
	stSendFrame.u8DT[0x00] = 0x00;
	stSendFrame.u8DT[0x01] = 0x00;
	stSendFrame.u8DT[0x02] = 0x00;	
	stSendFrame.u8DT[0x03] = 0x00;
	
	
	stSendFrame.u8DT[0x04] = (u8)(u16ScID&0xFF);
	stSendFrame.u8DT[0x05] = (u8)((u16ScID&0xFF00)>>8);
	stSendFrame.u8DT[0x06] = u8YkqID;

	stSendFrame.u8DT[0x07] = 0x00;
	
	if(u16ScID != IN_SC_SS_NUMB)	
		RfidTransformTx(&stSendFrame, u16ScID,RFID_KEYUP_SEND_TIMES_MAX, 90,ACK);//100*RFID_KEYUP_SEND_TIMES_MAX
	else
		RfidTransformTx(&stSendFrame, SYSTEM_SS_ID,RFID_KEYUP_SEND_TIMES_MAX, 90,ACK);//100*RFID_KEYUP_SEND_TIMES_MAX

	return 0x01;
}
#endif

/*********************************************************************************
** 函数名称: RfidGetDataLimitlessSendFlag
** 功能描述：获取RFID数据是否处于无限次发送的状态
** 函数输入：无
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32 RfidGetDataLimitlessSendFlag(void)
{
	u32 u32ReturnFlag = 0x00;
	u16 u16Read = s_u16RfidSendBufRead;
    u16 u16Write = s_u16RfidSendBufWrite;	
	
	while(u16Read != u16Write)
	{
		if(s_stRfidSendBuf[u16Read].u16SendTimes == RFID_SEND_TIMES_LIMITLESS)
		{
			u32ReturnFlag = 0x01;
			break;
		}
		u16Read++;
		u16Read %= RFID_SEND_BUF_SIZE;
	}
	return u32ReturnFlag;
}
/*********************************************************************************
** 函数名称: RfidRecvDataProc
** 功能描述：RFID数据处理
** 函数输入：无
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32 RfidSetDataLimitlessSendEnd(void)
{
	u32 u32ReturnFlag = 0x00;
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
		OS_CPU_SR  cpu_sr = 0;
	#endif
	OS_ENTER_CRITICAL(); 		
	s_u16RfidSendBufRead = s_u16RfidSendBufWrite;
	u32SendTimer = 0x00;
	OS_EXIT_CRITICAL();		
	return u32ReturnFlag;
}

/*********************************************************************************
** 函数名称: SendSetupToWl
** 功能描述：串口发送更改无线前导码函数
** 函数输入：type：33 55，isSend为是否为立即发送
** 函数输出：None
** 注意事项：
**********************************************************************************/
void SendSetupToWl(u8 type)
{	
	u8 data[3];
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
		OS_CPU_SR  cpu_sr = 0;
	#endif

	OS_ENTER_CRITICAL();	
	
	LogicParamApi(LOGIC_GET_YKQ_NUMBER, &data[0]);
	data[0] = 0xFF-data[0]; //遥控器编号	
	if(type == RFID_ININT_SYNC1)
	{
		data[1] = RFID_ININT_SYNC0;//0x33
		data[2] = RFID_ININT_SYNC1;//0x33
	}
	if(type == RFID_PRG_ININT_SYNC1)
	{
		data[1] = RFID_PRG_ININT_SYNC0;//0x33
		data[2] = RFID_PRG_ININT_SYNC1;//0x55
	}
	OS_EXIT_CRITICAL();	
	InsUsartTrsQueue(data,3,PARAMETERSETTING,0,1);
	InsUsartTrsQueue(data,3,PARAMETERSETTING,0,1);
	InsUsartTrsQueue(data,3,PARAMETERSETTING,0,1);

}
/*********************************************************************************
** 函数名称: HeartBeatCommErrTimer
** 功能描述：无线模块心跳相关处理
** 函数输入：无
** 函数输出：None
** 注意事项：
**********************************************************************************/
void HeartBeatCommErrTimer(void)
{
	u32 u32Temp;
	LogicRunInfApi(LOGIC_GET_RUN_STATUS,&u32Temp);
	if(u32Temp != LOGIC_STATUS_SHUTUP)//不是关机状态	
	{		
		if((WlHeartTime != TIMER_EXPIRED) && (WlHeartTime != TIMER_CLOSED))
		{
			WlHeartTime -= 10;

			if(WLUnconnectedFlag == 1)//中断后又连接上   jhy
			{
				WLUnconnectedFlag = 0;
			}		
		}
		//心跳计时器超时
		if(WlHeartTime == TIMER_EXPIRED)
		{		
			WLUnconnectedFlag = 1;
			
			for(u32Temp = 0;u32Temp<6;u32Temp++)
			{
				u8WlVersion[u32Temp] = '*';
			}
			LogicParamApi(LOGIC_SET_WL_VERSION,&u8WlVersion);
			//有长时间收不到心跳的情况，导致超时推出连接，后续找原因。		zzw
			//清除心跳
			if(RfidGetDataLimitlessSendFlag())
			{
				RfidSetDataLimitlessSendEnd();
			}				
//			KeyLedOnOff(0x00,KEY_LED_MAX);	//关闭按键灯
//			StateLed(0x00,ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED|KEY_SHIFT_LED);	
			//关闭信号指示
			u32Temp = RFID_SIGNAL_TOO_SMALL;
			LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u32Temp);
			//设置无线断开状态
//			u32Temp = LOGIC_STATUS_UNCONNECT_WL;
//			LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Temp);
			InterfaceInsertNote(WL_UNCONNECTED);
//			InterfaceSetCtrlMode(WL_UNCONNECTED);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
			OSSemPost(g_stSemInterface);	
			#if (HEART == 1)		//心跳帧			
			{			
				u64 u64TempValue;
				u64TempValue = 0x00;
				Heart.ActionType = SIGNAL_ACTION;
				Heart.SendMode = 0;
				Heart.ActionValue = 0;
				LogicRunInfApi(LOGIC_SET_HEART, &Heart);
			}	
			#endif	
		}	
		if(WlHeartTime == TIMER_EXPIRED)
		{
			WlHeartTime = TIMER_CLOSED;	
		}	
	}
}
/*********************************************************************************
** 函数名称: SetHeartBeat
** 功能描述：心跳数据处理
** 函数输入：无
** 函数输出：None
** 注意事项：
**********************************************************************************/
void SetHeartBeat(stUSARTDATA pFrame)
{
	LogicParamApi(LOGIC_GET_WL_VERSION,&u8WlVersion);
	if( u8WlVersion[1] != pFrame.SendBuf[5]+0x30 \
	||  u8WlVersion[3] != pFrame.SendBuf[6]+0x30 \
	||  u8WlVersion[5] != pFrame.SendBuf[7]+0x30)
	{
		u8WlVersion[0] = pFrame.SendBuf[4];
		u8WlVersion[1] = pFrame.SendBuf[5]+0x30;
		u8WlVersion[2] = '.';
		u8WlVersion[3] = pFrame.SendBuf[6]+0x30;
		u8WlVersion[4] = '.';
		u8WlVersion[5] = pFrame.SendBuf[7]+0x30;
		LogicParamApi(LOGIC_SET_WL_VERSION,&u8WlVersion);
	}
}
s8 s8RssiSignalInf;     //遥控器无线信号强度
s8 s8RssiSignalInftest;     //遥控器无线信号强度
s8 s8ScRssiSignal;      //控制器信号强度
static s8 s8oldRssiSignalInf;     //遥控器无线信号强度
static s8 s8oldScRssiSignal;      //控制器信号强度
/*********************************************************************************
** 函数名称: RfidRecvDataProc
** 功能描述：RFID数据处理
** 函数输入：无
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32 RfidRecvDataProc(void)
{
	st_RFIDRcvFrame RfidRcvFrm;
	stFRAME	stRxFrame;
	u8 u8YkqID;
	u8 u8I = 0x00;
	u32 u32ReturnFlag = 0x00;
	static u64 u64ReturnFlag;
	u16 u16DestScNumb;
	u16 u16DestScNumbBackup;	
	u32 u32RData;
	u16 u16I;
	COMMAND_ACTION ActionTemp;
	u64ReturnFlag = 0x00;
	#if RFID_TEST
        memcpy(&stRxFrame,&temp,sizeof(stRxFrame));
	#else

		RFID_FetchData(&RfidRcvFrm);
	
		stRxFrame.u32ID.u32Id = RfidRcvFrm.u8Data[0x00];
		stRxFrame.u32ID.u32Id |= (u32)(RfidRcvFrm.u8Data[0x01]<<8);
		stRxFrame.u32ID.u32Id |= (u32)(RfidRcvFrm.u8Data[0x02]<<16);
		stRxFrame.u32ID.u32Id |= (u32)(RfidRcvFrm.u8Data[0x03]<<24);	
		stRxFrame.u8DLC = RfidRcvFrm.u8Data[0x04];	
		s8RssiSignalInf = RfidRcvFrm.u8AppendStatus[0x00];
		//s8IqiSignalInf = RfidRcvFrm.u8AppendStatus[0x01];
		if(s8RssiSignalInf >= 128)
			s8RssiSignalInf = (s8RssiSignalInf-256)/2-RSSI_OFFSET_VALUE;
		else
			s8RssiSignalInf = (s8RssiSignalInf)/2-RSSI_OFFSET_VALUE;

		if(s8RssiSignalInf >= -30)
			u16DestScNumb = RFID_SIGNAL_NEGATIVE_30;
		else if(s8RssiSignalInf >= -40)
			u16DestScNumb = RFID_SIGNAL_NEGATIVE_40;
		else if(s8RssiSignalInf >= -50)
			u16DestScNumb = RFID_SIGNAL_NEGATIVE_50;
		else if(s8RssiSignalInf >= -60)
			u16DestScNumb = RFID_SIGNAL_NEGATIVE_60;
		else if(s8RssiSignalInf >= -80)
			u16DestScNumb = RFID_SIGNAL_NEGATIVE_80;
		else
			u16DestScNumb = RFID_SIGNAL_TOO_SMALL;
		LogicRunInfApi(LOGIC_GET_RFID_SIGNAL_STATUS, &u16DestScNumbBackup);
		if(u16DestScNumb != u16DestScNumbBackup)
		{
			LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DestScNumb);
			u16I = (u16)(InterfaceGetCtrlMode());
			if((u16I != INTERFACE_MODE_SELF_SET)&&(u16I != INTERFACE_MODE_SHUTSOWN)&&(u16I != INTERFACE_MODE_ININT))
			{
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_RFIDSIGNAL);
				OSSemPost(g_stSemInterface);				
			}
		}
	        if(RfidRcvFrm.u8DataLen > 13)
	            return 0x00;   
	        for(u8I = 0x00; u8I < RfidRcvFrm.u8DataLen; u8I++)
	        {
	            stRxFrame.u8DT[u8I] = RfidRcvFrm.u8Data[0x05+u8I];
	        }
	#endif
	
	if(stRxFrame.u32ID.ID.RxID != (WL_DEV_TYPE&0x000000FF))
		return u32ReturnFlag ;
	if((stRxFrame.u32ID.ID.TxID != (SC_DEV_TYPE&0x000000FF))&&(stRxFrame.u32ID.ID.TxID != (SS_DEV_TYPE&0x000000FF)))
		return u32ReturnFlag ;

	LogicParamApi(LOGIC_GET_YKQ_NUMBER, &u8YkqID);
	LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB, &u16DestScNumbBackup);	
	switch(stRxFrame.u32ID.ID.FrameType)
	{
		case RFID_FT_COLIBARATE_ACK:
		{
			if(stRxFrame.u8DT[0x05] != u8YkqID)
				break;
			if(stRxFrame.u8DT[0x04] != u16DestScNumbBackup)
				break;
			
 			if(u16ScLsNumbBackup == stRxFrame.u32ID.ID.LiuShuiNumb)
				return 0x00;
			
			u16ScLsNumbBackup = stRxFrame.u32ID.ID.LiuShuiNumb;	
			
			if(((stRxFrame.u8DT[0x02]&0x80)>>7) != YKQ_SIMPLE_FUNCTION)
					break;
			
			#if 0//为保证与原控制器之间的通用性，暂不对对码类型进行判断
				u16I = 0x00;
				LogicRunInfApi(LOGIC_GET_COLIBRATE_SC_TYPE,&u16I);
				if(u16I == LOGIC_COLIBRATE_SC_INCREASE_DECREASE)//2017.01.12,依据最新协议修改
				{
					if((stRxFrame.u8DT[0x02]&0x40) != ((LOGIC_COLIBRATE_SC_INCREASE_DECREASE-0x01)<<6))
						break;
				}
				else
				{
					if((stRxFrame.u8DT[0x02]&0x40) != ((LOGIC_COLIBRATE_SC_SHIFT_BUTTON-0x01)<<6))
						break;			
				}
			#endif
			
			if((stRxFrame.u8DT[0x02]&0x01) == 0x01)//对码成功
			{
				u8I = stRxFrame.u8DT[0x01];
				LogicRunInfApi(LOGIC_GET_SC_NUMB_ADD_DIRCT, &u8YkqID);
				if(u8I != u8YkqID)
				{
					LogicRunInfApi(LOGIC_SET_SC_NUMB_ADD_DIRCT, &u8I);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_SCCOLIBRATE);//更新对码显示
					OSSemPost(g_stSemInterface);					
				}
				u16DestScNumb = (u16)(stRxFrame.u8DT[0x07]<<8);
				u16DestScNumb |= stRxFrame.u8DT[0x06];
				u16DestScNumb *= 100;//单位0.1s
				LogicParamApi(LOGIC_SET_SC_CTRL_DISPALY_TIMER, &u16DestScNumb);

				LogicRunInfApi(LOGIC_GET_RUN_STATUS, &u8I);
				if(u8I == LOGIC_STATUS_RFID_COLIBARAE_ING)
				{
					u8I = LOGIC_STATUS_RFID_COLIBARAE_SUCCESS;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u8I);
					InterfaceSetCtrlMode(INTERFACE_MODE_COLIBRATE_CODE_SUCCESS);
				}
				if((stRxFrame.u8DT[0x03]) == 0x01)//配置文件crc一致	zzw
				{
					u8I = 1;
					LogicRunInfApi(LOGIG_SET_ACTIONINFO_CRC_IDENCAL,&u8I);
				}
				else
				{
					u8I = 0;
					LogicRunInfApi(LOGIG_SET_ACTIONINFO_CRC_IDENCAL,&u8I);			
				}
			}
			else
			{
				LogicRunInfApi(LOGIC_GET_RUN_STATUS, &u8I);
				if(u8I == LOGIC_STATUS_RFID_COLIBARAE_ING)
				{			
					u8I = LOGIC_STATUS_RFID_COLIBARAE_FAIL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u8I);
					InterfaceSetCtrlMode(INTERFACE_MODE_RELIEVE_COLIBRATE);
				}
			}
			OSSemPost(g_stLogicSem);
			u32ReturnFlag = 0x01;
		}break;

		case RFID_FT_KEY_DOWN_ACK:
//			if(stRxFrame.u8DT[0x06] != u8YkqID)
//				break;
//			u16DestScNumb = stRxFrame.u8DT[0x04];
//			u16DestScNumb |= (u16)(stRxFrame.u8DT[0x05]<<8);
//			if(u16DestScNumb != u16DestScNumbBackup)
//				break;	
//			
//			if(u16ScLsNumbBackup == stRxFrame.u32ID.ID.LiuShuiNumb)
//				return 0x00;
			
			u16ScLsNumbBackup = stRxFrame.u32ID.ID.LiuShuiNumb;	

			ActionTemp.ActionType.type = stRxFrame.u8DT[0x02];
			ActionTemp.ActionID = stRxFrame.u8DT[0x03];
			ActionTemp.OrderActionOrResult = stRxFrame.u8DT[0x04];
			LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE, &ActionTemp);
			
			if((stRxFrame.u8DT[0x00] != u8YkqID)|| (stRxFrame.u8DT[0x01] != u16DestScNumbBackup))
				break;
			
			if((stRxFrame.u8DT[0x04]&0x01) == 0x01)//按键接受
			{
				u16DestScNumb = SC_BUTTON_ACK_ACCEPT;
			}
			else
			{
				u16DestScNumb = SC_BUTTON_ACK_REJECT;	
			}

			LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS, &u16DestScNumb);
			LogicRunInfApi(LOGIC_GET_RUN_STATUS, &u16DestScNumb);
//			/**判断邻隔架操作时，是否为左右邻架键**/
//			if(u16DestScNumb == LOGIC_STATUS_RFID_SC_NEIGHBORL)
//			{
//				u8I = (u8)(u32ReturnFlag>>30);
//				if(u8I == RFID_FRAME_STATUS)
//				{
//					u32ReturnFlag &= 0x3FFFFFFF;
//					if((u32ReturnFlag == RFID_STATUS_NEIGHBOR_LEFT) ||(u32ReturnFlag == RFID_STATUS_NEIGHBOR_RIGHT))
//					{
//						u32ReturnFlag = (u32)(stRxFrame.u8DT[0x07]>>6);
//						if((u32ReturnFlag == SC_ACTION_SELF_CTRL) || (u32ReturnFlag == SC_ACTION_LEFT_CTRL)  || (u32ReturnFlag == SC_ACTION_RIGHT_CTRL) )
//						{
//							LogicRunInfApi(LOGIC_SET_SC_ACTION_TYPE,&u32ReturnFlag);
//							u32ReturnFlag = (u32)((stRxFrame.u8DT[0x07]&0x0E)>>1);
//							LogicRunInfApi(LOGIC_SET_SC_GEJIA_NUMBER,&u32ReturnFlag);	
//						}
//					}
//				}
//			}
			
			if(u16DestScNumb != LOGIC_STATUS_RFID_SC_GRP)//非成组操作时，可接收到按键回应即发送信号量
				OSSemPost(g_stLogicSem);//成组操作时，接收到成组信息才发送信号量
			else if((u16DestScNumb == LOGIC_STATUS_RFID_SC_GRP)&&((u32ReturnFlag&RFID_STATUS_NEIGHBOR_LEFT) == RFID_STATUS_NEIGHBOR_LEFT))
				OSSemPost(g_stLogicSem);//成组操作时，接收到成组信息才发送信号量
			else if((u16DestScNumb == LOGIC_STATUS_RFID_SC_GRP)&&((u32ReturnFlag&RFID_STATUS_NEIGHBOR_RIGHT) == RFID_STATUS_NEIGHBOR_RIGHT))
				OSSemPost(g_stLogicSem);//成组操作时，接收到成组信息才发送信号量
			else if((u16DestScNumb == LOGIC_STATUS_RFID_SC_GRP)&&((u32ReturnFlag&RFID_STATUS_START) == RFID_STATUS_START))
				OSSemPost(g_stLogicSem);//成组操作时，接收到成组信息才发送信号量
			else if((u16DestScNumb == LOGIC_STATUS_RFID_SC_GRP)&&((u32ReturnFlag&RFID_STATUS_STOP) == RFID_STATUS_STOP))
				OSSemPost(g_stLogicSem);//成组操作时，接收到成组信息才发送信号量
			else
			{
				#if RFID_TEST
				u32 u32Temp;
				temp.u32ID.ID.RxID = (u8)(WL_DEV_TYPE&0xFF);
				temp.u32ID.ID.TxID = (u8)(SC_DEV_TYPE&0xFF);
				temp.u32ID.ID.FrameType = RFID_FT_GRP_ACTION_INF;
				temp.u32ID.ID.Sum = 0;
				temp.u32ID.ID.Sub = 0;
				temp.u32ID.ID.ACK = NO_ACK;
				temp.u32ID.ID.Reservd = 0;
				temp.u32ID.ID.NoUsed = 0;
				temp.u8DLC = 0x08;
				LogicRunInfApi(LOGIC_GET_BUTTON_VALUE, &u32ReturnFlag);
				LogicRunInfApi(LOGIC_GET_RFID_FRAME_TYPE, &u8I);
				if((u32ReturnFlag == (RFID_STATUS_GRP_LEFT+0x80000000)) || (u32ReturnFlag == (RFID_STATUS_GRP_RIGHT+0x80000000)))
				{
					u32ReturnFlag = RFID_ACTION_YIJIA;
					u8I = RFID_FRAME_ACTION;
				}
				u32ReturnFlag = RfidGrpAckGetScAckCode((RFID_GRP_ACTION_TYPE *)&u32Temp, (RFID_FRAME_TYPE)u8I,(RFID_DATA_TYPE)u32ReturnFlag);
				if(u32ReturnFlag)
				{
					temp.u8DT[0x00] = 0x04;
					temp.u8DT[0x01] = (0x08<<1);
					temp.u8DT[0x02] = (0x01<<2);
					temp.u8DT[0x02] |= (u8)(u32Temp<<3);
					temp.u8DT[0x03] = 0x00;
					
					temp.u8DT[0x04] = (u16DestScNumbBackup&0xFF);
					temp.u8DT[0x05] = (u8)((u16DestScNumbBackup&0xFF00)>>8);
					temp.u8DT[0x06] = u8YkqID;
					temp.u8DT[0x07] = 0x01;	

					#if RFID_TEST
						OSSemPost(s_stRfidRecvd);
					#endif						
				}
				
			#endif
			}
            u32ReturnFlag = 0x01;            
			break;

		case RFID_FT_GRP_ACTION_INF:
			if(stRxFrame.u8DT[0x06] != u8YkqID)
				break;
			u16DestScNumb = stRxFrame.u8DT[0x04];
			u16DestScNumb |= (u16)(stRxFrame.u8DT[0x05]<<8);
			if(u16DestScNumb != u16DestScNumbBackup)
				break;	
			
			if(u16ScLsNumbBackup == stRxFrame.u32ID.ID.LiuShuiNumb)
				return 0x00;
			
			u16ScLsNumbBackup = stRxFrame.u32ID.ID.LiuShuiNumb;	
			
			/*获取信息**/
			u16DestScNumb = stRxFrame.u8DT[0x00];
			u16DestScNumb |= (u16 )(stRxFrame.u8DT[0x01]<<8);//
			u16DestScNumb &= 0x1FF;
			LogicRunInfApi(LOGIC_SET_GRP_NEAR_SC_NUMB, &u16DestScNumb);
			u16DestScNumb = stRxFrame.u8DT[0x01];
			u16DestScNumb |= (u16 )(stRxFrame.u8DT[0x02]<<8);
			u16DestScNumb >>= 1;
			u16DestScNumb &= 0x1FF;
			LogicRunInfApi(LOGIC_SET_GRP_REMOTE_SC_NUMB, &u16DestScNumb);
			u16DestScNumb = 0x00;
			u16DestScNumb = (stRxFrame.u8DT[0x02]&0x04);
			u16DestScNumb >>= 2;
			LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION, &u16DestScNumb);
			u16DestScNumb = (stRxFrame.u8DT[0x02]&0xF8);
			u16DestScNumb >>= 3;
			LogicRunInfApi(LOGIC_GET_RFID_FRAME_TYPE, &u8I);			
			LogicRunInfApi(LOGIC_SET_GRP_ACK_ACTION_NAME, &u16DestScNumb);
			if(RfidGrpAckGetRfidAction((RFID_GRP_ACTION_TYPE)u16DestScNumb,(RFID_FRAME_TYPE)u8I,&u32RData))
				LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u32RData);			
			OSSemPost(g_stLogicSem);
            u32ReturnFlag = 0x01;
			break;	

		case RFID_FT_RESET_PARAM:
			if(stRxFrame.u8DT[0x05] != u8YkqID)
				break;
			u16DestScNumb = stRxFrame.u8DT[0x02];//原对码架号
			u16DestScNumb |= (u16)((stRxFrame.u8DT[0x03]&0x01)<<8);	
			if(u16DestScNumb != u16DestScNumbBackup)
			{
				break;				
			}

			if(u16ScLsNumbBackup == stRxFrame.u32ID.ID.LiuShuiNumb)
				return 0x00;
			
			u16ScLsNumbBackup = stRxFrame.u32ID.ID.LiuShuiNumb;	
			
			u16DestScNumb = (u16)stRxFrame.u8DT[0x03];//对码架号
			u16DestScNumb >>= 1;//对码架号
			u16DestScNumb |= (u16)((stRxFrame.u8DT[0x04]&0x03)<<7);			
			if(u16DestScNumb != u16DestScNumbBackup)
			{
				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB, &u16DestScNumb);		
				LogicRunInfApi(LOGIC_SET_CONLIBRATE_SUCCESS_SC_NUMB,&u16DestScNumb);				
			}
			u16DestScNumb = stRxFrame.u8DT[0x06];//主控时间
			u16DestScNumb |= (u16)(stRxFrame.u8DT[0x07]<<8);
			u16DestScNumb *= 100;//单位0.1s
			LogicParamApi(LOGIC_SET_SC_CTRL_DISPALY_TIMER, &u16DestScNumb);	
			
			u16DestScNumb = stRxFrame.u8DT[0x01];//架号增向
			LogicRunInfApi(LOGIC_GET_SC_NUMB_ADD_DIRCT, &u16DestScNumbBackup);	
			if(u16DestScNumb != u16DestScNumbBackup)
				LogicRunInfApi(LOGIC_SET_SC_NUMB_ADD_DIRCT, &u16DestScNumb);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//更新对码显示
			OSSemPost(g_stSemInterface);			
			break;
			
		case RFID_FT_KEY_UP_ACK:
			if(stRxFrame.u8DT[0x06] != u8YkqID)
				break;
			u16DestScNumb = stRxFrame.u8DT[0x04];
			u16DestScNumb |= (u16)(stRxFrame.u8DT[0x05]<<8);
			if(u16DestScNumb != u16DestScNumbBackup)
				break;	

			if(u16ScLsNumbBackup == stRxFrame.u32ID.ID.LiuShuiNumb)
				return 0x00;
			
			u16ScLsNumbBackup = stRxFrame.u32ID.ID.LiuShuiNumb;	
#if 0			
			if((stRxFrame.u8DT[0x03] >> 6) != 0x01)	//对非动作字，处理方式不变；对动作字，须按新协议进行转换	30个动作增加至56个动作 2019.12.24 parry
			{
				u32ReturnFlag = stRxFrame.u8DT[0x00];
				u32ReturnFlag |= (u32)(stRxFrame.u8DT[0x01]<<8);
				u32ReturnFlag |= (u32)(stRxFrame.u8DT[0x02]<<16);
				u32ReturnFlag |= (u32)(stRxFrame.u8DT[0x03]<<24);			
				u64ReturnFlag = (u64)u32ReturnFlag;
				u64ReturnFlag = (((u64ReturnFlag & 0xC0000000) << 32) | (u64ReturnFlag & 0x3FFFFFFF));
			}
			else
			{
				u64ReturnFlag = ProtocolTransRx(&stRxFrame);
			}
#endif			
			ActionTemp.ActionType.type = stRxFrame.u8DT[0x00];
			ActionTemp.ActionID = stRxFrame.u8DT[0x01];
			ActionTemp.OrderActionOrResult = stRxFrame.u8DT[0x02];
			LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE, &ActionTemp);
			LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE, &u64ReturnFlag);
			
			if(stRxFrame.u8DT[0x02] == 0x01)//按键接受
			{
				u16DestScNumb = SC_BUTTON_ACK_ACCEPT;
			}
			else//按键拒绝
			{
				u16DestScNumb = SC_BUTTON_ACK_REJECT;		
			}	
			LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS, &u16DestScNumb);
			OSSemPost(g_stLogicSem);//更新界面显示
            u32ReturnFlag = 0x01;            			
			break;

		case RFID_FT_SC_UNCOLIBATARE://控制器直接要求解除对码
			if(stRxFrame.u8DLC != 0x08)
				break;
			if(stRxFrame.u8DT[0x05] != u8YkqID)//遥控器ID
				break;		
			if(u16DestScNumbBackup != (stRxFrame.u8DT[0x03]|(stRxFrame.u8DT[0x04]<<8)))//对码架号
				break;

			if(u16ScLsNumbBackup == stRxFrame.u32ID.ID.LiuShuiNumb)
				return 0x00;
			
			u16ScLsNumbBackup = stRxFrame.u32ID.ID.LiuShuiNumb;	
			u32RData = 0x00;
			LogicRunInfApi(LOGIC_GET_COLIBRATE_SC_TYPE,&u32RData);
			if(u32RData == LOGIC_COLIBRATE_SC_INCREASE_DECREASE )
			{
				LogicRunInfApi(LOGIC_GET_RUN_STATUS, &u8I);
				if(u8I == LOGIC_STATUS_RFID_COLIBARAE_ING)
				{
					u8I = LOGIC_STATUS_RFID_COLIBARAE_FAIL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u8I);
					InterfaceSetCtrlMode(INTERFACE_MODE_RELIEVE_COLIBRATE);
	          		OSSemPost(g_stLogicSem);
				}
				u32ReturnFlag = 0x01;  
				break;
			}
			#if (QUICK_KEY_CTRL_ENABLE_FLAG	!= 0)
				LogicRunInfApi(LOGIC_GET_SC_QUICK_TYPE, &u32Y);
				if((u32Y == SC_ACTION_QUICK_1)|| (u32Y == SC_ACTION_QUICK_2))
				{
					u32Y = SC_ACTION_QUICK_STOP;
					u32ReturnFlag = LogicRunInfApi(LOGIC_SET_SC_QUICK_TYPE, &u32Y);
				}
				else
			#endif	
				{
					u32ReturnFlag = LOGIC_STATUS_IDLE;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32ReturnFlag);	
				}		
			
			u16DestScNumb = RFID_SIGNAL_TOO_SMALL;
			LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DestScNumb);				
			InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_PASSIVE_UNCOLIBRATE);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_SC_UNCOLIBRATE);	
			OSSemPost(g_stSemInterface);
			/**设置RFID为休眠状态***/
			if(g_u32ID != B_type)
				;
			else
			{
				u8I = GetRfidCurStatus();//获取芯片状态
				if(u8I == CC1101_STATE_RX_OVERFLOW)
					RfidStrobe(CC1101_SFRX);    //冲洗 RX FIFO buffer.
				else if(u8I == CC1101_STATE_TX_UNDERFLOW)
					RfidStrobe(CC1101_SFTX);    //冲洗 TX FIFO buffer.
				else if(u8I == CC1101_STATE_IDLE)
					SetRfidSRX();
				else
					SetRFChipSleepMode();//让芯片进入休眠模式;
				u16DestScNumb = 0x00;
				while((u8I == CC1101_STATE_RX_OVERFLOW) || (u8I == CC1101_STATE_TX_UNDERFLOW) || (u8I == CC1101_STATE_IDLE))
				{						
					OSTimeDly(RFIDMNG_TASK_TIME_SET/TICK_TIME);
					u16DestScNumb ++;
					u8I = GetRfidCurStatus();//获取芯片状态
					if((u8I != CC1101_STATE_RX_OVERFLOW) && (u8I != CC1101_STATE_TX_UNDERFLOW) &&(u8I != CC1101_STATE_IDLE))
					{
						SetRFChipSleepMode();//让芯片进入休眠模式;
						break;
					}
					if(u16DestScNumb >= 10)
					{
						SetRFChipSleepMode();//让芯片进入休眠模式;
						break;
					}
				}
			}
			break;

		#if (KEY_AUTO_PRESS_PROC_FLAG != DISABLED)
			case RFID_FT_AUTOPRESS_ENABLED_ACK://自动补压开启应答
			case RFID_FT_AUTOPRESS_DISABLED_ACK://自动补压开启应答	
				if(stRxFrame.u8DT[0x06] != u8YkqID)
					break;
				u16DestScNumb = stRxFrame.u8DT[0x04];
				u16DestScNumb |= (u16)(stRxFrame.u8DT[0x05]<<8);
				if(u16DestScNumb != u16DestScNumbBackup)
					break;
				
				if(u16ScLsNumbBackup == stRxFrame.u32ID.ID.LiuShuiNumb)
					return 0x00;
				
				u16ScLsNumbBackup = stRxFrame.u32ID.ID.LiuShuiNumb;	
				
				if((stRxFrame.u8DT[0x07]&0x01) == 0x01)//自动补压设置应答接受
				{
					u32RData = 0x01;
					LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE, &u32RData);
				}
				else if((stRxFrame.u8DT[0x07]&0x01) == 0x00)
				{
					u32RData = 0x00;
					LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE, &u32RData);
				}
				else
					;
	            OSSemPost(g_stLogicSem);
	            u32ReturnFlag = 0x01;
				break;
		#endif
		case RFID_FT_NEAR_CTRL_CONFIRM:		//邻架控制应答
		{
			LogicParamApi(LOGIC_GET_YKQ_NUMBER, &u8YkqID);
			if(stRxFrame.u8DT[0x00] != u8YkqID)
				break;	
			LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&stRxFrame.u8DT[0x02]);
			OSSemPost(g_stLogicSem);
		}break;
		default:
			break;		
	}
	
	return u32ReturnFlag ;
}

/*********************************************************************************
** 函数名称: RfidRecvDataProc
** 功能描述：RFID数据处理
** 函数输入：无
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32 RfidRecvWaitTimeoutProc(void)
{
	u16 u16I = 0x00;

	/*解除对码状态的超时处理**/
	if(s_stRfidSendBuf[s_u16RfidSendBufRead].stFrame.u32ID.ID.FrameType == RFID_FT_COLIBARATE)
	{
		//按下加减键切换对码架时，为了能快速响应再次按下加减键
		if(getColiScNumStatus() > FIRSTDOWN)
		{		
			InterfaceSetCtrlMode(INTERFACE_MODE_SP);
			OSSemPost(g_stLogicSem);
		}
		else if(s_stRfidSendBuf[s_u16RfidSendBufRead].u16SendTimes == 0x00)
		{	
			LogicRunInfApi(LOGIC_GET_RUN_STATUS, &u16I);
			if(u16I == LOGIC_STATUS_RFID_COLIBARAE_ING)
			{
				u16I = LOGIC_STATUS_IR_COLIBARAE_FAIL;
				LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u16I);
				InterfaceSetCtrlMode(INTERFACE_MODE_COLIBRATE_CODE_FAIL);
				OSSemPost(g_stLogicSem);
			}
		}
	}
	/*按键第一次按下的超时处理*/
	if(s_stRfidSendBuf[s_u16RfidSendBufRead].stFrame.u32ID.ID.FrameType == RFID_FT_KEY_DOWN)
	{	
		if(s_stRfidSendBuf[s_u16RfidSendBufRead].u16SendTimes == 0x00)
		{
			u16I = SC_BUTTON_ACK_TIMEOUT;
			LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS, &u16I);			
			OSSemPost(g_stLogicSem);
			u16I = RFID_SIGNAL_TOO_SMALL;
			LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16I);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_RFIDSIGNAL);
			OSSemPost(g_stSemInterface);	
		}
	}	
	return 0x00;
}

/*********************************************************************************
** 函数名称: RfidRecvInvalidDataProc
** 功能描述：RFID数据处理数据无效时
** 函数输入：无
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32 RfidRecvInvalidDataProc(void)
{
	st_RFIDRcvFrame RfidRcvFrm;
	stFRAME	stRxFrame;
	u8 u8YkqID;
	u8 u8I = 0x00;
	u32 u32ReturnFlag = 0x00;
	u16 u16DestScNumb;
	u16 u16DestScNumbBackup;	
//	s8 s8RssiSignalInf;
	u32 u32RData;
	#if	(QUICK_KEY_CTRL_ENABLE_FLAG != 0)
		u32 u32Y;
	#endif
	u16 u16I;
	
	#if RFID_TEST
       	memcpy(&stRxFrame,&temp,sizeof(stRxFrame));
	#else

		RFID_FetchData(&RfidRcvFrm);

		stRxFrame.u32ID.u32Id = RfidRcvFrm.u8Data[0x00];
		stRxFrame.u32ID.u32Id |= (u32)(RfidRcvFrm.u8Data[0x01]<<8);
		stRxFrame.u32ID.u32Id |= (u32)(RfidRcvFrm.u8Data[0x02]<<16);
		stRxFrame.u32ID.u32Id |= (u32)(RfidRcvFrm.u8Data[0x03]<<24);	
		stRxFrame.u8DLC = RfidRcvFrm.u8Data[0x04];	
		s8RssiSignalInf = RfidRcvFrm.u8AppendStatus[0x00];
		//s8IqiSignalInf = RfidRcvFrm.u8AppendStatus[0x01];
		if(s8RssiSignalInf >= 128)
			s8RssiSignalInf = (s8RssiSignalInf-256)/2-RSSI_OFFSET_VALUE;
		else
			s8RssiSignalInf = (s8RssiSignalInf)/2-RSSI_OFFSET_VALUE;

		if(s8RssiSignalInf >= -30)
			u16DestScNumb = RFID_SIGNAL_NEGATIVE_30;
		else if(s8RssiSignalInf >= -40)
			u16DestScNumb = RFID_SIGNAL_NEGATIVE_40;
		else if(s8RssiSignalInf >= -50)
			u16DestScNumb = RFID_SIGNAL_NEGATIVE_50;
		else if(s8RssiSignalInf >= -60)
			u16DestScNumb = RFID_SIGNAL_NEGATIVE_60;
		else if(s8RssiSignalInf >= -80)
			u16DestScNumb = RFID_SIGNAL_NEGATIVE_80;
		else
			u16DestScNumb = RFID_SIGNAL_TOO_SMALL;
		LogicRunInfApi(LOGIC_GET_RFID_SIGNAL_STATUS, &u16DestScNumbBackup);	

		if(u16DestScNumb != u16DestScNumbBackup)
		{
			
			u16I = (u16)(InterfaceGetCtrlMode());
			if((u16I != INTERFACE_MODE_SELF_SET)&&(u16I != INTERFACE_MODE_SHUTSOWN)&&(u16I != INTERFACE_MODE_ININT))
			{
				InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_RFIDSIGNAL);
				OSSemPost(g_stSemInterface);				
			}				
		}
		
		if(RfidRcvFrm.u8DataLen > 13)
			return 0x00;   
		for(u8I = 0x00; u8I < 0x08; u8I++)
		{
			stRxFrame.u8DT[u8I] = RfidRcvFrm.u8Data[0x05+u8I];
		}
	#endif

	#ifndef HARDWARE_TEST
	if(u16ScLsNumbBackup == stRxFrame.u32ID.ID.LiuShuiNumb)
		return 0x00;
	
	u16ScLsNumbBackup = stRxFrame.u32ID.ID.LiuShuiNumb;

	if(stRxFrame.u32ID.ID.RxID != (WL_DEV_TYPE&0x000000FF))
		return u32ReturnFlag ;
	if((stRxFrame.u32ID.ID.TxID != (SC_DEV_TYPE&0x000000FF))&&(stRxFrame.u32ID.ID.TxID != (SS_DEV_TYPE&0x000000FF)))
		return u32ReturnFlag ;
		
	#endif

	LogicParamApi(LOGIC_GET_YKQ_NUMBER, &u8YkqID);
	LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB, &u16DestScNumbBackup);	

	switch(stRxFrame.u32ID.ID.FrameType)
	{
		
		#if HARDWARE_TEST//收到对码帧，进行应答
			case RFID_FT_COLIBARATE:			
				stRxFrame.u32ID.ID.ACK = 0;
				stRxFrame.u32ID.ID.FrameType = RFID_FT_COLIBARATE_ACK;
				stRxFrame.u32ID.ID.TxID = (u8)(SC_DEV_TYPE&0xFF);
				stRxFrame.u32ID.ID.RxID = (u8)(WL_DEV_TYPE&0xFF);
				stRxFrame.u8DLC = 0x08;
				stRxFrame.u8DT[0x00] = 0x00;
				stRxFrame.u8DT[0x01] = 0x00;
				stRxFrame.u8DT[0x02] = 0x01;
				stRxFrame.u8DT[0x03] = 0x00;
				stRxFrame.u8DT[0x04] = 0x00;
				stRxFrame.u8DT[0x05] = 0x00;
				stRxFrame.u8DT[0x06] = 0x00;
				stRxFrame.u8DT[0x07] = 0x00;
				u16DestScNumb = 0x00;
				RfidTransformTx(&stRxFrame, u16DestScNumb,0x01, 40,NO_ACK);

				u8I = LOGIC_STATUS_RFID_COLIBARAE_SUCCESS;
				LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u8I);
				InterfaceSetCtrlMode(INTERFACE_MODE_COLIBRATE_CODE_SUCCESS);				
				OSSemPost(g_stLogicSem);
				u32ReturnFlag = 0x01;
				break;
			#endif
		case RFID_FT_RESET_PARAM:
			if(stRxFrame.u8DT[0x05] != u8YkqID)
				break;
			u16DestScNumb = stRxFrame.u8DT[0x02];//原对码架号
			u16DestScNumb |= (u16)((stRxFrame.u8DT[0x03]&0x01)<<8);	
			if(u16DestScNumb != u16DestScNumbBackup)
			{
				break;				
			}
			
			u16DestScNumb = (u16)stRxFrame.u8DT[0x03];//对码架号
			u16DestScNumb >>= 1;//对码架号
			u16DestScNumb |= (u16)((stRxFrame.u8DT[0x04]&0x03)<<7);			
			if(u16DestScNumb != u16DestScNumbBackup)
			{
				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB, &u16DestScNumb);			
				LogicRunInfApi(LOGIC_SET_CONLIBRATE_SUCCESS_SC_NUMB,&u16DestScNumb);				
			}			

			u16DestScNumb = stRxFrame.u8DT[0x06];//主控时间
			u16DestScNumb |= (u16)(stRxFrame.u8DT[0x07]<<8);
			u16DestScNumb *= 100;//单位0.1s
			LogicParamApi(LOGIC_SET_SC_CTRL_DISPALY_TIMER, &u16DestScNumb);	
			
			u16DestScNumb = stRxFrame.u8DT[0x01];//架号增向
			LogicRunInfApi(LOGIC_GET_SC_NUMB_ADD_DIRCT, &u16DestScNumbBackup);	
			if(u16DestScNumb != u16DestScNumbBackup)
				LogicRunInfApi(LOGIC_SET_SC_NUMB_ADD_DIRCT, &u16DestScNumb);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//更新对码显示
			OSSemPost(g_stSemInterface);			
			break;

		case RFID_FT_GRP_ACTION_INF:
			if(stRxFrame.u8DT[0x06] != u8YkqID)
				break;
			u16DestScNumb = stRxFrame.u8DT[0x04];
			u16DestScNumb |= (u16)(stRxFrame.u8DT[0x05]<<8);
			if(u16DestScNumb != u16DestScNumbBackup)
				break;			
			/*获取信息**/
			u16DestScNumb = stRxFrame.u8DT[0x00];
			u16DestScNumb |= (u16 )(stRxFrame.u8DT[0x01]<<8);//
			u16DestScNumb &= 0x1FF;
			LogicRunInfApi(LOGIC_SET_GRP_NEAR_SC_NUMB, &u16DestScNumb);
			u16DestScNumb = stRxFrame.u8DT[0x01];
			u16DestScNumb |= (u16 )(stRxFrame.u8DT[0x02]<<8);
			u16DestScNumb >>= 1;
			u16DestScNumb &= 0x1FF;
			LogicRunInfApi(LOGIC_SET_GRP_REMOTE_SC_NUMB, &u16DestScNumb);
			u16DestScNumb = 0x00;
			u16DestScNumb = (stRxFrame.u8DT[0x02]&0x04);
			u16DestScNumb >>= 2;
			LogicRunInfApi(LOGIC_SET_GRP_ACTION_DIRECTION, &u16DestScNumb);
			u16DestScNumb = (stRxFrame.u8DT[0x02]&0xF8);
			u16DestScNumb >>= 3;
			LogicRunInfApi(LOGIC_GET_RFID_FRAME_TYPE, &u8I);			
			LogicRunInfApi(LOGIC_SET_GRP_ACK_ACTION_NAME, &u16DestScNumb);
			if(RfidGrpAckGetRfidAction((RFID_GRP_ACTION_TYPE)u16DestScNumb,(RFID_FRAME_TYPE)u8I,&u32RData))
				LogicRunInfApi(LOGIC_SET_SC_ACTION_NAME, &u32RData);			
			OSSemPost(g_stLogicSem);
            u32ReturnFlag = 0x01;
			break;

		case RFID_FT_SC_UNCOLIBATARE://控制器直接要求解除对码
			if(stRxFrame.u8DLC != 0x08)
				break;
			if(stRxFrame.u8DT[0x04] != u8YkqID)//遥控器ID
				break;		
			if(u16DestScNumbBackup != stRxFrame.u8DT[0x03])//对码架号
				break;
			u32RData = 0x00;
			//jhy			
			LogicRunInfApi(LOGIC_GET_COLIBRATE_SC_TYPE,&u32RData);
			if(u32RData == LOGIC_COLIBRATE_SC_INCREASE_DECREASE)
			{
				LogicRunInfApi(LOGIC_GET_RUN_STATUS, &u8I);
				if(u8I == LOGIC_STATUS_RFID_COLIBARAE_ING)
				{
					u8I = LOGIC_STATUS_RFID_COLIBARAE_FAIL;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u8I);
					InterfaceSetCtrlMode(INTERFACE_MODE_RELIEVE_COLIBRATE);
	          		OSSemPost(g_stLogicSem);

					u32ReturnFlag = 0x01;  
					break;
				}
				
			}	
			#if	(QUICK_KEY_CTRL_ENABLE_FLAG != 0)
				LogicRunInfApi(LOGIC_GET_SC_QUICK_TYPE, &u32Y);
				if((u32Y == SC_ACTION_QUICK_1)|| (u32Y == SC_ACTION_QUICK_2))
				{
					u32Y = SC_ACTION_QUICK_STOP;
					u32ReturnFlag = LogicRunInfApi(LOGIC_SET_SC_QUICK_TYPE, &u32Y);
				}
				else
			#endif	
				{
					u32ReturnFlag = LOGIC_STATUS_IDLE;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32ReturnFlag);	
				}
								
			u16DestScNumb = RFID_SIGNAL_TOO_SMALL;
			LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u16DestScNumb);				
			InterfaceInsertNote(INTERFACE_NOTE_SYSTEM_PASSIVE_UNCOLIBRATE);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_SC_UNCOLIBRATE);	
			OSSemPost(g_stSemInterface);
			if(g_u32ID == B_type)
			{
				/**设置RFID为休眠状态***/
				u8I = GetRfidCurStatus();//获取芯片状态
				if(u8I == CC1101_STATE_RX_OVERFLOW)
					RfidStrobe(CC1101_SFRX);    //冲洗 RX FIFO buffer.
				else if(u8I == CC1101_STATE_TX_UNDERFLOW)
					RfidStrobe(CC1101_SFTX);    //冲洗 TX FIFO buffer.
				else if(u8I == CC1101_STATE_IDLE)
					SetRfidSRX();
				else
					SetRFChipSleepMode();//让芯片进入休眠模式;
				u16DestScNumb = 0x00;
				while((u8I == CC1101_STATE_RX_OVERFLOW) || (u8I == CC1101_STATE_TX_UNDERFLOW) || (u8I == CC1101_STATE_IDLE))
				{						
					OSTimeDly(RFIDMNG_TASK_TIME_SET/TICK_TIME);
					u16DestScNumb ++;
					u8I = GetRfidCurStatus();//获取芯片状态
					if((u8I != CC1101_STATE_RX_OVERFLOW) && (u8I != CC1101_STATE_TX_UNDERFLOW) &&(u8I != CC1101_STATE_IDLE))
					{
						SetRFChipSleepMode();//让芯片进入休眠模式;
						break;
					}
					if(u16DestScNumb >= 10)
					{
						SetRFChipSleepMode();//让芯片进入休眠模式;
						break;
					}
				}	
			}			
			break;

		#if (KEY_AUTO_PRESS_PROC_FLAG != DISABLED)
			case RFID_FT_AUTOPRESS_ENABLED_ACK://自动补压开启应答
			case RFID_FT_AUTOPRESS_DISABLED_ACK://自动补压开启应答	
				if(stRxFrame.u8DT[0x06] != u8YkqID)
					break;
				u16DestScNumb = stRxFrame.u8DT[0x04];
				u16DestScNumb |= (u16)(stRxFrame.u8DT[0x05]<<8);
				if(u16DestScNumb != u16DestScNumbBackup)
					break;
				
				if(u16ScLsNumbBackup == stRxFrame.u32ID.ID.LiuShuiNumb)
					return 0x00;
				
				u16ScLsNumbBackup = stRxFrame.u32ID.ID.LiuShuiNumb;	
				
				if((stRxFrame.u8DT[0x07]&0x01) == 0x01)//自动补压设置应答接受
				{
					u32RData = 0x01;
					LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE, &u32RData);
				}
				else if((stRxFrame.u8DT[0x07]&0x01) == 0x00)
				{
					u32RData = 0x00;
					LogicRunInfApi(LOGIC_SET_BUTTON_ACK_VALUE, &u32RData);
				}
				else
					;
	            OSSemPost(g_stLogicSem);
	            u32ReturnFlag = 0x01;
				break;
		#endif
		case RFID_FT_WL_TEST_SEND_ACK:
        {
            RfidTestTimer = 300;        //无线测试超时时间
            s8RssiSignalInftest = RfidRcvFrm.u8AppendStatus[0x00];
            //s8IqiSignalInf = RfidRcvFrm.u8AppendStatus[0x01];
            if(s8RssiSignalInftest >= 128)
                s8RssiSignalInftest = (s8RssiSignalInftest-256)/2-RSSI_OFFSET_VALUE;
            else
                s8RssiSignalInftest = (s8RssiSignalInftest)/2-RSSI_OFFSET_VALUE;
            s8ScRssiSignal = stRxFrame.u8DT[0x02];
            if(s8ScRssiSignal >= 128)
                s8ScRssiSignal = (s8ScRssiSignal-256)/2-RSSI_OFFSET_VALUE;
            else
                s8ScRssiSignal = (s8ScRssiSignal)/2-RSSI_OFFSET_VALUE;            
            //两个数据由一个变化，发送界面更新信号量
             if(s8ScRssiSignal != s8oldScRssiSignal || s8RssiSignalInftest != s8oldRssiSignalInf)
             {
                 s8oldScRssiSignal = s8ScRssiSignal;
                 s8oldRssiSignalInf = s8RssiSignalInftest;
                 u16I = (u16)(InterfaceGetCtrlMode());
                if(u16I == INTERFACE_MODE_SC_UNCERTIFIED)
                {
                     InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_RFIDSIGNAL);
                     OSSemPost(g_stSemInterface);
                }
             }
        }break;		
		default:
			break;

	}
	
	return 0x01;

}
/*********************************************************************************
** 函数名称: UsartSendDataProc
** 功能描述：USART发送数据处理
** 函数输入：无
** 函数输出：None
** 注意事项：
**********************************************************************************/
void UsartSendDataProc(void)
{
	INT8U err;
				
	if((s_u16RfidUsartSendBufWrite != s_u16RfidUsartSendBufRead))
	{
		USART_SendData(WL_USART, *((u8*)&s_stRfidUsartSendBuf[s_u16RfidUsartSendBufRead]));
		USART_ITConfig(WL_USART, USART_IT_TXE, ENABLE);		
		OSSemPend(s_stUsartSend,20/TICK_TIME,&err);//等待数据接收完成
		
		if(s_stRfidUsartSendBuf[s_u16RfidUsartSendBufRead].ACK == 1)
		{
			OSSemPend(s_stUsartRecvd, 40/TICK_TIME, &err);//发送间隔
			if(err == OS_ERR_NONE)//接收数据，处理
			{
				if((s_stRfidUsartSendBuf[s_u16RfidUsartSendBufRead].FrameType == ACKFrameType)\
					&& ACKData == 0x01)
				{	
					s_u16RfidUsartSendBufRead++;
					s_u16RfidUsartSendBufRead %= USART_SEND_BUF_SIZE;
				}
				else
				{
					u8SendTime++;
				}	
			}
			else
			{
				u8SendTime++;
			}
			if(u8SendTime >= 3)
			{
				s_u16RfidUsartSendBufRead++;
				s_u16RfidUsartSendBufRead %= USART_SEND_BUF_SIZE;
				u8SendTime = 0;
			}			
		}
		else
		{
			s_u16RfidUsartSendBufRead++;
			s_u16RfidUsartSendBufRead %= USART_SEND_BUF_SIZE;
		}
	}
	else
	{
		;
	}	
}
/*********************************************************************************
** 函数名称: RfidSendDataProc
** 功能描述：RFID发送数据处理
** 函数输入：无
** 函数输出：None
** 注意事项：
**********************************************************************************/
u32 RfidSendDataProc(void)
{
	
	INT8U err;
	u32 u32ReturnFlag = 0x00;
	u16 u16Status;
	u32 u32ButtonVale = 0x00;
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
			OS_CPU_SR  cpu_sr = 0;
	#endif
	
	if(s_u16RfidSendBufRead != s_u16RfidSendBufWrite)
	{
		if(u32SendTimer == 0x00)
		{
							//按下加减键切换对码架时，为了能快速响应再次按下加减键，把本次的发送次数减小为1
			if(getColiScNumStatus() > FIRSTDOWN)
			{
				s_stRfidSendBuf[s_u16RfidSendBufRead].u16SendTimes  = 1;
//				s_u16RfidSendBufRead++;
//				s_u16RfidSendBufRead %= RFID_SEND_BUF_SIZE;
			}
			if(s_stRfidSendBuf[s_u16RfidSendBufRead].u16SendTimes)
			{
				u32ReturnFlag = RfidMngSendFrame(&(s_stRfidSendBuf[s_u16RfidSendBufRead]));
				if(s_stRfidSendBuf[s_u16RfidSendBufRead].u16SendTimes != RFID_SEND_TIMES_LIMITLESS)
					s_stRfidSendBuf[s_u16RfidSendBufRead].u16SendTimes--;
				if(s_stRfidSendBuf[s_u16RfidSendBufRead].u16SendTimes && (s_stRfidSendBuf[s_u16RfidSendBufRead].u16SendInterval != RFID_SEND_INTERVAL_CLOSED))
					u32SendTimer = s_stRfidSendBuf[s_u16RfidSendBufRead].u16SendInterval;					

				if(u32ReturnFlag && (s_stRfidSendBuf[s_u16RfidSendBufRead].u8WaitAckFlag == ACK))//发送正确完成
				{
					u32SendTimer = s_stRfidSendBuf[s_u16RfidSendBufRead].u16SendInterval;
					if((u32SendTimer == RFID_SEND_INTERVAL_CLOSED) || (u32SendTimer == 0x00))
						u32SendTimer= RFID_SEND_INTERVAL_DEFAULT;//防止出现死等的情况出现
					#if RFID_TEST
						OSSemPost(s_stRfidRecvd);
					#endif
					OSSemPend(s_stRfidRecvd, u32SendTimer/TICK_TIME, &err);//发送间隔								
					u32SendTimer = 0x00;
					if(err == OS_ERR_NONE)//接收数据，处理
					{					
						if(RfidRecvDataProc() == 0x01)//接收数据判定正确
						{						
							LogicRunInfApi(LOGIC_GET_RUN_STATUS, &u16Status);
							LogicRunInfApi(LOGIC_GET_BUTTON_VALUE, &u32ButtonVale);
							//非成组操作时，可接收到按键回应即结束
							//成组操作时，启动键应答即可结束
							//成组操作时，左右邻架键或停止键即可结束
							if(u16Status != LOGIC_STATUS_RFID_SC_GRP)
							{
								OS_ENTER_CRITICAL(); 
								s_u16RfidSendBufRead++;
								s_u16RfidSendBufRead %= RFID_SEND_BUF_SIZE;
								u16LiuShuiNumb++;
								u16LiuShuiNumb %= STFRAME_LS_MAX;	
								OS_EXIT_CRITICAL();
								u32SendTimer = 0x00;
							}
							else if((u16Status == LOGIC_STATUS_RFID_SC_GRP)&&(((u32ButtonVale&RFID_STATUS_NEIGHBOR_LEFT) == RFID_STATUS_NEIGHBOR_LEFT)||((u32ButtonVale&RFID_STATUS_NEIGHBOR_RIGHT) == RFID_STATUS_NEIGHBOR_RIGHT)))
							{
								OS_ENTER_CRITICAL(); 
								s_u16RfidSendBufRead++;
								s_u16RfidSendBufRead %= RFID_SEND_BUF_SIZE;
								u16LiuShuiNumb++;
								u16LiuShuiNumb %= STFRAME_LS_MAX;	
								OS_EXIT_CRITICAL();
								u32SendTimer = 0x00;								
							}
							else if((u16Status == LOGIC_STATUS_RFID_SC_GRP)&&(((u32ButtonVale&RFID_STATUS_START) == RFID_STATUS_START)||(((u32ButtonVale&RFID_STATUS_STOP) == RFID_STATUS_STOP))))
							{
								OS_ENTER_CRITICAL(); 
								s_u16RfidSendBufRead++;
								s_u16RfidSendBufRead %= RFID_SEND_BUF_SIZE;
								u16LiuShuiNumb++;
								u16LiuShuiNumb %= STFRAME_LS_MAX;	
								OS_EXIT_CRITICAL();
								u32SendTimer = 0x00;								
							}							
							else//成组操作时
							{
								u32SendTimer = s_stRfidSendBuf[s_u16RfidSendBufRead].u16SendInterval;
								if((u32SendTimer == RFID_SEND_INTERVAL_CLOSED) || (u32SendTimer == 0x00))
									u32SendTimer= RFID_SEND_INTERVAL_DEFAULT;//防止出现死等的情况出现
								OSSemPend(s_stRfidRecvd, u32SendTimer/TICK_TIME, &err);//发送间隔						
								u32SendTimer = 0x00;
								if(err == OS_ERR_NONE)//接收数据，处理	
								{
									if(RfidRecvDataProc() == 0x01)//接收数据判定正确(按键应答及成组信息均会发送一次，不存在多次发送的情况)
									{
										OS_ENTER_CRITICAL(); 
										s_u16RfidSendBufRead++;
										s_u16RfidSendBufRead %= RFID_SEND_BUF_SIZE;	
										u16LiuShuiNumb++;
										u16LiuShuiNumb %= STFRAME_LS_MAX;	
										OS_EXIT_CRITICAL();
										u32SendTimer = 0x00;										
									}
								}
								else
								{
									RfidRecvWaitTimeoutProc();
									u32SendTimer = 0x00;									
								}
							}
						}
						else
						{
							RfidRecvWaitTimeoutProc();
							u32SendTimer = 0x00;	
						}
					}
					else if(err == OS_ERR_TIMEOUT)//接收数据，超时
					{	
						RfidRecvWaitTimeoutProc();
						u32SendTimer = 0x00;
					}
				}
			}
			else
			{
				#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
					if((u16YkqSendScIdBackup == s_stRfidSendBuf[s_u16RfidSendBufRead].u8DestDevID) && (s_stRfidSendBuf[s_u16RfidSendBufRead].stFrame.u32ID.ID.FrameType == RFID_FT_WL_UNCOLIBATARE))
				#else
					if(s_stRfidSendBuf[s_u16RfidSendBufRead].stFrame.u32ID.ID.FrameType == RFID_FT_WL_UNCOLIBATARE)
				#endif
					{
						if(g_u32ID == B_type)
						{
							err = GetRfidCurStatus();//获取芯片状态
							if(err == CC1101_STATE_RX_OVERFLOW)
								RfidStrobe(CC1101_SFRX);    //冲洗 RX FIFO buffer.
							else if(err == CC1101_STATE_TX_UNDERFLOW)
								RfidStrobe(CC1101_SFTX);    //冲洗 TX FIFO buffer.
							else if(err == CC1101_STATE_IDLE)
								SetRfidSRX();
							else
								SetRFChipSleepMode();//让芯片进入休眠模式;
							u16Status = 0x00;
							while((err == CC1101_STATE_RX_OVERFLOW) || (err == CC1101_STATE_TX_UNDERFLOW) || (err == CC1101_STATE_IDLE))
							{						
								OSTimeDly(RFIDMNG_TASK_TIME_SET/TICK_TIME);
								u16Status ++;
								err = GetRfidCurStatus();//获取芯片状态
								if((err != CC1101_STATE_RX_OVERFLOW) && (err != CC1101_STATE_TX_UNDERFLOW) &&(err != CC1101_STATE_IDLE))
								{
									SetRFChipSleepMode();//让芯片进入休眠模式;
									break;
								}
								if(u16Status >= 10)
								{
									SetRFChipSleepMode();//让芯片进入休眠模式;
									break;
								}
							}
						}
									
					}
				
				OS_ENTER_CRITICAL(); 
				if(s_stRfidSendBuf[s_u16RfidSendBufRead].stFrame.u32ID.ID.FrameType == RFID_FT_WL_UNCOLIBATARE)
				{					
					#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
						if(u16LiuShuiNumb != u16YkqSendLsNumbBackup)
						{
							u16LiuShuiNumb = u16YkqSendLsNumbBackup;
						}
					#else	
						u16LiuShuiNumb = 0x00;
					#endif					
				}
				else
				{
					u16LiuShuiNumb++;
					u16LiuShuiNumb %= STFRAME_LS_MAX;	
				}
				s_u16RfidSendBufRead++;
				s_u16RfidSendBufRead %= RFID_SEND_BUF_SIZE;				
				OS_EXIT_CRITICAL();
				u32SendTimer = 0x00;				
			}
		}			
	}
	return u32ReturnFlag;
}

/*********************************************************************************
** 函数名称: RfidUsartMngInint
** 功能描述：串口变量初始化
** 函数输入：*src
** 函数输出：des
** 注意事项：
**********************************************************************************/
void RfidUsartMngInint(void)
{
	memset(s_stRfidUsartSendBuf,0x00,sizeof(stUSARTDATA)*USART_SEND_BUF_SIZE);
	s_u16RfidUsartSendBufWrite = 0x00;
	s_u16RfidUsartSendBufRead = 0x00;
	u16LiuShuiNumb = 0x00;	
}

/*********************************************************************************
** 函数名称: UsartRecvInvalidDataProc
** 功能描述：Usart数据接收处理函数
** 函数输入：无
** 函数输出：None
** 注意事项：
**********************************************************************************/
void UsartRecvInvalidDataProc(void)
{
	stUSARTDATA UsartRcvFrm;	
	if(Usart_GetFrame(&UsartRcvFrm) == 0)
	 return;
	if(UsartRcvFrm.ACK == 0 && UsartRcvFrm.FrameType > 0 && UsartRcvFrm.u8DLC == 1)
	{
		//无线数据不应答
		if(UsartRcvFrm.FrameType == WIRELESSDATA)
		{
			return;
		}
		ACKData	= UsartRcvFrm.SendBuf[0];
		ACKFrameType = UsartRcvFrm.FrameType;
		OSSemPost(s_stUsartRecvd);	//RFID接收到数据
		return;
	}
	switch(UsartRcvFrm.FrameType)
	{
		//心跳
		case HEARTBEAT:			
			SetHeartBeat(UsartRcvFrm);
			break;	
		//设置
		case PARAMETERSETTING:
			SendSetupToWl(RFID_ININT_SYNC1);
			break;	
		default:
			break;
	}			
}


/*********************************************************************************
** 函数名称: RfidMng_Task
** 功能描述：RFID管理任务处理
** 函数输入：无
** 函数输出：None
** 注意事项：
**********************************************************************************/
void RfidMng_Task(void *p_arg)
{
	INT8U err;
	RfidUsartMngInint();
	RfidMngInint();

	while(1)
	{	
		OSSemPend(s_stRfidRecvd, RFIDMNG_TASK_TIME_SET/TICK_TIME, &err);//发送间隔			
		if(err == OS_ERR_NONE)//接收数据，处理
		{
			RfidRecvInvalidDataProc();//接收数据处理
		}
		else
		{
			RfidSendDataProc();//发送数据处理
		}
		
		if(u32SendTimer && (u32SendTimer != RFID_SEND_INTERVAL_CLOSED))
		{
			if(u32SendTimer >= 10)
				u32SendTimer -= 10;
			else
				u32SendTimer = 0x00;
		}
		//usart数据接收处理
		if(g_u32ID != B_type)
				UsartRecvInvalidDataProc();	
	}
}


/********************************************************************************
** 功能描述：串口数据处理主任务
** 输　入：  无
** 输　出：  无
*********************************************************************************/
void Usart_Task(void *p_arg)
{	
	while(1)
	{	
		UsartSendDataProc();
		HeartBeatCommErrTimer();
		//执行周期10ms
		OSTimeDly(10/TICK_TIME);
	}
}

