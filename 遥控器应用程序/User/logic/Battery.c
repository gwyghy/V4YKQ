 /********************************************************************************
* �ļ����ƣ�Battery.c
* ��    �ߣ�������   
* ��    ����V1.0
* ��    �ڣ�2015.04.28
* ��������: ʵ�ֵ�ص�������

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
#define	BATTERY_INCHARGE_STATUS_SIZE			6//���ʱ��ʾ��״̬����
#define	BATTERY_SAMPLE_BUF_SIZE					10//�������������С

#define	BATTERY_TOOLOW_STATUS_SIZE				2//��ص���̫��ʱ��

#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//��Ӳ���У�ʹ��DS2756��ʽ�������
	#define	BATTERY_FULL_JUDGE_DATA				99//��������ֵ���ж�����
#else
	#define	BATTERY_FULL_JUDGE_DATA				990//��������ֵ���ж�����
#endif

#define	BATTERY_VOLTAGE_PERCENT_BUFSIZE			27

#define	BATTERY_COLUMDATA_CHANGE_DELAY			900000//�ϵ����ʱ�ɼ�

#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//��Ӳ���У�ʹ��DS2756��ʽ�������
	#define	BATTERYMNG_TASK_TIME_SET			1000//����ִ������
	#define	BATTERY_TASK_START_TIME_SET			10//����ʼִ�У�����
	#define	BATTERY_SAMPLE_INVALID_TIMESSET		5//ǰn�ζ�ȡ��Ч�Ĵ���
	
	#define	BATTERY_FULL_VOLTAGE_BASE			4000//��س�����ѹ��׼��,mv	
	#define	BATTERY_INCHARGE_SAMPLE_DELAY		30000//������ʱ�ɼ���ѹ	
#else
	#define	BATTERYMNG_TASK_TIME_SET			50//����ִ������	
	#define	BATTERY_TASK_POWERUP_DELAY			1000//�ϵ���ʱʱ��	
	#define	BATTERY_INCHARGE_SAMPLE_DELAY		30000//����ɼ���ѹ���,��ʱ�������޸�	

	#define	BATTERY_INCHARGE_TIME_DELAY			300000//������ʱ�ɼ�
	#define	BATTERY_LINEOFF_TIME_DELAY			900000//�ϵ����ʱ�ɼ�	
	
	
	
	#define	BATTERY_INCHARGE_TIME_DELAY_D			10000//������ʱ�ɼ�
	#define	BATTERY_LINEOFF_TIME_DELAY_D			30000//�ϵ����ʱ�ɼ�	
#endif
/********************************************************************************
* ��������
*********************************************************************************/
static BATTERY_VOLTAGE_PERCENT_TYPE const s_stBatteryVoltagePercent[BATTERY_VOLTAGE_PERCENT_BUFSIZE] = 
{
	{20190,	22442, 	0,			450},
	{22600,	24140,	30,			450},
	{23190,	24840,	60,			450},
	{23500,	25940,	80,			450},
	{23800,	26200,	100,		450},

	{23900,	26280, 	160,		450},
	{24100,	26408,	200,		450},
	{24250,	26500,	240,		450},
	{24400,	26570,	280,		450},
	{24500,	26620, 	330,		450},

	{24800,	26730,	400,		450},
	{24900,	26870,	440,		450},
	{25000,	26980, 	530,		450},
	{25100,	27070, 	580,		450},
	{25200,	27170, 	600,		450},

	{25280,	27170, 	630,		450},
	{25360,	27250,	680,		450},
	{25440,	27410,	720,		450},
	{25500,	27500, 	760,		450},	
	{25600,	27600, 	800,		450},
	
	{25800,	27700, 	850,		450},	
	{26000,	27800,	880,		450},
	{26100,	27900,	900,		450},
	{26300,	28000, 	930,		450},
	{26500,	28200, 	950,		450},	

	{26900,	28300, 	990,		450},	
	{27000,	29000,	1000,		450}
};



static BATTERY_VOLTAGE_PERCENT_TYPE const s_stBatteryVoltagePercent_D[BATTERY_VOLTAGE_PERCENT_BUFSIZE] = 
{
	{27200,	30710, 	0,			500},
	{27200,	30710,	0,			500},
	{31500,	33500,	13,			500},
	{32800,	35400,	30,			500},
	{33600,	36000,	55,	  	500},

	{34000,	37500, 	120,		500},
	{34360,	37900,	161,		500},
	{34750,	38200,	214,		500},
	{35070,	38500,	255,		500},
	{35400,	38800, 	295,		500},

	{35770,	39000,	336,		500},
	{36200,	39230,	376,		500},
	{36600,	39800, 	417,		500},
	{37100,	40000, 	458,		500},
	{37400,	40800, 	526,		500},
                          
	{37700,	41100, 	594,		500},
	{37900,	41400,	635,		500},
	{38100,	41500,	675,		500},
	{38300,	41600, 	716,		500},	
	{38600,	41700, 	756,		500},
	
	{38800,	41800, 	797,		500},	
	{39000,	41800,	837,		500},
	{39200,	41800,	878,		500},
	{39430,	41800, 	918,		500},
	{39500,	41900, 	946,		500},	

	{39800,	41900, 	973,		500},	
	{40600,	42300,	1000,		500}
};


/**��ص����ж�������**//**��ʵ�ʲ��ԣ����ַ�ѹ��ʽ��2-5��ʹ��ʱ��ϳ���1����˸�ȳ���ʱ��϶�***/
static const BATTERY_CALCULATE_TYPE s_stBatteryCalculateList[BATTERY_COULOMETRY_TYPE_MAX] =
{
	{0,		3,		BATTERY_COULOMETRY_EMPTY},//�ػ�
	{3,		5,		BATTERY_COULOMETRY_3_PERCENT},//��ʾ
	{5,		8,		BATTERY_COULOMETRY_5_PERCENT},//��
	{8,		12,		BATTERY_COULOMETRY_10_PERCENT},//��
	{12,	20,		BATTERY_COULOMETRY_20_PERCENT},//1
	{20,	40,		BATTERY_COULOMETRY_40_PERCENT},//2
	{40,	60,		BATTERY_COULOMETRY_60_PERCENT},//3
	{60,	80,		BATTERY_COULOMETRY_80_PERCENT},//4
	{80,	110,	BATTERY_COULOMETRY_100_PERCENT},//105,5
	{110,	130,	BATTERY_COULOMETRY_WRONG_PERCENT}//130
};

/**���ʱϵ��ʾ��״̬����**/
static const BATTERY_COULOMETRY_TYPE s_stBatteryInCharge[BATTERY_INCHARGE_STATUS_SIZE] = 
{
	BATTERY_COULOMETRY_10_PERCENT,
	BATTERY_COULOMETRY_20_PERCENT,
	BATTERY_COULOMETRY_40_PERCENT,
	BATTERY_COULOMETRY_60_PERCENT,
	BATTERY_COULOMETRY_80_PERCENT,
	BATTERY_COULOMETRY_100_PERCENT
};

/**��ص���̫��ʱ��ʾ�ĵ�״̬����**/
static const BATTERY_COULOMETRY_TYPE s_stBatteryTooLow[BATTERY_TOOLOW_STATUS_SIZE] = 
{
	BATTERY_COULOMETRY_EMPTY,
	BATTERY_COULOMETRY_10_PERCENT		
};

static BATTERY_STATUS_TYPE		s_stBatteryStatus;//���״̬
static BATTERY_STATUS_TYPE		s_stBatteryStatus_D;//���״̬
static BATTERY_COULOMETRY_TYPE	s_stBatteryCouloStatus;//��ص���״̬
static BATTERY_COULOMETRY_TYPE	s_stBatteryCouloStatus_D;//��ص���״̬
static u16 s_u16BatteryCouloData;//ʵ�ʵ�����ֵ���ٷ���(0~100)
static u16 s_u16BatteryCouloData_D;//ʵ�ʵ�����ֵ���ٷ���(0~100)
static u32 s_u32BatteryInchargeTimer;//���ʱ��	
static u16 s_u16BatteryCouloDataBackup;//ʵ�ʵ�����ֵ���ٷ���(0~100)���� 
static u32 s_u32BatteryColumDataInint = 0x00;
static u32 s_u32BatteryChangeTimer = 0x00;
	
#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//��Ӳ���У�ʹ��DS2756��ʽ�������
	static s32 s32BatteryCurrentBuf[BATTERY_SAMPLE_BUF_SIZE];
	static u16 s_u16BatteryCurtBufpointer ;
	static s32 s32BatteryCurrentValue;
		
	static s32 s32BatteryVoltBuf[BATTERY_SAMPLE_BUF_SIZE] ;
	static u16 s_u16BatteryVoltBufpointer;
	static s32 s32BatteryVoltValue;
	
	static s32 s32BatteryAccCurrentBuf[BATTERY_SAMPLE_BUF_SIZE];
	static u16 s_u16BatteryAccCurtBufpointer ;
	static s32 s32BatteryAccCurtValue;
	
	static s32 s32BatteryTempBuf[BATTERY_SAMPLE_BUF_SIZE] ;
	static u16 s_u16BatteryTempBufpointer;	
	static s32 s32BatteryTempValue;
	static u32 s_u32BatteryCalTimes = 0x00;//���ڼ�¼��ʼ��ȡ����

	static BATTERY_COULOMETRY_TYPE	s_stBatteryCouloStatusBackup;//��ص���״̬����	
#else
	static u32 s_u32BatteryVoltageBackup;//��ѹֵ����
	static u32 s_u32BatteryVoltageBackup_D;//��ѹֵ����
	static u32 s_u32BatteryVoltageBackup_B;//��ѹֵ�������ڽ�����ʾ
	static u32 s_u32BatteryCurrentTotalBackup;//����ֵ����
	static u32 s_u32BatteryCurrentTotalBackup_D;//����ֵ����
	static u32 s_u32BatteryVoltageBackupBkp;//��ѹֵ����
	static u16 s_u16BatteryVoltageBuf[BATTERY_SAMPLE_BUF_SIZE];//����������ֵ
	static u16 s_u16BatteryVoltageBuf_D[BATTERY_SAMPLE_BUF_SIZE];//����������ֵ
	static u16 s_u16CpuVrefIntBuf[BATTERY_SAMPLE_BUF_SIZE];//CPU���յ�ѹ������ֵ
	static u16 s_u16BatteryVoltBufpointer ;//ָ����״̬�����ָ��	
	static u32 s_u32BatteryCurrentTotalInint = 0x00;
	static u32 s_u32BatteryCurrentTotalBackupBkp;//����ֵ����	
	static BATTERY_COULOMETRY_TYPE	s_stBatteryCouloStatusBackup;//��ص���״̬����
	static BATTERY_COULOMETRY_TYPE	s_stBatteryCouloStatusBackup_D;//��ص���״̬����
	static u32 s_u32BatteryTaskTimer = 0x00;//�������������ʱ��
	static u32 s_u32AdVerfintDefault = 0x00;//AD���յ�ѹ�ĳ���Ĭ��ֵ,��λ0.01mV	
#endif

/********************************************************************************
* ��������
*********************************************************************************/

/******************************************************************************************
* �������ƣ�FilterAndSum 
* ��������������ȥ��һ�����ֵ����Сֵ�����������ݵĺ�
* ��ڲ�����adc�����ݣ�length�����ݸ���
* ���ڲ�������
* ʹ��˵������
*******************************************************************************************/	
#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//��Ӳ���У�ʹ��DS2756��ʽ�������	
s32 FilterAndAverage ( s32 *adc,u8 length)
{
    s32 sum = 0;
	s32 max = *adc;
	s32 min = *adc;
	u8 i = 0;
	
	for (i=0;i<length;i++)
	{
	    if (*(adc+i) > max)   
		     max = *(adc+i);
		if (*(adc+i) < min)   
		     min = *(adc+i);
	    sum += *(adc+i);
	}
	
	sum = (sum- max - min);// (sum- max - min)/(length-2)
	sum /= (length-0x02);
	
	return sum;
}
#else
s32 FilterAndAverage ( u16 *adc,u8 length)
{
  u32 sum = 0;
	u32 max = *adc;
	u32 min = *adc;
	u8 i = 0;
	
	for (i=0;i<length;i++)
	{
	    if (*(adc+i) > max)   
		     max = *(adc+i);
		if (*(adc+i) < min)   
		     min = *(adc+i);
	    sum += *(adc+i);
	}
	
	sum = (sum- max - min);// (sum- max - min)/(length-2)
	sum /= (length-0x02);
	
	return sum;
}
#endif
/***************************************************************************************
** ��������:BatteryVoltageProc
** �����������ɼ����������ֵ��״̬
** �������룺��
** ���������None
** ע�����
******************************************************************************************/
#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//��Ӳ���У�ʹ��DS2756��ʽ�������
u32 BatteryReRalculateCurrent(void)
{
	s8 s8I = 0x00,s8J = 0x00;
	s32 s32Temp = 0x00;
	s32 s32TempAcr = 0x00;
	s32 s32TempVoltage = 0x00;	
	u32 u32PercentAcr = 0x00;
	u32 u32PercentCalculate = 0x00;
	u8 u8Flag = 0x00;
	
	OSTimeDly(1000/TICK_TIME);
	Ds2756Api(DS2756_READ_CURRENT,&s32Temp);
	OSTimeDly(1000/TICK_TIME);
	/**��ȡ�ۼƵ���ֵ**/
	Ds2756Api(DS2756_READ_ACCUMULATED_CURRENT,&s32TempAcr);
	OSTimeDly(1000/TICK_TIME);
	/**��ȡ��ѹֵ**/
	Ds2756Api(DS2756_READ_VOLTAGE,&s32TempVoltage);
	OSTimeDly(1000/TICK_TIME);
	s32TempVoltage *= 10;
	if(s32TempAcr > 0)
		u32PercentAcr = s32TempAcr*1000/BATTERY_ACR_MAX;//ԭ����Ϊǧ��֮һ��λ	
	else
		u32PercentAcr = 0;	
	
	if((s32TempVoltage <= 0x00)||(s32TempAcr == 0x00))
		return 0x00;
	
	if(s32TempAcr > BATTERY_ACR_MAX)
		u8Flag = 0x01;
		
	InputGetValue(BATTERY_CDZT_IN_CHANNEL, (INPUT_VALUE_TYPE *)&s8I);
	
	s8J = 0xFF;
	if(s8I == INPUT_HIGH)	
	{
		for(s8I = BATTERY_VOLTAGE_PERCENT_BUFSIZE - 0x01; s8I >= 0; s8I--)
		{
			if(s32TempVoltage >= s_stBatteryVoltagePercent[s8I].u32VoltageInCharge)
			{
				for(s8J= s8I+0x01; s8J < BATTERY_VOLTAGE_PERCENT_BUFSIZE; s8J++)
				{
					if(s32TempVoltage < s_stBatteryVoltagePercent[s8J].u32VoltageInCharge)
						break;
				}
				break;
			}
		}
		/**���쳣��Χ���д���***/
		if((s8I == (BATTERY_VOLTAGE_PERCENT_BUFSIZE - 0x01))&&(s8J == 0xFF))
		{
			s8J = BATTERY_VOLTAGE_PERCENT_BUFSIZE - 0x01;
			s8I = BATTERY_VOLTAGE_PERCENT_BUFSIZE - 0x02;
		}		
		if(s8J != 0xFF)
		{
			u32PercentCalculate = (s_stBatteryVoltagePercent[s8J].u32BatteryPercent - s_stBatteryVoltagePercent[s8I].u32BatteryPercent)*(s32TempVoltage - s_stBatteryVoltagePercent[s8I].u32VoltageInCharge);
			u32PercentCalculate /= (s_stBatteryVoltagePercent[s8J].u32VoltageInCharge - s_stBatteryVoltagePercent[s8I].u32VoltageInCharge);
			u32PercentCalculate += s_stBatteryVoltagePercent[s8I].u32BatteryPercent;	
			if(((u32PercentCalculate >= u32PercentAcr)&&(fabs(u32PercentCalculate-u32PercentAcr)>=50))\
				||((u32PercentAcr >= u32PercentCalculate)&&(fabs(u32PercentAcr-u32PercentCalculate)>=50))\
				||(u8Flag == 0x01))
			{
				s32TempAcr = u32PercentCalculate*BATTERY_ACR_MAX/1000;
				if(s32TempAcr > BATTERY_ACR_MAX)
					s32TempAcr = BATTERY_ACR_MAX;
				Ds2756Api(DS2756_SET_ACCUMULATED_CURRENT,&s32TempAcr);
			}
		}
		else
		{
			if(s32TempVoltage< s_stBatteryVoltagePercent[0x00].u32VoltageInCharge)
			{
				s32TempAcr = BATTERY_VOLTAGE_EMPTY_PERCENT*BATTERY_ACR_MAX/100;
				Ds2756Api(DS2756_SET_ACCUMULATED_CURRENT,&s32TempAcr);				
			}
			else if(s32TempVoltage >= s_stBatteryVoltagePercent[BATTERY_VOLTAGE_PERCENT_BUFSIZE - 0x01].u32VoltageInCharge)
			{
				s32TempAcr = BATTERY_VOLTAGE_FULL_PERCENT*BATTERY_ACR_MAX/100;
				Ds2756Api(DS2756_SET_ACCUMULATED_CURRENT,&s32TempAcr);				
				
			}
			else
				;
		}
	}
	else
	{
		for(s8I = BATTERY_VOLTAGE_PERCENT_BUFSIZE - 0x01; s8I >= 0; s8I--)
		{
			if(s32TempVoltage >= s_stBatteryVoltagePercent[s8I].u32VoltageWork)
			{
				for(s8J= s8I+0x01; s8J < BATTERY_VOLTAGE_PERCENT_BUFSIZE; s8J++)
				{
					if(s32TempVoltage < s_stBatteryVoltagePercent[s8J].u32VoltageWork)
						break;
				}
				break;
			}
		}
		/**���쳣��Χ���д���***/
		if((s8I == (BATTERY_VOLTAGE_PERCENT_BUFSIZE - 0x01))&&(s8J == 0xFF))
		{
			s8J = BATTERY_VOLTAGE_PERCENT_BUFSIZE - 0x01;
			s8I = BATTERY_VOLTAGE_PERCENT_BUFSIZE - 0x02;
		}
		if(s8J != 0xFF)
		{
			u32PercentCalculate = (s_stBatteryVoltagePercent[s8J].u32BatteryPercent - s_stBatteryVoltagePercent[s8I].u32BatteryPercent)*(s32TempVoltage - s_stBatteryVoltagePercent[s8I].u32VoltageWork);
			u32PercentCalculate /= (s_stBatteryVoltagePercent[s8J].u32VoltageWork - s_stBatteryVoltagePercent[s8I].u32VoltageWork);
			u32PercentCalculate += s_stBatteryVoltagePercent[s8I].u32BatteryPercent;	
			if(((u32PercentCalculate >= u32PercentAcr)&&(fabs(u32PercentCalculate-u32PercentAcr)>=50)) \
				||((u32PercentAcr >= u32PercentCalculate)&&(fabs(u32PercentAcr-u32PercentCalculate)>=50)) \
				|| (u8Flag == 0x01))
			{
				s32TempAcr = u32PercentCalculate*BATTERY_ACR_MAX/1000;
				if(s32TempAcr > BATTERY_ACR_MAX)
					s32TempAcr = BATTERY_ACR_MAX;				
				Ds2756Api(DS2756_SET_ACCUMULATED_CURRENT,&s32TempAcr);
			}			
		}
		else
		{
			if(s32TempVoltage< s_stBatteryVoltagePercent[0x00].u32VoltageWork)
			{
				s32TempAcr = BATTERY_VOLTAGE_EMPTY_PERCENT*BATTERY_ACR_MAX/100;
				Ds2756Api(DS2756_SET_ACCUMULATED_CURRENT,&s32TempAcr);				
			}
			else if(s32TempVoltage >= s_stBatteryVoltagePercent[BATTERY_VOLTAGE_PERCENT_BUFSIZE - 0x01].u32VoltageWork)
			{
				s32TempAcr = BATTERY_VOLTAGE_FULL_PERCENT*BATTERY_ACR_MAX/100;
				Ds2756Api(DS2756_SET_ACCUMULATED_CURRENT,&s32TempAcr);				
				
			}
			else
				;
		}
	}
	
	return 0x01;

}		
#endif
/***************************************************************************************
** ��������: BatteryGetCoulomeData
** ������������ȡ��ص�������ֵ
** �������룺��
** ���������None
** ע�������
******************************************************************************************/
u32 BatteryGetCoulomeData(u16 *u16Data)
{
		
//	if(g_u32ID == B_type)
	{
		*u16Data = s_u16BatteryCouloData;
	}
//	else
//	{
//		if(s_stBatteryStatus == BATTERY_ININT||s_stBatteryStatus == BATTERY_START_CHARGED||s_stBatteryStatus == BATTERY_BEING_CHARGED||s_stBatteryStatus == BATTERY_CHARGE_GONNA_OVER||s_stBatteryStatus == BATTERY_CHARGE_OVER)
//		{
//			*u16Data = (s_u16BatteryCouloData>s_u16BatteryCouloData_D? s_u16BatteryCouloData_D:s_u16BatteryCouloData);
//		}
//		else
//		{
//			*u16Data = (s_u16BatteryCouloData>s_u16BatteryCouloData_D? s_u16BatteryCouloData:s_u16BatteryCouloData_D);
//		}
//	}

	return 0x01;	
}

/***************************************************************************************
** ��������: BatteryGetCoulomeData
** ������������ȡ��ص�������ֵ
** �������룺��
** ���������None
** ע�������
******************************************************************************************/
u32 BatteryGetCoulomeData_D(u16 *u16Data)
{
	*u16Data = s_u16BatteryCouloData_D;	
	return 0x01;	
}

/***************************************************************************************
** ��������:BatteryVoltageProc
** �����������ɼ����������ֵ��״̬
** �������룺��
** ���������None
** ע�����
******************************************************************************************/
u32 BatteryGetVoltageTempProc(void)
{
	return s_u32BatteryVoltageBackup;
}


/***************************************************************************************
** ��������:BatteryVoltageProc
** �����������ɼ����������ֵ��״̬
** �������룺��
** ���������None
** ע�����
******************************************************************************************/
u32 BatteryGetVoltageTempProc_B(void)
{
	if(s_u32BatteryVoltageBackup_B>20000)
		return s_u32BatteryVoltageBackup_B;
	else
		return 0;
}


/***************************************************************************************
** ��������:BatteryVoltageProc
** �����������ɼ����������ֵ��״̬
** �������룺��
** ���������None
** ע�����
******************************************************************************************/
u32 BatteryGetVoltageTempProc_D(void)
{
	if(s_u32BatteryVoltageBackup_D>20000)
		return s_u32BatteryVoltageBackup_D;
	else
		return 0;
}

/***************************************************************************************
** ��������:BatteryGetCoulomeStatus
** ������������ȡ��ص�����״̬
** �������룺��
** ���������None
** ע�������
******************************************************************************************/
u32 BatteryGetCoulomeStatus(BATTERY_COULOMETRY_TYPE *pData)
{
	static u16 u16Pointer = 0x00;
	u16 u16I = 0x00;
	
 	switch(s_stBatteryStatus)
	{
		case BATTERY_ININT:
		case BATTERY_FAULT:
		case BATTERY_WORK_NORMAL:
			*pData = s_stBatteryCouloStatus;
			if((s_stBatteryCouloStatus == BATTERY_COULOMETRY_3_PERCENT)||(s_stBatteryCouloStatus == BATTERY_COULOMETRY_5_PERCENT))
			{
				u16Pointer++;
				u16Pointer %= BATTERY_TOOLOW_STATUS_SIZE;
				*pData =  s_stBatteryTooLow[u16Pointer];				
			}				
			break;

		case BATTERY_START_CHARGED:
		case BATTERY_BEING_CHARGED:
		case BATTERY_CHARGE_GONNA_OVER:		
			for(u16I = 0x00; u16I < BATTERY_INCHARGE_STATUS_SIZE;u16I++)
			{
				if(s_stBatteryCouloStatus == s_stBatteryInCharge[u16I])
					break;
			}
			u16Pointer++;
			u16Pointer %= BATTERY_INCHARGE_STATUS_SIZE;
			if((u16Pointer < u16I)&&(u16I != BATTERY_INCHARGE_STATUS_SIZE))
			{
				if(u16I != (BATTERY_INCHARGE_STATUS_SIZE-0x01))
					u16Pointer = u16I;
				else 
					u16Pointer = u16I-0x01;
			}	
			*pData = s_stBatteryInCharge[u16Pointer];
			#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_ADC_MNG)//��Ӳ���У�ʹ��AD��ʽ�������
				s_u32BatteryColumDataInint = 0xFFFFFFFF;//��ֹ�ڳ������жϵ磬��ʾ��׼ȷ��
				s_u32BatteryTaskTimer = 0xFFFFFFFF;
			#endif
			break;

		case BATTERY_CHARGE_OVER:	
			u16Pointer = BATTERY_INCHARGE_STATUS_SIZE-0x01;
			*pData = s_stBatteryInCharge[u16Pointer];
			break;
			
		case BATTERY_TOO_LOW://��ص���̫��״̬
			u16Pointer++;
			u16Pointer %= BATTERY_TOOLOW_STATUS_SIZE;
			*pData =  s_stBatteryTooLow[u16Pointer];
			break;
		
		default:
			break;
		
	}

	return 0x01;	
}

/***************************************************************************************
** ��������:BatteryGetStatus
** ������������ȡ��ص�״̬
** �������룺��
** ���������None
** ע�����
******************************************************************************************/
u32 BatteryGetStatus(BATTERY_STATUS_TYPE *pStatus)
{
	*pStatus = s_stBatteryStatus;

	return 0x01;
}

/***************************************************************************************
** ��������:BatteryGetAcrBackup
** ������������ȡ��ص����ı�������
** �������룺��
** ���������None
** ע�����
******************************************************************************************/
#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_ADC_MNG)//��Ӳ���У�ʹ��ADC��ʽ�������
u32 BatteryGetAcrBackup(u32 *pAcrData)
{
	*pAcrData = s_u32BatteryCurrentTotalBackup;

	return 0x01;
}
#endif
/***************************************************************************************
** ��������:BatteryGetInchargeVoltBackup
** ������������ȡ��ص����ı�������
** �������룺��
** ���������None
** ע�����
******************************************************************************************/
#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_ADC_MNG)//��Ӳ���У�ʹ��ADC��ʽ�������
u32 BatteryGetVoltBackup(u32 *pVoltageData)
{
	*pVoltageData = s_u32BatteryVoltageBackup;

	return 0x01;
}
#endif
/***************************************************************************************
** ��������:BatterySetTotalAcr
** �������������õ�ص����ı�������
** �������룺��
** ���������None
** ע�����
******************************************************************************************/
#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_ADC_MNG)//��Ӳ���У�ʹ��ADC��ʽ�������
u32 BatterySetTotalAcr(u32 u32AcrValue)
{
	s_u32BatteryCurrentTotalBackup = u32AcrValue ;
    s_u32BatteryCurrentTotalInint = u32AcrValue;
	   
	return 0x01;	
}
#endif
/***************************************************************************************
** ��������:BatterySetCoulomeData
** �������������õ�ص�ǰ�����ٷֱ�
** �������룺��
** ���������None
** ע�����
******************************************************************************************/
#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_ADC_MNG)//��Ӳ���У�ʹ��ADC��ʽ�������
u32 BatterySetCoulomeData(u32 u32CoulValue)
{
	u8 u8I = 0x00;
	
	s_u16BatteryCouloData = u32CoulValue ;
	s_u32BatteryColumDataInint = u32CoulValue;
	
	/**�ж������ٷֱȣ���Ҫ������ʾ**/
	for(u8I = 0x00; u8I < BATTERY_COULOMETRY_TYPE_MAX; u8I++)
	{
		if((s_u16BatteryCouloData >= s_stBatteryCalculateList[u8I].u16ValueMin) \
			&& (s_u16BatteryCouloData < s_stBatteryCalculateList[u8I].u16ValueMax))
		{
			s_stBatteryCouloStatus = s_stBatteryCalculateList[u8I].sBatteryColumnStatus;
			s_stBatteryCouloStatusBackup = s_stBatteryCalculateList[u8I].sBatteryColumnStatus;
			break;
		}
	}
							
	return 0x01;	
}
#endif

/***************************************************************************************
** ��������:BatteryGetInchargeTimer
** �����������ض���س���ʱ��
** �������룺��
** ���������None
** ע�����
******************************************************************************************/
#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_ADC_MNG)//��Ӳ���У�ʹ��ADC��ʽ�������
u32 BatteryGetInchargeTimer(u32 *pData)
{
	*pData = s_u32BatteryInchargeTimer;
	
	return 0x01;	
}
#endif
/***************************************************************************************
** ��������: BatteryChongdianPinProc
** �����������ɼ���������CHRG��ֵ��״̬
** �������룺��
** ���������None
** ע�����
******************************************************************************************/
void BatteryChongdianPinProc(void)
{
	INPUT_VALUE_TYPE temp;
	u32 u32Sum = 0x00;
	u32 u32Sum_D = 0x00;
	u32 u32Temp = 0x00;
	
	InputGetValue(BATTERY_CDZT_IN_CHANNEL, &temp);
	
	if(temp == INPUT_HIGH)
	{
		Battery_bgn_charge:	
		#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//��Ӳ���У�ʹ��DS2756��ʽ�������
			if(((s_u16BatteryCouloData < BATTERY_FULL_JUDGE_DATA)||(s_u16BatteryCouloData == 0xFFFF)))
		#else
			if(((s_u16BatteryCouloData < (BATTERY_FULL_JUDGE_DATA/10))||(s_u16BatteryCouloData == 0xFFFF)))
		#endif	
			{
				s_u32BatteryColumDataInint = 0xFFFFFFFF;
				
				if(s_stBatteryStatus == BATTERY_START_CHARGED)
					s_stBatteryStatus = BATTERY_BEING_CHARGED;
				if(((s_stBatteryStatus != BATTERY_START_CHARGED))&&((s_stBatteryStatus != BATTERY_BEING_CHARGED))\
					&&(s_stBatteryStatus != BATTERY_CHARGE_GONNA_OVER)&&(s_stBatteryStatus != BATTERY_CHARGE_OVER))
				{
					s_stBatteryStatus = BATTERY_START_CHARGED;	
					#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//��Ӳ���У�ʹ��DS2756��ʽ�������
						s_u32BatteryInchargeTimer = 0;
					#else
						s_u32BatteryInchargeTimer = 0;
						while(temp == INPUT_HIGH)
						{

							OSTimeDly(1000/TICK_TIME);
							s_stBatteryStatus = BATTERY_BEING_CHARGED;	
							s_u32BatteryInchargeTimer += 1000;
//							if(g_u32ID == B_type)
							{
								if(s_u32BatteryInchargeTimer >= BATTERY_INCHARGE_TIME_DELAY)
								{								
									break;		
								}
							}
//							else
//							{
//								if(s_u32BatteryInchargeTimer >= BATTERY_INCHARGE_TIME_DELAY_D)
//								{								
//									break;		
//								}
//							}
							InputGetValue(BATTERY_CDZT_IN_CHANNEL, &temp);
							if(temp == INPUT_LOW)//�ֶϿ����
							{
								goto Battery_charge_lineoff;
							}
						}
						s_u32BatteryInchargeTimer = 0;
					#endif
				}			
			}
		#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//��Ӳ���У�ʹ��DS2756��ʽ�������	
			else if(s_u16BatteryCouloData >= BATTERY_FULL_JUDGE_DATA)
		#else
			else if(s_u16BatteryCouloData >= (BATTERY_FULL_JUDGE_DATA/10))	
		#endif	
			{
				s_stBatteryStatus = BATTERY_CHARGE_OVER;
			}
			else
				;
	}
	else
	{
		Battery_charge_lineoff:	
		if((s_stBatteryStatus == BATTERY_START_CHARGED)||(s_stBatteryStatus == BATTERY_BEING_CHARGED)\
			||(s_stBatteryStatus == BATTERY_CHARGE_GONNA_OVER)||(s_stBatteryStatus == BATTERY_CHARGE_OVER))
		{
			#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//��Ӳ���У�ʹ��DS2756��ʽ�������			
				s_u32BatteryInchargeTimer = 0xFFFFFFFF;	
			#else
				s_u32BatteryInchargeTimer = 0;
				if(s_u16BatteryCouloData <= BATTERY_VOLTAGE_8_PERCENT)
					s_stBatteryStatus = BATTERY_TOO_LOW;
				else
					s_stBatteryStatus = BATTERY_WORK_NORMAL;	
				while(temp == INPUT_LOW)
				{
					OSTimeDly(1000/TICK_TIME);	
					s_u32BatteryInchargeTimer += 1000;
//					if(g_u32ID == B_type)
					{
						if(s_u32BatteryInchargeTimer >= BATTERY_LINEOFF_TIME_DELAY)
						{						
							break;		
						}
					}				
//					else
//					{
//						if(s_u32BatteryInchargeTimer >= BATTERY_LINEOFF_TIME_DELAY_D)
//						{								
//							break;		
//						}
//					}
					InputGetValue(BATTERY_CDZT_IN_CHANNEL, &temp);
					if(temp == INPUT_HIGH)//�ֿ�ʼ���
					{
						goto Battery_bgn_charge;
					}
					u32Sum = 0x00;
					u32Sum_D = 0x00;					
					GetAdcAverageValue(BATTERY_VOLTAGE_AD_CHANNEL,(u16 *)&u32Sum);
					GetAdcAverageValue(CPU_VREFINT_AD_CHANNEL,(u16 *)&u32Temp);
					
					if(g_u32ID != B_type)
					{
						GetAdcAverageValue(BATTERY_VOLTAGE_AD_CHANNEL_1,(u16 *)&u32Sum_D);
						u32Sum = (u32Sum>u32Sum_D ? u32Sum_D:u32Sum);
					}
					
					
					if((u32Sum > AD_SAMPLE_MIN) && (u32Sum <=  AD_SAMPLE_MAX))//��ֹ�ڴ˽׶ε�ѹ����
					{
						
						u32Sum *= s_u32AdVerfintDefault;
						u32Sum /= u32Temp;
						if((u32Sum%10) >= 0x05)
						{
							u32Sum /= 10;//��λ0.1mV
							u32Sum += 1;//��λ0.1mV
						}
						else
						{
							u32Sum /= 10;//��λ0.1mV
						}
						if(g_u32ID == B_type)
							u32Sum -= BATTERY_AD_WRAP;//��������ʵ��ƫ��20mV	
								
						if(g_u32ID == B_type)
						{
							if(u32Sum <= s_stBatteryVoltagePercent[0x01].u32VoltageWork)
							{
								break;
							}
						}
						else
						{
							u32Sum  = u32Sum/2*3;
							if(u32Sum <= s_stBatteryVoltagePercent_D[0x01].u32VoltageWork)
							{
								break;
							}	
						}
					}
					else 
					{
						break;
					}
				}
				s_u32BatteryInchargeTimer = 0xFFFFFFFF;		
			#endif		
		}
		if(s_u16BatteryCouloData <= BATTERY_VOLTAGE_8_PERCENT)
		{
			s_stBatteryStatus = BATTERY_TOO_LOW;
		}
		else
		{
			s_stBatteryStatus = BATTERY_WORK_NORMAL;
		}
		
	}
}

/***************************************************************************************
** ��������: BatteryVoltageProc
** �����������ɼ����������ֵ��״̬
** �������룺��
** ���������None
** ע�����
******************************************************************************************/
#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//��Ӳ���У�ʹ��DS2756��ʽ�������
	void BatteryVoltageProc(u32 u32Timer)
	{
		s32 s32Temp = 0x00;
		s8 s8I = 0x00;
		INPUT_VALUE_TYPE temp;
		
		/**��ȡƽ������ֵ**/
		Ds2756Api(DS2756_READ_CURRENT,&s32Temp);
		s32BatteryCurrentBuf[s_u16BatteryCurtBufpointer] = s32Temp;
		s_u16BatteryCurtBufpointer++ ;	
		if(s_u16BatteryCurtBufpointer == BATTERY_SAMPLE_BUF_SIZE)
		{
			s32BatteryCurrentValue = FilterAndAverage(s32BatteryCurrentBuf,BATTERY_SAMPLE_BUF_SIZE);
			if((s_stBatteryStatus == BATTERY_START_CHARGED)||(s_stBatteryStatus == BATTERY_BEING_CHARGED)\
				||(s_stBatteryStatus == BATTERY_CHARGE_GONNA_OVER))
			{
				s_u32BatteryInchargeTimer += u32Timer*BATTERY_SAMPLE_BUF_SIZE;
				if((s_u32BatteryInchargeTimer >= BATTERY_INCHARGE_SAMPLE_DELAY)&&(s32BatteryCurrentValue != 0x00)&&(s32BatteryCurrentValue <= 70))//��������̫Сʱ��Ĭ�ϳ�����
				{
					InputGetValue(BATTERY_CDZT_IN_CHANNEL, &temp);
					Ds2756Api(DS2756_READ_VOLTAGE,&s32Temp);
					if((temp == INPUT_HIGH)&&(s32Temp >= BATTERY_FULL_VOLTAGE_BASE))
					{						
						s_stBatteryStatus = BATTERY_CHARGE_OVER;
						s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
						s32Temp = BATTERY_VOLTAGE_FULL_PERCENT*BATTERY_ACR_MAX/100;
						Ds2756Api(DS2756_SET_ACCUMULATED_CURRENT,&s32Temp);		
					}						
				}
			}				
		}
		s_u16BatteryCurtBufpointer %= BATTERY_SAMPLE_BUF_SIZE;

		
		/**��ȡ��ѹֵ**/
		Ds2756Api(DS2756_READ_VOLTAGE,&s32Temp);
		s32Temp *= 10;
		s32BatteryVoltBuf[s_u16BatteryVoltBufpointer] = s32Temp;
		s_u16BatteryVoltBufpointer++ ;	
		if(s_u16BatteryVoltBufpointer == BATTERY_SAMPLE_BUF_SIZE)
		{
			s32BatteryVoltValue = FilterAndAverage(s32BatteryVoltBuf,BATTERY_SAMPLE_BUF_SIZE);
			if(s32BatteryVoltValue&&(s32BatteryVoltValue <= s_stBatteryVoltagePercent[0x00].u32VoltageWork))
			{
				if((s_stBatteryStatus != BATTERY_START_CHARGED)&&(s_stBatteryStatus != BATTERY_BEING_CHARGED)\
				&&(s_stBatteryStatus != BATTERY_CHARGE_GONNA_OVER)&&((s_stBatteryStatus != BATTERY_CHARGE_OVER)))
				{			
					s_stBatteryStatus = BATTERY_TOO_LOW;
					s_stBatteryCouloStatus = BATTERY_COULOMETRY_EMPTY;
					s_u16BatteryCouloData = BATTERY_VOLTAGE_3_PERCENT;
					s32Temp = 0x00;
					Ds2756Api(DS2756_SET_ACCUMULATED_CURRENT,&s32Temp);
				}
			}				

		}
		s_u16BatteryVoltBufpointer %= BATTERY_SAMPLE_BUF_SIZE;
		
		/**��ȡ�ۼƵ���ֵ**/
		Ds2756Api(DS2756_READ_ACCUMULATED_CURRENT,&s32Temp);
		s32BatteryAccCurrentBuf[s_u16BatteryAccCurtBufpointer] = s32Temp;
		if(s_u32BatteryCalTimes <= BATTERY_SAMPLE_INVALID_TIMESSET)
		{	
			s_u32BatteryCalTimes++;		
			if((s32Temp > 0) && (s32Temp <= BATTERY_ACR_MAX)&&(s_u16BatteryCouloData == 100))
			{
				s_u16BatteryCouloData = (u16)(s32Temp*100/BATTERY_ACR_MAX);
				/**�ж������ٷֱȣ���Ҫ������ʾ**/
				for(s8I = 0x00; s8I < BATTERY_COULOMETRY_TYPE_MAX; s8I++)
				{
					if((s_u16BatteryCouloData >= s_stBatteryCalculateList[s8I].u16ValueMin) && (s_u16BatteryCouloData < s_stBatteryCalculateList[s8I].u16ValueMax))
					{
						s_stBatteryCouloStatus = s_stBatteryCalculateList[s8I].sBatteryColumnStatus;
						break;
					}
				}				
			}
			else if((s32Temp < 0)&&(s_u16BatteryCouloData == 100))
			{
				BatteryReRalculateCurrent();
				Ds2756Api(DS2756_READ_ACCUMULATED_CURRENT,&s32Temp);
				s32BatteryAccCurrentBuf[s_u16BatteryAccCurtBufpointer] = s32Temp;				
			}
			else
				;
		}
		s_u16BatteryAccCurtBufpointer++ ;	
		if(s_u16BatteryAccCurtBufpointer == BATTERY_SAMPLE_BUF_SIZE)
		{
			s32BatteryAccCurtValue = FilterAndAverage(s32BatteryAccCurrentBuf,BATTERY_SAMPLE_BUF_SIZE);
			if((s32BatteryAccCurtValue > 0))
			{				
				if((s32BatteryAccCurtValue > BATTERY_ACR_MAX))
				{
					Ds2756Api(DS2756_READ_CURRENT,&s32Temp);
					if((s_stBatteryStatus == BATTERY_START_CHARGED)||(s_stBatteryStatus == BATTERY_BEING_CHARGED)\
						||(s_stBatteryStatus == BATTERY_CHARGE_GONNA_OVER)||(s_stBatteryStatus == BATTERY_CHARGE_OVER))
					{
						if(s32Temp > 70)
						{
							s_stBatteryStatus = BATTERY_CHARGE_OVER;
							s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
							s32Temp = BATTERY_VOLTAGE_FULL_PERCENT*BATTERY_ACR_MAX/100;
							Ds2756Api(DS2756_SET_ACCUMULATED_CURRENT,&s32Temp);		
						}
					}
					else
					{
						s_stBatteryStatus = BATTERY_CHARGE_OVER;
						s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
						s32Temp = BATTERY_VOLTAGE_FULL_PERCENT*BATTERY_ACR_MAX/100;
						Ds2756Api(DS2756_SET_ACCUMULATED_CURRENT,&s32Temp);					
					}
				}		
				if(s_stBatteryStatus != BATTERY_CHARGE_OVER)
					s_u16BatteryCouloData = (u16)(s32BatteryAccCurtValue*100/BATTERY_ACR_MAX);
				if((s_u16BatteryCouloData <= BATTERY_VOLTAGE_3_PERCENT)&&(s_stBatteryStatus == BATTERY_WORK_NORMAL))
					s_stBatteryStatus = BATTERY_TOO_LOW;
				
				/**�ж������ٷֱȣ���Ҫ������ʾ**/
				for(s8I = 0x00; s8I < BATTERY_COULOMETRY_TYPE_MAX; s8I++)
				{
					if((s_u16BatteryCouloData >= s_stBatteryCalculateList[s8I].u16ValueMin) && (s_u16BatteryCouloData < s_stBatteryCalculateList[s8I].u16ValueMax))
					{
						s_stBatteryCouloStatus = s_stBatteryCalculateList[s8I].sBatteryColumnStatus;
						break;
					}
				}
				
				if((s_stBatteryStatus == BATTERY_START_CHARGED)||(s_stBatteryStatus == BATTERY_CHARGE_GONNA_OVER))
				{	
					if(s_u16BatteryCouloData >= BATTERY_VOLTAGE_FULL_PERCENT)
					{
						s_stBatteryStatus = BATTERY_CHARGE_OVER;	
					}				
					else if(s_u16BatteryCouloData >= BATTERY_VOLTAGE_90_PERCENT)
					{
						s_stBatteryStatus = BATTERY_CHARGE_GONNA_OVER;	
					}
				}	
			}
			else
			{
				if((s_stBatteryStatus != BATTERY_START_CHARGED)&&(s_stBatteryStatus != BATTERY_BEING_CHARGED)\
					&&(s_stBatteryStatus != BATTERY_CHARGE_GONNA_OVER)&&(s_stBatteryStatus != BATTERY_CHARGE_OVER))
				{	
					Ds2756Api(DS2756_READ_VOLTAGE,&s32Temp);//��ȡ��ѹ������У��
					s32Temp *= 10;
					if((s32Temp != 0x00)&&(s32Temp <= s_stBatteryVoltagePercent[0x00].u32VoltageWork))
					{	
						s_u16BatteryCouloData = BATTERY_VOLTAGE_EMPTY_PERCENT;
						if((s_u16BatteryCouloData <= BATTERY_VOLTAGE_3_PERCENT)&&(s_stBatteryStatus == BATTERY_WORK_NORMAL))
							s_stBatteryStatus = BATTERY_TOO_LOW;					
					}
					else
					{
						BatteryReRalculateCurrent();
					}
				}				
			}
			
		}		
		s_u16BatteryAccCurtBufpointer %= BATTERY_SAMPLE_BUF_SIZE;
		if((s_u32BatteryColumDataInint == 0xFFFFFFFF)&&(s_u16BatteryCouloData <= BATTERY_VOLTAGE_FULL_PERCENT))//������ֵδ��ʼ��
		{
			s_u32BatteryColumDataInint = s_u16BatteryCouloData;
		}
		
		/**�ж������ٷֱȣ���Ҫ������ʾ**/
		for(s8I = 0x00; s8I < BATTERY_COULOMETRY_TYPE_MAX; s8I++)
		{
			if((s_u16BatteryCouloData >= s_stBatteryCalculateList[s8I].u16ValueMin) && (s_u16BatteryCouloData < s_stBatteryCalculateList[s8I].u16ValueMax))
			{
				s_stBatteryCouloStatus = s_stBatteryCalculateList[s8I].sBatteryColumnStatus;
				break;
			}
		}
			
		/**�ж��ڹ���ʱ�Ƿ�ȳ�ʼֵ����***/
		if(s_stBatteryStatus == BATTERY_WORK_NORMAL)
		{
			if((s_u16BatteryCouloData > s_u32BatteryColumDataInint)&&(s_u32BatteryColumDataInint != 0xFFFFFFFF))	//��������ģʽ�£������ܱȳ�ʼֵ����
			{
				s_u16BatteryCouloData  = s_u32BatteryColumDataInint;
				s_u32BatteryCurrentTotalBackup = s_u32BatteryCurrentTotalInint;
			}

			if(( s_stBatteryCouloStatus != s_stBatteryCouloStatusBackup)&&(s_u16BatteryCouloDataBackup != 0xFFFF)
				&&(s_stBatteryCouloStatus != BATTERY_COULOMETRY_5_PERCENT)&&(s_stBatteryCouloStatus != BATTERY_COULOMETRY_3_PERCENT)\
				&&(s_stBatteryCouloStatus != BATTERY_COULOMETRY_EMPTY)&&(s_stBatteryCouloStatus != BATTERY_COULOMETRY_10_PERCENT)\
				&&(s_stBatteryCouloStatusBackup != BATTERY_COULOMETRY_5_PERCENT)&&(s_stBatteryCouloStatusBackup != BATTERY_COULOMETRY_3_PERCENT)\
				&&(s_stBatteryCouloStatusBackup != BATTERY_COULOMETRY_EMPTY))
			{
				s_u32BatteryChangeTimer += BATTERY_SAMPLE_TIME_SET;
				if(s_u32BatteryChangeTimer >= BATTERY_COLUMDATA_CHANGE_DELAY)
				{
					;	
				}	
				else
				{
					s_u16BatteryCouloData = s_u16BatteryCouloDataBackup;
					s_u32BatteryCurrentTotalBackup = s_u32BatteryCurrentTotalBackupBkp;
					s_u32BatteryVoltageBackup = s_u32BatteryVoltageBackupBkp;
					s_stBatteryCouloStatus = s_stBatteryCouloStatusBackup;
				}
			}
			else
			{
				s_u32BatteryChangeTimer = 0x00;
			}
		}	
		s_u16BatteryCouloDataBackup = s_u16BatteryCouloData;
		s_stBatteryCouloStatusBackup = s_stBatteryCouloStatus;
		
		/**�ж������ٷֱȣ���Ҫ������ʾ**/
		for(s8I = 0x00; s8I < BATTERY_COULOMETRY_TYPE_MAX; s8I++)
		{
			if((s_u16BatteryCouloData >= s_stBatteryCalculateList[s8I].u16ValueMin) && (s_u16BatteryCouloData < s_stBatteryCalculateList[s8I].u16ValueMax))
			{
				s_stBatteryCouloStatus = s_stBatteryCalculateList[s8I].sBatteryColumnStatus;
				break;
			}
		}
			
		/**��ȡ�¶�**/
		Ds2756Api(DS2756_READ_TEMPERATURE,&s32Temp);
		s32BatteryTempBuf[s_u16BatteryTempBufpointer] = s32Temp;
		s_u16BatteryTempBufpointer++ ;	
		if(s_u16BatteryTempBufpointer == BATTERY_SAMPLE_BUF_SIZE)
		{
			s32BatteryTempValue = FilterAndAverage(s32BatteryTempBuf,BATTERY_SAMPLE_BUF_SIZE);	
			if((s32BatteryTempValue > BATTERY_INCHARGE_TEMPER_MAX))
			{
				s_stBatteryStatus = BATTERY_FAULT;			
			}			
		}
		s_u16BatteryTempBufpointer %= BATTERY_SAMPLE_BUF_SIZE;		

	}
#elif (YKQ_HARDWARE_BATTERY_MNG == BATTERY_ADC_MNG)//��Ӳ���У�ʹ��AD��ʽ�������
/***************************************************************************************
** ��������: BatteryVoltageInintProc
** �����������ɼ���������CHRG��ֵ��״̬,�ڳ�ʼ��״̬
** �������룺��
** ���������None
** ע�����
******************************************************************************************/
u16 BatteryVoltageInintProc(u32 u32VoltageValue)
{
	s8 s8I = 0x00;
	s8 s8J = 0x00;
	u32 u32Sum = u32VoltageValue;
	u32 u32Temp = 0x00;
	
	InputGetValue(BATTERY_CDZT_IN_CHANNEL, (INPUT_VALUE_TYPE *)&u32Temp);
	if(((s_stBatteryStatus >= BATTERY_START_CHARGED) && (s_stBatteryStatus <= BATTERY_CHARGE_OVER))\
		||((s_stBatteryStatus == BATTERY_ININT)&&(u32Temp == INPUT_HIGH)))
	{
		s_u32BatteryVoltageBackup = u32Sum;
		
		if(g_u32ID == B_type)
		{
			for(s8I = BATTERY_VOLTAGE_PERCENT_BUFSIZE - 0x01; s8I >= 0; s8I--)
			{
				if(s_u32BatteryVoltageBackup >= s_stBatteryVoltagePercent[s8I].u32VoltageInCharge)
				{
					for(s8J= s8I+0x01; s8J < BATTERY_VOLTAGE_PERCENT_BUFSIZE; s8J++)
					{
						if(s_u32BatteryVoltageBackup < s_stBatteryVoltagePercent[s8J].u32VoltageInCharge)
							break;
					}	
					if(s8J < BATTERY_VOLTAGE_PERCENT_BUFSIZE)
					{
						u32Temp = (s_stBatteryVoltagePercent[s8J].u32BatteryPercent - s_stBatteryVoltagePercent[s8I].u32BatteryPercent)*(s_u32BatteryVoltageBackup - s_stBatteryVoltagePercent[s8I].u32VoltageInCharge);
						u32Temp /= (s_stBatteryVoltagePercent[s8J].u32VoltageInCharge - s_stBatteryVoltagePercent[s8I].u32VoltageInCharge);
						u32Temp += s_stBatteryVoltagePercent[s8I].u32BatteryPercent;
						s_u32BatteryCurrentTotalBackup = u32Temp*BATTERY_CURRENT_MAX;
						s_u32BatteryCurrentTotalBackup /= 1000;
						if(u32Temp >= BATTERY_FULL_JUDGE_DATA)
						{
							s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
							s_u32BatteryCurrentTotalBackup = BATTERY_CURRENT_MAX;							
						}
						else
						{
							u32Temp/= 10;
							s_u16BatteryCouloData = (u16)u32Temp;							
						}
						break;
					}
					else if(s8J == BATTERY_VOLTAGE_PERCENT_BUFSIZE)//u8IΪ�����е����һ��
					{
						if(s_u32BatteryVoltageBackup >= s_stBatteryVoltagePercent[BATTERY_VOLTAGE_PERCENT_BUFSIZE-0x01].u32VoltageInCharge)	
						{
							s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
							s_u32BatteryCurrentTotalBackup = BATTERY_CURRENT_MAX;						
							break;
						}
					}
					else
						;							
				}
			}
			
			/**�Գ���쳣������д���**/
			if(s8I < 0)
			{
				if((s_u32BatteryVoltageBackup >= s_stBatteryVoltagePercent[BATTERY_VOLTAGE_PERCENT_BUFSIZE-0x01].u32VoltageInCharge)&&(s_u32BatteryVoltageBackup != 0xFFFF))
				{
					s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
					s_u32BatteryCurrentTotalBackup = BATTERY_CURRENT_MAX;	
					s_stBatteryCouloStatus = BATTERY_COULOMETRY_100_PERCENT;
				}
				else if((s_u32BatteryVoltageBackup < s_stBatteryVoltagePercent[0].u32VoltageInCharge)||(s_u32BatteryVoltageBackup != 0xFFFF))
				{
					s_u16BatteryCouloData = BATTERY_VOLTAGE_EMPTY_PERCENT;
					s_u32BatteryCurrentTotalBackup = 0;	
					s_stBatteryCouloStatus = BATTERY_COULOMETRY_EMPTY;
				}
				else
					;					
			}
			else
			{
				/**�ж������ٷֱȣ���Ҫ������ʾ**/
				for(s8I = 0x00; s8I < BATTERY_COULOMETRY_TYPE_MAX; s8I++)
				{
					if((s_u16BatteryCouloData >= s_stBatteryCalculateList[s8I].u16ValueMin) && (s_u16BatteryCouloData < s_stBatteryCalculateList[s8I].u16ValueMax))
					{
						s_stBatteryCouloStatus = s_stBatteryCalculateList[s8I].sBatteryColumnStatus;
						break;
					}
				}							
			}
		}
		else
		{
			for(s8I = BATTERY_VOLTAGE_PERCENT_BUFSIZE - 0x01; s8I >= 0; s8I--)
			{
				if(s_u32BatteryVoltageBackup >= s_stBatteryVoltagePercent_D[s8I].u32VoltageInCharge)
				{
					for(s8J= s8I+0x01; s8J < BATTERY_VOLTAGE_PERCENT_BUFSIZE; s8J++)
					{
						if(s_u32BatteryVoltageBackup < s_stBatteryVoltagePercent_D[s8J].u32VoltageInCharge)
							break;
					}	
					if(s8J < BATTERY_VOLTAGE_PERCENT_BUFSIZE)
					{
						u32Temp = (s_stBatteryVoltagePercent_D[s8J].u32BatteryPercent - s_stBatteryVoltagePercent_D[s8I].u32BatteryPercent)*(s_u32BatteryVoltageBackup - s_stBatteryVoltagePercent_D[s8I].u32VoltageInCharge);
						u32Temp /= (s_stBatteryVoltagePercent_D[s8J].u32VoltageInCharge - s_stBatteryVoltagePercent_D[s8I].u32VoltageInCharge);
						u32Temp += s_stBatteryVoltagePercent_D[s8I].u32BatteryPercent;
						s_u32BatteryCurrentTotalBackup = u32Temp*BATTERY_CURRENT_MAX_D;
						s_u32BatteryCurrentTotalBackup /= 1000;
						if(u32Temp >= BATTERY_FULL_JUDGE_DATA)
						{
							s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
							s_u32BatteryCurrentTotalBackup = BATTERY_CURRENT_MAX_D;							
						}
						else
						{
							u32Temp/= 10;
							s_u16BatteryCouloData = (u16)u32Temp;							
						}
						break;
					}
					else if(s8J == BATTERY_VOLTAGE_PERCENT_BUFSIZE)//u8IΪ�����е����һ��
					{
						if(s_u32BatteryVoltageBackup >= s_stBatteryVoltagePercent_D[BATTERY_VOLTAGE_PERCENT_BUFSIZE-0x01].u32VoltageInCharge)	
						{
							s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
							s_u32BatteryCurrentTotalBackup = BATTERY_CURRENT_MAX;						
							break;
						}
					}
					else
						;							
				}
			}
			/**�Գ���쳣������д���**/
			if(s8I < 0)
			{
				if((s_u32BatteryVoltageBackup >= s_stBatteryVoltagePercent_D[BATTERY_VOLTAGE_PERCENT_BUFSIZE-0x01].u32VoltageInCharge)&&(s_u32BatteryVoltageBackup != 0xFFFF))
				{
					s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
					s_u32BatteryCurrentTotalBackup = BATTERY_CURRENT_MAX_D;	
					s_stBatteryCouloStatus = BATTERY_COULOMETRY_100_PERCENT;
				}
				else if((s_u32BatteryVoltageBackup < s_stBatteryVoltagePercent_D[0].u32VoltageInCharge)||(s_u32BatteryVoltageBackup != 0xFFFF))
				{
					s_u16BatteryCouloData = BATTERY_VOLTAGE_EMPTY_PERCENT;
					s_u32BatteryCurrentTotalBackup = 0;	
					s_stBatteryCouloStatus = BATTERY_COULOMETRY_EMPTY;
				}
				else
					;					
			}
			else
			{
				/**�ж������ٷֱȣ���Ҫ������ʾ**/
				for(s8I = 0x00; s8I < BATTERY_COULOMETRY_TYPE_MAX; s8I++)
				{
					if((s_u16BatteryCouloData >= s_stBatteryCalculateList[s8I].u16ValueMin) && (s_u16BatteryCouloData < s_stBatteryCalculateList[s8I].u16ValueMax))
					{
						s_stBatteryCouloStatus = s_stBatteryCalculateList[s8I].sBatteryColumnStatus;
						break;
					}
				}							
			}			
		}
		return  0x01;
	}
	return 0x00;
}


/***************************************************************************************
** ��������: BatteryVoltageIntegralProc
** �����������������ݻ������ѹ
** �������룺��
** ���������None
** ע�����
******************************************************************************************/
void BatteryVoltageIntegralProc(u32 u32Sum)
{

	s8 s8I = 0x00;
	s8 s8J = 0x00;
	u32 u32Temp = 0x00;
	
	for(s8I = BATTERY_VOLTAGE_PERCENT_BUFSIZE - 0x01; s8I >= 0; s8I--)
	{
		if((s_stBatteryStatus == BATTERY_WORK_NORMAL) || (s_stBatteryStatus == BATTERY_TOO_LOW) || (s_stBatteryStatus == BATTERY_ININT))
		{
			s_u32BatteryVoltageBackup = u32Sum;			
			if(g_u32ID == B_type)//B��
			{
				if((u32Sum >= s_stBatteryVoltagePercent[s8I].u32VoltageWork))
				{
					for(s8J= s8I+0x01; s8J < BATTERY_VOLTAGE_PERCENT_BUFSIZE; s8J++)
					{
						if(u32Sum < s_stBatteryVoltagePercent[s8J].u32VoltageWork)
							break;
					}
					if(s8J < BATTERY_VOLTAGE_PERCENT_BUFSIZE)
					{
						u32Sum = (s_stBatteryVoltagePercent[s8J].u32BatteryPercent - s_stBatteryVoltagePercent[s8I].u32BatteryPercent)*(u32Sum - s_stBatteryVoltagePercent[s8I].u32VoltageWork);
						u32Sum /= (s_stBatteryVoltagePercent[s8J].u32VoltageWork - s_stBatteryVoltagePercent[s8I].u32VoltageWork);
						u32Sum += s_stBatteryVoltagePercent[s8I].u32BatteryPercent;
						s_u32BatteryCurrentTotalBackup = u32Sum*BATTERY_CURRENT_MAX;
						s_u32BatteryCurrentTotalBackup /= 1000;
						if(u32Sum >= BATTERY_FULL_JUDGE_DATA)
						{
							s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
							s_u32BatteryCurrentTotalBackup = BATTERY_CURRENT_MAX;							
						}
						else
						{
							u32Sum/= 10;
							s_u16BatteryCouloData = (u16)u32Sum;		
						}
						break;
					}
					else if(s8J == BATTERY_VOLTAGE_PERCENT_BUFSIZE)//u8IΪ�����е����һ��
					{
						if(u32Sum >= s_stBatteryVoltagePercent[BATTERY_VOLTAGE_PERCENT_BUFSIZE-0x01].u32VoltageWork)	
						{
							s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
							s_u32BatteryCurrentTotalBackup = BATTERY_CURRENT_MAX;
							break;
						}
					}
					else
						;
				}
			}
			else//D��
			{
				if((u32Sum >= s_stBatteryVoltagePercent_D[s8I].u32VoltageWork))
				{
					for(s8J= s8I+0x01; s8J < BATTERY_VOLTAGE_PERCENT_BUFSIZE; s8J++)
					{
						if(u32Sum < s_stBatteryVoltagePercent_D[s8J].u32VoltageWork)
							break;
					}
					if(s8J < BATTERY_VOLTAGE_PERCENT_BUFSIZE)
					{
						u32Sum = (s_stBatteryVoltagePercent_D[s8J].u32BatteryPercent - s_stBatteryVoltagePercent_D[s8I].u32BatteryPercent)*(u32Sum - s_stBatteryVoltagePercent_D[s8I].u32VoltageWork);
						u32Sum /= (s_stBatteryVoltagePercent_D[s8J].u32VoltageWork - s_stBatteryVoltagePercent_D[s8I].u32VoltageWork);
						u32Sum += s_stBatteryVoltagePercent_D[s8I].u32BatteryPercent;
						s_u32BatteryCurrentTotalBackup = u32Sum*BATTERY_CURRENT_MAX_D;
						s_u32BatteryCurrentTotalBackup /= 1000;
						if(u32Sum >= BATTERY_FULL_JUDGE_DATA)
						{
							s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
							s_u32BatteryCurrentTotalBackup = BATTERY_CURRENT_MAX_D;							
						}
						else
						{
							u32Sum/= 10;
							s_u16BatteryCouloData = (u16)u32Sum;		
						}
						break;
					}
					else if(s8J == BATTERY_VOLTAGE_PERCENT_BUFSIZE)//u8IΪ�����е����һ��
					{
						if(u32Sum >= s_stBatteryVoltagePercent_D[BATTERY_VOLTAGE_PERCENT_BUFSIZE-0x01].u32VoltageWork)	
						{
							s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
							s_u32BatteryCurrentTotalBackup = BATTERY_CURRENT_MAX_D;
							break;
						}
					}
					else
						;
				}		
			}
		}
		else if((s_stBatteryStatus >= BATTERY_START_CHARGED) && (s_stBatteryStatus <= BATTERY_CHARGE_OVER))
		{
			s_u32BatteryVoltageBackup = u32Sum;
			u32Temp =s_u32BatteryCurrentTotalBackup*1000;

			
			if(g_u32ID == B_type)//B��
			{
				u32Temp /=  BATTERY_CURRENT_MAX;//ǧ��֮��
				if((u32Temp >= s_stBatteryVoltagePercent[s8I].u32BatteryPercent))
				{
					for(s8J= s8I+0x01; s8J < BATTERY_VOLTAGE_PERCENT_BUFSIZE; s8J++)
					{
						if(u32Temp < s_stBatteryVoltagePercent[s8J].u32BatteryPercent)
							break;
					}
					if(s8J < BATTERY_VOLTAGE_PERCENT_BUFSIZE)
					{
						if(s_stBatteryVoltagePercent[s8I].u32CurrentIncharge >= s_stBatteryVoltagePercent[s8J].u32CurrentIncharge)
							u32Temp = (s_stBatteryVoltagePercent[s8I].u32CurrentIncharge - s_stBatteryVoltagePercent[s8J].u32CurrentIncharge)*(u32Temp - s_stBatteryVoltagePercent[s8I].u32BatteryPercent);
						else
							u32Temp = (s_stBatteryVoltagePercent[s8J].u32CurrentIncharge - s_stBatteryVoltagePercent[s8I].u32CurrentIncharge)*(u32Temp - s_stBatteryVoltagePercent[s8I].u32BatteryPercent);
							
						u32Temp /= (s_stBatteryVoltagePercent[s8J].u32BatteryPercent - s_stBatteryVoltagePercent[s8I].u32BatteryPercent);
						u32Temp += s_stBatteryVoltagePercent[s8J].u32CurrentIncharge;							
							
						if(s_u32BatteryInchargeTimer >= BATTERY_INCHARGE_SAMPLE_DELAY)
						{	
							u32Temp *= (BATTERY_INCHARGE_SAMPLE_DELAY/30000);
							s_u32BatteryCurrentTotalBackup += u32Temp;
						}							
						u32Temp = s_u32BatteryCurrentTotalBackup*1000;
						u32Temp /= BATTERY_CURRENT_MAX;//ǧ��֮��
						if(u32Temp >= BATTERY_FULL_JUDGE_DATA)
						{
							s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
							s_u32BatteryCurrentTotalBackup = BATTERY_CURRENT_MAX;									
						}
						else
						{
							s_u16BatteryCouloData = (u16)(u32Temp/10);													
						}
						break;
					}
					else if(s8J == BATTERY_VOLTAGE_PERCENT_BUFSIZE)//u8IΪ�����е����һ��
					{
						if(u32Temp >= s_stBatteryVoltagePercent[BATTERY_VOLTAGE_PERCENT_BUFSIZE-0x01].u32BatteryPercent)	
						{
							s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
							s_u32BatteryCurrentTotalBackup = BATTERY_CURRENT_MAX;
							s_u32BatteryInchargeTimer = 0xFFFFFFFF;
							break;
						}
					}
					else
						;
				}
			}
			else//D��
			{
				u32Temp /=  BATTERY_CURRENT_MAX_D;//ǧ��֮��
				if((u32Temp >= s_stBatteryVoltagePercent_D[s8I].u32BatteryPercent))
				{
					for(s8J= s8I+0x01; s8J < BATTERY_VOLTAGE_PERCENT_BUFSIZE; s8J++)
					{
						if(u32Temp < s_stBatteryVoltagePercent_D[s8J].u32BatteryPercent)
							break;
					}
					if(s8J < BATTERY_VOLTAGE_PERCENT_BUFSIZE)
					{
						if(s_stBatteryVoltagePercent[s8I].u32CurrentIncharge >= s_stBatteryVoltagePercent_D[s8J].u32CurrentIncharge)
							u32Temp = (s_stBatteryVoltagePercent_D[s8I].u32CurrentIncharge - s_stBatteryVoltagePercent_D[s8J].u32CurrentIncharge)*(u32Temp - s_stBatteryVoltagePercent_D[s8I].u32BatteryPercent);
						else
							u32Temp = (s_stBatteryVoltagePercent_D[s8J].u32CurrentIncharge - s_stBatteryVoltagePercent_D[s8I].u32CurrentIncharge)*(u32Temp - s_stBatteryVoltagePercent_D[s8I].u32BatteryPercent);
							
						u32Temp /= (s_stBatteryVoltagePercent_D[s8J].u32BatteryPercent - s_stBatteryVoltagePercent_D[s8I].u32BatteryPercent);
						u32Temp += s_stBatteryVoltagePercent_D[s8J].u32CurrentIncharge;							
							
						if(s_u32BatteryInchargeTimer >= BATTERY_INCHARGE_SAMPLE_DELAY)
						{	
							u32Temp *= (BATTERY_INCHARGE_SAMPLE_DELAY/30000);
							s_u32BatteryCurrentTotalBackup += u32Temp;
						}							
						u32Temp = s_u32BatteryCurrentTotalBackup*1000;
						u32Temp /= BATTERY_CURRENT_MAX_D;//ǧ��֮��
						if(u32Temp >= BATTERY_FULL_JUDGE_DATA)
						{
							s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
							s_u32BatteryCurrentTotalBackup = BATTERY_CURRENT_MAX_D;									
						}
						else
						{
							s_u16BatteryCouloData = (u16)(u32Temp/10);													
						}
						break;
					}
					else if(s8J == BATTERY_VOLTAGE_PERCENT_BUFSIZE)//u8IΪ�����е����һ��
					{
						if(u32Temp >= s_stBatteryVoltagePercent_D[BATTERY_VOLTAGE_PERCENT_BUFSIZE-0x01].u32BatteryPercent)	
						{
							s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
							s_u32BatteryCurrentTotalBackup = BATTERY_CURRENT_MAX_D;
							s_u32BatteryInchargeTimer = 0xFFFFFFFF;
							break;
						}
					}
					else
						;
				}
			}
		}
		else
		{
				s_u16BatteryCouloData = 0;
				break;
		}
			
	}
	/**�Գ���쳣������д���**/
	if(g_u32ID == B_type)
	{
		if(s8I < 0)
		{
			if((s_stBatteryStatus == BATTERY_WORK_NORMAL) || (s_stBatteryStatus == BATTERY_TOO_LOW))
			{
				if(u32Sum >= s_stBatteryVoltagePercent[BATTERY_VOLTAGE_PERCENT_BUFSIZE-0x01].u32VoltageWork)
				{
					s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
				}
				else if(u32Sum < s_stBatteryVoltagePercent[0].u32VoltageWork)
				{
					s_u16BatteryCouloData = BATTERY_VOLTAGE_EMPTY_PERCENT;
				}
				else
					;
			}
			else if((s_stBatteryStatus >= BATTERY_START_CHARGED) && (s_stBatteryStatus <= BATTERY_CHARGE_OVER))	
			{
				if(u32Sum >= s_stBatteryVoltagePercent[BATTERY_VOLTAGE_PERCENT_BUFSIZE-0x01].u32VoltageInCharge)
				{
					s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
					s_u32BatteryCurrentTotalBackup = BATTERY_CURRENT_MAX;
				}
				else if(u32Sum < s_stBatteryVoltagePercent[0].u32VoltageInCharge)
				{
					s_u16BatteryCouloData = BATTERY_VOLTAGE_EMPTY_PERCENT;
				}
				else
					;					
			}
			else
				;
		}
	}
	else  //D��
	{
		if(s8I < 0)
		{
			if((s_stBatteryStatus == BATTERY_WORK_NORMAL) || (s_stBatteryStatus == BATTERY_TOO_LOW))
			{
				if(u32Sum >= s_stBatteryVoltagePercent_D[BATTERY_VOLTAGE_PERCENT_BUFSIZE-0x01].u32VoltageWork)
				{
					s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
				}
				else if(u32Sum < s_stBatteryVoltagePercent_D[0].u32VoltageWork)
				{
					s_u16BatteryCouloData = BATTERY_VOLTAGE_EMPTY_PERCENT;
				}
				else
					;
			}
			else if((s_stBatteryStatus >= BATTERY_START_CHARGED) && (s_stBatteryStatus <= BATTERY_CHARGE_OVER))	
			{
				if(u32Sum >= s_stBatteryVoltagePercent_D[BATTERY_VOLTAGE_PERCENT_BUFSIZE-0x01].u32VoltageInCharge)
				{
					s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
					s_u32BatteryCurrentTotalBackup = BATTERY_CURRENT_MAX_D;
				}
				else if(u32Sum < s_stBatteryVoltagePercent_D[0].u32VoltageInCharge)
				{
					s_u16BatteryCouloData = BATTERY_VOLTAGE_EMPTY_PERCENT;
				}
				else
					;					
			}
			else
				;
		}	
	}
}


/***************************************************************************************
** ��������: BatteryVoltageInintProc
** �����������ɼ���������CHRG��ֵ��״̬,����ʽ����״̬
** �������룺��
** ���������None
** ע�����
******************************************************************************************/
void BatteryVoltageMidProc(u32 u32Sum,u32 u32Temp)
{
	
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
		OS_CPU_SR  cpu_sr = 0;
	#endif	
	
	if((u32Sum > AD_SAMPLE_MIN) && (u32Sum <=  AD_SAMPLE_MAX))
	{
		u32Sum *= s_u32AdVerfintDefault;
		u32Sum /= u32Temp;
		if((u32Sum%10) >= 0x05)
		{
			u32Sum /= 10;//��λ0.1mV
			u32Sum += 1;//��λ0.1mV
		}
		else
		{
			u32Sum /= 10;//��λ0.1mV
		}
		
		if(g_u32ID != B_type)
			u32Sum = u32Sum/2*3;
		
		if(g_u32ID == B_type)
			u32Sum -= BATTERY_AD_WRAP;//��������ʵ��ƫ��20mV��
		

			
		

		/*�ϵ��ʼ����ͬʱ�Ѿ��ڳ��,�����¼����ʱ��Ӧ�Ĺ�����ѹֵ*/
		if(s_u32BatteryVoltageBackup == 0xFFFF||s_u32BatteryVoltageBackup_D == 0xFFFF)
		{
//			if(g_u32ID == B_type)
			{
				if(BatteryVoltageInintProc(u32Sum) == 0x01)
				{	
					s_u32BatteryTaskTimer = 0xFFFFFFFF;
					return ;
				}
			}
		}
		
		OS_ENTER_CRITICAL();		
		BatteryVoltageIntegralProc(u32Sum);		
		OS_EXIT_CRITICAL();
	
	}
	else
	{
		/*����״̬*/
		s_stBatteryCouloStatus = BATTERY_COULOMETRY_WRONG_PERCENT;
		s_u16BatteryCouloData = 0;
	}
}
/***************************************************************************************
** ��������: BatteryVoltageInintProc
** �����������ɼ���������CHRG��ֵ��״̬,����ʽ����״̬
** �������룺��
** ���������None
** ע�����
******************************************************************************************/
void BatteryVoltageProc(u32 u32Timer)
{
	
	u32 u32Sum = 0x00;
	u32 u32Temp = 0x00;
	s8 s8I = 0x00;

	#if 0//2017.03.29��ʱ����
		if(s_u32BatteryTaskTimer != 0xFFFFFFFF)
		{
			s_u32BatteryTaskTimer += u32Timer;
			if(s_u32BatteryTaskTimer >= BATTERY_LINEOFF_TIME_DELAY)
			{
				s_u32BatteryTaskTimer = 0xFFFFFFFF;
				s_u32BatteryColumDataInint = 0xFFFFFFFF;
			}
		}
	#endif
	GetAdcAverageValue(BATTERY_VOLTAGE_AD_CHANNEL,s_u16BatteryVoltageBuf+s_u16BatteryVoltBufpointer);
	GetAdcAverageValue(CPU_VREFINT_AD_CHANNEL,s_u16CpuVrefIntBuf+s_u16BatteryVoltBufpointer);
	//����D��ڶ����ص�adc
	if(g_u32ID != B_type)
		GetAdcAverageValue(BATTERY_VOLTAGE_AD_CHANNEL_1,s_u16BatteryVoltageBuf_D+s_u16BatteryVoltBufpointer);
	
	s_u16BatteryVoltBufpointer++;
	if(s_u16BatteryVoltBufpointer == BATTERY_SAMPLE_BUF_SIZE)//����ƽ��ֵ
	{
		s_u16BatteryVoltBufpointer %= BATTERY_SAMPLE_BUF_SIZE;
		//��ʼ�����֮����еĲ���
		if((s_u16BatteryCouloData != 0xFFFF)&&(s_u32BatteryInchargeTimer != 0xFFFFFFFF))
		{
			s_u32BatteryInchargeTimer += u32Timer*BATTERY_SAMPLE_BUF_SIZE;
			if((s_stBatteryStatus >= BATTERY_START_CHARGED) && (s_stBatteryStatus <= BATTERY_CHARGE_OVER)\
				&&(s_u32BatteryInchargeTimer%BATTERY_INCHARGE_SAMPLE_DELAY))
				return;
		}
		
		u32Temp = FilterAndAverage(s_u16CpuVrefIntBuf,BATTERY_SAMPLE_BUF_SIZE);
		//����D��ڶ����صĻ�ȡ����
		if(g_u32ID != B_type)
		{
			u32Sum = FilterAndAverage(s_u16BatteryVoltageBuf_D,BATTERY_SAMPLE_BUF_SIZE);
			BatteryVoltageMidProc(u32Sum,u32Temp);
						
			s_stBatteryStatus_D = s_stBatteryStatus;
			s_stBatteryCouloStatus_D = s_stBatteryCouloStatus;
			s_u16BatteryCouloData_D = s_u16BatteryCouloData;
			s_stBatteryCouloStatusBackup_D = s_stBatteryCouloStatusBackup;
			s_u32BatteryCurrentTotalBackup_D = s_u32BatteryCurrentTotalBackup;
			s_u32BatteryVoltageBackup_D = s_u32BatteryVoltageBackup;//��ѹֵ����
			
			
			if(s_stBatteryStatus == BATTERY_ININT)
			{
				s_stBatteryCouloStatus = BATTERY_COULOMETRY_100_PERCENT;
				s_u16BatteryCouloData = 0xFFFF;
				s_u32BatteryVoltageBackup = 0xFFFF;
			}
		}
						
		u32Sum = FilterAndAverage(s_u16BatteryVoltageBuf,BATTERY_SAMPLE_BUF_SIZE);
		BatteryVoltageMidProc(u32Sum,u32Temp);
		
		s_u32BatteryVoltageBackup_B = s_u32BatteryVoltageBackup;
		
		//�ж������ص����ݣ������������ʾ
		if(g_u32ID != B_type)
		{
			if(s_u16BatteryCouloData_D>s_u16BatteryCouloData)
			{
				s_stBatteryStatus = s_stBatteryStatus_D;
				s_stBatteryCouloStatus = s_stBatteryCouloStatus_D;
				s_u16BatteryCouloData = s_u16BatteryCouloData_D;
				s_stBatteryCouloStatusBackup =  s_stBatteryCouloStatusBackup_D;
				s_u32BatteryCurrentTotalBackup = s_u32BatteryCurrentTotalBackup_D;
				s_u32BatteryVoltageBackup = s_u32BatteryVoltageBackup_D;			
			}
		}
		
		if(g_u32ID == B_type)
		{		
			if((s_stBatteryStatus == BATTERY_START_CHARGED)||(s_stBatteryStatus == BATTERY_BEING_CHARGED)||(s_stBatteryStatus == BATTERY_CHARGE_GONNA_OVER))
			{
				if((s_u16BatteryCouloData >= BATTERY_VOLTAGE_FULL_PERCENT)||(s_u32BatteryInchargeTimer >= BATTERY_INCHARGE_TIME_MAX))
				{
					s_stBatteryStatus = BATTERY_CHARGE_OVER;	
					s_u32BatteryCurrentTotalBackup = BATTERY_CURRENT_MAX;
				}				
				else if(s_u16BatteryCouloData >= BATTERY_VOLTAGE_90_PERCENT)
				{
					s_stBatteryStatus = BATTERY_CHARGE_GONNA_OVER;	
				}
				else
					;					
			}
			
			/**�ж��Ƿ��Ѿ�����**/						
			if((s_stBatteryStatus == BATTERY_CHARGE_GONNA_OVER))
			{
				if(u32Sum >= s_stBatteryVoltagePercent[BATTERY_VOLTAGE_PERCENT_BUFSIZE-0x01].u32VoltageInCharge)
				{
					s_stBatteryStatus = BATTERY_CHARGE_OVER;
					s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
					s_u32BatteryCurrentTotalBackup = BATTERY_CURRENT_MAX;
					s_u32BatteryVoltageBackup = u32Sum;	
				}
			}
		}
		else
		{
			if((s_stBatteryStatus == BATTERY_START_CHARGED)||(s_stBatteryStatus == BATTERY_BEING_CHARGED)||(s_stBatteryStatus == BATTERY_CHARGE_GONNA_OVER))
			{
				if((s_u16BatteryCouloData >= BATTERY_VOLTAGE_FULL_PERCENT)||(s_u32BatteryInchargeTimer >= BATTERY_INCHARGE_TIME_MAX))
				{
					s_stBatteryStatus = BATTERY_CHARGE_OVER;	
					s_u32BatteryCurrentTotalBackup = BATTERY_CURRENT_MAX_D;
				}				
				else if(s_u16BatteryCouloData >= BATTERY_VOLTAGE_90_PERCENT)
				{
					s_stBatteryStatus = BATTERY_CHARGE_GONNA_OVER;	
				}
				else
					;					
			}
			
			
			/**�ж��Ƿ��Ѿ�����**/						
			if((s_stBatteryStatus == BATTERY_CHARGE_GONNA_OVER))
			{
				if(u32Sum >= s_stBatteryVoltagePercent_D[BATTERY_VOLTAGE_PERCENT_BUFSIZE-0x01].u32VoltageInCharge)
				{
					s_stBatteryStatus = BATTERY_CHARGE_OVER;
					s_u16BatteryCouloData = BATTERY_VOLTAGE_FULL_PERCENT;
					s_u32BatteryCurrentTotalBackup = BATTERY_CURRENT_MAX_D;
					s_u32BatteryVoltageBackup = u32Sum;	
				}
			}
		}		
		
		if((s_u16BatteryCouloData <= BATTERY_VOLTAGE_8_PERCENT)&&(s_stBatteryStatus == BATTERY_WORK_NORMAL))
			s_stBatteryStatus = BATTERY_TOO_LOW;	
		
		/**�ж������ٷֱȣ���Ҫ������ʾ**/
		for(s8I = 0x00; s8I < BATTERY_COULOMETRY_TYPE_MAX; s8I++)
		{
			if((s_u16BatteryCouloData >= s_stBatteryCalculateList[s8I].u16ValueMin) && (s_u16BatteryCouloData < s_stBatteryCalculateList[s8I].u16ValueMax))
			{
				s_stBatteryCouloStatus = s_stBatteryCalculateList[s8I].sBatteryColumnStatus;
				break;
			}
		}
		
		/**�ж��ڹ���ʱ�Ƿ�ȳ�ʼֵ����***/
		if((s_stBatteryStatus == BATTERY_WORK_NORMAL)||(s_stBatteryStatus == BATTERY_TOO_LOW))
		{
			if((s_u16BatteryCouloData > s_u32BatteryColumDataInint)&&(s_u32BatteryColumDataInint != 0xFFFFFFFF))	//��������ģʽ�£������ܱȳ�ʼֵ����
			{
				s_u16BatteryCouloData  = s_u32BatteryColumDataInint;
				s_u32BatteryCurrentTotalBackup = s_u32BatteryCurrentTotalInint;
			}

			if((s_stBatteryCouloStatus != s_stBatteryCouloStatusBackup)&&(s_u16BatteryCouloDataBackup != 0xFFFF)
				&&(s_stBatteryCouloStatus != BATTERY_COULOMETRY_EMPTY))
			{
				s_u32BatteryChangeTimer += BATTERY_SAMPLE_TIME_SET;
				if(s_u32BatteryChangeTimer >= BATTERY_COLUMDATA_CHANGE_DELAY)
				{
					;	
				}	
				else
				{
					s_u16BatteryCouloData = s_u16BatteryCouloDataBackup;
					s_u32BatteryCurrentTotalBackup = s_u32BatteryCurrentTotalBackupBkp;						
					s_stBatteryCouloStatus = s_stBatteryCouloStatusBackup;
				}
			}
			else
			{
				s_u32BatteryChangeTimer = 0x00;
			}
		}
		s_u16BatteryCouloDataBackup = s_u16BatteryCouloData;
		s_u32BatteryCurrentTotalBackupBkp = s_u32BatteryCurrentTotalBackup;
		s_u32BatteryVoltageBackupBkp = s_u32BatteryVoltageBackup;
		
		/**�ж������ٷֱȣ���Ҫ������ʾ**/
		for(s8I = 0x00; s8I < BATTERY_COULOMETRY_TYPE_MAX; s8I++)
		{
			if((s_u16BatteryCouloData >= s_stBatteryCalculateList[s8I].u16ValueMin) && (s_u16BatteryCouloData < s_stBatteryCalculateList[s8I].u16ValueMax))
			{
				s_stBatteryCouloStatus = s_stBatteryCalculateList[s8I].sBatteryColumnStatus;
				break;
			}
		}
		if((s_u16BatteryCouloData <= BATTERY_VOLTAGE_8_PERCENT)&&(s_stBatteryStatus == BATTERY_WORK_NORMAL))
			s_stBatteryStatus = BATTERY_TOO_LOW;
		
		if(s8I == BATTERY_COULOMETRY_TYPE_MAX)//����״̬
		{
			s_stBatteryCouloStatus = BATTERY_COULOMETRY_WRONG_PERCENT;
			s_u16BatteryCouloData = 0;
		}

		s_stBatteryCouloStatusBackup = s_stBatteryCouloStatus;			
			
	}
	s_u16BatteryVoltBufpointer %= BATTERY_SAMPLE_BUF_SIZE;
}
#endif
/***************************************************************************************
** ��������: BatteryInit
** ���������������ز�����ʼ��
** �������룺��
** ���������None
** ע�������
******************************************************************************************/
u32 BatteryInit(void)
{
	u8 u8I;
	
	s_stBatteryStatus = BATTERY_ININT;
	s_stBatteryCouloStatus = BATTERY_COULOMETRY_100_PERCENT;
	s_u16BatteryCouloData = 0xFFFF;
	s_u16BatteryCouloData_D = 0xFFFF;
	s_u32BatteryInchargeTimer = 0xFFFFFFFF;
  s_u32BatteryChangeTimer = 0x00;
	s_u32BatteryColumDataInint = 0xFFFFFFFF;
	
	s_stBatteryStatus_D = BATTERY_ININT;//���״̬
  s_stBatteryCouloStatus_D = BATTERY_COULOMETRY_100_PERCENT;
		
	#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//��Ӳ���У�ʹ��DS2756��ʽ�������
		s_u32BatteryCalTimes = 0x00;	
		s_u16BatteryCurtBufpointer = 0x00;
		s_u16BatteryVoltBufpointer = 0x00;
		s_u16BatteryAccCurtBufpointer = 0x00;
		s_u16BatteryTempBufpointer = 0x00;
		s32BatteryCurrentValue = 0x00;
		s32BatteryVoltValue = 0x00;
		s32BatteryAccCurtValue = 0x00;
		s32BatteryTempValue = 0x00;
		s_u16BatteryCouloDataBackup = 0xFFFF;
		s_stBatteryCouloStatusBackup = s_stBatteryCouloStatus;
		for(u8I = 0x00; u8I < BATTERY_SAMPLE_BUF_SIZE;u8I ++)
		{
			s32BatteryCurrentBuf[u8I] = 0x00;
			s32BatteryVoltBuf[u8I] = 0x00;
			s32BatteryAccCurrentBuf[u8I] = 0x00;
			s32BatteryTempBuf[u8I] = 0x00;			
		}		
	#else
		s_u16BatteryVoltBufpointer = 0x00;		
		s_u32BatteryVoltageBackup = 0xFFFF;
		s_u32BatteryVoltageBackup_D = 0xFFFF;
		s_u32BatteryCurrentTotalBackup = 0xFFFFFFFF;
		s_u16BatteryCouloDataBackup = 0xFFFF;
	    s_u32BatteryCurrentTotalInint = 0x00;
		s_u32BatteryCurrentTotalBackupBkp = 0x00;//����ֵ����	
		s_u32BatteryVoltageBackupBkp = 0x00;//��ѹֵ����			
		s_stBatteryCouloStatusBackup = s_stBatteryCouloStatus;	
		s_u32BatteryTaskTimer = 0x00;
		
		/**��ȡVREFINT��ѹAD�洢ֵ****/
		s_u32AdVerfintDefault = 0x00;	
		s_u32AdVerfintDefault = *(__IO uint32_t *)(0x1FF800f8);
		s_u32AdVerfintDefault &= 0xFFFF;	
		s_u32AdVerfintDefault *= 3000;
		s_u32AdVerfintDefault *= 100;//��λ0.01mV
		s_u32AdVerfintDefault /= 4096;
		
		for(u8I = 0x00; u8I < BATTERY_SAMPLE_BUF_SIZE-0x01;u8I ++)
		{
			GetAdcAverageValue(BATTERY_VOLTAGE_AD_CHANNEL,s_u16BatteryVoltageBuf+s_u16BatteryVoltBufpointer);
			GetAdcAverageValue(CPU_VREFINT_AD_CHANNEL,	s_u16CpuVrefIntBuf+s_u16BatteryVoltBufpointer);
			if(g_u32ID != B_type)
				GetAdcAverageValue(BATTERY_VOLTAGE_AD_CHANNEL_1,	s_u16BatteryVoltageBuf_D+s_u16BatteryVoltBufpointer);
			s_u16BatteryVoltBufpointer++;
		}		
		BatteryVoltageProc(BATTERYMNG_TASK_TIME_SET);			
	#endif

	return 0x01;
}
	
/***************************************************************************************
** ��������: BatteryMngTask
** ������������ع���������
** �������룺��
** ���������None
** ע�������
******************************************************************************************/
void BatteryMngTask(void *p_arg)
{
	#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//��Ӳ���У�ʹ��DS2756��ʽ�������
	#else
		INT8U err;
	#endif
	
	#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//��Ӳ���У�ʹ��DS2756��ʽ�������
		BatteryInit();
	#else	
		OSTimeDly(BATTERY_TASK_POWERUP_DELAY/TICK_TIME);//���ϵ�ʱ����ʱ���д���
		BatteryInit();	
		#ifndef HARDWARE_TEST	
			OSSemPend(g_stLogicSem, 0/TICK_TIME, &err);
		#endif
	#endif
	
	while(1)
	{
		#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//��Ӳ���У�ʹ��DS2756��ʽ�������
			if((s_u32BatteryCalTimes <= BATTERY_SAMPLE_INVALID_TIMESSET)||(s_u16BatteryCouloData == 0xFFFF))
				OSTimeDly(BATTERY_TASK_START_TIME_SET/TICK_TIME);
			else
				OSTimeDly(BATTERYMNG_TASK_TIME_SET/TICK_TIME);
		#else
			OSTimeDly(BATTERYMNG_TASK_TIME_SET/TICK_TIME);
		#endif
		/*�ж��Ƿ���״̬**/
		BatteryChongdianPinProc();		
		/**�ɼ����������ֵ��״̬**/
		BatteryVoltageProc(BATTERYMNG_TASK_TIME_SET);	
	}
}

