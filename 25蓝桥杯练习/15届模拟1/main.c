#include "system.h"

u8 mode1=0;
u8 flag=0;
u8 flag2=0;
u8 a1,a2,a3,a4;
hextobin led;
u8 hour,min,sec;
u8 hour1,min1,sec1;
u16 shuju=9999,shuju_last=9999;
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
void at24c02_process()
{
	if(flag2==1)
	{
		flag2=0;
		at24C02_write(0x00,hour1);
		at24C02_write(0x01,min1);
		if(flag==1)
		{
			shuju_last=shuju;
			shuju=a1;
			at24C02_write(0x02,0);
			at24C02_write(0x03,a1);
		}
		else if(flag==2)
		{
			shuju_last=shuju;
			shuju=a1+a2*10;
			at24C02_write(0x02,0);
			at24C02_write(0x03,a1+a2*10);
		}
		else if(flag==3)
		{
			shuju_last=shuju;
			shuju=a1+a2*10+a3*100;
			at24C02_write(0x02,(a1+a2*10+a3*100)/256);
			at24C02_write(0x03,(a1+a2*10+a3*100)%256);
		}
		else if(flag==4)
		{
			shuju_last=shuju;
			shuju=a1+a2*10+a3*100+a4*1000;
			at24C02_write(0x02,(a1+a2*10+a3*100+a4*1000)/256);
			at24C02_write(0x03,(a1+a2*10+a3*100+a4*1000)%256);
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
			if(mode1==1)
				flag=0;
			if(mode1==2)
			{
				hour1=hour;
				min1=min;
				flag2=1;
			}
			if(mode1==3)
				mode1=0;
		}
		if(trg16==0x84)		//s5
		{
			if(mode1==1)
				flag=0;
		}
		if(trg16==0x82)		//s6
		{
			if(mode1==1)
			{
				flag++;
				if(flag==1)
				{
					a1=0;
				}
				else if(flag==2)
				{
					a2=a1;
					a1=0;
				}
				else if(flag==3)
				{
					a3=a2;
					a2=a1;
					a1=0;
				}
				else if(flag==4)
				{
					a4=a3;
					a3=a2;
					a2=a1;
					a1=0;
				}
				else if(flag==5)
					flag=4;
			}
		}
		if(trg16==0x48)		//s8
		{
			if(mode1==1)
			{
				flag++;
				if(flag==1)
				{
					a1=7;
				}
				else if(flag==2)
				{
					a2=a1;
					a1=7;
				}
				else if(flag==3)
				{
					a3=a2;
					a2=a1;
					a1=7;
				}
				else if(flag==4)
				{
					a4=a3;
					a3=a2;
					a2=a1;
					a1=7;
				}
				else if(flag==5)
					flag=4;			
			}
		}
		if(trg16==0x44)		//s9
		{
			if(mode1==1)
			{
				flag++;
				if(flag==1)
				{
					a1=4;
				}
				else if(flag==2)
				{
					a2=a1;
					a1=4;
				}
				else if(flag==3)
				{
					a3=a2;
					a2=a1;
					a1=4;
				}
				else if(flag==4)
				{
					a4=a3;
					a3=a2;
					a2=a1;
					a1=4;
				}
				else if(flag==5)
					flag=4;			
			}
		}
		if(trg16==0x42)		//s10
		{
			if(mode1==1)
			{
				flag++;
				if(flag==1)
				{
					a1=1;
				}
				else if(flag==2)
				{
					a2=a1;
					a1=1;
				}
				else if(flag==3)
				{
					a3=a2;
					a2=a1;
					a1=1;
				}
				else if(flag==4)
				{
					a4=a3;
					a3=a2;
					a2=a1;
					a1=1;
				}
				else if(flag==5)
					flag=4;			
			}
		}
		if(trg16==0x28)		//s12
		{
			if(mode1==1)
			{
				flag++;
				if(flag==1)
				{
					a1=8;
				}
				else if(flag==2)
				{
					a2=a1;
					a1=8;
				}
				else if(flag==3)
				{
					a3=a2;
					a2=a1;
					a1=8;
				}
				else if(flag==4)
				{
					a4=a3;
					a3=a2;
					a2=a1;
					a1=8;
				}
				else if(flag==5)
					flag=4;			
			}
		}
		if(trg16==0x24)		//s13
		{
			if(mode1==1)
			{
				flag++;
				if(flag==1)
				{
					a1=5;
				}
				else if(flag==2)
				{
					a2=a1;
					a1=5;
				}
				else if(flag==3)
				{
					a3=a2;
					a2=a1;
					a1=5;
				}
				else if(flag==4)
				{
					a4=a3;
					a3=a2;
					a2=a1;
					a1=5;
				}
				else if(flag==5)
					flag=4;			
			}
		}
		if(trg16==0x22)		//s14
		{
			if(mode1==1)
			{
				flag++;
				if(flag==1)
				{
					a1=2;
				}
				else if(flag==2)
				{
					a2=a1;
					a1=2;
				}
				else if(flag==3)
				{
					a3=a2;
					a2=a1;
					a1=2;
				}
				else if(flag==4)
				{
					a4=a3;
					a3=a2;
					a2=a1;
					a1=2;
				}
				else if(flag==5)
					flag=4;			
			}
		}
		if(trg16==0x18)		//s16
		{
			if(mode1==1)
			{
				flag++;
				if(flag==1)
				{
					a1=9;
				}
				else if(flag==2)
				{
					a2=a1;
					a1=9;
				}
				else if(flag==3)
				{
					a3=a2;
					a2=a1;
					a1=9;
				}
				else if(flag==4)
				{
					a4=a3;
					a3=a2;
					a2=a1;
					a1=9;
				}
				else if(flag==5)
					flag=4;			
			}
		}
		if(trg16==0x14)		//s17
		{
			if(mode1==1)
			{
				flag++;
				if(flag==1)
				{
					a1=6;
				}
				else if(flag==2)
				{
					a2=a1;
					a1=6;
				}
				else if(flag==3)
				{
					a3=a2;
					a2=a1;
					a1=6;
				}
				else if(flag==4)
				{
					a4=a3;
					a3=a2;
					a2=a1;
					a1=6;
				}
				else if(flag==5)
					flag=4;			
			}
		}
		if(trg16==0x12)		//s18
		{
			if(mode1==1)
			{
				flag++;
				if(flag==1)
				{
					a1=3;
				}
				else if(flag==2)
				{
					a2=a1;
					a1=3;
				}
				else if(flag==3)
				{
					a3=a2;
					a2=a1;
					a1=3;
				}
				else if(flag==4)
				{
					a4=a3;
					a3=a2;
					a2=a1;
					a1=3;
				}
				else if(flag==5)
					flag=4;			
			}
		}
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

void smg_shuru()
{
	smg[0]=Seg_Table[12];
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	if(flag==0)
	{
		smg[4]=0xff;
		smg[5]=0xff;
		smg[6]=0xff;
		smg[7]=0xff;
	}
	else if(flag==1)
	{
		smg[4]=0xff;
		smg[5]=0xff;
		smg[6]=0xff;
		smg[7]=Seg_Table[a1];
	}
	else if(flag==2)
	{
		smg[4]=0xff;
		smg[5]=0xff;
		smg[6]=Seg_Table[a2];
		smg[7]=Seg_Table[a1];
	}
	else if(flag==3)
	{
		smg[4]=0xff;
		smg[5]=Seg_Table[a3];
		smg[6]=Seg_Table[a2];
		smg[7]=Seg_Table[a1];
	}
	else if(flag==4)
	{
		smg[4]=Seg_Table[a4];
		smg[5]=Seg_Table[a3];
		smg[6]=Seg_Table[a2];
		smg[7]=Seg_Table[a1];
	}
}

void smg_jilu()
{
	smg[0]=Seg_Table[14];
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=Seg_Table[hour1/10];
	smg[4]=Seg_Table[hour1%10];
	smg[5]=0xbf;
	smg[6]=Seg_Table[min1/10];
	smg[7]=Seg_Table[min1%10];
}
void smg_process()
{
	if(mode1==0)
		smg_shijian();
	else if(mode1==1)
		smg_shuru();
	else if(mode1==2)
		smg_jilu();
}
u8 led_count;
void led_process()
{
	if(led_count>=10)
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
		
		if(shuju>shuju_last)
			led.b.b3=0;
		else
			led.b.b3=1;
		drive(led.hex,0x80);
	}
}

void main()
{
	system_init();
	Timer2_Init();
	ds1302_set(23,9,59);
	while(1)
	{
		key16_process();
		ds1302_process();
		smg_process();
		led_process();
		at24c02_process();
	}
}

void Timer2_Isr(void) interrupt 12
{
	key16_count++;
	ds1302_count++;
	led_count++;
	
	smg_display();
}