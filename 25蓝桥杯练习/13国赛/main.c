#include "system.h"

hextobin led;
u8 canshu_mode=1;

void counter0_init()
{
	TMOD|=0X05;
	TH0=TL0=0;
	TR0=1;
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
void system_init()
{
	led.hex=0xff;
	drive(led.hex,0x80);
	drive(0,0xa0);
	smg_init();
	smg_display();
}

u8 csb_count;
u16 csb_dat;
void csb_process()
{
	if(csb_count>=100)
	{
		csb_count=0;
		csb_dat=csb_get();
	}
}

u16 ne555_count;
u16 ne555_dat;
void ne555_process()
{
	if(ne555_count>=1000)
	{
		ne555_count=0;
		ne555_dat=(TH0<<8)|TL0;
		TH0=TL0=0;
	}
}

u8 ad_count;
u8 ch3;
u8 shidu;
void ad_process()
{
	if(ad_count>=10)
	{
		ad_count=0;
		ch3=ad_read(0x43);
		shidu=ch3*20/51;
	}
}

u8 key4_count;
void key4_process()
{
	if(key4_count>=10)
	{
		key4_count=0;
		key4();
		if(trg4&0x08)		//s4
		{
			
		}
		else if(trg4&0x04)		//s5
		{
			
		}
		else if(trg4&0x02)		//s6
		{
			
		}
		else if(trg4&0x01)		//s7
		{
			
		}
	}
}
void smg_pinlv()
{
	smg[0]=smg_set[15];
	smg[1]=0xff;
	smg[2]=0xff;
	if(ne555_dat>=10000)
	{
		smg[3]=smg_set[ne555_dat/10000];
		smg[4]=smg_set[ne555_dat/1000%10];
		smg[5]=smg_set[ne555_dat/100%10];
		smg[6]=smg_set[ne555_dat/10%10];
		smg[7]=smg_set[ne555_dat%10];
	}
	else if(ne555_dat>=1000)
	{
		smg[3]=0xff;
		smg[4]=smg_set[ne555_dat/1000];
		smg[5]=smg_set[ne555_dat/100%10];
		smg[6]=smg_set[ne555_dat/10%10];
		smg[7]=smg_set[ne555_dat%10];
	}
	else if(ne555_dat>=100)
	{
		smg[3]=0xff;
		smg[4]=0xff;
		smg[5]=smg_set[ne555_dat/100];
		smg[6]=smg_set[ne555_dat/10%10];
		smg[7]=smg_set[ne555_dat%10];
	}
	else if(ne555_dat>=10)
	{
		smg[3]=0xff;
		smg[4]=0xff;
		smg[5]=0xff;
		smg[6]=smg_set[ne555_dat/10];
		smg[7]=smg_set[ne555_dat%10];
	}
	else
	{
		smg[3]=0xff;
		smg[4]=0xff;
		smg[5]=0xff;
		smg[6]=0xff;
		smg[7]=smg_set[ne555_dat];
	}
}

void smg_shidu()
{
	smg[0]=0x89;
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=0xff;
	smg[6]=smg_set[shidu/10%10];
	smg[7]=smg_set[shidu%10];
}

void smg_ceju()
{
	smg[0]=0x88;	//1000 1000
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	if(csb_dat>=100)
	{
		smg[5]=smg_set[csb_dat/100];
		smg[6]=smg_set[csb_dat/10%10];
		smg[7]=smg_set[csb_dat%10];
	}
	else if(csb_dat>=10)
	{
		smg[5]=0xff;
		smg[6]=smg_set[csb_dat/10];
		smg[7]=smg_set[csb_dat%10];
	}
	else
	{
		smg[5]=0xff;
		smg[6]=0xff;
		smg[7]=smg_set[csb_dat];
	}

}
u8 pinlv_canshu,shidu_canshu;
void smg_canshu()
{
	smg[0]=0x8c;10001100
	smg[1]=smg_set[canshu_mode];
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	if(shidu_mode==1)
	{
		if(pinlv_canshu>=100)
		smg[5]=smg_set[pinlv_canshu];
		smg[6]=smg_set[shidu/10%10];
		smg[7]=smg_set[shidu%10];
	}
}

void main()
{
	counter0_init();
	Timer2_Init();
	system_init();
	while(1)
	{
		key4_process();
		csb_process();
		ad_process();
		
		smg_shidu();
	}
}

void Timer2_Isr(void) interrupt 12
{
	key4_count++;
	csb_count++;
	ne555_count++;
	ad_count++;
	
	ne555_process();
	smg_display();
}