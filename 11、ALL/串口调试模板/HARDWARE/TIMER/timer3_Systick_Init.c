#include "timer3_Systick_Init.h"
#include "usart.h"
#include "key.h"
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!

void TIM3_Systick_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 	
	TIM_ITConfig(TIM3,TIM_IT_Update ,ENABLE);  //ʹ�ܻ���ʧ��ָ����TIM�ж�
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����					 
}

//**************����ṹ�����͵ı��� TaskInit**************************//

u8 keyValue=0;	//����ɨ��ļ�ֵ
u8 keyFlag=0;
u8 exitFlag=0;
u8 TimeChagePageFlag =0;	//��ҳ��־

//����ṹ�����TaskInit
TaskInitStruct TaskInit=RUNTIME;	  

struct {
	u8 keyScanFlag_10ms;
	u8 exitScanFlag_100ms;
	u16 ChagePage_1000ms;
	
	u8 reLoad_10ms;
	u8 reLoad_100ms;
	u16 reLoad_1000ms;
}TaskScanTime={5,100,1000, 5,100,1000};

void TIM3_IRQHandler(void)   //TIM3�ж�
{
	u8 i=0;

	u8 setnum = 0,addNum=0,reduceNum=0;
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		
		TaskScanTime.keyScanFlag_10ms--;				//����
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
		
		TaskScanTime.exitScanFlag_100ms--;			//�ⲿ�ж�ȥ��
		if(TaskScanTime.exitScanFlag_100ms==0)
		{
			TaskScanTime.exitScanFlag_100ms=TaskScanTime.reLoad_100ms;
			exitFlag=1;
		}
		TaskScanTime.ChagePage_1000ms--;			//ҳ���л�
		if(TaskScanTime.ChagePage_1000ms==0)
		{
			TaskScanTime.ChagePage_1000ms=TaskScanTime.reLoad_1000ms;
			TimeChagePageFlag=1;
		}
		
		for(i=0;i<Task_MAX;i++)	//������
		{
			if(TaskInit.Timer[i])	//����ɨ��ʱ�䲻Ϊ0
			{
				TaskInit.Timer[i]--;
			}
			if(TaskInit.Timer[i]==0)
			{
				TaskInit.Timer[i]=TaskInit.ReLoadTime[i];
				TaskInit.Run[i]=1;	//�������б�־
			}
		}
	}
}


