#include "system.h"

hextobin led;

u8 mode1=0,mode2=0,baojing=0;
u8 xuanniu_shang=0,xuanniu_xia=0;
u8 xiaxian=10,shangxian=60;


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
			mode1++;
			if(mode1==3)
				mode1=0;
		}
		if(trg16==0x84)			//s5
		{
			if(mode1==1)
			{
				mode2++;
				if(mode2==0)
				{
					xuanniu_shang=0;
					xuanniu_xia=0;
				}
				if(mode2==2)
					mode2=0;
			}
			else if(mode1==2)
			{
				baojing=0;
			}	
		}
		if(trg16==0x48)			//s8
		{
			if(mode1==1&&mode2==0)
			{
				xiaxian+=10;
				if(xiaxian==50)
					xiaxian=0;
			}
			if(mode1==1&&mode2==1)
			{
				xuanniu_xia=!xuanniu_xia;
				xuanniu_shang=0;
			}
		}
		if(trg16==0x44)			//s9
		{
			if(mode1==1&&mode2==0)
			{
				shangxian+=10;
				if(shangxian==100)
					shangxian=50;
			}
			if(mode1==1&&mode2==1)
			{
				xuanniu_shang=!xuanniu_shang;
				xuanniu_xia=0;
			}
		}
	}
}
u8 ad_count;
u8 ch3;
u8 ch3_05;
void ad_process()
{
	if(ad_count>=10)
	{
		ad_count=0;
		ch3=ad_read(0x43);
		ch3_05=ch3/51;
		if(mode1==1&&mode2==1)
		{
			if(ch3_05>=4)
			{
				if(xuanniu_shang==1)
					shangxian=90;
				if(xuanniu_xia==1)
					xiaxian=40;
			}
			else if(ch3_05>=3)
			{
				if(xuanniu_shang==1)
					shangxian=80;
				if(xuanniu_xia==1)
					xiaxian=30;
			}
			else if(ch3_05>=2)
			{
				if(xuanniu_shang==1)
					shangxian=70;
				if(xuanniu_xia==1)
					xiaxian=20;
			}
			else if(ch3_05>=1)
			{
				if(xuanniu_shang==1)
					shangxian=60;
				if(xuanniu_xia==1)
					xiaxian=10;
			}
			else
			{
				if(xuanniu_shang==1)
					shangxian=50;
				if(xuanniu_xia==1)
					xiaxian=0;
			}
		}
	}
}

u8 csb_count;
u16 csb_date; 
u8 baojing_flag=0;
void csb_process()				//超声波测距
{
	if(csb_count>=100)			//100ms执行一次就行
	{
		csb_count=0;
		csb_date=csb_get();		//单位cm
		if((csb_date<xiaxian||csb_date>shangxian)&&baojing_flag==0)
		{
			baojing_flag=1;
			baojing++;
		}
		if(csb_date>=xiaxian&&csb_date<=shangxian)
			baojing_flag=0;
	}
}
void smg_ceju()
{
	smg[0]=0x88;
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	if(csb_date>=100)
	{
		smg[5]=Seg_Table[csb_date/100];
		smg[6]=Seg_Table[csb_date/10%10];
		smg[7]=Seg_Table[csb_date%10];
	}
	else if(csb_date>=10)
	{
		smg[5]=0xff;
		smg[6]=Seg_Table[csb_date/10];
		smg[7]=Seg_Table[csb_date%10];
	}
	else if(csb_date>=0)
	{
		smg[5]=0xff;
		smg[6]=0xff;
		smg[7]=Seg_Table[csb_date];
	}
	else
	{
		smg[5]=0xff;
		smg[6]=0xff;
		smg[7]=0xff;
	}
}

void smg_canshu()
{
	smg[0]=0x8c;
	if(mode2==0)
		smg[1]=Seg_Table[1];
	else if(mode2==1)
		smg[1]=Seg_Table[2];
	smg[2]=0xff;
	smg[3]=Seg_Table[xiaxian/10];
	smg[4]=Seg_Table[xiaxian%10];
	smg[5]=0xbf;
	smg[6]=Seg_Table[shangxian/10];
	smg[7]=Seg_Table[shangxian%10];
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
	if(baojing<10)
		smg[7]=Seg_Table[baojing];
	else
		smg[7]=0xbf;
}

void smg_process()
{
	if(mode1==0)
		smg_ceju();
	else if(mode1==1)
		smg_canshu();
	else if(mode1==2)
		smg_jilu();
}
u8 led_count;
void led_process()
{
	if(led_count>=100)
	{
		led_count=0;
		
		if(mode1==0)
			led.b.b0=0;
		else
			led.b.b0=1;
		
		if(mode1==1)
			led.b.b1=0;
		else
			led.b.b1=1;
		
		if(mode1==2)
			led.b.b2=0;
		else
			led.b.b2=1;
		
		if(csb_date>=xiaxian&&csb_date<=shangxian)
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
		csb_process();
		ad_process();
		smg_process();
		led_process();
	}
}

void Timer2_Isr(void) interrupt 12
{
	key16_count++;
	csb_count++;
	led_count++;
	ad_count++;
	
	smg_display();
}