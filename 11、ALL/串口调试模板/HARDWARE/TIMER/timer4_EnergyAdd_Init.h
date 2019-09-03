#ifndef __TIMER4_ENERGYADD_INIT_H
#define __TIMER4_ENERGYADD_INIT_H
#include "sys.h"

//声明功率的单位
typedef enum  {
	W=0,
	KW=1,
}PowerUnit;

extern void TIM4_EnergyAdd_Init(u16 arr,u16 psc);

extern u8 flagEnergyUnit;

#endif
