/********************************************************************************
* 文件名：	led.c
* 作者：	马如意   
* 版本：   	V1.0
* 日期：    2015.04.24
* 功能描述:  实现LED的驱动

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
static u32	s_u32LedState = 0x00;	//LED指示灯的状态

/********************************************************************************
* 函数定义
*********************************************************************************/

/***************************************************************************************
** 函数名称:Led_Init
** 功能描述：LED灯初始化
** 函数输入：无
** 函数输出：1：正常，0：异常
** 注意事项：
******************************************************************************************/
u32 LedInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIO Periph clock enable */
	RCC_AHBPeriphClockCmd(LED_1_GPIO_RCC|LED_2_GPIO_RCC|LED_3_GPIO_RCC, ENABLE);

	/* Configure LED_LOCK_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = LED_1_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_1_PORT, &GPIO_InitStructure);

	/* Set LED_LOCK_PIN */
	GPIO_SetBits(LED_1_PORT, LED_1_PIN);

	/* Configure LED_STOP_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = LED_2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_2_PORT, &GPIO_InitStructure);

	/* Set LED_STOP_PIN */
	GPIO_SetBits(LED_2_PORT, LED_2_PIN);

	/* Configure LED_STOP_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = LED_3_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_3_PORT, &GPIO_InitStructure);

	/* Set LED_STOP_PIN */
	GPIO_ResetBits(LED_3_PORT, LED_3_PIN);
	
	s_u32LedState = 0x00;
	
	return 0x01;
}

/***************************************************************************************
** 函数名称:StateLed
** 功能描述：LED灯的处理
** 函数输入：u16On:需要点亮的LED灯,u16Off:需要熄灭的LED灯
** 函数输出：1：正常，0：异常
** 注意事项：
******************************************************************************************/
u32 StateLed(u16 u16On, u16 u16Off)
{
	if (u16On)
	{
		s_u32LedState |= u16On;
		if (u16On & LED_1)
			GPIO_ResetBits(LED_1_PORT, LED_1_PIN);
		if (u16On & LED_2)
			GPIO_ResetBits(LED_2_PORT, LED_2_PIN);
		if (u16On & LED_3)
			GPIO_SetBits(LED_3_PORT, LED_3_PIN);		
	}
	
	if (u16Off)
	{
		s_u32LedState &= (~u16Off);
		if (u16Off & LED_1)
			GPIO_SetBits(LED_1_PORT, LED_1_PIN);
		if (u16Off & LED_2)
			GPIO_SetBits(LED_2_PORT, LED_2_PIN);
		if (u16Off & LED_3)
			GPIO_ResetBits(LED_3_PORT, LED_3_PIN);		
	}
	
	return 0x01;
}

/***************************************************************************************
** 函数名称:LedToggle
** 功能描述：LED灯进行闪烁的处理
** 函数输入：u16Toggle:需要闪烁的LED灯
** 函数输出：1：正常，0：异常
** 注意事项：
******************************************************************************************/
u32 LedToggle(u16 u16Toggle)
{
	if (u16Toggle)
	{
		if (u16Toggle & LED_1)
		{
			if((s_u32LedState&LED_1) == 0x00)
			{	
				GPIO_ResetBits(LED_1_PORT, LED_1_PIN);
				s_u32LedState |= u16Toggle;
			}
			else
			{	
				GPIO_SetBits(LED_1_PORT, LED_1_PIN);			
				s_u32LedState &= (~u16Toggle);
			}
		}
		if (u16Toggle & LED_2)
		{	
			if((s_u32LedState&LED_2) == 0x00)
			{	
				GPIO_ResetBits(LED_2_PORT, LED_2_PIN);
				s_u32LedState |= u16Toggle;
			}
			else
			{	
				GPIO_SetBits(LED_2_PORT, LED_2_PIN);			
				s_u32LedState &= (~u16Toggle);
			}
		}
		if (u16Toggle & LED_3)
		{	
			if((s_u32LedState&LED_3) == 0x00)
			{	
				GPIO_SetBits(LED_3_PORT, LED_3_PIN);;
				s_u32LedState |= u16Toggle;
			}
			else
			{	
				GPIO_ResetBits(LED_3_PORT, LED_3_PIN);				
				s_u32LedState &= (~u16Toggle);
			}		
		}
		return 0x01;	
	}
	return 0x00;
}
