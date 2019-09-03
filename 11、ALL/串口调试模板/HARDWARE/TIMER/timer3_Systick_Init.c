#include "timer3_Systick_Init.h"
#include "usart.h"
#include "key.h"
//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!

void TIM3_Systick_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 	
	TIM_ITConfig(TIM3,TIM_IT_Update ,ENABLE);  //使能或者失能指定的TIM中断
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设					 
}

//**************定义结构体类型的变量 TaskInit**************************//

u8 keyValue=0;	//按键扫描的键值
u8 keyFlag=0;
u8 exitFlag=0;
u8 TimeChagePageFlag =0;	//换页标志

//定义结构体变量TaskInit
TaskInitStruct TaskInit=RUNTIME;	  

struct {
	u8 keyScanFlag_10ms;
	u8 exitScanFlag_100ms;
	u16 ChagePage_1000ms;
	
	u8 reLoad_10ms;
	u8 reLoad_100ms;
	u16 reLoad_1000ms;
}TaskScanTime={5,100,1000, 5,100,1000};

void TIM3_IRQHandler(void)   //TIM3中断
{
	u8 i=0;

	u8 setnum = 0,addNum=0,reduceNum=0;
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
		
		TaskScanTime.keyScanFlag_10ms--;				//按键
		if(TaskScanTime.keyScanFlag_10ms==0)
		{
			TaskScanTime.keyScanFlag_10ms=TaskScanTime.reLoad_10ms;
			
			setnum = keySetScan();
			addNum = keyAddScan();
			reduceNum = keyReduceScan();
			if(setnum)
			{
				keyValue = setnum;
				keyFlag = 1;
			}
			else if(addNum)
			{
				keyValue = addNum;
				keyFlag = 1;
			}
			else if(reduceNum)
			{
				keyValue = reduceNum;
				keyFlag = 1;
			}
		}
		
		TaskScanTime.exitScanFlag_100ms--;			//外部中断去抖
		if(TaskScanTime.exitScanFlag_100ms==0)
		{
			TaskScanTime.exitScanFlag_100ms=TaskScanTime.reLoad_100ms;
			exitFlag=1;
		}
		TaskScanTime.ChagePage_1000ms--;			//页面切换
		if(TaskScanTime.ChagePage_1000ms==0)
		{
			TaskScanTime.ChagePage_1000ms=TaskScanTime.reLoad_1000ms;
			TimeChagePageFlag=1;
		}
		
		for(i=0;i<Task_MAX;i++)	//任务数
		{
			if(TaskInit.Timer[i])	//任务扫描时间不为0
			{
				TaskInit.Timer[i]--;
			}
			if(TaskInit.Timer[i]==0)
			{
				TaskInit.Timer[i]=TaskInit.ReLoadTime[i];
				TaskInit.Run[i]=1;	//任务运行标志
			}
		}
	}
}


