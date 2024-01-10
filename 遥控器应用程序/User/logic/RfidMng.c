/********************************************************************************
* �ļ����ƣ�RfidMng.c
* ��    �ߣ�������   
* ��    ����V1.0
* ��    �ڣ�2015.05.18
* ��������: ʵ��RFID��Э��㴦��

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
#include	"includes.h"

/********************************************************************************
* #define�궨��
*********************************************************************************/
__packed typedef struct
{
	u8 u8DestDevID;
	u16 u16SendTimes;	
	u8 u8WaitAckFlag;//�Ƿ���Ҫ�ȴ��Է�Ӧ���־
	u8 u8OtherAckFlag;//�Է��Ѿ�Ӧ���־
	
	stFRAME stFrame;
	u32 u32SendLength;
	u16 u16SendInterval;	
	
}RFID_MNG_DATA_TYPE;

/**����SCӦ������������������ʾ�Ķ�Ӧ��ϵ**/
typedef struct
{
	RFID_GRP_ACTION_TYPE	stGrpActionAckCode;//SCӦ��ĳ��鶯����
	RFID_FRAME_TYPE			stActionCodeType;//�����������
	u32			u32ActionCode;//RFID�ж���Ķ�����(��ʵ�ʱ��ؼ�ִ�еĶ���)	
}RFID_GRP_ACK_INTERFACE_TYPE;

u32 RfidTestTimer = 0xffff;      //���߲��Գ�ʱʱ��
/********************************************************************************
* ��������
*********************************************************************************/
#define	RFIDMNG_TASK_TIME_SET				10//������ʱ����
#define	RFID_SEND_BUF_SIZE					20
#define STFRAME_LS_MAX						255//��ˮ�ŵ����ֵ

#define	RFID_SEND_INTERVAL_CLOSED			0xFFFFFFFF//�ȴ�ʱ����Ч(�ر�)
#define	RFID_SEND_INTERVAL_DEFAULT			40//Ĭ�ϵĵȴ����

#define	RFID_SEND_TIMES_LIMITLESS			0xFFFF//���޴η���

#define	RFID_SEND_ALLDEV_ID					0xFF//Ⱥ����ַ

#define	RFID_TEST							0

#define	RSSI_OFFSET_VALUE					74

#define RFID_KEYUP_SEND_TIMES_MAX			3//�ײ��������з��͵�������

#define RFID_NO_ACK_NUMBER    30
/********************************************************************************
* ��������
*********************************************************************************/
/**����SCӦ����鶯�����������ʾ�����Ķ�Ӧ��ϵ��**/
static RFID_GRP_ACK_INTERFACE_TYPE const	s_stGrpActionInterfaceList[RFID_GRP_ACTION_MAX] = 
{
	/*SCӦ��ĳ��鶯����**/				/**����������**/					/**������**/
	{RFID_GRP_YIJIA ,					       RFID_FRAME_ACTION,					RFID_ACTION_YIJIA},
	{RFID_GRP_LALIU ,					       RFID_FRAME_ACTION,					RFID_ACTION_YIJIA},
	{RFID_GRP_TUILIU ,				       RFID_FRAME_ACTION,					RFID_ACTION_TUILIU},
	{RFID_GRP_LA_HOULIU ,			       RFID_FRAME_ACTION,					RFID_ACTION_LA_HOULIU},
	{RFID_GRP_SHOU_HUBANG ,			     RFID_FRAME_ACTION,					RFID_ACTION_SHOU_HUBANG},
															    
	{RFID_GRP_SHEN_HUBANG ,				   RFID_FRAME_ACTION,					RFID_ACTION_SHEN_HUBANG},
	{RFID_GRP_SHOU_HUBANG2 ,			   RFID_FRAME_ACTION,					RFID_ACTION_SHOU_HUBANG2},
	{RFID_GRP_SHEN_HUBANG2 ,			   RFID_FRAME_ACTION,					RFID_ACTION_SHEN_HUBANG2},
	{RFID_GRP_FANGMEI_PENWU ,			   RFID_FRAME_ACTION,					RFID_ACTION_FANGMEI_PENWU},
	{RFID_GRP_SHOUHUBANG_UNION ,	   RFID_FRAME_STATUS,					RFID_STATUS_GRP_SHOUHUBANG_UNION},//�����ջ���������������
															    
	{RFID_GRP_SHENHUBANG_UNION ,	   RFID_FRAME_STATUS,					RFID_STATUS_GRP_SHENHUBANG_UNION},//�컥��������������	
	{RFID_GRP_SHOU_SHENSUOLIANG ,    RFID_FRAME_ACTION,					RFID_ACTION_SHEN_SHENSUOLIANG},
	{RFID_GRP_SHEN_SHENSUOLIANG ,    RFID_FRAME_ACTION,					RFID_ACTION_SHOU_SHENSUOLIANG},
	{RFID_GRP_DINGLIANG_PENWU ,		   RFID_FRAME_ACTION,					RFID_ACTION_DINGLIANG_PENWU},
//	{RFID_GRP_FANGDINGMEI ,			   RFID_FRAME_ACTION,					RFID_ACTION_DINGLIANG_PENWU},
															    
	{RFID_GRP_BUYA ,							   RFID_FRAME_ACTION,					RFID_ACTION_SHENGZHU},	//  ����ģʽ��,��������ɲ�ѹ 
	{RFID_GRP_SHENG_TIAOLIU ,			   RFID_FRAME_ACTION,					RFID_ACTION_SHENG_TIAOLIU},	
	{RFID_GRP_JIANG_TIAOLIU ,			   RFID_FRAME_ACTION,					RFID_ACTION_JIANG_TIAOLIU},	
	{RFID_GRP_SHEN_HUBANG3 ,			   RFID_FRAME_ACTION,					RFID_ACTION_SHEN_HUBANG3},	
	{RFID_GRP_SHOU_HUBANG3 ,			   RFID_FRAME_ACTION,					RFID_ACTION_SHOU_HUBANG3}	
	
};

static u16 u16LiuShuiNumb;//��ˮ��
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
OS_EVENT *s_stUsartRecvd ;   //���ڽ����ź���

OS_EVENT *s_stUsartSend ;   //���ڽ����ź���

u8 ACKFrameType;
u8 ACKData;

u8 WLUnconnectedFlag= 0;

extern ACTION_INFO ActionInformation;
/********************************************************************************
* ��������
*********************************************************************************/
/*****************************************************************
** ��������MessageForRfidRcved
** �䡡�룺��
** �䡡������
** ����������RFID���ݽ��պ�Ļص������������ź���
******************************************************************/
void RfidMessageForRfidRcved(void)
{
	OSSemPost(s_stRfidRecvd);	//RFID���յ�����
}
/*****************************************************************
** ��������RfidMngSetRecvCallBack
** �䡡�룺��
** �䡡������
** ����������RFID���ݽ��պ�Ļص������������ź���
******************************************************************/
u32 RfidMngSetRecvCallBack(void)
{
	return RFID_RcvIntTransMsgRegister((RcvIntTransMsgFunc)RfidMessageForRfidRcved);
}
/*****************************************************************
** ��������RfidRecvSemCreate
** �䡡�룺��
** �䡡������
** ����������RFID���ݽ��պ�Ļص������������ź���
******************************************************************/
u32 RfidRecvSemCreate(void)
{
	s_stRfidRecvd  = OSSemCreate(0);	//�������������ź���
	if(g_u32ID != B_type)
	{
		s_stUsartRecvd  = OSSemCreate(0);	//�������������ź���
		s_stUsartSend = OSSemCreate(0);
	}
	return 0x01;
}

/*********************************************************************************
** ��������: RfidMngInint
** ����������RFID���������ʼ��
** �������룺��
** ���������None
** ע�����
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

		// ����RFID�������ݺ����ź���
		RfidMngSetRecvCallBack();

		/**����RFID��Ĭ�ϵ�ַ**/
		LogicParamApi(LOGIC_GET_RFID_NUMBER, &u8I);
		SetLocalAddress(u8I);

		u8I = GetRfidCurStatus();//��ȡоƬ״̬
		if(u8I == CC1101_STATE_RX_OVERFLOW)
			RfidStrobe(CC1101_SFRX);    //��ϴ RX FIFO buffer.
		else if(u8I == CC1101_STATE_TX_UNDERFLOW)
			RfidStrobe(CC1101_SFTX);    //��ϴ TX FIFO buffer.
		else if(u8I == CC1101_STATE_IDLE)
			SetRfidSRX();
		else
			SetRFChipSleepMode();//��оƬ��������ģʽ;
		u16Status = 0x00;
		while((u8I == CC1101_STATE_RX_OVERFLOW) || (u8I == CC1101_STATE_TX_UNDERFLOW) || (u8I == CC1101_STATE_IDLE))
		{						
			OSTimeDly(RFIDMNG_TASK_TIME_SET/TICK_TIME);
			u16Status ++;
			u8I = GetRfidCurStatus();//��ȡоƬ״̬
			if((u8I != CC1101_STATE_RX_OVERFLOW) && (u8I != CC1101_STATE_TX_UNDERFLOW) &&(u8I != CC1101_STATE_IDLE))
			{
				SetRFChipSleepMode();//��оƬ��������ģʽ;
				break;
			}
			if(u16Status >= 10)
			{
				SetRFChipSleepMode();//��оƬ��������ģʽ;
				break;
			}
		}		
	}
}

/*********************************************************************************
** ��������: RfidGrpAckGetRfidAction
** �������������ҳ���Ӧ��������Ӧ��RFID����
** �������룺��
** ���������None
** ע�����
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
** ��������: RfidGrpAckGetScAckCode
** ��������������RFID��������Ӧ�ĳ���Ӧ����
** �������룺��
** ���������None
** ע�����
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
* ���������� ����usart���Ͷ���
* ��ڲ�����
* ����ֵ����
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
	//����crc
	Crc16UsartSendChick((u8*)&(s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].Address),s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].u8DLC+5,&u16CrcTemp);
	s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].CRCHigh = (u8)((u16CrcTemp&0xFF00)>>8);
	s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite].CRCLow = (u8)(u16CrcTemp&0x00FF);
	OS_EXIT_CRITICAL();	
	//������������
	if(isSend)
	{
		USART_SendData(WL_USART, *((u8*)&s_stRfidUsartSendBuf[s_u16RfidUsartSendBufWrite]));
		USART_ITConfig(WL_USART, USART_IT_TXE, ENABLE);
		s_stUsartSend->OSEventCnt = 0;
		OSSemPend(s_stUsartSend,20/TICK_TIME,&err);//�ȴ����ݽ������
		u8ReturnFlag = (err == OS_ERR_TIMEOUT) ? 0 : 1;	
	}
	//���ݴ滺��
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
** ��������������Ӧ������
** �������룺��
** ���������None
** ע�����
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
**�������ƣ�HubCanTransformTx
**�������ã�HubCan�������ݴ���
**������������
**�����������
**ע�����u8WaitAckFlag]:1,u16IntervalӦ��ȥ������ʱ��u8WaitAckFlag:0����Ϊʵ�ʵķ��ͼ��
*******************************************************************************************/
u8 RfidTransformTx(stFRAME *pFrame,u8 u8DevID,u16 u16SendTimes,u16 u16Interval,u8 u8WaitAckFlag)//����Ŀ��ͳһʹ����չ֡���ޱ�׼֡
{
	u16 u16TxBufWriteBackup = 0x00;
	u32 u32ReturnFlag = 0x01;
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
		OS_CPU_SR  cpu_sr = 0;
	#endif

	/**�����ݼ���ʱ���Զ�ֹ֮ͣǰ�κ����ݵķ���**/
	OS_ENTER_CRITICAL(); 
	if(s_u16RfidSendBufRead != s_u16RfidSendBufWrite)
	{
		s_u16RfidSendBufRead = s_u16RfidSendBufWrite;
	}
	OS_EXIT_CRITICAL();	

	/**��ֹ���������������**/
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
		u32ReturnFlag =  0x00;//�����������������ټ��������
	}	
	
	return u32ReturnFlag;
}

/*******************************************************************************************
**�������ƣ�RfidMngSendFrame
**�������ã��������ݴ���
**������������
**�����������
**ע�������
*******************************************************************************************/
u8 RfidMngSendFrame(RFID_MNG_DATA_TYPE *pData)//����Ŀ��ͳһʹ����չ֡���ޱ�׼֡
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
		||(pData->stFrame.u32ID.ID.FrameType == RFID_FT_HEART))//�����������£���ˮ��Ӧ�øı�
	{
		OS_ENTER_CRITICAL(); 
		pData->stFrame.u32ID.ID.LiuShuiNumb = u16LiuShuiNumb;
		
		u16LiuShuiNumb++;
		u16LiuShuiNumb %= STFRAME_LS_MAX;
		
		u32SendTimer = 0x00;//���������´η���
		OS_EXIT_CRITICAL();	
	}
	else
	{
		pData->stFrame.u32ID.ID.LiuShuiNumb = u16LiuShuiNumb;
	}	
	// zzw 2023.12.26 ���ӳ��ȳ���8�Ĵ���
	if(pData->stFrame.u8DLC > 8)	//���ݳ��ȴ���8
	{
		//��һ��
		pData->stFrame.u32ID.ID.Sum = u16LiuShuiNumb; //u16LiuShuiNumb
		pData->stFrame.u32ID.ID.Sub = 0;
//		pData->stFrame.u8DLC = 8;
		pData->stFrame.u32ID.ID.LiuShuiNumb = pData->stFrame.u8DLC/8;; //pData->stFrame.u8DLC/8;
		u16LiuShuiNumb++;
		u16LiuShuiNumb %= STFRAME_LS_MAX;
		
		memcpy(u8Buf,&(pData->stFrame.u32ID),0x04);
		u32SendLength = 0x04;
		u8Buf[u32SendLength++] = 8;	//����
		memcpy(u8Buf+u32SendLength,&(pData->stFrame.u8DT[0x00]),8);
		u32SendLength += 8;
		//�ڶ��� ��֡
		pData->stFrame.u32ID.ID.Sum = u16LiuShuiNumb;
		pData->stFrame.u32ID.ID.Sub = 1;
		pData->stFrame.u32ID.ID.LiuShuiNumb = 0;
		u16LiuShuiNumb++;
		u16LiuShuiNumb %= STFRAME_LS_MAX;
		memcpy(u8Buf1,&(pData->stFrame.u32ID),0x04);
		u32SendLength1 = 0x04;
		u8Buf1[u32SendLength1++] = pData->stFrame.u8DLC - 8;	//����
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
	
	#if 0//����֤���˷�ʽ����(���붼���ܳɹ�)
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
			OSTimeDly(RFIDMNG_TASK_TIME_SET/TICK_TIME);//���ͼ��	
		}

		if(u32SendTimes >= RFID_DRIVER_SEND_TIMES_MAX)//�����豸����
		{
			InterfaceInsertNote(INTERFACE_NOTE_RFID_SEND_FAIL);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);//��ʾ����������Ϣ	
			OSSemPost(g_stSemInterface);
		}
	
	#else
	//D��usart����
	if(g_u32ID != B_type)
	{	if(pData->stFrame.u8DLC > 8)
		{
			//����һ֡
			OS_ENTER_CRITICAL();
			memset(u8tmp_Usart,0x00,u32SendLength+1);
			u8tmp_Usart[0] = pData->u8DestDevID;
			memcpy(&u8tmp_Usart[1],u8Buf,u32SendLength);
			OS_EXIT_CRITICAL();		
			InsUsartTrsQueue(u8tmp_Usart,u32SendLength+1,WIRELESSDATA,NOACK,1);			
			OSTimeDly(10);			
			//���ڶ�֡
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
	//B��RFID����
	else
	{
		RFID_SendData(pData->u8DestDevID,u8Buf,&u32SendLength);
	}
		u32ReturnValue= 0x01;
	#endif
	
	return u32ReturnValue;
}

/***************************************************************************************
** ��������: RfidMngSendDatacolibarate
** �������������Ͷ�������
** �������룺��
** ���������None
** ע�����
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
	if(u16DelayTime < LOGIC_SEND_TIMEOUT_MIN)//�ȴ�ʱ��Ϊ0ʱ����ֹ�������ȵ��������
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
			stSendFrame.u8DT[0x02] |= (u8)((LOGIC_COLIBRATE_SC_INCREASE_DECREASE-0x01)<<6);//2017.01.12.��������Э���޸�
			/**2016.03.15,���ݻ����Ҫ,+-��ʱ������8�Σ����150ms**/
//			if(u16ScID != IN_SC_SS_NUMB)
//				RfidTransformTx(&stSendFrame, (u8)u16ScID,u16SendTimes+0x05, u16DelayTime+50-RFIDMNG_TASK_TIME_SET,ACK);//���ʹ����Ȳ���������
//			else
//				RfidTransformTx(&stSendFrame, SYSTEM_SS_ID,u16SendTimes+0x05, u16DelayTime+50-RFIDMNG_TASK_TIME_SET,ACK);//���ʹ����Ȳ���������
			if(u16ScID != IN_SC_SS_NUMB)
				RfidTransformTx(&stSendFrame, (u8)u16ScID,u16SendTimes+0x1b, u16DelayTime-RFIDMNG_TASK_TIME_SET,ACK);//���ʹ���30��jhyjhy
			else
				RfidTransformTx(&stSendFrame, SYSTEM_SS_ID,u16SendTimes+0x05, u16DelayTime+50-RFIDMNG_TASK_TIME_SET,ACK);//���ʹ����Ȳ���������			
			
		}
		else
		{
			stSendFrame.u8DT[0x02] |= (u8)((LOGIC_COLIBRATE_SC_SHIFT_BUTTON-0x01)<<6);//2017.01.12.��������Э���޸�
			if(u16ScID != IN_SC_SS_NUMB)
				RfidTransformTx(&stSendFrame, (u8)u16ScID,u16SendTimes+0x02, u16DelayTime-RFIDMNG_TASK_TIME_SET,ACK);//���ʹ����Ȳ���������
			else
				RfidTransformTx(&stSendFrame, SYSTEM_SS_ID,u16SendTimes+0x02, u16DelayTime-RFIDMNG_TASK_TIME_SET,ACK);//���ʹ����Ȳ���������			
		}
	#else
		if(u16ScID != IN_SC_SS_NUMB)
			RfidTransformTx(&stSendFrame, (u8)u16ScID,u16SendTimes+0x02, u16DelayTime-RFIDMNG_TASK_TIME_SET,ACK);//���ʹ����Ȳ���������
		else
			RfidTransformTx(&stSendFrame, SYSTEM_SS_ID,u16SendTimes+0x02, u16DelayTime-RFIDMNG_TASK_TIME_SET,ACK);//���ʹ����Ȳ���������
	#endif
	
	#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG != DISABLED)
		u16YkqSendScIdBackup = u16ScID;//����
		u16YkqSendLsNumbBackup = u16LiuShuiNumb;//���б���
		u16LiuShuiNumb = 0x00;//ÿ�ζ����0��ʼ����
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
** ��������: RfidMngSendDatacolibarate
** �������������ͽ�������
** �������룺��
** ���������None
** ע�����
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
			u16LiuShuiNumb = u16YkqSendLsNumbBackup;//���лָ�	
			u16YkqSendLsNumbBackup = u16ScID;
		}
	#endif
	
	return 0x01;
}

/***************************************************************************************
** ��������: ProtocolTransRx
** �������������ն�����ʱ��ԭЭ��Ϊ1bit��ʾ1����������Э��Ϊ1��������ű�ʾһ���������������Ĺ�����Э��ת��
** �������룺stFrameType�����յĽṹ������
** ���������������
** ע�����
******************************************************************************************/
u64 ProtocolTransRx(stFRAME *pFrame)
{
	u8 u8ActionSerialNum[6];	//����ͬʱ����6������
	u8 u8i = 0;
	u32 u32j = 0;
	static u64 u64k;
	u64k = 0x00;

	u32j |= (u32)(pFrame->u8DT[0x00]) ;		
	u32j |= (u32)(pFrame->u8DT[0x01]) << 8 ;
	u32j |= (u32)(pFrame->u8DT[0x02]) << 16 ;
	u32j |= (u32)(pFrame->u8DT[0x03] & 0x3F) << 24;	
	for(u8i = 0; u8i < 5; u8i++)	//���30λ���ݰ�����5����ЧλΪ6λ�����ݣ�ȡ����5������(ÿ�����ݱ�ʾ1���������)
	{
		u8ActionSerialNum[u8i] = (u8)((u32j & ((u32)0x3F << (u8i * 6))) >> (u8i * 6));
	}
	u8ActionSerialNum[5] = pFrame->u8DT[0x07] >> 0x02;	//��6�����������[0x07]�ĸ�6λ��
	for(u8i = 0; u8i < 6; u8i++)	//����6���������ת��Ϊ����λ
	{
		if(u8ActionSerialNum[u8i] != 0x00)
			u64k |= (u64)0x01 << (u8ActionSerialNum[u8i] - 1);
	}
	u64k |= (u64)(0x01) << 62;	//�ö����������ʶ
	return u64k;
}

/***************************************************************************************
** ��������: ProtocolTransTx
** �������������Ͷ�����ʱ��ԭЭ��Ϊ1bit��ʾ1����������Э��Ϊ1��������ű�ʾһ���������������Ĺ�����Э��ת��
** �������룺stFrameType��Ҫ���͵Ľṹ�����ݣ�u64FrameData��������
** ���������None
** ע�����
******************************************************************************************/
u32 ProtocolTransTx(stFRAME *pFrame,u64 u64FrameData)
{
	u8 u8ActionSerialNum[6];	//����ͬʱ����6������
	u8 u8i,u8j=0;
	static u64 u64k;
	u64k = 0x00;
	memset(u8ActionSerialNum, 0x00, 6);
	for(u8i = 0; u8i < MAX_ACTION_NUM; u8i++)	
	{
		if(u64FrameData & ((u64)0x01 << u8i))		//��λ�ж��Ƿ񺬶�����־λ
		{
			u8ActionSerialNum[u8j] = u8i + 1;
			u8j++;
			if(u8j > 5) break;
			u64k = (u64)(((u64)0x01 << (u8i + 1)) - 1);
			if(((u64FrameData & 0x00FFFFFFFFFFFFFF) & (u64)(~u64k)) == (u64)0x00) //�ж��Ƿ��ж�����־?
				break;
		}
	}
	u64k = 0x00;
	//ÿ��������ŵĵ�6λ��Ч��ǰ5���������ȡ�������30bit���ֽڣ��ٰ�byte��ֵ�����ͽṹ��;��6�������ĵ�6λ��������u8DT[7]�ĸ�6λ��
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
** ��������: RfidMngSendDatacolibarate
** �������������Ͷ�����������
** �������룺��
** ���������None
** ע�����
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
	if(u16DelayTime < LOGIC_SEND_TIMEOUT_MIN)//�ȴ�ʱ��Ϊ0ʱ����ֹ�������ȵ��������
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
		LogicRunInfApi(LOGIC_GET_GRP_REMOTE_SC_NUMB,&u16GrpRemoSc);//Զ�˼ܺ�
		LogicRunInfApi(LOGIC_GET_GRP_NEAR_SC_NUMB,&u16GrpNearSc);//���˼ܺ�	
		LogicRunInfApi(LOGIC_GET_GRP_ACTION_DIRECTION,&u16Direction);//ִ�з���
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
	//�������£���5�Σ�	
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
** ��������: RfidMngNearCtrlReq
** �����������ڼܿ�������
** �������룺��
** ���������None
** ע�����
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
	if(u16DelayTime < LOGIC_SEND_TIMEOUT_MIN)//�ȴ�ʱ��Ϊ0ʱ����ֹ�������ȵ��������
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
	
	LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB, &CtrlID);		//����ܺ�
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
** ��������: RfidMngSendHeart
** ��������������֡
** �������룺��
** ���������u64FrameData��40����������ÿһλ����һ������id��ActionType����������
** ע������������ֶ�����ʱsendmodeΪ1�����������Ϊ0
******************************************************************************************/
u32 RfidMngSendHeart(u64 u64FrameData,u8 ActionType,u8 Sendmode)
{
	stFRAME stSendFrame;
	u8 u8YkqID;
	u16 u16ScID;
	
	memset(&stSendFrame,0x00,sizeof(stFRAME));
	
	stSendFrame.u32ID.ID.RxID = (u8)(SC_DEV_TYPE&0xFF);
	stSendFrame.u32ID.ID.TxID = (u8)(WL_DEV_TYPE&0xFF);
	stSendFrame.u32ID.ID.FrameType = 0;	//zzw2023.12.25 �����������
	stSendFrame.u32ID.ID.Sum = 0;
	stSendFrame.u32ID.ID.Sub = 0;
	stSendFrame.u32ID.ID.ACK = NO_ACK;
	stSendFrame.u32ID.ID.Reservd = 0;
	stSendFrame.u32ID.ID.NoUsed = 0;

	if(Sendmode == 1)		//�������ֶ�����ʱ
	{
		stSendFrame.u8DLC = 11;
		
		LogicRunInfApi(LOGIC_GET_CONLIBRATE_SC_NUMB, &u16ScID);
		
		LogicParamApi(LOGIC_GET_YKQ_NUMBER, &u8YkqID);

		stSendFrame.u8DT[0x00] = u16ScID;
		stSendFrame.u8DT[0x01] = u8YkqID;
		stSendFrame.u8DT[0x02] = ActionType;
		//zzw2023.12.25 ����֧�ֵ�64��
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
		//zzw2023.12.25 ����֧�ֵ�48��
		stSendFrame.u8DT[0x03] = (u8)( u64FrameData&0x00000000000000ff);
		stSendFrame.u8DT[0x04] = (u8)((u64FrameData&0x000000000000ff00)>>8);
		stSendFrame.u8DT[0x05] = (u8)((u64FrameData&0x0000000000ff0000)>>16);
		stSendFrame.u8DT[0x06] = (u8)((u64FrameData&0x00000000ff000000)>>24);
		stSendFrame.u8DT[0x07] = (u8)((u64FrameData&0x000000ff00000000)>>32);	
	}

	if(Sendmode == 1)		//�������ֶ�����ʱ
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
** ��������: RfidMngSendDatacolibarate
** �������������Ͱ�����������
** �������룺��
** ���������None
** ע�����ֻ�ڵ��ܡ��ڼܡ����ܶ���ʱ��Ч��(���鶯����Ч)
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

	if(stFrameType != 0x01)	//���ͷǶ�����ʱ��������Э��ת�������Ͷ�����ʱ������Э��ת��
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
	else	//�Զ����֣�����Э�飺30������������56������ 2019.12.24 parry
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
** ��������: RfidMngSendDatacolibarate
** �������������Ͱ���̧���������
** �������룺��
** ���������None
** ע�����
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
	
	if(stFrameType != 0x01)	//���ͷǶ�����ʱ��������Э��ת�������Ͷ�����ʱ������Э��ת��
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
	else	//�Զ����֣�����Э�飺30������������56������ 2019.12.24 parry
	{
		ProtocolTransTx(&stSendFrame, u64FrameData);
		stSendFrame.u8DT[0x03] |= (stFrameType <<= 6);
		stSendFrame.u8DT[0x04] = (u8)(u16ScID&0xFF);
		stSendFrame.u8DT[0x05] = (u8)((u16ScID&0xFF00)>>8);
		stSendFrame.u8DT[0x06] = u8YkqID;
	}
	/*��ȡ��ǰ�Ŀ���ģʽ**/	
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
				RfidTransformTx(&stSendFrame, u16ScID,5, 90,ACK);//100*10 = 1s,֧�ܿ���������ʱ5s���Զ�ֹͣ
			else
				RfidTransformTx(&stSendFrame, SYSTEM_SS_ID,10, 90,ACK);//100*10 = 1s,֧�ܿ���������ʱ5s���Զ�ֹͣ
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
** ��������: RfidMngWLTestSend
** �������������߲��Է�������
** �������룺��
** ���������None
** ע�����
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
** ��������: RfidMngSendAutoPressEnabledUp
** ���������������Զ���ѹ��������
** �������룺��
** ���������None
** ע�����
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
** ��������: RfidGetDataLimitlessSendFlag
** ������������ȡRFID�����Ƿ������޴η��͵�״̬
** �������룺��
** ���������None
** ע�����
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
** ��������: RfidRecvDataProc
** ����������RFID���ݴ���
** �������룺��
** ���������None
** ע�����
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
** ��������: SendSetupToWl
** �������������ڷ��͸�������ǰ���뺯��
** �������룺type��33 55��isSendΪ�Ƿ�Ϊ��������
** ���������None
** ע�����
**********************************************************************************/
void SendSetupToWl(u8 type)
{	
	u8 data[3];
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
		OS_CPU_SR  cpu_sr = 0;
	#endif

	OS_ENTER_CRITICAL();	
	
	LogicParamApi(LOGIC_GET_YKQ_NUMBER, &data[0]);
	data[0] = 0xFF-data[0]; //ң�������	
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
** ��������: HeartBeatCommErrTimer
** ��������������ģ��������ش���
** �������룺��
** ���������None
** ע�����
**********************************************************************************/
void HeartBeatCommErrTimer(void)
{
	u32 u32Temp;
	LogicRunInfApi(LOGIC_GET_RUN_STATUS,&u32Temp);
	if(u32Temp != LOGIC_STATUS_SHUTUP)//���ǹػ�״̬	
	{		
		if((WlHeartTime != TIMER_EXPIRED) && (WlHeartTime != TIMER_CLOSED))
		{
			WlHeartTime -= 10;

			if(WLUnconnectedFlag == 1)//�жϺ���������   jhy
			{
				WLUnconnectedFlag = 0;
			}		
		}
		//������ʱ����ʱ
		if(WlHeartTime == TIMER_EXPIRED)
		{		
			WLUnconnectedFlag = 1;
			
			for(u32Temp = 0;u32Temp<6;u32Temp++)
			{
				u8WlVersion[u32Temp] = '*';
			}
			LogicParamApi(LOGIC_SET_WL_VERSION,&u8WlVersion);
			//�г�ʱ���ղ�����������������³�ʱ�Ƴ����ӣ�������ԭ��		zzw
			//�������
			if(RfidGetDataLimitlessSendFlag())
			{
				RfidSetDataLimitlessSendEnd();
			}				
//			KeyLedOnOff(0x00,KEY_LED_MAX);	//�رհ�����
//			StateLed(0x00,ADJUST_CODE_ING_LED|ADJUST_CODE_SUCCEED_LED|KEY_SHIFT_LED);	
			//�ر��ź�ָʾ
			u32Temp = RFID_SIGNAL_TOO_SMALL;
			LogicRunInfApi(LOGIC_SET_RFID_SIGNAL_STATUS, &u32Temp);
			//�������߶Ͽ�״̬
//			u32Temp = LOGIC_STATUS_UNCONNECT_WL;
//			LogicRunInfApi(LOGIC_SET_RUN_STATUS,&u32Temp);
			InterfaceInsertNote(WL_UNCONNECTED);
//			InterfaceSetCtrlMode(WL_UNCONNECTED);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_NOTE);
			OSSemPost(g_stSemInterface);	
			#if (HEART == 1)		//����֡			
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
** ��������: SetHeartBeat
** �����������������ݴ���
** �������룺��
** ���������None
** ע�����
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
s8 s8RssiSignalInf;     //ң���������ź�ǿ��
s8 s8RssiSignalInftest;     //ң���������ź�ǿ��
s8 s8ScRssiSignal;      //�������ź�ǿ��
static s8 s8oldRssiSignalInf;     //ң���������ź�ǿ��
static s8 s8oldScRssiSignal;      //�������ź�ǿ��
/*********************************************************************************
** ��������: RfidRecvDataProc
** ����������RFID���ݴ���
** �������룺��
** ���������None
** ע�����
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
			
			#if 0//Ϊ��֤��ԭ������֮���ͨ���ԣ��ݲ��Զ������ͽ����ж�
				u16I = 0x00;
				LogicRunInfApi(LOGIC_GET_COLIBRATE_SC_TYPE,&u16I);
				if(u16I == LOGIC_COLIBRATE_SC_INCREASE_DECREASE)//2017.01.12,��������Э���޸�
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
			
			if((stRxFrame.u8DT[0x02]&0x01) == 0x01)//����ɹ�
			{
				u8I = stRxFrame.u8DT[0x01];
				LogicRunInfApi(LOGIC_GET_SC_NUMB_ADD_DIRCT, &u8YkqID);
				if(u8I != u8YkqID)
				{
					LogicRunInfApi(LOGIC_SET_SC_NUMB_ADD_DIRCT, &u8I);
					InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_SCCOLIBRATE);//���¶�����ʾ
					OSSemPost(g_stSemInterface);					
				}
				u16DestScNumb = (u16)(stRxFrame.u8DT[0x07]<<8);
				u16DestScNumb |= stRxFrame.u8DT[0x06];
				u16DestScNumb *= 100;//��λ0.1s
				LogicParamApi(LOGIC_SET_SC_CTRL_DISPALY_TIMER, &u16DestScNumb);

				LogicRunInfApi(LOGIC_GET_RUN_STATUS, &u8I);
				if(u8I == LOGIC_STATUS_RFID_COLIBARAE_ING)
				{
					u8I = LOGIC_STATUS_RFID_COLIBARAE_SUCCESS;
					LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u8I);
					InterfaceSetCtrlMode(INTERFACE_MODE_COLIBRATE_CODE_SUCCESS);
				}
				if((stRxFrame.u8DT[0x03]) == 0x01)//�����ļ�crcһ��	zzw
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
			
			if((stRxFrame.u8DT[0x04]&0x01) == 0x01)//��������
			{
				u16DestScNumb = SC_BUTTON_ACK_ACCEPT;
			}
			else
			{
				u16DestScNumb = SC_BUTTON_ACK_REJECT;	
			}

			LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS, &u16DestScNumb);
			LogicRunInfApi(LOGIC_GET_RUN_STATUS, &u16DestScNumb);
//			/**�ж��ڸ��ܲ���ʱ���Ƿ�Ϊ�����ڼܼ�**/
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
			
			if(u16DestScNumb != LOGIC_STATUS_RFID_SC_GRP)//�ǳ������ʱ���ɽ��յ�������Ӧ�������ź���
				OSSemPost(g_stLogicSem);//�������ʱ�����յ�������Ϣ�ŷ����ź���
			else if((u16DestScNumb == LOGIC_STATUS_RFID_SC_GRP)&&((u32ReturnFlag&RFID_STATUS_NEIGHBOR_LEFT) == RFID_STATUS_NEIGHBOR_LEFT))
				OSSemPost(g_stLogicSem);//�������ʱ�����յ�������Ϣ�ŷ����ź���
			else if((u16DestScNumb == LOGIC_STATUS_RFID_SC_GRP)&&((u32ReturnFlag&RFID_STATUS_NEIGHBOR_RIGHT) == RFID_STATUS_NEIGHBOR_RIGHT))
				OSSemPost(g_stLogicSem);//�������ʱ�����յ�������Ϣ�ŷ����ź���
			else if((u16DestScNumb == LOGIC_STATUS_RFID_SC_GRP)&&((u32ReturnFlag&RFID_STATUS_START) == RFID_STATUS_START))
				OSSemPost(g_stLogicSem);//�������ʱ�����յ�������Ϣ�ŷ����ź���
			else if((u16DestScNumb == LOGIC_STATUS_RFID_SC_GRP)&&((u32ReturnFlag&RFID_STATUS_STOP) == RFID_STATUS_STOP))
				OSSemPost(g_stLogicSem);//�������ʱ�����յ�������Ϣ�ŷ����ź���
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
			
			/*��ȡ��Ϣ**/
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
			u16DestScNumb = stRxFrame.u8DT[0x02];//ԭ����ܺ�
			u16DestScNumb |= (u16)((stRxFrame.u8DT[0x03]&0x01)<<8);	
			if(u16DestScNumb != u16DestScNumbBackup)
			{
				break;				
			}

			if(u16ScLsNumbBackup == stRxFrame.u32ID.ID.LiuShuiNumb)
				return 0x00;
			
			u16ScLsNumbBackup = stRxFrame.u32ID.ID.LiuShuiNumb;	
			
			u16DestScNumb = (u16)stRxFrame.u8DT[0x03];//����ܺ�
			u16DestScNumb >>= 1;//����ܺ�
			u16DestScNumb |= (u16)((stRxFrame.u8DT[0x04]&0x03)<<7);			
			if(u16DestScNumb != u16DestScNumbBackup)
			{
				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB, &u16DestScNumb);		
				LogicRunInfApi(LOGIC_SET_CONLIBRATE_SUCCESS_SC_NUMB,&u16DestScNumb);				
			}
			u16DestScNumb = stRxFrame.u8DT[0x06];//����ʱ��
			u16DestScNumb |= (u16)(stRxFrame.u8DT[0x07]<<8);
			u16DestScNumb *= 100;//��λ0.1s
			LogicParamApi(LOGIC_SET_SC_CTRL_DISPALY_TIMER, &u16DestScNumb);	
			
			u16DestScNumb = stRxFrame.u8DT[0x01];//�ܺ�����
			LogicRunInfApi(LOGIC_GET_SC_NUMB_ADD_DIRCT, &u16DestScNumbBackup);	
			if(u16DestScNumb != u16DestScNumbBackup)
				LogicRunInfApi(LOGIC_SET_SC_NUMB_ADD_DIRCT, &u16DestScNumb);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//���¶�����ʾ
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
			if((stRxFrame.u8DT[0x03] >> 6) != 0x01)	//�ԷǶ����֣�����ʽ���䣻�Զ����֣��밴��Э�����ת��	30������������56������ 2019.12.24 parry
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
			
			if(stRxFrame.u8DT[0x02] == 0x01)//��������
			{
				u16DestScNumb = SC_BUTTON_ACK_ACCEPT;
			}
			else//�����ܾ�
			{
				u16DestScNumb = SC_BUTTON_ACK_REJECT;		
			}	
			LogicRunInfApi(LOGIC_SET_BUTTON_ACCEPT_STATUS, &u16DestScNumb);
			OSSemPost(g_stLogicSem);//���½�����ʾ
            u32ReturnFlag = 0x01;            			
			break;

		case RFID_FT_SC_UNCOLIBATARE://������ֱ��Ҫ��������
			if(stRxFrame.u8DLC != 0x08)
				break;
			if(stRxFrame.u8DT[0x05] != u8YkqID)//ң����ID
				break;		
			if(u16DestScNumbBackup != (stRxFrame.u8DT[0x03]|(stRxFrame.u8DT[0x04]<<8)))//����ܺ�
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
			/**����RFIDΪ����״̬***/
			if(g_u32ID != B_type)
				;
			else
			{
				u8I = GetRfidCurStatus();//��ȡоƬ״̬
				if(u8I == CC1101_STATE_RX_OVERFLOW)
					RfidStrobe(CC1101_SFRX);    //��ϴ RX FIFO buffer.
				else if(u8I == CC1101_STATE_TX_UNDERFLOW)
					RfidStrobe(CC1101_SFTX);    //��ϴ TX FIFO buffer.
				else if(u8I == CC1101_STATE_IDLE)
					SetRfidSRX();
				else
					SetRFChipSleepMode();//��оƬ��������ģʽ;
				u16DestScNumb = 0x00;
				while((u8I == CC1101_STATE_RX_OVERFLOW) || (u8I == CC1101_STATE_TX_UNDERFLOW) || (u8I == CC1101_STATE_IDLE))
				{						
					OSTimeDly(RFIDMNG_TASK_TIME_SET/TICK_TIME);
					u16DestScNumb ++;
					u8I = GetRfidCurStatus();//��ȡоƬ״̬
					if((u8I != CC1101_STATE_RX_OVERFLOW) && (u8I != CC1101_STATE_TX_UNDERFLOW) &&(u8I != CC1101_STATE_IDLE))
					{
						SetRFChipSleepMode();//��оƬ��������ģʽ;
						break;
					}
					if(u16DestScNumb >= 10)
					{
						SetRFChipSleepMode();//��оƬ��������ģʽ;
						break;
					}
				}
			}
			break;

		#if (KEY_AUTO_PRESS_PROC_FLAG != DISABLED)
			case RFID_FT_AUTOPRESS_ENABLED_ACK://�Զ���ѹ����Ӧ��
			case RFID_FT_AUTOPRESS_DISABLED_ACK://�Զ���ѹ����Ӧ��	
				if(stRxFrame.u8DT[0x06] != u8YkqID)
					break;
				u16DestScNumb = stRxFrame.u8DT[0x04];
				u16DestScNumb |= (u16)(stRxFrame.u8DT[0x05]<<8);
				if(u16DestScNumb != u16DestScNumbBackup)
					break;
				
				if(u16ScLsNumbBackup == stRxFrame.u32ID.ID.LiuShuiNumb)
					return 0x00;
				
				u16ScLsNumbBackup = stRxFrame.u32ID.ID.LiuShuiNumb;	
				
				if((stRxFrame.u8DT[0x07]&0x01) == 0x01)//�Զ���ѹ����Ӧ�����
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
		case RFID_FT_NEAR_CTRL_CONFIRM:		//�ڼܿ���Ӧ��
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
** ��������: RfidRecvDataProc
** ����������RFID���ݴ���
** �������룺��
** ���������None
** ע�����
**********************************************************************************/
u32 RfidRecvWaitTimeoutProc(void)
{
	u16 u16I = 0x00;

	/*�������״̬�ĳ�ʱ����**/
	if(s_stRfidSendBuf[s_u16RfidSendBufRead].stFrame.u32ID.ID.FrameType == RFID_FT_COLIBARATE)
	{
		//���¼Ӽ����л������ʱ��Ϊ���ܿ�����Ӧ�ٴΰ��¼Ӽ���
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
	/*������һ�ΰ��µĳ�ʱ����*/
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
** ��������: RfidRecvInvalidDataProc
** ����������RFID���ݴ���������Чʱ
** �������룺��
** ���������None
** ע�����
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
		
		#if HARDWARE_TEST//�յ�����֡������Ӧ��
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
			u16DestScNumb = stRxFrame.u8DT[0x02];//ԭ����ܺ�
			u16DestScNumb |= (u16)((stRxFrame.u8DT[0x03]&0x01)<<8);	
			if(u16DestScNumb != u16DestScNumbBackup)
			{
				break;				
			}
			
			u16DestScNumb = (u16)stRxFrame.u8DT[0x03];//����ܺ�
			u16DestScNumb >>= 1;//����ܺ�
			u16DestScNumb |= (u16)((stRxFrame.u8DT[0x04]&0x03)<<7);			
			if(u16DestScNumb != u16DestScNumbBackup)
			{
				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB, &u16DestScNumb);			
				LogicRunInfApi(LOGIC_SET_CONLIBRATE_SUCCESS_SC_NUMB,&u16DestScNumb);				
			}			

			u16DestScNumb = stRxFrame.u8DT[0x06];//����ʱ��
			u16DestScNumb |= (u16)(stRxFrame.u8DT[0x07]<<8);
			u16DestScNumb *= 100;//��λ0.1s
			LogicParamApi(LOGIC_SET_SC_CTRL_DISPALY_TIMER, &u16DestScNumb);	
			
			u16DestScNumb = stRxFrame.u8DT[0x01];//�ܺ�����
			LogicRunInfApi(LOGIC_GET_SC_NUMB_ADD_DIRCT, &u16DestScNumbBackup);	
			if(u16DestScNumb != u16DestScNumbBackup)
				LogicRunInfApi(LOGIC_SET_SC_NUMB_ADD_DIRCT, &u16DestScNumb);
			InterfaceSetUpdateMainMethod(INTERFACE_SHOW_MAIN_ALL);//���¶�����ʾ
			OSSemPost(g_stSemInterface);			
			break;

		case RFID_FT_GRP_ACTION_INF:
			if(stRxFrame.u8DT[0x06] != u8YkqID)
				break;
			u16DestScNumb = stRxFrame.u8DT[0x04];
			u16DestScNumb |= (u16)(stRxFrame.u8DT[0x05]<<8);
			if(u16DestScNumb != u16DestScNumbBackup)
				break;			
			/*��ȡ��Ϣ**/
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

		case RFID_FT_SC_UNCOLIBATARE://������ֱ��Ҫ��������
			if(stRxFrame.u8DLC != 0x08)
				break;
			if(stRxFrame.u8DT[0x04] != u8YkqID)//ң����ID
				break;		
			if(u16DestScNumbBackup != stRxFrame.u8DT[0x03])//����ܺ�
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
				/**����RFIDΪ����״̬***/
				u8I = GetRfidCurStatus();//��ȡоƬ״̬
				if(u8I == CC1101_STATE_RX_OVERFLOW)
					RfidStrobe(CC1101_SFRX);    //��ϴ RX FIFO buffer.
				else if(u8I == CC1101_STATE_TX_UNDERFLOW)
					RfidStrobe(CC1101_SFTX);    //��ϴ TX FIFO buffer.
				else if(u8I == CC1101_STATE_IDLE)
					SetRfidSRX();
				else
					SetRFChipSleepMode();//��оƬ��������ģʽ;
				u16DestScNumb = 0x00;
				while((u8I == CC1101_STATE_RX_OVERFLOW) || (u8I == CC1101_STATE_TX_UNDERFLOW) || (u8I == CC1101_STATE_IDLE))
				{						
					OSTimeDly(RFIDMNG_TASK_TIME_SET/TICK_TIME);
					u16DestScNumb ++;
					u8I = GetRfidCurStatus();//��ȡоƬ״̬
					if((u8I != CC1101_STATE_RX_OVERFLOW) && (u8I != CC1101_STATE_TX_UNDERFLOW) &&(u8I != CC1101_STATE_IDLE))
					{
						SetRFChipSleepMode();//��оƬ��������ģʽ;
						break;
					}
					if(u16DestScNumb >= 10)
					{
						SetRFChipSleepMode();//��оƬ��������ģʽ;
						break;
					}
				}	
			}			
			break;

		#if (KEY_AUTO_PRESS_PROC_FLAG != DISABLED)
			case RFID_FT_AUTOPRESS_ENABLED_ACK://�Զ���ѹ����Ӧ��
			case RFID_FT_AUTOPRESS_DISABLED_ACK://�Զ���ѹ����Ӧ��	
				if(stRxFrame.u8DT[0x06] != u8YkqID)
					break;
				u16DestScNumb = stRxFrame.u8DT[0x04];
				u16DestScNumb |= (u16)(stRxFrame.u8DT[0x05]<<8);
				if(u16DestScNumb != u16DestScNumbBackup)
					break;
				
				if(u16ScLsNumbBackup == stRxFrame.u32ID.ID.LiuShuiNumb)
					return 0x00;
				
				u16ScLsNumbBackup = stRxFrame.u32ID.ID.LiuShuiNumb;	
				
				if((stRxFrame.u8DT[0x07]&0x01) == 0x01)//�Զ���ѹ����Ӧ�����
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
            RfidTestTimer = 300;        //���߲��Գ�ʱʱ��
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
            //����������һ���仯�����ͽ�������ź���
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
** ��������: UsartSendDataProc
** ����������USART�������ݴ���
** �������룺��
** ���������None
** ע�����
**********************************************************************************/
void UsartSendDataProc(void)
{
	INT8U err;
				
	if((s_u16RfidUsartSendBufWrite != s_u16RfidUsartSendBufRead))
	{
		USART_SendData(WL_USART, *((u8*)&s_stRfidUsartSendBuf[s_u16RfidUsartSendBufRead]));
		USART_ITConfig(WL_USART, USART_IT_TXE, ENABLE);		
		OSSemPend(s_stUsartSend,20/TICK_TIME,&err);//�ȴ����ݽ������
		
		if(s_stRfidUsartSendBuf[s_u16RfidUsartSendBufRead].ACK == 1)
		{
			OSSemPend(s_stUsartRecvd, 40/TICK_TIME, &err);//���ͼ��
			if(err == OS_ERR_NONE)//�������ݣ�����
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
** ��������: RfidSendDataProc
** ����������RFID�������ݴ���
** �������룺��
** ���������None
** ע�����
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
							//���¼Ӽ����л������ʱ��Ϊ���ܿ�����Ӧ�ٴΰ��¼Ӽ������ѱ��εķ��ʹ�����СΪ1
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

				if(u32ReturnFlag && (s_stRfidSendBuf[s_u16RfidSendBufRead].u8WaitAckFlag == ACK))//������ȷ���
				{
					u32SendTimer = s_stRfidSendBuf[s_u16RfidSendBufRead].u16SendInterval;
					if((u32SendTimer == RFID_SEND_INTERVAL_CLOSED) || (u32SendTimer == 0x00))
						u32SendTimer= RFID_SEND_INTERVAL_DEFAULT;//��ֹ�������ȵ��������
					#if RFID_TEST
						OSSemPost(s_stRfidRecvd);
					#endif
					OSSemPend(s_stRfidRecvd, u32SendTimer/TICK_TIME, &err);//���ͼ��								
					u32SendTimer = 0x00;
					if(err == OS_ERR_NONE)//�������ݣ�����
					{					
						if(RfidRecvDataProc() == 0x01)//���������ж���ȷ
						{						
							LogicRunInfApi(LOGIC_GET_RUN_STATUS, &u16Status);
							LogicRunInfApi(LOGIC_GET_BUTTON_VALUE, &u32ButtonVale);
							//�ǳ������ʱ���ɽ��յ�������Ӧ������
							//�������ʱ��������Ӧ�𼴿ɽ���
							//�������ʱ�������ڼܼ���ֹͣ�����ɽ���
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
							else//�������ʱ
							{
								u32SendTimer = s_stRfidSendBuf[s_u16RfidSendBufRead].u16SendInterval;
								if((u32SendTimer == RFID_SEND_INTERVAL_CLOSED) || (u32SendTimer == 0x00))
									u32SendTimer= RFID_SEND_INTERVAL_DEFAULT;//��ֹ�������ȵ��������
								OSSemPend(s_stRfidRecvd, u32SendTimer/TICK_TIME, &err);//���ͼ��						
								u32SendTimer = 0x00;
								if(err == OS_ERR_NONE)//�������ݣ�����	
								{
									if(RfidRecvDataProc() == 0x01)//���������ж���ȷ(����Ӧ�𼰳�����Ϣ���ᷢ��һ�Σ������ڶ�η��͵����)
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
					else if(err == OS_ERR_TIMEOUT)//�������ݣ���ʱ
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
							err = GetRfidCurStatus();//��ȡоƬ״̬
							if(err == CC1101_STATE_RX_OVERFLOW)
								RfidStrobe(CC1101_SFRX);    //��ϴ RX FIFO buffer.
							else if(err == CC1101_STATE_TX_UNDERFLOW)
								RfidStrobe(CC1101_SFTX);    //��ϴ TX FIFO buffer.
							else if(err == CC1101_STATE_IDLE)
								SetRfidSRX();
							else
								SetRFChipSleepMode();//��оƬ��������ģʽ;
							u16Status = 0x00;
							while((err == CC1101_STATE_RX_OVERFLOW) || (err == CC1101_STATE_TX_UNDERFLOW) || (err == CC1101_STATE_IDLE))
							{						
								OSTimeDly(RFIDMNG_TASK_TIME_SET/TICK_TIME);
								u16Status ++;
								err = GetRfidCurStatus();//��ȡоƬ״̬
								if((err != CC1101_STATE_RX_OVERFLOW) && (err != CC1101_STATE_TX_UNDERFLOW) &&(err != CC1101_STATE_IDLE))
								{
									SetRFChipSleepMode();//��оƬ��������ģʽ;
									break;
								}
								if(u16Status >= 10)
								{
									SetRFChipSleepMode();//��оƬ��������ģʽ;
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
** ��������: RfidUsartMngInint
** �������������ڱ�����ʼ��
** �������룺*src
** ���������des
** ע�����
**********************************************************************************/
void RfidUsartMngInint(void)
{
	memset(s_stRfidUsartSendBuf,0x00,sizeof(stUSARTDATA)*USART_SEND_BUF_SIZE);
	s_u16RfidUsartSendBufWrite = 0x00;
	s_u16RfidUsartSendBufRead = 0x00;
	u16LiuShuiNumb = 0x00;	
}

/*********************************************************************************
** ��������: UsartRecvInvalidDataProc
** ����������Usart���ݽ��մ�����
** �������룺��
** ���������None
** ע�����
**********************************************************************************/
void UsartRecvInvalidDataProc(void)
{
	stUSARTDATA UsartRcvFrm;	
	if(Usart_GetFrame(&UsartRcvFrm) == 0)
	 return;
	if(UsartRcvFrm.ACK == 0 && UsartRcvFrm.FrameType > 0 && UsartRcvFrm.u8DLC == 1)
	{
		//�������ݲ�Ӧ��
		if(UsartRcvFrm.FrameType == WIRELESSDATA)
		{
			return;
		}
		ACKData	= UsartRcvFrm.SendBuf[0];
		ACKFrameType = UsartRcvFrm.FrameType;
		OSSemPost(s_stUsartRecvd);	//RFID���յ�����
		return;
	}
	switch(UsartRcvFrm.FrameType)
	{
		//����
		case HEARTBEAT:			
			SetHeartBeat(UsartRcvFrm);
			break;	
		//����
		case PARAMETERSETTING:
			SendSetupToWl(RFID_ININT_SYNC1);
			break;	
		default:
			break;
	}			
}


/*********************************************************************************
** ��������: RfidMng_Task
** ����������RFID����������
** �������룺��
** ���������None
** ע�����
**********************************************************************************/
void RfidMng_Task(void *p_arg)
{
	INT8U err;
	RfidUsartMngInint();
	RfidMngInint();

	while(1)
	{	
		OSSemPend(s_stRfidRecvd, RFIDMNG_TASK_TIME_SET/TICK_TIME, &err);//���ͼ��			
		if(err == OS_ERR_NONE)//�������ݣ�����
		{
			RfidRecvInvalidDataProc();//�������ݴ���
		}
		else
		{
			RfidSendDataProc();//�������ݴ���
		}
		
		if(u32SendTimer && (u32SendTimer != RFID_SEND_INTERVAL_CLOSED))
		{
			if(u32SendTimer >= 10)
				u32SendTimer -= 10;
			else
				u32SendTimer = 0x00;
		}
		//usart���ݽ��մ���
		if(g_u32ID != B_type)
				UsartRecvInvalidDataProc();	
	}
}


/********************************************************************************
** �����������������ݴ���������
** �䡡�룺  ��
** �䡡����  ��
*********************************************************************************/
void Usart_Task(void *p_arg)
{	
	while(1)
	{	
		UsartSendDataProc();
		HeartBeatCommErrTimer();
		//ִ������10ms
		OSTimeDly(10/TICK_TIME);
	}
}

