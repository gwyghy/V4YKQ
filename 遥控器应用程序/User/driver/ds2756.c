/********************************************************************************
* �ļ�����	ds2756.c
* ���ߣ�	������   
* �汾��   	V1.0
* ���ڣ�    2015.10.23
* ��������:  ʵ��DS2756������

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

#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//��Ӳ���У�ʹ��DS2756��ʽ�������
/********************************************************************************
* #define�궨��
*********************************************************************************/
#define DqOutputHigh()		GPIO_SetBits(DS2756_DATA_PORT, DS2756_DATA_PIN)//����ߵ�ƽ
#define DqOutputLow()		GPIO_ResetBits(DS2756_DATA_PORT, DS2756_DATA_PIN)//����͵�ƽ	

#define DQ_DATA_KEEP_TIME		20//��λus

#define	DQ_DATA_RESET_TIME_1 	800//��λus
#define	DQ_DATA_RESET_TIME_2 	250//��λus
#define	DQ_DATA_RESET_TIME_3 	400//��λus

#define	VOLTAGE_SAMPLE_UNITS				4880//��λuv
#define	CURRENT_SAMPLE_UNITS				15625 //��λuv
#define	AVERRAGE_CURRENT_SAMPLE_UNITS		1953 //��λuv
#define	ACCUMULATED_CURRENT_SAMPLE_UNITS	6250 //��λuvh
#define	TEMPERATURE_SAMPLE_UNITS			125 //��λǧ��֮һ��
/********************************************************************************
* ��������
*********************************************************************************/
/**�Ĵ�������**/
enum
{
	DS_REG_SPEC_FEA	=	0x08,
	DS_REG_VOLT_MSB =	0x0C,
	DS_REG_VOLT_LSB	=	0x0D,
	DS_REG_CURT_MSB	=	0x0E,
	DS_REG_CURT_LSB	=	0x0F,
	DS_REG_ACR_MSB	=	0x10,
	DS_REG_ACR_LSB	=	0x11,
	DS_REG_TEMP_MSB	=	0x18,
	DS_REG_TEMP_LSB	=	0x19,
	DS_REG_AV_CURT_MSB	=	0x1A,
	DS_REG_AV_CURT_LSB	=	0x1B,

	DS_REG_ACR_BIAS	=	0x33,//���������ƫ��
};

/**�����**/
enum
{
	DS_CMD_READ_DATA = 0x69,
	DS_CMD_WRITE_DATA = 0x6C,
	DS_CMD_COPY_DATA = 0x48,//��RAM������E2PROM����
	DS_CMD_RECALL_DATA = 0xB8,//��E2PROM��ȡ����д��RAM
	DS_CMD_LOCK = 0x64,
	DS_CMD_SYNC = 0xD2,
};

/**��ȡ�����ַ���**/
enum
{
	DS_NETADDRESS_READ_METHOD1 = 0x33,
	DS_NETADDRESS_READ_METHOD2 = 0x39,
	DS_NETADDRESS_MATCH = 0x55,	
	DS_NETADDRESS_SKIP = 0xCC,
	DS_NETADDRESS_SEARCH = 0xF0	
};
/********************************************************************************
* ��������
*********************************************************************************/

/********************************************************************************
* ��������
*********************************************************************************/

/***************************************************************************************
** ��������:Ds2756PortInit
** ����������Ds2756�˿ڳ�ʼ��
** �������룺��
** ���������1��������0���쳣
** ע�����
******************************************************************************************/
u32 Ds2756PortInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	#if 0
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	#endif
	
	/* GPIO Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_DS2756_DATA_GPIO, ENABLE);
	
	/* Configure DS2756_DATA_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
	GPIO_InitStructure.GPIO_Pin = DS2756_DATA_PIN;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(DS2756_DATA_PORT, &GPIO_InitStructure);		

	/* Configure DS2756_INT_PIN pin */
	GPIO_InitStructure.GPIO_Pin = DS2756_INT_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(DS2756_INT_PORT, &GPIO_InitStructure);
		
	#if 0//��ʱ�������жϷ�ʽ		
		/**INT�ⲿ�ж���ص�����**///INT�ж��������ж�Ϊͬһ���жϴ�����
		SYSCFG_EXTILineConfig(DS2756_INT_PORT_SOURCE,DS2756_INT_PIN_SOURCE);

		/* DS2756_INT_EXTI_LINE */
		EXTI_InitStructure.EXTI_Line = DS2756_INT_EXTI_LINE;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);


		/* EXTI9_5_IRQn */
		NVIC_InitStructure.NVIC_IRQChannel = DS2756_INT_EXTI_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;//���ȼ����
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure); 
	#endif
	
	return 0x00;
}

/***************************************************************************************
** ��������:Ds2756DelayUs
** ����������Ds2756��ʱ΢�뺯��
** �������룺��
** ���������1��������0���쳣
** ע�����
******************************************************************************************/
void Ds2756DelayUs(u32 u32Data)
{
    u32 u32I;
    for(u32I=u32Data*2;u32I>0;u32I--)
			;
}

/***************************************************************************************
** ��������:Ds2756WriteByte
** ����������Ds2756дһ���ֽڵ�����
** �������룺��
** �����������
** ע�����
******************************************************************************************/
void Ds2756WriteByte(u8 u8Data)
{
 	u8 u8I = 0x00;
	
	for(u8I=0;u8I<8;u8I++)
	{	
		DqOutputLow();
		Ds2756DelayUs(10);
		if(u8Data&0x01)
		{	
			DqOutputHigh();
		}
		else
		{	
			DqOutputLow();
		}
		Ds2756DelayUs(100);
		DqOutputHigh();
		Ds2756DelayUs(10);
		u8Data>>=1;
	}
}

/***************************************************************************************
** ��������:Ds2756WriteByte
** ����������Ds2756дһ���ֽڵ�����
** �������룺��
** �����������
** ע�����
******************************************************************************************/
u8 Ds2756ReadByte(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	u8 u8I = 0x00,u8J;
	u8 u8Data = 0x00;
	
 	for (u8I=8;u8I>0;u8I--)
 	{	
		/* Configure DS2756_DATA_PIN in output pushpull mode */
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
		GPIO_InitStructure.GPIO_Pin = DS2756_DATA_PIN;	
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(DS2756_DATA_PORT, &GPIO_InitStructure);
		
  		DqOutputLow();	
		Ds2756DelayUs(10);	
  		u8Data>>=1;
  		DqOutputHigh();	
		Ds2756DelayUs(10);
		
		/* Configure DS2756_INT_PIN pin */
		GPIO_InitStructure.GPIO_Pin = DS2756_DATA_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
		GPIO_Init(DS2756_DATA_PORT, &GPIO_InitStructure);
		
		u8J = GPIO_ReadInputDataBit(DS2756_DATA_PORT, DS2756_DATA_PIN);
  		if(u8J == Bit_SET)
			u8Data|=0x80;
  		Ds2756DelayUs(80);

  		DqOutputHigh();	
		Ds2756DelayUs(5);		
 	}

	/* Configure DS2756_DATA_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
	GPIO_InitStructure.GPIO_Pin = DS2756_DATA_PIN;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(DS2756_DATA_PORT, &GPIO_InitStructure);
	
 	return(u8Data);
}

/***************************************************************************************
** ��������:Ds2756Reset
** ����������Ds2756��λ
** �������룺��
** �����������
** ע�����
******************************************************************************************/
u32 Ds2756Reset(void)
{
	u32 u32SetFlag = 0x00;

	DqOutputLow();
	Ds2756DelayUs(DQ_DATA_RESET_TIME_1);
	DqOutputHigh();	
	Ds2756DelayUs(DQ_DATA_KEEP_TIME*2);

	DqOutputLow();	
	Ds2756DelayUs(DQ_DATA_RESET_TIME_2);
	
	DqOutputHigh();	
	Ds2756DelayUs(DQ_DATA_RESET_TIME_3);
	
	return u32SetFlag;
}
/***************************************************************************************
** ��������:Ds2756ReadVoltage
** ����������Ds2756��ȡ��ѹֵ
** �������룺��
** �����������
** ע�����
******************************************************************************************/
u16 Ds2756ReadVoltage(s32 *pValue)
{
	u32 u32tvalue = 0x00;
	u32 u32Temp = 0x00;
	
	Ds2756Reset(); 
	Ds2756WriteByte(DS_NETADDRESS_SKIP);//*���������к�*/ 
	Ds2756WriteByte(DS_CMD_READ_DATA);//read data 2756 
 
	Ds2756WriteByte(DS_REG_VOLT_MSB);//V
	u32Temp = (u32)(Ds2756ReadByte());
	u32tvalue = (u32)(Ds2756ReadByte());
	u32Temp <<= 8;
	u32tvalue |= u32Temp; 
	u32tvalue >>= 5;
	
	if(u32tvalue > 0x3FF)
	{
		u32tvalue -= 0x01;
		u32tvalue = ~u32tvalue;
		u32tvalue &= 0x03FF;
		*pValue = u32tvalue*(VOLTAGE_SAMPLE_UNITS)/1000;//��λmv
		*pValue *= -1;
	}
	else
	{
		*pValue = u32tvalue*(VOLTAGE_SAMPLE_UNITS)/1000;//��λmv
	}
	return 0x01;
}

/***************************************************************************************
** ��������:Ds2756ReadCurrent
** ����������Ds2756��ȡ����ֵ
** �������룺��
** �����������
** ע�����
******************************************************************************************/
u16 Ds2756ReadCurrent(s32 *pValue)
{
	u32 u32tvalue;
	u32 u32Temp;
	
	Ds2756Reset(); 
	Ds2756WriteByte(DS_NETADDRESS_SKIP);//*���������к�*/ 
	Ds2756WriteByte(DS_CMD_READ_DATA);//read data 2756 
 
	Ds2756WriteByte(DS_REG_CURT_MSB);//����ֵ
	u32Temp = (u32)(Ds2756ReadByte());
	u32tvalue = (u32)(Ds2756ReadByte());
	u32Temp <<= 8;
	u32tvalue |= u32Temp; 
	u32tvalue >>= 3;
	
	if(u32tvalue > 0x0FFF)
	{
		u32tvalue -= 0x01;
		u32tvalue = ~u32tvalue;
		u32tvalue &= 0x0FFF;
		*pValue = u32tvalue*CURRENT_SAMPLE_UNITS/DS2756_RSNS_VALUE/1000;//��λmA
		*pValue *= -1;//��λmA
	}
	else
	{	
		*pValue = u32tvalue*CURRENT_SAMPLE_UNITS/DS2756_RSNS_VALUE/1000;//��λmA
	}	
	
	
	return 0x01;
}
/***************************************************************************************
** ��������:Ds2756ReadAverageCurrent
** ����������Ds2756��ȡ�ۼƵ���ֵ
** �������룺��
** �����������
** ע�����
******************************************************************************************/
u16 Ds2756ReadAverageCurrent(s32 *pValue)
{
	u32 u32tvalue;
	u32 u32Temp;
	
	Ds2756Reset(); 
	Ds2756WriteByte(DS_NETADDRESS_SKIP);//*���������к�*/ 
	Ds2756WriteByte(DS_CMD_READ_DATA);//read data 2756 
 
	Ds2756WriteByte(DS_REG_AV_CURT_MSB);//ƽ������ֵ
	u32Temp = (u32)(Ds2756ReadByte());
	u32tvalue = (u32)(Ds2756ReadByte());
	u32Temp <<= 8;
	u32tvalue |= u32Temp; 
	
	if(u32tvalue > 0x7FFF)
	{
		u32tvalue -= 0x01;
		u32tvalue = ~u32tvalue;
		u32tvalue &= 0x7FFF;
		*pValue = u32tvalue*AVERRAGE_CURRENT_SAMPLE_UNITS/DS2756_RSNS_VALUE/1000;//��λmA
		*pValue *= -1;
	}
	else
	{
		*pValue = u32tvalue*AVERRAGE_CURRENT_SAMPLE_UNITS/DS2756_RSNS_VALUE/1000;//��λmA
	}
	return 0x01;
}

/***************************************************************************************
** ��������:Ds2756ReadAccumulatedCurrent
** ����������Ds2756��ȡ�ۼƵ���ֵ
** �������룺��
** �����������
** ע�����
******************************************************************************************/
u16 Ds2756ReadAccumulatedCurrent(s32 *pValue)
{
	u32 u32tvalue;
	u32 u32Temp;
	
	Ds2756Reset(); 
	Ds2756WriteByte(DS_NETADDRESS_SKIP);//*���������к�*/ 
	Ds2756WriteByte(DS_CMD_READ_DATA);//read data 2756 
 
	Ds2756WriteByte(DS_REG_ACR_MSB);//�ۼƵ���ֵ
	u32Temp = (u32)(Ds2756ReadByte());
	u32tvalue = (u32)(Ds2756ReadByte());
	u32Temp <<= 8;
	u32tvalue |= u32Temp; 
	
	if(u32tvalue > 0x7FFF)
	{
		u32tvalue -= 0x01;
		u32tvalue = ~u32tvalue;
		u32tvalue &= 0x7FFF;
		*pValue = u32tvalue*ACCUMULATED_CURRENT_SAMPLE_UNITS/DS2756_RSNS_VALUE/1000;//��λmAH	
		*pValue *= -1;
	}
	else
	{		
		*pValue = u32tvalue*ACCUMULATED_CURRENT_SAMPLE_UNITS/DS2756_RSNS_VALUE/1000;//��λmAH		
	}
	return 0x01;
}

/***************************************************************************************
** ��������:Ds2756ReadAccumulatedCurrent
** ����������Ds2756��ȡ��ѹֵ
** �������룺��
** �����������
** ע�����
******************************************************************************************/
u16 Ds2756ReadTemperature(s32 *pValue)
{
	u32 u32tvalue;
	u32 u32Temp;
	
	Ds2756Reset(); 
	Ds2756WriteByte(DS_NETADDRESS_SKIP);//*���������к�*/ 
	Ds2756WriteByte(DS_CMD_READ_DATA);//read data 2756 
 
	Ds2756WriteByte(DS_REG_TEMP_MSB);//�¶�ֵ
	u32Temp = (u32)(Ds2756ReadByte());
	u32tvalue = (u32)(Ds2756ReadByte());
	u32Temp <<= 8;
	u32tvalue |= u32Temp; 
	u32tvalue >>= 5;
	
	if(u32tvalue > 0x03FF)
	{
		u32tvalue -= 0x01;
		u32tvalue = ~u32tvalue;
		u32tvalue &= 0x03FF;
		*pValue = u32tvalue*TEMPERATURE_SAMPLE_UNITS/1000;//��λ1��
		*pValue *= -1;
	}
	else
	{
		*pValue = u32tvalue*TEMPERATURE_SAMPLE_UNITS/1000;//��λ1��		
	}
	return 0x01;
}

/***************************************************************************************
** ��������:Ds2756ReadAccumulatedCurrent
** ����������Ds2756��ȡ��ѹֵ
** �������룺��
** �����������
** ע�����
******************************************************************************************/
u16 Ds2756SetAccumulationCurrentBias(s32 *pValue)
{
	u8 u8Temp;
	
	Ds2756Reset(); 
	if(*pValue < 0 )
	{
		u8Temp = ~((*pValue)-0x01);
		u8Temp |= 0x80;
	}
	Ds2756WriteByte(DS_NETADDRESS_SKIP);//*���������к�*/ 
	Ds2756WriteByte(DS_CMD_WRITE_DATA);//read data 2756 
 
	Ds2756WriteByte(DS_REG_ACR_BIAS);//�������ƫ��
	Ds2756WriteByte(u8Temp);//�������ƫ��

	return 0x01;
}

/***************************************************************************************
** ��������:Ds2756SetAccumulationCurrent
** ����������Ds2756�����ۼƵ���ֵ
** �������룺��
** �����������
** ע�����
******************************************************************************************/
u16 Ds2756SetAccumulationCurrent(s32 *pValue)
{
	u32 u32Temp;
	
	Ds2756Reset(); 
	
	u32Temp = fabs(*pValue)*1000*DS2756_RSNS_VALUE;
	u32Temp /= ACCUMULATED_CURRENT_SAMPLE_UNITS;
	
	if(*pValue < 0)
	{
		u32Temp = ~(u32Temp)+0x01;
		u32Temp &= 0x7FFF;
		u32Temp |= 0x8000;
	}
	Ds2756WriteByte(DS_NETADDRESS_SKIP);//*���������к�*/ 
	Ds2756WriteByte(DS_CMD_WRITE_DATA);//
 
	Ds2756WriteByte(DS_REG_ACR_MSB);//�������ƫ��
	Ds2756WriteByte((u8)((u32Temp&0xFF00)>>8));//�������ƫ��
	Ds2756WriteByte((u8)((u32Temp&0x00FF)));//�������ƫ��

	return 0x01;	
}
/***************************************************************************************
** ��������:Ds2756Inint
** ����������Ds2756��λ
** �������룺��
** �����������
** ע�����
******************************************************************************************/
u32	Ds2756Inint(void)
{
	u8 u8Temp[8];
	
	Ds2756PortInit();

	Ds2756Reset();  
	
	//Ds2756WriteByte(DS_NETADDRESS_SKIP);//*���������к�*/ 
	Ds2756WriteByte(DS_NETADDRESS_READ_METHOD1);
	u8Temp[7] = Ds2756ReadByte();
	u8Temp[6] = Ds2756ReadByte();
	u8Temp[5] = Ds2756ReadByte();
	u8Temp[4] = Ds2756ReadByte();
	u8Temp[3] = Ds2756ReadByte();
	u8Temp[2] = Ds2756ReadByte();
	u8Temp[1] = Ds2756ReadByte();
	u8Temp[0] = Ds2756ReadByte();
	
	return 0x01;
}
/***************************************************************************************
** ��������:Ds2756Inint
** ����������Ds2756��λ
** �������룺��
** �����������
** ע�����
******************************************************************************************/
u32	Ds2756Api(DS2756_API_MSG_TYPE sMsg,s32 *pData)
{
	u32 u32ReturnValue = 0x00;
	
	switch(sMsg)
	{
		case DS2756_READ_VOLTAGE:
			u32ReturnValue = Ds2756ReadVoltage(pData);
			break;
		
		case DS2756_READ_CURRENT:
			u32ReturnValue = Ds2756ReadCurrent(pData);
			break;
		
		case DS2756_READ_AVER_CURRENT:
			u32ReturnValue = Ds2756ReadAverageCurrent(pData);
			break;
		
		case DS2756_READ_ACCUMULATED_CURRENT:
			u32ReturnValue = Ds2756ReadAccumulatedCurrent(pData);
			break;
		
		case DS2756_READ_TEMPERATURE:
			u32ReturnValue = Ds2756ReadTemperature(pData);
			break;
		
		case DS2756_SET_ACCUMULATION_CURRENT_BIAS:
			u32ReturnValue = Ds2756SetAccumulationCurrentBias(pData);
			break;
		
		case DS2756_SET_ACCUMULATED_CURRENT:
			u32ReturnValue = Ds2756SetAccumulationCurrent(pData);
			break;
		
		default:
			break;
	}
	
	return u32ReturnValue;
}

#endif
