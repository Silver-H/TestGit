#include "exti.h"
#include "bsp_spi_flash.h"
#include "operation_flash.h"
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//外部中断 驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/12/01  
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved	  
////////////////////////////////////////////////////////////////////////////////// 	  
 
 
//外部中断初始化函数
void EXTIX_Init(void)
{
		 //定义IO初始化结构体   
    GPIO_InitTypeDef GPIO_InitStructure; 
		//外部中断结构体
 	  EXTI_InitTypeDef EXTI_InitStructure;
		//中断优先级
 	  NVIC_InitTypeDef NVIC_InitStructure;
		//初始化GPIO、中断脚时钟
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;           //设置为输入   
    GPIO_Init(GPIOA, &GPIO_InitStructure);                          //GPIOB初始化   
    //配置中断源
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
		//配置下降沿触发
		EXTI_ClearITPendingBit(EXTI_Line4);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//抢占优先级1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
		
//		 //EXTI_Line1中断允许   
//    EXTI_GenerateSWInterrupt(EXTI_Line12);  
}

//读取的ID存储位置
extern __IO uint32_t DeviceID;
extern __IO uint32_t FlashID;
extern u8 exitFlag;
//检测到异常，MCU就将今天的数据写入到Flash

void EXTI4_IRQHandler(void)
{
//	if(EXTI_GetITStatus(EXTI_Line12) != RESET)
//	{
		if(exitFlag)	//延迟100ms，消抖
		{
			exitFlag = 0;
			if (FlashID == sFLASH_ID) 
			{
				printf("\r\n检测到SPI FLASH W25Q128 !\r\n");
				MCU_FlashWrite();
			}
			else
			{
				printf("\r\n没有检测到SPI FLASH W25Q128 !\r\n");
			}
		} 
		EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE12线路挂起位
//	}
	
}
