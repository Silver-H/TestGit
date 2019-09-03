#include "Get_ADC.h"
#include "adc.h"
#include "stdio.h"	//用于调用sprintf函数

#define Voltage_P 5		// 测量电阻两端电压与总电压的比值
#define Current_P 0.185	//   电流传感器为185mV/A

AdcStruct AdcStructInitC;	//定义电量数据的存储空间
AdcStruct AdcStructInitNC;	//定义电量数据的存储空间

/**
		函数名称：FloatToChar
		函数功能：将浮点数据转化成字符串数组
		入口参数：value---待转化的数据；
						 array---接收数组；
		返回值：无

		***原理***
		1、将浮点数的整数部分和小数部分分开
		2、分别对他们取余操作
		3、存入数组
*/
void FloatToChar(char* array,float value)
{
	u16 InterPart;
	float DecimalPart;
	u8 i=0,j=0,temp;
	
	//分离整数和小数部分
	if(value>=1)
	{
		InterPart=(u16)value;
		DecimalPart = value-InterPart;
	}
	else
	{
		InterPart=0;
		DecimalPart = value-InterPart;
	}
	
	//转化整数部分
	if(InterPart==0)
	{
		array[0] = 0+'0';
		array[1] = '.';
		i++;
	}
	else
	{
		while(InterPart>0)
		{
			array[i++] = InterPart%10+'0';	//从个位依次取
			InterPart = InterPart/10;
		}
		i--;	// i 代表存放数据的下标
		
		//修正转换顺序（首位存高位）
		for(j=0;j+1<=(i+1)/2;j++)
		{
			temp=array[i-j];
			array[j] = array[i-1];
			array[i-1] = temp;
		}
		i++;
		array[i] = '.';
	}
	
	//转化小数部分，最大保留三位
	i++;
	
	array[i++] = (u16)(DecimalPart*10)%10+'0';	
	array[i++] = (u16)(DecimalPart*100)%10+'0';	
	array[i++] = (u16)(DecimalPart*1000)%10+'0';	
//	array[i++] = (u16)(DecimalPart*10000)%10+'0';	
}
//获取adc的电压和电流
void adcGetDat(void)
{
		u16 adcx;
		float temp;
	
		adcx=Get_Adc_Average(ADC_Channel_0,5);
		temp=adcx*(3.3/4096)*Voltage_P;
		AdcStructInitC.Voltage_F = temp;
	
		adcx=Get_Adc_Average(ADC_Channel_1,5);
		temp=((temp*3.3/4096)-2.5)/Current_P;		//通道1的电流数据 Vout=2.5+0.185*IP(IP---电流值);
		AdcStructInitC.Current_F = temp;
		
		AdcStructInitC.Power_F=AdcStructInitC.Voltage_F * AdcStructInitC.Current_F;
	
		AdcStructInitC.batteryPercentage_F = AdcStructInitC.Power_F/AdcStructInitC.Power_set;
	//--------------------------二路	
		adcx=Get_Adc_Average(ADC_Channel_8,5);
		temp=adcx*(3.3/4096)*Voltage_P;
		AdcStructInitNC.Voltage_F = temp;
	
		adcx=Get_Adc_Average(ADC_Channel_9,5);
		temp=((temp*3.3/4096)-2.5)/Current_P;		//通道1的电流数据 Vout=2.5+0.185*IP(IP---电流值);
		AdcStructInitNC.Current_F = temp;
		
		AdcStructInitNC.Power_F=AdcStructInitNC.Voltage_F * AdcStructInitNC.Current_F;
		
		AdcStructInitNC.batteryPercentage_F = AdcStructInitNC.Power_F/AdcStructInitNC.Power_set;
	
//----------------------------------------------------------------------------------------
	 	FloatToChar(AdcStructInitC.Voltage_S,AdcStructInitC.Voltage_F);  //浮点数据转化为指定类型的字符串

		FloatToChar(AdcStructInitC.Current_S,AdcStructInitC.Current_F);  //浮点数据转化为指定类型的字符串
		
		FloatToChar(AdcStructInitC.Power_S,AdcStructInitC.Power_F);  //浮点数据转化为指定类型的字符串
		
		FloatToChar(AdcStructInitC.batteryPercentage_S,AdcStructInitC.batteryPercentage_F);
		//-------------------------二路
				
	 	FloatToChar(AdcStructInitNC.Voltage_S,AdcStructInitNC.Voltage_F);  //浮点数据转化为指定类型的字符串
	
		FloatToChar(AdcStructInitNC.Current_S,AdcStructInitNC.Current_F);  //浮点数据转化为指定类型的字符串
		
		FloatToChar(AdcStructInitNC.Power_S,AdcStructInitNC.Power_F);  //浮点数据转化为指定类型的字符串
		
		FloatToChar(AdcStructInitNC.batteryPercentage_S,AdcStructInitNC.batteryPercentage_F);
}
////获取adc的电压和电流
//void adcGetDat(void)
//{
//		u16 adcx;
//		float temp;
//	
//		adcx=Get_Adc_Average(ADC_Channel_0,5);
//		temp=adcx*(3.3/4096)*((float)Voltage_P);
//		AdcStructInitC.Voltage_F = temp;
//	
//		adcx=Get_Adc_Average(ADC_Channel_1,5);
//		temp=((temp*3.3/4096)-2.5)/Current_P;		//通道1的电流数据 Vout=2.5+0.185*IP(IP---电流值);
//		AdcStructInitC.Current_F = temp;
//		
//		AdcStructInitC.Power_F=AdcStructInitC.Voltage_F * AdcStructInitC.Current_F;
//	
//		AdcStructInitC.batteryPercentage_F = AdcStructInitC.Power_F/AdcStructInitC.Power_set;
//	//--------------------------二路	
//		adcx=Get_Adc_Average(ADC_Channel_8,5);
//		temp=adcx*(3.3/4096)*((float)Voltage_P);
//		AdcStructInitNC.Voltage_F = temp;
//	
//		adcx=Get_Adc_Average(ADC_Channel_9,5);
//		temp=((temp*3.3/4096)-2.5)/Current_P;		//通道1的电流数据 Vout=2.5+0.185*IP(IP---电流值);
//		AdcStructInitNC.Current_F = temp;
//		
//		AdcStructInitNC.Power_F=AdcStructInitNC.Voltage_F * AdcStructInitNC.Current_F;
//		
//		AdcStructInitNC.batteryPercentage_F = AdcStructInitNC.Power_F/AdcStructInitNC.Power_set;
//	
////----------------------------------------------------------------------------------------
//	 	FloatToChar(AdcStructInitC.Voltage_S,AdcStructInitC.Voltage_F);  //浮点数据转化为指定类型的字符串

//		FloatToChar(AdcStructInitC.Current_S,AdcStructInitC.Current_F);  //浮点数据转化为指定类型的字符串
//		
//		FloatToChar(AdcStructInitC.Power_S,AdcStructInitC.Power_F);  //浮点数据转化为指定类型的字符串
//		
//		FloatToChar(AdcStructInitC.batteryPercentage_S,AdcStructInitC.batteryPercentage_F);
//		//-------------------------二路
//				
//	 	FloatToChar(AdcStructInitNC.Voltage_S,AdcStructInitNC.Voltage_F);  //浮点数据转化为指定类型的字符串
//	
//		FloatToChar(AdcStructInitNC.Current_S,AdcStructInitNC.Current_F);  //浮点数据转化为指定类型的字符串
//		
//		FloatToChar(AdcStructInitNC.Power_S,AdcStructInitNC.Power_F);  //浮点数据转化为指定类型的字符串
//		
//		FloatToChar(AdcStructInitNC.batteryPercentage_S,AdcStructInitNC.batteryPercentage_F);
//}

//----------------------------END--------------------------
 

	
 
 
 
 
 













