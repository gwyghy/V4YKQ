/*************************************************************** 文件名:  driver.c** Copyright (c) 1998-1999 天津华宁电子技术有限公司技术开发部** 创建人:  ** 日　期:  ** 修改人:	** 日　期:	** 描　述:  电液控项目UCOS_II系统应用之底层驱动。**** 版　本:	V1.0A**--------------------------------------------------------------------------*************************************************************//********************************************************************************* .h头文件*********************************************************************************/#include "includes.h"/********************************************************************************* #define宏定义*********************************************************************************//********************************************************************************* 常量定义*********************************************************************************//********************************************************************************* 变量定义*********************************************************************************//********************************************************************************* 函数声明*********************************************************************************//*********************************************************** 函数名：	 Driver_Init** 输　入：  无** 输　出：  无** 功能描述：系统驱动初始化子程序。** 作　者:	 李微辰** 日　期：  2013.03.06** 版本：    V1.0A************************************************************/void Driver_Init(void){		PwrPinInint();	OledInit();	#if (YKQ_HARDWARE_BATTERY_MNG == BATTERY_DS2756_MNG)		Ds2756Inint();	#elif (YKQ_HARDWARE_BATTERY_MNG == BATTERY_ADC_MNG)		AdcInit();		#endif	KeyInit();	LedInit();	#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)		KeyLedInit();	#endif	#if (YKQ_HARDWARE_BEEP_EXIST_DEFINE != DISABLED)		BeepInit();	#endif		InputInit();		RfidRecvSemCreate();		IrSendTIMConfig();	IrUsartInit();		if(g_u32ID != B_type)	{		WlUsartInit();		TIM4_Int_Init(199,99);//20Mhz的计数频率，计数到99为1ms	}  else	{			RFID_Init(RFID_ININT_SYNC0,RFID_ININT_SYNC1);	}	EepromInint();	NcIoInit();		#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!=	DISABLED)		ZZZ_Init();	#endif	}