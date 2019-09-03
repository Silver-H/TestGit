#include "Get_ADC.h"
#include "adc.h"
#include "stdio.h"	//���ڵ���sprintf����

#define Voltage_P 5		// �����������˵�ѹ���ܵ�ѹ�ı�ֵ
#define Current_P 0.185	//   ����������Ϊ185mV/A

AdcStruct AdcStructInitC;	//����������ݵĴ洢�ռ�
AdcStruct AdcStructInitNC;	//����������ݵĴ洢�ռ�

/**
		�������ƣ�FloatToChar
		�������ܣ�����������ת�����ַ�������
		��ڲ�����value---��ת�������ݣ�
						 array---�������飻
		����ֵ����

		***ԭ��***
		1�������������������ֺ�С�����ַֿ�
		2���ֱ������ȡ�����
		3����������
*/
void FloatToChar(char* array,float value)
{
	u16 InterPart;
	float DecimalPart;
	u8 i=0,j=0,temp;
	
	//����������С������
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
	
	//ת����������
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
			array[i++] = InterPart%10+'0';	//�Ӹ�λ����ȡ
			InterPart = InterPart/10;
		}
		i--;	// i ���������ݵ��±�
		
		//����ת��˳����λ���λ��
		for(j=0;j+1<=(i+1)/2;j++)
		{
			temp=array[i-j];
			array[j] = array[i-1];
			array[i-1] = temp;
		}
		i++;
		array[i] = '.';
	}
	
	//ת��С�����֣��������λ
	i++;
	
	array[i++] = (u16)(DecimalPart*10)%10+'0';	
	array[i++] = (u16)(DecimalPart*100)%10+'0';	
	array[i++] = (u16)(DecimalPart*1000)%10+'0';	
//	array[i++] = (u16)(DecimalPart*10000)%10+'0';	
}
//��ȡadc�ĵ�ѹ�͵���
void adcGetDat(void)
{
		u16 adcx;
		float temp;
	
		adcx=Get_Adc_Average(ADC_Channel_0,5);
		temp=adcx*(3.3/4096)*Voltage_P;
		AdcStructInitC.Voltage_F = temp;
	
		adcx=Get_Adc_Average(ADC_Channel_1,5);
		temp=((temp*3.3/4096)-2.5)/Current_P;		//ͨ��1�ĵ������� Vout=2.5+0.185*IP(IP---����ֵ);
		AdcStructInitC.Current_F = temp;
		
		AdcStructInitC.Power_F=AdcStructInitC.Voltage_F * AdcStructInitC.Current_F;
	
		AdcStructInitC.batteryPercentage_F = AdcStructInitC.Power_F/AdcStructInitC.Power_set;
	//--------------------------��·	
		adcx=Get_Adc_Average(ADC_Channel_8,5);
		temp=adcx*(3.3/4096)*Voltage_P;
		AdcStructInitNC.Voltage_F = temp;
	
		adcx=Get_Adc_Average(ADC_Channel_9,5);
		temp=((temp*3.3/4096)-2.5)/Current_P;		//ͨ��1�ĵ������� Vout=2.5+0.185*IP(IP---����ֵ);
		AdcStructInitNC.Current_F = temp;
		
		AdcStructInitNC.Power_F=AdcStructInitNC.Voltage_F * AdcStructInitNC.Current_F;
		
		AdcStructInitNC.batteryPercentage_F = AdcStructInitNC.Power_F/AdcStructInitNC.Power_set;
	
//----------------------------------------------------------------------------------------
	 	FloatToChar(AdcStructInitC.Voltage_S,AdcStructInitC.Voltage_F);  //��������ת��Ϊָ�����͵��ַ���

		FloatToChar(AdcStructInitC.Current_S,AdcStructInitC.Current_F);  //��������ת��Ϊָ�����͵��ַ���
		
		FloatToChar(AdcStructInitC.Power_S,AdcStructInitC.Power_F);  //��������ת��Ϊָ�����͵��ַ���
		
		FloatToChar(AdcStructInitC.batteryPercentage_S,AdcStructInitC.batteryPercentage_F);
		//-------------------------��·
				
	 	FloatToChar(AdcStructInitNC.Voltage_S,AdcStructInitNC.Voltage_F);  //��������ת��Ϊָ�����͵��ַ���
	
		FloatToChar(AdcStructInitNC.Current_S,AdcStructInitNC.Current_F);  //��������ת��Ϊָ�����͵��ַ���
		
		FloatToChar(AdcStructInitNC.Power_S,AdcStructInitNC.Power_F);  //��������ת��Ϊָ�����͵��ַ���
		
		FloatToChar(AdcStructInitNC.batteryPercentage_S,AdcStructInitNC.batteryPercentage_F);
}
////��ȡadc�ĵ�ѹ�͵���
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
//		temp=((temp*3.3/4096)-2.5)/Current_P;		//ͨ��1�ĵ������� Vout=2.5+0.185*IP(IP---����ֵ);
//		AdcStructInitC.Current_F = temp;
//		
//		AdcStructInitC.Power_F=AdcStructInitC.Voltage_F * AdcStructInitC.Current_F;
//	
//		AdcStructInitC.batteryPercentage_F = AdcStructInitC.Power_F/AdcStructInitC.Power_set;
//	//--------------------------��·	
//		adcx=Get_Adc_Average(ADC_Channel_8,5);
//		temp=adcx*(3.3/4096)*((float)Voltage_P);
//		AdcStructInitNC.Voltage_F = temp;
//	
//		adcx=Get_Adc_Average(ADC_Channel_9,5);
//		temp=((temp*3.3/4096)-2.5)/Current_P;		//ͨ��1�ĵ������� Vout=2.5+0.185*IP(IP---����ֵ);
//		AdcStructInitNC.Current_F = temp;
//		
//		AdcStructInitNC.Power_F=AdcStructInitNC.Voltage_F * AdcStructInitNC.Current_F;
//		
//		AdcStructInitNC.batteryPercentage_F = AdcStructInitNC.Power_F/AdcStructInitNC.Power_set;
//	
////----------------------------------------------------------------------------------------
//	 	FloatToChar(AdcStructInitC.Voltage_S,AdcStructInitC.Voltage_F);  //��������ת��Ϊָ�����͵��ַ���

//		FloatToChar(AdcStructInitC.Current_S,AdcStructInitC.Current_F);  //��������ת��Ϊָ�����͵��ַ���
//		
//		FloatToChar(AdcStructInitC.Power_S,AdcStructInitC.Power_F);  //��������ת��Ϊָ�����͵��ַ���
//		
//		FloatToChar(AdcStructInitC.batteryPercentage_S,AdcStructInitC.batteryPercentage_F);
//		//-------------------------��·
//				
//	 	FloatToChar(AdcStructInitNC.Voltage_S,AdcStructInitNC.Voltage_F);  //��������ת��Ϊָ�����͵��ַ���
//	
//		FloatToChar(AdcStructInitNC.Current_S,AdcStructInitNC.Current_F);  //��������ת��Ϊָ�����͵��ַ���
//		
//		FloatToChar(AdcStructInitNC.Power_S,AdcStructInitNC.Power_F);  //��������ת��Ϊָ�����͵��ַ���
//		
//		FloatToChar(AdcStructInitNC.batteryPercentage_S,AdcStructInitNC.batteryPercentage_F);
//}

//----------------------------END--------------------------
 

	
 
 
 
 
 













