/********************************************************************************
* �ļ����ƣ�	Battery.h
* ��	�ߣ�	������   
* ��ǰ�汾��   	V1.0
* ������ڣ�    2015.05.08
* ��������: 	����Battery.hͷ�ļ�
* ��ʷ��Ϣ��   
*           	�汾��Ϣ     ���ʱ��      ԭ����        ע��
*
*       >>>>  �ڹ����е�λ��  <<<<
*             3-Ӧ�ò�
*          ��  2-Э���
*          	  1-Ӳ��������
*********************************************************************************
* Copyright (c) 2014,������������޹�˾ All rights reserved.
*********************************************************************************/
#ifndef __BATTERY_H__  
#define __BATTERY_H__

/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/
#include "includes.h"

/********************************************************************************
* #define�궨��
*********************************************************************************/
/**���Ŷ���**/
#define	BATTERY_VOLTAGE_AD_CHANNEL			ADINPUT_1
#define	CPU_VREFINT_AD_CHANNEL				ADINPUT_2
#define	BATTERY_VOLTAGE_AD_CHANNEL_1			ADINPUT_3
#define	BATTERY_CDZT_IN_CHANNEL				INPUT_1//��CHRG,��ƽ

/**������ֵ���Ͷ���**/
typedef enum
{
	BATTERY_COULOMETRY_EMPTY = 0,//��INTERFACE������ʾ���Ӧ
	BATTERY_COULOMETRY_10_PERCENT ,
	BATTERY_COULOMETRY_20_PERCENT,	 
	BATTERY_COULOMETRY_40_PERCENT,	 
	BATTERY_COULOMETRY_60_PERCENT,	 
	BATTERY_COULOMETRY_80_PERCENT,	
	BATTERY_COULOMETRY_100_PERCENT, 
	BATTERY_COULOMETRY_WRONG_PERCENT, 

	BATTERY_COULOMETRY_3_PERCENT ,//ֻ����״̬�ж�	
	BATTERY_COULOMETRY_5_PERCENT ,	
	BATTERY_COULOMETRY_TYPE_MAX
}BATTERY_COULOMETRY_TYPE;

/**����������ж�����**/
__packed typedef struct
{
	u16 u16ValueMin;
	u16 u16ValueMax;
	BATTERY_COULOMETRY_TYPE sBatteryColumnStatus;
}BATTERY_CALCULATE_TYPE;

/**��������ڵ�״̬����**/
typedef	enum
{
	BATTERY_ININT = 0,//��ʼ��״̬
	
	BATTERY_START_CHARGED,//��ʼ���
	BATTERY_BEING_CHARGED,//���ڳ�������
	BATTERY_CHARGE_GONNA_OVER,//��缴�����
	BATTERY_CHARGE_OVER,//����Ѿ����
	
	BATTERY_WORK_NORMAL,//��������
	BATTERY_TOO_LOW,//����̫��״̬(С��3%)
	BATTERY_FAULT,//����
}BATTERY_STATUS_TYPE;

/**��ز���硢������ص����Ľ��ƹ�ϵ**/
typedef struct
{
	u32 u32VoltageWork;//�����ʱ�ĵ�ѹ
	u32 u32VoltageInCharge;//���ʱ�ĵ�ѹ
	u32 u32BatteryPercent;//��Ӧ�ĵ���ǧ�ֱ�
	u32 u32CurrentIncharge;//���ʱ�ĵ���ֵ
}BATTERY_VOLTAGE_PERCENT_TYPE;
/********************************************************************************
* ��������
*********************************************************************************/
/*��ص���**/
#define		BATTERY_CURRENT_MAX				312000//2600(mA/H)*60(����)*2(0.5����)

#define		BATTERY_CURRENT_MAX_D				312000//1500(mA/H)*60(����)*2(0.5����)
#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//��Ӳ���У�ʹ��DS2756��ʽ�������
	#define		BATTERY_ACR_MAX					2600//2600(mA/H)
#endif
#define		BATTERY_INCHARGE_TIME_MAX		23400000//���ʱ�����ֵ

/**�ο���ѹ**/
#define 	AD_REF_VOLTAGE_SET				3300//��λmv
#define		AD_SAMPLE_MIN					0//12λAD��������Сֵ
#define		AD_SAMPLE_MAX					4095//12λAD���������ֵ

/**ADת�������**/
#define		BATTERY_AD_WRAP					200//��λ0.1mV����������ʵ��ƫ��20mV.

/**�����صĵ�ѹ����(��mvΪ��λ)***/
#define		BATTERY_VOLTAGE_FULL_VALUE_MAX	4200//��ص������ֵ
#define   	BATTERY_VOLTAGE_FULL_PERCENT	100
#define   	BATTERY_VOLTAGE_90_PERCENT		90
#define   	BATTERY_VOLTAGE_8_PERCENT		8
#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//��Ӳ���У�ʹ��DS2756��ʽ�������
	#define   	BATTERY_VOLTAGE_3_PERCENT		3
#endif
#define   	BATTERY_VOLTAGE_EMPTY_PERCENT	0
#define   	BATTERY_VOLTAGE_FULLMAX_PERCENT	110//��������ֵ

/**���CHRG��״̬(��mvΪ��λ)**/
#define		BATTERY_VOLTAGE_MAX				32000


#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//��Ӳ���У�ʹ��DS2756��ʽ�������
	#define		BATTERY_INCHARGE_TEMPER_MAX		100//��
#endif

#define		BATTERY_TURNOFF_TURNON_VOLTAGE_DIFFERENCE	1000//��λ0.1mV
#define		BATTERY_TURNOFF_TURNON_VOLTAGE_DIFFERENCE2	3000//��λ0.1mV
/********************************************************************************
* ȫ�ֱ�������
*********************************************************************************/


/********************************************************************************
* ��������
*********************************************************************************/
u32 BatteryGetCoulomeData(u16 *u16Data);
u32 BatteryGetStatus(BATTERY_STATUS_TYPE *pStatus);
u32 BatteryGetCoulomeStatus(BATTERY_COULOMETRY_TYPE *pData);
#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_ADC_MNG)//��Ӳ���У�ʹ��ADC��ʽ�������
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

