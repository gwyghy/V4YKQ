/********************************************************************************
* 文件名：	ds2756.c
* 作者：	马如意   
* 版本：   	V1.0
* 日期：    2015.10.23
* 功能描述:  实现DS2756的驱动

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
#include "includes.h"

#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//在硬件中，使用DS2756方式计算电量
/********************************************************************************
* #define宏定义
*********************************************************************************/
#define DqOutputHigh()		GPIO_SetBits(DS2756_DATA_PORT, DS2756_DATA_PIN)//输出高电平
#define DqOutputLow()		GPIO_ResetBits(DS2756_DATA_PORT, DS2756_DATA_PIN)//输出低电平	

#define DQ_DATA_KEEP_TIME		20//单位us

#define	DQ_DATA_RESET_TIME_1 	800//单位us
#define	DQ_DATA_RESET_TIME_2 	250//单位us
#define	DQ_DATA_RESET_TIME_3 	400//单位us

#define	VOLTAGE_SAMPLE_UNITS				4880//单位uv
#define	CURRENT_SAMPLE_UNITS				15625 //单位uv
#define	AVERRAGE_CURRENT_SAMPLE_UNITS		1953 //单位uv
#define	ACCUMULATED_CURRENT_SAMPLE_UNITS	6250 //单位uvh
#define	TEMPERATURE_SAMPLE_UNITS			125 //单位千分之一度
/********************************************************************************
* 常量定义
*********************************************************************************/
/**寄存器定义**/
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

	DS_REG_ACR_BIAS	=	0x33,//计算电流的偏差
};

/**命令定义**/
enum
{
	DS_CMD_READ_DATA = 0x69,
	DS_CMD_WRITE_DATA = 0x6C,
	DS_CMD_COPY_DATA = 0x48,//将RAM数据向E2PROM备份
	DS_CMD_RECALL_DATA = 0xB8,//从E2PROM读取数据写入RAM
	DS_CMD_LOCK = 0x64,
	DS_CMD_SYNC = 0xD2,
};

/**读取网络地址相关**/
enum
{
	DS_NETADDRESS_READ_METHOD1 = 0x33,
	DS_NETADDRESS_READ_METHOD2 = 0x39,
	DS_NETADDRESS_MATCH = 0x55,	
	DS_NETADDRESS_SKIP = 0xCC,
	DS_NETADDRESS_SEARCH = 0xF0	
};
/********************************************************************************
* 变量定义
*********************************************************************************/

/********************************************************************************
* 函数定义
*********************************************************************************/

/***************************************************************************************
** 函数名称:Ds2756PortInit
** 功能描述：Ds2756端口初始化
** 函数输入：无
** 函数输出：1：正常，0：异常
** 注意事项：
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
		
	#if 0//暂时不适用中断方式		
		/**INT外部中断相关的配置**///INT中断与无线中断为同一个中断处理函数
		SYSCFG_EXTILineConfig(DS2756_INT_PORT_SOURCE,DS2756_INT_PIN_SOURCE);

		/* DS2756_INT_EXTI_LINE */
		EXTI_InitStructure.EXTI_Line = DS2756_INT_EXTI_LINE;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);


		/* EXTI9_5_IRQn */
		NVIC_InitStructure.NVIC_IRQChannel = DS2756_INT_EXTI_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;//优先级最低
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure); 
	#endif
	
	return 0x00;
}

/***************************************************************************************
** 函数名称:Ds2756DelayUs
** 功能描述：Ds2756延时微秒函数
** 函数输入：无
** 函数输出：1：正常，0：异常
** 注意事项：
******************************************************************************************/
void Ds2756DelayUs(u32 u32Data)
{
    u32 u32I;
    for(u32I=u32Data*2;u32I>0;u32I--)
			;
}

/***************************************************************************************
** 函数名称:Ds2756WriteByte
** 功能描述：Ds2756写一个字节的数据
** 函数输入：无
** 函数输出：无
** 注意事项：
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
** 函数名称:Ds2756WriteByte
** 功能描述：Ds2756写一个字节的数据
** 函数输入：无
** 函数输出：无
** 注意事项：
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
** 函数名称:Ds2756Reset
** 功能描述：Ds2756复位
** 函数输入：无
** 函数输出：无
** 注意事项：
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
** 函数名称:Ds2756ReadVoltage
** 功能描述：Ds2756读取电压值
** 函数输入：无
** 函数输出：无
** 注意事项：
******************************************************************************************/
u16 Ds2756ReadVoltage(s32 *pValue)
{
	u32 u32tvalue = 0x00;
	u32 u32Temp = 0x00;
	
	Ds2756Reset(); 
	Ds2756WriteByte(DS_NETADDRESS_SKIP);//*跳过读序列号*/ 
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
		*pValue = u32tvalue*(VOLTAGE_SAMPLE_UNITS)/1000;//单位mv
		*pValue *= -1;
	}
	else
	{
		*pValue = u32tvalue*(VOLTAGE_SAMPLE_UNITS)/1000;//单位mv
	}
	return 0x01;
}

/***************************************************************************************
** 函数名称:Ds2756ReadCurrent
** 功能描述：Ds2756读取电流值
** 函数输入：无
** 函数输出：无
** 注意事项：
******************************************************************************************/
u16 Ds2756ReadCurrent(s32 *pValue)
{
	u32 u32tvalue;
	u32 u32Temp;
	
	Ds2756Reset(); 
	Ds2756WriteByte(DS_NETADDRESS_SKIP);//*跳过读序列号*/ 
	Ds2756WriteByte(DS_CMD_READ_DATA);//read data 2756 
 
	Ds2756WriteByte(DS_REG_CURT_MSB);//电流值
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
		*pValue = u32tvalue*CURRENT_SAMPLE_UNITS/DS2756_RSNS_VALUE/1000;//单位mA
		*pValue *= -1;//单位mA
	}
	else
	{	
		*pValue = u32tvalue*CURRENT_SAMPLE_UNITS/DS2756_RSNS_VALUE/1000;//单位mA
	}	
	
	
	return 0x01;
}
/***************************************************************************************
** 函数名称:Ds2756ReadAverageCurrent
** 功能描述：Ds2756读取累计电量值
** 函数输入：无
** 函数输出：无
** 注意事项：
******************************************************************************************/
u16 Ds2756ReadAverageCurrent(s32 *pValue)
{
	u32 u32tvalue;
	u32 u32Temp;
	
	Ds2756Reset(); 
	Ds2756WriteByte(DS_NETADDRESS_SKIP);//*跳过读序列号*/ 
	Ds2756WriteByte(DS_CMD_READ_DATA);//read data 2756 
 
	Ds2756WriteByte(DS_REG_AV_CURT_MSB);//平均电流值
	u32Temp = (u32)(Ds2756ReadByte());
	u32tvalue = (u32)(Ds2756ReadByte());
	u32Temp <<= 8;
	u32tvalue |= u32Temp; 
	
	if(u32tvalue > 0x7FFF)
	{
		u32tvalue -= 0x01;
		u32tvalue = ~u32tvalue;
		u32tvalue &= 0x7FFF;
		*pValue = u32tvalue*AVERRAGE_CURRENT_SAMPLE_UNITS/DS2756_RSNS_VALUE/1000;//单位mA
		*pValue *= -1;
	}
	else
	{
		*pValue = u32tvalue*AVERRAGE_CURRENT_SAMPLE_UNITS/DS2756_RSNS_VALUE/1000;//单位mA
	}
	return 0x01;
}

/***************************************************************************************
** 函数名称:Ds2756ReadAccumulatedCurrent
** 功能描述：Ds2756读取累计电量值
** 函数输入：无
** 函数输出：无
** 注意事项：
******************************************************************************************/
u16 Ds2756ReadAccumulatedCurrent(s32 *pValue)
{
	u32 u32tvalue;
	u32 u32Temp;
	
	Ds2756Reset(); 
	Ds2756WriteByte(DS_NETADDRESS_SKIP);//*跳过读序列号*/ 
	Ds2756WriteByte(DS_CMD_READ_DATA);//read data 2756 
 
	Ds2756WriteByte(DS_REG_ACR_MSB);//累计电流值
	u32Temp = (u32)(Ds2756ReadByte());
	u32tvalue = (u32)(Ds2756ReadByte());
	u32Temp <<= 8;
	u32tvalue |= u32Temp; 
	
	if(u32tvalue > 0x7FFF)
	{
		u32tvalue -= 0x01;
		u32tvalue = ~u32tvalue;
		u32tvalue &= 0x7FFF;
		*pValue = u32tvalue*ACCUMULATED_CURRENT_SAMPLE_UNITS/DS2756_RSNS_VALUE/1000;//单位mAH	
		*pValue *= -1;
	}
	else
	{		
		*pValue = u32tvalue*ACCUMULATED_CURRENT_SAMPLE_UNITS/DS2756_RSNS_VALUE/1000;//单位mAH		
	}
	return 0x01;
}

/***************************************************************************************
** 函数名称:Ds2756ReadAccumulatedCurrent
** 功能描述：Ds2756读取电压值
** 函数输入：无
** 函数输出：无
** 注意事项：
******************************************************************************************/
u16 Ds2756ReadTemperature(s32 *pValue)
{
	u32 u32tvalue;
	u32 u32Temp;
	
	Ds2756Reset(); 
	Ds2756WriteByte(DS_NETADDRESS_SKIP);//*跳过读序列号*/ 
	Ds2756WriteByte(DS_CMD_READ_DATA);//read data 2756 
 
	Ds2756WriteByte(DS_REG_TEMP_MSB);//温度值
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
		*pValue = u32tvalue*TEMPERATURE_SAMPLE_UNITS/1000;//单位1度
		*pValue *= -1;
	}
	else
	{
		*pValue = u32tvalue*TEMPERATURE_SAMPLE_UNITS/1000;//单位1度		
	}
	return 0x01;
}

/***************************************************************************************
** 函数名称:Ds2756ReadAccumulatedCurrent
** 功能描述：Ds2756读取电压值
** 函数输入：无
** 函数输出：无
** 注意事项：
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
	Ds2756WriteByte(DS_NETADDRESS_SKIP);//*跳过读序列号*/ 
	Ds2756WriteByte(DS_CMD_WRITE_DATA);//read data 2756 
 
	Ds2756WriteByte(DS_REG_ACR_BIAS);//计算电量偏差
	Ds2756WriteByte(u8Temp);//计算电量偏差

	return 0x01;
}

/***************************************************************************************
** 函数名称:Ds2756SetAccumulationCurrent
** 功能描述：Ds2756设置累计电量值
** 函数输入：无
** 函数输出：无
** 注意事项：
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
	Ds2756WriteByte(DS_NETADDRESS_SKIP);//*跳过读序列号*/ 
	Ds2756WriteByte(DS_CMD_WRITE_DATA);//
 
	Ds2756WriteByte(DS_REG_ACR_MSB);//计算电量偏差
	Ds2756WriteByte((u8)((u32Temp&0xFF00)>>8));//计算电量偏差
	Ds2756WriteByte((u8)((u32Temp&0x00FF)));//计算电量偏差

	return 0x01;	
}
/***************************************************************************************
** 函数名称:Ds2756Inint
** 功能描述：Ds2756复位
** 函数输入：无
** 函数输出：无
** 注意事项：
******************************************************************************************/
u32	Ds2756Inint(void)
{
	u8 u8Temp[8];
	
	Ds2756PortInit();

	Ds2756Reset();  
	
	//Ds2756WriteByte(DS_NETADDRESS_SKIP);//*跳过读序列号*/ 
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
** 函数名称:Ds2756Inint
** 功能描述：Ds2756复位
** 函数输入：无
** 函数输出：无
** 注意事项：
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
