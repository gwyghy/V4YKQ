/********************************************************************************
* �ļ����ƣ�	key.h
* ��	�ߣ�	������   
* ��ǰ�汾��   	V1.0
* ������ڣ�    2015.04.24
* ��������: 	����key.hͷ�ļ�
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
#ifndef __KEY_H__
#define __KEY_H__

/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/
#include	"includes.h"

/********************************************************************************
* #define�궨��
*********************************************************************************/
#define	SCANKEY_TASK_TIME_SET	10//����ִ������

// �����¼����Ͷ���
enum
{
	KEY_UP,
	KEY_DOWN ,	
	KEY_LONGDOWN
};

// ����ֵ�ṹ
typedef struct
{
	u32	u32Key;	// �������ɨ��ֵ
	u32	u32Status;	// ����״̬��00-�ͷţ�01-���£�02-��������
}stKEY;

/********************************************************************************
* ��������
*********************************************************************************/
/**������ֵ����(ʵ�ʴ��ڵİ���)**/


/*************************************************B��**********D��***********************/
#define	KEY_11				(u32)0x00000001			
#define	KEY_12				(u32)0x00000002                   //9		9
#define	KEY_13				(u32)0x00000004     //N��         //c		a
#define	KEY_14				(u32)0x00000008		//Q��         //d		c
#define	KEY_15				(u32)0x00000010		//7           //7		1

#define	KEY_21				(u32)0x00000020		//8           //8		2
#define	KEY_22				(u32)0x00000040	    //1	          //1		b
#define	KEY_23				(u32)0x00000080	    //2           //2		d
#define	KEY_24				(u32)0x00000100	    //9           //9		7
#define	KEY_25				(u32)0x00000200	    //0           //0		8

#define	KEY_31				(u32)0x00000400		//3           //3		6
#define	KEY_32				(u32)0x00000800		//4           //4		5
#define	KEY_33				(u32)0x00001000		//-           //+		+
#define	KEY_34				(u32)0x00002000	    //��          //-		-
#define	KEY_35				(u32)0x00004000	    //5           //5		3

#define	KEY_41				(u32)0x00008000		//6           //6		4
#define	KEY_42				(u32)0x00010000		//�����      //N
#define	KEY_43				(u32)0x00020000		//�ҳ���      //Q
#define	KEY_44				(u32)0x00040000	                  //0		0
#define	KEY_45				(u32)0x00080000     

#define	KEY_51				(u32)0x00100000	 	//����
#define	KEY_52				(u32)0x00200000	 	//ȷ��	
#define	KEY_53				(u32)0x00400000		//a�� zzw2023.12.24
#define	KEY_54				(u32)0x00800000		//b�� zzw2023.12.24
#define	KEY_55				(u32)0x01000000  	//����

/**���Ա���**/
#define	KEY_61				(u32)0x02000000//Ӳ���в�����	
#define	KEY_62				(u32)0x04000000//Ӳ���в�����	
#define	KEY_63				(u32)0x08000000//Ӳ���в�����	
#define	KEY_64				(u32)0x10000000//Ӳ���в�����	
#define	KEY_65				(u32)0x20000000//�������ü�ֵʹ��

/**���ⶨ�尴��**/
#define	KEY_71				(u32)0x40000000//Ĭ��Ϊ���ּ�
#define	KEY_72				(u32)0x80000000//Ĭ��Ϊ���ػ���

#define	KEY_INVALID			(u32)0xFFFFFFFF//��Ч����

#define ACT_KEY_NUMBER_1        KEY_13
#define ACT_KEY_NUMBER_2        KEY_14
#define ACT_KEY_NUMBER_3        KEY_22
#define ACT_KEY_NUMBER_4        KEY_23
#define ACT_KEY_NUMBER_5		KEY_31
#define ACT_KEY_NUMBER_6		KEY_32
#define ACT_KEY_NUMBER_7		KEY_35
#define ACT_KEY_NUMBER_8		KEY_41
#define ACT_KEY_NUMBER_9		KEY_15
#define ACT_KEY_NUMBER_10		KEY_21
#define ACT_KEY_NUMBER_11		KEY_24
#define ACT_KEY_NUMBER_12		KEY_25
#define ACT_KEY_NUMBER_13		(KEY_13|KEY_MULRIPLE_VALUE)
#define ACT_KEY_NUMBER_14		(KEY_14|KEY_MULRIPLE_VALUE)
#define ACT_KEY_NUMBER_15		(KEY_22|KEY_MULRIPLE_VALUE)
#define ACT_KEY_NUMBER_16		(KEY_23|KEY_MULRIPLE_VALUE)
#define ACT_KEY_NUMBER_17		(KEY_31|KEY_MULRIPLE_VALUE)
#define ACT_KEY_NUMBER_18		(KEY_32|KEY_MULRIPLE_VALUE)
#define ACT_KEY_NUMBER_19		(KEY_35|KEY_MULRIPLE_VALUE)
#define ACT_KEY_NUMBER_20		(KEY_41|KEY_MULRIPLE_VALUE)
#define ACT_KEY_NUMBER_21		(KEY_15|KEY_MULRIPLE_VALUE)
#define ACT_KEY_NUMBER_22		(KEY_21|KEY_MULRIPLE_VALUE)
#define ACT_KEY_NUMBER_23		(KEY_24|KEY_MULRIPLE_VALUE)
#define ACT_KEY_NUMBER_24		(KEY_25|KEY_MULRIPLE_VALUE)


/********************************************************************************
* ȫ�ֱ�������
*********************************************************************************/

/********************************************************************************
* ��������
*********************************************************************************/
extern OS_EVENT	 *g_stKeySem;//�����¼��ź���

u32 KeyInit(void);
#if (YKQ_HARDWARE_KEYLED_EXIST_DEFINE != DISABLED)
u32 KeyCorrespendLed(u32 u32KeyVal,u32 *pLedVal);
#endif
u32 AcceptKey(u32 u32KeyVal, u32 u32Info);

void KeyScanProc(void);
//�Ӱ�������ȡ��
u32 GetKey(stKEY *stKey);
//�Ӱ������в�ѯ���������Ӱ������������
u16 InquireKey(stKEY *stKey);
//ȡ�������м���
u16 GetKeyNum(void);
//���GetKeyNum��������
void KeyEmpty(void);

#endif

