/********************************************************************************
* �ļ����ƣ� IrMng.c
* ��    �ߣ� ������   
* ��    ���� V1.0
* ��    �ڣ� 2015.04.28
* ��������:  ʵ�ֺ����Э��㴦��

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

/********************************************************************************
* ��������
*********************************************************************************/
#define	IRMNG_TASK_TIME_SET			10//������ʱ����
#define	IR_SEND_BUF_SIZE			10
#define IR_DATA_SIZE				3//���ݳ���
/********************************************************************************
* ��������
*********************************************************************************/
extern u16 IRSEND;
static IR_FRAMNE_TYPE	s_stIrSendBuf[IR_SEND_BUF_SIZE];
static u16 s_u16IrSendBufWrite;
static u16 s_u16IrSendBufRead;
static u16 s_u16IrSendTimes[IR_SEND_BUF_SIZE];
#if (SET_YKQ_PRGPT_USE_FLAG != 0)
	static u32 u32VersionTemp;
#endif
/********************************************************************************
* ��������
*********************************************************************************/
/*********************************************************************************
** ��������: IrMngInint
** ����������������������ʼ��
** �������룺��
** ���������None
** ע�����
**********************************************************************************/
void	IrMngInint(void)
{
	u8 u8I = 0x00;

	for(u8I = 0x00; u8I < IR_SEND_BUF_SIZE; u8I ++)
	{
		s_stIrSendBuf[u8I].u16UserID.u16Id = 0x00;
		s_stIrSendBuf[u8I].u8UserData.u8Data = 0x00;
		s_u16IrSendTimes[u8I] = 0x00;
	}
	
	s_u16IrSendBufWrite = 0x00;
	s_u16IrSendBufRead = 0x00;	
}

/*********************************************************************************
** ��������: IrSendCalibrateCodeInterface
** �������������Ͷ�������
** �������룺��
** ���������None
** ע�����
**********************************************************************************/
u32 IrSendCalibrateCodeInterface(u8 u8Ykq_id)
{
	u16 u16TxBufWriteBackup = 0x00;
	u32 u32ReturnFlag = 0x01;
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
			OS_CPU_SR  cpu_sr = 0;
	#endif
	
	OS_ENTER_CRITICAL(); 
	
	/**��ֹ���������������**/
	u16TxBufWriteBackup = s_u16IrSendBufWrite;
	u16TxBufWriteBackup++;
	u16TxBufWriteBackup %= IR_SEND_BUF_SIZE;
	
	if((u16TxBufWriteBackup != s_u16IrSendBufRead))
	{
		s_stIrSendBuf[s_u16IrSendBufWrite].u16UserID.ID.ActType = IR_ACT_YKQ;
		s_stIrSendBuf[s_u16IrSendBufWrite].u16UserID.ID.ScNumbLow = 0x00;
		s_stIrSendBuf[s_u16IrSendBufWrite].u16UserID.ID.Flag = IR_FLAG_INVALID;
		
		s_stIrSendBuf[s_u16IrSendBufWrite].u16UserID.ID.ScNumbHigh = YKQ_TARGET_USE_LOCATION_DEFINE-0x01;
		s_stIrSendBuf[s_u16IrSendBufWrite].u16UserID.ID.SendDirection = IR_DIRECTION_YKQ;
		s_stIrSendBuf[s_u16IrSendBufWrite].u16UserID.ID.Resverd = 0;

		s_stIrSendBuf[s_u16IrSendBufWrite].u8UserData.DATA.DeviceCommFlag = IR_COMM_STATUS_ASK;
		s_stIrSendBuf[s_u16IrSendBufWrite].u8UserData.DATA.Ack = IR_ACK;
		s_stIrSendBuf[s_u16IrSendBufWrite].u8UserData.DATA.DeviceId = u8Ykq_id;
		s_stIrSendBuf[s_u16IrSendBufWrite].u8UserData.DATA.ScNumbAddDirection = SC_DIRCT_RIGHT;
		s_stIrSendBuf[s_u16IrSendBufWrite].u8UserData.DATA.Resverd = 0;

		s_stIrSendBuf[s_u16IrSendBufWrite].u8UserData.DATA.ScNumbAddDirection = YKQ_ALL_FUNCTION;
		
		s_u16IrSendTimes[s_u16IrSendBufWrite] = 0x00;
		
		s_u16IrSendBufWrite++;
		s_u16IrSendBufWrite %= IR_SEND_BUF_SIZE;        	
	}
	else
	{
		u32ReturnFlag =  0x00;//�����������������ټ��������
	}
	
	OS_EXIT_CRITICAL();	
	
	return u32ReturnFlag;	
	
}

/*********************************************************************************
** ��������: IrSendCalibrateCodeInterface
** �������������Ͷ�������
** �������룺��
** ���������None
** ע�����
**********************************************************************************/
u32 IrSendSearchYkqCodeInterface(u8 u8Ykq_id)
{
	u16 u16TxBufWriteBackup = 0x00;
	u32 u32ReturnFlag = 0x01;
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
			OS_CPU_SR  cpu_sr = 0;
	#endif
	
	OS_ENTER_CRITICAL(); 
	
	/**��ֹ���������������**/
	u16TxBufWriteBackup = s_u16IrSendBufWrite;
	u16TxBufWriteBackup++;
	u16TxBufWriteBackup %= IR_SEND_BUF_SIZE;
	
	if((u16TxBufWriteBackup != s_u16IrSendBufRead))
	{
		s_stIrSendBuf[s_u16IrSendBufWrite].u16UserID.ID.ActType = IR_ACT_SEARCH_YKQ_ID;
		s_stIrSendBuf[s_u16IrSendBufWrite].u16UserID.ID.ScNumbLow = (u8Ykq_id&0x03);
		s_stIrSendBuf[s_u16IrSendBufWrite].u16UserID.ID.Flag = IR_FLAG_INVALID;
		
		s_stIrSendBuf[s_u16IrSendBufWrite].u16UserID.ID.ScNumbHigh = (u8Ykq_id&0xFC);
		s_stIrSendBuf[s_u16IrSendBufWrite].u16UserID.ID.SendDirection = IR_DIRECTION_SC;
		s_stIrSendBuf[s_u16IrSendBufWrite].u16UserID.ID.Resverd = 0;

		s_stIrSendBuf[s_u16IrSendBufWrite].u8UserData.DATA.DeviceCommFlag = IR_COMM_STATUS_ASK;
		s_stIrSendBuf[s_u16IrSendBufWrite].u8UserData.DATA.Ack = IR_ACK;
		s_stIrSendBuf[s_u16IrSendBufWrite].u8UserData.DATA.DeviceId = u8Ykq_id;
		s_stIrSendBuf[s_u16IrSendBufWrite].u8UserData.DATA.Resverd = 0;
		s_stIrSendBuf[s_u16IrSendBufWrite].u8UserData.DATA.ScNumbAddDirection = YKQ_ALL_FUNCTION;
		
		s_u16IrSendTimes[s_u16IrSendBufWrite] = 0x00;
		
		s_u16IrSendBufWrite++;
		s_u16IrSendBufWrite %= IR_SEND_BUF_SIZE;        	
	}
	else
	{
		u32ReturnFlag =  0x00;//�����������������ټ��������
	}
	
	OS_EXIT_CRITICAL();	
	
	return u32ReturnFlag;	
	
}

/*********************************************************************************
** ��������: IrSendCalibrateCodeInterface
** �������������Ͷ�������
** �������룺��
** ���������None
** ע�����
**********************************************************************************/
u32 IrSendSetYkqIdCodeInterface(u8 u8Ykq_id)
{
	u16 u16TxBufWriteBackup = 0x00;
	u32 u32ReturnFlag = 0x01;
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
			OS_CPU_SR  cpu_sr = 0;
	#endif
	
	OS_ENTER_CRITICAL(); 
	
	/**��ֹ���������������**/
	u16TxBufWriteBackup = s_u16IrSendBufWrite;
	u16TxBufWriteBackup++;
	u16TxBufWriteBackup %= IR_SEND_BUF_SIZE;
	
	if((u16TxBufWriteBackup != s_u16IrSendBufRead))
	{
		s_stIrSendBuf[s_u16IrSendBufWrite].u16UserID.ID.ActType = IR_ACT_SET_YKQ_ID;
		s_stIrSendBuf[s_u16IrSendBufWrite].u16UserID.ID.ScNumbLow = 0x03;
		s_stIrSendBuf[s_u16IrSendBufWrite].u16UserID.ID.Flag = IR_FLAG_INVALID;
		
		s_stIrSendBuf[s_u16IrSendBufWrite].u16UserID.ID.ScNumbHigh = 0x00;
		s_stIrSendBuf[s_u16IrSendBufWrite].u16UserID.ID.SendDirection = IR_DIRECTION_SC;
		s_stIrSendBuf[s_u16IrSendBufWrite].u16UserID.ID.Resverd = 0;

		s_stIrSendBuf[s_u16IrSendBufWrite].u8UserData.DATA.DeviceCommFlag = IR_COMM_STATUS_ASK;
		s_stIrSendBuf[s_u16IrSendBufWrite].u8UserData.DATA.Ack = IR_ACK;
		s_stIrSendBuf[s_u16IrSendBufWrite].u8UserData.DATA.DeviceId = u8Ykq_id;
		s_stIrSendBuf[s_u16IrSendBufWrite].u8UserData.DATA.Resverd = 0;
		s_stIrSendBuf[s_u16IrSendBufWrite].u8UserData.DATA.ScNumbAddDirection = YKQ_ALL_FUNCTION;
		
		s_u16IrSendTimes[s_u16IrSendBufWrite] = 0x00;
		
		s_u16IrSendBufWrite++;
		s_u16IrSendBufWrite %= IR_SEND_BUF_SIZE;        	
	}
	else
	{
		u32ReturnFlag =  0x00;//�����������������ټ��������
	}
	
	OS_EXIT_CRITICAL();	
	
	return u32ReturnFlag;	
	
}


/*********************************************************************************
** ��������: IrRecvDataProc
** ��������������������ݴ���
** �������룺��
** ���������None
** ע�����
**********************************************************************************/
u32 IrRecvDataProc(void)
{
	u32 u32ReturnFlag = 0x00;
	IR_FRAMNE_TYPE sFrameTemp;
	u32 u32RxSizeBackup = 0x00;
	u32 u32WritePtrBackup = 0x00;
	u32 u32ReadPtrBackup = 0x00;
    u32 u32I = 0x00;
	u8 u8DevNumb = 0x00;
	u16 u16ScNumb = 0x00;
	u16 u16ScNumbAddDirc = 0x00;
	u16 u16Temp;
	#if (SET_YKQ_PRGPT_USE_FLAG != 0)
		char s[0x09];
		char s2[0x02];
	#endif
	
	IrUsartRxBufSizeGet(&u32RxSizeBackup);
	IrUsartRxRdRtrGet(&u32ReadPtrBackup);
	IrUsartRxWtrRtrGet(&u32WritePtrBackup);
	if(u32WritePtrBackup > u32ReadPtrBackup)
		u32I = u32WritePtrBackup - u32ReadPtrBackup;
	else		
		u32I = u32ReadPtrBackup - u32WritePtrBackup;
	if(u32I < IR_DATA_SIZE)//���ݳ��Ȳ���
	{
		IrUsartRxRdRtrSet(&u32WritePtrBackup);	//���ٶ����ݽ��д���
		return u32ReturnFlag;
	}

	LogicRunInfApi(LOGIC_GET_RUN_STATUS, &u16Temp);//��ȡ��ǰ����״̬				
	if(u16Temp == LOGIC_STATUS_IR_COLIBARAE_ING)
	{
		/**���ݳ�����ȷ����ȡ���ݲ����з���(ֻ��ȡ��һӦ��֡����Ч����)**/
		while((u32ReadPtrBackup != u32WritePtrBackup) )
		{
			u32I = u32ReadPtrBackup;
			if(u32I == u32WritePtrBackup)//�����ݿ��Զ�ȡ
			{
				IrUsartRxRdRtrSet(&u32I);	
				break;
			}		
			IrUsartReadRxBuf(&u8DevNumb,u32I);
			sFrameTemp.u16UserID.u16Id = u8DevNumb;
			u32I ++;
			u32I %= u32RxSizeBackup;
			if(u32I == u32WritePtrBackup)//�����ݿ��Զ�ȡ
			{
				IrUsartRxRdRtrSet(&u32I);	
				break;
			}
			IrUsartReadRxBuf((u8 *)&u16ScNumb,u32I);
			sFrameTemp.u16UserID.u16Id |= (u16)(u16ScNumb<<8);
			u32I ++;
			u32I %= u32RxSizeBackup;
			if(u32I == u32WritePtrBackup)//�����ݿ��Զ�ȡ
			{
				IrUsartRxRdRtrSet(&u32I);	
				break;
			}		
			IrUsartReadRxBuf(&u8DevNumb,u32I);	
			sFrameTemp.u8UserData.u8Data = u8DevNumb;
			u32I++;
			u32I %= u32RxSizeBackup;
			IrUsartRxRdRtrSet(&u32ReadPtrBackup);	//������������Ķ�ָ��
			if(sFrameTemp.u16UserID.ID.Flag != IR_FLAG_INVALID)
			{
				u32ReadPtrBackup++;
				u32ReadPtrBackup %= u32RxSizeBackup;
				continue;
			}
			if(sFrameTemp.u16UserID.ID.ActType != IR_ACT_SC)
			{
				u32ReadPtrBackup++;
				u32ReadPtrBackup %= u32RxSizeBackup;
				continue;
			}
			
			if(sFrameTemp.u16UserID.ID.SendDirection != IR_DIRECTION_SC)
			{
				u32ReadPtrBackup++;
				u32ReadPtrBackup %= u32RxSizeBackup;
				continue;
			}
			
			LogicParamApi(LOGIC_GET_YKQ_NUMBER, &u8DevNumb);
			if(sFrameTemp.u8UserData.DATA.DeviceId != u8DevNumb)
			{
				u32ReadPtrBackup++;
				u32ReadPtrBackup %= u32RxSizeBackup;
				continue;
			}	
			
			if(sFrameTemp.u8UserData.DATA.ScNumbAddDirection >= SC_DIRCT_MAX)
			{
				u32ReadPtrBackup++;
				u32ReadPtrBackup %= u32RxSizeBackup;
				continue;
			}
			
			u16ScNumb = 0x00;
			u16ScNumb = sFrameTemp.u16UserID.ID.ScNumbLow;
			u16ScNumb |= (sFrameTemp.u16UserID.ID.ScNumbHigh << 3);
			if((u16ScNumb > IN_SC_SS_NUMB) || (u16ScNumb < LOGIC_SC_NUMB_MIN))
			{
				u32ReadPtrBackup++;
				u32ReadPtrBackup %= u32RxSizeBackup;
				continue;
			}
			
			/**������Ч***/
			LogicRunInfApi(LOGIC_GET_RUN_STATUS, &u16Temp);//��ȡ��ǰ����״̬				
			if(u16Temp == LOGIC_STATUS_IR_COLIBARAE_ING)
			{		
				StateLed(ADJUST_CODE_ING_LED,0x00);
				u16ScNumbAddDirc = sFrameTemp.u8UserData.DATA.ScNumbAddDirection;
				
				LogicRunInfApi(LOGIC_SET_SC_NUMB_ADD_DIRCT, &u16ScNumbAddDirc);
				LogicRunInfApi(LOGIC_SET_COLIBRATE_SC_NUMB, &u16ScNumb);
				LogicRunInfApi(LOGIC_SET_CONLIBRATE_SUCCESS_SC_NUMB,&u16ScNumb);						
				
				LogicRunInfApi(LOGIC_SET_CONTRL_SC_NUMB,&u16ScNumb);	
				u16ScNumb = LOGIC_STATUS_IR_COLIBARAE_SUCCESS;
				LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u16ScNumb);//���ò����ɹ�
			

				IrUsartRxWtrRtrGet(&u32WritePtrBackup);
				IrUsartRxRdRtrSet(&u32WritePtrBackup);	//�������������дָ�룬����֮����յ����ݲ��ٴ���

				u32ReturnFlag = 0x01;
			}
			break;
		}
	}
	#if (SET_YKQ_PRGPT_USE_FLAG != 0)
		else if(u16Temp == LOGIC_STATUS_IR_SEARCH_YQK_ING)
		{
			/**���ݳ�����ȷ����ȡ���ݲ����з���(ֻ��ȡ��һӦ��֡����Ч����)**/
			while((u32ReadPtrBackup != u32WritePtrBackup) )
			{
				u32I = u32ReadPtrBackup;
				if(u32I == u32WritePtrBackup)//�����ݿ��Զ�ȡ
				{
					IrUsartRxRdRtrSet(&u32I);	
					break;
				}		
				IrUsartReadRxBuf(&u8DevNumb,u32I);
				sFrameTemp.u16UserID.u16Id = u8DevNumb;
				u32I ++;
				u32I %= u32RxSizeBackup;
				if(u32I == u32WritePtrBackup)//�����ݿ��Զ�ȡ
				{
					IrUsartRxRdRtrSet(&u32I);	
					break;
				}
				IrUsartReadRxBuf((u8 *)&u16ScNumb,u32I);
				sFrameTemp.u16UserID.u16Id |= (u16)(u16ScNumb<<8);
				u32I ++;
				u32I %= u32RxSizeBackup;
				if(u32I == u32WritePtrBackup)//�����ݿ��Զ�ȡ
				{
					IrUsartRxRdRtrSet(&u32I);	
					break;
				}		
				IrUsartReadRxBuf(&u8DevNumb,u32I);	
				sFrameTemp.u8UserData.u8Data = u8DevNumb;
				u32I++;
				u32I %= u32RxSizeBackup;
				IrUsartRxRdRtrSet(&u32ReadPtrBackup);	//������������Ķ�ָ��
				if(sFrameTemp.u16UserID.ID.Flag != IR_FLAG_INVALID)
				{
					u32ReadPtrBackup++;
					u32ReadPtrBackup %= u32RxSizeBackup;
					continue;
				}
				if((sFrameTemp.u16UserID.ID.ActType != IR_ACT_SET_YKQ_ID)&&(sFrameTemp.u16UserID.ID.ActType != IR_ACT_SEARCH_YKQ_ID))
				{
					u32ReadPtrBackup++;
					u32ReadPtrBackup %= u32RxSizeBackup;
					continue;
				}			
			
				if(sFrameTemp.u8UserData.DATA.ScNumbAddDirection >= SC_DIRCT_MAX)
				{
					u32ReadPtrBackup++;
					u32ReadPtrBackup %= u32RxSizeBackup;
					continue;
				}
				
				/**������Ч***/
				LogicRunInfApi(LOGIC_GET_RUN_STATUS, &u16Temp);//��ȡ��ǰ����״̬				
				if(u16Temp == LOGIC_STATUS_IR_SEARCH_YQK_ING)
				{		
					if(sFrameTemp.u16UserID.ID.ActType == IR_ACT_SET_YKQ_ID)
					{
						LogicParamApi(LOGIC_GET_COLIBRATE_YKQ_ID, &u16ScNumb);	
						if((sFrameTemp.u16UserID.ID.SendDirection == IR_DIRECTION_YKQ)&&(u16ScNumb == sFrameTemp.u8UserData.DATA.DeviceId))
						{
							OSSemPost(g_stLogicSem);
							u32ReturnFlag = 0x01;	
						}
					}
					else if(sFrameTemp.u16UserID.ID.ActType == IR_ACT_SEARCH_YKQ_ID)//�����豸
					{
						StateLed(ADJUST_CODE_ING_LED,0x00);
						if(sFrameTemp.u8UserData.DATA.DeviceCommFlag == IR_COMM_STATUS_ANSWER)
						{
							u16ScNumb = sFrameTemp.u8UserData.DATA.DeviceId;
							LogicParamApi(LOGIC_SET_COLIBRATE_YKQ_ID, &u16ScNumb);	
							u16ScNumb = sFrameTemp.u8UserData.DATA.ScNumbAddDirection;
							LogicParamApi(LOGIC_SET_COLIBRATE_YKQ_TYPE, &u16ScNumb);
							u16ScNumb = sFrameTemp.u16UserID.ID.ScNumbHigh;
							LogicParamApi(LOGIC_SET_COLIBRATE_YKQ_STATION, &u16ScNumb);	
							u32VersionTemp = 0x00;
							u32VersionTemp =  sFrameTemp.u16UserID.ID.ScNumbLow;
							u32VersionTemp |= (sFrameTemp.u16UserID.ID.SendDirection<<3);
							u32ReturnFlag = 0x01;
						}
						else
						{
							u32VersionTemp |= (sFrameTemp.u16UserID.ID.ScNumbLow<<4);
							u32VersionTemp |= (sFrameTemp.u16UserID.ID.ScNumbHigh<<7);
							u32VersionTemp |= (sFrameTemp.u16UserID.ID.SendDirection<<13);
							u32VersionTemp |= (sFrameTemp.u8UserData.DATA.ScNumbAddDirection<<14);
							u32VersionTemp |= (sFrameTemp.u8UserData.DATA.Resverd<<15);
							memset(s,0x00,sizeof(s));
							LogicParamApi(LOGIC_GET_COLIBRATE_YKQ_TYPE, &u16ScNumb);
							if(u16ScNumb == YKQ_SIMPLE_FUNCTION)
								strcpy(s,"V3.");
							else
								strcpy(s,"V2.");
							u16ScNumb =(u32VersionTemp%26);
							u16ScNumb = 'a'+u16ScNumb;//Ӣ����ĸ
							u32VersionTemp /= 26;
							u16ScNumbAddDirc = (u32VersionTemp/100);//V2	
							u16Temp = ((u32VersionTemp/10)%10);//V3
							u32I = 0x00;
							u32I = (u32VersionTemp%10);//V4
							memset(s2,0x00,sizeof(s2));
							sprintf(s2,"%d",u16ScNumbAddDirc);//V2
							strcat(s,s2);
							strcat(s,".");
							
							memset(s2,0x00,sizeof(s2));//V3
							sprintf(s2,"%d",u16Temp);
							strcat(s,s2);
							strcat(s,".");

							memset(s2,0x00,sizeof(s2));//V4
							sprintf(s2,"%d",u32I);
							strcat(s,s2);
							
							memset(s2,0x00,sizeof(s2));//Ӣ����ĸ
							sprintf(s2,"%c",u16ScNumb);
							strcat(s,s2);		
							LogicParamApi(LOGIC_SET_COLIBRATE_YKQ_VERSION, s);	
							InterfaceSetCtrlMode(INTERFACE_MODE_COLIBRATE_CODE_SUCCESS);
							OSSemPost(g_stLogicSem);
							u32ReturnFlag = 0x01;
						}
					}
		
					IrUsartRxWtrRtrGet(&u32WritePtrBackup);
					IrUsartRxRdRtrSet(&u32WritePtrBackup);	//�������������дָ�룬����֮����յ����ݲ��ٴ���

					u32ReturnFlag = 0x01;
				}
				break;
			}		
		}
	#endif
	else
	{
		IrUsartRxWtrRtrGet(&u32WritePtrBackup);
		IrUsartRxRdRtrSet(&u32WritePtrBackup);	//�������������дָ�룬����֮����յ����ݲ��ٴ���
		
	}
	return u32ReturnFlag;
	
}

/*********************************************************************************
** ��������: IrMngIdleRecvDataProc
** �����������������״̬�������ݴ���
** �������룺��
** ���������None
** ע�����
**********************************************************************************/
void IrMngIdleRecvDataProc(void)
{
	u32 u32WritePtrBackup = 0x00;
	u32 u32ReadPtrBackup = 0x00;
	
	IrUsartRxWtrRtrGet(&u32WritePtrBackup);
	IrUsartRxRdRtrGet(&u32ReadPtrBackup);

	IrUsartRxRdRtrSet(&u32WritePtrBackup);	//�������������дָ�룬����֮����յ����ݲ��ٴ���
	
}
/*********************************************************************************
** ��������: IrMngSendDataProc
** �������������ⷢ�����ݴ���
** �������룺��
** ���������None
** ע�����
**********************************************************************************/
void IrMngSendDataProc(void)
{
	u32 u32ReturnFlag = 0x00;
	u16 u16DelayTime = 0x00,u16Temp;
	u16 u16SendTimes = 0x00;
	INT8U err;
	
	if(s_u16IrSendBufRead != s_u16IrSendBufWrite)
	{
		LogicRunInfApi(LOGIC_GET_RUN_STATUS, &u16Temp);		
		
		#if (SET_YKQ_PRGPT_USE_FLAG != 0)
			if((u16Temp != LOGIC_STATUS_IR_COLIBARAE_ING)&&(u16Temp != LOGIC_STATUS_IR_SEARCH_YQK_ING))//�����ͺ����״̬�Ѿ��ı�ʱ�����ٽ��к������ݵķ���
		#else
			if(u16Temp != LOGIC_STATUS_IR_COLIBARAE_ING)//�����ͺ����״̬�Ѿ��ı�ʱ�����ٽ��к������ݵķ���
		#endif
			{
				s_u16IrSendBufRead = s_u16IrSendBufWrite;
				return ;
			}			
			
		LogicParamApi(LOGIC_GET_IR_SENDTIMES, &u16SendTimes);
		if(u16SendTimes > LOGIC_SEND_TIMES_MAX)
			u16SendTimes = LOGIC_SEND_TIMES_MAX;
		else if(u16SendTimes <LOGIC_SEND_TIMES_MIN)
			u16SendTimes = LOGIC_SEND_TIMES_MIN;
		else
			;
		
		#if (SET_YKQ_PRGPT_USE_FLAG != 0)
			if(u16Temp == LOGIC_STATUS_IR_SEARCH_YQK_ING)
			{
				u16SendTimes = LOGIC_SEND_TIMES_MIN;//ֻ��Ӧ��һ��
			}
		#endif
			
		if(s_u16IrSendTimes[s_u16IrSendBufRead] < u16SendTimes)
		{
			u32ReturnFlag = IrSendInsertTxBuf(s_stIrSendBuf[s_u16IrSendBufRead].u16UserID.u16Id,s_stIrSendBuf[s_u16IrSendBufRead].u8UserData.u8Data);
			if(u32ReturnFlag == 0x00)//����δ���
			{
				s_u16IrSendTimes[s_u16IrSendBufRead]++;
				LogicParamApi(LOGIC_GET_IR_TINMEOUT, &u16DelayTime);
				u16DelayTime -= IRMNG_TASK_TIME_SET;//������ʱʱ��
				if(u16DelayTime < LOGIC_SEND_TIMEOUT_MIN)//�ȴ�ʱ��Ϊ0ʱ����ֹ�������ȵ��������
					u16DelayTime = LOGIC_SEND_TIMEOUT_MIN;
				else if(u16DelayTime > LOGIC_SEND_TIMEOUT_MAX)
					u16DelayTime = LOGIC_SEND_TIMEOUT_MAX;
				else
					;
				OSTimeDly(u16DelayTime/TICK_TIME);//�ȴ����ݽ������					
			}
			else//������������
			{
                IrUsartRxEnable();//��������ʹ��
				s_u16IrSendTimes[s_u16IrSendBufRead]++;					
				LogicParamApi(LOGIC_GET_IR_TINMEOUT, &u16DelayTime);
				u16DelayTime -= IRMNG_TASK_TIME_SET;//������ʱʱ��
				if(u16DelayTime == 0)//�ȴ�ʱ��Ϊ0ʱ����ֹ�������ȵ��������
					u16DelayTime = LOGIC_SEND_TIMEOUT_MAX;					
				OSSemPend(s_stIrRecvd,u16DelayTime/TICK_TIME,&err);//�ȴ����ݽ������
				if(err == OS_ERR_NONE)
				{
					while(1)
					{
						OSSemPend(s_stIrRecvd, USART_SEND_CHAR_INTERVAL, &err);
						if(err == OS_ERR_TIMEOUT)//�������
						{
							if(IrRecvDataProc() == 0x01)//���������ж���ȷ
							{
								#if (SET_YKQ_PRGPT_USE_FLAG != 0)
									if((s_stIrSendBuf[s_u16IrSendBufRead].u16UserID.ID.ActType == IR_ACT_YKQ)||(s_stIrSendBuf[s_u16IrSendBufRead].u16UserID.ID.ActType == IR_ACT_SET_YKQ_ID))
									{
										s_u16IrSendBufRead++;
										s_u16IrSendBufRead %= IR_SEND_BUF_SIZE;										
									}
									else if(s_stIrSendBuf[s_u16IrSendBufRead].u16UserID.ID.ActType == IR_ACT_SEARCH_YKQ_ID)
									{
										OSSemPend(s_stIrRecvd,u16DelayTime/TICK_TIME,&err);//�ȴ����ݽ������
										while(1)
										{
											OSSemPend(s_stIrRecvd, USART_SEND_CHAR_INTERVAL, &err);
											if(err == OS_ERR_TIMEOUT)//�������
											{
												if(IrRecvDataProc() == 0x01)//���������ж���ȷ
												{
													s_u16IrSendBufRead++;
													s_u16IrSendBufRead %= IR_SEND_BUF_SIZE;													
												}
												break;	
											}
										}
									}
									else
									{
										s_u16IrSendBufRead++;
										s_u16IrSendBufRead %= IR_SEND_BUF_SIZE;										
										
									}
								#else
									s_u16IrSendBufRead++;
									s_u16IrSendBufRead %= IR_SEND_BUF_SIZE;
								#endif
							}                               
							break;								
						}                            
					}
				}
                IrUsartRxDisable();//�رս���ʹ��
			}
		}
		else
		{
			s_u16IrSendBufRead++;
			s_u16IrSendBufRead %= IR_SEND_BUF_SIZE;
			LogicRunInfApi(LOGIC_GET_RUN_STATUS, &u16SendTimes);//���ò���ʧ��		
			if(u16SendTimes == LOGIC_STATUS_IR_COLIBARAE_ING)
			{
				LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u16SendTimes);//���ò���ʧ��z	
				u16SendTimes = LOGIC_STATUS_IR_COLIBARAE_FAIL;
				LogicRunInfApi(LOGIC_SET_RUN_STATUS, &u16SendTimes);//���ò���ʧ��z				
				InterfaceSetCtrlMode(INTERFACE_MODE_COLIBRATE_CODE_FAIL);	
				OSSemPost(g_stLogicSem);			
			}
		}
	}
}
/*********************************************************************************
** ��������: IrMng_Task
** �����������������������
** �������룺��
** ���������None
** ע�����
**********************************************************************************/
void IrMng_Task(void *p_arg)
{
	INT8U err;
	
    IrMngInint();
	while(1)
	{
		OSSemPend(s_stIrRecvd,IRMNG_TASK_TIME_SET/TICK_TIME,&err);//�ȴ����ݽ������
		if(IRSEND)
		{
			IrLOCSendInsertTxBuf(0x0001,0x56);
			OSTimeDly(10/TICK_TIME);
		}
		if(err ==OS_ERR_NONE)
		{
			IrMngIdleRecvDataProc();
		}
		else
		{
			IrMngSendDataProc();
		}
	}
}

