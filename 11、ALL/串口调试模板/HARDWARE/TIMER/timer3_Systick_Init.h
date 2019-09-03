#ifndef __TIMER3_SYSTICK_INIT_H
#define __TIMER3_SYSTICK_INIT_H
#include "sys.h"

#define Task_MAX 5		//���������
#define RUNTIME {0,0,0,0,0, 500,600,2000,20,1000,   500,600,2000,20,1000}	//ʱ���־����λms��

typedef struct _TaskInitStruct		
{
	u8 Run[Task_MAX];		//���б�־
	u16 Timer[Task_MAX];	//ɨ��ʱ��
	u16 ReLoadTime[Task_MAX];//��װ��ʱ��
}TaskInitStruct;

extern TaskInitStruct TaskInit;		//�����ṹ��
extern void TIM3_Systick_Init(u16 arr,u16 psc);
extern u8 keyValue;
extern u8 keyFlag;
extern u8 TimeChagePageFlag;	//��ҳ��־

#endif
