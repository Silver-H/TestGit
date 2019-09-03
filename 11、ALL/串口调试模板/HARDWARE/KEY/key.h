#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define Key_Set  		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)//读取代表加的按键
#define Key_Add  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)//读取代表减的按键
#define Key_Reduce  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)//读取代表减的按键
//#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键2 

#define PRESS 0
#define ON    1

#define KEY_SET    1
#define KEY_ADD    2
#define KEY_REDUCE 3

#define Enable  1
#define Disable 0

typedef struct {
	u8 Display_page;
	u8 QueryMomth;
	u8 QueryDay;
	u8 QueryYear;
}keySetModeStruct;

typedef struct {
	u8 queryTimeMonth[3];
	u8 queryTimeDay[3];
	u8 queryTimeYear[3];
}queryStruct;

extern queryStruct queryStructInit;
extern keySetModeStruct keySetMode;

void KEY_Init(void);//IO初始化
u8 keySetScan(void);	 
u8 keyAddScan(void);
u8 keyReduceScan(void);	

void keyValueFunction(u8 Value);

#endif
