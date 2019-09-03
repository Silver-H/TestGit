#include "key.h"
#include "delay.h"
#include "rtc.h"
#include "operation_flash.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//按键输入 驱动代码		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/06
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									   
//////////////////////////////////////////////////////////////////////////////////	 


//按键初始化函数 
//PA15和PC5 设置成输入
void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9;//PB4/PB5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入 默认高电平
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9);
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;//PC5
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
// 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC5
// 
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA0
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0
	
} 
//-----------------------按键扫描，判断按键是否动作----------------------------------------------------------------//
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
//		case 1:	//稳态
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
//		case 2:	//弹起
//					if(Key_Set ==ON)
//					{
//						index = 0; 
//						return keyNum;
//					}
//					else 
//					{
//						index = 2;	//继续检测弹起
//						
//					}
//					break;
//		case 3:	//弹起
//					if(Key_Add ==ON)
//					{
//						index = 0; 
//						return keyNum;
//					}
//					else 
//					{
//						index = 2;	//继续检测弹起
//						
//					}
//					break;
//		case 4:	//弹起
//					if(Key_Reduce ==ON)
//					{
//						index = 0; 
//						return keyNum;
//					}
//					else 
//					{
//						index = 2;	//继续检测弹起
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
		if(keySetMode.Display_page == 0)	//查询模式
		{
			keySetMode.Display_page = 1;		//显示第一页
			Value = 0;
		}
	}
	switch (keySetMode.Display_page)
	{
		case 1://查询哪月
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
						keySetMode.QueryDay = sysTime.day/16*10+sysTime.day%16;	//必需提前赋值
						
						keySetMode.Display_page=2;triggerMonth = Enable;
					}
					break;
		case 2://查询哪天

				if(triggerDay == Enable)
				{
					triggerDay = Disable;
					
					//判断平年还是闰年，以及哪一个月决定查询二月有多少天。
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
					//二月 
					else if(keySetMode.QueryMomth == 2)
					{
						year = 2000+sysTime.year/16*10+sysTime.year%16;
						//四年一闰，百年不闰，四百年再闰
						if(((year/4 == 0)&&(year/100 != 0))||(year/400 == 0))	//是闰年
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
					MCU_FlashRead(keySetMode.QueryMomth,keySetMode.QueryDay);	//数据需要提前读取
					keySetMode.Display_page=3;triggerDay = Enable;
				}
				break;
		case 3://查询的数据
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
		函数名：u8 Key_Scan(void)
		返回值：u8
		参数：
		描述：三个按键的状态机扫描函数；
					按键间互斥；
					按键释放返回键值
**/ 


////按键处理函数
////返回按键值
////mode:0,不支持连续按;1,支持连续按;
////返回值：
////0，没有任何按键按下
////KEY0_PRES，KEY0按下
////KEY1_PRES，KEY1按下
////WKUP_PRES，WK_UP按下 
////注意此函数有响应优先级,KEY0>KEY1>WK_UP!!
//u8 KEY_Scan(u8 mode)
//{	 
//	static u8 key_up=1;//按键按松开标志
//	if(mode)key_up=1;  //支持连按		  
//	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
//	{
//		delay_ms(10);//去抖动 
//		key_up=0;
//		if(KEY0==0)return KEY0_PRES;
//		else if(KEY1==0)return KEY1_PRES;
//		else if(WK_UP==1)return WKUP_PRES; 
//	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 	     
//	return 0;// 无按键按下
//}
