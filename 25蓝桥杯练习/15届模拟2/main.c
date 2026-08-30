#include "system.h"

hextobin led;

u8 mode1=0,mode2=0;

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
		
		if(trg16==0x88)			//s4
		{
			if(mode1==0)
			{
				mode2++;
				if(mode2==2)
					mode2=0;
			}
			
		}
		if(trg16==0x84)			//s5
		{
			mode1++;
			if(mode1==2)
				mode1=0;
		}
	}
}

u8 temp_count;
u32 temp;
void temp_process()
{
	if(temp_count>=100)
	{
		temp_count=0;
		temp=(u32)(temp_read()*10000);
	}
}

u8 ch1;
u8 ad_count;
void ad_process()
{
	if(ad_count>=10)
	{
		ad_count=0;
		ch1=ad_read(0x41);
		
	}
}

u8 da_count;
u8 da_date;
void da_process()
{
	if(da_count>=2)
	{
		da_count=0;
		if(mode2==0)
		{
			if((temp/10000)<=10)
				da_date=51;
			else if(temp/10000<40)
				da_date=6.8*temp/10000-17;
			else
				da_date=255;
		}
		else if(mode2==1)
		{
			if(ch1<=10)
				da_date=51;
			else if(ch1<240)
				da_date=0.887*ch1+42.13;
			else
				da_date=255;
		}
		da_write(da_date);
	}
}
void smg_moshi()
{
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	
	if(mode2==0)
	{
		smg[0]=Seg_Table[1];
		
		smg[5]=Seg_Table[temp/100000];
		smg[6]=Seg_Table[temp/10000%10]&0x7f;
		smg[7]=Seg_Table[temp/1000%10];
	}
	else if(mode2==1)
	{
		smg[0]=Seg_Table[2];
		
		if(ch1>=100)
		{
			smg[5]=Seg_Table[ch1/100];
			smg[6]=Seg_Table[ch1/10%10];
			smg[7]=Seg_Table[ch1%10];
		}
		else if(ch1>=10)
		{
			smg[5]=0xff;
			smg[6]=Seg_Table[ch1/10];
			smg[7]=Seg_Table[ch1%10];
		}
		else if(ch1>=0)
		{
			smg[5]=0xff;
			smg[6]=0xff;
			smg[7]=Seg_Table[ch1];
		}
	}
}

void smg_shuchu()
{
	smg[0]=0xc1;
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=0xff;
	smg[6]=Seg_Table[(da_date*10/51)/10]&0x7f;
	smg[7]=Seg_Table[(da_date*10/51)%10];
}
void smg_process()
{
	if(mode1==0)
		smg_moshi();
	else if(mode1==1)
		smg_shuchu();

}
u8 led_count;
void led_process()
{
	if(led_count>=50)
	{
		led_count=0;
		if(mode2==0)
			led.b.b0=0;
		else
			led.b.b0=1;
		
		if(mode2==1)
			led.b.b1=0;
		else
			led.b.b1=1;
		
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
		temp_process();
		ad_process();
		da_process();
		led_process();
		smg_process();
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
	
}