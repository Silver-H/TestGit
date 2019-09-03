/**
  ******************************************************************************
  * @file    rtc.c
  * @author  FAE
  * @version V1.0
  * @date    2018-04-10
  * @brief   i2c RTC应用函数
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//置为开漏，外部上拉。设置为推挽可能会有问题
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//置为开漏，外部上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 
}

/*********************************************
 * 函数名：I2Cdelay
 * 描  述：I2C延时函数
 * 输  入：无
 * 输  出：无
 ********************************************/
static void I2C1delay(void)
{	
   u8 i=WAITEDELAY; //这里可以优化速度，通常延时3~10us，可以用示波器看波形来调试
   while(i) 
   { 
     i--; 
   }
}

/*********************************************
 * 函数名：IIC_Init
 * 描  述：I2C端口初始化
 * 输  入：无
 * 输  出：无
 ********************************************/
void IIC_Init_OLED(void)
{
  /* GPIO configuration */
  GPIO_Configuration();
}

/*********************************************
 * 函数名：I2CStart
 * 描  述：开启I2C总线
 * 输  入：无
 * 输  出：TRUE:操作成功，FALSE:操作失败
 ********************************************/
static u8 I2C1Start(void)
{
    SDA1_H;
		I2C1delay();
    SCL1_H;
    I2C1delay();
    if(!SDA1_read)return FALSE;	//SDA线为低电平则总线忙,退出
    SDA1_L;
    I2C1delay();
    SCL1_L;
    I2C1delay();
    return TRUE;
}

/*********************************************
 * 函数名：I2CStop
 * 描  述：释放I2C总线
 * 输  入：无
 * 输  出：无
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
 * 函数名：I2CAck
 * 描  述：发送ASK
 * 输  入：无
 * 输  出：无
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
 * 函数名：I2CNoAck
 * 描  述：发送NOASK
 * 输  入：无
 * 输  出：无
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
 * 函数名：I2CWaitAck
 * 描  述：读取ACK信号
 * 输  入：无
 * 输  出：TRUE=有ACK,FALSE=无ACK
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
 * 函数名：I2CSendByte
 * 描  述：MCU发送一个字节
 * 输  入：无
 * 输  出：无
 ********************************************/
static void I2C1SendByte(u8 SendByte) //数据从高位到低位
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
 * 函数名：I2CReceiveByte
 * 描  述：MCU读入一个字节
 * 输  入：无
 * 输  出：ReceiveByte
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
    I2C1SendByte(0x78);//OLED地址
    I2C1WaitAck();
    I2C1SendByte(0x00);//寄存器地址
    I2C1WaitAck();
    I2C1SendByte(command);
    I2C1WaitAck();
    I2C1Stop();
}
void WriteDat(u8 data)
{
    I2C1Start();
    I2C1SendByte(0x78);//OLED地址
    I2C1WaitAck();
    I2C1SendByte(0x40);//寄存器地址
    I2C1WaitAck();
    I2C1SendByte(data);
    I2C1WaitAck();
    I2C1Stop();
}
/*********************************************END OF FILE**********************/
