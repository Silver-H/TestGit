#ifndef __GET_ADC_H
#define __GET_ADC_H	
#include "sys.h"
 
typedef struct 
{
	int Power_set;
	float batteryPercentage_F;
	char Voltage_S[10];	//电压
	char Current_S[10];	//电流
	
	char Power_S[10];	//功率

//	char Energy_S[10];	//电能
	
	char batteryPercentage_S[6];	//电量百分比 P实/P设置
	
	float Voltage_F;	//电压
	float Current_F;	//电流
	float Power_F;	//功率
}AdcStruct;

extern AdcStruct AdcStructInitC;
extern AdcStruct AdcStructInitNC;

void adcGetDat(void);
void FloatToChar(char* array,float value);
#endif 


