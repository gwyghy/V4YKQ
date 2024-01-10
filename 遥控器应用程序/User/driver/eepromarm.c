/********************************************************************************
* �ļ�����	eeprom_arm.c
* ���ߣ�	������  
* �汾��   	v1.0
* ���ڣ�    2015.4.27
* ��������:  ����STM32Lϵ���ڲ�EEPROM�Ķ�д��
* �޸�˵����   
*
*       >>>>  �ڹ����е�λ��  <<<<
*             3-Ӧ�ò�
*             2-Э���
*          ��  1-Ӳ��������
*********************************************************************************
* @copy
* <h2><center>&copy; COPYRIGHT ������������޹�˾ �з����� �����</center></h2>
*********************************************************************************/
/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/
#include "includes.h"

/********************************************************************************
* #define�궨��
*********************************************************************************/
#define	EEPROM_MUTEX_WAIT_TIME		5000//�ȴ������ź��������ʱ��

/********************************************************************************
* ��������
*********************************************************************************/
	
/********************************************************************************
* ��������
*********************************************************************************/
static OS_EVENT  *s_stMutexEeprom;	//���������ź���

/********************************************************************************
* ��������
*********************************************************************************/
/*********************************************************
** �������ƣ�EEPROM_WriteWords
** ������������������ʽ��EEPROM����д��
** �������룺Addr:��ַ,Buffer������ָ�룬Length������
** ���������None
** ע�����
************************************************************/
u32 EepromInint(void)
{
	INT8U err;

	s_stMutexEeprom = OSMutexCreate(MUTEX_EEPROM_PRIO, &err);	//�������������ź���	
	return 0x01;
}
/*********************************************************
** �������ƣ�EepromReadWords
** ������������������ʽ��EEPROM���ݶ��� 
** �������룺u32Addr:��ַ,pBuffer������ָ�룬u32Length������
** ���������None
** ע�����
************************************************************/
u32 EepromReadBytes(u32 u32Addr,u8 *pBuffer,u32 u32Length)  
{  
    u32 u32AddrTemp;  
	INT8U err;
	
	OSMutexPend(s_stMutexEeprom,EEPROM_MUTEX_WAIT_TIME/TICK_TIME,&err);
	if(err != OS_ERR_NONE)
		return 0x00;	
	/**�Ե�ַ��Χ����ȷ��**/
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
** �������ƣ�EepromReadWords
** ������������������ʽ��EEPROM���ݶ��� 
** �������룺u32Addr:��ַ,pBuffer������ָ�룬u32Length������
** ���������None
** ע�����
************************************************************/
u32 EepromReadWords(u32 u32Addr,u32 *pBuffer,u32 u32Length)  
{  
    u32 u32AddrTemp;  
	INT8U err;
	
	OSMutexPend(s_stMutexEeprom,EEPROM_MUTEX_WAIT_TIME/TICK_TIME,&err);
	if(err != OS_ERR_NONE)
		return 0x00;	
	/**�Ե�ַ��Χ����ȷ��**/
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
** �������ƣ�EepromWriteBytes
** ���������������ֽ���ʽ��EEPROM����д��
** �������룺u32Addr:��ַ,pBuffer������ָ�룬u32Length������
** ���������None
** ע�����
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
	/**�Ե�ַ��Χ����ȷ��**/
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
** �������ƣ�EepromWriteWords
** ������������������ʽ��EEPROM����д��
** �������룺u32Addr:��ַ,pBuffer������ָ�룬u32Length������
** ���������None
** ע�����
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
	/**�Ե�ַ��Χ����ȷ��**/
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



