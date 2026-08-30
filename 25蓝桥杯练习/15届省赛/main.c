#include "system.h"

hextobin led;

u16 chaoxian=2000;

s16 jiaozhun=0;
u16 jiaozhunbianhao=0;

u8 mode1=0,mode2=0,mode3=0;

long pinlvjiaozhun=0;
void Timer2_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xFB;			//定时器时钟12T模式
	T2L = 0x18;				//设置定时初始值
	T2H = 0xFC;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时
	IE2 |= 0x04;			//使能定时器2中断
	EA=1;
}

void counter0_init()
{
	TMOD|=0X05;
	TL0=0;
	TH0=0;
	TR0=1;
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
	smg_display();
}
u16 ne555_count;
s32 ne555_date;
u16 ne555_date_max;

u8 hour,min,sec;
u8 hour_max,min_max,sec_max;

void ne555_process()
{
	if(ne555_count>=1000)
	{
		ne555_count=0;
		ne555_date=TH0<<8|TL0;
		pinlvjiaozhun=ne555_date+jiaozhun;
		if(pinlvjiaozhun>ne555_date_max)
		{
			ne555_date_max=pinlvjiaozhun;
			hour_max=hour;
			min_max=min;
			sec_max=sec;
		}
		
		TH0=0;
		TL0=0;
	}
}


u16 ds1302_count;
void ds1302_process()
{
	if(ds1302_count>=500)
	{
		ds1302_count=0;
		sec=bcdtodec(Read_Ds1302_Byte(0x81));
		min=bcdtodec(Read_Ds1302_Byte(0x83));
		hour=bcdtodec(Read_Ds1302_Byte(0x85));
	}
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
			if(mode1==1)
				mode2=0;
			
			if(mode1==3)
				mode3=0;
			
			if(mode1==4)
				mode1=0;
		}
		if(trg16==0x84)			//s5
		{
			if(mode1==1)
			{
				mode2++;
				if(mode2==2)
					mode2=0;
			}
			
			if(mode1==3)
			{
				mode3++;
				if(mode3==2)
					mode3=0;
			}
		}
		if(trg16==0x48)			//s8
		{
			if(mode1==1)
			{
				if(mode2==0)
				{
					chaoxian+=1000;
					if(chaoxian==10000)
						chaoxian=9000;
				}
				else if(mode2==1)
				{
					jiaozhun+=100;
					if(jiaozhun==1000)
						jiaozhun=900;
				}
			}
		}
		if(trg16==0x44)			//s9
		{
			if(mode1==1)
			{
				if(mode2==0)
				{
					chaoxian-=1000;
					if(chaoxian==0)
						chaoxian=1000;
				}
				else if(mode2==1)
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
	if(pinlvjiaozhun>=0)
	{
		if(pinlvjiaozhun/10000)
		{
			smg[3]=Seg_Table[pinlvjiaozhun/10000];
			smg[4]=Seg_Table[pinlvjiaozhun/1000%10];
			smg[5]=Seg_Table[pinlvjiaozhun/100%10];
			smg[6]=Seg_Table[pinlvjiaozhun/10%10];
			smg[7]=Seg_Table[pinlvjiaozhun%10];
		}	
		else if(pinlvjiaozhun/1000%10)
		{
			smg[3]=0xff;
			smg[4]=Seg_Table[pinlvjiaozhun/1000%10];
			smg[5]=Seg_Table[pinlvjiaozhun/100%10];
			smg[6]=Seg_Table[pinlvjiaozhun/10%10];
			smg[7]=Seg_Table[pinlvjiaozhun%10];		
		}
		else if(pinlvjiaozhun/100%10)
		{
			smg[3]=0xff;
			smg[4]=0xff;
			smg[5]=Seg_Table[pinlvjiaozhun/100%10];
			smg[6]=Seg_Table[pinlvjiaozhun/10%10];
			smg[7]=Seg_Table[pinlvjiaozhun%10];
		}
		else if(pinlvjiaozhun/10%10)
		{
			smg[3]=0xff;
			smg[4]=0xff;
			smg[5]=0xff;
			smg[6]=Seg_Table[pinlvjiaozhun/10%10];
			smg[7]=Seg_Table[pinlvjiaozhun%10];
		}
		else if(pinlvjiaozhun%10)
		{
			smg[3]=0xff;
			smg[4]=0xff;
			smg[5]=0xff;
			smg[6]=0xff;
			smg[7]=Seg_Table[pinlvjiaozhun%10];
		}
		else
		{
			smg[3]=0xff;
			smg[4]=0xff;
			smg[5]=0xff;
			smg[6]=0xff;
			smg[7]=Seg_Table[0];
		}
	}
	else
	{
		smg[0]=Seg_Table[15];
		smg[1]=0xff;
		smg[2]=0xff;	
		smg[3]=0xff;
		smg[4]=0xff;
		smg[5]=0xff;
		smg[6]=0xc7;
		smg[7]=0xc7;
	}
}
void smg_canshu()
{
	if(mode2==0)
	{
		smg[0]=0x8c;
		smg[1]=Seg_Table[2];
		smg[2]=0xff;
		smg[3]=0xff;
		
		smg[4]=Seg_Table[chaoxian/1000];
		smg[5]=Seg_Table[0];
		smg[6]=Seg_Table[0];
		smg[7]=Seg_Table[0];

	}
	else if(mode2==1)
	{	
		smg[0]=0x8c;
		smg[1]=Seg_Table[2];
		smg[2]=0xff;
		smg[3]=0xff;
		
		if(jiaozhun>=0)
		{
			if(jiaozhun/100%10)
			{
				smg[4]=0xff;
				smg[5]=Seg_Table[jiaozhun/100%10];
				smg[6]=Seg_Table[jiaozhun/10%10];
				smg[7]=Seg_Table[jiaozhun%10];
			}
			else if(jiaozhun/10%10)
			{
				smg[4]=0xff;
				smg[5]=0xff;
				smg[6]=Seg_Table[jiaozhun/10%10];
				smg[7]=Seg_Table[jiaozhun%10];
			}
			else if(jiaozhun%10)
			{
				smg[4]=0xff;
				smg[5]=0xff;
				smg[6]=0xff;
				smg[7]=Seg_Table[jiaozhun%10];
			}
			else
			{
				smg[4]=0xff;
				smg[5]=0xff;
				smg[6]=0xff;
				smg[7]=Seg_Table[0];
			}
		}
	}	
	if(jiaozhun<0)
	{
		jiaozhunbianhao=-jiaozhun;
		if(jiaozhunbianhao/100%10)
		{
			smg[4]=0xbf;
			smg[5]=Seg_Table[jiaozhunbianhao/100%10];
			smg[6]=Seg_Table[jiaozhunbianhao/10%10];
			smg[7]=Seg_Table[jiaozhunbianhao%10];
		}
		else if(jiaozhunbianhao/10%10)
		{
			smg[4]=0xff;
			smg[5]=0xbf;
			smg[6]=Seg_Table[jiaozhunbianhao/10%10];
			smg[7]=Seg_Table[jiaozhunbianhao%10];
		}
		else if(jiaozhunbianhao%10)
		{
			smg[4]=0xff;
			smg[5]=0xff;
			smg[6]=0xbf;
			smg[7]=Seg_Table[jiaozhunbianhao%10];
		}
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
void smg_huixian()
{
	if(mode3==0)
	{
		smg[0]=0x89;
		smg[1]=Seg_Table[15];
		smg[2]=0xff;
		
		if(ne555_date_max/10000)
		{
			smg[3]=Seg_Table[ne555_date_max/10000];
			smg[4]=Seg_Table[ne555_date_max/1000%10];
			smg[5]=Seg_Table[ne555_date_max/100%10];
			smg[6]=Seg_Table[ne555_date_max/10%10];
			smg[7]=Seg_Table[ne555_date_max%10];
		}	
		else if(ne555_date_max/1000%10)
		{
			smg[3]=0xff;
			smg[4]=Seg_Table[ne555_date_max/1000%10];
			smg[5]=Seg_Table[ne555_date_max/100%10];
			smg[6]=Seg_Table[ne555_date_max/10%10];
			smg[7]=Seg_Table[ne555_date_max%10];		
		}
		else if(ne555_date_max/100%10)
		{
			smg[3]=0xff;
			smg[4]=0xff;
			smg[5]=Seg_Table[ne555_date_max/100%10];
			smg[6]=Seg_Table[ne555_date_max/10%10];
			smg[7]=Seg_Table[ne555_date_max%10];
		}
		else if(ne555_date_max/10%10)
		{
			smg[3]=0xff;
			smg[4]=0xff;
			smg[5]=0xff;
			smg[6]=Seg_Table[ne555_date_max/10%10];
			smg[7]=Seg_Table[ne555_date_max%10];
		}
		else if(ne555_date_max%10)
		{
			smg[3]=0xff;
			smg[4]=0xff;
			smg[5]=0xff;
			smg[6]=0xff;
			smg[7]=Seg_Table[ne555_date_max%10];
		}
		else
		{
			smg[3]=0xff;
			smg[4]=0xff;
			smg[5]=0xff;
			smg[6]=0xff;
			smg[7]=Seg_Table[0];
		}
	}
	else if(mode3==1)
	{
		smg[0]=0x89;
		smg[1]=0x88;
		
		smg[2]=Seg_Table[hour_max/10];
		smg[3]=Seg_Table[hour_max%10];
		smg[4]=Seg_Table[min_max/10];
		smg[5]=Seg_Table[min_max%10];
		smg[6]=Seg_Table[sec_max/10];
		smg[7]=Seg_Table[sec_max%10];	
	}
}
void smg_process()
{
	if(mode1==0)
		smg_pinlv();
	else if(mode1==1)
		smg_canshu();
	else if(mode1==2)
		smg_shijian();
	else if(mode1==3)
		smg_huixian();
}
u8 led_count;
u8 time200_1=0,time200_2=0;
void led_process()
{
	if(led_count>=50)
	{
		led_count=0;
		if(mode1==0)
		{
			time200_1++;
			if(time200_1>=4)
			{
				led.b.b0=~led.b.b0;
				time200_1=0;
			}
		}
		else
			led.b.b0=1;
		if(pinlvjiaozhun>=0)
		{
			if(pinlvjiaozhun>chaoxian)
			{
				time200_2++;
				if(time200_2>=4)
				{
					led.b.b1=~led.b.b1;
					time200_2=0;
				}
			}
			else
				led.b.b1=1;
		}
		else
			led.b.b1=0;
		
		drive(led.hex,0x80);
	}
}
u8 da_count;
void da_process()
{
	if(da_count>=2)
	{
		da_count=0;
		if(pinlvjiaozhun<0)
			da_write(0);
		else if(pinlvjiaozhun<=500)
			da_write(255/5);
		else if(pinlvjiaozhun<chaoxian)
			da_write(204*(pinlvjiaozhun-500)/(chaoxian-500)+51);
		else if(pinlvjiaozhun>=chaoxian)
			da_write(255);
	}
}

void main()
{
	system_init();
	Timer2_Init();
	counter0_init();
	ds1302_write(13,3,5);
	while(1)
	{
		key16_process();
		ds1302_process();
		smg_process();
		led_process();
		da_process();
	}
}

void Timer2_Isr(void) interrupt 12
{
	key16_count++;
	ne555_count++;
	ds1302_count++;
	led_count++;
	da_count++;
	
	ne555_process();
	smg_display();
}
