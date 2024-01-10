/********************************************************************************
* �ļ�����	led.c
* ���ߣ�	������   
* �汾��   	V1.0
* ���ڣ�    2015.04.24
* ��������:  ʵ��LED������

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
#ifndef  __DRIVER_H__
#define  __DRIVER_H__

/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/
#include "includes.h"

#include "oled.h"
#include "eepromarm.h"
#include "adc.h"
#include "pwr.h"

#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
	#include "beep.h"
#endif

#include "led.h"
#include "key.h"
#include "keyled.h"
#include "uart.h"
#include "uart1.h"
#include "ir_send.h"
#include "input.h"
#include "rfid_driver.h"
#include "rfid_cc1101.h"
#include "rfid_config.h"
#include "pwr.h"
#include "ncio.h"
#include "timer.h"

#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)
	#include "ds2756.h"
#elif (YKQ_HARDWARE_BATTERY_MNG == BATTERY_ADC_MNG)
	#include "adc.h"
#endif

#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!=	DISABLED)
	#include "zzz.h"
#endif
/********************************************************************************
* #define�궨��
*********************************************************************************/
#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!=	DISABLED)
	#define SUCCESS 0
	#define ERROR_1 1
	#define ERROR_2 2
	#define ERROR_3 3
	#define ERROR_4 4
	#define ERROR_5 5
	#define ERROR_6 6
	#define ERROR_7 7
	#define ERROR_8 8
	#define ERROR_9 9
	#define ERROR_10 10

	#define ERROR_UP_OVERFLOW     ERROR_1
	#define ERROR_DOWN_OVERFLOW   ERROR_2
	#define ERROR_OUTOFFRAME      ERROR_3
	#define ERROR_OVERTIME		  ERROR_4
	#define ERROR_DATA_NUM		  ERROR_5
#endif

/********************************************************************************
* ��������
*********************************************************************************/
/********************************************************************************
* ȫ�ֱ�������
*********************************************************************************/

enum{
  INTERFACE_IWDG = 0,
  KEYSCAN_IWDG,
  BATTERYMNG_IWDG,
	LOGIC_IWDG,
	IRMNG_IWDG,
	RFIDMNG_IWDG,
	RFIDMNG_UPDATE_IWDG,
	USARTHANDLE_IWDG,
};
/********************************************************************************
* ��������
*********************************************************************************/
void Driver_Init(void);
#endif



