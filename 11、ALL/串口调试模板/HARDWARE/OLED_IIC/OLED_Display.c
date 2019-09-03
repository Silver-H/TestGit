#include "OLED_Display.h"
#include "OLED_I2C.h"
#include "delay.h"
#include "Get_ADC.h"
#include "timer4_EnergyAdd_Init.h"
#include "key.h"
#include "operation_flash.h"

//-----------------------��ʼ��---------------------------------------------------------------//

//OLED������ʾ��ʼ��
void oledStartDis(void)
{
	u8 i=3;
	extern const unsigned char BMP1[];
	
	//****************OLED��?3?��??��**********************//
	I2C_Configuration();

	OLED_Init();
	OLED_Fill(0x00);//???
//	while(i>0)
//	{
//		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//??BMP????
//		delay_ms(700);
//		i--;
//		OLED_Fill(0x00);//???
//		delay_ms(300);
//	}
}

oledStruct oledStructInit;

char Time_Sys_One[11]={48,49,50,51,52,53,54};	//OLEDʱ����ʾ������
char Time_Sys_Two[9]={0};
//OLED��ʾ
void HexToChar(Time_Def *time)
{
	
	
	Time_Sys_One[0]=time->year/16+'0';
	Time_Sys_One[1]=time->year%16+'0';
	Time_Sys_One[2]=':';
	Time_Sys_One[3]=time->month/16+'0';
	Time_Sys_One[4]=time->month%16+'0';
	Time_Sys_One[5]=':';
	Time_Sys_One[6]=time->day/16+'0';
	Time_Sys_One[7]=time->day%16+'0';
	Time_Sys_One[8]='-';
	Time_Sys_One[9]=time->week+'0';
	Time_Sys_One[10]='\0';
	
	Time_Sys_Two[0]=time->hour/16+'0';
	Time_Sys_Two[1]=time->hour%16+'0';
	Time_Sys_Two[2]=':';
	Time_Sys_Two[3]=time->minute/16+'0';
	Time_Sys_Two[4]=time->minute%16+'0';
	Time_Sys_Two[5]=':';
	Time_Sys_Two[6]=time->second/16+'0';
	Time_Sys_Two[7]=time->second%16+'0';
	Time_Sys_Two[8]='\0';
}

extern u8 TimeChagePageFlag;	//��ҳ��־
extern keySetModeStruct keySetMode;
/*
		128*64����
		8*16 ---ÿ����ʾ16�����ֻ���ĸ
						������
		16*16 
*/
u8 trigger1 = Enable,trigger2 = Enable,trigger3 = Enable;	

void	oledDis(void)
{
	static u8 ChagePage=0;	//ʱ�䴥����ҳ ��1s/�Σ�
	static u8 pageTrigger[5]={Enable,Enable,Enable,Enable,Enable};
	
	//KEY_SET����û������������δ����
	switch(keySetMode.Display_page)
	{
		case 0:
			if(TimeChagePageFlag)
			{
				TimeChagePageFlag = 0;
				ChagePage++;
				if(ChagePage == 5)
				{
					ChagePage = 0;
					pageTrigger[0] = Enable;
					pageTrigger[1] = Enable;
					pageTrigger[2] = Enable;
					pageTrigger[3] = Enable;
					pageTrigger[4] = Enable;
				}
			}
			switch(ChagePage)
			{
				case 0:	//������ʾ"��ǰʱ��"
								if(pageTrigger[0] == Enable)
								{
									OLED_CLS();	//����
									pageTrigger[0] = Disable;
									OLED_ShowCN(2*16,0,0);	//��ʾ'��'
									OLED_ShowCN(3*16,0,1);	//��ʾ'��'
									OLED_ShowCN(4*16,0,2);	//��ʾ'ʱ'
									OLED_ShowCN(5*16,0,3);	//��ʾ'��'
								}
								OLED_ShowStr(24,2,Time_Sys_One,3);//��ʾ�����գ�����
								OLED_ShowStr(32,4,Time_Sys_Two,3);//��ʾʱ����
								break;
								
				case 1:	//������ʾ"�ɾ����������"		---��һ��
								if(pageTrigger[1] == Enable)
								{
									OLED_CLS();	//����
									pageTrigger[1] = Disable;
									OLED_ShowCN(0*16,0,11);	
									OLED_ShowCN(1*16,0,12);	
									OLED_ShowCN(2*16,0,13);	
									OLED_ShowCN(3*16,0,4);	
									OLED_ShowCN(4*16,0,14);	
									OLED_ShowCN(5*16,0,15);	
									OLED_ShowCN(6*16,0,16);	
									//��ʾ'��ѹ:'		---�ڶ���
									OLED_ShowCN(0*16,2,4);	//��ʾ'��'
									OLED_ShowCN(1*16,2,5);	//��ʾ'ѹ'
									OLED_ShowStr(32,2,":",3);	//��ʾ��:��
							
										//��ʾ'����:'		---������
									OLED_ShowCN(0*16,4,4);	//��ʾ'��'
									OLED_ShowCN(1*16,4,6);	//��ʾ'��'
									OLED_ShowStr(32,4,":",3);	//��ʾ��:��
							
								}
								OLED_ShowStr(40,2,AdcStructInitC.Voltage_S,3);	//��ʾ��ѹ����
							
								OLED_ShowStr(40,4,AdcStructInitC.Current_S,3);
								break;
				case 2: //��ʾ"�ɾ����������"		---��һ��
								if(pageTrigger[2] == Enable)
								{
									OLED_CLS();	//����
									pageTrigger[2] = Disable;
									OLED_ShowCN(0*16,0,11);	
									OLED_ShowCN(1*16,0,12);	
									OLED_ShowCN(2*16,0,13);	
									OLED_ShowCN(3*16,0,4);	
									OLED_ShowCN(4*16,0,14);	
									OLED_ShowCN(5*16,0,15);	
									OLED_ShowCN(6*16,0,16);	
									//��ʾ'����:'		---�ڶ���
									OLED_ShowCN(0*16,2,7);	//��ʾ'��'
									OLED_ShowCN(1*16,2,8);	//��ʾ'��'
									OLED_ShowStr(32,2,":",3);	//��ʾ��:��
									//��ʾ'��������:'		---�����У�ʣ��7���ַ���
									OLED_ShowCN(0*16,4,7);	//��ʾ'��'
									OLED_ShowCN(1*16,4,8);	//��ʾ'��'
									OLED_ShowCN(2*16,4,9);	//��ʾ'��'
									OLED_ShowCN(3*16,4,10);	//��ʾ'��'
									OLED_ShowStr(64,4,":",3);	//��ʾ��:��
								}
								
								OLED_ShowStr(40,2,AdcStructInitC.Power_S,3);	//��ʾ��ѹ����
								AdcStructInitC.batteryPercentage_S[5] = '\0';
								OLED_ShowStr(72,4,AdcStructInitC.batteryPercentage_S,3);
								break;
				case 3:	//��ʾ"���ɾ����������"		---��һ��
								if(pageTrigger[3] == Enable)
								{
									OLED_CLS();	//����
									pageTrigger[3] = Disable;
									OLED_ShowCN(0*16,0,17);	
									OLED_ShowCN(1*16,0,11);	
									OLED_ShowCN(2*16,0,12);	
									OLED_ShowCN(3*16,0,13);	
									OLED_ShowCN(4*16,0,4);	
									OLED_ShowCN(5*16,0,14);	
									OLED_ShowCN(6*16,0,15);	
									OLED_ShowCN(7*16,0,16);	
									//��ʾ'��ѹ:'		---�ڶ���
									OLED_ShowCN(0*16,2,4);	//��ʾ'��'
									OLED_ShowCN(1*16,2,5);	//��ʾ'ѹ'
									OLED_ShowStr(32,2,":",3);	//��ʾ��:��
								
										//��ʾ'����:'		---������
									OLED_ShowCN(0*16,4,4);	//��ʾ'��'
									OLED_ShowCN(1*16,4,6);	//��ʾ'��'
									OLED_ShowStr(32,4,":",3);	//��ʾ��:��
							
								}
								
								OLED_ShowStr(40,2,AdcStructInitNC.Voltage_S,3);	//��ʾ��ѹ����
								
								OLED_ShowStr(40,4,AdcStructInitNC.Current_S,3);
								break;
				case 4: //������ʾ"�ɾ����������"		---��һ��
								if(pageTrigger[4] == Enable)
								{
									OLED_CLS();	//����
									pageTrigger[4] = Disable;
									OLED_ShowCN(0*16,0,17);	
									OLED_ShowCN(1*16,0,11);	
									OLED_ShowCN(2*16,0,12);	
									OLED_ShowCN(3*16,0,13);	
									OLED_ShowCN(4*16,0,4);	
									OLED_ShowCN(5*16,0,14);	
									OLED_ShowCN(6*16,0,15);	
									OLED_ShowCN(7*16,0,16);	
									//��ʾ'����:'		---�ڶ���
									OLED_ShowCN(0*16,2,7);	//��ʾ'��'
									OLED_ShowCN(1*16,2,8);	//��ʾ'��'
									OLED_ShowStr(32,2,":",3);	//��ʾ��:��
									//��ʾ'��������:'		---�����У�ʣ��7���ַ���
									OLED_ShowCN(0*16,4,7);	//��ʾ'��'
									OLED_ShowCN(1*16,4,8);	//��ʾ'��'
									OLED_ShowCN(2*16,4,9);	//��ʾ'��'
									OLED_ShowCN(3*16,4,10);	//��ʾ'��'
									OLED_ShowStr(64,4,":",3);	//��ʾ��:��
								}
								
								OLED_ShowStr(40,2,AdcStructInitNC.Power_S,3);	//��ʾ��ѹ����
								AdcStructInitNC.batteryPercentage_S[5] = '\0';
								OLED_ShowStr(72,4,AdcStructInitNC.batteryPercentage_S,3);
								break;
			}
	
						break;
			case 1://1����ʾ"������"
						if(trigger1 == Enable)
						{
							trigger1 = Disable;
							OLED_CLS();	//����
							
							
							OLED_ShowCN(2*16,0,18);		//��һ��
							OLED_ShowCN(3*16,0,19);	
							OLED_ShowCN(4*16,0,25);
							
							OLED_ShowCN(4*16,2,25);	//��ʾ��
							
							OLED_ShowCN(0*16,6,26);		//'��'
							OLED_ShowStr(48,6,"NEXT",3);	//
							OLED_ShowCN(7*16,6,27);		//������
						}
												//��keySetMode.QueryMomthת��Ϊ�ַ�
						queryStructInit.queryTimeMonth[0] = keySetMode.QueryMomth/10+'0';
						queryStructInit.queryTimeMonth[1] = keySetMode.QueryMomth%10+'0';
						queryStructInit.queryTimeMonth[2] = '\0';
						OLED_ShowStr(48,2,(char*)queryStructInit.queryTimeMonth,3);//�ڶ���
						
						break;
			case 2://1����ʾ"������"
						if(trigger2 == Enable)
						{
							trigger2 = Disable;
							OLED_CLS();	//����
							
							OLED_ShowCN(2*16,0,18);		//��һ��
							OLED_ShowCN(3*16,0,19);	
							OLED_ShowCN(4*16,0,20);
							
							OLED_ShowCN(4*16,2,20);	//��ʾ��
							
							OLED_ShowCN(0*16,6,26);		//'��'
							OLED_ShowStr(48,6,"NEXT",3);	//
							OLED_ShowCN(7*16,6,27);		//������
						}
											//��keySetMode.QueryDayת��Ϊ�ַ�
						queryStructInit.queryTimeDay[0] = keySetMode.QueryDay/10+'0';
						queryStructInit.queryTimeDay[1] = keySetMode.QueryDay%10+'0';
						queryStructInit.queryTimeDay[2] = '\0';
						OLED_ShowStr(48,2,(char*)queryStructInit.queryTimeDay,3);//�ڶ���
						
						break;
			case 3://��ʾ����ѯʱ�䡯
				if(GetDatFull == FALSE)
				{
						if(trigger3 == Enable)
						{
							OLED_CLS();	//����
							trigger3 = Disable;
							
							OLED_ShowCN(2*16,0,18);	
							OLED_ShowCN(3*16,0,19);	
							OLED_ShowCN(4*16,0,2);	
							OLED_ShowCN(5*16,0,3);	 //��һ��
							OLED_ShowStr(0,2,"20",3);
							
								//Clean
							OLED_ShowStr(0,4,"C:",3);
							
							OLED_ShowStr(0,6,"NC:",3);			
						}
						
						OLED_ShowStr(16,2,(char*)queryStructInit.queryTimeYear,3);//�ڶ���
						OLED_ShowStr(32,2,":",3);//�ڶ���
						OLED_ShowStr(40,2,(char*)queryStructInit.queryTimeMonth,3);//�ڶ���
						OLED_ShowStr(56,2,":",3);
						OLED_ShowStr(64,2,(char*)queryStructInit.queryTimeDay,3);//�ڶ���
					//	OLED_ShowStr(80,2,"      ",3);//�ڶ���
						//OLED_ShowStr(16,2,(char*)mcuReadFlashStructInit.timeOfOnePage,3);//��ʾ�����գ�����
					
						OLED_ShowStr(16,4,(char*)mcuReadFlashStructInit.batteryPercentageClearOfThreePage,3);//������
						
					  OLED_ShowStr(24,6,(char*)mcuReadFlashStructInit.batteryPercentageNoClearOfFourPage,3);//������
				}
				else		//��ʾ"��ѯ���ڲ����ڣ�����������ʱ��"
				{
					GetDatFull = FALSE;
					if(trigger3 == Enable)
					{
						OLED_CLS();	//����
						trigger3 = Disable;
						
						OLED_ShowCN(0*16,2,18);	
						OLED_ShowCN(1*16,2,19);	
						OLED_ShowCN(2*16,2,20);	
						OLED_ShowCN(3*16,2,21);	
						OLED_ShowCN(4*16,2,17);	
						OLED_ShowCN(5*16,2,28);	
						OLED_ShowCN(6*16,2,29);	
						OLED_ShowStr(112,2,",",3);
									
						OLED_ShowCN(0*16,4,30);	
						OLED_ShowCN(1*16,4,31);	
						OLED_ShowCN(2*16,4,32);	
						OLED_ShowCN(3*16,4,33);	
						OLED_ShowCN(4*16,4,34);	
						OLED_ShowCN(5*16,4,2);	
						OLED_ShowCN(6*16,4,3);	
						OLED_ShowStr(112,4,"!",3);
					}
				}
						break;
		}
}

//void oledDis(void)
//{
////	OLED_ShowStr(40,0,AdcStructInit.Voltage_S,3);//??��?��??1
////	OLED_ShowStr(40,2,AdcStructInit.Current_S,3);//??��?��?����
//	OLED_ShowStr(40,0,Time_Sys_One,3);//��ʾ�����գ�����
//	OLED_ShowStr(40,2,Time_Sys_Two,3);//��ʾʱ����

//	if((keyValue == Key_Set) ||(keyValue ==Key_Set ))
//	{
//		OLED_ShowCN(0*16,4,4);
//		OLED_ShowCN(1*16,4,5);	//��ʾ��ѹ
//		OLED_ShowCN(0*16,6,6);
//		OLED_ShowCN(1*16,6,7);	//��ʾ����
//		OLED_ShowStr(40,4,AdcStructInit.Voltage_S,3);//??��?��?����
//		OLED_ShowStr(40,6,AdcStructInit.Current_S,3);//??��?????����
//		
//		OLED_ShowStr(120,4,"V",3);//��ʾ"V"
//		OLED_ShowStr(120,6,"A",3);//��ʾ"A"
//	}
//	else
//	{
//		
//		OLED_ShowCN(0*16,4,8);
//		OLED_ShowCN(1*16,4,9);	//��ʾ����
//		OLED_ShowCN(0*16,6,10);
//		OLED_ShowCN(1*16,6,11);	//��ʾ����
//		
//		OLED_ShowStr(40,4,AdcStructInit.Power_S,3);
//		OLED_ShowStr(40,6,AdcStructInit.Energy_S,3);
//		
//		OLED_ShowStr(120,4,"W",3);//��ʾ"W"
//		if(flagEnergyUnit==W)
//		{
//			OLED_ShowStr(111,6,"WH",3);//��ʾ"WH"
//		}
//		else if(flagEnergyUnit==KW)
//		{
//			OLED_ShowStr(103,6,"KWH",3);//��ʾ"KWH"
//		}
//	}
//}




