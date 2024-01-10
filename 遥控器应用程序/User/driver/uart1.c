/********************************************************************************
* �ļ�����	usart.c
* ���ߣ�	������   
* �汾��   	V1.0
* ���ڣ�    2015.04.24
* ��������:  ʵ��UART��������ʵ�ֺ������ݽ��յĵײ�������

* �޸�˵����   
*
*       >>>>  �ڹ����е�λ��  <<<<
*             3-Ӧ�ò�
*             2-Э���
*         ��   1-Ӳ��������
*********************************************************************************
* @copy
* <h2><center>&copy; COPYRIGHT ������������޹�˾ �з����� �����</center></h2>
*********************************************************************************/
/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/
#include "includes.h"

/********************************************************************************
* #define�궨��
*********************************************************************************/
/********************************************************************************
* ��������
*********************************************************************************/
#define  USART_TMP_BUF_MAX            160    //��ʱ�����������ֵ
#define  USART_RX_BUF_MAX              64   //���ն������ֵ

uint8_t  UsartReveiveDataBuf[USART_TMP_BUF_MAX];   //��ʱ��������

uint8_t  tmpTxWrtPtr  = 0;  //��ʱ����дָ��

stUSARTDATA  USARTKEYRxBuf[USART_RX_BUF_MAX];						//���ջ���
uint8_t	 USARTKEYRxWrtPtr = 0;			   						    //����дָ��
uint8_t	 USARTKEYRxRdPtr =  0;			   							//���ն�ָ��

static uint16_t  u16UsartSendPtr;

extern st_RFIDRcvFrame st_RFIDRcvFrmPool[];		// RFID�������ݴ洢��
extern u32 u32RcvFrmPoolWritePtr;
extern u32 u32RcvFrmPoolCnt;

extern OS_EVENT *s_stUsartSend ;   //���ڽ����ź���

extern stUSARTDATA	s_stRfidUsartSendBuf[];
extern u16 s_u16RfidUsartSendBufRead;

extern u32 WlHeartTime;

/********************************************************************************
* ��������
*********************************************************************************/


u16 const u16CrcUsarttab[256] = 
{
0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};

/********************************************************************************
* ��������
*********************************************************************************/

/*******************************************************************************************
**�������ã�����CRC16У��ֵ
**����������u8Buf:��ҪУ���������ʼ��ַ��u32Len:��ҪУ�����ݵĳ��ȣ�*u16CheckOld:������CRCУ��ֵ
**�����������
**ע�������
*******************************************************************************************/
void Crc16UsartSendChick(const u8 *u8Buf, u32 u32Len, u16 *u16CheckOld)
{
	u32 u32Cnt = 0x00;
	u16 u16Crc = *u16CheckOld;
	
	for( u32Cnt = 0; u32Cnt < u32Len; u32Cnt++)
		u16Crc = (u16Crc<<8) ^ u16CrcUsarttab[((u16Crc>>8) ^ *(u8 *)u8Buf++)&0x00FF];
	*u16CheckOld = u16Crc;
}
/*******************************************************************************************
**�������ã�����CRC16У��ֵ
**����������u8Buf:��ҪУ���������ʼ��ַ��u32Len:��ҪУ�����ݵĳ��ȣ�*u16CheckOld:������CRCУ��ֵ
**�����������
**ע�������
*******************************************************************************************/
void Crc16UsartReceiveChick(const u8 *u8Buf, u32 u32Len, u16 *u16CheckOld)
{
	u32 u32Cnt = 0x00;
	
	u16 u16Crc = *u16CheckOld;
	
	for( u32Cnt = 0; u32Cnt < u32Len; u32Cnt++)
		u16Crc = (u16Crc<<8) ^ u16CrcUsarttab[((u16Crc>>8) ^ *(u8 *)u8Buf++)&0x00FF];
	*u16CheckOld = u16Crc;
}
/***********************************************************************************************
** �� �� ����	RFID_GetFrame
** ����������	�ӽ��ջ�����ȡ��һ�����ݰ�
** �䡡  �룺	
** �䡡  ����	
** ����  �ߣ�	����
** �ա�  �ڣ�	2014.12.26
** ��    ����	V1.0.0
** ���¼�¼��
** ���¼�¼��
** 					��    ��      ��    ��                    ��      ��
** 					==========  =============  ========================================
**
************************************************************************************************/
u32 Usart_GetFrame(stUSARTDATA *UsartcvFrm)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr = 0;
	#endif
	if(USARTKEYRxRdPtr != USARTKEYRxWrtPtr)
	{
		OS_ENTER_CRITICAL();//��ȫ���ж�
		*UsartcvFrm = USARTKEYRxBuf[USARTKEYRxRdPtr];
		USARTKEYRxRdPtr++;
		USARTKEYRxRdPtr %= USART_RX_BUF_MAX;
		
		OS_EXIT_CRITICAL();//��ȫ���ж�
		return 1;
	}
	else
	{
		return 0;
	}
}
/**********************************************************************************************************
*	�� �� ��: WlUsartInit
*	����˵��: WlUsart�豸��ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
**********************************************************************************************************/
void WlUsartInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;   
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
		
	RCC_APB1PeriphClockCmd(RCC_WL_USART_TX_GPIO, ENABLE);//���ڶ˿�ʱ��ʹ��
	WL_USART_APBxCmd();	
	
  /* Configure USART2 Tx as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//GPIO_Mode_AF
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_Pin = WL_USART_TX_PIN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(WL_USART_TX_GPIO, &GPIO_InitStructure);

	GPIO_PinAFConfig(WL_USART_TX_GPIO,WL_USART_TX_PinSource,WL_USART_PIN_AF_SET); //fuyong

	/* Configure USART2 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = WL_USART_RX_PIN;
	GPIO_Init(WL_USART_RX_GPIO, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(WL_USART_RX_GPIO,WL_USART_RX_PinSource,WL_USART_PIN_AF_SET); //����
	
	USART_InitStructure.USART_BaudRate = WL_USART_BAUNDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;		//��У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;//|USART_Mode_Tx; 
	USART_Init(WL_USART, &USART_InitStructure);				//	

	/* Configure the NVIC Preemption Priority Bits */  
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = WL_USART_IRQN;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ʹ���ж����ȼ�Ƕ�ס���ΪSysTick���ж����ȼ�Ϊ0x0f
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(WL_USART, ENABLE);						//����ʹ��
	USART_ITConfig(WL_USART, USART_IT_RXNE, ENABLE);	//�ж�ʹ��
}


/**********************************************************************************************************
*	�� �� ��: WL_USART_IRQHandler
*	����˵��: �жϴ���
*	��    ��: ��
*	�� �� ֵ: ��
**********************************************************************************************************/
void WL_USART_IRQHandler(void)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
			OS_CPU_SR  cpu_sr = 0;
	#endif

	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
	OSIntEnter();
	OS_EXIT_CRITICAL();
	//usart����
	if(USART_GetITStatus(WL_USART, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(WL_USART, USART_IT_RXNE);
		UsartReveiveDataBuf[tmpTxWrtPtr] = (u8)WL_USART->DR;
		//�������ݵ�һ����TIM4
		if(tmpTxWrtPtr == 0)
		{
			TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4
		}
		//���յ��������������������
		else
		{
			TIM4->CNT = 0;//����TIM4��ʱ
		}
		tmpTxWrtPtr ++;
		tmpTxWrtPtr %= USART_TMP_BUF_MAX;
	}
	//usart�����쳣
	else if(USART_GetFlagStatus(WL_USART,USART_FLAG_ORE) != RESET)
	{
		USART_ClearFlag(WL_USART,USART_FLAG_ORE);
		WL_USART->DR;
		TIM4->CNT = 0;
	}
	else
	{
		//usart����
		if ((WL_USART->SR & USART_FLAG_TXE) != 0)//������λ��ɽ������ݴ���
		{
			u16UsartSendPtr++;
			if(u16UsartSendPtr == (sizeof(s_stRfidUsartSendBuf[0])-1))//����һ�������һ����
			{
				USART_SendData(WL_USART, *((u8*)&s_stRfidUsartSendBuf[s_u16RfidUsartSendBufRead]+u16UsartSendPtr));
				USART_ITConfig(WL_USART, USART_IT_TXE, DISABLE); //�ر�USART_IT_TXE�ж�
				USART_ClearFlag(WL_USART, USART_FLAG_TC); 
				USART_ITConfig(WL_USART, USART_IT_TC, ENABLE);//USART_IT_TC�ж�
			}
			else if(u16UsartSendPtr < (sizeof(s_stRfidUsartSendBuf[0])-1))
			{
				if(u16UsartSendPtr == (s_stRfidUsartSendBuf[s_u16RfidUsartSendBufRead].u8DLC + 5))
					u16UsartSendPtr = sizeof(s_stRfidUsartSendBuf[0]) - 2;
				USART_SendData(WL_USART, *((u8*)&s_stRfidUsartSendBuf[s_u16RfidUsartSendBufRead]+u16UsartSendPtr));
				USART_ClearFlag(WL_USART, USART_FLAG_TC); //���USART_FLAG_TC�жϱ�־
			}			
		}
		if ((WL_USART->SR & USART_FLAG_TC) != 0)
		{		
			if (u16UsartSendPtr >= (sizeof(s_stRfidUsartSendBuf[0])-1))//���һ�����ݷ������
			{
				USART_ClearFlag(WL_USART, USART_FLAG_TC); //���USART_FLAG_TC�жϱ�־
				USART_ITConfig(WL_USART, USART_IT_TC, DISABLE);	//USART_IT_TC�ж�
				u16UsartSendPtr = 0;
				OSSemPost(s_stUsartSend);
			}
		}
	}
	OSIntExit();
}

/***********************************************************************************************
** ����������	TIM4�жϷ�����
** �䡡  �룺	��
** �䡡  ����	��
************************************************************************************************/
void TIM4_IRQHandler(void)   //TIM4�ж�
{   
	u16 u16CrcTemp = 0x00;
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
			OS_CPU_SR  cpu_sr = 0;
	#endif

	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
	OSIntEnter();
	OS_EXIT_CRITICAL();
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		tmpTxWrtPtr = 0;
		
		if(UsartReveiveDataBuf[0] == USART_WL_ADDRESS)//����Ϊң��������
		{
			/*���մ����߼�*/				
			Crc16UsartReceiveChick(UsartReveiveDataBuf,UsartReveiveDataBuf[4]+5,&u16CrcTemp);
			//У����յ������Ƿ���ȷ
			if(UsartReveiveDataBuf[UsartReveiveDataBuf[4]+5] == (u8)((u16CrcTemp&0xFF00)>>8)  
			 && UsartReveiveDataBuf[UsartReveiveDataBuf[4]+6] == (u8)(u16CrcTemp&0x00FF))
			{
				//��ȷ
				if(UsartReveiveDataBuf[2] == WIRELESSDATA && UsartReveiveDataBuf[4]>1) //��������Ϊ�������ݣ������������ݳ��ȴ���1��Ϊ���յ����������ݣ�ֱ�Ӵ���ԭ��RFID���У��������ź���
				{
					//���ݴ���ԭ��RFID�����ݻ������
					OS_ENTER_CRITICAL(); 
					st_RFIDRcvFrmPool[u32RcvFrmPoolWritePtr].u8DataLen = UsartReveiveDataBuf[4]-1;
					st_RFIDRcvFrmPool[u32RcvFrmPoolWritePtr].u8DestAddr = 0x00;
					memcpy(st_RFIDRcvFrmPool[u32RcvFrmPoolWritePtr].u8Data,&UsartReveiveDataBuf[5],UsartReveiveDataBuf[4]-1);
					st_RFIDRcvFrmPool[u32RcvFrmPoolWritePtr].u8AppendStatus[0] = UsartReveiveDataBuf[UsartReveiveDataBuf[4]+4];
					u32RcvFrmPoolWritePtr++;
					u32RcvFrmPoolWritePtr %= 50;
					u32RcvFrmPoolCnt++;				
					OS_EXIT_CRITICAL();
					
					OSSemPost(s_stRfidRecvd);
				}
				//�������������ݴ���usart�Ļ�����
				else
				{
					memset(&(USARTKEYRxBuf[USARTKEYRxWrtPtr]), 0x00  , sizeof(stUSARTDATA));
					memcpy(&(USARTKEYRxBuf[USARTKEYRxWrtPtr]), UsartReveiveDataBuf,UsartReveiveDataBuf[4]+5);
					//��Ӧ��
					if(UsartReveiveDataBuf[3] == 1)
						SendResponseData((USARTKEYRxBuf[USARTKEYRxWrtPtr].Serial),1,UsartReveiveDataBuf[2]);
					
					USARTKEYRxWrtPtr ++;
					USARTKEYRxWrtPtr %= USART_RX_BUF_MAX;		
				}				
			}
			else
				;
			WlHeartTime = SendHeartTimerMAX;//WL������ʱ����ֵ
		}		
		TIM_Cmd(TIM4, DISABLE);  //�ر�TIMx
	}
	OSIntExit();
}

