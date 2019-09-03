 /**
  ******************************************************************************
  * @file    spi_flash_application.c
  * @author  
  * @version 
  * @date    
  * @brief   
	  һ������һ�죻
	  д����ʱ���Ȳ�����д���������
	  
	  16������һ���飻
	  ������һ���£�
	  12����24���飻
  ���ݴ洢��ʽ�������ţ�sectorAddr��+ʱ�䣨OLED��ʾ���ַ����ݣ�+�������ٷֱ�
  ��ǰ������					��0ҳ               ��һ��ҳ			             ����ҳ
  ******************************************************************************/
  
#include "operation_flash.h"

#include "bsp_spi_flash.h"
#include "rtc.h"
#include "OLED_Display.h"
#include "Get_ADC.h"

/*��ʮ��������תΪʮ����*/
static u8 HexToDec(u8 dat)
{
	u8 temp;

	temp= (dat/16*10)+(dat%16);

	return temp;
}
 /**
  * @brief    ��flash��д������
  * @param  ��
  			��	
  * @retval ÿҳ���256���ֽ� 16ҳ
  */

void MCU_FlashWrite(void)
{
	u32 writeSectorAddr;	//��д������
	u8 month=1,day=1;
	u8 sectorAddr_Table[3];
	/*
		ͨ��ʱ�����жϴ�����������
	*/
	month=HexToDec(0x02);		//ÿ����ռ������
	day=HexToDec(0x02);			//ÿ��ռһ������
	month=HexToDec(sysTime.month);		//ÿ����ռ������
	day=HexToDec(sysTime.day);			//ÿ��ռһ������
	writeSectorAddr = (16*2*(month-1)+(day-1))*4096;  //������������ַ
	
	sectorAddr_Table[0] = ((writeSectorAddr & 0xFF0000) >> 16);	
	sectorAddr_Table[1] = (writeSectorAddr & 0xFF00) >> 8;	//�����Ÿ�λ
	sectorAddr_Table[2] = writeSectorAddr & 0xFF;				//�����ŵ�λ
	/*��������*/
  SPI_FLASH_SectorErase(writeSectorAddr);	//������д����
	printf("����OK������\n");
	//��0ҳд��������
	SPI_FLASH_PageWrite(sectorAddr_Table,writeSectorAddr+0,sizeof(sectorAddr_Table));
	//��һ����ҳд��ʱ��
	SPI_FLASH_PageWrite((u8*) Time_Sys_One,writeSectorAddr+256*1,sizeof(Time_Sys_One));
	SPI_FLASH_PageWrite((u8*)Time_Sys_Two,writeSectorAddr+256*2,sizeof(Time_Sys_Two));
	
	//����ҳд������ٷֱ�(�ַ������ݣ�������λС����0.333)
	SPI_FLASH_PageWrite((u8*)AdcStructInitC.batteryPercentage_S,writeSectorAddr+256*3,sizeof(AdcStructInitC.batteryPercentage_S));
	//����ҳд������ٷֱ�(�ַ������ݣ�������λС����0.333)
	SPI_FLASH_PageWrite((u8*)AdcStructInitNC.batteryPercentage_S,writeSectorAddr+256*4,sizeof(AdcStructInitNC.batteryPercentage_S));
}
/**
 * @brief	 ��ѯָ������������
 * @param  ��SectorQueryAddr---����ȡ����
		   �� 
 * @retval ��
 */
 
mcuReadFlashStruct mcuReadFlashStructInit={48,48,58,48,48,58,48,48,32,48};

u8 GetDatFull = FALSE;

void MCU_FlashRead(u8 readMonth,u8 readDay)
{
	u32 readSectorAddr;	//��������
	u8 readSectorAddr_Table[3];
	u8 k=0;
	
	readSectorAddr = (16*2*(readMonth-1)+(readDay-1))*4096; 

	readSectorAddr_Table[0] = ((readSectorAddr & 0xFF0000) >> 16);	
	readSectorAddr_Table[1] = (readSectorAddr & 0xFF00) >> 8;	//��������λ
	readSectorAddr_Table[2] = readSectorAddr & 0xFF;				//�����ŵ�λ
	/*��ȡ��0ҳ���ݱ�־λ*/
  SPI_FLASH_BufferRead(mcuReadFlashStructInit.sectorAddrOfZeroPage, readSectorAddr+256*0, sizeof(mcuReadFlashStructInit.sectorAddrOfZeroPage));

	if((mcuReadFlashStructInit.sectorAddrOfZeroPage[0] == readSectorAddr_Table[0])
		&&(mcuReadFlashStructInit.sectorAddrOfZeroPage[1] == readSectorAddr_Table[1])
	&&(mcuReadFlashStructInit.sectorAddrOfZeroPage[2] == readSectorAddr_Table[2]))
	{
		 printf("\r\n��⵽���ݱ�־\r\n");
		 /*��ȡ��һҳ����*/
        SPI_FLASH_BufferRead((u8*)mcuReadFlashStructInit.timeOfOnePage, readSectorAddr+256*1,11);
				
		 /*��ȡ�ڶ�ҳ����*/
        SPI_FLASH_BufferRead((u8*)mcuReadFlashStructInit.timeOfTwoPage, readSectorAddr+256*2,9);
	
		 /*��ȡ����ҳ����*/
        SPI_FLASH_BufferRead((u8*)mcuReadFlashStructInit.batteryPercentageClearOfThreePage,  readSectorAddr+256*3,6);

		 /*��ȡ����ҳ����*/
        SPI_FLASH_BufferRead((u8*)mcuReadFlashStructInit.batteryPercentageNoClearOfFourPage,  readSectorAddr+256*4,6);
		 
		printf("\r\n��оƬ�������ݣ�\r\n");	
		printf("���ݱ�־0\r\n");
		
		k=0;
    while(mcuReadFlashStructInit.sectorAddrOfZeroPage[k] != '\0')
		{
			printf("%d ",mcuReadFlashStructInit.sectorAddrOfZeroPage[k]);	
			k++; 
		}	
		printf("\r\n");	
		printf("ʱ��1\r\n");			
		
		k=0;
    while(mcuReadFlashStructInit.timeOfOnePage[k] != '\0')
		{
			printf("%c",mcuReadFlashStructInit.timeOfOnePage[k]);	
			k++;
		}
		printf("\r\n");	
		printf("ʱ��2\r\n");	
		k=0;
		while(mcuReadFlashStructInit.timeOfTwoPage[k] != '\0')
		{
			printf("%c ",mcuReadFlashStructInit.timeOfTwoPage[k]);	
			k++;			
		}
		printf("\r\n");	
		printf("����3\r\n");	
		
		k=0;
		while(mcuReadFlashStructInit.batteryPercentageClearOfThreePage[k] != '\0')
		{
			printf("%c ",mcuReadFlashStructInit.batteryPercentageClearOfThreePage[k]);		
			k++;
		}
		printf("\r\n");	
		printf("����4\r\n");
		
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
			printf("���ݱ�־ = %d \r\n",mcuReadFlashStructInit.sectorAddrOfZeroPage[k]);		
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
		printf("\r\nû�м�⵽���ݱ�־\r\n");
	}
	
}
/*********************************************END OF FILE**********************/

 
