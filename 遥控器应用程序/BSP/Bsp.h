/********************************************************************************
* �ļ����ƣ�	bsp.h
* ��	�ߣ�	������   
* ��ǰ�汾��   	V1.0
* ������ڣ�    2015.04.24
* ��������: 	����bsp.hͷ�ļ�
* ��ʷ��Ϣ��   
*           	�汾��Ϣ     ���ʱ��      ԭ����        ע��
*
*       >>>>  �ڹ����е�λ��  <<<<
*             3-Ӧ�ò�
*          	  2-Э���
*          ��  1-Ӳ��������
*********************************************************************************
* Copyright (c) 2014,������������޹�˾ All rights reserved.
*********************************************************************************/
#ifndef  __BSP_H__ 
#define  __BSP_H__ 

/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/
#include "includes.h"
extern u32 g_u32ID;
/********************************************************************************
* #define�궨��
*********************************************************************************/

#define B_type    0x300


/*************************Ӳ���ӿڶ���*****************************************/
/*HSE = 20 Mhz**/
/* HCLK = SYSCLK/1 = 20 Mhz*/
/* PCLK2 = HCLK/1 = 20 Mhz*/
/* PCLK1 = HCLK /1 = 20 Mhz*/
/*************OLED GPIO �˿����ã��������ߺͿ������ߣ�*******************/
#define OLED_DATA_PINS				GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 ;
#define OLED_DATA_CLK				RCC_AHBPeriph_GPIOA
#define OLED_DATA_PORT				GPIOA

#define OLED_DC_PIN					GPIO_Pin_0
#define OLED_DC_CLK					RCC_AHBPeriph_GPIOC
#define OLED_DC_PORT				GPIOC

#define OLED_WR_PIN					GPIO_Pin_1
#define OLED_WR_CLK					RCC_AHBPeriph_GPIOC
#define OLED_WR_PORT				GPIOC

#define OLED_RD_PIN					GPIO_Pin_2
#define OLED_RD_CLK					RCC_AHBPeriph_GPIOC
#define OLED_RD_PORT				GPIOC

#define OLED_CS_PIN					GPIO_Pin_4
#define OLED_CS_CLK					RCC_AHBPeriph_GPIOC
#define OLED_CS_PORT				GPIOC

#define OLED_RT_PIN					GPIO_Pin_5
#define OLED_RT_CLK					RCC_AHBPeriph_GPIOC
#define OLED_RT_PORT				GPIOC

/********************************BATTERY ADC�ӿ�***********************************/
#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_ADC_MNG)//���õ����Ƽ������ֵ
	#define	INPUT_ADC					ADC1
	#define	RCC_INPUT_ADC_AHBxCmd()		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE)

	#define	DMA_CHANNEL_INPUT_ADC		DMA1_Channel1
	#define RCC_INPUT_ADC_DMA_Cmd()		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE)
	#define INPUT_ADC_DR_ADDRESS    	((uint32_t)0x40012458)

	/**VOLTAGE**/
	#define	GPIO_INPUT1_ADC				GPIOB
	#define	PIN_INPUT1_ADC				GPIO_Pin_1
	#define	INPUT1_ADC_CHANNEL			ADC_Channel_9
	#define RCC_INPUT1_ADC_PortCmd()	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE)
	

	/**VOLTAGE**/
	#define	GPIO_INPUT1_ADC1				GPIOB
	#define	PIN_INPUT1_ADC1					GPIO_Pin_14
	#define	INPUT1_ADC_CHANNEL1			ADC_Channel_20
	
#endif

/**************����˿�����*******************************************************************************/
#define	INPUT_WAKEUP1_PORT			GPIOA//Wakeup1
#define INPUT_WAKEUP1_PIN			GPIO_Pin_0  
#define RCC_INPUT_WAKEUP1_GPIO      RCC_AHBPeriph_GPIOA

/********************************KEY�ӿ�***********************************/
#define KEY_L0_PORT					GPIOB
#define KEY_L0_PIN					GPIO_Pin_3
#define KEY_L0_GPIO_RCC				RCC_AHBPeriph_GPIOB

#define KEY_L1_PORT					GPIOD
#define KEY_L1_PIN					GPIO_Pin_2
#define KEY_L1_GPIO_RCC				RCC_AHBPeriph_GPIOD

#define KEY_L2_PORT					GPIOC
#define KEY_L2_PIN					GPIO_Pin_12
#define KEY_L2_GPIO_RCC				RCC_AHBPeriph_GPIOC

#define KEY_L3_PORT					GPIOC
#define KEY_L3_PIN					GPIO_Pin_11
#define KEY_L3_GPIO_RCC				RCC_AHBPeriph_GPIOC

#define KEY_L4_PORT					GPIOC
#define KEY_L4_PIN					GPIO_Pin_10
#define KEY_L4_GPIO_RCC				RCC_AHBPeriph_GPIOC

#define KEY_R0_PORT					GPIOB
#define KEY_R0_PIN					GPIO_Pin_8
#define KEY_R0_GPIO_RCC				RCC_AHBPeriph_GPIOB

#define KEY_R1_PORT					GPIOB
#define KEY_R1_PIN					GPIO_Pin_7
#define KEY_R1_GPIO_RCC				RCC_AHBPeriph_GPIOB

#define KEY_R2_PORT					GPIOB
#define KEY_R2_PIN					GPIO_Pin_6
#define KEY_R2_GPIO_RCC				RCC_AHBPeriph_GPIOB

#define KEY_R3_PORT					GPIOB
#define KEY_R3_PIN					GPIO_Pin_5
#define KEY_R3_GPIO_RCC				RCC_AHBPeriph_GPIOB

#define KEY_R4_PORT					GPIOB
#define KEY_R4_PIN					GPIO_Pin_4
#define KEY_R4_GPIO_RCC				RCC_AHBPeriph_GPIOB

#define KEY_R5WKUP2_PORT			GPIOC
#define KEY_R5WKUP2_PIN				GPIO_Pin_13
#define KEY_R5WKUP2_GPIO_RCC		RCC_AHBPeriph_GPIOC

#define KEY_ZIBAO_PORT			GPIOA
#define KEY_ZIBAO_PIN				GPIO_Pin_9
#define KEY_ZIBAO_GPIO_RCC		RCC_AHBPeriph_GPIOA

/************************�������ӿ�*****************************************/
#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)
	#define BEEP_PORT				GPIOA
	#define BEEP_PIN				GPIO_Pin_9
	#define BEEP_GPIO_RCC			RCC_AHBPeriph_GPIOA
#endif

/**************************LED�ӿ�******************************************/
#define LED_1_PORT					GPIOA
#define LED_1_PIN					GPIO_Pin_11
#define LED_1_GPIO_RCC				RCC_AHBPeriph_GPIOA

#define LED_2_PORT					GPIOA
#define LED_2_PIN					GPIO_Pin_12
#define LED_2_GPIO_RCC				RCC_AHBPeriph_GPIOA

#define LED_3_PORT					GPIOA
#define LED_3_PIN					GPIO_Pin_15
#define LED_3_GPIO_RCC				RCC_AHBPeriph_GPIOA

/**************************KEY LED�ӿ�******************************************/
#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
	#define KEY_LED_MOSI_PORT			GPIOC
	#define KEY_LED_MOSI_PIN			GPIO_Pin_3
	#define KEY_MOSI_GPIO_RCC			RCC_AHBPeriph_GPIOC

	#define KEY_LED_LCK_PORT			GPIOC
	#define KEY_LED_LCK_PIN				GPIO_Pin_14
	#define KEY_LCK_GPIO_RCC			RCC_AHBPeriph_GPIOC

	#define KEY_LED_SCK_PORT			GPIOC
	#define KEY_LED_SCK_PIN				GPIO_Pin_15
	#define KEY_SCK_GPIO_RCC			RCC_AHBPeriph_GPIOC
	
	
	
	#define KEY_595_ENABLE_PORT   GPIOB
	#define KEY_595_ENABLE_PIN    GPIO_Pin_12
	#define KEY_595_ENABLE_RCC    RCC_AHBPeriph_GPIOB
#endif
/**************USART�ӿ�*******************************************************/
#define	IR_USART					USART1
#define	IR_USART_APBxCmd()			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);//���ڹ���ʱ��ʹ��
#define IR_USART_IRQN				USART1_IRQn
#define IR_USART_IRQHandler			USART1_IRQHandler

#define	IR_USART_PIN_AF_SET			GPIO_AF_USART1
#define	IR_USART_BAUNDRATE			9600

/**�����������Ŷ���**/
#define	IR_USART_RX_PIN				GPIO_Pin_10
#define	IR_USART_RX_GPIO			GPIOA
#define	IR_USART_RX_PinSource 		GPIO_PinSource10
#define RCC_IR_USART_RX_GPIO    	RCC_AHBPeriph_GPIOA

/***����ʹ�����Ŷ���**/
#define	IR_USART_ENABLE_PIN			GPIO_Pin_11
#define	IR_USART_ENABLE_GPIO		GPIOB
#define RCC_IR_USART_ENABLE_GPIO  	RCC_AHBPeriph_GPIOB


/**************USART1�ӿ�*******************************************************/
#define	WL_USART					USART3
#define	WL_USART_APBxCmd()			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);//���ڹ���ʱ��ʹ��
#define WL_USART_IRQN				USART3_IRQn
#define WL_USART_IRQHandler			USART3_IRQHandler

#define	WL_USART_PIN_AF_SET			GPIO_AF_USART3
#define	WL_USART_BAUNDRATE			115200

/**�����������Ŷ���**/
#define	WL_USART_RX_PIN				GPIO_Pin_11
#define	WL_USART_RX_GPIO			GPIOB
#define	WL_USART_RX_PinSource 		GPIO_PinSource11
#define RCC_WL_USART_RX_GPIO    	RCC_AHBPeriph_GPIOB

/**�����������Ŷ���**/
#define	WL_USART_TX_PIN				GPIO_Pin_10
#define	WL_USART_TX_GPIO			GPIOB
#define	WL_USART_TX_PinSource 		GPIO_PinSource10
#define RCC_WL_USART_TX_GPIO    	RCC_AHBPeriph_GPIOB

/**************���ⷢ��ӿ�*****************************************************/

/**************************	IDʶ�����ýӿ�******************************************/
#define	ID_PORT				GPIOC
#define ID_GPIO_RCC			RCC_AHBPeriph_GPIOC
#define ID0_PIN				GPIO_Pin_9
#define ID1_PIN				GPIO_Pin_8

/**************************	δʹ�ö˿ڶ���******************************************/
#define NCIO_1_PORT					GPIOC
#define NCIO_1_PIN					GPIO_Pin_8
#define RCC_NCIO_1_GPIO				RCC_AHBPeriph_GPIOC

#define NCIO_2_PORT					GPIOC
#define NCIO_2_PIN					GPIO_Pin_9
#define RCC_NCIO_2_GPIO				RCC_AHBPeriph_GPIOC
/********************************************************************************/

#define	RCC_IR_SEND_TIM()			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE)
#define	IR_SEND_TIM					TIM2//��ʹ�õĶ�ʱ������
#define	IR_SEND_TIMER_CLK			10000000//��ʹ�õĶ�ʱ��ʱ�Ӷ���

#define	IR_SEND_TIMOCxInit			TIM_OC3Init
#define	IR_SEND_TIMOCxReloadConf	TIM_OC3PreloadConfig

#define	IR_SEND_TIM_CHANNEL			TIM_Channel_3
#define	IR_SEND_IT_CCX				TIM_IT_CC3	
#define	IR_SEND_OCxPolariuyCfg		TIM_OC3PolarityConfig
#define	IR_SEND_CC_IRQHandler		//TIM2_IRQHandler
#define	IR_SEND_TIMx_IRQn			TIM2_IRQn

#define	IR_SEND_TIM_GetCaptureX		TIM_GetCapture3
#define	IR_SEND_TIM_SetCompareX		TIM_SetCompare3


/**************************��Դ����******************************************/
#define PWR_OLED_PORT				GPIOB
#define PWR_OLED_PIN				GPIO_Pin_0
#define RCC_PWR_OLED_GPIO			RCC_AHBPeriph_GPIOB

#define PWR_WL_PORT					GPIOB
#define PWR_WL_PIN					GPIO_Pin_9
#define RCC_PWR_WL_GPIO				RCC_AHBPeriph_GPIOB

#define LTC3440_MODE_PORT			GPIOB
#define LTC3440_MODE_PIN			GPIO_Pin_2
#define RCC_LTC3440_MODE_GPIO		RCC_AHBPeriph_GPIOB

#define LTC3440_MODE_PORT			GPIOB
#define LTC3440_MODE_PIN			GPIO_Pin_2
#define RCC_LTC3440_MODE_GPIO		RCC_AHBPeriph_GPIOB

/**************************	�����ƶ˿ڶ���******************************************/
#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)//���õ����Ƽ������ֵ
	#define DS2756_DATA_PORT		GPIOC
	#define DS2756_DATA_PIN			GPIO_Pin_8
	#define RCC_DS2756_DATA_GPIO	RCC_AHBPeriph_GPIOC

	#define DS2756_INT_PORT			GPIOC
	#define DS2756_INT_PIN			GPIO_Pin_9
	#define RCC_DS2756INT_GPIO		RCC_AHBPeriph_GPIOC	
#endif

/**************************	δʹ�ö˿ڶ���******************************************/
#define NCIO_1_PORT					GPIOC
#define NCIO_1_PIN					GPIO_Pin_8
#define RCC_NCIO_1_GPIO				RCC_AHBPeriph_GPIOC

#define NCIO_2_PORT					GPIOC
#define NCIO_2_PIN					GPIO_Pin_9
#define RCC_NCIO_2_GPIO				RCC_AHBPeriph_GPIOC
/********************************************************************************
* ��������
*********************************************************************************/

/********************************************************************************
* ȫ�ֱ�������
*********************************************************************************/

/********************************************************************************
* ��������
*********************************************************************************/
void     BSP_Init(void);
void	 SysTick_Init(void);
void     RCC_Configuration(void);
 
#endif
