#include "system.h"

hextobin led,buzzer;
u8 mode;
u8 max=30,min=20;
u8 maxtemp=30,mintemp=20;
u32 temp;
u8 flag;
u16 time;
u8 haha;
void Timer2_Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x04;			//定时器时钟1T模式
	T2L = 0x20;				//设置定时初始值
	T2H = 0xD1;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时
	IE2 |= 0x04;			//使能定时器2中断
	EA=1;
}

void smg_xianshi()
{
	smg[0]=Seg_Table[12];
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=0xff;
	smg[6]=Seg_Table[temp/100000];
	smg[7]=Seg_Table[temp/10000%10];
}
void smg_shezhi()
{
	smg[0]=0x8c;
	smg[1]=0xff;
	smg[2]=0xff;
	if(flag==1)
	{
		smg[6]=Seg_Table[min/10];
		smg[7]=Seg_Table[min%10];
		
		if(time<=400)
		{
			smg[3]=Seg_Table[max/10];
			smg[4]=Seg_Table[max%10];
		}
		else if(time<=800)
		{
			smg[3]=0xff;
			smg[4]=0xff;
		}
		else
			time=0;
	}
	smg[5]=0xff;
	
	if(flag==0)
	{
		smg[3]=Seg_Table[max/10];
		smg[4]=Seg_Table[max%10];
		
		if(time<=400)
		{
			smg[6]=Seg_Table[min/10];
			smg[7]=Seg_Table[min%10];
		}
		else if(time<=800)
		{
			smg[6]=0xff;
			smg[7]=0xff;
		}
		else
			time=0;
	}
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

u8 temp_count;
void temp_process()
{
	if(temp_count>=100)
	{
		temp_count=0;
		temp=(u32)(temp_read()*10000);
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
			mode=!mode;
		}
		if(trg4&0x04)		//s5
		{
			if(mode==1)
				flag=!flag;
		}
		if(trg4&0x02)		//s6
		{
			if(mode==1)
			{
				if(flag==0)
					min++;
				else
					max++;
				if(max>=100)
					max=99;
				if(min>=100)
					min=99;
			}
				
		}
		if(trg4&0x01)		//s7
		{
			if(mode==1)
			{
				if(max==0)
					max=1;
				if(min==0)
					min=1;
				if(flag==0)
					min--;
				else
					max--;
			}
		}
	}
}
u8 da_count;
void da_process()
{
	if(da_count>=2)
	{
		da_count=0;
		if(mode==0)
		{
			if(temp/10000>max)
				DA_write(204);
			else if(temp/10000<min)
				DA_write(102);
			else
				DA_write(153);
		}
	}
}
u8 led_count;
void led_process()
{
	if(led_count>=10)
	{
		led_count=0;
		if(mode==0)
		{
			if(temp/10000>max)
				led.b.b0=0;
			else
				led.b.b0=1;
			
			if((temp/10000<=max)&&(temp/10000>=min))
				led.b.b1=0;
			else
				led.b.b1=1;
			if(temp/10000<min)
				led.b.b2=0;
			else
				led.b.b2=1;
			if(haha==1)
				led.b.b3=0;
			else
				led.b.b3=1;
			drive(led.hex,0x80);
		}
	}
}
void main()
{
	while(temp_read()==85);
	temp=(u32)(temp_read()*10000);
	system_int();
	Timer2_Init();
	while(1)
	{
		key4_process();
		temp_process();
		da_process();
		
		if(mode==0)
		{	
			if(max!=maxtemp||min!=mintemp)
			{
				if(max>=min)
				{
					haha=0;
					maxtemp=max;
					mintemp=min;
				}
				else
				{
					haha=1;
					max=maxtemp;
					min=mintemp;
				}
			}
			flag=0;
			smg_xianshi();
		}
		if(mode==1)
		{
			smg_shezhi();
		}
	}
}
void Timer2_Isr(void) interrupt 12
{
	key4_count++;
	temp_count++;
	time++;
	da_count++;
	led_count++;
	
	smg_display();
	led_process();
}