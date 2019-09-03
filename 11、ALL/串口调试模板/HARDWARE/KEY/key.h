#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define Key_Set  		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)//��ȡ����ӵİ���
#define Key_Add  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)//��ȡ������İ���
#define Key_Reduce  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)//��ȡ������İ���
//#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����2 

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

void KEY_Init(void);//IO��ʼ��
u8 keySetScan(void);	 
u8 keyAddScan(void);
u8 keyReduceScan(void);	

void keyValueFunction(u8 Value);

#endif
