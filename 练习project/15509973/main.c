#include "system.h"
u8 mode1=1,mode2=1;
u32 chaoxian=2000;
u16 ne555_datemax;
u8 sec,min,hour;
u8 secmax,minmax,hourmax;
int jiaozhun;
hextobin led,buzzer;
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
	smg_display();
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
void conter0_init()
{
	TMOD|=0X05;
	TL0=0;
	TH0=0;
	TR0=1;
}
u16 ne555_count;
u16 date;
long ne555_date;
u8 flag;
void ne555_process()
{
	if(ne555_count>=1000)
	{
		ne555_count=0;
		date=(TH0<<8)|TL0;
		if(jiaozhun>=0)
		{
			flag=0;
			ne555_date=jiaozhun+date;
		}
		else if(jiaozhun<0)
		{
			if(date>=(-jiaozhun))
			{
				flag=0;
				ne555_date=jiaozhun+date;
			}
			else
				flag=1;
		}
		TH0=0;
		TL0=0;
		if(ne555_date>ne555_datemax)
		{
			ne555_datemax=ne555_date;
			secmax=sec;
			minmax=min;
			hourmax=hour;
		}
	}
}
void ds1302_process()
{
	sec = bcdtodec(Read_Ds1302_Byte(0x81));
	min = bcdtodec(Read_Ds1302_Byte(0x83));
	hour = bcdtodec(Read_Ds1302_Byte(0x85));
}

u8 da_count;
void da_process()
{
	if(da_count>=2)
	{
		da_count=0;
		if(flag==1)
			da_write(0);
		if(ne555_date>=0)
		{
			if(ne555_date<=500)
				da_write(51);
			else if(ne555_date>=chaoxian)
				da_write(255);
			else
			{
				long a=51+204*(ne555_date-500)/(chaoxian-500);
				da_write(a);
			}
		}
	}
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
			if(mode1==5)
				mode1=1;
		}
		if(trg16==0x84)		//s5
		{
			if(mode1==2||mode1==4)
				mode2++;
			if(mode2==3)
				mode2=1;
		}
		if(trg16==0x48)		//s8
		{
			if(mode1==2)
			{
				if(mode2==1)
				{
					chaoxian+=1000;
					if(chaoxian==10000)
						chaoxian=9000;
				}
				if(mode2==2)
				{
					jiaozhun+=100;
					if(jiaozhun==1000)
						jiaozhun=900;
				}
			}
		}
		if(trg16==0x44)		//s9
		{
			if(mode1==2)
			{
				if(mode2==1)
				{
					chaoxian-=1000;
					if(chaoxian==0)
						chaoxian=1000;
				}
				if(mode2==2)
				{
					jiaozhun-=100;
					if(jiaozhun==-1000)
						jiaozhun=-900;
				}
			}
		}
	}
}
void smg_pinlv()
{
	smg[0]=Seg_Table[15];
	smg[1]=0xff;
	smg[2]=0xff;
	if(flag==1)
	{
		smg[3]=0xff;
		smg[4]=0xff;
		smg[5]=0xff;
		smg[6]=0xc7;
		smg[7]=0xc7;
	}
	else 
	{
		if(ne555_date<10)
		{
			smg[3]=0xff;
			smg[4]=0xff;
			smg[5]=0xff;
			smg[6]=0xff;
			smg[7]=Seg_Table[ne555_date];
		}
		else if(ne555_date<100)
		{
			smg[3]=0xff;
			smg[4]=0xff;
			smg[5]=0xff;
			smg[6]=Seg_Table[ne555_date/10];
			smg[7]=Seg_Table[ne555_date%10];
		}
		else if(ne555_date<1000)
		{
			smg[3]=0xff;
			smg[4]=0xff;
			smg[5]=Seg_Table[ne555_date/100%10];
			smg[6]=Seg_Table[ne555_date/10%10];
			smg[7]=Seg_Table[ne555_date%10];
		}
		else if(ne555_date<10000)
		{
			smg[3]=0xff;
			smg[4]=Seg_Table[ne555_date/1000%10];
			smg[5]=Seg_Table[ne555_date/100%10];
			smg[6]=Seg_Table[ne555_date/10%10];
			smg[7]=Seg_Table[ne555_date%10];
		}
		else if(ne555_date<100000)
		{
			smg[3]=Seg_Table[ne555_date/10000];
			smg[4]=Seg_Table[ne555_date/1000%10];
			smg[5]=Seg_Table[ne555_date/100%10];
			smg[6]=Seg_Table[ne555_date/10%10];
			smg[7]=Seg_Table[ne555_date%10];
		}
	}
}
void smg_canshu1()
{
	smg[0]=0x8C;
	smg[1]=Seg_Table[1];
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=Seg_Table[chaoxian/1000%10];
	smg[5]=Seg_Table[chaoxian/100%10];
	smg[6]=Seg_Table[chaoxian/10%10];
	smg[7]=Seg_Table[chaoxian%10];
}
void smg_canshu2()
{
	smg[0]=0x8C;
	smg[1]=Seg_Table[2];
	smg[2]=0xff;
	smg[3]=0xff;

	if(jiaozhun<0)
	{	
		smg[4]=0xbf;
		smg[5]=Seg_Table[(-jiaozhun)/100];
		smg[6]=Seg_Table[0];
		smg[7]=Seg_Table[0];
	}
	else if(jiaozhun==0)
	{
		smg[4]=0xff;
		smg[5]=0xff;
		smg[6]=0xff;
		smg[7]=Seg_Table[0];
	}
	else if(jiaozhun>0)
	{
		smg[4]=0xff;
		smg[5]=Seg_Table[jiaozhun/100];
		smg[6]=Seg_Table[0];
		smg[7]=Seg_Table[0];
	}
}
void smg_shijian()
{
	smg[0]=Seg_Table[hour/10];
	smg[1]=Seg_Table[hour%10];
	smg[2]=0xbf;
	smg[3]=Seg_Table[min/10];
	smg[4]=Seg_Table[min%10];
	smg[5]=0xbf;
	smg[6]=Seg_Table[sec/10];
	smg[7]=Seg_Table[sec%10];
}
void smg_huixian1()
{
	smg[0]=0x89;
	smg[1]=Seg_Table[15];
	smg[2]=0xff;
	
	if(ne555_datemax<10)
	{
		smg[3]=0xff;
		smg[4]=0xff;
		smg[5]=0xff;
		smg[6]=0xff;
		smg[7]=Seg_Table[ne555_datemax];
	}
	else if(ne555_datemax<100)
	{
		smg[3]=0xff;
		smg[4]=0xff;
		smg[5]=0xff;
		smg[6]=Seg_Table[ne555_datemax/10];
		smg[7]=Seg_Table[ne555_datemax%10];
	}
	else if(ne555_datemax<1000)
	{
		smg[3]=0xff;
		smg[4]=0xff;
		smg[5]=Seg_Table[ne555_datemax/100%10];
		smg[6]=Seg_Table[ne555_datemax/10%10];
		smg[7]=Seg_Table[ne555_datemax%10];
	}
	else if(ne555_datemax<10000)
	{
		smg[3]=0xff;
		smg[4]=Seg_Table[ne555_datemax/1000%10];
		smg[5]=Seg_Table[ne555_datemax/100%10];
		smg[6]=Seg_Table[ne555_datemax/10%10];
		smg[7]=Seg_Table[ne555_datemax%10];
	}
	else if(ne555_datemax<100000)
	{
		smg[3]=Seg_Table[ne555_datemax/10000];
		smg[4]=Seg_Table[ne555_datemax/1000%10];
		smg[5]=Seg_Table[ne555_datemax/100%10];
		smg[6]=Seg_Table[ne555_datemax/10%10];
		smg[7]=Seg_Table[ne555_datemax%10];
	}
}
void smg_huixian2()
{
	smg[0]=0x89;
	smg[1]=0x88;
	smg[2]=Seg_Table[hourmax/10];
	smg[3]=Seg_Table[hourmax%10];
	smg[4]=Seg_Table[minmax/10];
	smg[5]=Seg_Table[minmax%10];
	smg[6]=Seg_Table[secmax/10];
	smg[7]=Seg_Table[secmax%10];
}
u8 led_count;
void led_process()
{
	if(led_count>=200)
	{
		led_count=0;
		if(mode1==1)
				led.b.b0=~led.b.b0;
		else
			led.b.b0=1;
		
		if(ne555_date>chaoxian)			
				led.b.b1=~led.b.b1;
		else
			led.b.b1=1;
		
		if(flag==1)
			led.b.b1=0;
		if(flag==0&&ne555_date<chaoxian)
			led.b.b1=1;
		
		drive(led.hex,0x80);
	}
}
void main()
{
	system_init();
	Timer2_Init();
	conter0_init();
	ds1302_set(10,22,30);
	while(1)
	{
		key16_process();
		ds1302_process();
		da_process();
		
		if(mode1==1)
		{
			mode2=1;
			smg_pinlv();
			
		}
		if(mode1==2)
		{
			if(mode2==1)
				smg_canshu1();
			if(mode2==2)
				smg_canshu2();
		}
		if(mode1==3)
		{
			mode2=1;
			smg_shijian();
		}
		if(mode1==4)
		{
			if(mode2==1)
			{
				smg_huixian1();
			}
			if(mode2==2)
			{
				smg_huixian2();
			}
		}
	}
}
void Timer2_Isr(void) interrupt 12
{
	key16_count++;
	ne555_count++;
	da_count++;
	led_count++;
	
	smg_display();
	ne555_process();
	led_process();
}