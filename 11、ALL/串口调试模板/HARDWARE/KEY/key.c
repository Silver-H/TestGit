#include "key.h"
#include "delay.h"
#include "rtc.h"
#include "operation_flash.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//�������� ��������		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/06
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									   
//////////////////////////////////////////////////////////////////////////////////	 


//������ʼ������ 
//PA15��PC5 ���ó�����
void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����
	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9;//PB4/PB5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó��������� Ĭ�ϸߵ�ƽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	GPIO_SetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9);
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;//PC5
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
// 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC5
// 
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA0
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0
	
} 
//-----------------------����ɨ�裬�жϰ����Ƿ���----------------------------------------------------------------//
keySetModeStruct keySetMode = {0,1,1,1};

u8 keySetScan(void)
{
	static u8 index = 0;
	static u8 keyNum = 0;
	
	switch(index)
	{
		case 0:
					if(Key_Set == PRESS)	
					{
						index = 1;
					}
					else 
						index = 0;
					break;
		case 1:
					if(Key_Set == PRESS)	
					{
						index = 2;keyNum = KEY_SET;
					}
					else 
						index = 0;
					break;			
		case 2:
					if(Key_Set == ON)	
					{
						index = 0;return keyNum;
					}
					else 
						index = 2;
					break;		
	}
	return 0;
}
u8 keyAddScan(void)
{
	static u8 index = 0;
	static u8 keyNum = 0;
	
	switch(index)
	{
		case 0:
					if(Key_Add == PRESS)	
					{
						index = 1;
					}
					else 
						index = 0;
					break;
		case 1:
					if(Key_Add == PRESS)	
					{
						index = 2;keyNum = KEY_ADD;
					}
					else 
						index = 0;
					break;			
		case 2:
					if(Key_Add == ON)	
					{
						index = 0;return keyNum;
					}
					else 
						index = 2;
					break;		
	}
	return 0;
}
u8 keyReduceScan(void)
{
	static u8 index = 0;
	static u8 keyNum = 0;
	
	switch(index)
	{
		case 0:
					if(Key_Reduce == PRESS)	
					{
						index = 1;
					}
					else 
						index = 0;
					break;
		case 1:
					if(Key_Reduce == PRESS)	
					{
						index = 2;keyNum = KEY_REDUCE;
					}
					else 
						index = 0;
					break;			
		case 2:
					if(Key_Reduce == ON)	
					{
						index = 0;return keyNum;
					}
					else 
						index = 2;
					break;		
	}
	return 0;
}

//u8 keyScan(void)
//{
//	static u8 index = 0;
//	static u8 keyNum = 0;
//	
//	switch(index)
//	{
//		case 0:
//					if(Key_Set == PRESS)
//					{
//						index = 1;
//					}
//					else if(Key_Add == PRESS)
//					{
//						index = 1;
//					}
//					else if(Key_Reduce == PRESS)
//					{
//						index = 1;
//					}
//					else if((Key_Set != PRESS)&&(Key_Add != PRESS)&&(Key_Reduce != PRESS))
//					{
//						index = 0;
//					}
//					break;
//		case 1:	//��̬
//					if(Key_Set ==PRESS)
//					{
//						index=2;;keyNum = KEY_SET;
//					}
//					else if(Key_Add ==PRESS)
//					{
//						index=3;keyNum = KEY_ADD;
//					}
//					else if(Key_Reduce ==PRESS)
//					{
//						index=4;keyNum = KEY_REDUCE;
//					}
//					else if((Key_Set != PRESS)&&(Key_Add != PRESS)&&(Key_Reduce != PRESS))
//					{
//						index = 0;
//					}
//					break;
//		case 2:	//����
//					if(Key_Set ==ON)
//					{
//						index = 0; 
//						return keyNum;
//					}
//					else 
//					{
//						index = 2;	//������ⵯ��
//						
//					}
//					break;
//		case 3:	//����
//					if(Key_Add ==ON)
//					{
//						index = 0; 
//						return keyNum;
//					}
//					else 
//					{
//						index = 2;	//������ⵯ��
//						
//					}
//					break;
//		case 4:	//����
//					if(Key_Reduce ==ON)
//					{
//						index = 0; 
//						return keyNum;
//					}
//					else 
//					{
//						index = 2;	//������ⵯ��
//						
//					}
//					break;	
//	}
//	return 0;
//}
#include "stdio.h"

queryStruct queryStructInit;
extern u8 trigger1,trigger2 ,trigger3;	
void keyValueFunction(u8 Value)
{
	static u8 MaxDayNum=30;
	u16 year;
	static u8 triggerDay = Enable;
	static u8 triggerMonth = Enable;
	static u8 triggerQueryData=Enable;
	
	if(Value == KEY_SET)
	{
		if(keySetMode.Display_page == 0)	//��ѯģʽ
		{
			keySetMode.Display_page = 1;		//��ʾ��һҳ
			Value = 0;
		}
	}
	switch (keySetMode.Display_page)
	{
		case 1://��ѯ����
					if(triggerMonth == Enable)
					{
						triggerMonth = Disable;
						keySetMode.QueryMomth = sysTime.month/16*10+sysTime.month%16;
						keySetMode.QueryYear = sysTime.year/16*10+sysTime.year%16;
						  
						queryStructInit.queryTimeYear[0] = keySetMode.QueryYear/10+'0';
						queryStructInit.queryTimeYear[1] = keySetMode.QueryYear%10+'0';	
						queryStructInit.queryTimeYear[2] = '\0';
					}	
					if(Value == KEY_ADD)
					{
						keySetMode.QueryMomth++;
						
						if(keySetMode.QueryMomth>=13)
						{
							keySetMode.QueryMomth=1;
						}
					}
					if(Value == KEY_REDUCE)
					{
						keySetMode.QueryMomth--;
						if(keySetMode.QueryMomth<=0)
						{
							keySetMode.QueryMomth=12;
						}
					}
					if(Value == KEY_SET)
					{
						keySetMode.QueryDay = sysTime.day/16*10+sysTime.day%16;	//������ǰ��ֵ
						
						keySetMode.Display_page=2;triggerMonth = Enable;
					}
					break;
		case 2://��ѯ����

				if(triggerDay == Enable)
				{
					triggerDay = Disable;
					
					//�ж�ƽ�껹�����꣬�Լ���һ���¾�����ѯ�����ж����졣
					if((keySetMode.QueryMomth == 1)||(keySetMode.QueryMomth == 3)
						||(keySetMode.QueryMomth == 5)||(keySetMode.QueryMomth == 7)
					||(keySetMode.QueryMomth == 8)||(keySetMode.QueryMomth == 10)||(keySetMode.QueryMomth == 12))
					{
						MaxDayNum =31;
					}
					else if((keySetMode.QueryMomth == 4)||(keySetMode.QueryMomth == 6)
						||(keySetMode.QueryMomth == 9)||(keySetMode.QueryMomth == 11))
					{
						MaxDayNum =30;
					}
					//���� 
					else if(keySetMode.QueryMomth == 2)
					{
						year = 2000+sysTime.year/16*10+sysTime.year%16;
						//����һ�򣬰��겻���İ�������
						if(((year/4 == 0)&&(year/100 != 0))||(year/400 == 0))	//������
						{
							MaxDayNum = 29;
						}
						else 
							MaxDayNum = 28;
					}
				}
				if(Value == KEY_ADD)
				{
					keySetMode.QueryDay++;
					
					if(keySetMode.QueryDay>=(MaxDayNum+1))
					{
						keySetMode.QueryDay=1;
					}
				}
				if(Value == KEY_REDUCE)
				{
					keySetMode.QueryDay--;
					if(keySetMode.QueryDay<=0)
					{
						keySetMode.QueryDay=MaxDayNum;
					}
				}
				if(Value == KEY_SET)
				{
					MCU_FlashRead(keySetMode.QueryMomth,keySetMode.QueryDay);	//������Ҫ��ǰ��ȡ
					keySetMode.Display_page=3;triggerDay = Enable;
				}
				break;
		case 3://��ѯ������
					if(triggerQueryData == Enable)
					{
						triggerQueryData = Disable;
						
					}
					
					if(Value == KEY_SET)
					{
						triggerQueryData = Enable;
						keySetMode.Display_page=0;
						trigger1 = Enable;trigger2 = Enable;
						trigger3 = Enable;
					}
				break;
	}

}
/*
		��������u8 Key_Scan(void)
		����ֵ��u8
		������
		����������������״̬��ɨ�躯����
					�����以�⣻
					�����ͷŷ��ؼ�ֵ
**/ 


////����������
////���ذ���ֵ
////mode:0,��֧��������;1,֧��������;
////����ֵ��
////0��û���κΰ�������
////KEY0_PRES��KEY0����
////KEY1_PRES��KEY1����
////WKUP_PRES��WK_UP���� 
////ע��˺�������Ӧ���ȼ�,KEY0>KEY1>WK_UP!!
//u8 KEY_Scan(u8 mode)
//{	 
//	static u8 key_up=1;//�������ɿ���־
//	if(mode)key_up=1;  //֧������		  
//	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
//	{
//		delay_ms(10);//ȥ���� 
//		key_up=0;
//		if(KEY0==0)return KEY0_PRES;
//		else if(KEY1==0)return KEY1_PRES;
//		else if(WK_UP==1)return WKUP_PRES; 
//	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 	     
//	return 0;// �ް�������
//}
