/********************************************************************************
* �ļ����ƣ�RfidUpdate.h
* ��	�ߣ�������   
* ��ǰ�汾��V1.0
* ������ڣ�2017.06.22
* ��������: ����RfidMng.hͷ�ļ�
* ��ʷ��Ϣ��   
*           	�汾��Ϣ     ���ʱ��      ԭ����        ע��
*
*       >>>>  �ڹ����е�λ��  <<<<
*             3-Ӧ�ò�
*          �� 2-Э���
*          	  1-Ӳ��������
*********************************************************************************
* Copyright (c) 2014,������������޹�˾ All rights reserved.
*********************************************************************************/


#ifndef __RFIDUPDATE_H__
#define __RFIDUPDATE_H__

#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/
#include "includes.h"

/********************************************************************************
* #define�궨��
*********************************************************************************/
/**����Ŀ��ʹ�õ��������ݸ�ʽ***/
__packed typedef struct
{
	__packed union
	{
		__packed struct
		{
			unsigned int	SDL:6;				//���շ�ID�����ͷ�ID������6bit��
			unsigned int	FrameType:10;		//֡���
			unsigned int	Reservd:16;			//Ԥ������
		} ID;//֡ID
		u32 u32Id;//֡ID
	} u32ID;	//֡ID
	u8	u8DLC;				//֡����
	u8	u8DT[55];			//֡����
}RFID_UPDATE_FRAME_TYPE;

/**֡���Ͷ���**/
enum
{
	RFID_FT_UPDATE_DWL_VERSION = 0x1EF,//����汾��Ϣ
	RFID_FT_UPDATE_WRITE_VERSION = 0x1EE,//д��汾��Ϣ	
	RFID_FT_UPDATE_DWL_CODE = 0x1ED,//����������
	RFID_FT_UPDATE_WRITE_CODE = 0x1EC,//д��汾��Ϣ
	RFID_FT_UPDATE_END = 0x1EB	//���¹��̽���
};

enum
{
	WL_RFID_FT_UPDATE_DWL_VERSION = 0x2EF,//����汾��Ϣ
	WL_RFID_FT_UPDATE_WRITE_VERSION = 0x2EE,//д��汾��Ϣ	
	WL_RFID_FT_UPDATE_DWL_CODE = 0x2ED,//����������
	WL_RFID_FT_UPDATE_WRITE_CODE = 0x2EC,//д��汾��Ϣ
	WL_RFID_FT_UPDATE_END = 0x2EB	//���¹��̽���
};

//�����ļ�ͬ��
enum
{
	ACTION_INFO_SYNCHRO_DWL_VERSION = 0x1DF,//�����ļ��汾��Ϣ֡
	ACTION_INFO_SYNCHRO_WRITE_VERSION = 0x1DE,//�����ļ��汾��Ϣд��֡	
	ACTION_INFO_SYNCHRO_DWL_CODE = 0x1DD,//�����ļ�����֡
	ACTION_INFO_SYNCHRO_WRITE_CODE = 0x1DC,//�����ļ�����д��֡
	ACTION_INFO_SYNCHRO_END = 0x1DB	//�����ļ�����֡
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
u32  RfidUpdateSendPrgData(u8 u8DevID,u32 u32FrameType,u32 u32PacketNumb,u8 *data,u32 u32DataLength);
void RfidUpdateMngInint(void);
void RfidMng_Update_Task(void *p_arg);

#endif

#endif

