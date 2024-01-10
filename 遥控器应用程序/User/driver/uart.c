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
#define  IR_USART_RX_BUF_MAX              200
#define  IR_UASRT_TX_BUF_MAX              200

/********************************************************************************
* ��������
*********************************************************************************/
static uint8_t		s_IrUsartRxBuf[IR_USART_RX_BUF_MAX];	//���ջ���
static uint32_t	s_u32IrUsartRxWrtPtr = 0;			   	//����дָ��
static uint32_t	s_u32IrUsartRxRdPtr = 0;			   		//���ն�ָ��

static uint8_t  	s_IrUsartTxBuf[IR_UASRT_TX_BUF_MAX];	//���ͻ���
static uint32_t	s_u32IrUsartTxWrtPtr = 0;			   	//����дָ��
static uint32_t	s_u32IrUsartTxRdPtr = 0;			   		//���Ͷ�ָ��

OS_EVENT  *s_stIrRecvd;	//��������ź���

/********************************************************************************
* ��������
*********************************************************************************/

/**********************************************************************************************************
*	�� �� ��: IrUsartParamInit
*	����˵��: IrUsart������ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
**********************************************************************************************************/
void IrUsartParamInit(void)
{
	s_u32IrUsartRxWrtPtr = 0;
	s_u32IrUsartRxRdPtr = 0;
	memset(s_IrUsartTxBuf,0x00,sizeof(s_IrUsartTxBuf));
	
	s_u32IrUsartTxWrtPtr = 0;
	s_u32IrUsartTxRdPtr = 0;	
	memset(s_IrUsartRxBuf,0x00,sizeof(s_IrUsartRxBuf));
	
}
/**********************************************************************************************************
*	�� �� ��: IrUsartInit
*	����˵��: IrUsart�豸��ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
**********************************************************************************************************/
void IrUsartInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;   
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	IrUsartParamInit();
	
	RCC_AHBPeriphClockCmd(RCC_IR_USART_RX_GPIO|RCC_IR_USART_ENABLE_GPIO|RCC_AHBPeriph_GPIOC, ENABLE);//���ڶ˿�ʱ��ʹ��
	IR_USART_APBxCmd();
	
	/* Configure USART2 Rx as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//GPIO_Mode_AF
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//GPIO_OType_PP	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = IR_USART_RX_PIN;
	GPIO_Init(IR_USART_RX_GPIO, &GPIO_InitStructure); 

	GPIO_PinAFConfig(IR_USART_RX_GPIO,IR_USART_RX_PinSource,IR_USART_PIN_AF_SET); //��ӳ��	
	
	if(g_u32ID == B_type)
	{
		/**����ʹ�ܶ˿ڳ�ʼ��***/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//GPIO_OType_PP
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = IR_USART_ENABLE_PIN;	
		GPIO_Init(IR_USART_ENABLE_GPIO, &GPIO_InitStructure);

		/* Set LED_LOCK_PIN */
		IrUsartChipPwrOff();//�رս���
	}
	else
	{
		/**����ʹ�ܶ˿ڳ�ʼ��***/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//GPIO_OType_PP
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		/* Set LED_LOCK_PIN */
		GPIO_SetBits(GPIOC, GPIO_Pin_6);//�رս���
	}

	////�������������Ҫ������ȱ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,ENABLE);//��λ����2
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,DISABLE);//ֹͣ��λ
	
	USART_InitStructure.USART_BaudRate = IR_USART_BAUNDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;		//��У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;//|USART_Mode_Tx; 
	
	USART_Init(IR_USART, &USART_InitStructure);				//����

	/* Configure the NVIC Preemption Priority Bits */  
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = IR_USART_IRQN;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x07;//��ʹ���ж����ȼ�Ƕ�ס���ΪSysTick���ж����ȼ�Ϊ0x0f
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	s_stIrRecvd  = OSSemCreate(0);	//�������������ź���

	USART_ITConfig(IR_USART, USART_IT_RXNE, ENABLE);	//�ж�ʹ��
	USART_ITConfig(IR_USART, USART_IT_TXE, DISABLE);
	USART_ITConfig(IR_USART, USART_IT_TC, DISABLE);
	

	IrUsartRxDisable();
}

/**********************************************************************************************************
*	�� �� ��: IrUsartReadRxBuf
*	����˵��: ��ȡIrUsart�Ľ��ջ�����
*	��    ��: u8Data:����֡��ָ��
*	�� �� ֵ: 0x00:��������Ҫ����0x01:��������Ҫ����
**********************************************************************************************************/
u32 IrUsartReadRxBuf(u8 *u8Data,u32 u32Ptr)
{
	*u8Data =  s_IrUsartRxBuf[u32Ptr];

	return 0x01;
}

/**********************************************************************************************************
*	�� �� ��: IrUsartRxBufSizeGet
*	����˵��: ��ȡIrUsart���ջ������Ĵ�С
*	��    ��: u32Size:����֡��ָ��
*	�� �� ֵ: 0x01:������������
**********************************************************************************************************/
u32 IrUsartRxBufSizeGet(u32 *u32Size)
{
	*u32Size = IR_USART_RX_BUF_MAX;
	
	return 0x01;
}

/**********************************************************************************************************
*	�� �� ��: IrUsartRxRdRtrGet
*	����˵��: ��ȡIrUsart�Ľ��ջ�������ָ��
*	��    ��: u8Data:���ݵ�ָ��
*	�� �� ֵ: 0x01:������������
**********************************************************************************************************/
u32 IrUsartRxRdRtrGet(u32 *u32Ptr)
{
	*u32Ptr = s_u32IrUsartRxRdPtr;
	
	return 0x01;
}

/**********************************************************************************************************
*	�� �� ��: IrUsartReadRxBuf
*	����˵��: ��ȡIrUsart�Ľ��ջ�����
*	��    ��: u8Data:����֡��ָ��
*	�� �� ֵ: 0x01:������������
**********************************************************************************************************/
u32 IrUsartRxRdRtrSet(u32 *u32Data)
{
	s_u32IrUsartRxRdPtr = *u32Data;
	s_u32IrUsartRxRdPtr %= IR_USART_RX_BUF_MAX;
	
	return 0x01;	
}

/**********************************************************************************************************
*	�� �� ��: IrUsartRxWtrRtrGet
*	����˵��: ��ȡIrUsart�Ľ��ջ�����дָ��
*	��    ��: u8Data:���ݵ�ָ��
*	�� �� ֵ: 0x00:��������Ҫ����0x01:��������Ҫ����
**********************************************************************************************************/
u32 IrUsartRxWtrRtrGet(u32 *u32Data)
{
	*u32Data = s_u32IrUsartRxWrtPtr;
	
	return 0x01;
}

/**********************************************************************************************************
*	�� �� ��: IR_USART_IRQHandler
*	����˵��: �жϴ���
*	��    ��: ��
*	�� �� ֵ: ��
**********************************************************************************************************/
void IR_USART_IRQHandler(void)
{
	uint16_t  data;
	uint16_t c;
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
			OS_CPU_SR  cpu_sr = 0;
	#endif


	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
	OSIntEnter();
	OS_EXIT_CRITICAL();
	if(USART_GetITStatus(IR_USART, USART_IT_RXNE) != RESET)
	{
		data = USART_ReceiveData(IR_USART);
		OS_ENTER_CRITICAL();
		s_IrUsartRxBuf[s_u32IrUsartRxWrtPtr ++] = (u8)(data & 0xff);
        s_u32IrUsartRxWrtPtr %= IR_USART_RX_BUF_MAX;
		OS_EXIT_CRITICAL();	
		#if 0//test
			s_IrUsartRxBuf[0x00] = 0x82;
			s_IrUsartRxBuf[0x01] = 0x41;
			s_IrUsartRxBuf[0x02] = 0x47;
			s_u32IrUsartRxRdPtr = 0x00;
			s_u32IrUsartRxWrtPtr = 0x03;	
		#endif		
		OSSemPost(s_stIrRecvd);
	}
	
	if (USART_GetITStatus(IR_USART, USART_IT_TC) != RESET)
	{ 	
		USART_ClearITPendingBit(IR_USART, USART_FLAG_TC);
		s_u32IrUsartTxRdPtr %= IR_UASRT_TX_BUF_MAX;
		USART_SendData(IR_USART, c);
		if(s_u32IrUsartTxRdPtr == s_u32IrUsartTxWrtPtr)
		{
			USART_ITConfig(IR_USART, USART_IT_TXE, DISABLE);		
		}
	}
	OSIntExit();
}

