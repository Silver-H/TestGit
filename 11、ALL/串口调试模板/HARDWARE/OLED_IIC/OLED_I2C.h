#ifndef __OLED_I2C_H
#define	__OLED_I2C_H

#include "stm32f10x.h"
#include "adc.h"
#include "pwm.h"

#define  SCL1_H         GPIOB->BSRR = GPIO_Pin_6 
#define  SCL1_L         GPIOB->BRR  = GPIO_Pin_6 

#define  SDA1_H         GPIOB->BSRR = GPIO_Pin_7
#define  SDA1_L         GPIOB->BRR  = GPIO_Pin_7

#define SDA1_read        (GPIOB->IDR  & GPIO_Pin_7)


#define		TRUE            1
#define		FALSE           0

#define OLED_ADDRESS	0x78 //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78

void I2C_Configuration(void);
void I2C_WriteByte(uint8_t addr,uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y,  char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);

#endif
