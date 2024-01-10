/********************************************************************************
* 文件名：	adc.c
* 作者：	马如意 
* 版本：   	v1.0
* 日期：    2015.4.27
* 功能描述:  用于STM32L系列内部AD转换。
* 修改说明：   
*
*       >>>>  在工程中的位置  <<<<
*             3-应用层
*             2-协议层
*           √ 1-硬件驱动层
*********************************************************************************
* @copy
* <h2><center>&copy; COPYRIGHT 天津华宁电子有限公司 研发中心 软件部</center></h2>
*********************************************************************************/
/********************************************************************************
* .h头文件
*********************************************************************************/
#include "includes.h"

#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_ADC_MNG)//在硬件中，使用AD方式计算电量
/********************************************************************************
* #define宏定义
*********************************************************************************/
#define ADC_BUF_SIZE  							60
#define	ADC_CHANNEL_MAX							3
#define	ADC_PERCHANNEL_SAMPLE_TIMES	(ADC_BUF_SIZE/(ADC_CHANNEL_MAX-1))

#define	ADC_PERCHANNEL_SAMPLE_TIMES_D	(ADC_BUF_SIZE/(ADC_CHANNEL_MAX))

/********************************************************************************
* 常量定义
*********************************************************************************/
static uint16_t s_u16ConvertedValue[ADC_BUF_SIZE];


/********************************************************************************
* 变量定义
*********************************************************************************/

/********************************************************************************
* 函数声明
*********************************************************************************/
/*********************************************************
** 函数名称：Input1AdcInit
** 功能描述：ADC初始化
** 函数输入：无
** 函数输出：无
** 注意事项：
************************************************************/
u32 InputAdcInit(void)
{
	DMA_InitTypeDef DMA_InitStructure;	
	NVIC_InitTypeDef NVIC_InitTypeStructure;	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure; 


	
	RCC_HSICmd(ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟
	ADC_TempSensorVrefintCmd((FunctionalState)ENABLED);
	RCC_INPUT1_ADC_PortCmd();////端口时钟

	//PB1 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = PIN_INPUT1_ADC;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//模拟输入引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIO_INPUT1_ADC, &GPIO_InitStructure);
	
	if(g_u32ID != B_type)
	{
		//PB1 作为模拟通道输入引脚                         
		GPIO_InitStructure.GPIO_Pin = PIN_INPUT1_ADC1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//模拟输入引脚
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
		GPIO_Init(GPIO_INPUT1_ADC1, &GPIO_InitStructure);	
	}

	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInit(&ADC_CommonInitStructure);   //ADC 预分频	

	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//外部触发电平选择方式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	if(g_u32ID == B_type)
		ADC_InitStructure.ADC_NbrOfConversion = ADC_CHANNEL_MAX-1;	//顺序进行规则转换的ADC通道的数目
	else
		ADC_InitStructure.ADC_NbrOfConversion = ADC_CHANNEL_MAX;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

	ADC_RegularChannelConfig(INPUT_ADC, INPUT1_ADC_CHANNEL, 1, ADC_SampleTime_384Cycles ); //Sample time equal to 96 cycles
	
	if(g_u32ID != B_type)
		ADC_RegularChannelConfig(INPUT_ADC, INPUT1_ADC_CHANNEL1, 3, ADC_SampleTime_384Cycles ); //Sample time equal to 96 cycles
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_Vrefint, 2, ADC_SampleTime_384Cycles ); // Sample time equal to 96 cycles

	NVIC_InitTypeStructure.NVIC_IRQChannel=ADC1_IRQn;
	NVIC_InitTypeStructure.NVIC_IRQChannelPreemptionPriority=0x09;//不使用中断优先级嵌套。因为SysTick的中断优先级为0x0f
	NVIC_InitTypeStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitTypeStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitTypeStructure);

	ADC_ITConfig(INPUT_ADC,ADC_IT_OVR,ENABLE); //OVR中断使能

	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1

	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)          // Wait until ADC1 ON status 
	{
	}
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_RCNR) != RESET);          // 0 regular channel ready;1 regular channel not ready,SWSTART must not be set 

	/* Enable DMA1 clock */
	RCC_INPUT_ADC_DMA_Cmd();

	/* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_DeInit(DMA_CHANNEL_INPUT_ADC);
	DMA_InitStructure.DMA_PeripheralBaseAddr = INPUT_ADC_DR_ADDRESS;    //DMA对应的外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&s_u16ConvertedValue;   //内存存储基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	//DMA的转换模式为SRC模式，由外设搬移到内存
	DMA_InitStructure.DMA_BufferSize = ADC_BUF_SIZE;		   //DMA缓存大小，ADC_BUF_SIZE个,单位为DMA_MemoryDataSize
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//接收一次数据后，设备地址禁止后移
	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;//开启接收一次数据后，目标内存地址后移
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //定义外设数据宽度为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //DMA搬数据尺寸，HalfWord就是为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   //转换模式，循环缓存模式。
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//DMA优先级高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		  //M2M模式禁用
	DMA_Init(DMA_CHANNEL_INPUT_ADC, &DMA_InitStructure);    

	/* Enable ADC1 DMA */
	ADC_DMACmd(INPUT_ADC, ENABLE);	  //ADC命令，使能

	/* Disable ADC1 DMA REQUEST*/
	ADC_DMARequestAfterLastTransferCmd(INPUT_ADC, ENABLE);//ADC请求，允许

	ADC_SoftwareStartConv(ADC1);//Enables the selected ADC software start conversion of the regular channels.

	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA_CHANNEL_INPUT_ADC, ENABLE);

	return 0x01;

}

/*********************************************************
** 函数名称：AdcInit
** 功能描述：ADC初始化
** 函数输入：无
** 函数输出：无
** 注意事项：
************************************************************/
u32 AdcInit(void)
{
	u16 u16I = 0x00;

	for (u16I=0;u16I<ADC_BUF_SIZE;u16I++)
	{
		s_u16ConvertedValue[u16I] = 0x00;
	}	
	
	InputAdcInit();

	return 0x01;
}

/*********************************************************
** 函数名称：GetBatteryAdcAverageValue
** 功能描述：获取AD采样的平均值
** 函数输入：u16Data：数据指针
** 函数输出：1:操作成功
** 注意事项：
************************************************************/
u32 GetAdcAverageValue(ADINPUT_CHANNEL_TYPE channel,u16 *u16Data)
{
	u32 u32sum = 0x00;
	u16 u16I = 0x00;
	u32 u32returnvalue = 0x00;
	
	if(g_u32ID == B_type)
	{
		switch(channel)
		{
			case ADINPUT_1:
				for (u16I=0;u16I<ADC_BUF_SIZE;u16I++,u16I++)
				{
					u32sum += s_u16ConvertedValue[u16I];
				}
				u32returnvalue = 0x01;
				break;

			case ADINPUT_2:
				for (u16I=1;u16I<ADC_BUF_SIZE;u16I++,u16I++)
				{
					u32sum += s_u16ConvertedValue[u16I];
				}
				u32returnvalue = 0x01;
				break;
				
			default:
				u32returnvalue = 0x00;
				break;
		}
		*u16Data = (u16)(u32sum/ADC_PERCHANNEL_SAMPLE_TIMES);
	}
	else if(g_u32ID != B_type)
	{
		switch(channel)
		{
			case ADINPUT_1:
				for (u16I=0;u16I<ADC_BUF_SIZE;u16I++,u16I++,u16I++)
				{
					u32sum += s_u16ConvertedValue[u16I];
				}
				u32returnvalue = 0x01;
				break;

			case ADINPUT_2:
				for (u16I=1;u16I<ADC_BUF_SIZE;u16I++,u16I++,u16I++)
				{
					u32sum += s_u16ConvertedValue[u16I];
				}
				u32returnvalue = 0x01;
				break;
				
			case ADINPUT_3:
				for (u16I=2;u16I<ADC_BUF_SIZE;u16I++,u16I++,u16I++)
				{
					u32sum += s_u16ConvertedValue[u16I];
				}
				u32returnvalue = 0x01;
				break;
				
			default:
				u32returnvalue = 0x00;
				break;
		}		
	*u16Data = (u16)(u32sum/ADC_PERCHANNEL_SAMPLE_TIMES_D);
	}	
	return u32returnvalue;
}

/*********************************************************
** 函数名称：ADC_IRQHandler
** 功能描述：ADC中断处理
** 函数输入：无
** 函数输出：无
** 注意事项：
************************************************************/
void ADC1_IRQHandler(void)
{
	OSIntEnter();

	if(ADC_GetFlagStatus(INPUT_ADC,ADC_FLAG_OVR))
	{
		ADC_ClearFlag(INPUT_ADC,ADC_FLAG_OVR);
		/* Enable DMA1 channel1 */
		DMA_Cmd(DMA_CHANNEL_INPUT_ADC, DISABLE);
		InputAdcInit();
	}
	
	OSIntExit();
}
#endif
