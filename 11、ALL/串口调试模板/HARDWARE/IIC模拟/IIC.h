#ifndef __IIC_H__
#define __IIC_H__

#include "stm32f10x.h"

#define  SCL1_H         GPIOB->BSRR = GPIO_Pin_6 
#define  SCL1_L         GPIOB->BRR  = GPIO_Pin_6 

#define  SDA1_H         GPIOB->BSRR = GPIO_Pin_7
#define  SDA1_L         GPIOB->BRR  = GPIO_Pin_7

#define SDA1_read        (GPIOB->IDR  & GPIO_Pin_7)


#define		TRUE            1
#define		FALSE           0
/*******I2C���ֽ�������д����********/


/*********I2C�˿ڳ�ʼ������*********/
void IIC_Init_OLED(void);

/*********RTC�ж����ú���*********/
void WriteCmd(u8 command);
void WriteDat(u8 data);
#endif /* __RTC_H */
