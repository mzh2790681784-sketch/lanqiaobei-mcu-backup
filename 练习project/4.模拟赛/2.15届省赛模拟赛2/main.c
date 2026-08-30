#include "system.h"

hextobin led,buzzer;
u8 da;
u16 smgda;
u32 temp;
u8 mode1=1,mode2=1;

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
void smg_wendu()
{
	smg[0]=Seg_Table[1];
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=Seg_Table[temp/100000];
	smg[6]=Seg_Table[temp/10000%10]&0x7f;
	smg[7]=Seg_Table[temp/1000%10];
}
u8 ad_count;
u8 ch1;
void ad_process()
{
	if(ad_count>=100)
	{
		ad_count=0;
		ch1=ad_read(0x41);
	}
}
void smg_guangzhao()
{
	smg[0]=Seg_Table[2];
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	if(ch1<10)
	{
		smg[5]=0xff;
		smg[6]=0xff;
		smg[7]=Seg_Table[ch1];
	}
	else if(ch1<100)
	{
		smg[5]=0xff;
		smg[6]=Seg_Table[ch1/10];
		smg[7]=Seg_Table[ch1%10];

	}
	else if(ch1<1000)
	{
		smg[5]=Seg_Table[ch1/100%10];
		smg[6]=Seg_Table[ch1/10%10];
		smg[7]=Seg_Table[ch1%10];
	}
}
u8 da_count;
void da_process()
{
	if(da_count>=100)
	{
		da_count=0;
		if(mode2==1)
		{
			if(temp/10000<=10)
				da=51;
			else if(temp/10000>=40)
				da=255;
			else 
				da=6.8*(temp/10000)-17;
				
		}
		if(mode2==2)
		{
			if(ch1<=10)
				da=51;
			else if(ch1>=240)
				da=255;
			else
				da=0.8869565*ch1+42.1304;
		}
		smgda=da*100/51;
		da_write(da);
	}
}
void smg_da()
{
	smg[0]=0xc1;
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=0xff;
	smg[6]=Seg_Table[smgda/100]&0x7f;
	smg[7]=Seg_Table[smgda/10%10];
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
			if(mode1==1)
				mode2++;
			if(mode2==3)
				mode2=1;
		}
		if(trg16==0x84)		//s5
		{
			mode1++;
			if(mode1==3)
				mode1=1;
		}
	}
}

u8 temp_count;
void temp_process()
{
	if(temp_count>=100)
	{
		temp_count=0;
		temp=(u32)(temp_read()*10000);
	}
}
u8 led_count;
void led_process()
{
	if(led_count>=50)
	{
		led_count=0;
		if(mode2==1)
			led.b.b0=0;
		else
			led.b.b0=1;
		if(mode2==2)
			led.b.b1=0;
		else
			led.b.b1=1;
		
		drive(led.hex,0x80);
	}
}
void main()
{
	while(temp_read()==85);
	temp=(u32)(temp_read()*10000);
	system_init();
	Timer2_Init();
	while(1)
	{
		key16_process();
		temp_process();
		ad_process();
		da_process();
		if(mode1==1)
		{
			if(mode2==1)
			{
				smg_wendu();
			}
			if(mode2==2)
			{
				smg_guangzhao();
			}
		}
		if(mode1==2)
		{
			smg_da();
		}
		
	}
}
void Timer2_Isr(void) interrupt 12
{
	key16_count++;
	temp_count++;
	ad_count++;
	da_count++;
	led_count++;
	
	smg_display();
	led_process();
}