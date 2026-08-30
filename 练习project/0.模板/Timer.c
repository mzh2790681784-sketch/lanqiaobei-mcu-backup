#include "system.h"

void Timer2_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xFB;			//定时器时钟12T模式
	T2L = 0x18;				//设置定时初始值
	T2H = 0xFC;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时
	IE2 |= 0x04;			//使能定时器2中断
	EA=1;
}

void Counter0_Init(void)
{
	TMOD|=0x05;			//定时器0/计数器0配置成计数模式且不自动重载
	TL0=0x00;
	TH0=0x00;			
	TR0=1;				//启用计数器
}

