#include "system.h"

hextobin led,buzzer;
u8 mode,flag;
u16 ch1_05,ch3_05;
u16 canshu1=250,canshu3=300;
u16 cs1=25,cs3=300;
void Timer2_Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x04;			//定时器时钟1T模式
	T2L = 0x20;				//设置定时初始值
	T2H = 0xD1;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时
	IE2 |= 0x04;			//使能定时器2中断
	EA=1;
}

void smg_int()
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
void smg_shuju()
{
	smg[0]=0xc1;
	if(flag==0)
	{
		smg[1]=Seg_Table[1];
		smg[5]=Seg_Table[ch1_05/100]&0x7f;
		smg[6]=Seg_Table[ch1_05/10%10];
		smg[7]=Seg_Table[ch1_05%10];
	}
	else
	{
		smg[1]=Seg_Table[3];
		smg[5]=Seg_Table[ch3_05/100]&0x7f;
		smg[6]=Seg_Table[ch3_05/10%10];
		smg[7]=Seg_Table[ch3_05%10];
	}
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
}
void smg_canshu()
{
	smg[0]=0x8c;
	if(flag==0)
	{
		smg[1]=Seg_Table[1];
		smg[5]=Seg_Table[canshu1/100]&0x7f;
		smg[6]=Seg_Table[canshu1/10%10];
		smg[7]=Seg_Table[canshu1%10];
	}
	else
	{
		smg[1]=Seg_Table[3];
		smg[5]=Seg_Table[canshu3/100]&0x7f;
		smg[6]=Seg_Table[canshu3/10%10];
		smg[7]=Seg_Table[canshu3%10];
	}
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
}
void system_int()
{
	drive(0,0xa0);
	led.hex=0xff;
	drive(led.hex,0x80);
	smg_int();
	smg_display();
}

u8 ad_count;
u8 ch1,ch3;
void ad_process()
{
	if(ad_count>=100)
	{
		ad_count=0;
		
		ad_read(0x41);
		ch1=ad_read(0x41);
		ad_read(0x43);
		ch3=ad_read(0x43);
		
		ch1_05=ch1*100/51;
		ch3_05=ch3*100/51;
	}
}
u8 key4_count;
void key4_process()
{
	if(key4_count>=10)
	{
		key4();
		key4_count=0;
		if(trg4&0x08)		//s4
		{
			flag=!flag;
		}
		if(trg4&0x04)		//s5
		{
			mode=!mode;
		}
		if(trg4&0x02)		//s6
		{
			if(mode==1)
			{
				if(flag==0)
					canshu1+=20;
				else
					canshu3+=20;
				if(canshu1==510)
					canshu1=490;
				if(canshu3==520)
					canshu3=500;
			}
		}
		if(trg4&0x01)		//s7
		{
			if(mode==1)
			{				
				if(canshu1==10)
					canshu1=30;
				if(canshu3==0)
					canshu3=20;
				if(flag==0)
					canshu1-=20;
				else
					canshu3-=20;

				
			}
		}
	}
}

u8 led_count;
void led_process()
{
	if(led_count>=10)
	{
		led_count=0;
		if(ch1_05>cs1)
			led.b.b0=0;
		else
			led.b.b0=1;
		
		if(ch3_05>cs3)
			led.b.b1=0;
		else
			led.b.b1=1;
		if(flag==0)
			led.b.b2=0;
		else
			led.b.b2=1;
		if(mode==0)
			led.b.b3=0;
		else
			led.b.b3=1;
		if(ch1>ch3)
			led.b.b4=0;
		else
			led.b.b4=1;
		
		drive(led.hex,0x80);
	}
}
void main()
{
	system_int();
	Timer2_Init();
	ad_read(0x41);
	ch1=ad_read(0x41);
	ad_read(0x43);
	ch3=ad_read(0x43);
		
	ch1_05=ch1*100/51;
	ch3_05=ch3*100/51;
	while(1)
	{
		key4_process();
		ad_process();
		
		if(mode==0)
		{
			cs1=canshu1;
			cs3=canshu3;
			smg_shuju();
		}
		else
		{
			smg_canshu();
		}
		
	}
}

void Timer2_Isr(void) interrupt 12
{
	key4_count++;
	ad_count++;
	led_count++;
	
	smg_display();
	led_process();
}