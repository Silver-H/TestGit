#ifndef _rs485_H
#define _rs485_H

#include "sys.h"

extern u8 USART2_RX_BUF[64];  //接收缓存，最大64字节
extern u8 USART2_RX_CNT;


//模式控制
#define RS485_TX_EN		PBout(13)	//485模式控制.0,接收;1,发送.
														 
void RS485_Init(u32 bound);
void TIM2_485_Init(void);
void UartRxMonitor(u8 ms); //串口接收监控
void UartDriver(void); //串口驱动函数void UartRead(u8 *buf, u8 len); //串口接收数据
void rs485_UartWrite(u8 *buf2 ,u8 len2);  //串口发送数据
u8 UartRead(u8 *buf, u8 len) ;

extern u8 _485Addr;	//设置_485Addr的默认地址
#endif

