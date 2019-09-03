 /**
  ******************************************************************************
  * @file    spi_flash_application.c
  * @author  
  * @version 
  * @date    
  * @brief   
	  一个扇区一天；
	  写数据时，先擦除待写区域的扇区
	  
	  16个扇区一个块；
	  两个块一个月；
	  12个月24个块；
  数据存储格式：扇区号（sectorAddr）+时间（OLED显示的字符数据）+发电量百分比
  当前扇区：					第0页               第一二页			             第三页
  ******************************************************************************/
  
#include "operation_flash.h"

#include "bsp_spi_flash.h"
#include "rtc.h"
#include "OLED_Display.h"
#include "Get_ADC.h"

/*将十六进制数转为十进制*/
static u8 HexToDec(u8 dat)
{
	u8 temp;

	temp= (dat/16*10)+(dat%16);

	return temp;
}
 /**
  * @brief    向flash中写入数据
  * @param  ：
  			：	
  * @retval 每页最多256个字节 16页
  */

void MCU_FlashWrite(void)
{
	u32 writeSectorAddr;	//待写入扇区
	u8 month=1,day=1;
	u8 sectorAddr_Table[3];
	/*
		通过时间来判断待擦除的扇区
	*/
	month=HexToDec(0x02);		//每个月占两个块
	day=HexToDec(0x02);			//每天占一个扇区
	month=HexToDec(sysTime.month);		//每个月占两个块
	day=HexToDec(sysTime.day);			//每天占一个扇区
	writeSectorAddr = (16*2*(month-1)+(day-1))*4096;  //擦除的扇区地址
	
	sectorAddr_Table[0] = ((writeSectorAddr & 0xFF0000) >> 16);	
	sectorAddr_Table[1] = (writeSectorAddr & 0xFF00) >> 8;	//扇区号高位
	sectorAddr_Table[2] = writeSectorAddr & 0xFF;				//扇区号低位
	/*擦除扇区*/
  SPI_FLASH_SectorErase(writeSectorAddr);	//擦除待写扇区
	printf("擦除OK！！！\n");
	//第0页写入扇区号
	SPI_FLASH_PageWrite(sectorAddr_Table,writeSectorAddr+0,sizeof(sectorAddr_Table));
	//第一、二页写入时间
	SPI_FLASH_PageWrite((u8*) Time_Sys_One,writeSectorAddr+256*1,sizeof(Time_Sys_One));
	SPI_FLASH_PageWrite((u8*)Time_Sys_Two,writeSectorAddr+256*2,sizeof(Time_Sys_Two));
	
	//第三页写入电量百分比(字符型数据，保留三位小数：0.333)
	SPI_FLASH_PageWrite((u8*)AdcStructInitC.batteryPercentage_S,writeSectorAddr+256*3,sizeof(AdcStructInitC.batteryPercentage_S));
	//第四页写入电量百分比(字符型数据，保留三位小数：0.333)
	SPI_FLASH_PageWrite((u8*)AdcStructInitNC.batteryPercentage_S,writeSectorAddr+256*4,sizeof(AdcStructInitNC.batteryPercentage_S));
}
/**
 * @brief	 查询指定扇区的数据
 * @param  ：SectorQueryAddr---待读取扇区
		   ： 
 * @retval 无
 */
 
mcuReadFlashStruct mcuReadFlashStructInit={48,48,58,48,48,58,48,48,32,48};

u8 GetDatFull = FALSE;

void MCU_FlashRead(u8 readMonth,u8 readDay)
{
	u32 readSectorAddr;	//待读扇区
	u8 readSectorAddr_Table[3];
	u8 k=0;
	
	readSectorAddr = (16*2*(readMonth-1)+(readDay-1))*4096; 

	readSectorAddr_Table[0] = ((readSectorAddr & 0xFF0000) >> 16);	
	readSectorAddr_Table[1] = (readSectorAddr & 0xFF00) >> 8;	//扇区号中位
	readSectorAddr_Table[2] = readSectorAddr & 0xFF;				//扇区号低位
	/*读取第0页数据标志位*/
  SPI_FLASH_BufferRead(mcuReadFlashStructInit.sectorAddrOfZeroPage, readSectorAddr+256*0, sizeof(mcuReadFlashStructInit.sectorAddrOfZeroPage));

	if((mcuReadFlashStructInit.sectorAddrOfZeroPage[0] == readSectorAddr_Table[0])
		&&(mcuReadFlashStructInit.sectorAddrOfZeroPage[1] == readSectorAddr_Table[1])
	&&(mcuReadFlashStructInit.sectorAddrOfZeroPage[2] == readSectorAddr_Table[2]))
	{
		 printf("\r\n检测到数据标志\r\n");
		 /*读取第一页数据*/
        SPI_FLASH_BufferRead((u8*)mcuReadFlashStructInit.timeOfOnePage, readSectorAddr+256*1,11);
				
		 /*读取第二页数据*/
        SPI_FLASH_BufferRead((u8*)mcuReadFlashStructInit.timeOfTwoPage, readSectorAddr+256*2,9);
	
		 /*读取第三页数据*/
        SPI_FLASH_BufferRead((u8*)mcuReadFlashStructInit.batteryPercentageClearOfThreePage,  readSectorAddr+256*3,6);

		 /*读取第四页数据*/
        SPI_FLASH_BufferRead((u8*)mcuReadFlashStructInit.batteryPercentageNoClearOfFourPage,  readSectorAddr+256*4,6);
		 
		printf("\r\n从芯片读到数据：\r\n");	
		printf("数据标志0\r\n");
		
		k=0;
    while(mcuReadFlashStructInit.sectorAddrOfZeroPage[k] != '\0')
		{
			printf("%d ",mcuReadFlashStructInit.sectorAddrOfZeroPage[k]);	
			k++; 
		}	
		printf("\r\n");	
		printf("时间1\r\n");			
		
		k=0;
    while(mcuReadFlashStructInit.timeOfOnePage[k] != '\0')
		{
			printf("%c",mcuReadFlashStructInit.timeOfOnePage[k]);	
			k++;
		}
		printf("\r\n");	
		printf("时间2\r\n");	
		k=0;
		while(mcuReadFlashStructInit.timeOfTwoPage[k] != '\0')
		{
			printf("%c ",mcuReadFlashStructInit.timeOfTwoPage[k]);	
			k++;			
		}
		printf("\r\n");	
		printf("电量3\r\n");	
		
		k=0;
		while(mcuReadFlashStructInit.batteryPercentageClearOfThreePage[k] != '\0')
		{
			printf("%c ",mcuReadFlashStructInit.batteryPercentageClearOfThreePage[k]);		
			k++;
		}
		printf("\r\n");	
		printf("电量4\r\n");
		
		k=0;
		while(mcuReadFlashStructInit.batteryPercentageNoClearOfFourPage[k] != '\0')
		{
			printf("%c ",mcuReadFlashStructInit.batteryPercentageNoClearOfFourPage[k]);		
			k++;
		}
	}
	else
	{
		GetDatFull = TRUE;
		for( k=0; k<3; k++ )
		{
			printf("数据标志 = %d \r\n",mcuReadFlashStructInit.sectorAddrOfZeroPage[k]);		
		}		
    for( k=0; k<10; k++ )
		{
			printf("1 = %d \r\n",mcuReadFlashStructInit.timeOfOnePage[k]);		
		}
		for( k=0; k<8; k++ )
		{
			printf("2 = %d \r\n",mcuReadFlashStructInit.timeOfTwoPage[k]);		
		}
		for( k=0; k<5; k++ )
		{
			printf("3 = %d \r\n",mcuReadFlashStructInit.batteryPercentageClearOfThreePage[k]);		
		}
		printf("\r\n没有检测到数据标志\r\n");
	}
	
}
/*********************************************END OF FILE**********************/

 
