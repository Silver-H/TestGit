#ifndef __PWM_MPPT_H
#define __PWM_MPPT_H

#include "sys.h"
#include "Get_ADC.h"

typedef struct
{
	u32 Present_Power;
	u32 Last_Power;
	u32 Present_Voltage;
	u32 Last_Voltage;
	
	u16 Present_Dk;	//��ʼռ�ձ�Ϊ50��0~65535��
	u16 Next_Dk;			//��һ����ռ�ձ�
	
	char Present_Dk_S[10];
}Power_Max_Compare_Struct;
extern Power_Max_Compare_Struct Power_Max_Compare_Struct_Init;

void Power_Max_Compare(AdcStruct	dat);

#endif
