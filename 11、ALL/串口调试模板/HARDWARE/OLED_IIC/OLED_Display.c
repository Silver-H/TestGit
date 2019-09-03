#include "OLED_Display.h"
#include "OLED_I2C.h"
#include "delay.h"
#include "Get_ADC.h"
#include "timer4_EnergyAdd_Init.h"
#include "key.h"
#include "operation_flash.h"

//-----------------------初始化---------------------------------------------------------------//

//OLED开机显示初始化
void oledStartDis(void)
{
	u8 i=3;
	extern const unsigned char BMP1[];
	
	//****************OLEDμ?3?ê??ˉ**********************//
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

char Time_Sys_One[11]={48,49,50,51,52,53,54};	//OLED时间显示缓冲区
char Time_Sys_Two[9]={0};
//OLED显示
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

extern u8 TimeChagePageFlag;	//换页标志
extern keySetModeStruct keySetMode;
/*
		128*64点阵
		8*16 ---每行显示16个数字或字母
						共四行
		16*16 
*/
u8 trigger1 = Enable,trigger2 = Enable,trigger3 = Enable;	

void	oledDis(void)
{
	static u8 ChagePage=0;	//时间触发换页 （1s/次）
	static u8 pageTrigger[5]={Enable,Enable,Enable,Enable,Enable};
	
	//KEY_SET按键没动作并且曾经未触发
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
				case 0:	//居中显示"当前时间"
								if(pageTrigger[0] == Enable)
								{
									OLED_CLS();	//清屏
									pageTrigger[0] = Disable;
									OLED_ShowCN(2*16,0,0);	//显示'北'
									OLED_ShowCN(3*16,0,1);	//显示'京'
									OLED_ShowCN(4*16,0,2);	//显示'时'
									OLED_ShowCN(5*16,0,3);	//显示'间'
								}
								OLED_ShowStr(24,2,Time_Sys_One,3);//显示年月日，星期
								OLED_ShowStr(32,4,Time_Sys_Two,3);//显示时分秒
								break;
								
				case 1:	//居中显示"干净板电量数据"		---第一行
								if(pageTrigger[1] == Enable)
								{
									OLED_CLS();	//清屏
									pageTrigger[1] = Disable;
									OLED_ShowCN(0*16,0,11);	
									OLED_ShowCN(1*16,0,12);	
									OLED_ShowCN(2*16,0,13);	
									OLED_ShowCN(3*16,0,4);	
									OLED_ShowCN(4*16,0,14);	
									OLED_ShowCN(5*16,0,15);	
									OLED_ShowCN(6*16,0,16);	
									//显示'电压:'		---第二行
									OLED_ShowCN(0*16,2,4);	//显示'电'
									OLED_ShowCN(1*16,2,5);	//显示'压'
									OLED_ShowStr(32,2,":",3);	//显示‘:’
							
										//显示'电流:'		---第三行
									OLED_ShowCN(0*16,4,4);	//显示'电'
									OLED_ShowCN(1*16,4,6);	//显示'流'
									OLED_ShowStr(32,4,":",3);	//显示‘:’
							
								}
								OLED_ShowStr(40,2,AdcStructInitC.Voltage_S,3);	//显示电压数据
							
								OLED_ShowStr(40,4,AdcStructInitC.Current_S,3);
								break;
				case 2: //显示"干净板电量数据"		---第一行
								if(pageTrigger[2] == Enable)
								{
									OLED_CLS();	//清屏
									pageTrigger[2] = Disable;
									OLED_ShowCN(0*16,0,11);	
									OLED_ShowCN(1*16,0,12);	
									OLED_ShowCN(2*16,0,13);	
									OLED_ShowCN(3*16,0,4);	
									OLED_ShowCN(4*16,0,14);	
									OLED_ShowCN(5*16,0,15);	
									OLED_ShowCN(6*16,0,16);	
									//显示'功率:'		---第二行
									OLED_ShowCN(0*16,2,7);	//显示'功'
									OLED_ShowCN(1*16,2,8);	//显示'率'
									OLED_ShowStr(32,2,":",3);	//显示‘:’
									//显示'功率因数:'		---第三行（剩余7个字符）
									OLED_ShowCN(0*16,4,7);	//显示'电'
									OLED_ShowCN(1*16,4,8);	//显示'流'
									OLED_ShowCN(2*16,4,9);	//显示'因'
									OLED_ShowCN(3*16,4,10);	//显示'数'
									OLED_ShowStr(64,4,":",3);	//显示‘:’
								}
								
								OLED_ShowStr(40,2,AdcStructInitC.Power_S,3);	//显示电压数据
								AdcStructInitC.batteryPercentage_S[5] = '\0';
								OLED_ShowStr(72,4,AdcStructInitC.batteryPercentage_S,3);
								break;
				case 3:	//显示"不干净板电量数据"		---第一行
								if(pageTrigger[3] == Enable)
								{
									OLED_CLS();	//清屏
									pageTrigger[3] = Disable;
									OLED_ShowCN(0*16,0,17);	
									OLED_ShowCN(1*16,0,11);	
									OLED_ShowCN(2*16,0,12);	
									OLED_ShowCN(3*16,0,13);	
									OLED_ShowCN(4*16,0,4);	
									OLED_ShowCN(5*16,0,14);	
									OLED_ShowCN(6*16,0,15);	
									OLED_ShowCN(7*16,0,16);	
									//显示'电压:'		---第二行
									OLED_ShowCN(0*16,2,4);	//显示'电'
									OLED_ShowCN(1*16,2,5);	//显示'压'
									OLED_ShowStr(32,2,":",3);	//显示‘:’
								
										//显示'电流:'		---第三行
									OLED_ShowCN(0*16,4,4);	//显示'电'
									OLED_ShowCN(1*16,4,6);	//显示'流'
									OLED_ShowStr(32,4,":",3);	//显示‘:’
							
								}
								
								OLED_ShowStr(40,2,AdcStructInitNC.Voltage_S,3);	//显示电压数据
								
								OLED_ShowStr(40,4,AdcStructInitNC.Current_S,3);
								break;
				case 4: //居中显示"干净板电量数据"		---第一行
								if(pageTrigger[4] == Enable)
								{
									OLED_CLS();	//清屏
									pageTrigger[4] = Disable;
									OLED_ShowCN(0*16,0,17);	
									OLED_ShowCN(1*16,0,11);	
									OLED_ShowCN(2*16,0,12);	
									OLED_ShowCN(3*16,0,13);	
									OLED_ShowCN(4*16,0,4);	
									OLED_ShowCN(5*16,0,14);	
									OLED_ShowCN(6*16,0,15);	
									OLED_ShowCN(7*16,0,16);	
									//显示'功率:'		---第二行
									OLED_ShowCN(0*16,2,7);	//显示'功'
									OLED_ShowCN(1*16,2,8);	//显示'率'
									OLED_ShowStr(32,2,":",3);	//显示‘:’
									//显示'功率因数:'		---第三行（剩余7个字符）
									OLED_ShowCN(0*16,4,7);	//显示'电'
									OLED_ShowCN(1*16,4,8);	//显示'流'
									OLED_ShowCN(2*16,4,9);	//显示'因'
									OLED_ShowCN(3*16,4,10);	//显示'数'
									OLED_ShowStr(64,4,":",3);	//显示‘:’
								}
								
								OLED_ShowStr(40,2,AdcStructInitNC.Power_S,3);	//显示电压数据
								AdcStructInitNC.batteryPercentage_S[5] = '\0';
								OLED_ShowStr(72,4,AdcStructInitNC.batteryPercentage_S,3);
								break;
			}
	
						break;
			case 1://1、显示"设置月"
						if(trigger1 == Enable)
						{
							trigger1 = Disable;
							OLED_CLS();	//清屏
							
							
							OLED_ShowCN(2*16,0,18);		//第一行
							OLED_ShowCN(3*16,0,19);	
							OLED_ShowCN(4*16,0,25);
							
							OLED_ShowCN(4*16,2,25);	//显示月
							
							OLED_ShowCN(0*16,6,26);		//'加'
							OLED_ShowStr(48,6,"NEXT",3);	//
							OLED_ShowCN(7*16,6,27);		//‘减’
						}
												//将keySetMode.QueryMomth转换为字符
						queryStructInit.queryTimeMonth[0] = keySetMode.QueryMomth/10+'0';
						queryStructInit.queryTimeMonth[1] = keySetMode.QueryMomth%10+'0';
						queryStructInit.queryTimeMonth[2] = '\0';
						OLED_ShowStr(48,2,(char*)queryStructInit.queryTimeMonth,3);//第二行
						
						break;
			case 2://1、显示"设置日"
						if(trigger2 == Enable)
						{
							trigger2 = Disable;
							OLED_CLS();	//清屏
							
							OLED_ShowCN(2*16,0,18);		//第一行
							OLED_ShowCN(3*16,0,19);	
							OLED_ShowCN(4*16,0,20);
							
							OLED_ShowCN(4*16,2,20);	//显示日
							
							OLED_ShowCN(0*16,6,26);		//'加'
							OLED_ShowStr(48,6,"NEXT",3);	//
							OLED_ShowCN(7*16,6,27);		//‘减’
						}
											//将keySetMode.QueryDay转换为字符
						queryStructInit.queryTimeDay[0] = keySetMode.QueryDay/10+'0';
						queryStructInit.queryTimeDay[1] = keySetMode.QueryDay%10+'0';
						queryStructInit.queryTimeDay[2] = '\0';
						OLED_ShowStr(48,2,(char*)queryStructInit.queryTimeDay,3);//第二行
						
						break;
			case 3://显示‘查询时间’
				if(GetDatFull == FALSE)
				{
						if(trigger3 == Enable)
						{
							OLED_CLS();	//清屏
							trigger3 = Disable;
							
							OLED_ShowCN(2*16,0,18);	
							OLED_ShowCN(3*16,0,19);	
							OLED_ShowCN(4*16,0,2);	
							OLED_ShowCN(5*16,0,3);	 //第一行
							OLED_ShowStr(0,2,"20",3);
							
								//Clean
							OLED_ShowStr(0,4,"C:",3);
							
							OLED_ShowStr(0,6,"NC:",3);			
						}
						
						OLED_ShowStr(16,2,(char*)queryStructInit.queryTimeYear,3);//第二行
						OLED_ShowStr(32,2,":",3);//第二行
						OLED_ShowStr(40,2,(char*)queryStructInit.queryTimeMonth,3);//第二行
						OLED_ShowStr(56,2,":",3);
						OLED_ShowStr(64,2,(char*)queryStructInit.queryTimeDay,3);//第二行
					//	OLED_ShowStr(80,2,"      ",3);//第二行
						//OLED_ShowStr(16,2,(char*)mcuReadFlashStructInit.timeOfOnePage,3);//显示年月日，星期
					
						OLED_ShowStr(16,4,(char*)mcuReadFlashStructInit.batteryPercentageClearOfThreePage,3);//第三行
						
					  OLED_ShowStr(24,6,(char*)mcuReadFlashStructInit.batteryPercentageNoClearOfFourPage,3);//第四行
				}
				else		//显示"查询日期不存在，请重新设置时间"
				{
					GetDatFull = FALSE;
					if(trigger3 == Enable)
					{
						OLED_CLS();	//清屏
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
////	OLED_ShowStr(40,0,AdcStructInit.Voltage_S,3);//??ê?μ??1
////	OLED_ShowStr(40,2,AdcStructInit.Current_S,3);//??ê?μ?á÷
//	OLED_ShowStr(40,0,Time_Sys_One,3);//显示年月日，星期
//	OLED_ShowStr(40,2,Time_Sys_Two,3);//显示时分秒

//	if((keyValue == Key_Set) ||(keyValue ==Key_Set ))
//	{
//		OLED_ShowCN(0*16,4,4);
//		OLED_ShowCN(1*16,4,5);	//显示电压
//		OLED_ShowCN(0*16,6,6);
//		OLED_ShowCN(1*16,6,7);	//显示电流
//		OLED_ShowStr(40,4,AdcStructInit.Voltage_S,3);//??ê?μ?á÷
//		OLED_ShowStr(40,6,AdcStructInit.Current_S,3);//??ê?????±è
//		
//		OLED_ShowStr(120,4,"V",3);//显示"V"
//		OLED_ShowStr(120,6,"A",3);//显示"A"
//	}
//	else
//	{
//		
//		OLED_ShowCN(0*16,4,8);
//		OLED_ShowCN(1*16,4,9);	//显示功率
//		OLED_ShowCN(0*16,6,10);
//		OLED_ShowCN(1*16,6,11);	//显示电能
//		
//		OLED_ShowStr(40,4,AdcStructInit.Power_S,3);
//		OLED_ShowStr(40,6,AdcStructInit.Energy_S,3);
//		
//		OLED_ShowStr(120,4,"W",3);//显示"W"
//		if(flagEnergyUnit==W)
//		{
//			OLED_ShowStr(111,6,"WH",3);//显示"WH"
//		}
//		else if(flagEnergyUnit==KW)
//		{
//			OLED_ShowStr(103,6,"KWH",3);//显示"KWH"
//		}
//	}
//}




