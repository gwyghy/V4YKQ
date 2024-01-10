/********************************************************************************
* �ļ����ƣ�	app_cfg.h
* ��	�ߣ�		������   
* ��ǰ�汾��  V1.0
* ������ڣ�  2015.05.11
* ��������: 	����app_cfg.hͷ�ļ�
* ��ʷ��Ϣ��   
*           	�汾��Ϣ     ���ʱ��      ԭ����        ע��
*
*       >>>>  �ڹ����е�λ��  <<<<
*         ��   3-Ӧ�ò�
*          	  2-Э���
*          		1-Ӳ��������
*********************************************************************************
* Copyright (c) 2014,������������޹�˾ All rights reserved.
*********************************************************************************/
/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/
#ifndef __APP_CFG_H__
#define __APP_CFG_H__

/*******************task priority �������ȼ�����*********************************/ 
#define MUTEX_BEEP_PRIO						5	//�����źŻ�����
#define MUTEX_EEPROM_PRIO					6	//EEPROM���������źŻ�����

#define SCAN_KEY_TASK_PRIO					7	//����ɨ���������ȼ�
#define RFIDMNG_TASK_PRIO					8 	//RFID���ݴ����������ȼ�
#define USART_TASK_PRIO					12 	//USART���ݴ����������ȼ�

#define IRMNG_TASK_PRIO						9//�������ݴ����������ȼ�
#define LOGIC_TASK_PRIO						10	//�߼����������ȼ�
#define INTERFACE_TASK_PRIO					11	//�˻�������ʾ�밴�������������ȼ�

#ifdef HARDWARE_TEST
	#define	HARAWARE_TEST_TASK_PRIO			14//�߼��������ջ��С
#endif

#define BATTERYMNG_TASK_PRIO				13   //��ع�������
#define BUZZERMNG_TASK_PRIO					16//��������������

#define RFIDMNG_UPDATE_TASK_PRIO			15//������������

#define APPMNG_TASK_PRIO					28	//Ӧ�ò����������ȼ�


#define IWDG_TASK_PRIO						29	//�������Ź�����	//ע�⣺���Ƕ����������ȼ������������������Ҫ�ı�OS_LOWEST_PRIO
	
//OS_LOWEST_PRIO������OS��������OS_LOWEST_PRIO-1������OSͳ������

/*******************�����ջ��С����***************************************************/
#define APPMNG_TASK_STK_SIZE				TASK_STK_SIZE_256	//Ӧ�ò��������ջ��С

#define INTERFACE_TASK_STK_SIZE				TASK_STK_SIZE_512	//�˻����������ջ��С
#define KEYSCAN_TASK_STK_SIZE				TASK_STK_SIZE_128	//����ɨ�������ջ��С
#define BATTERYMNG_TASK_STK_SIZE			TASK_STK_SIZE_128	//��ع��������ջ��С
#define IWDG_TASK_STK_SIZE					TASK_STK_SIZE_64	//���Ź����������ջ��С


#define BUZZERMNG_TASK_STK_SIZE				TASK_STK_SIZE_128	//��ع��������ջ��С

#define LOGIC_TASK_STK_SIZE					TASK_STK_SIZE_256//�߼��������ջ��С
#define IRMNG_TASK_STK_SIZE					TASK_STK_SIZE_256//�߼��������ջ��С
#define RFIDMNG_TASK_STK_SIZE				TASK_STK_SIZE_512//�߼��������ջ��С
#define USARTHANDLE_TASK_STK_SIZE				TASK_STK_SIZE_256//�߼��������ջ��С

#define RFIDMNG_UPDATE_TASK_STK_SIZE		TASK_STK_SIZE_512//�߼��������ջ��С

#ifdef HARDWARE_TEST
	#define	HARAWARE_TEST_STK_SIZE			TASK_STK_SIZE_256//�߼��������ջ��С
#endif

#endif
