#ifndef __OLED_DISPLAY_H
#define	__OLED_DISPLAY_H

#include "sys.h"
#include "rtc.h"

#define OLED_MAX_Dis 10	//OLED列最大显示8*16

typedef struct _oledStruct
{
	char  Oled_Voltage[OLED_MAX_Dis];
	char  Oled_Current[OLED_MAX_Dis];
	char  Oled_Power[OLED_MAX_Dis];
	char  Oled_Energy[OLED_MAX_Dis];
	char  Oled_ZhanKongBi[OLED_MAX_Dis];	//??????????
}oledStruct;
extern char Time_Sys_One[11];	//OLED时间显示缓冲区
extern char Time_Sys_Two[9];
extern oledStruct oledStructInit;
extern u8 trigger1,trigger2 ,trigger3;	


void HexToChar(Time_Def *time);
void oledStartDis(void);
void oledDis(void);

#endif
