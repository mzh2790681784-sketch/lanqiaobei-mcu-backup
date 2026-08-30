#include "system.h"

hextobin led,buzzer;
u8 cishu;
u8 flag3,flag4;
u8 flag1,flag2;
u8 mode1=1,mode2=1;
u8 canshu1=60,canshu2=10;
u8 canshu11=60,canshu12=60,canshu21=10,canshu22=10;
u8 CSB_count;
u16 CSB_date; 

void CSB_process()				//超声波测距
{
	if(CSB_count>=100)			//100ms执行一次就行
	{
		CSB_count=0;
		CSB_date=CSB_get();		//单位cm
		
			flag2=flag1;
		if(CSB_date<=canshu1&&CSB_date>=canshu2)
			flag1=1;
		else
			flag1=0;
		if(flag1==0&&flag2==1)
			cishu++;
	}
}

u8 ad_count;
u8 ch3;
u16 ch3_05;
void ad_process()
{
	if(ad_count>=10)
	{
		ad_count=0;
		ch3=AD_read(0x43);
		
		ch3_05=ch3/51.1;
		if(flag3==1)
			canshu12=50+ch3_05*10;
		if(flag4==1)
			canshu22=ch3_05*10;
	}
}

void Timer2_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xFB;			//定时器时钟12T模式
	T2L = 0x18;				//设置定时初始值
	T2H = 0xFC;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时
	IE2 |= 0x04;			//使能定时器2中断
	EA=1;
}
void smg_init()
{
	smg[0]=0xff;
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=0xff;
	smg[6]=0xff;
	smg[7]=0xff;
}
void smg_ceju()
{
	smg[0]=0x88;
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	if(CSB_date<10)
	{
		smg[5]=0xff;
		smg[6]=0xff;
		smg[7]=Seg_Table[CSB_date];
	}
	else if(CSB_date<100)
	{
		smg[5]=0xff;
		smg[6]=Seg_Table[CSB_date/10];
		smg[7]=Seg_Table[CSB_date%10];
	}
	else if(CSB_date<1000)
	{
		smg[5]=Seg_Table[CSB_date/100];
		smg[6]=Seg_Table[CSB_date/10%10];
		smg[7]=Seg_Table[CSB_date%10];
	}
}
void smg_canshu()
{
	smg[0]=0x8c;
	smg[1]=Seg_Table[mode2];
	smg[2]=0xff;
	smg[3]=Seg_Table[canshu2/10];
	smg[4]=Seg_Table[canshu2%10];
	smg[5]=0xbf;
	smg[6]=Seg_Table[canshu1/10];
	smg[7]=Seg_Table[canshu1%10];
}
void smg_jilu()
{
	smg[0]=Seg_Table[14];
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=0xff;
	smg[6]=0xff;
	if(cishu<=9)
		smg[7]=Seg_Table[cishu];
	else
		smg[7]=0xbf;
}
void system_init()
{
	drive(0,0xa0);
	led.hex=0xff;
	drive(led.hex,0x80);
	smg_init();
}
u8 key16_count;
void key16_process()
{
	if(key16_count>=10)
	{
		key16_count=0;
		key16();
		if(trg16==0x88)		//s4
		{
			mode1++;
			if(mode1==4)
			{
				mode1=1;
			}
		}
		if(trg16==0x84)		//s5
		{
			if(mode1==2)
			{
				mode2++;
				if(mode2==3)
					mode2=1;
				if(mode2==2)
				{
					canshu12=canshu11;
					canshu22=canshu21;
				}
				if(mode2==1)
				{
					canshu11=canshu12;
					canshu21=canshu22;
				}
			}
			
			
			if(mode1==3)
				cishu=0;
		}
		if(trg16==0x44)		//s9
		{
			if(mode1==2&&mode2==1)
			{
				canshu11+=10;
				if(canshu11==100)
					canshu11=50;
			}
			if(mode1==2&&mode2==2)
			{
				flag3=1;
				flag4=0;
			}
		}
		if(trg16==0x48)		//s8
		{
			if(mode1==2&&mode2==1)
			{
				canshu21+=10;
				if(canshu21==50)
					canshu21=0;
			}
			if(mode1==2&&mode2==2)
			{
				flag3=0;
				flag4=1;
			}
		}
	}
}
u8 led_count;
void led_process()
{
	if(led_count>=100)
	{
		led_count=0;
		if(mode1==1)
			led.b.b0=0;
		else
			led.b.b0=1;
		
		if(mode1==2)
			led.b.b1=0;
		else
			led.b.b1=1;
		
		if(mode1==3)
			led.b.b2=0;
		else
			led.b.b2=1;
		
		if(flag1==1)
			led.b.b7=0;
		else
			led.b.b7=!led.b.b7;
		
		drive(led.hex,0x80);
	}
}
void main()
{	
	system_init();
	Timer2_Init();

	while(1)
	{
		key16_process();
		CSB_process();
		ad_process();
		if(mode1==1)
		{
			smg_ceju();
		}
		if(mode1==2)
		{
			if(mode2==1)
			{
				canshu1=canshu11;
				canshu2=canshu21;
			}
			if(mode2==2)
			{
				canshu1=canshu12;
				canshu2=canshu22;
			}
			smg_canshu();
		}
		if(mode1==3)
		{
			flag3=0;
			flag4=0;
			smg_jilu();
		}
	}
}

void Timer2_Isr(void) interrupt 12
{
	key16_count++;
	led_count++;
	ad_count++;
	CSB_count++;
	
	SMG_display();
	led_process();
}