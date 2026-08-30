#include <reg52.h>
#include <intrins.h>
#include "ds1302.h"

sbit SCK = P2^7;		
sbit SDA = P2^6;		
sbit RST = P2^5;   // DS1302∏¥Œª
												

unsigned char nowTime[7];//√Î∑÷ ±13:30:00
code unsigned char r_add[] = {0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};
/**
  * @brief  Send a byte to the DS1302.
  * @param  temp: value to write to DS1302.
  * @retval None
  */
void Write_Ds1302_Byte(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK=0;
		SDA=temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}  
 
/**
  * @brief  Writes to the selected DS1302 register.
  * @param  address: address of the selected register.
  * @param  dat: value to write to the selected register.
  * @retval None
  */
void Write_Ds1302( unsigned char address,unsigned char dat )     
{
 	RST=0;
	_nop_();
 	SCK=0;
	_nop_();
 	RST=1;	
   	_nop_();  
 	Write_Ds1302_Byte(address);	
 	Write_Ds1302_Byte(dat);		
 	RST=0; 
}

/**
  * @brief  Reads the selected DS1302 Register.
  * @param  address: address of the selected register.
  * @retval DS1302 Register Value.
  */
unsigned char Read_Ds1302 ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;
	_nop_();
 	SCK=0;
	_nop_();
 	RST=1;
	_nop_();
 	Write_Ds1302_Byte(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(SDA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;
	_nop_();
 	RST=0;
	SCK=0;
	_nop_();
	SCK=1;
	_nop_();
	SDA=0;
	_nop_();
	SDA=1;
	_nop_();
	return (temp);			
}

void DS1302_Init(void)
{
	Write_Ds1302(REG_PROTECT,0x00); 
	Write_Ds1302(REG_SECOND,0);
	Write_Ds1302(REG_MINUTES,0);
	Write_Ds1302(REG_HOUR,0);
	Write_Ds1302(REG_PROTECT,0x80);	
}

void Get_Time(void)
{
	unsigned char i;
	EA = 0;
	for(i=0;i<7;i++){
		nowTime[i] = Read_Ds1302(r_add[i]);
	}
	EA = 1;
}
