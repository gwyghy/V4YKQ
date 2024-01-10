/*********************************************************************************************************************************
** �ļ���:  rfid_CTRL.h
** �衡��:  RFID����ģ��ͷ�ļ�
** ������: 	����
** �ա���:  2014-12-26
** �޸���:	
** �ա���:	
**
** �桡��:	V1.0.0.0
** ���¼�¼:
** ���¼�¼	��
** 					��    ��      ��    ��                    ��      ��
** 					==========  =============  ========================================
**
**--------------------------------------------------------------------------
**************************Copyright (c) 1998-1999 ��������Ӽ������޹�˾����������*************************************************/
#ifndef __RFID_CTRL_H__
#define __RFID_CTRL_H__

#include "stm32l1xx.h"

/*
 * RFID - SPI ��Ӧ�ܽź궨��
 */
#define RFID_SPI_MCU_CS_PIN				GPIO_Pin_12
#define RFID_SPI_MCU_SCK_PIN			GPIO_Pin_13
#define RFID_SPI_MCU_MISO_PIN			GPIO_Pin_14
#define RFID_SPI_MCU_MOSI_PIN			GPIO_Pin_15
#define RFID_SPI_MCU_GDO2_PIN			GPIO_Pin_7
#define RFID_SPI_MCU_GDO0_PIN			GPIO_Pin_6

/*
 * RFID - SPI ��Ӧ�˿ں궨��
 */
#define RFID_SPI_MCU_CS_PORT			GPIOB			
#define RFID_SPI_MCU_SCK_PORT			GPIOB
#define RFID_SPI_MCU_MISO_PORT			GPIOB
#define RFID_SPI_MCU_MOSI_PORT			GPIOB
#define RFID_SPI_MCU_GDO2_PORT			GPIOC
#define RFID_SPI_MCU_GDO0_PORT			GPIOC

/*
 * RFID - SPI ��Ӧ�˿�ʱ�Ӻ궨��
 */
#define RFID_SPI_MCU_PORT_RCC			RCC_AHBPeriph_GPIOB			//RCC_AHBPeriph_GPIOB,�޸�Ϊʹ��MCU��SPI�˿�
#define RFID_SPI_MCU_RCC				RCC_APB1Periph_SPI2			//RCC_APB1Periph_SPI2
#define RFID_SPI_MCU_AFIO_RCC										//RCC_APB2Periph_AFIO
#define RFID_GDO_MCU_RCC				RCC_AHBPeriph_GPIOC
#define RFID_SPI_PORT					SPI2

/*
 * GDOx�ж�����
 */
#define RFID_CC1101_GDOx_PORT_SOURCE	EXTI_PortSourceGPIOC		//GPIO_PortSourceGPIOC
#define RFID_CC1101_GDO0_PIN_SOURCE		EXTI_PinSource6				//GPIO_PinSource6
#define RFID_CC1101_GDO2_PIN_SOURCE		EXTI_PinSource7				//GPIO_PinSource7
#define RFID_CC1101_GDO0_EXTI_LINE		EXTI_Line6
#define RFID_CC1101_GDO2_EXTI_LINE		EXTI_Line7
#define RFID_CC1101_GDOx_EXTI_IRQn		EXTI9_5_IRQn

/*
 * ��������
 */
#define CC1101_ADDR_FILTER				1	// =1��ʹ�ܵ�ַ���˹��ܣ�=0��ʧ�ܵ�ַ���˹���

/*
 * ״̬�Ĵ����Ļ���ַ
 */
#define StatusRegBaseAddress			(0x30)

/*
 * ���ñ�������Ĭ�ϵ�ַ
 */
#define DEFAULT_ADDRESS					(0xFE)

/*
 * CC1101-TX-FIFO Size
 */
#define CC1101_TX_FIFO_SIZE				(64)
#define CC1101_RX_FIFO_SIZE				(64)

/*
 * RFID - �ز�����,��λ hz
 */
#define CC1101_CARRIER_FREQUENCY_DEFAULT	(433000000)
#define CC1101_CARRIER_FREQUENCY_MAX		(464000000)
#define CC1101_CARRIER_FREQUENCY_MIN		(387000000)

/*
 * CC1101 CCA����ʹ�ܻ�ʧ�ܶ���
 */
#define CC1101_CCA_ENABLED				1
#define CC1101_CCA_DISABLED				0

/*
 * CC1101Ĭ������
 */
#define DEFAULT_RF_PWRIDX   			(7)    		//Default RF Output Power Index
#define DEFAULT_RF_ATNIDX    			(2)    		//Default RF Attenuation Index//0
#define DEFAULT_RF_VELOCITY  			(38)  		//kbit/s
#define DEFAULT_RES_INTERVAL_TIME  		(0)			//s
#define DEFAULT_SLOT_TIME  				(20)	    //ms
#define DEFAULT_SLOT_SND_NUM  			(0)
#define	DEFAULT_CCA_USE_FLAG			CC1101_CCA_ENABLED

#define	DEFAULT_CHANNEL_SPACING			400000//hz,�ŵ���ȣ������400��������ֵ���������޸ģ�����������й�(rfid_cc1101.c)

/*
 * ͬ���ֽ�Ĭ������
 */
#if (YKQ_TARGET_USE_LOCATION_DEFINE == YKQ_USE_IN_WORK_STATION)
	#define	RFID_ININT_SYNC0		0x55
	#define	RFID_ININT_SYNC1		0x8E
#elif (YKQ_TARGET_USE_LOCATION_DEFINE == YKQ_USE_IN_WORK_TAIL)
	#define	RFID_ININT_SYNC0		0x55
	#define	RFID_ININT_SYNC1		0x8E
#elif (YKQ_TARGET_USE_LOCATION_DEFINE == YKQ_USE_IN_WORK_HEAD)
	#define	RFID_ININT_SYNC0		0x56
	#define	RFID_ININT_SYNC1		0x8E
#elif (YKQ_TARGET_USE_LOCATION_DEFINE == YKQ_USE_IN_INTELLIGNET_TRANSPORT)
	#define	RFID_ININT_SYNC0		0x57
	#define	RFID_ININT_SYNC1		0x8E
#else	
	#define	RFID_ININT_SYNC0		0x33
	#define	RFID_ININT_SYNC1		0x33
#endif

#define	RFID_PRG_ININT_SYNC0		0x33//Ĭ��ͬ���ֽ�0�����ڴ���������
#define	RFID_PRG_ININT_SYNC1		0x55//Ĭ��ͬ���ֽ�1�����ڴ���������

/*
 * ����״̬
 */
#define RET_OK							(0)
#define RET_ERR							(1)
#define TX_LENGTH_ERR					(2)
#define CHIP_STATUS_ABNORMAL			(3)
#define RX_LENGTH_VIOLATION				(4)
#define RX_CRC_MISMATCH					(5)
#define RX_FRM_POOL_EMPTY				(6)
#define TX_OK							RET_OK
#define RX_OK							RET_OK
#define TIME_OVER						(0xff)

/*
 * ���ȴ�״̬��ʱʱ��
 */
#define MAX_STATUS_DELAY				(10)
#define MAX_TX_DELAY					(50)

/*
 * ���ù���ģʽ����
 */
#define OBUMODE_ONROAD       			(0xfe01)	//·��ģʽ
#define OBUMODE_SLEEP        			(0xfe00)	//����ģʽ

/*
 * RFID��ر�ʶλ����
 */
typedef enum 
{
    RFCHIP_IDLE_POSITIVE = 0,
    RFCHIP_RESET_NECESSARILY = 1
}mRfChipRstFlag;

typedef enum 
{
    RFCHIP_RX_CLEARUP = 0,
    RFCHIP_TX_CLEARUP = 1
}mRFChipLastState;


//----------------------------------------------------------------------------------------
// 		�ṹ������
//----------------------------------------------------------------------------------------
/*
 * ����˥����������
 */
typedef struct
{
    u8 u8RFPwrIdx;    					//Output Power Table Index
    u8 u8RFAtnIdx;    					//Attenuation Table Index
    u8 u8RFVelocity;					//Velocity Table Index
    u8 u8ResIntervalTime;
    u8 u8SlotTime;
    u8 u8SlotSndNum;
} Tag_RFChipPar;

/* 
 * T_DRV_RF_CONFIG is a data structure which contains all relevant CC1101 registers
 */

typedef struct tagT_RF_CONFIG
{
	//u8 u8FSCTRL2;   // Frequency synthesizer control.
    u8 u8FSCTRL1;   // Frequency synthesizer control.
    u8 u8FSCTRL0;   // Frequency synthesizer control.
    u8 u8FREQ2;     // Frequency control word, high byte.
    u8 u8FREQ1;     // Frequency control word, middle byte.
    u8 u8FREQ0;     // Frequency control word, low byte.

    u8 u8MDMCFG4;   // Modem configuration.
    u8 u8MDMCFG3;   // Modem configuration.
    u8 u8MDMCFG2;   // Modem configuration.
    u8 u8MDMCFG1;   // Modem configuration.
    u8 u8MDMCFG0;   // Modem configuration.

    u8 u8CHANNR;    // Channel number.
    u8 u8DEVIATN;   // Modem deviation setting (when FSK modulation is enabled).
    u8 u8FREND1;    // Front end RX configuration.
    u8 u8FREND0;    // Front end RX configuration.
    u8 u8MCSM0;     // Main Radio Control State Machine configuration.

    u8 u8FOCCFG;    // Frequency Offset Compensation Configuration.
    u8 u8BSCFG;     // Bit synchronization Configuration.
    u8 u8AGCCTRL2;  // AGC control.
    u8 u8AGCCTRL1;  // AGC control.
    u8 u8AGCCTRL0;  // AGC control.

    u8 u8FSCAL3;    // Frequency synthesizer calibration.
    u8 u8FSCAL2;    // Frequency synthesizer calibration.
    u8 u8FSCAL1;    // Frequency synthesizer calibration.
    u8 u8FSCAL0;    // Frequency synthesizer calibration.
    u8 u8FSTEST;    // Frequency synthesizer calibration control

    u8 u8TEST2;     // Various test settings.
    u8 u8TEST1;     // Various test settings.
    u8 u8TEST0;     // Various test settings.
    u8 u8IOCFG2;    // GDO2 output pin configuration
    u8 u8IOCFG0;    // GDO0 output pin configuration

    u8 u8PKTCTRL1;  // Packet automation control.
    u8 u8PKTCTRL0;  // Packet automation control.
    u8 u8ADDR;      // Device address.
    u8 u8PKTLEN;    // Packet length.
} Tag_RF_CONFIG;

#endif /* __RFID_CTRL_H__*/
/*********************************������������޹�˾*************************************************************/
