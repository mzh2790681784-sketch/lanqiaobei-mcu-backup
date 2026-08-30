#include "system.h"

hextobin led,buzzer;
u8 mode=1;
u8 canshu;
u8 count;
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

void system_int()
{
	drive(0,0xa0);
	led.hex=0xff;
	drive(led.hex,0x80);
	smg_int();
	smg_display();
}

u8 ad_count;
u8 ch3;
u16 ch3_05;
u16 ch3_05_temp;
void ad_process()
{
	if(ad_count>=100)
	{
		ch3_05_temp=ch3_05;
		ad_count=0;
		ch3=AD_read(0x43);
		
		ch3_05=ch3*100/51;
		if(ch3_05<=canshu*10&&ch3_05_temp>=canshu*10)
			count++;
	}
}

void smg_shuju()
{
	smg[0]=0xc1;
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=Seg_Table[ch3_05/100]&0x7f;
	smg[6]=Seg_Table[ch3_05/10%10];
	smg[7]=Seg_Table[ch3_05%10];
}
void smg_canshu()
{
	smg[0]=0x8c;
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=Seg_Table[canshu/10]&0x7f;
	smg[6]=Seg_Table[canshu%10];
	smg[7]=Seg_Table[0];
}
void smg_jishu()
{
	smg[0]=0xc8;
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=0xff;
	smg[6]=Seg_Table[count/10];
	smg[7]=Seg_Table[count%10];
}

u8 key16_count;
u8 haha;
void key16_process()
{
	if(key16_count>=10)
	{
		key16();
		key16_count=0;
		if(trg16==0x88||trg16==0x84||trg16==0x82||trg16==0x81||trg16==0x48||trg16==0x44||trg16==0x42||trg16==0x41||trg16==0x22||trg16==0x21||trg16==0x12||trg16==0x11)
		{
			haha++;
		}
		if(trg16==0x28)		//s12
		{
			led.b.b2=1;
			haha=0;
			mode++;
			if(mode==3)
				AT24C02_write(0,canshu);
			if(mode==4)
				mode=1;
		}
		if(trg16==0x24)		//s13
		{
			if(mode==1||mode==2)
				haha++;

			if(mode==3)
			{
				haha=0;
				led.b.b2=1;
			}
			if(mode==3)
				count=0;
		}
		if(trg16==0x18)		//s16
		{
			if(mode==1||mode==3)
				haha++;
			if(mode==2)
			{
				haha=0;
				led.b.b2=1;
			}
			if(mode==2)
				canshu+=5;
			if(canshu==55)
				canshu=0;
		}
		if(trg16==0x14)		//s17
		{
			if(mode==1||mode==3)
				haha++;
			if(mode==2)
			{
				haha=0;
				led.b.b2=1;
			}
			if(canshu==0)
				canshu=55;
			if(mode==2)
				canshu-=5;
		}
	}
}
u8 led_count;
u16 time;
void led_process()
{
	if(led_count>=10)
	{
		led_count=0;
		if(ch3_05<canshu*10)
			time++;
		else
		{
			led.b.b0=1;
			time=0;
		}
		if(time>=500)
			led.b.b0=0;
		if(time>=1000)
			time=550;
		
		if((count%2)!=0||count==0)
			led.b.b1=0;
		else
			led.b.b1=1;
		
		if(haha>=3)
		{
			led.b.b2=0;
			haha=3;
		}
		drive(led.hex,0x80);
	}
}
void main()
{
	system_int();
	canshu=AT24CO2_read(0);
	Timer2_Init();
	AD_read(0x43);
	ch3=AD_read(0x43);
	ch3_05=ch3*100/51;
	while(1)
	{
		key16_process();
		ad_process();
		
		if(mode==1)
		{
			smg_shuju();
		}
		if(mode==2)
		{
			smg_canshu();
		}
		if(mode==3)
		{
			smg_jishu();
		}
		
	}
}

void Timer2_Isr(void) interrupt 12
{
	key16_count++;
	ad_count++;
	led_count++;
	
	smg_display();
	led_process();
}