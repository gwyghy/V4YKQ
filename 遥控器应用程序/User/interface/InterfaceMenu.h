/********************************************************************************
* �ļ�����	InterfaceMenu.h
* ���ߣ�	������   
* �汾��   	V1.0
* ���ڣ�    2015.09.09  
* ��������: ����InterfaceMenu.hͷ�ļ�
* 			
*			
* �޸�˵����   
*
*       >>>>  �ڹ����е�λ��  <<<<
*             3-Ӧ�ò�
*         ��   2-Э���
*             1-Ӳ��������
*********************************************************************************
* @copy
* <h2><center>&copy; COPYRIGHT ������������޹�˾ �з����� �����</center></h2>
*********************************************************************************/
#ifndef  __INTERFACE_MENU_H__
#define  __INTERFACE_MENU_H__

/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/
#include "includes.h"


/********************************************************************************
* #define�궨��
*********************************************************************************/
#define 	KEY_MENU_0		KEY_25
#define 	KEY_MENU_1		KEY_22
#define		KEY_MENU_2		KEY_23
#define 	KEY_MENU_3		KEY_31
#define 	KEY_MENU_4		KEY_32
#define 	KEY_MENU_5		KEY_35
#define 	KEY_MENU_6		KEY_41
#define		KEY_MENU_7		KEY_15
#define 	KEY_MENU_8		KEY_21
#define 	KEY_MENU_9		KEY_24


#define		KEY_MENU_UP		KEY_34
#define 	KEY_MENU_DOWN	KEY_33

#define 	KEY_MENU_LEFT	KEY_42
#define		KEY_MENU_RIGHT	KEY_43		

#define		KEY_MENU_DEFINE	KEY_55

#define		KEY_MENU_N	  KEY_13
#define 	KEY_MENU_Q		KEY_14

#if (INCREASE_DECREASE_CTRL_ENABLE_FLAG	!= DISABLED)
	#define 	KEY_MENU_OK		KEY_52
#else 
	#define 	KEY_MENU_OK		KEY_52
#endif

#define	KEY_MENU_STOP		KEY_72	

/*�ṹ������*/
typedef struct
{
	u16 u16CharCnt;		//��ʾ������ַ���
	u16 u16BufInd;		//�����ַ����������ڲ�����������ʱ��=0��ʾ��ǰ�������ִ����ֵ��=1��ʾ��������˳����������ĸ�λ
	u16 u16VarType;		//0-integer; 1-float; 2-select; 3-command
	u16 u16Row;			//�����к�0-7
	u16 u16Col;			//������ʼ�к�0-127
	u16 u16VarMax;		//�������ֵ
	u16 u16VarMin;		//������Сֵ
	u16 u16GlobalPar;	//�������1-���壻2-���أ�3-����&����
	u16 u16MenuGrpNo;	//������Сֵ
	u16 u16MenuItemNo;	//�������1-���壻2-���أ�3-����&����
	const u8 *pSelItem;	//ѡ�����
	u16 u16InputVal;	//��ʾֵ
	u16 u16Offset;		//�ڲ�������ƫ�Ƶ�
}stEDIT_PAR;

/********************************************************************************
* ��������
*********************************************************************************/
//�˵������
enum
{
	MENU_YKQ_MACHINE_NO	=	0,			//��������

	#if (QUICK_KEY_CTRL_ENABLE_FLAG	!= DISABLED)
		#if (QUICK_KEY_1_ENABLE_FLAG != DISABLED)
			MENU_SHORTCUT_F1,					//��ݼ�1
		#endif
	
		#if (QUICK_KEY_2_ENABLE_FLAG != DISABLED)
			MENU_SHORTCUT_F2,					//��ݼ�2
		#endif

		#if (QUICK_KEY_3_ENABLE_FLAG != DISABLED)
			MENU_SHORTCUT_F3,					//��ݼ�2
		#endif
	
		#if (QUICK_KEY_4_ENABLE_FLAG != DISABLED)
			MENU_SHORTCUT_F4,					//��ݼ�2
		#endif	
	#endif

	#if (ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)
		MENU_ZIBAO_PARAM_SET,					//�Ա�����
	#endif
	
	MENU_DEFAULT_GRP_NO	,				//ȱʡ����

	#if (SET_YKQ_PRGPT_USE_FLAG != 0)
		MENU_OTHER_YKQ_PARAM_NO,	
	#endif
	
//	MENU_KEY_FILM_SELECT,		//ѡ�����Ĥ	2020.01.15 parry
	
//	MENU_KEY_ACTION_SELECT,	//�趨��������	2020.01.15 parry
	
	MENU_ID_MAX
};

//�˵�������
enum
{
	PAR_EDIT_INT 	=	0,		//��������
	PAR_EDIT_FLOAT	=	1,		//���븡����
	PAR_EDIT_SEL 	=	2,		//ѡ��������
	PAR_EDIT_NOTE	=	3,		//��ʾ��޲���
	PAR_EDIT_CMD	=	4,		//����˵���
	PAR_EDIT_TIME	=	5		//��������ʱ��
};

//�������ͷ���
enum
{
	PAR_GLOBAL	=			1,		//�������
	PAR_LOCAL	=			2,		//���ز���
	PAR_GLOBAL_LOCAL =		3		//����&���ز���
};

//����ֵת��Ϊ�ַ��������ڣ�
enum
{
	PAR_TO_STR_FOR_SHOW	 =	0,		//�˵�����ʾ
	PAR_TO_STR_FOR_EDIT	 =	1		//�༭�޸�����
};

//�����������ڣ�
enum
{
 	INPUT_INT_FOR_PWD =	1,			//��������
	 INPUT_INT_FOR_EDIT =	0,			//��������
};

//�˵���ʾ
enum
{
	MENU_SHOW_NOT_INIT	=	0,			//��ʾ�˵���������ʼ���˵���
	MENU_SHOW_AND_INIT	=	1			//��ʾ�˵���ͬʱ��ʼ���˵���
};

//����״̬����
enum
{
	INTERFACEMENU_STATE_NONE = 0x00,
	INTERFACEMENU_STATE_MENU_START,	
	INTERFACEMENU_STATE_EDIT_ININT,			
	INTERFACEMENU_STATE_EDIT_PWR,//��������	
	INTERFACEMENU_STATE_EDIT_PWR_OK,//��������		
	INTERFACEMENU_STATE_EDIT,
	INTERFACEMENU_STATE_EDIT_SEL_GLOBAL,
	INTERFACEMENU_STATE_EDIT_SEL_OK,
	INTERFACEMENU_STATE_MAX
};


//"ң��������";
enum 
{
	MENU_YKQ_YKQ_ID = 0x00,				//"ң�������",
	MENU_YKQ_SCR_BRIGHTNESS,			//"��������",
	MENU_YKQ_SCR_PROTECTED_BRIGHTNESS,	//"��������",	
	MENU_YKQ_SCR_PROTECTED_MUL,			//"�������",
	MENU_YKQ_SCR_AUTO_SHUTDOWN_TIMER,//"�Զ��ػ���ʱ"
	MENU_YKQ_BATTERY_VOLTAGE_1,
	MENU_YKQ_BATTERY_VOLTAGE_2,
};

/**��ݼ�����ö��**/
#if (QUICK_KEY_CTRL_ENABLE_FLAG	!= DISABLED)
	enum
	{
		MENU_QUICK_ENABLE = 0x00,
		
		#if (QUICK_CTRL_SUPPORT_ACTION_MAX >= 1)
			MENU_QUICK_ACTION_1,
			MENU_QUICK_ACTION_1_START,
			MENU_QUICK_ACTION_1_ING,
		#endif
		
		#if (QUICK_CTRL_SUPPORT_ACTION_MAX >= 2)
			MENU_QUICK_ACTION_2,
			MENU_QUICK_ACTION_2_START,
			MENU_QUICK_ACTION_2_ING,
		#endif
		
		#if (QUICK_CTRL_SUPPORT_ACTION_MAX >= 3)
			MENU_QUICK_ACTION_3,
			MENU_QUICK_ACTION_3_START,
			MENU_QUICK_ACTION_3_ING,
		#endif
		
		#if (QUICK_CTRL_SUPPORT_ACTION_MAX >= 4)
			MENU_QUICK_ACTION_4,
			MENU_QUICK_ACTION_4_START,
			MENU_QUICK_ACTION_4_ING,
		#endif
		
		#if (QUICK_CTRL_SUPPORT_ACTION_MAX >= 5)
			MENU_QUICK_ACTION_5,
			MENU_QUICK_ACTION_5_START,
			MENU_QUICK_ACTION_5_ING,	
		#endif
		
		#if (QUICK_CTRL_SUPPORT_ACTION_MAX >= 6)
			MENU_QUICK_ACTION_6,
			MENU_QUICK_ACTION_6_START,
			MENU_QUICK_ACTION_6_ING,	
		#endif
		
		#if (QUICK_CTRL_SUPPORT_ACTION_MAX >= 7)
			MENU_QUICK_ACTION_7,
			MENU_QUICK_ACTION_7_START,
			MENU_QUICK_ACTION_7_ING,
		#endif
		
		#if (QUICK_CTRL_SUPPORT_ACTION_MAX >= 8)
			MENU_QUICK_ACTION_8,
			MENU_QUICK_ACTION_8_START,
			MENU_QUICK_ACTION_8_ING,
		#endif
		
		#if (QUICK_CTRL_SUPPORT_ACTION_MAX >= 9)
			MENU_QUICK_ACTION_9,
			MENU_QUICK_ACTION_9_START,
			MENU_QUICK_ACTION_9_ING,
		#endif
		
		#if (QUICK_CTRL_SUPPORT_ACTION_MAX >= 10)
			MENU_QUICK_ACTION_10,
			MENU_QUICK_ACTION_10_START,
			MENU_QUICK_ACTION_10_ING,
		#endif
	};
#endif

//"�Ա�����";
#if (ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)
 	enum
	{
		MENU_ZIBAO_ENABLE = 0x00,//�Ա��Ƿ�ʹ��
		MENU_ZIBAO_ACTION_MAX,//��֧�ֵ��Ա����ܵ��������		

		#if (ZIBAO_CTRL_METHOD_TYPE	== ZIBAO_PARAM_CTRL_METHOD)
			MENU_ZIBAO_TIMEOUT,//�Ա���ʱʱ������
			#if (SHENG_ZHU_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHENGZHU_ENABLE_FLAG,
				MENU_ZIBAO_SHENGZHU_DOING_TIME,
			#endif		
			#if (JIANG_ZHU_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_JIANGZHU_ENABLE_FLAG,
				MENU_ZIBAO_JIANGZHU_DOING_TIME,
			#endif
			#if (TUILIU_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_TUILIU_ENABLE_FLAG,
				MENU_ZIBAO_TUILIU_DOING_TIME,
			#endif		
			#if (YIJIA_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_YIJIA_ENABLE_FLAG,
				MENU_ZIBAO_YIJIA_DOING_TIME,
			#endif
			#if (LAJIA_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_LAJIA_ENABLE_FLAG,
				MENU_ZIBAO_LAJIA_DOING_TIME,
			#endif
			#if (SHENG_QIANZHU_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHENGQIANZHU_FLAG,
				MENU_ZIBAO_SHENGQIANZHU_DOING_TIME,
			#endif
			#if (JIANG_QIANZHU_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_JIANGQIANZHU_ENABLE_FLAG,
				MENU_ZIBAO_JIANGQIANZHU_DOING_TIME,
			#endif
			#if (SHENG_QIANHOUZHU_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHENGQIANHOUZHU_ENABLE_FLAG,
				MENU_ZIBAO_SHENGQIANHOUZHU_DOING_TIME,
			#endif			
			#if (JIANG_QIANHOUZHU_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_JIANGQIANHOUZHU_ENABLE_FLAG,
				MENU_ZIBAO_JIANGQIANHOUZHU_DOING_TIME,
			#endif


			#if (SHEN_SHENSUOLIANG_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHENSHENSUOLIANG_ENABLE_FLAG,
				MENU_ZIBAO_SHENSHENSUOLIANG_DOING_TIME,
			#endif
			#if (SHOU_SHENSUOLIANG_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHOUSHENSUOLIANG_ENABLE_FLAG,
				MENU_ZIBAO_SHOUSHENSUOLIANG_DOING_TIME,
			#endif
			#if (SHEN_PINGHENG_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHENPINGHENG_ENABLE_FLAG,
				MENU_ZIBAO_SHENPINGHENG_DOING_TIME,
			#endif
			#if (SHOU_PINGHENG_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHOUPINGHENG_ENABLE_FLAG,
				MENU_ZIBAO_SHOUPINGHENG_DOING_TIME,
			#endif
			#if (SHEN_QIANCHABAN_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHENQIANCHABAN_ENABLE_FLAG,
				MENU_ZIBAO_SHENQIANCHABAN_DOING_TIME,
			#endif
			#if (SHOU_QIANCHABAN_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHOUQIANCHABAN_ENABLE_FLAG,
				MENU_ZIBAO_SHOUQIANCHABAN_DOING_TIME,
			#endif			


			#if (SHEN_HUBANG_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHENHUBANG_ENABLE_FLAG,
				MENU_ZIBAO_SHENHUBANG_DOING_TIME,
			#endif
			#if (SHOU_HUBANG_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHOUHUBANG_ENABLE_FLAG,
				MENU_ZIBAO_SHOUHUBANG_DOING_TIME,
			#endif
			#if (SHEN_QIANHUBANG_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHENQIANHUBANG_ENABLE_FLAG,
				MENU_ZIBAO_SHENQIANHUBANG_DOING_TIME,
			#endif
			#if (SHOU_QIANHUBANG_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHOUQIANHUBANG_ENABLE_FLAG,
				MENU_ZIBAO_SHOUQIANHUBANG_DOING_TIME,
			#endif
			#if (SHEN_CEHU_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHENCEHU_ENABLE_FLAG,
				MENU_ZIBAO_SHENCEHU_DOING_TIME,
			#endif
			#if (SHOU_CEHU_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHOUCEHU_ENABLE_FLAG,
				MENU_ZIBAO_SHOUCEHU_DOING_TIME,
			#endif
			#if (SHEN_CETUI_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHENCETUI_ENABLE_FLAG,
				MENU_ZIBAO_SHENCETUI_DOING_TIME,
			#endif
			#if (SHOU_CETUI_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHOUCETUI_ENABLE_FLAG,
				MENU_ZIBAO_SHOUCETUI_DOING_TIME,
			#endif


			#if (TAI_DIZUO_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_TAIDIZUO_ENABLE_FLAG,
				MENU_ZIBAO_TAIDIZUO_DOING_TIME,
			#endif
			#if (JIANG_DIZUO_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_JIANGDIZUO_ENABLE_FLAG,
				MENU_ZIBAO_JIANGDIZUO_DOING_TIME,
			#endif
			#if (SHEN_HUBANG2_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHENHUBANG2_ENABLE_FLAG,
				MENU_ZIBAO_SHENHUBANG2_DOING_TIME,
			#endif
			#if (SHOU_HUBANG2_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHOUHUBANG2_ENABLE_FLAG,
				MENU_ZIBAO_SHOUHUBANG2_DOING_TIME,
			#endif
			

			#if (WEILIANG_SHANGBAI_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_WEILIANGSHANGBAI_ENABLE_FLAG,
				MENU_ZIBAO_WEILIANGSHANGBAI_DOING_TIME,
			#endif
			#if (WEILIANG_XIABAI_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_WEILIANGXIABAI_ENABLE_FLAG,
				MENU_ZIBAO_WEILIANGXIABAI_DOING_TIME,
			#endif
			#if (SHEN_CHABAN_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHENCHABAN_ENABLE_FLAG,
				MENU_ZIBAO_SHENCHABAN_DOING_TIME,
			#endif
			#if (SHOU_CHABAN_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHOUCHABAN_ENABLE_FLAG,
				MENU_ZIBAO_SHOUCHABAN_DOING_TIME,
			#endif	


			#if (SHENG_HOUZHU_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHENGHOUZHU_ENABLE_FLAG,
				MENU_ZIBAO_SHENGHOUZHU_DOING_TIME,
			#endif	
			#if (JIANG_HOUZHU_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_JIANGHOUZHU_ENABLE_FLAG,
				MENU_ZIBAO_JIANGHOUZHU_DOING_TIME,
			#endif	
			#if (DINGLIANG_PENWU_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_DINGLIANGPENWU_ENABLE_FLAG,
				MENU_ZIBAO_DINGLIANGPENWU_DOING_TIME,
			#endif	
			#if (JIDAO_PENWU_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_JIDAOPENWU_ENABLE_FLAG,
				MENU_ZIBAO_JIDAOPENWU_DOING_TIME,
			#endif	
			#if (PENWU_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_PENWU_ENABLE_FLAG,
				MENU_ZIBAO_PENWU_DOING_TIME,
			#endif				


			#if (FANGMEI_PENWU_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_FANGMEIPENWU_ENABLE_FLAG,
				MENU_ZIBAO_FANGMEIPENWU_DOING_TIME,
			#endif	
			#if (LA_HOULIU_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_LAHOULIU_ENABLE_FLAG,
				MENU_ZIBAO_LAHOULIU_DOING_TIME,
			#endif	
			#if (SHEN_DITIAO_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHENDITIAO_ENABLE_FLAG,
				MENU_ZIBAO_SHENDITIAO_DOING_TIME,
			#endif	
			#if (SHOU_DITIAO_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SHOUDITIAO_ENABLE_FLAG,
				MENU_ZIBAO_SHOUDITIAO_DOING_TIME,
			#endif				
			#if (SONG_HOULIU_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_SONGHOULIU_ENABLE_FLAG,
				MENU_ZIBAO_SONGHOULIU_DOING_TIME,
			#endif	


			#if (QIANLIANG_SHANGBAI_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_QIANLIANGSHANGBAI_ENABLE_FLAG,
				MENU_ZIBAO_QIANLIANGSHANGBAI_DOING_TIME,
			#endif
			#if (QIANLIANG_XIABAI_ACT_ENABLED_FLAG	!= DISABLED) 
				MENU_ZIBAO_QIANLIANGXIABAI_ENABLE_FLAG,
				MENU_ZIBAO_QIANLIANGXIABAI_DOING_TIME,
			#endif				
		#endif
	};
#endif

//"ȱʡ����";
enum 
{
	#if (SET_YKQ_PRGPT_USE_FLAG != DISABLED)
		MENU_DEFAULT_STATION = 0x00,				//"Ӧ�ó���
		MENU_DEFAULT_VERSION = 0x01,				//"�汾"
	#else
	    MENU_DEFAULT_STATION = 0x00,				//"Ӧ�ó���    //qinxi
		MENU_DEFAULT_VERSION = 0x01,				//"�汾"
		MENU_WL_VERSION,                      //���߰汾   jhy
	#endif
	#if (ONLINE_UPDATE_SELF_PRG_PT_FLAG != DISABLED)
		MENU_DEFAULT_UPDATE_PRG,					//"����ң��������"
		MENU_CHENGUXABNG_UPDATE_PRG,	        //���������
		MENU_WL_UPDATE_PRG,
		MENU_CHENGUXABNG_WL_UPDATE_PRG,	        //���������
	#endif
	MENU_DEFAULT_REETORE_PARAM,
	#if IR_SEND >0
	MENU_IR_SEND_TEST,						//���ⷢ�����
	#endif
	MENU_FASTCOLIBARATE_ENABLE,				//���ٶ���
    MENU_WL_SEND_TEST,                      //���߲���
};

//"΢��ң��������";
enum 
{
	MENU_OTHER_YKQ_ID = 0x00,			//"ң�������",
	MENU_OTHER_YKQ_STATION,				//Ӧ�ó���
	MENU_OTHER_YKQ_VERSION,				//"�汾"
};

//"���̲���";
enum
{
	MENU_KEYFILM_SELECT = 0x00,			//ѡ�����Ĥ	2020.01.15 parry
//	MENU_FASTCOLIBARATE_ENABLE,
};

////"�趨������Ӧ�Ķ���";
//enum		
//{
//	MENU_ACTIONSET_KEYN_1 = 0x00,  	//KEY_NUMBER_N��һ����		�趨��������	2020.01.15 parry
//	MENU_ACTIONSET_KEYQ_1,					//KEY_NUMBER_Q��һ����
//	MENU_ACTIONSET_KEY1_1,					//KEY_NUMBER_1��һ����
//	MENU_ACTIONSET_KEY2_1,					//KEY_NUMBER_2��һ����
//	MENU_ACTIONSET_KEY3_1,					//KEY_NUMBER_3��һ����
//	MENU_ACTIONSET_KEY4_1,					//KEY_NUMBER_4��һ����

//	MENU_ACTIONSET_KEY5_1,  				//KEY_NUMBER_5��һ����
//	MENU_ACTIONSET_KEY6_1,					//KEY_NUMBER_6��һ����
//	MENU_ACTIONSET_KEY7_1,					//KEY_NUMBER_7��һ����
//	MENU_ACTIONSET_KEY8_1,					//KEY_NUMBER_8��һ����
//	MENU_ACTIONSET_KEY9_1,					//KEY_NUMBER_9��һ����
//	MENU_ACTIONSET_KEY0_1,					//KEY_NUMBER_0��һ����

//	MENU_ACTIONSET_KEYN_2,  				//KEY_NUMBER_N�ڶ�����
//	MENU_ACTIONSET_KEYQ_2,					//KEY_NUMBER_Q�ڶ�����
//	MENU_ACTIONSET_KEY1_2,					//KEY_NUMBER_1�ڶ�����
//	MENU_ACTIONSET_KEY2_2,					//KEY_NUMBER_2�ڶ�����
//	MENU_ACTIONSET_KEY3_2,					//KEY_NUMBER_3�ڶ�����
//	MENU_ACTIONSET_KEY4_2,					//KEY_NUMBER_4�ڶ�����

//	MENU_ACTIONSET_KEY5_2,  				//KEY_NUMBER_5�ڶ�����
//	MENU_ACTIONSET_KEY6_2,					//KEY_NUMBER_6�ڶ�����
//	MENU_ACTIONSET_KEY7_2,					//KEY_NUMBER_7�ڶ�����
//	MENU_ACTIONSET_KEY8_2,					//KEY_NUMBER_8�ڶ�����
//	MENU_ACTIONSET_KEY9_2,					//KEY_NUMBER_9�ڶ�����
//	MENU_ACTIONSET_KEY0_2,					//KEY_NUMBER_0�ڶ�����
//	
//	
////	MENU_ACTIONSET_KEYL_1,					//KEY_NUMBER_7�ڶ�����
////	MENU_ACTIONSET_KEYR_1,					//KEY_NUMBER_8�ڶ�����
////	MENU_ACTIONSET_KEYL_2,					//KEY_NUMBER_9�ڶ�����
////	MENU_ACTIONSET_KEYR_2,					//KEY_NUMBER_0�ڶ�����
//	
//	
//};

/********************************************************************************
* ��������
*********************************************************************************/
static u32 u32MenuGroupIDs[MENU_ID_MAX] = //��Ų˵����
{	
	MENU_YKQ_MACHINE_NO,
	
	#if (QUICK_KEY_CTRL_ENABLE_FLAG	!= DISABLED)
		#if (QUICK_KEY_1_ENABLE_FLAG != DISABLED)
			MENU_SHORTCUT_F1,			//��ݼ�1
		#endif
	
		#if (QUICK_KEY_2_ENABLE_FLAG != DISABLED)
			MENU_SHORTCUT_F2,			//��ݼ�2
		#endif	

		#if (QUICK_KEY_3_ENABLE_FLAG != DISABLED)
			MENU_SHORTCUT_F3,			//��ݼ�3
		#endif	
	
		#if (QUICK_KEY_4_ENABLE_FLAG != DISABLED)
			MENU_SHORTCUT_F4,			//��ݼ�4
		#endif	
	#endif	
	
	#if (ZIBAO_CTRL_ENABLE_FLAG	!= DISABLED)
		MENU_ZIBAO_PARAM_SET,					//�Ա�����
	#endif
	
	MENU_DEFAULT_GRP_NO,		//ȱʡ����
	
	#if (SET_YKQ_PRGPT_USE_FLAG != DISABLED)
		MENU_OTHER_YKQ_PARAM_NO,
	#else	
	
	#endif
	
//	MENU_KEY_FILM_SELECT,		//ѡ�����Ĥ	2020.01.15 parry
	
//	MENU_KEY_ACTION_SELECT	//�趨��������	2020.01.15 parry
};

/********************************************************************************
* ����ȫ�ֲ���
*********************************************************************************/

/********************************************************************************
* ��������
*********************************************************************************/
#if (SET_YKQ_PRGPT_USE_FLAG != DISABLED)
	u32 ParEditSetSearchDevFlagProc(u32 u32Flag);
#endif
u32 ParEdit(stKEY stNewKey,u32 *pIntfState);
#endif


