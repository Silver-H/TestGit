#ifndef __TIMER3_SYSTICK_INIT_H
#define __TIMER3_SYSTICK_INIT_H
#include "sys.h"

#define Task_MAX 5		//最大任务数
#define RUNTIME {0,0,0,0,0, 500,600,2000,20,1000,   500,600,2000,20,1000}	//时间标志，单位ms。

typedef struct _TaskInitStruct		
{
	u8 Run[Task_MAX];		//运行标志
	u16 Timer[Task_MAX];	//扫描时间
	u16 ReLoadTime[Task_MAX];//重装载时间
}TaskInitStruct;

extern TaskInitStruct TaskInit;		//声明结构体
extern void TIM3_Systick_Init(u16 arr,u16 psc);
extern u8 keyValue;
extern u8 keyFlag;
extern u8 TimeChagePageFlag;	//换页标志

#endif
