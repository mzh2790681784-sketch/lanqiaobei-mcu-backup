#include "system.h"

void Timer2_Init(void)		//100微秒@11.0592MHz
{
	AUXR |= 0x04;			//定时器时钟1T模式
	T2L = 0xAE;				//设置定时初始值
	T2H = 0xFB;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时
	IE2 |= 0x04;			//使能定时器2中断
	EA=1;
}

void Counter0_Init(void)
{
	TMOD|=0x05;
	TL0=0x00;
	TH0=0x00;			
	TR0=1;
}

