#ifndef __GET_ADC_H
#define __GET_ADC_H	
#include "sys.h"
 
typedef struct 
{
	int Power_set;
	float batteryPercentage_F;
	char Voltage_S[10];	//��ѹ
	char Current_S[10];	//����
	
	char Power_S[10];	//����

//	char Energy_S[10];	//����
	
	char batteryPercentage_S[6];	//�����ٷֱ� Pʵ/P����
	
	float Voltage_F;	//��ѹ
	float Current_F;	//����
	float Power_F;	//����
}AdcStruct;

extern AdcStruct AdcStructInitC;
extern AdcStruct AdcStructInitNC;

void adcGetDat(void);
void FloatToChar(char* array,float value);
#endif 


