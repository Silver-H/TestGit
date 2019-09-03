#include "timer4_EnergyAdd_Init.h"
#include "Get_ADC.h"
#include "usart.h"
//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!

//定时器4初始化，作为电能的时间
void TIM4_EnergyAdd_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 	
	TIM_ITConfig(TIM4,TIM_IT_Update ,ENABLE);  //使能或者失能指定的TIM中断
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设					 
}

u8 flagEnergyUnit=0;
//对电能单位的判断（W，KW）
void JudgeEnergyUnit()
{	
	if(flagEnergyUnit==W)	//单位为W
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


//对电能进行累加
void ElectricAdd()
{
	static u16 cnt=3600;	//时间计数器
	
	//1、更新PowerAdd_F值每秒
	JudgeEnergyUnit();		//判断单位
	if(flagEnergyUnit==W)
	{
		AdcStructInit.PowerAdd_F += AdcStructInit.Power_F;
	}
	else if(flagEnergyUnit==KW)
	{
		AdcStructInit.PowerAdd_F+=AdcStructInit.Power_F/1000;
	}
	
	//2、到1H将PowerAdd赋值给Energy
	cnt--;
	if(cnt==0)
	{
		AdcStructInit.Energy_F=AdcStructInit.PowerAdd_F;	//到1H就将PowerAdd中的值赋给Energy
		cnt=3600;
	}
}
	/***************** 每秒中断，对功率进行累加 *****************/	 
void TIM4_IRQHandler(void)	//TIM4中断，作为电能值的时间
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
		
		/*对电能数据进行累加*/
		ElectricAdd();
	
//		ElectricAdd(&Adc_Struct_Init); 
	}
}




