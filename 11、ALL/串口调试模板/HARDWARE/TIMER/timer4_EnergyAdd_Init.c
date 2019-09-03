#include "timer4_EnergyAdd_Init.h"
#include "Get_ADC.h"
#include "usart.h"
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!

//��ʱ��4��ʼ������Ϊ���ܵ�ʱ��
void TIM4_EnergyAdd_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 	
	TIM_ITConfig(TIM4,TIM_IT_Update ,ENABLE);  //ʹ�ܻ���ʧ��ָ����TIM�ж�
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����					 
}

u8 flagEnergyUnit=0;
//�Ե��ܵ�λ���жϣ�W��KW��
void JudgeEnergyUnit()
{	
	if(flagEnergyUnit==W)	//��λΪW
	{
		if(AdcStructInit.PowerAdd_F/1000<0)
		{
			flagEnergyUnit= W;
		}
		else if(AdcStructInit.PowerAdd_F/1000>0)
		{
			flagEnergyUnit= KW;
			AdcStructInit.PowerAdd_F = AdcStructInit.PowerAdd_F/1000;
		}
	}
}


//�Ե��ܽ����ۼ�
void ElectricAdd()
{
	static u16 cnt=3600;	//ʱ�������
	
	//1������PowerAdd_Fֵÿ��
	JudgeEnergyUnit();		//�жϵ�λ
	if(flagEnergyUnit==W)
	{
		AdcStructInit.PowerAdd_F += AdcStructInit.Power_F;
	}
	else if(flagEnergyUnit==KW)
	{
		AdcStructInit.PowerAdd_F+=AdcStructInit.Power_F/1000;
	}
	
	//2����1H��PowerAdd��ֵ��Energy
	cnt--;
	if(cnt==0)
	{
		AdcStructInit.Energy_F=AdcStructInit.PowerAdd_F;	//��1H�ͽ�PowerAdd�е�ֵ����Energy
		cnt=3600;
	}
}
	/***************** ÿ���жϣ��Թ��ʽ����ۼ� *****************/	 
void TIM4_IRQHandler(void)	//TIM4�жϣ���Ϊ����ֵ��ʱ��
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		
		/*�Ե������ݽ����ۼ�*/
		ElectricAdd();
	
//		ElectricAdd(&Adc_Struct_Init); 
	}
}




