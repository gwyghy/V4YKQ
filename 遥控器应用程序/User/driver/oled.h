/********************************************************************************
* 文件名称：	oled.h
* 作	者：	马如意   
* 当前版本：   	V1.0
* 完成日期：    2015.04.24
* 功能描述: 	定义bsp.h头文件
* 历史信息：   
*           	版本信息     完成时间      原作者        注释
*
*       >>>>  在工程中的位置  <<<<
*             3-应用层
*          	  2-协议层
*          √  1-硬件驱动层
*********************************************************************************
* Copyright (c) 2014,天津华宁电子有限公司 All rights reserved.
*********************************************************************************/
#ifndef __OLED_H__ 
#define __OLED_H__
/********************************************************************************
* .h头文件
*********************************************************************************/
#include "includes.h"

/********************************************************************************
* #define宏定义
*********************************************************************************/
#define OLED_USE_EXFLASH_ZIKU	0
#define OLED_USE_INFLASH_ZIKU	1
#define	OLED_ZIKU_USEFLAG		OLED_USE_INFLASH_ZIKU

/********************************************************************************
* 常量定义
*********************************************************************************/
enum
{
	OLED_DISPLAY_NORMAL = 0x00,//正常显示
	OLED_DISPLAY_INTERVAL	= 0x01//反色显示
};

/********************************************************************************
* 全局变量声明
*********************************************************************************/

/********************************************************************************
* 函数声明
*********************************************************************************/
u32 OledContrastAdj(uint8_t dt);
u32 OledInit(void);
u32 OledShutDown(void);
u32 OledShutOn(void);

u32 LcdClr(uint16_t u16Inv);
u32 LcdAreaClr(uint16_t u16Inv, uint16_t u16Top, uint16_t u16Left, uint16_t u16Bottom, uint16_t u16Right);
uint32_t ShowChar_HZ16b(uint16_t u16Inv, const uint16_t u16Val, uint16_t u16Row, uint16_t u16Col);
void Display_Character(uint16_t no,uint8_t site);

u32 ShowStr16b(uint16_t u16Inv, const u8 *cu8Str,uint16_t u16Row, uint16_t u16Col);
uint32_t ShowChar_ASCII16b(uint16_t u16Inv, const uint16_t u16Val, uint16_t u16Row, uint16_t u16Column);
#if (OLED_ZIKU_USEFLAG	==	OLED_USE_INFLASH_ZIKU)
	u32  ShowChar_Special(uint16_t u16Inv, uint8_t Number,uint16_t u16Row,uint16_t u16column);                     //显示一个字
#endif

#endif //__OLED_H__

