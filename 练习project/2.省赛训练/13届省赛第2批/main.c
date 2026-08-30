#include "SYSTEM.H"

u8 mode1=1,mode2=1;
u8 time;
u8 flag1,flag2;
hextobin led,buzzer;
u8 canshushang1=45,canshuxia1=5;
u8 canshushang2=45,canshuxia2=5;
u16 csb_date;
u16 ch3_05;

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
u8 key4_count;
void key4_process()
{
	if(key4_count>=10)
	{
		key4_count=0;
		key4();
		if(trg4&0x08)	//s4
		{
			mode1++;
			if(mode1==4)
				mode1=1;
		}
		if(trg4&0x04)	//s5
		{
			if(mode1==3)
				mode2++;
			if(mode2==3)
				mode2=1;
		}
		if(trg4&0x02)	//s6
		{
			if(mode1==3&&mode2==1)
				canshushang1+=5;
			if(canshushang1==55)
				canshushang1=5;
			
			if(mode1==3&&mode2==2)
				canshuxia1+=5;
			if(canshuxia1==55)
				canshuxia1=5;
		}
		if(trg4&0x01)	//s7
		{
			if(mode1==3&&mode2==1)
				canshushang1-=5;
			if(canshushang1==0)
				canshushang1=50;
			
			if(mode1==3&&mode2==2)
				canshuxia1-=5;
			if(canshuxia1==0)
				canshuxia1=50;
		}
	}
	
}

void smg_dianya()
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
void smg_canshu1()
{
	smg[0]=0x8c;
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=Seg_Table[canshushang1/10]&0x7f;
	smg[4]=Seg_Table[canshushang1%10];
	smg[5]=0xff;
	smg[6]=Seg_Table[canshuxia1/10]&0x7f;
	smg[7]=Seg_Table[canshuxia1%10];
}
void smg_canshu2()
{
	smg[0]=0x8c;
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=Seg_Table[canshushang1/10]&0x7f;
	smg[4]=Seg_Table[canshushang1%10];
	smg[5]=0xff;
	smg[6]=Seg_Table[canshuxia1/10]&0x7f;
	smg[7]=Seg_Table[canshuxia1%10];
}
void smg_ceju1()
{
	smg[0]=0xc3;
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;

	if(csb_date<10)
	{
		smg[5]=0xff;
		smg[6]=0xff;
		smg[7]=Seg_Table[csb_date];
	}
	else if(csb_date<100)
	{
		smg[5]=0xff;
		smg[6]=Seg_Table[csb_date/10];
		smg[7]=Seg_Table[csb_date%10];
	}
	else if(csb_date<1000)
	{
		smg[5]=Seg_Table[csb_date/100];;
		smg[6]=Seg_Table[csb_date/10%10];
		smg[7]=Seg_Table[csb_date%10];
	}
}
void smg_ceju2()
{
	smg[0]=0xc3;
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=0x88;
	smg[6]=0x88;
	smg[7]=0x88;
}
u8 csb_count;
void csb_process()
{
	if(csb_count>=100)
	{
		csb_count=0;
		csb_date=CSB_get();
	}
}
u8 ad_count;
u8 ch3;
void ad_process()
{
	if(ad_count>=10)
	{
		ad_count=0;
		ch3=ad_read(0x43);
		
		ch3_05=ch3*100/51;
	}
}
u8 da_count;
void da_process()
{
	if(da_count>=2)
	{
		da_count=0;
		if(flag1==1)
		{
			if(csb_date<=20)
				da_write(51);
			else if(csb_date>=80)
				da_write(255);
			else
				da_write(3.4*csb_date-17);
		}
		else
			da_write(0);
	}
}
u8 led_count;
void led_process()
{
	if(led_count>=50)
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
		
		if(flag2==1)
		{
			led.b.b7=~led.b.b7;
			flag2=0;
		}
		
		drive(led.hex,0x80);
	}
}
void main()
{
	Timer2_Init();
	system_init();
	while(1)
	{
		key4_process();
		ad_process();
		da_process();
		csb_process();
		if(mode1==1)
		{
			if(ch3_05/10<canshushang2&&ch3_05/10>canshuxia2)
			{
				flag1=1;
			}
			else
			{
				flag1=0;
			}
			mode2=1;
			canshushang2=canshushang1;
			canshuxia2=canshuxia1;
			smg_dianya();
		}
		if(mode1==2)
		{
			if(ch3_05/10<canshushang2&&ch3_05/10>canshuxia2)
			{
				flag1=1;
				smg_ceju1();
			}
			else
			{
				flag1=0;
				time=0;
				smg_ceju2();
			}
		}
		if(mode1==3)
		{
			if(mode2==1)
			{
				smg_canshu1();
			}
			if(mode2==2)
			{
				smg_canshu2();
			}
		}
	}
}

void Timer2_Isr(void) interrupt 12
{
	key4_count++;
	csb_count++;
	ad_count++;
	da_count++;
	led_count++;
	
	if(flag1==1)
	{
		time++;
		if(time>=100)
		{
			time=0;
			flag2=1;
		}
	}
	else
	{
		flag2=0;
		led.b.b7=1;
	}
	
	smg_display();
	led_process();
}