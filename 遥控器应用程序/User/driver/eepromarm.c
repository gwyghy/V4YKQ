/********************************************************************************
* 文件名：	eeprom_arm.c
* 作者：	马如意  
* 版本：   	v1.0
* 日期：    2015.4.27
* 功能描述:  用于STM32L系列内部EEPROM的读写。
* 修改说明：   
*
*       >>>>  在工程中的位置  <<<<
*             3-应用层
*             2-协议层
*          √  1-硬件驱动层
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
#define	EEPROM_MUTEX_WAIT_TIME		5000//等待互斥信号量的最大时间

/********************************************************************************
* 常量定义
*********************************************************************************/
	
/********************************************************************************
* 变量定义
*********************************************************************************/
static OS_EVENT  *s_stMutexEeprom;	//发声互斥信号量

/********************************************************************************
* 函数定义
*********************************************************************************/
/*********************************************************
** 函数名称：EEPROM_WriteWords
** 功能描述：按照字形式，EEPROM数据写入
** 函数输入：Addr:地址,Buffer：数据指针，Length：长度
** 函数输出：None
** 注意事项：
************************************************************/
u32 EepromInint(void)
{
	INT8U err;

	s_stMutexEeprom = OSMutexCreate(MUTEX_EEPROM_PRIO, &err);	//创建发声互斥信号量	
	return 0x01;
}
/*********************************************************
** 函数名称：EepromReadWords
** 功能描述：按照字形式，EEPROM数据读出 
** 函数输入：u32Addr:地址,pBuffer：数据指针，u32Length：长度
** 函数输出：None
** 注意事项：
************************************************************/
u32 EepromReadBytes(u32 u32Addr,u8 *pBuffer,u32 u32Length)  
{  
    u32 u32AddrTemp;  
	INT8U err;
	
	OSMutexPend(s_stMutexEeprom,EEPROM_MUTEX_WAIT_TIME/TICK_TIME,&err);
	if(err != OS_ERR_NONE)
		return 0x00;	
	/**对地址范围进行确认**/
	if(u32Addr < DATA_EEPROM_START_ADDR)
		return  0x00;
	if((u32Addr+u32Length) > DATA_EEPROM_END_ADDR)
		return	0x00;
	
    u32AddrTemp = u32Addr;  
    while(u32AddrTemp < (u32Addr+u32Length))
	{  
        *pBuffer = *(u8 *)u32AddrTemp; 
		pBuffer++;	
		u32AddrTemp += 0x01;
    }
	OSMutexPost(s_stMutexEeprom);
	return 0x01;
}
/*********************************************************
** 函数名称：EepromReadWords
** 功能描述：按照字形式，EEPROM数据读出 
** 函数输入：u32Addr:地址,pBuffer：数据指针，u32Length：长度
** 函数输出：None
** 注意事项：
************************************************************/
u32 EepromReadWords(u32 u32Addr,u32 *pBuffer,u32 u32Length)  
{  
    u32 u32AddrTemp;  
	INT8U err;
	
	OSMutexPend(s_stMutexEeprom,EEPROM_MUTEX_WAIT_TIME/TICK_TIME,&err);
	if(err != OS_ERR_NONE)
		return 0x00;	
	/**对地址范围进行确认**/
	if(u32Addr < DATA_EEPROM_START_ADDR)
		return  0x00;
	if((u32Addr+u32Length) > DATA_EEPROM_END_ADDR)
		return	0x00;
	
    u32AddrTemp = u32Addr;  
    while(u32AddrTemp < (u32Addr+u32Length))
	{  
        *pBuffer = *(u32 *)u32AddrTemp; 
		pBuffer++;	
		u32AddrTemp += 0x04;
    }
	OSMutexPost(s_stMutexEeprom);
	return 0x01;
}

/*********************************************************
** 函数名称：EepromWriteBytes
** 功能描述：按照字节形式，EEPROM数据写入
** 函数输入：u32Addr:地址,pBuffer：数据指针，u32Length：长度
** 函数输出：None
** 注意事项：
************************************************************/
u32 EepromWriteBytes(u32 u32Addr,u8 *pBuffer,u32 u32Length)  
{  
    u32 u32AddrTemp;  
	INT8U err;
	FLASH_Status FLASHStatus = FLASH_COMPLETE;
	u16 u16WriteTimes = 0x00;
	
	OSMutexPend(s_stMutexEeprom,EEPROM_MUTEX_WAIT_TIME/TICK_TIME,&err);
	if(err != OS_ERR_NONE)
		return 0x00;
	/**对地址范围进行确认**/
	if(u32Addr < DATA_EEPROM_START_ADDR)
		return  0x00;
	if((u32Addr+u32Length) > DATA_EEPROM_END_ADDR)
		return	0x00;	
	
	u32AddrTemp = u32Addr;
	u16WriteTimes = 0x00;
	DATA_EEPROM_Unlock();
	while(u32AddrTemp < (u32Addr+u32Length ))
	{
		if(((u32Addr+u32Length )-u32AddrTemp)>=0x04)
			FLASHStatus = DATA_EEPROM_FastProgramWord(u32AddrTemp, *(u32 *)pBuffer);
		else
			FLASHStatus = DATA_EEPROM_FastProgramByte(u32AddrTemp, *pBuffer);

		if(FLASHStatus == FLASH_COMPLETE)
		{
			if(((u32Addr+u32Length )-u32AddrTemp)>=0x04)
			{
				u32AddrTemp += 0x04;
				pBuffer += 0x04;			
			}
			else
			{
				u32AddrTemp += 0x01;
				pBuffer++;
			}
		}
		else
		{
			FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
			   | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR);
			u16WriteTimes++;
			if(u16WriteTimes >= 500)
				break;
		}
	}	
	DATA_EEPROM_Lock();
	OSMutexPost(s_stMutexEeprom);
	
	if(u16WriteTimes < 500)
		return 0x01;
	else
		return 0x00;
}

/*********************************************************
** 函数名称：EepromWriteWords
** 功能描述：按照字形式，EEPROM数据写入
** 函数输入：u32Addr:地址,pBuffer：数据指针，u32Length：长度
** 函数输出：None
** 注意事项：
************************************************************/
u32 EepromWriteWords(u32 u32Addr,u32 *pBuffer,u32 u32Length)  
{  
    u32 u32AddrTemp;  
	INT8U err;
	FLASH_Status FLASHStatus = FLASH_COMPLETE;
	u16 u16WriteTimes = 0x00;
	
	OSMutexPend(s_stMutexEeprom,EEPROM_MUTEX_WAIT_TIME/TICK_TIME,&err);
	if(err != OS_ERR_NONE)
		return 0x00;
	/**对地址范围进行确认**/
	if(u32Addr < DATA_EEPROM_START_ADDR)
		return  0x00;
	if((u32Addr+u32Length) > DATA_EEPROM_END_ADDR)
		return	0x00;	
	
	u32AddrTemp = u32Addr;
	u16WriteTimes = 0x00;
	DATA_EEPROM_Unlock();
	while(u32AddrTemp < (u32Addr+u32Length ))
	{
		FLASHStatus = DATA_EEPROM_ProgramWord(u32AddrTemp, *pBuffer);

		if(FLASHStatus == FLASH_COMPLETE)
		{
			u32AddrTemp += 0x04;
			pBuffer++;
		}
		else
		{
			FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
			   | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR);
			u16WriteTimes++;
			if(u16WriteTimes >= 500)
				break;
		}
	}	
	DATA_EEPROM_Lock();
	OSMutexPost(s_stMutexEeprom);
	
	if(u16WriteTimes < 500)
		return 0x01;
	else
		return 0x00;
}



