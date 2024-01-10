/********************************************************************************
* 文件名：	pwr.c
* 作者：	马如意  
* 版本：   	v1.0
* 日期：    2015.4.28
* 功能描述:  用于STM32L系列低功耗电源管理。
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

/********************************************************************************
* #define宏定义
*********************************************************************************/

/********************************************************************************
* 常量定义
*********************************************************************************/
/********************************************************************************
* 变量定义
*********************************************************************************/

/********************************************************************************
* 函数声明
*********************************************************************************/
/***************************************************************************************
** 函数名称:PwrPinInit
** 功能描述：OLED电源管理初始化
** 函数输入：无
** 函数输出：1：正常，0：异常
** 注意事项：
******************************************************************************************/
u32 PwrPinInint(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIO Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_PWR_OLED_GPIO|RCC_PWR_WL_GPIO|RCC_LTC3440_MODE_GPIO, ENABLE);

	/* Configure LTC3440_MODE_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = LTC3440_MODE_PIN;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LTC3440_MODE_PORT, &GPIO_InitStructure);
	if(g_u32ID == B_type)
	{
		Ltc3440BurstModeDisabled();
	}
	else
	{
		Ltc3440BurstModeEnabled();
	}
	
	/* Configure LED_STOP_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = PWR_WL_PIN;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(PWR_WL_PORT, &GPIO_InitStructure);

	/* 打开无线供电电源 */
	WirelessChipPwrOn();

	/* Configure PWR_OLED_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = PWR_OLED_PIN;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(PWR_OLED_PORT, &GPIO_InitStructure);

	/* 打开OLED供电电源 */
	OledChipPwrOn();
	
	return 0x01;
	
}

