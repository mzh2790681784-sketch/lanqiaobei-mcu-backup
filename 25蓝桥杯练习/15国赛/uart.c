#include "system.h"

void Uart1_Init(void)	//9600bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器时钟1T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xC7;			//设置定时初始值
	TH1 = 0xFE;			//设置定时初始值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
	ES=1;
	EA=1;
}
u8 busy,ck_flag;
u8 ck_time,ck_count;
u8 ck_string[20];
extern u8 mudidi_flag;
void Uart() interrupt 4
{
    if (RI)
    {
        RI = 0;                 //清除RI位
		ck_time=0;
		ck_string[ck_count++]=SBUF;
		if(ck_string[ck_count-1]==')')
		{
			ck_flag=1;
			mudidi_flag=1;
			ck_count=0;
		}
		else if(ck_string[ck_count-1]=='?')
		{
			ck_flag=2;
			ck_count=0;
		}
		else if(ck_string[ck_count-1]=='#')
		{
			ck_flag=3;
			ck_count=0;
		}
		
    }
    if (TI)
    {
        TI = 0;                 //清除TI位
        busy = 0;               //清忙标志
    }
}

/*----------------------------
发送串口数据
----------------------------*/
void SendData(u8 dat)
{
    while (busy);               //等待前面的数据发送完成
    busy = 1;
    SBUF = dat;                 //写数据到UART数据寄存器
}

/*----------------------------
发送字符串
----------------------------*/
void SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        SendData(*s++);         //发送当前字符
    }
}

void ck_relax()
{
	ck_time++;
	if(ck_time>=50)
	{
		ck_time=0;
		if(ck_count!=0)
			ck_flag=4;
		ck_count=0;
		memset(ck_string,'\0',sizeof(ck_string));
	}
}


