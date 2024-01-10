/********************************************************************************
* �ļ�����	oled.c
* ���ߣ�	������   
* �汾��   	V1.0
* ���ڣ�    2015.04.24
* ��������:  ʵ��OLED������

* �޸�˵����   
*
*       >>>>  �ڹ����е�λ��  <<<<
*             3-Ӧ�ò�
*             2-Э���
*         ��   1-Ӳ��������
*********************************************************************************
* @copy
* <h2><center>&copy; COPYRIGHT ������������޹�˾ �з����� �����</center></h2>
*********************************************************************************/
/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/
#include "includes.h"

#include "const.h"

/********************************************************************************
* #define�궨��
*********************************************************************************/
/* task stack size */ 
#define SUCCESS 0
#define ERROR_1 1
#define ERROR_2 2
#define ERROR_3 3
#define ERROR_4 4
#define ERROR_5 5
#define ERROR_6 6
#define ERROR_7 7
#define ERROR_8 8
#define ERROR_9 9
#define ERROR_10 10

#define ERROR_UP_OVERFLOW     ERROR_1
#define ERROR_DOWN_OVERFLOW   ERROR_2
#define ERROR_OUTOFFRAME      ERROR_3
#define ERROR_OVERTIME		  ERROR_4

#define PAR_MAX_BODER 384

/********************************************************************************
* ��������
*********************************************************************************/

/********************************************************************************
* ��������
*********************************************************************************/
/********************************************************************************
* ��������
*********************************************************************************/
/*********************************************************
** ��������OLED_Send_Data
** �䡡�룺dt --- oled data
** �䡡����None
** ����������Send a config command to OLED
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.27
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
void OledSendData(uint8_t dt)
{
	u16 u16Temp = 0x00;

	u16Temp = (((u16)dt) << 1);
	
	OLED_DATA_PORT->ODR = (OLED_DATA_PORT->IDR & 0xFE01) | (u16Temp & 0x1FE);
}

/*********************************************************
** ��������OLED_Write_Command
** �䡡�룺cm --- oled command
** �䡡����None
** ����������Send a config command to OLED
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.27
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
void OledWriteCommand(uint8_t cm)
{
	GPIO_ResetBits(OLED_DC_PORT, OLED_DC_PIN);
	GPIO_ResetBits(OLED_CS_PORT, OLED_CS_PIN);
	GPIO_ResetBits(OLED_WR_PORT, OLED_WR_PIN);

	OledSendData(cm);

	GPIO_SetBits(OLED_WR_PORT, OLED_WR_PIN);
	GPIO_SetBits(OLED_CS_PORT, OLED_CS_PIN);
	GPIO_SetBits(OLED_DC_PORT, OLED_DC_PIN);
}

/*********************************************************
** ��������OLED_Write_Data
** �䡡�룺dt --- oled data
** �䡡����None
** ����������Send data to OLED
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.27
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
u32 OledWriteData(uint8_t dt)
{
	GPIO_SetBits(OLED_DC_PORT, OLED_DC_PIN);
	GPIO_ResetBits(OLED_CS_PORT, OLED_CS_PIN);
	GPIO_ResetBits(OLED_WR_PORT, OLED_WR_PIN);

	OledSendData(dt);

	GPIO_SetBits(OLED_WR_PORT, OLED_WR_PIN);
	GPIO_SetBits(OLED_CS_PORT, OLED_CS_PIN);
	GPIO_SetBits(OLED_DC_PORT, OLED_DC_PIN);

	return 0x01;
}

/*********************************************************
** ��������OLED_Contrast_ADJ
** �䡡�룺dt --- contrast
** �䡡����None
** ����������Send data to OLED
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.27
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
u32 OledContrastAdj(uint8_t dt)
{
	OledWriteCommand(0x81);  //contrast: 159					//���ͶԱȶ�������
	OledWriteCommand(dt);  //-----------------------			//���ͶԱȶȲ���

	return 0x01;
}

/***********************************************************************************************
** �� �� ����	DelayUs()
** ����������	��ʱ������΢�뼶
** �䡡  �룺	
** �䡡  ����	
** ����  �ߣ�	������
** �ա�  �ڣ�	2016.09.21
** ��    ����	V1.0.0
** ���¼�¼��
** ���¼�¼��
** 					��    ��      ��    ��                    ��      ��
** 					==========  =============  ========================================
**
************************************************************************************************/
static void DelayUs(u32 u32Counter)
{
	u32 u32i;
	u32 u32j;
	u32 uSLoops;
	RCC_ClocksTypeDef  rcc_clocks; 

	RCC_GetClocksFreq(&rcc_clocks);   //���ñ�׼�⺯������ȡϵͳʱ�ӡ�
	uSLoops = rcc_clocks.HCLK_Frequency/1000000;
	for (u32i = 0; u32i < u32Counter; u32i++)
	{
		u32j = 0;
		while(u32j++<uSLoops);
	}
}

/*********************************************************
** ��������OLED_LowLevel_Init
** �䡡�룺None
** �䡡����None
** ����������Initialize the OLED's port.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.27
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
u32 OledLowLevelInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* GPIOD Periph clock enable */
	RCC_AHBPeriphClockCmd(OLED_DATA_CLK | OLED_CS_CLK | OLED_DC_CLK | OLED_WR_CLK | OLED_RD_CLK | OLED_RT_CLK, ENABLE);		   //ʹ��OLEDʹ�ö˿�ʱ��

    /* Configure PD0 and PD2 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = OLED_DATA_PINS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(OLED_DATA_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = OLED_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(OLED_CS_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = OLED_DC_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(OLED_DC_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = OLED_WR_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(OLED_WR_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = OLED_RD_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(OLED_RD_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = OLED_RT_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(OLED_RT_PORT, &GPIO_InitStructure);

	GPIO_SetBits(OLED_CS_PORT, OLED_CS_PIN);
	GPIO_SetBits(OLED_DC_PORT, OLED_DC_PIN);
	GPIO_SetBits(OLED_WR_PORT, OLED_WR_PIN);
	GPIO_SetBits(OLED_RD_PORT, OLED_RD_PIN);
	GPIO_SetBits(OLED_RT_PORT, OLED_RT_PIN);

	GPIO_ResetBits(OLED_RT_PORT, OLED_RT_PIN);							   //��λOLED

	DelayUs(10000);//�������ʱ

	GPIO_SetBits(OLED_RT_PORT, OLED_RT_PIN);

	return 0x01;
}

/*********************************************************
** ��������OLED_Init
** �䡡�룺None
** �䡡����None
** ����������Initialize the OLED's fuction.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.27
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
u32 OledInit(void)
{
	uint8_t i,j;

	OledLowLevelInit();

	OledWriteCommand(0xae);/* set display off */

	OledWriteCommand(0x00);/* set lower column start address */
	OledWriteCommand(0x10);/* set higher column start address */

if(g_u32ID != B_type)	
	OledWriteCommand(0xa8); 
else
	OledWriteCommand(0x40);/* set display start line */ 
	OledWriteCommand(0x2e);  
	
	OledWriteCommand(0x81);/* set contrast control */
	OledWriteCommand(0x32);  
	OledWriteCommand(0x82);  
	OledWriteCommand(0x80); 
	
	
if(g_u32ID != B_type)
{
	OledWriteCommand(0x21);
	OledWriteCommand(3);
	OledWriteCommand(131);
//	OledWriteCommand(0);
//	OledWriteCommand(127); 
}
	
if(g_u32ID != B_type)	
	OledWriteCommand(0xa0);/* set segment remap */
else	
	OledWriteCommand(0xa1);/* set segment remap */ 
	
	OledWriteCommand(0xa6);/* set normal display */
	OledWriteCommand(0xa8);/* set multiplex ratio */
	OledWriteCommand(0x3f);/* 1/64 */
	OledWriteCommand(0xad);/* master configuration */
	OledWriteCommand(0x8e);/* external vcc supply */
	
if(g_u32ID != B_type)	
	OledWriteCommand(0xc0);/* set segment remap */
else	
	OledWriteCommand(0xc8);/* set com scan direction */

	OledWriteCommand(0xd3);/* set display offset */
	OledWriteCommand(0x40); 
	OledWriteCommand(0xd5);/* set display clock divide/oscillator frequency */
	OledWriteCommand(0xf0);

	OledWriteCommand(0xd8);/*set area color mode off */
	OledWriteCommand(0x05);
	OledWriteCommand(0xd9);
	OledWriteCommand(0xf1); 
	OledWriteCommand(0xda);/* set com pin configuartion */ 

	OledWriteCommand(0x12);
	OledWriteCommand(0x91); 
	OledWriteCommand(0x3f); 
	OledWriteCommand(0x3f);
	OledWriteCommand(0x3f);  

	OledWriteCommand(0x3f); 
	OledWriteCommand(0xaf);/* set display on */
	
	for(i=0;i<8;i++)
	{
		OledWriteCommand(0xb0|i);
		OledWriteCommand(0x00);
		OledWriteCommand(0x10);
		
		for(j=0;j<132;j++)
		{
			OledWriteData(0x00);
		}
	}

	return 0x01;
}

/***************************************************************************************
** ��������:OledShutDown
** ����������Oled�ػ�ʱ�Ĵ���
** �������룺
** ���������
** ע�����
******************************************************************************************/
u32 OledShutDown(void)
{
	OledWriteCommand(0xae);  //turn off lcd
	
	return 0x01;
}

/***************************************************************************************
** ��������:OledShutDown
** ����������Oled����ʱ�Ĵ���
** �������룺
** ���������
** ע�����
******************************************************************************************/
u32 OledShutOn(void)
{
	OledWriteCommand(0xaf);  //turn on lcd
	
	return 0x01;
}

/*********************************************************
** ��������	 LcdClr
** �䡡�룺  uint16_t u16Inv   ---- ����0��/����1���ֱ�־								   
** �䡡����  ��
** ���������������ӳ���
** ������:	 ��΢��
** �ա��ڣ�  2013.03.06
** �汾��    V1.0A
************************************************************/
u32 LcdClr(uint16_t u16Inv)
{
	uint16_t i = 0x00,j = 0x00,cr = 0x00;
	
	cr=((u16Inv==OLED_DISPLAY_NORMAL)?0x00:0xff);
	for(i=0;i<8;i++)
	{
		OledWriteCommand(0xb0|i);				  //����ҳ��ַ
		OledWriteCommand(0x00);					  //����ƫ����
		OledWriteCommand(0x10);
		
		for(j=0;j<132;j++)
		{
			OledWriteData(cr);				 //����
		}
	}

	return 0x01;
}

/*********************************************************
** ��������	 LcdAreaClr
** �䡡�룺  uint16_t u16Inv   ---- ����0��/����1���ֱ�־
**           uint16_t u16Top   ---- ��ʼҳ��ַ
**           uint16_t u16Left  ----	��ʼƫ����
**           uint16_t u16Bottom---- ����ҳ��ַ 
**           uint16_t u16Right ---- ����ƫ����								   
** �䡡����  ��
** �������������������ӳ���
** ������:	 ��΢��
** �ա��ڣ�  2013.03.06
** �汾��    V1.0A
************************************************************/
u32 LcdAreaClr(uint16_t u16Inv, uint16_t u16Top, uint16_t u16Left, uint16_t u16Bottom, uint16_t u16Right)
{
	uint16_t i,j,cr;
	
	cr=((u16Inv==OLED_DISPLAY_NORMAL)?0x00:0xff);
	
	for(i=u16Top;i<=u16Bottom;i++)
	{
		OledWriteCommand(0xb0|i);						//����ҳ��ַ
		OledWriteCommand(0x00+((u16Left)&0x000f));	//����ƫ����
		OledWriteCommand(0x10+((u16Left)>>4));
		
		if(g_u32ID == B_type)
		{
			for(j=u16Left+2;j<=(u16Right+2);j++)
			{
				OledWriteData(cr);						//����
			}
		}
		else
		{
			for(j=u16Left+2;j<=(u16Right+6);j++)
			{
				OledWriteData(cr);						//����
			}
		}
	}
	return 0x01;
}

/*********************************************************
** ��������Display_Character
** �䡡�룺no --- where is number 
**         site --- where to display
** �䡡����None
** ����������Display a certain character on the fixed place on pannel.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.27
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
#if (OLED_ZIKU_USEFLAG	==	OLED_USE_EXFLASH_ZIKU)
	void OledDisplayCharacter(uint16_t no,uint8_t site)
	{
		uint8_t pg=no/2000;
		uint16_t ad=(no%2000)*32;
		uint8_t count;
		uint8_t schar[32];//={0x0,0x0,0x82,0xff,0x80,0x0,0x0,0x0,0,0,0,0,0,0,0,0,0x0,0x0,0x00,0x0,0x00,0,0,0,0,0,0,0,0,0,0,0};
		
		W25QXX_ReadBuffer(schar, (pg<<16)+ad, 32);

		pg=(site/8)*2;
		ad=site%8;
		OledWriteCommand(0xb0|pg);
		OledWriteCommand(0x00+2);
		OledWriteCommand(0x10+ad);
		for(count=0;count<16;count++)
		{
			OledWriteCommand(schar[count]);				
		}
		
		OledWriteCommand(0xb0|(pg+1));
		OledWriteCommand(0x00+2);
		OledWriteCommand(0x10+ad);
		for(count=0;count<16;count++)
		{
			OledWriteCommand(schar[16+count]);
		}
		
		return 0x01;
	}
#endif
/*********************************************************
** ��������	 ShowChar_HZ16b
** �䡡�룺  uint16_t u16Inv   ---- ����0��/����1���ֱ�־
**           const uint16_t u16Val  ----	��ʾ�ַ�
**			 u16 u16Row        ---- ��ʼҳ��ַ
**			 u16 u16Col        ----	��ʼƫ����								   
** �䡡����  ��
** ������������ʾ�����ַ��ӳ���
** ������:	 ��΢��
** �ա��ڣ�  2013.03.06
** �汾��    V1.0A
************************************************************/
#if 1		//zzw
u32 ShowChar_HZ16b(uint16_t u16Inv, const uint16_t u16Val, uint16_t u16Row, uint16_t u16Col)
{
	uint8_t pg;
	u32 ad;
	uint8_t count;
	uint8_t schar[32];//={0x0,0x0,0x82,0xff,0x80,0x0,0x0,0x0,0,0,0,0,0,0,0,0,0x0,0x0,0x00,0x0,0x00,0,0,0,0,0,0,0,0,0,0,0};
	uint16_t tmp;
//	INT8U err;
	
	tmp = ((u16Val>>8)-0xb0)*96 + (u16Val&0x00ff)-0xa0;
// 	pg = tmp/1920;
	ad = ZIKU_BASH_ADDR + tmp*32;

	//ֻ���������ֲ���
	if((u16Val>>8)<0xB0) 
		return ERROR_UP_OVERFLOW;			  
	if((u16Val&0x00ff)<0xa0) 
		return ERROR_DOWN_OVERFLOW;
	if((u16Val>>8)>0xD7) 
		return ERROR_UP_OVERFLOW;
	if(u16Row>6) 
		return ERROR_OUTOFFRAME;
	if(u16Col>112) 
		return ERROR_OUTOFFRAME;
	
	//�ȴ��ź���
//	OSMutexPend(mutex_w25qxx_wr, 0, &err);//�ȴ��ź���
	IapReadBuf(schar, ad, 32);				 //��ȡ�ֿ�����
//	OSMutexPost(mutex_w25qxx_wr);//�����ź���
	
	pg=u16Row;
	ad=u16Col + 4;			//zzw
	OledWriteCommand(0xb0|pg);							 //����ҳ��ַ
	OledWriteCommand(0x00+ad&0x0f);						 //����ƫ����
	OledWriteCommand(0x10+(ad>>4));
	for(count=0;count<16;count++)
	{
		OledWriteData((u16Inv==OLED_DISPLAY_NORMAL)?schar[count]:~schar[count]);	//д�����ֻ���			
	}
	
	OledWriteCommand(0xb0|(pg+1));						 //����ҳ��ַ
	OledWriteCommand(0x00+ad&0x0f);						 //����ƫ����
	OledWriteCommand(0x10+(ad>>4));
	for(count=0;count<16;count++)
	{
		OledWriteData((u16Inv==OLED_DISPLAY_NORMAL)?schar[16+count]:~schar[16+count]);   //д�����ֻ���
	}
	
	return SUCCESS;
}
#endif
/*********************************************************
** ��������	 ShowChar_ASCII16b
** �䡡�룺  uint16_t u16Inv   ---- ����0��/����1���ֱ�־
**           const uint16_t u16Val  ----	��ʾ�ַ�
**			 u16 u16Row        ---- ��ʼҳ��ַ
**			 u16 u16Col        ----	��ʼƫ����								   
** �䡡����  ��
** ������������ʾASCII�ַ��ӳ���
** ������:	 ��΢��
** �ա��ڣ�  2013.03.06
** �汾��    V1.0A
************************************************************/
#if (OLED_ZIKU_USEFLAG	==	OLED_USE_EXFLASH_ZIKU)
u32 ShowCharAscii16b(uint16_t u16Inv, const uint16_t u16Val, uint16_t u16Row, uint16_t u16Col)
{
	uint8_t pg;
	uint16_t ad;
	uint8_t count;
	uint8_t schar[16];//={0x0,0x0,0x82,0xff,0x80,0x0,0x0,0x0,0,0,0,0,0,0,0,0,0x0,0x0,0x00,0x0,0x00,0,0,0,0,0,0,0,0,0,0,0};
	uint16_t tmp;
	INT8U err;
	tmp = u16Val-0x20;
	pg = 5;
	ad = tmp*16;

	if(u16Val>0x83) 
		return ERROR_UP_OVERFLOW;
	if(u16Row>6) 
		return ERROR_OUTOFFRAME;
	if(u16Col>120) 
		return ERROR_OUTOFFRAME;

	//�ȴ��ź���
	OSMutexPend(mutex_w25qxx_wr, 0, &err);//�ȴ��ź���
	W25QXX_ReadBuffer(schar, (pg<<16)+ad, 16);			  //��ȡ�ֿ�����
	OSMutexPost(mutex_w25qxx_wr);//�����ź���
	
	pg=u16Row;
	ad=u16Col;
	OledWriteCommand(0xb0|pg);						  //����ҳ��ַ
	OledWriteCommand(0x00+ad&0x0f);					  //����ƫ����
	OledWriteCommand(0x10+(ad>>4));
	for(count=0;count<8;count++)
	{
		OledWriteData((u16Inv==OLED_DISPLAY_NORMAL)?schar[count]:~schar[count]);	//д�����ֻ���			
	}
	OledWriteCommand(0xb0|(pg+1));					  //����ҳ��ַ
	OledWriteCommand(0x00+ad&0x0f);					  //����ƫ����
	OledWriteCommand(0x10+(ad>>4));
	for(count=0;count<8;count++)
	{
		OledWriteData((u16Inv==OLED_DISPLAY_NORMAL)?schar[8+count]:~schar[8+count]);
	}
	
	return SUCCESS;
}
#endif

/***************************************************************************************
** ��������:ShowStr_16b
** ������������ʾ�ַ���
** �������룺cu8Str:�����ַ����׵�ַ�� u16Row������ҳ��u16Col��������
** ���������None
** ע�����
******************************************************************************************/
#if (OLED_ZIKU_USEFLAG	==	OLED_USE_EXFLASH_ZIKU)
u32 ShowStr16b(uint16_t u16Inv, const u8 *cu8Str,uint8_t len, u16 u16Row, u16 u16Col)
{
	uint8_t i,p;
	
	for(i=0,p=0;i<len;i++)
	{
		if(*(cu8Str+p)>0xa0) 
		{
			ShowChar_HZ16b(u16Inv,(*(cu8Str+p+1))+((*(cu8Str+p))<<8), u16Row, u16Col+(8*p));
			p+=2;
		}
		else
		{
			ShowChar_ASCII16b(u16Inv,*(cu8Str+p), u16Row, u16Col+(8*p));
			p++;
		}
	}

	return 0x01;
}
#endif

/*********************************************************
** ��������	 ShowStr_16b
** �䡡�룺  uint16_t u16Inv   ---- ����0��/����1���ֱ�־
**           const u8 *cu8Str  ----	��ʾ�ַ���
**			 uint8_t len       ----	����
**			 u16 u16Row        ---- ��ʼҳ��ַ
**			 u16 u16Col        ----	��ʼƫ����							   
** �䡡����  ��
** ������������ʾ�ַ����ӳ���
** ������:	 ��΢��
** �ա��ڣ�  2013.03.06
** �汾��    V1.0A
** ������:	 �ﶫ��
** �ա��ڣ�  2014.04.15
** �汾��    V1.2
** �������������ӳ����ַ���������ʾ���ܣ�������ʾ�س����У������ַ�����ʾ��Ϻ������λ�á�
************************************************************/
#if 1		//zzw
	u32 ShowStr16b(uint16_t u16Inv, const u8 *cu8Str,u16 u16Row, u16 u16Col)
	{
		uint16_t i;
		uint16_t hanzi = 0;			//=1:�����ַ���
		uint16_t lenx = strlen((const char *)cu8Str);
		uint16_t u16Rowx = u16Row;
		uint16_t u16Colx = u16Col;

		hanzi = 0;
		for (i = 0; i < lenx; i++)
		{
			if (*(cu8Str+i) > 0xa0)					//�ж��Ǻ��ֻ���ASCII��
			{
				hanzi = 1;
				break;
			}
		}
		
		for (i = 0; i < lenx; i++)
		{
			if (*(cu8Str+i) > 0xa0)					//�ж��Ǻ��ֻ���ASCII��
			{
				if (u16Colx > 112)					//��ʾ��������
				{
					u16Colx = 0;
					u16Rowx += 2;					//������Ҫ2��
				}
				if (u16Rowx > 6)					//�ַ�����������Ļ��ʾ����
					break;
					
				ShowChar_HZ16b(u16Inv, (*(cu8Str+i+1))+((*(cu8Str+i))<<8), u16Rowx, u16Colx);
				u16Colx += 16;
				i++;								//����2�ֽ�
			}
			else
			{
				if ((*(cu8Str+i) == 0x0a) || (*(cu8Str+i) == 0x0d))	//�ж��Ƿ�س�����
				{
					u16Colx = 0;
					u16Rowx++;
					if (hanzi == 1)
						u16Rowx++;					//������Ҫ2��
				}
				else
				{
					if (u16Colx > 120)				//��ʾ��������
					{
						u16Colx = 0;
						u16Rowx++;
						if (hanzi == 1)
							u16Rowx++;				//������Ҫ2��
					}
					if (hanzi == 1)					//�Ƿ����ַ���
					{
						if (u16Rowx > 6)			//�ַ�����������Ļ��ʾ����
							break;
					}
					else
					{
						if (u16Rowx > 7)			//�ַ�����������Ļ��ʾ����
							break;
					}
					
					ShowChar_ASCII16b(u16Inv,*(cu8Str+i), u16Rowx, u16Colx);  //��ʾASCII��
					u16Colx += 8;
				}
			}
		}
		return (((u32)(u16Colx) << 16) + u16Rowx);
	}
#endif

/*********************************************************
** �������ƣ�OLED_Display1
** ����������OLED��ʾ16*16���ַ�(��Ҫ�����������ַ�)
** �������룺u16Inv:����0��/����1���ֱ�־,��ʾ���ַ�Number����ʾ��ҳpage����ʾ����column
** ���������1��������0���쳣
** ע�����
************************************************************/
#if (OLED_ZIKU_USEFLAG	==	OLED_USE_INFLASH_ZIKU)
u32  ShowChar_Special(uint16_t u16Inv, uint8_t Number,uint16_t u16Row,uint16_t u16column)	 //��ʾ������
{

	uint16_t ad,count;
	
	if(Number >= (sizeof(SpecialSymbol)/32))
		return 0x00;
	
	ad=Number*32;
	if(g_u32ID != B_type)
		u16column = u16column + 4;	//��Ļ�ֱ���Ϊ132*64����Ҫ��ַƫ������֤��ʾ��������ʾ������
	
	//oled��ʾ
	OledWriteCommand(0xb0|u16Row);                       //oledдҳ����
	OledWriteCommand(0x00+(u16column&0x0F));              //oledд������
	OledWriteCommand(0x10+((u16column&0xF0)>>4));
	for (count=0;count<16;count++)
	{  
		OledWriteData(SpecialSymbol[ad+count]);
	}

	OledWriteCommand(0xb0|(u16Row+1));
	OledWriteCommand(0x00+(u16column&0x0F));
	OledWriteCommand(0x10+((u16column&0xF0)>>4));
	for (count=0;count<16;count++)
	{  
		OledWriteData(SpecialSymbol[ad+count+16]&(0xff>>(u16Row/6)));//ȡ�����һҳ��һ��
	}
	
	return 0x01;
	
}
#endif
/*********************************************************
** ��������	 ShowChar_ASCII16b
** �䡡�룺  uint16_t u16Inv   ---- ����0��/����1���ֱ�־
**           const uint16_t u16Val  ----	��ʾ�ַ�
**			 u16 u16Row        ---- ��ʼҳ��ַ
**			 u16 u16Col        ----	��ʼƫ����								   
** �䡡����  ��
** ������������ʾASCII�ַ��ӳ���
** ������:	 ��΢��
** �ա��ڣ�  2013.03.06
** �汾��    V1.0A
************************************************************/
#if (OLED_ZIKU_USEFLAG	==	OLED_USE_INFLASH_ZIKU)
uint32_t ShowChar_ASCII16b(uint16_t u16Inv, const uint16_t u16Val, uint16_t u16Row, uint16_t u16Column)
{
	uint8_t count;
	uint16_t tmp;
	//INT8U err;
	
	tmp = u16Val-0x20;

	if(u16Val>0x83) 
		return ERROR_UP_OVERFLOW;
	if(u16Row>6) 
		return ERROR_OUTOFFRAME;
	if(u16Column>120) 
		return ERROR_OUTOFFRAME;

	if(g_u32ID != B_type)
		u16Column = u16Column + 4;	//��Ļ�ֱ���Ϊ132*64����Ҫ��ַƫ������֤��ʾ��������ʾ������
	
	OledWriteCommand(0xb0|u16Row);						  //����ҳ��ַ
	OledWriteCommand(0x00+((u16Column)&0x0F));					  //����ƫ����
	OledWriteCommand(0x10+(((u16Column)&0xF0)>>4));
	for(count=0;count<8;count++)
	{
		OledWriteData((u16Inv==OLED_DISPLAY_NORMAL)?ASCII[tmp][count]:~ASCII[tmp][count]);	//д�����ֻ���			
	}
	
	OledWriteCommand(0xb0|(u16Row+1));						//����ҳ��ַ
	OledWriteCommand(0x00+((u16Column)&0x0F));					//����ƫ����
	OledWriteCommand(0x10+(((u16Column)&0xF0)>>4));
	for(count=0;count<8;count++)
	{
		OledWriteData((u16Inv==OLED_DISPLAY_NORMAL)?ASCII[tmp][8+count]:~ASCII[tmp][8+count]);
	}
	
	return SUCCESS;
}
#endif
/***************************************************************************************
** ��������: seek_word(uint16_t number)
** ���������������ַ�
** �������룺�����ַ�����
** ���������1��������0���쳣
** ע�����
******************************************************************************************/
#if 0			//zzw
 u32   seek_word(uint16_t number, uint16_t *pId)
{
	uint16_t head,tail;
	uint32_t u32ReturnValue = 0x00;
	
	head=0;
	tail=(sizeof codeGB_16/sizeof(typFNT_GB16));

	while(head < tail)
	{	  
		if(number==codeGB_16[head].Index[0]*256+codeGB_16[head].Index[1])
		{
			*pId = head;
			u32ReturnValue = 0x01;
			break;
		}
		head ++;
	}	
	return  u32ReturnValue;
}

/***************************************************************************************
** ��������:ShowStr_16b(uint8_t fashion,const u8 *cu8Str,uint16_t u16Row, uint16_t u16Col)
** ������������ʾ������
** �������룺u16Val����ʾ�ַ���ţ� u16Row������ҳ��u16Col��������
** ���������1��������0���쳣
** ע�����
******************************************************************************************/
u32  OledDisplayZ(uint16_t u16Inv, uint16_t u16Val,uint8_t page,uint8_t column)	 //��ʾ������
{
	uint16_t count;
	
  if(g_u32ID != B_type)
		column += 4;
	OledWriteCommand(0xb0|page);
	OledWriteCommand(0x00+((column)&0x0F));
	OledWriteCommand(0x10+(((column)&0xF0)>>4));
	for (count=0;count<16;count++)
	{  
		OledWriteData((u16Inv==OLED_DISPLAY_NORMAL)?codeGB_16[u16Val].Msk[count]:~codeGB_16[u16Val].Msk[count]);	//д�����ֻ���		
	}

	OledWriteCommand(0xb0|(page+1));
	OledWriteCommand(0x00+((column)&0x0F));
	OledWriteCommand(0x10+(((column)&0xF0)>>4));
	for (count=0;count<16;count++)
	{  
		OledWriteData((u16Inv==OLED_DISPLAY_NORMAL)?codeGB_16[u16Val].Msk[count+16]:~codeGB_16[u16Val].Msk[count+16]);
	}  
	return 0x01;
}
   
/***************************************************************************************
** ��������:ShowStr_16b
** ������������ʾ�ַ���
** �������룺cu8Str:�����ַ����׵�ַ�� u16Row������ҳ��u16Col��������
** ���������1��������0���쳣
** ע�����
******************************************************************************************/
u32 ShowStr16b(uint16_t u16Inv, const u8 *cu8Str,uint16_t u16Row, uint16_t u16Col)
{
   	uint16_t i,j;
	uint16_t lenx=strlen((const char *)cu8Str);
	uint16_t hanzi = 0;			//=1:�����ַ���
	uint16_t u16Rowx = u16Row;
	uint16_t u16Colx = u16Col;
	uint32_t u32Flag = 0x00;
	
	hanzi = 0;
	for (i = 0; i < lenx; i++)
	{
		if (*(cu8Str+i) > 0xa0)					//�ж��Ǻ��ֻ���ASCII��
		{
			hanzi = 1;
			break;
		}
	}
	
//	for(i=0;i<lenx;i++)
//	{
//		j=seek_word((*(cu8Str+i+1))+((*(cu8Str+i))<<8));
//		i++;
//		OledDisplayZ(u16Inv,j,u16Row,u16Col+(8*i));	
//	}
	
	for (i = 0; i < lenx; i++)
	{
		if (*(cu8Str+i) > 0xa0)					//�ж��Ǻ��ֻ���ASCII��
		{
			if (u16Colx > 112)					//��ʾ��������
			{
				u16Colx = 0;
				u16Rowx += 2;					//������Ҫ2��
			}
			if (u16Rowx > 6)					//�ַ�����������Ļ��ʾ����
				break;
			
			u32Flag = seek_word((*(cu8Str+i+1))+((*(cu8Str+i))<<8),&j);	
			if(u32Flag == 0x01)
				OledDisplayZ(u16Inv,j,u16Rowx,u16Colx);		
			u16Colx += 16;
			i++;
// 			ShowChar_HZ16b(u16Inv, (*(cu8Str+i+1))+((*(cu8Str+i))<<8), u16Rowx, u16Colx);
// 			u16Colx += 16;
// 			i++;								//����2�ֽ�
		}
		else
		{
			if ((*(cu8Str+i) == 0x0a) || (*(cu8Str+i) == 0x0d))	//�ж��Ƿ�س�����
			{
				u16Colx = 0;
				u16Rowx++;
				if (hanzi == 1)
					u16Rowx++;					//������Ҫ2��
			}
			else
			{
				if (u16Colx > 120)				//��ʾ��������
				{
					u16Colx = 0;
					u16Rowx++;
					if (hanzi == 1)
						u16Rowx++;				//������Ҫ2��
				}
				if (hanzi == 1)					//�Ƿ����ַ���
				{
					if (u16Rowx > 6)			//�ַ�����������Ļ��ʾ����
						break;
				}
				else
				{
					if (u16Rowx > 7)			//�ַ�����������Ļ��ʾ����
						break;
				}
				
				ShowChar_ASCII16b(u16Inv,*(cu8Str+i), u16Rowx, u16Colx);  //��ʾASCII��
				u16Colx += 8;
			}
		}
	}
	
	return 0x01;
}
#endif


