#include "exti.h"
#include "bsp_spi_flash.h"
#include "operation_flash.h"
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//�ⲿ�ж� ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/01  
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved	  
////////////////////////////////////////////////////////////////////////////////// 	  
 
 
//�ⲿ�жϳ�ʼ������
void EXTIX_Init(void)
{
		 //����IO��ʼ���ṹ��   
    GPIO_InitTypeDef GPIO_InitStructure; 
		//�ⲿ�жϽṹ��
 	  EXTI_InitTypeDef EXTI_InitStructure;
		//�ж����ȼ�
 	  NVIC_InitTypeDef NVIC_InitStructure;
		//��ʼ��GPIO���жϽ�ʱ��
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;           //����Ϊ����   
    GPIO_Init(GPIOA, &GPIO_InitStructure);                          //GPIOB��ʼ��   
    //�����ж�Դ
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
		//�����½��ش���
		EXTI_ClearITPendingBit(EXTI_Line4);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�����ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//��ռ���ȼ�1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
		
//		 //EXTI_Line1�ж�����   
//    EXTI_GenerateSWInterrupt(EXTI_Line12);  
}

//��ȡ��ID�洢λ��
extern __IO uint32_t DeviceID;
extern __IO uint32_t FlashID;
extern u8 exitFlag;
//��⵽�쳣��MCU�ͽ����������д�뵽Flash

void EXTI4_IRQHandler(void)
{
//	if(EXTI_GetITStatus(EXTI_Line12) != RESET)
//	{
		if(exitFlag)	//�ӳ�100ms������
		{
			exitFlag = 0;
			if (FlashID == sFLASH_ID) 
			{
				printf("\r\n��⵽SPI FLASH W25Q128 !\r\n");
				MCU_FlashWrite();
			}
			else
			{
				printf("\r\nû�м�⵽SPI FLASH W25Q128 !\r\n");
			}
		} 
		EXTI_ClearITPendingBit(EXTI_Line4);  //���LINE12��·����λ
//	}
	
}
