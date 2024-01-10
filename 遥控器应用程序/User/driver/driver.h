/********************************************************************************
* 文件名：	led.c
* 作者：	马如意   
* 版本：   	V1.0
* 日期：    2015.04.24
* 功能描述:  实现LED的驱动

* 修改说明：   
*
*       >>>>  在工程中的位置  <<<<
*             3-应用层
*             2-协议层
*         √   1-硬件驱动层
*********************************************************************************
* @copy
* <h2><center>&copy; COPYRIGHT 天津华宁电子有限公司 研发中心 软件部</center></h2>
*********************************************************************************/
/********************************************************************************
* .h头文件
*********************************************************************************/
#ifndef  __DRIVER_H__
#define  __DRIVER_H__

/********************************************************************************
* .h头文件
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
* #define宏定义
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
* 常量定义
*********************************************************************************/
/********************************************************************************
* 全局变量定义
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
* 函数声明
*********************************************************************************/
void Driver_Init(void);
#endif



