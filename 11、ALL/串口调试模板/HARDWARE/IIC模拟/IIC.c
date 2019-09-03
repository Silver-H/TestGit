/**
  ******************************************************************************
  * @file    rtc.c
  * @author  FAE
  * @version V1.0
  * @date    2018-04-10
  * @brief   i2c RTCӦ�ú���
  ******************************************************************************
  * @attention
  *
  * Website: http://www.whwave.com.cn
  * E-mail : fae@whwave.com.cn
  * Tel    : 0755-83114387
  *
  ******************************************************************************
  */ 

#include "IIC.h"


/* Private define ------------------------------------------------------------*/
#define WAITEDELAY  50

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configure the used I/O ports pin
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void GPIO_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
  /* Configure I2C1 pins: SCL and SDA */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//��Ϊ��©���ⲿ����������Ϊ������ܻ�������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//��Ϊ��©���ⲿ����
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 
}

/*********************************************
 * ��������I2Cdelay
 * ��  ����I2C��ʱ����
 * ��  �룺��
 * ��  ������
 ********************************************/
static void I2C1delay(void)
{	
   u8 i=WAITEDELAY; //��������Ż��ٶȣ�ͨ����ʱ3~10us��������ʾ����������������
   while(i) 
   { 
     i--; 
   }
}

/*********************************************
 * ��������IIC_Init
 * ��  ����I2C�˿ڳ�ʼ��
 * ��  �룺��
 * ��  ������
 ********************************************/
void IIC_Init_OLED(void)
{
  /* GPIO configuration */
  GPIO_Configuration();
}

/*********************************************
 * ��������I2CStart
 * ��  ��������I2C����
 * ��  �룺��
 * ��  ����TRUE:�����ɹ���FALSE:����ʧ��
 ********************************************/
static u8 I2C1Start(void)
{
    SDA1_H;
		I2C1delay();
    SCL1_H;
    I2C1delay();
    if(!SDA1_read)return FALSE;	//SDA��Ϊ�͵�ƽ������æ,�˳�
    SDA1_L;
    I2C1delay();
    SCL1_L;
    I2C1delay();
    return TRUE;
}

/*********************************************
 * ��������I2CStop
 * ��  �����ͷ�I2C����
 * ��  �룺��
 * ��  ������
 ********************************************/
static void I2C1Stop(void)
{
    SCL1_L;
    I2C1delay();
    SDA1_L;
    I2C1delay();
    SCL1_H;
    I2C1delay();
    SDA1_H;
    I2C1delay();
}

/*********************************************
 * ��������I2CAck
 * ��  ��������ASK
 * ��  �룺��
 * ��  ������
 ********************************************/
//static void I2CAck(void)
//{
//    SCL1_L;
//    I2Cdelay();
//    SDA1_L;
//    I2Cdelay();
//    SCL1_H;
//    I2Cdelay();
//    SCL1_L;
//    I2Cdelay();
//}

/*********************************************
 * ��������I2CNoAck
 * ��  ��������NOASK
 * ��  �룺��
 * ��  ������
 ********************************************/
//static void I2CNoAck(void)
//{
//    SCL1_L;
//    I2Cdelay();
//    SDA1_H;
//    I2Cdelay();
//    SCL1_H;
//    I2Cdelay();
//    SCL1_L;
//    I2Cdelay();
//}

/*********************************************
 * ��������I2CWaitAck
 * ��  ������ȡACK�ź�
 * ��  �룺��
 * ��  ����TRUE=��ACK,FALSE=��ACK
 ********************************************/
static u8 I2C1WaitAck(void)
{
    SCL1_L;
    I2C1delay();
    SDA1_H;			
    I2C1delay();
    SCL1_H;
    I2C1delay();
    if(SDA1_read)
    {
			SCL1_L;
			return FALSE;
    }
    SCL1_L;
    return TRUE;
}

/*********************************************
 * ��������I2CSendByte
 * ��  ����MCU����һ���ֽ�
 * ��  �룺��
 * ��  ������
 ********************************************/
static void I2C1SendByte(u8 SendByte) //���ݴӸ�λ����λ
{
    u8 i=8;
	
		while(i--)
		{
			SCL1_L;
			I2C1delay();
			if(SendByte&0x80)
			SDA1_H;  
			else 
			SDA1_L;   
			SendByte<<=1;
			I2C1delay();
			SCL1_H;
			I2C1delay();
		}
    SCL1_L;
}

/*********************************************
 * ��������I2CReceiveByte
 * ��  ����MCU����һ���ֽ�
 * ��  �룺��
 * ��  ����ReceiveByte
 ********************************************/
//static u8 I2CReceiveByte(void)
//{
//    u8 i=8;
//    u8 ReceiveByte=0;
//		
//    SDA1_H;				
//    while(i--)
//    {
//      ReceiveByte<<=1;         
//      SCL1_L;
//      I2Cdelay();
//      SCL1_H;
//      I2Cdelay();	
//      if(SDA1_read)
//      {
//        ReceiveByte|=0x1;
//      }
//    }
//    SCL1_L;
//    return ReceiveByte;   
//}
void WriteCmd(u8 command)
{
    I2C1Start();
    I2C1SendByte(0x78);//OLED��ַ
    I2C1WaitAck();
    I2C1SendByte(0x00);//�Ĵ�����ַ
    I2C1WaitAck();
    I2C1SendByte(command);
    I2C1WaitAck();
    I2C1Stop();
}
void WriteDat(u8 data)
{
    I2C1Start();
    I2C1SendByte(0x78);//OLED��ַ
    I2C1WaitAck();
    I2C1SendByte(0x40);//�Ĵ�����ַ
    I2C1WaitAck();
    I2C1SendByte(data);
    I2C1WaitAck();
    I2C1Stop();
}
/*********************************************END OF FILE**********************/
