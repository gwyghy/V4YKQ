/********************************************************************************
* 文件名称：	Battery.h
* 作	者：	马如意   
* 当前版本：   	V1.0
* 完成日期：    2015.05.08
* 功能描述: 	定义Battery.h头文件
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
#ifndef __BATTERY_H__  
#define __BATTERY_H__

/********************************************************************************
* .h头文件
*********************************************************************************/
#include "includes.h"

/********************************************************************************
* #define宏定义
*********************************************************************************/
/**引脚定义**/
#define	BATTERY_VOLTAGE_AD_CHANNEL			ADINPUT_1
#define	CPU_VREFINT_AD_CHANNEL				ADINPUT_2
#define	BATTERY_VOLTAGE_AD_CHANNEL_1			ADINPUT_3
#define	BATTERY_CDZT_IN_CHANNEL				INPUT_1//即CHRG,电平

/**电量数值类型定义**/
typedef enum
{
	BATTERY_COULOMETRY_EMPTY = 0,//与INTERFACE界面显示相对应
	BATTERY_COULOMETRY_10_PERCENT ,
	BATTERY_COULOMETRY_20_PERCENT,	 
	BATTERY_COULOMETRY_40_PERCENT,	 
	BATTERY_COULOMETRY_60_PERCENT,	 
	BATTERY_COULOMETRY_80_PERCENT,	
	BATTERY_COULOMETRY_100_PERCENT, 
	BATTERY_COULOMETRY_WRONG_PERCENT, 

	BATTERY_COULOMETRY_3_PERCENT ,//只用于状态判断	
	BATTERY_COULOMETRY_5_PERCENT ,	
	BATTERY_COULOMETRY_TYPE_MAX
}BATTERY_COULOMETRY_TYPE;

/**计算电量的判定依据**/
__packed typedef struct
{
	u16 u16ValueMin;
	u16 u16ValueMax;
	BATTERY_COULOMETRY_TYPE sBatteryColumnStatus;
}BATTERY_CALCULATE_TYPE;

/**电池所处于的状态定义**/
typedef	enum
{
	BATTERY_ININT = 0,//初始化状态
	
	BATTERY_START_CHARGED,//开始充电
	BATTERY_BEING_CHARGED,//正在充电过程中
	BATTERY_CHARGE_GONNA_OVER,//充电即将完成
	BATTERY_CHARGE_OVER,//充电已经完成
	
	BATTERY_WORK_NORMAL,//正常工作
	BATTERY_TOO_LOW,//电量太低状态(小于3%)
	BATTERY_FAULT,//故障
}BATTERY_STATUS_TYPE;

/**电池不充电、充电与电池电量的近似关系**/
typedef struct
{
	u32 u32VoltageWork;//不充电时的电压
	u32 u32VoltageInCharge;//充电时的电压
	u32 u32BatteryPercent;//对应的电量千分比
	u32 u32CurrentIncharge;//充电时的电流值
}BATTERY_VOLTAGE_PERCENT_TYPE;
/********************************************************************************
* 常量定义
*********************************************************************************/
/*电池电量**/
#define		BATTERY_CURRENT_MAX				312000//2600(mA/H)*60(分钟)*2(0.5分钟)

#define		BATTERY_CURRENT_MAX_D				312000//1500(mA/H)*60(分钟)*2(0.5分钟)
#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//在硬件中，使用DS2756方式计算电量
	#define		BATTERY_ACR_MAX					2600//2600(mA/H)
#endif
#define		BATTERY_INCHARGE_TIME_MAX		23400000//充电时间最大值

/**参考电压**/
#define 	AD_REF_VOLTAGE_SET				3300//单位mv
#define		AD_SAMPLE_MIN					0//12位AD采样的最小值
#define		AD_SAMPLE_MAX					4095//12位AD采样的最大值

/**AD转化的误差**/
#define		BATTERY_AD_WRAP					200//单位0.1mV。经测量，实际偏大20mV.

/**电池相关的电压定义(以mv为单位)***/
#define		BATTERY_VOLTAGE_FULL_VALUE_MAX	4200//电池的最大极限值
#define   	BATTERY_VOLTAGE_FULL_PERCENT	100
#define   	BATTERY_VOLTAGE_90_PERCENT		90
#define   	BATTERY_VOLTAGE_8_PERCENT		8
#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//在硬件中，使用DS2756方式计算电量
	#define   	BATTERY_VOLTAGE_3_PERCENT		3
#endif
#define   	BATTERY_VOLTAGE_EMPTY_PERCENT	0
#define   	BATTERY_VOLTAGE_FULLMAX_PERCENT	110//电量极限值

/**充电CHRG的状态(以mv为单位)**/
#define		BATTERY_VOLTAGE_MAX				32000


#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//在硬件中，使用DS2756方式计算电量
	#define		BATTERY_INCHARGE_TEMPER_MAX		100//度
#endif

#define		BATTERY_TURNOFF_TURNON_VOLTAGE_DIFFERENCE	1000//单位0.1mV
#define		BATTERY_TURNOFF_TURNON_VOLTAGE_DIFFERENCE2	3000//单位0.1mV
/********************************************************************************
* 全局变量声明
*********************************************************************************/


/********************************************************************************
* 函数声明
*********************************************************************************/
u32 BatteryGetCoulomeData(u16 *u16Data);
u32 BatteryGetStatus(BATTERY_STATUS_TYPE *pStatus);
u32 BatteryGetCoulomeStatus(BATTERY_COULOMETRY_TYPE *pData);
#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_ADC_MNG)//在硬件中，使用ADC方式计算电量
	u32 BatteryGetAcrBackup(u32 *pAcrData);
	u32 BatteryGetVoltBackup(u32 *pVoltageData);
	u32 BatterySetTotalAcr(u32 u32AcrValue);
	u32 BatterySetCoulomeData(u32 u32CoulValue);
	u32 BatteryGetCoulomeData_D(u16 *u16Data);
	u32 BatteryGetVoltageTempProc(void);
	u32 BatteryGetVoltageTempProc_B(void);	
	u32 BatteryGetVoltageTempProc_D(void);
	u32 BatteryGetInchargeTimer(u32 *pVoltageData);	
#endif
void BatteryMngTask(void *p_arg);

#endif

