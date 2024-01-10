/********************************************************************************
* 文件名称：RfidUpdate.h
* 作	者：马如意   
* 当前版本：V1.0
* 完成日期：2017.06.22
* 功能描述: 定义RfidMng.h头文件
* 历史信息：   
*           	版本信息     完成时间      原作者        注释
*
*       >>>>  在工程中的位置  <<<<
*             3-应用层
*          √ 2-协议层
*          	  1-硬件驱动层
*********************************************************************************
* Copyright (c) 2014,天津华宁电子有限公司 All rights reserved.
*********************************************************************************/


#ifndef __RFIDUPDATE_H__
#define __RFIDUPDATE_H__

#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG	!= DISABLED)
/********************************************************************************
* .h头文件
*********************************************************************************/
#include "includes.h"

/********************************************************************************
* #define宏定义
*********************************************************************************/
/**本项目中使用的无线数据格式***/
__packed typedef struct
{
	__packed union
	{
		__packed struct
		{
			unsigned int	SDL:6;				//接收方ID、发送方ID，共计6bit。
			unsigned int	FrameType:10;		//帧类别
			unsigned int	Reservd:16;			//预留区域。
		} ID;//帧ID
		u32 u32Id;//帧ID
	} u32ID;	//帧ID
	u8	u8DLC;				//帧长度
	u8	u8DT[55];			//帧数据
}RFID_UPDATE_FRAME_TYPE;

/**帧类型定义**/
enum
{
	RFID_FT_UPDATE_DWL_VERSION = 0x1EF,//传输版本信息
	RFID_FT_UPDATE_WRITE_VERSION = 0x1EE,//写入版本信息	
	RFID_FT_UPDATE_DWL_CODE = 0x1ED,//传输程序代码
	RFID_FT_UPDATE_WRITE_CODE = 0x1EC,//写入版本信息
	RFID_FT_UPDATE_END = 0x1EB	//更新过程结束
};

enum
{
	WL_RFID_FT_UPDATE_DWL_VERSION = 0x2EF,//传输版本信息
	WL_RFID_FT_UPDATE_WRITE_VERSION = 0x2EE,//写入版本信息	
	WL_RFID_FT_UPDATE_DWL_CODE = 0x2ED,//传输程序代码
	WL_RFID_FT_UPDATE_WRITE_CODE = 0x2EC,//写入版本信息
	WL_RFID_FT_UPDATE_END = 0x2EB	//更新过程结束
};

//配置文件同步
enum
{
	ACTION_INFO_SYNCHRO_DWL_VERSION = 0x1DF,//配置文件版本信息帧
	ACTION_INFO_SYNCHRO_WRITE_VERSION = 0x1DE,//配置文件版本信息写入帧	
	ACTION_INFO_SYNCHRO_DWL_CODE = 0x1DD,//配置文件数据帧
	ACTION_INFO_SYNCHRO_WRITE_CODE = 0x1DC,//配置文件数据写入帧
	ACTION_INFO_SYNCHRO_END = 0x1DB	//配置文件结束帧
};

/********************************************************************************
* 常量定义
*********************************************************************************/

/********************************************************************************
* 全局变量声明
*********************************************************************************/

/********************************************************************************
* 函数声明
*********************************************************************************/
u32  RfidUpdateSendPrgData(u8 u8DevID,u32 u32FrameType,u32 u32PacketNumb,u8 *data,u32 u32DataLength);
void RfidUpdateMngInint(void);
void RfidMng_Update_Task(void *p_arg);

#endif

#endif

