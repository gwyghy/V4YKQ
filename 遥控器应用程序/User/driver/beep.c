/********************************************************************************
* 文件名：	beep.c
* 作者：	马如意   
* 版本：   	V1.0
* 日期：    2015.04.24
* 功能描述:  实现BEEP的驱动

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
#include	"includes.h"

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
* 函数定义
*********************************************************************************/
/***************************************************************************************
** 函数名称:Beep_Init
** 功能描述：蜂鸣器初始化
** 函数输入：无
** 函数输出：None
** 注意事项：
******************************************************************************************/
#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
	u32 BeepInit(void)
	{
		GPIO_InitTypeDef GPIO_InitStructure;

		/* BEEP_PORT Periph clock enable */
		RCC_AHBPeriphClockCmd(BEEP_GPIO_RCC, ENABLE);

		/* Configure BEEP_PIN in output pushpull mode */
		GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(BEEP_PORT, &GPIO_InitStructure);

		/* Reset BEEP_PIN */
		BEEP_OFF();

		return 0x01;
	}
#endif
