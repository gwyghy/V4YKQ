/********************************************************************************
* 文件名：	oled.c
* 作者：	马如意   
* 版本：   	V1.0
* 日期：    2015.04.24
* 功能描述:  实现OLED的驱动

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

#include "const.h"

/********************************************************************************
* #define宏定义
*********************************************************************************/
/* task stack size */ 
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

#define PAR_MAX_BODER 384

/********************************************************************************
* 常量定义
*********************************************************************************/

/********************************************************************************
* 变量定义
*********************************************************************************/
/********************************************************************************
* 函数定义
*********************************************************************************/
/*********************************************************
** 函数名：OLED_Send_Data
** 输　入：dt --- oled data
** 输　出：None
** 功能描述：Send a config command to OLED
** 全局变量：None
** 调用模块：None
** 作　者:	李微辰
** 日　期： 2012.12.27
** 修　改：
** 日　期：
** 版本：V1.0A
************************************************************/
void OledSendData(uint8_t dt)
{
	u16 u16Temp = 0x00;

	u16Temp = (((u16)dt) << 1);
	
	OLED_DATA_PORT->ODR = (OLED_DATA_PORT->IDR & 0xFE01) | (u16Temp & 0x1FE);
}

/*********************************************************
** 函数名：OLED_Write_Command
** 输　入：cm --- oled command
** 输　出：None
** 功能描述：Send a config command to OLED
** 全局变量：None
** 调用模块：None
** 作　者:	李微辰
** 日　期： 2012.12.27
** 修　改：
** 日　期：
** 版本：V1.0A
************************************************************/
void OledWriteCommand(uint8_t cm)
{
	GPIO_ResetBits(OLED_DC_PORT, OLED_DC_PIN);
	GPIO_ResetBits(OLED_CS_PORT, OLED_CS_PIN);
	GPIO_ResetBits(OLED_WR_PORT, OLED_WR_PIN);

	OledSendData(cm);

	GPIO_SetBits(OLED_WR_PORT, OLED_WR_PIN);
	GPIO_SetBits(OLED_CS_PORT, OLED_CS_PIN);
	GPIO_SetBits(OLED_DC_PORT, OLED_DC_PIN);
}

/*********************************************************
** 函数名：OLED_Write_Data
** 输　入：dt --- oled data
** 输　出：None
** 功能描述：Send data to OLED
** 全局变量：None
** 调用模块：None
** 作　者:	李微辰
** 日　期： 2012.12.27
** 修　改：
** 日　期：
** 版本：V1.0A
************************************************************/
u32 OledWriteData(uint8_t dt)
{
	GPIO_SetBits(OLED_DC_PORT, OLED_DC_PIN);
	GPIO_ResetBits(OLED_CS_PORT, OLED_CS_PIN);
	GPIO_ResetBits(OLED_WR_PORT, OLED_WR_PIN);

	OledSendData(dt);

	GPIO_SetBits(OLED_WR_PORT, OLED_WR_PIN);
	GPIO_SetBits(OLED_CS_PORT, OLED_CS_PIN);
	GPIO_SetBits(OLED_DC_PORT, OLED_DC_PIN);

	return 0x01;
}

/*********************************************************
** 函数名：OLED_Contrast_ADJ
** 输　入：dt --- contrast
** 输　出：None
** 功能描述：Send data to OLED
** 全局变量：None
** 调用模块：None
** 作　者:	李微辰
** 日　期： 2012.12.27
** 修　改：
** 日　期：
** 版本：V1.0A
************************************************************/
u32 OledContrastAdj(uint8_t dt)
{
	OledWriteCommand(0x81);  //contrast: 159					//发送对比度命令字
	OledWriteCommand(dt);  //-----------------------			//发送对比度参数

	return 0x01;
}

/***********************************************************************************************
** 函 数 名：	DelayUs()
** 功能描述：	延时函数，微秒级
** 输　  入：	
** 输　  出：	
** 作　  者：	马如意
** 日　  期：	2016.09.21
** 版    本：	V1.0.0
** 更新记录：
** 更新记录：
** 					日    期      姓    名                    描      述
** 					==========  =============  ========================================
**
************************************************************************************************/
static void DelayUs(u32 u32Counter)
{
	u32 u32i;
	u32 u32j;
	u32 uSLoops;
	RCC_ClocksTypeDef  rcc_clocks; 

	RCC_GetClocksFreq(&rcc_clocks);   //调用标准库函数，获取系统时钟。
	uSLoops = rcc_clocks.HCLK_Frequency/1000000;
	for (u32i = 0; u32i < u32Counter; u32i++)
	{
		u32j = 0;
		while(u32j++<uSLoops);
	}
}

/*********************************************************
** 函数名：OLED_LowLevel_Init
** 输　入：None
** 输　出：None
** 功能描述：Initialize the OLED's port.
** 全局变量：None
** 调用模块：None
** 作　者:	李微辰
** 日　期： 2012.12.27
** 修　改：
** 日　期：
** 版本：V1.0A
************************************************************/
u32 OledLowLevelInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* GPIOD Periph clock enable */
	RCC_AHBPeriphClockCmd(OLED_DATA_CLK | OLED_CS_CLK | OLED_DC_CLK | OLED_WR_CLK | OLED_RD_CLK | OLED_RT_CLK, ENABLE);		   //使能OLED使用端口时钟

    /* Configure PD0 and PD2 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = OLED_DATA_PINS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(OLED_DATA_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = OLED_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(OLED_CS_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = OLED_DC_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(OLED_DC_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = OLED_WR_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(OLED_WR_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = OLED_RD_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(OLED_RD_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = OLED_RT_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(OLED_RT_PORT, &GPIO_InitStructure);

	GPIO_SetBits(OLED_CS_PORT, OLED_CS_PIN);
	GPIO_SetBits(OLED_DC_PORT, OLED_DC_PIN);
	GPIO_SetBits(OLED_WR_PORT, OLED_WR_PIN);
	GPIO_SetBits(OLED_RD_PORT, OLED_RD_PIN);
	GPIO_SetBits(OLED_RT_PORT, OLED_RT_PIN);

	GPIO_ResetBits(OLED_RT_PORT, OLED_RT_PIN);							   //复位OLED

	DelayUs(10000);//必须的延时

	GPIO_SetBits(OLED_RT_PORT, OLED_RT_PIN);

	return 0x01;
}

/*********************************************************
** 函数名：OLED_Init
** 输　入：None
** 输　出：None
** 功能描述：Initialize the OLED's fuction.
** 全局变量：None
** 调用模块：None
** 作　者:	李微辰
** 日　期： 2012.12.27
** 修　改：
** 日　期：
** 版本：V1.0A
************************************************************/
u32 OledInit(void)
{
	uint8_t i,j;

	OledLowLevelInit();

	OledWriteCommand(0xae);/* set display off */

	OledWriteCommand(0x00);/* set lower column start address */
	OledWriteCommand(0x10);/* set higher column start address */

if(g_u32ID != B_type)	
	OledWriteCommand(0xa8); 
else
	OledWriteCommand(0x40);/* set display start line */ 
	OledWriteCommand(0x2e);  
	
	OledWriteCommand(0x81);/* set contrast control */
	OledWriteCommand(0x32);  
	OledWriteCommand(0x82);  
	OledWriteCommand(0x80); 
	
	
if(g_u32ID != B_type)
{
	OledWriteCommand(0x21);
	OledWriteCommand(3);
	OledWriteCommand(131);
//	OledWriteCommand(0);
//	OledWriteCommand(127); 
}
	
if(g_u32ID != B_type)	
	OledWriteCommand(0xa0);/* set segment remap */
else	
	OledWriteCommand(0xa1);/* set segment remap */ 
	
	OledWriteCommand(0xa6);/* set normal display */
	OledWriteCommand(0xa8);/* set multiplex ratio */
	OledWriteCommand(0x3f);/* 1/64 */
	OledWriteCommand(0xad);/* master configuration */
	OledWriteCommand(0x8e);/* external vcc supply */
	
if(g_u32ID != B_type)	
	OledWriteCommand(0xc0);/* set segment remap */
else	
	OledWriteCommand(0xc8);/* set com scan direction */

	OledWriteCommand(0xd3);/* set display offset */
	OledWriteCommand(0x40); 
	OledWriteCommand(0xd5);/* set display clock divide/oscillator frequency */
	OledWriteCommand(0xf0);

	OledWriteCommand(0xd8);/*set area color mode off */
	OledWriteCommand(0x05);
	OledWriteCommand(0xd9);
	OledWriteCommand(0xf1); 
	OledWriteCommand(0xda);/* set com pin configuartion */ 

	OledWriteCommand(0x12);
	OledWriteCommand(0x91); 
	OledWriteCommand(0x3f); 
	OledWriteCommand(0x3f);
	OledWriteCommand(0x3f);  

	OledWriteCommand(0x3f); 
	OledWriteCommand(0xaf);/* set display on */
	
	for(i=0;i<8;i++)
	{
		OledWriteCommand(0xb0|i);
		OledWriteCommand(0x00);
		OledWriteCommand(0x10);
		
		for(j=0;j<132;j++)
		{
			OledWriteData(0x00);
		}
	}

	return 0x01;
}

/***************************************************************************************
** 函数名称:OledShutDown
** 功能描述：Oled关机时的处理
** 函数输入：
** 函数输出：
** 注意事项：
******************************************************************************************/
u32 OledShutDown(void)
{
	OledWriteCommand(0xae);  //turn off lcd
	
	return 0x01;
}

/***************************************************************************************
** 函数名称:OledShutDown
** 功能描述：Oled开机时的处理
** 函数输入：
** 函数输出：
** 注意事项：
******************************************************************************************/
u32 OledShutOn(void)
{
	OledWriteCommand(0xaf);  //turn on lcd
	
	return 0x01;
}

/*********************************************************
** 函数名：	 LcdClr
** 输　入：  uint16_t u16Inv   ---- 正（0）/反（1）字标志								   
** 输　出：  无
** 功能描述：清屏子程序。
** 作　者:	 李微辰
** 日　期：  2013.03.06
** 版本：    V1.0A
************************************************************/
u32 LcdClr(uint16_t u16Inv)
{
	uint16_t i = 0x00,j = 0x00,cr = 0x00;
	
	cr=((u16Inv==OLED_DISPLAY_NORMAL)?0x00:0xff);
	for(i=0;i<8;i++)
	{
		OledWriteCommand(0xb0|i);				  //发送页地址
		OledWriteCommand(0x00);					  //发送偏移量
		OledWriteCommand(0x10);
		
		for(j=0;j<132;j++)
		{
			OledWriteData(cr);				 //清屏
		}
	}

	return 0x01;
}

/*********************************************************
** 函数名：	 LcdAreaClr
** 输　入：  uint16_t u16Inv   ---- 正（0）/反（1）字标志
**           uint16_t u16Top   ---- 起始页地址
**           uint16_t u16Left  ----	起始偏移量
**           uint16_t u16Bottom---- 结束页地址 
**           uint16_t u16Right ---- 结束偏移量								   
** 输　出：  无
** 功能描述：部分清屏子程序。
** 作　者:	 李微辰
** 日　期：  2013.03.06
** 版本：    V1.0A
************************************************************/
u32 LcdAreaClr(uint16_t u16Inv, uint16_t u16Top, uint16_t u16Left, uint16_t u16Bottom, uint16_t u16Right)
{
	uint16_t i,j,cr;
	
	cr=((u16Inv==OLED_DISPLAY_NORMAL)?0x00:0xff);
	
	for(i=u16Top;i<=u16Bottom;i++)
	{
		OledWriteCommand(0xb0|i);						//发送页地址
		OledWriteCommand(0x00+((u16Left)&0x000f));	//发送偏移量
		OledWriteCommand(0x10+((u16Left)>>4));
		
		if(g_u32ID == B_type)
		{
			for(j=u16Left+2;j<=(u16Right+2);j++)
			{
				OledWriteData(cr);						//清屏
			}
		}
		else
		{
			for(j=u16Left+2;j<=(u16Right+6);j++)
			{
				OledWriteData(cr);						//清屏
			}
		}
	}
	return 0x01;
}

/*********************************************************
** 函数名：Display_Character
** 输　入：no --- where is number 
**         site --- where to display
** 输　出：None
** 功能描述：Display a certain character on the fixed place on pannel.
** 全局变量：None
** 调用模块：None
** 作　者:	李微辰
** 日　期： 2012.12.27
** 修　改：
** 日　期：
** 版本：V1.0A
************************************************************/
#if (OLED_ZIKU_USEFLAG	==	OLED_USE_EXFLASH_ZIKU)
	void OledDisplayCharacter(uint16_t no,uint8_t site)
	{
		uint8_t pg=no/2000;
		uint16_t ad=(no%2000)*32;
		uint8_t count;
		uint8_t schar[32];//={0x0,0x0,0x82,0xff,0x80,0x0,0x0,0x0,0,0,0,0,0,0,0,0,0x0,0x0,0x00,0x0,0x00,0,0,0,0,0,0,0,0,0,0,0};
		
		W25QXX_ReadBuffer(schar, (pg<<16)+ad, 32);

		pg=(site/8)*2;
		ad=site%8;
		OledWriteCommand(0xb0|pg);
		OledWriteCommand(0x00+2);
		OledWriteCommand(0x10+ad);
		for(count=0;count<16;count++)
		{
			OledWriteCommand(schar[count]);				
		}
		
		OledWriteCommand(0xb0|(pg+1));
		OledWriteCommand(0x00+2);
		OledWriteCommand(0x10+ad);
		for(count=0;count<16;count++)
		{
			OledWriteCommand(schar[16+count]);
		}
		
		return 0x01;
	}
#endif
/*********************************************************
** 函数名：	 ShowChar_HZ16b
** 输　入：  uint16_t u16Inv   ---- 正（0）/反（1）字标志
**           const uint16_t u16Val  ----	显示字符
**			 u16 u16Row        ---- 起始页地址
**			 u16 u16Col        ----	起始偏移量								   
** 输　出：  无
** 功能描述：显示汉字字符子程序。
** 作　者:	 李微辰
** 日　期：  2013.03.06
** 版本：    V1.0A
************************************************************/
#if 1		//zzw
u32 ShowChar_HZ16b(uint16_t u16Inv, const uint16_t u16Val, uint16_t u16Row, uint16_t u16Col)
{
	uint8_t pg;
	u32 ad;
	uint8_t count;
	uint8_t schar[32];//={0x0,0x0,0x82,0xff,0x80,0x0,0x0,0x0,0,0,0,0,0,0,0,0,0x0,0x0,0x00,0x0,0x00,0,0,0,0,0,0,0,0,0,0,0};
	uint16_t tmp;
//	INT8U err;
	
	tmp = ((u16Val>>8)-0xb0)*96 + (u16Val&0x00ff)-0xa0;
// 	pg = tmp/1920;
	ad = ZIKU_BASH_ADDR + tmp*32;

	//只包括常见字部分
	if((u16Val>>8)<0xB0) 
		return ERROR_UP_OVERFLOW;			  
	if((u16Val&0x00ff)<0xa0) 
		return ERROR_DOWN_OVERFLOW;
	if((u16Val>>8)>0xD7) 
		return ERROR_UP_OVERFLOW;
	if(u16Row>6) 
		return ERROR_OUTOFFRAME;
	if(u16Col>112) 
		return ERROR_OUTOFFRAME;
	
	//等待信号量
//	OSMutexPend(mutex_w25qxx_wr, 0, &err);//等待信号量
	IapReadBuf(schar, ad, 32);				 //读取字库数据
//	OSMutexPost(mutex_w25qxx_wr);//发送信号量
	
	pg=u16Row;
	ad=u16Col + 4;			//zzw
	OledWriteCommand(0xb0|pg);							 //发送页地址
	OledWriteCommand(0x00+ad&0x0f);						 //发送偏移量
	OledWriteCommand(0x10+(ad>>4));
	for(count=0;count<16;count++)
	{
		OledWriteData((u16Inv==OLED_DISPLAY_NORMAL)?schar[count]:~schar[count]);	//写入正字或反字			
	}
	
	OledWriteCommand(0xb0|(pg+1));						 //发送页地址
	OledWriteCommand(0x00+ad&0x0f);						 //发送偏移量
	OledWriteCommand(0x10+(ad>>4));
	for(count=0;count<16;count++)
	{
		OledWriteData((u16Inv==OLED_DISPLAY_NORMAL)?schar[16+count]:~schar[16+count]);   //写入正字或反字
	}
	
	return SUCCESS;
}
#endif
/*********************************************************
** 函数名：	 ShowChar_ASCII16b
** 输　入：  uint16_t u16Inv   ---- 正（0）/反（1）字标志
**           const uint16_t u16Val  ----	显示字符
**			 u16 u16Row        ---- 起始页地址
**			 u16 u16Col        ----	起始偏移量								   
** 输　出：  无
** 功能描述：显示ASCII字符子程序。
** 作　者:	 李微辰
** 日　期：  2013.03.06
** 版本：    V1.0A
************************************************************/
#if (OLED_ZIKU_USEFLAG	==	OLED_USE_EXFLASH_ZIKU)
u32 ShowCharAscii16b(uint16_t u16Inv, const uint16_t u16Val, uint16_t u16Row, uint16_t u16Col)
{
	uint8_t pg;
	uint16_t ad;
	uint8_t count;
	uint8_t schar[16];//={0x0,0x0,0x82,0xff,0x80,0x0,0x0,0x0,0,0,0,0,0,0,0,0,0x0,0x0,0x00,0x0,0x00,0,0,0,0,0,0,0,0,0,0,0};
	uint16_t tmp;
	INT8U err;
	tmp = u16Val-0x20;
	pg = 5;
	ad = tmp*16;

	if(u16Val>0x83) 
		return ERROR_UP_OVERFLOW;
	if(u16Row>6) 
		return ERROR_OUTOFFRAME;
	if(u16Col>120) 
		return ERROR_OUTOFFRAME;

	//等待信号量
	OSMutexPend(mutex_w25qxx_wr, 0, &err);//等待信号量
	W25QXX_ReadBuffer(schar, (pg<<16)+ad, 16);			  //读取字库数据
	OSMutexPost(mutex_w25qxx_wr);//发送信号量
	
	pg=u16Row;
	ad=u16Col;
	OledWriteCommand(0xb0|pg);						  //发送页地址
	OledWriteCommand(0x00+ad&0x0f);					  //发送偏移量
	OledWriteCommand(0x10+(ad>>4));
	for(count=0;count<8;count++)
	{
		OledWriteData((u16Inv==OLED_DISPLAY_NORMAL)?schar[count]:~schar[count]);	//写入正字或反字			
	}
	OledWriteCommand(0xb0|(pg+1));					  //发送页地址
	OledWriteCommand(0x00+ad&0x0f);					  //发送偏移量
	OledWriteCommand(0x10+(ad>>4));
	for(count=0;count<8;count++)
	{
		OledWriteData((u16Inv==OLED_DISPLAY_NORMAL)?schar[8+count]:~schar[8+count]);
	}
	
	return SUCCESS;
}
#endif

/***************************************************************************************
** 函数名称:ShowStr_16b
** 功能描述：显示字符串
** 函数输入：cu8Str:输入字符串首地址， u16Row：输入页，u16Col：输入列
** 函数输出：None
** 注意事项：
******************************************************************************************/
#if (OLED_ZIKU_USEFLAG	==	OLED_USE_EXFLASH_ZIKU)
u32 ShowStr16b(uint16_t u16Inv, const u8 *cu8Str,uint8_t len, u16 u16Row, u16 u16Col)
{
	uint8_t i,p;
	
	for(i=0,p=0;i<len;i++)
	{
		if(*(cu8Str+p)>0xa0) 
		{
			ShowChar_HZ16b(u16Inv,(*(cu8Str+p+1))+((*(cu8Str+p))<<8), u16Row, u16Col+(8*p));
			p+=2;
		}
		else
		{
			ShowChar_ASCII16b(u16Inv,*(cu8Str+p), u16Row, u16Col+(8*p));
			p++;
		}
	}

	return 0x01;
}
#endif

/*********************************************************
** 函数名：	 ShowStr_16b
** 输　入：  uint16_t u16Inv   ---- 正（0）/反（1）字标志
**           const u8 *cu8Str  ----	显示字符串
**			 uint8_t len       ----	长度
**			 u16 u16Row        ---- 起始页地址
**			 u16 u16Col        ----	起始偏移量							   
** 输　出：  无
** 功能描述：显示字符串子程序。
** 作　者:	 李微辰
** 日　期：  2013.03.06
** 版本：    V1.0A
** 作　者:	 田东升
** 日　期：  2014.04.15
** 版本：    V1.2
** 功能描述：增加超长字符串换行显示功能；可以显示回车换行；返回字符串显示完毕后的行列位置。
************************************************************/
#if 1		//zzw
	u32 ShowStr16b(uint16_t u16Inv, const u8 *cu8Str,u16 u16Row, u16 u16Col)
	{
		uint16_t i;
		uint16_t hanzi = 0;			//=1:汉字字符串
		uint16_t lenx = strlen((const char *)cu8Str);
		uint16_t u16Rowx = u16Row;
		uint16_t u16Colx = u16Col;

		hanzi = 0;
		for (i = 0; i < lenx; i++)
		{
			if (*(cu8Str+i) > 0xa0)					//判断是汉字还是ASCII码
			{
				hanzi = 1;
				break;
			}
		}
		
		for (i = 0; i < lenx; i++)
		{
			if (*(cu8Str+i) > 0xa0)					//判断是汉字还是ASCII码
			{
				if (u16Colx > 112)					//显示超宽，换行
				{
					u16Colx = 0;
					u16Rowx += 2;					//汉字需要2行
				}
				if (u16Rowx > 6)					//字符串过长，屏幕显示不下
					break;
					
				ShowChar_HZ16b(u16Inv, (*(cu8Str+i+1))+((*(cu8Str+i))<<8), u16Rowx, u16Colx);
				u16Colx += 16;
				i++;								//汉字2字节
			}
			else
			{
				if ((*(cu8Str+i) == 0x0a) || (*(cu8Str+i) == 0x0d))	//判断是否回车或换行
				{
					u16Colx = 0;
					u16Rowx++;
					if (hanzi == 1)
						u16Rowx++;					//汉字需要2行
				}
				else
				{
					if (u16Colx > 120)				//显示超宽，换行
					{
						u16Colx = 0;
						u16Rowx++;
						if (hanzi == 1)
							u16Rowx++;				//汉字需要2行
					}
					if (hanzi == 1)					//是否汉字字符串
					{
						if (u16Rowx > 6)			//字符串过长，屏幕显示不下
							break;
					}
					else
					{
						if (u16Rowx > 7)			//字符串过长，屏幕显示不下
							break;
					}
					
					ShowChar_ASCII16b(u16Inv,*(cu8Str+i), u16Rowx, u16Colx);  //显示ASCII码
					u16Colx += 8;
				}
			}
		}
		return (((u32)(u16Colx) << 16) + u16Rowx);
	}
#endif

/*********************************************************
** 函数名称：OLED_Display1
** 功能描述：OLED显示16*16的字符(主要是特殊自制字符)
** 函数输入：u16Inv:正（0）/反（1）字标志,显示的字符Number，显示的页page，显示的列column
** 函数输出：1：正常，0：异常
** 注意事项：
************************************************************/
#if (OLED_ZIKU_USEFLAG	==	OLED_USE_INFLASH_ZIKU)
u32  ShowChar_Special(uint16_t u16Inv, uint8_t Number,uint16_t u16Row,uint16_t u16column)	 //显示单个字
{

	uint16_t ad,count;
	
	if(Number >= (sizeof(SpecialSymbol)/32))
		return 0x00;
	
	ad=Number*32;
	if(g_u32ID != B_type)
		u16column = u16column + 4;	//屏幕分辨率为132*64，需要地址偏移量保证显示内容在显示区域内
	
	//oled显示
	OledWriteCommand(0xb0|u16Row);                       //oled写页命令
	OledWriteCommand(0x00+(u16column&0x0F));              //oled写列命令
	OledWriteCommand(0x10+((u16column&0xF0)>>4));
	for (count=0;count<16;count++)
	{  
		OledWriteData(SpecialSymbol[ad+count]);
	}

	OledWriteCommand(0xb0|(u16Row+1));
	OledWriteCommand(0x00+(u16column&0x0F));
	OledWriteCommand(0x10+((u16column&0xF0)>>4));
	for (count=0;count<16;count++)
	{  
		OledWriteData(SpecialSymbol[ad+count+16]&(0xff>>(u16Row/6)));//取出最后一页的一行
	}
	
	return 0x01;
	
}
#endif
/*********************************************************
** 函数名：	 ShowChar_ASCII16b
** 输　入：  uint16_t u16Inv   ---- 正（0）/反（1）字标志
**           const uint16_t u16Val  ----	显示字符
**			 u16 u16Row        ---- 起始页地址
**			 u16 u16Col        ----	起始偏移量								   
** 输　出：  无
** 功能描述：显示ASCII字符子程序。
** 作　者:	 李微辰
** 日　期：  2013.03.06
** 版本：    V1.0A
************************************************************/
#if (OLED_ZIKU_USEFLAG	==	OLED_USE_INFLASH_ZIKU)
uint32_t ShowChar_ASCII16b(uint16_t u16Inv, const uint16_t u16Val, uint16_t u16Row, uint16_t u16Column)
{
	uint8_t count;
	uint16_t tmp;
	//INT8U err;
	
	tmp = u16Val-0x20;

	if(u16Val>0x83) 
		return ERROR_UP_OVERFLOW;
	if(u16Row>6) 
		return ERROR_OUTOFFRAME;
	if(u16Column>120) 
		return ERROR_OUTOFFRAME;

	if(g_u32ID != B_type)
		u16Column = u16Column + 4;	//屏幕分辨率为132*64，需要地址偏移量保证显示内容在显示区域内
	
	OledWriteCommand(0xb0|u16Row);						  //发送页地址
	OledWriteCommand(0x00+((u16Column)&0x0F));					  //发送偏移量
	OledWriteCommand(0x10+(((u16Column)&0xF0)>>4));
	for(count=0;count<8;count++)
	{
		OledWriteData((u16Inv==OLED_DISPLAY_NORMAL)?ASCII[tmp][count]:~ASCII[tmp][count]);	//写入正字或反字			
	}
	
	OledWriteCommand(0xb0|(u16Row+1));						//发送页地址
	OledWriteCommand(0x00+((u16Column)&0x0F));					//发送偏移量
	OledWriteCommand(0x10+(((u16Column)&0xF0)>>4));
	for(count=0;count<8;count++)
	{
		OledWriteData((u16Inv==OLED_DISPLAY_NORMAL)?ASCII[tmp][8+count]:~ASCII[tmp][8+count]);
	}
	
	return SUCCESS;
}
#endif
/***************************************************************************************
** 函数名称: seek_word(uint16_t number)
** 功能描述：查找字符
** 函数输入：查找字符编码
** 函数输出：1：正常，0：异常
** 注意事项：
******************************************************************************************/
#if 0			//zzw
 u32   seek_word(uint16_t number, uint16_t *pId)
{
	uint16_t head,tail;
	uint32_t u32ReturnValue = 0x00;
	
	head=0;
	tail=(sizeof codeGB_16/sizeof(typFNT_GB16));

	while(head < tail)
	{	  
		if(number==codeGB_16[head].Index[0]*256+codeGB_16[head].Index[1])
		{
			*pId = head;
			u32ReturnValue = 0x01;
			break;
		}
		head ++;
	}	
	return  u32ReturnValue;
}

/***************************************************************************************
** 函数名称:ShowStr_16b(uint8_t fashion,const u8 *cu8Str,uint16_t u16Row, uint16_t u16Col)
** 功能描述：显示单个字
** 函数输入：u16Val：显示字符编号， u16Row：输入页，u16Col：输入列
** 函数输出：1：正常，0：异常
** 注意事项：
******************************************************************************************/
u32  OledDisplayZ(uint16_t u16Inv, uint16_t u16Val,uint8_t page,uint8_t column)	 //显示单个字
{
	uint16_t count;
	
  if(g_u32ID != B_type)
		column += 4;
	OledWriteCommand(0xb0|page);
	OledWriteCommand(0x00+((column)&0x0F));
	OledWriteCommand(0x10+(((column)&0xF0)>>4));
	for (count=0;count<16;count++)
	{  
		OledWriteData((u16Inv==OLED_DISPLAY_NORMAL)?codeGB_16[u16Val].Msk[count]:~codeGB_16[u16Val].Msk[count]);	//写入正字或反字		
	}

	OledWriteCommand(0xb0|(page+1));
	OledWriteCommand(0x00+((column)&0x0F));
	OledWriteCommand(0x10+(((column)&0xF0)>>4));
	for (count=0;count<16;count++)
	{  
		OledWriteData((u16Inv==OLED_DISPLAY_NORMAL)?codeGB_16[u16Val].Msk[count+16]:~codeGB_16[u16Val].Msk[count+16]);
	}  
	return 0x01;
}
   
/***************************************************************************************
** 函数名称:ShowStr_16b
** 功能描述：显示字符串
** 函数输入：cu8Str:输入字符串首地址， u16Row：输入页，u16Col：输入列
** 函数输出：1：正常，0：异常
** 注意事项：
******************************************************************************************/
u32 ShowStr16b(uint16_t u16Inv, const u8 *cu8Str,uint16_t u16Row, uint16_t u16Col)
{
   	uint16_t i,j;
	uint16_t lenx=strlen((const char *)cu8Str);
	uint16_t hanzi = 0;			//=1:汉字字符串
	uint16_t u16Rowx = u16Row;
	uint16_t u16Colx = u16Col;
	uint32_t u32Flag = 0x00;
	
	hanzi = 0;
	for (i = 0; i < lenx; i++)
	{
		if (*(cu8Str+i) > 0xa0)					//判断是汉字还是ASCII码
		{
			hanzi = 1;
			break;
		}
	}
	
//	for(i=0;i<lenx;i++)
//	{
//		j=seek_word((*(cu8Str+i+1))+((*(cu8Str+i))<<8));
//		i++;
//		OledDisplayZ(u16Inv,j,u16Row,u16Col+(8*i));	
//	}
	
	for (i = 0; i < lenx; i++)
	{
		if (*(cu8Str+i) > 0xa0)					//判断是汉字还是ASCII码
		{
			if (u16Colx > 112)					//显示超宽，换行
			{
				u16Colx = 0;
				u16Rowx += 2;					//汉字需要2行
			}
			if (u16Rowx > 6)					//字符串过长，屏幕显示不下
				break;
			
			u32Flag = seek_word((*(cu8Str+i+1))+((*(cu8Str+i))<<8),&j);	
			if(u32Flag == 0x01)
				OledDisplayZ(u16Inv,j,u16Rowx,u16Colx);		
			u16Colx += 16;
			i++;
// 			ShowChar_HZ16b(u16Inv, (*(cu8Str+i+1))+((*(cu8Str+i))<<8), u16Rowx, u16Colx);
// 			u16Colx += 16;
// 			i++;								//汉字2字节
		}
		else
		{
			if ((*(cu8Str+i) == 0x0a) || (*(cu8Str+i) == 0x0d))	//判断是否回车或换行
			{
				u16Colx = 0;
				u16Rowx++;
				if (hanzi == 1)
					u16Rowx++;					//汉字需要2行
			}
			else
			{
				if (u16Colx > 120)				//显示超宽，换行
				{
					u16Colx = 0;
					u16Rowx++;
					if (hanzi == 1)
						u16Rowx++;				//汉字需要2行
				}
				if (hanzi == 1)					//是否汉字字符串
				{
					if (u16Rowx > 6)			//字符串过长，屏幕显示不下
						break;
				}
				else
				{
					if (u16Rowx > 7)			//字符串过长，屏幕显示不下
						break;
				}
				
				ShowChar_ASCII16b(u16Inv,*(cu8Str+i), u16Rowx, u16Colx);  //显示ASCII码
				u16Colx += 8;
			}
		}
	}
	
	return 0x01;
}
#endif


