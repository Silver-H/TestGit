#include "pwm_MPPT.h"

/***********************************My Application**************************************************/
/************最大功率比较*************************************/

Power_Max_Compare_Struct Power_Max_Compare_Struct_Init={0,0,0,0,50,0};
#define D_Add 1  //D_Add值得增量

#define D_Min 1
#define D_Max 899
//限制占空比的最值
//min--最小值，max--最大值,
u16 Limit_D(u16 Compare)
{
	if(Compare<=D_Min)
	{
		Compare=D_Min;
	}
	else if(Compare>=D_Max)
	{
		Compare=D_Max;
	}
	else
	{
		//
	}
	return Compare;
}

void Power_Max_Compare(AdcStruct	dat)
{
	u16 temp;

	Power_Max_Compare_Struct_Init.Present_Voltage=dat.Voltage_F;	//P=UI;
	Power_Max_Compare_Struct_Init.Present_Power=(dat.Voltage_F*1000 * dat.Current_F*1000);	//P=UI;
	
	//寻找最大功率，计算占空比DK
	if(Power_Max_Compare_Struct_Init.Present_Power > Power_Max_Compare_Struct_Init.Last_Power)
	{
		if(Power_Max_Compare_Struct_Init.Present_Voltage > Power_Max_Compare_Struct_Init.Last_Voltage)	//UK>U(K-1)
		{
			temp = Power_Max_Compare_Struct_Init.Present_Dk+D_Add;
			Power_Max_Compare_Struct_Init.Next_Dk = Limit_D(temp);
		}
		else if(Power_Max_Compare_Struct_Init.Present_Voltage < Power_Max_Compare_Struct_Init.Last_Voltage)//UK<U(K-1)
		{
			temp = Power_Max_Compare_Struct_Init.Present_Dk-D_Add;
			Power_Max_Compare_Struct_Init.Next_Dk = Limit_D(temp);
		}
	}
	else if(Power_Max_Compare_Struct_Init.Present_Power < Power_Max_Compare_Struct_Init.Last_Power)
	{
		if(Power_Max_Compare_Struct_Init.Present_Voltage > Power_Max_Compare_Struct_Init.Last_Voltage)	//UK>U(K-1)
		{
			temp = Power_Max_Compare_Struct_Init.Present_Dk-D_Add;
			Power_Max_Compare_Struct_Init.Next_Dk = Limit_D(temp);
		}
		else if(Power_Max_Compare_Struct_Init.Present_Voltage < Power_Max_Compare_Struct_Init.Last_Voltage)//UK<U(K-1)
		{
			temp = Power_Max_Compare_Struct_Init.Present_Dk+D_Add;
			Power_Max_Compare_Struct_Init.Next_Dk = Limit_D(temp);
		}
	}
	//将Present存入Last，用于下次数据的计算
	
	Power_Max_Compare_Struct_Init.Last_Voltage=Power_Max_Compare_Struct_Init.Present_Voltage;	//P=UI;
	Power_Max_Compare_Struct_Init.Last_Power=Power_Max_Compare_Struct_Init.Present_Power;	//P=UI;
	Power_Max_Compare_Struct_Init.Present_Dk=Power_Max_Compare_Struct_Init.Next_Dk;
}

