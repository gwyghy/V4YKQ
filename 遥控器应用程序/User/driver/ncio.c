/********************************************************************************
* 文件名：	ncio.c
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


/********************************************************************************
* 函数定义
*********************************************************************************/

/***************************************************************************************
** 函数名称:NcIoInit
** 功能描述：未使用端口灯初始化
** 函数输入：无
** 函数输出：1：正常，0：异常
** 注意事项：
******************************************************************************************/
u32 NcIoInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	if(g_u32ID == B_type)
	{
		/* GPIO Periph clock enable */
		RCC_AHBPeriphClockCmd(RCC_NCIO_1_GPIO|RCC_NCIO_2_GPIO, ENABLE);
		
		
		/* Configure LED_LOCK_PIN in output pushpull mode */
		GPIO_InitStructure.GPIO_Pin = NCIO_1_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(NCIO_1_PORT, &GPIO_InitStructure);

	/* Set LED_LOCK_PIN */
	GPIO_ResetBits(NCIO_1_PORT, NCIO_1_PIN);

	/* Configure LED_LOCK_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = NCIO_2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(NCIO_2_PORT, &GPIO_InitStructure);

		/* Set LED_LOCK_PIN */
		GPIO_ResetBits(NCIO_2_PORT, NCIO_2_PIN);	
	}
	
	return 0x01;
}
