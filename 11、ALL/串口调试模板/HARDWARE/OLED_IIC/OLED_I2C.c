/************************************************************************************
*  Copyright (c), 2014, HelTec Automatic Technology co.,LTD.
*            All rights reserved.
*
* Http:    www.heltec.cn
* Email:   cn.heltec@gmail.com
* WebShop: heltec.taobao.com
*
* File name: OLED_I2C.c
* Project  : HelTec.uvprij
* Processor: STM32F103C8T6
* Compiler : MDK fo ARM
* 
* Author : 小林
* Version: 1.00
* Date   : 2014.4.8
* Email  : hello14blog@gmail.com
* Modification: none
* 
* Description:128*64点阵的OLED显示屏驱动文件，仅适用于惠特自动化(heltec.taobao.com)的SD1306驱动IIC通信方式显示屏
*
* Others: none;
*
* Function List:
*	1. void I2C_Configuration(void) -- 配置CPU的硬件I2C
* 2. void I2C_WriteByte(uint8_t addr,uint8_t data) -- 向寄存器地址写一个byte的数据
* 3. void WriteCmd(unsigned char I2C_Command) -- 写命令
* 4. void WriteDat(unsigned char I2C_Data) -- 写数据
* 5. void OLED_Init(void) -- OLED屏初始化
* 6. void OLED_SetPos(unsigned char x, unsigned char y) -- 设置起始点坐标
* 7. void OLED_Fill(unsigned char fill_Data) -- 全屏填充
* 8. void OLED_CLS(void) -- 清屏
* 9. void OLED_ON(void) -- 唤醒
* 10. void OLED_OFF(void) -- 睡眠
* 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize) -- 显示字符串(字体大小有6*8和8*16两种)
* 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) -- 显示中文(中文需要先取模，然后放到codetab.h中)
* 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]) -- BMP图片
*
* History: none;
*
*************************************************************************************/

#include "OLED_I2C.h"
#include "delay.h"
#include "codetab.h"

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

////IIC1驱动,模拟方式
//void IIC_Init(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructer;
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//    GPIO_InitStructer.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7; //6--SCL   7--SDA
//    GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
//    GPIO_InitStructer.GPIO_Mode=GPIO_Mode_Out_PP;

//    GPIO_Init(GPIOB, &GPIO_InitStructer);
//}

///**
//**  设置SDA为输出
//**/
//void SDA_OUT(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructer;
//    GPIO_InitStructer.GPIO_Pin= GPIO_Pin_7;
//    GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
//    GPIO_InitStructer.GPIO_Mode=GPIO_Mode_Out_PP;
//    GPIO_Init(GPIOB, &GPIO_InitStructer);
//}


///**
//**  设置SDA为输入
//**/
//void SDA_IN(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructer;
//    GPIO_InitStructer.GPIO_Pin= GPIO_Pin_7;
//    GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
//    GPIO_InitStructer.GPIO_Mode=GPIO_Mode_IPU;
//    GPIO_Init(GPIOB, &GPIO_InitStructer);
//}

////开始信号
//void IIC_Start(void)
//{
//    SDA_OUT();
//    IIC_SDA=1;
//    IIC_SCL=1;
//    delay_us(2);
//    IIC_SDA=0;
//    delay_us(2);
//    IIC_SCL=0;
//    delay_us(2);
//}

//void IIC_Stop(void)
//{
//    IIC_SCL=1;
//    IIC_SDA=0;
//    delay_us(2);
//    IIC_SDA=1;
//    delay_us(2);
//}


///*
//*   返回1--应答出错
//*   放回0--应答正确
//*/
//u8 IIC_Wait_Ask(void)
//{
//    int count=0;
//    SDA_IN();
//    IIC_SCL=1;
//    delay_us(2);
//    while(READ_SDA)
//    {
//        count++;
//        if(count>250)
//        {
//            IIC_Stop();
//            return 1;
//        }   
//    }
//    IIC_SCL=0;
//    delay_us(2);
//    return 0;
//}

////写一个字节
//void IIC_WriteByte(u8 data)
//{
//    u8 i;
//    SDA_OUT();
//    for(i=0;i<8;i++)
//    {
//        IIC_SCL=0;
//        delay_us(2);
//        if(data & 0x80)     //MSB,从高位开始一位一位传输
//            IIC_SDA=1;
//        else
//            IIC_SDA=0;
//        IIC_SCL=1;
//        delay_us(2);
//        IIC_SCL=0;
//        data<<=1;

//    }
//}


//u8 IIC_ReadByte(void)
//{
//    u8 data,i;
//    IIC_SDA=1;
//    delay_us(2);
//    for(i=0;i<8;i++)
//    {
//        data<<=1;
//        IIC_SCL=0;
//        delay_us(2);
//        IIC_SCL=1;
//        delay_us(2);
//        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7))
//            data=data | 0x01;
//        else 
//            data=data & 0xFE;

//    }
//    IIC_SCL=0;
//    delay_us(2);
//    return data;

//}


//void WriteCmd(u8 command)
//{
//    I2CStart();
//    IIC_WriteByte(0x78);//OLED地址
//    IIC_Wait_Ask();
//    IIC_WriteByte(0x00);//寄存器地址
//    IIC_Wait_Ask();
//    IIC_WriteByte(command);
//    IIC_Wait_Ask();
//    IIC_Stop();
//}


//void WriteDat(u8 data)
//{
//    IIC_Start();
//    IIC_WriteByte(0x78);//OLED地址
//    IIC_Wait_Ask();
//    IIC_WriteByte(0x40);//寄存器地址
//    IIC_Wait_Ask();
//    IIC_WriteByte(data);
//    IIC_Wait_Ask();
//    IIC_Stop();
//}

//void I2C_Configuration(void)
//{
//	I2C_InitTypeDef  I2C_InitStructure;
//	GPIO_InitTypeDef  GPIO_InitStructure; 

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

//	/*STM32F103C8T6芯片的硬件I2C1: PB6 -- SCL; PB7 -- SDA */
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//I2C必须开漏输出
//	GPIO_Init(GPIOB, &GPIO_InitStructure);

////	I2C_DeInit(I2C1);//使用I2C2
////	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
////	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
////	I2C_InitStructure.I2C_OwnAddress1 = 0x30;//主机的I2C地址,随便写的
////	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
////	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
////	I2C_InitStructure.I2C_ClockSpeed = 400000;//400K

////	I2C_Cmd(I2C1, ENABLE);
////	I2C_Init(I2C1, &I2C_InitStructure);
//}
//void I2C_WriteByte(uint8_t addr,uint8_t data)
//{
//  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
//	
//	I2C_GenerateSTART(I2C1, ENABLE);//开启I2C1
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

//	I2C_Send7bitAddress(I2C1, OLED_ADDRESS, I2C_Direction_Transmitter);//器件地址 -- 默认0x78
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));	
//	

//	I2C_SendData(I2C1, addr);//寄存器地址
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));	

//	I2C_SendData(I2C1, data);//发送数据
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));	
//	I2C_GenerateSTOP(I2C1, ENABLE);//关闭I2C1总线
//}
//#define  OverTime 60000
//u32 I2C_TimeOut;
//void I2C_WriteByte(uint8_t addr,uint8_t data)
//{
//	I2C_TimeOut=OverTime;
//  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
//	{
//		if(I2C_TimeOut--)
//		{
//			break;
//		}
//	}
//	
//	I2C_GenerateSTART(I2C1, ENABLE);//开启I2C1
//	I2C_TimeOut=OverTime;
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))/*EV5,主模式*/
//	{
//		if(I2C_TimeOut--)
//		{
//			break;
//		}
//	}
//	I2C_Send7bitAddress(I2C1, OLED_ADDRESS, I2C_Direction_Transmitter);//器件地址 -- 默认0x78
//	I2C_TimeOut=OverTime;
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
//	{
//		if(I2C_TimeOut--)
//		{
//			break;
//		}
//	}		

//	I2C_SendData(I2C1, addr);//寄存器地址
//	I2C_TimeOut=OverTime;
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
//	{
//		if(I2C_TimeOut--)
//		{
//			break;
//		}
//	}

//	I2C_SendData(I2C1, data);//发送数据
//	I2C_TimeOut=OverTime;
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
//	{
//		if(I2C_TimeOut--)
//		{
//			break;
//		}
//	}
//	I2C_GenerateSTOP(I2C1, ENABLE);//关闭I2C1总线
//}

//void WriteCmd(unsigned char I2C_Command)//写命令
//{
//	I2C_WriteByte(0x00, I2C_Command);
//}

//void WriteDat(unsigned char I2C_Data)//写数据
//{
//	I2C_WriteByte(0x40, I2C_Data);
//}
void I2C_Configuration()
{
	IIC_Init_OLED();
}
void OLED_Init(void)
{
	delay_ms(100); //这里的延时很重要
	
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //亮度调节 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
}

void OLED_SetPos(unsigned char x, unsigned char y) //设置起始点坐标
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

void OLED_Fill(unsigned char fill_Data)//全屏填充
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				WriteDat(fill_Data);
			}
	}
}

void OLED_CLS(void)//清屏
{
	OLED_Fill(0x00);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          : 
// Parameters     : none
// Description    : 将OLED从休眠中唤醒
//--------------------------------------------------------------
void OLED_ON(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X14);  //开启电荷泵
	WriteCmd(0XAF);  //OLED唤醒
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          : 
// Parameters     : none
// Description    : 让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X10);  //关闭电荷泵
	WriteCmd(0XAE);  //OLED休眠
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); ch[] -- 要显示的字符串; TextSize -- 字符大小(1:6*8 ; 2:8*16)
// Description    : 显示codetab.h中的ASCII字符,有6*8和8*16可选择
//--------------------------------------------------------------
void OLED_ShowStr(unsigned char x, unsigned char y,char ch[], unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		/*case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		*/
//		case 2:
//		{
//			while(ch[j] != '\0')
//			{
//				c = ch[j] - 32;
//				if(x > 128)
//				{
//					x = 0;
//					y++;
//				}
//				OLED_SetPos(x,y);
//				for(i=0;i<8;i++)
//					WriteDat(F8x8[c][i]);
//				x += 8;
//				j++;
//			}
//		}break;
		case 3:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); N:汉字在codetab.h中的索引
// Description    : 显示codetab.h中的汉字,16*16点阵
// Function				：写一个16*16的字。
//--------------------------------------------------------------
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int  adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)	//第一页
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)	//第二页
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
// Calls          : 
// Parameters     : x0,y0 -- 起始点坐标(x0:0~127, y0:0~7); x1,y1 -- 起点对角线(结束点)的坐标(x1:1~128,y1:1~8)
// Description    : 显示BMP位图
//--------------------------------------------------------------
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			WriteDat(BMP[j++]);
		}
	}
}
