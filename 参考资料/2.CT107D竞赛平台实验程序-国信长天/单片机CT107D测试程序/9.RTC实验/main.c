#include "reg52.h"
#include "ds1302.h"

#define SYSCLK 12000000l

sfr P4 		= 0xC0;
sfr IE2		= 0xAF;              
sfr AUXR    = 0x8E;          	  
sfr T2H     = 0xD6;               
sfr T2L     = 0xD7;               

sbit LED1 	= P4^1;
sbit LED2 	= P4^2;
sbit LED3 	= P4^3;
sbit LED4 	= P4^4;

volatile unsigned char x = 0;
bit flag = 0;
extern unsigned char nowTime[7];

void ISR_T2(void)  interrupt 12	//
{
	if(++x == 100){
		x = 0;	//0.01秒到
		flag = 1;
	}
	IE2 &= ~0x04;            
	IE2 |= 0x04;	
}

//主函数
void main(void)
{
	
	unsigned ss  = 0,mm  = 0,hh = 0;
//	unsigned char i;

	T2L = (65536-SYSCLK/1000) ;
	T2H = (65536-SYSCLK/1000)>>8; 

	AUXR |= 0x04;	//1T
	AUXR |= 0x10;
	
	IE2 |= 0x04;	//定时器2中断
	EA = 1;
	
	DS1302_Init();
	
	while(1){
		if(flag == 1){
			flag = 0;
			Get_Time();
			
			if(ss != ((nowTime[0]>>4)*10) + (nowTime[0]&0x0F)){				
				LED1 = ~LED1;
				ss = ((nowTime[0]>>4)*10) + (nowTime[0]&0x0F);
			}
			if(mm != ((nowTime[1]>>4)*10) + (nowTime[1]&0x0F)){
				LED2 = ~LED2;
				mm = ((nowTime[1]>>4)*10) + (nowTime[1]&0x0F);
			}
			if(hh != ((nowTime[2]>>4)*10) + (nowTime[2]&0x0F)){
				LED3 = ~LED3;
				hh = ((nowTime[2]>>4)*10) + (nowTime[2]&0x0F);
			}
			
		}
	}
}

