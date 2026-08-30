#include <STC15F2K60S2.H>

typedef unsigned char u8;
typedef char s8;
typedef unsigned int u16;
typedef int s16;
typedef unsigned long u32;
typedef long s32;

typedef struct
{
	u8 b0:1;
	u8 b1:1;
	u8 b2:1;
	u8 b3:1;
	u8 b4:1;
	u8 b5:1;
	u8 b6:1;
	u8 b7:1;
}bits;

typedef union
{
	u8 hex;
	bits b;
}hextobin;

void Timer2_Init(void)		//1毫秒@12.000MHz
{
	
	AUXR &= 0xFB;			//定时器时钟12T模式
	T2L = 0x18;				//设置定时初始值
	T2H = 0xFC;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时
	IE2 |= 0x04;			//使能定时器2中断
    EA = 1;
}
hextobin buzzer,led;



void drive(u8 p0,u8 p2)
{
	P0=p0;
	P2=p2&0x1F|p2;
	P2=P2&0x1F;
}
void main()
{
	Timer2_Init();
	drive(0,0xa0);
	led.hex=0xff;
	drive(led.hex,0x80);
	while(1)
	{
		
	}
}


void Timer2_Isr() interrupt 12
{
	static u8 i;
	static u16 count1,count2,count3;
	count1++;
	count3++;
	if(count1>=500)
	{
		led.b.b0=~led.b.b0;
		drive(led.hex,0x80);
		count1=0;
		count2++;
		if(count2==10)
		{
			buzzer.b.b4=~buzzer.b.b4;
			drive(buzzer.hex,0xa0);
			count2=0;
		}
	}
	if(count3==100)
	{
		count3=0;
		if(i==0)	led.hex=(led.hex|0xf8)&0xf7;
		if(i==1)	led.hex=(led.hex|0xf8)&0xef;
		if(i==2)	led.hex=(led.hex|0xf8)&0xdf;
		if(i==3)	led.hex=(led.hex|0xf8)&0xbf;
		if(i==4)	led.hex=(led.hex|0xf8)&0x7f;
		
//		led.b.b3 = ~(0x01>>i);
//		led.b.b4 = ~(0x02>>i);
//		led.b.b5 = ~(0x04>>i);
//		led.b.b6 = ~(0x08>>i);
//		led.b.b7 = ~(0x10>>i);
		drive(led.hex,0x80);
		i++;
		if(i==5)
			i=0;
	}
}