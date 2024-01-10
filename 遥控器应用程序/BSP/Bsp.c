/********************************************************************************
* 文件名：	bsp.c
* 作者：	马如意   
* 版本：   	V1.0
* 日期：    2015.04.28
* 功能描述:  实现BSP及时钟的初始化

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
#include <includes.h>
u32 g_u32ID;
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
/*********************************************************
** 函数名称：BSP_Init
** 功能描述：BSP初始化
** 函数输入：无
** 函数输出：无
** 注意事项：
************************************************************/
void  BSP_Init (void)
{
u32 i;
	GPIO_InitTypeDef GPIO_InitStructure;
	SystemInit();

	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);				//没有从优先级
	
	// 配置接口
	/* GPIOC Periph clock enable */
	RCC_AHBPeriphClockCmd(ID_GPIO_RCC, ENABLE);			   //使能配置使用端口时钟

	/* Configure ID0_PIN in input nopull mode */
	GPIO_InitStructure.GPIO_Pin = ID0_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(ID_PORT, &GPIO_InitStructure);
	
	
	/* Configure ID1_PIN in input nopull mode */
	GPIO_InitStructure.GPIO_Pin = ID1_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(ID_PORT, &GPIO_InitStructure);


	i = GPIO_ReadInputData(ID_PORT);
	i = GPIO_ReadInputData(ID_PORT);
	i = GPIO_ReadInputData(ID_PORT);
	i &= (ID0_PIN|ID1_PIN);
	g_u32ID = i;
}

/*********************************************************
** 函数名称：SysTick_Init
** 功能描述：时钟初始化
** 函数输入：无
** 函数输出：无
** 注意事项：
************************************************************/
void SysTick_Init(void) 
{ 
    RCC_ClocksTypeDef  rcc_clocks; 

    RCC_GetClocksFreq(&rcc_clocks);   //调用标准库函数，获取系统时钟。
    SysTick_Config(rcc_clocks.HCLK_Frequency / OS_TICKS_PER_SEC); //初始化并使能SysTick，OS_TICKS_PER_SEC指定每毫秒节拍器
}
