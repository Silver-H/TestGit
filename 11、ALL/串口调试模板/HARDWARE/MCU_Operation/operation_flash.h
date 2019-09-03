#ifndef __OPERATION_FLASH_H
#define __OPERATION_FLASH_H

#include "sys.h"

typedef struct
{
	char timeOfOnePage[11];
	char timeOfTwoPage[9];
	u8 sectorAddrOfZeroPage[3];
	
	char batteryPercentageClearOfThreePage[6];
	char batteryPercentageNoClearOfFourPage[6];
}mcuReadFlashStruct;

extern mcuReadFlashStruct mcuReadFlashStructInit;
extern u8 GetDatFull;

void MCU_FlashWrite(void);	//向MCU的flash中写入数据
void MCU_FlashRead(u8 readMonth,u8 readDay);	//读取指定地方的数据

#endif 

