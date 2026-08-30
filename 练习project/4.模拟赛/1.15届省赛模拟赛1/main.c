#include "system.h"

hextobin led,buzzer;
u8 i;
u8 mode1=1;
u8 mode2;
u16 shuru,shurujiu;
u8 shuru1,shuru2,shuru3,shuru4;
u8 hour,min,sec;
u8 hourjiu,minjiu;

//union AT24C02_u16
//{
//	u16 a;
//	u8 b[2];
//}u16_write,u16_read;

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
	drive(0,0x0a);
	led.hex=0xff;
	drive(led.hex,0x80);
	smg_init();
	smg_display();
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
	if(mode2==0)
	{
		shuru1=0;
		shuru2=0;
		shuru3=0;
		shuru4=0;
		smg[4]=0xff;
		smg[5]=0xff;
		smg[6]=0xff;
		smg[7]=0xff;
	}
	if(mode2==1)
	{
		smg[4]=0xff;
		smg[5]=0xff;
		smg[6]=0xff;
		smg[7]=Seg_Table[shuru1];
	}
	if(mode2==2)
	{
		smg[4]=0xff;
		smg[5]=0xff;
		smg[6]=Seg_Table[shuru1];
		smg[7]=Seg_Table[shuru2];
	}
	if(mode2==3)
	{
		smg[4]=0xff;
		smg[5]=Seg_Table[shuru1];
		smg[6]=Seg_Table[shuru2];
		smg[7]=Seg_Table[shuru3];
	}
	if(mode2==4)
	{
		smg[4]=Seg_Table[shuru1];
		smg[5]=Seg_Table[shuru2];
		smg[6]=Seg_Table[shuru3];
		smg[7]=Seg_Table[shuru4];
	}
}
void smg_jilu()
{
	smg[0]=Seg_Table[14];
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=Seg_Table[hourjiu/10];
	smg[4]=Seg_Table[hourjiu%10];
	smg[5]=0xbf;
	smg[6]=Seg_Table[minjiu/10];
	smg[7]=Seg_Table[minjiu%10];
}
u8 key_count;
void key16_process()
{
	if(key_count>=10)
	{
		key_count=0;
		key16();
		if(trg16==0x88)		//s4
		{
			mode1++;
			if(mode1==4)
				mode1=1;
			if(mode1==3)
			{
				hourjiu=hour;
				minjiu=min;
				at24c02_write(0,hour);
				at24c02_write(1,min);
				if(shuru!=0)
					shurujiu=shuru;
				if(mode2==1)
					shuru=shuru1;
				if(mode2==2)
					shuru=shuru1*10+shuru2;
				if(mode2==3)
					shuru=shuru1*100+shuru2*10+shuru3;
				if(mode2==4)
					shuru=shuru1*1000+shuru2*100+shuru3*10+shuru4;
				
//				u16_write.a=shuru;
//				for(i=0;i<sizeof(u16);i++)
//					at24c02_write(2+i,u16_write.b[i]);
//				
				at24c02_write(2,shuru/256);
				at24c02_write(3,shuru%256);
			}
		}
		if(mode1==2)
		{
			if(trg16==0x84)		//s5
			{
				mode2=0;
			}
			if(trg16==0x82)		//s6
			{
				mode2++;
				if(mode2==6)
					mode2=5;
				
				if(mode2==1)
					shuru1=0;
				if(mode2==2)
					shuru2=0;
				if(mode2==3)
					shuru3=0;
				if(mode2==4)
					shuru4=0;
			}
			if(trg16==0x42)		//s10
			{
				mode2++;
				if(mode2==6)
					mode2=5;
				
				if(mode2==1)
					shuru1=1;
				if(mode2==2)
					shuru2=1;
				if(mode2==3)
					shuru3=1;
				if(mode2==4)
					shuru4=1;
			}
			if(trg16==0x22)		//s14
			{
				mode2++;
				if(mode2==6)
					mode2=5;
				
				if(mode2==1)
					shuru1=2;
				if(mode2==2)
					shuru2=2;
				if(mode2==3)
					shuru3=2;
				if(mode2==4)
					shuru4=2;
			}
			if(trg16==0x12)		//s18
			{
				mode2++;
				if(mode2==6)
					mode2=5;
				
				if(mode2==1)
					shuru1=3;
				if(mode2==2)
					shuru2=3;
				if(mode2==3)
					shuru3=3;
				if(mode2==4)
					shuru4=3;
			}
			if(trg16==0x44)		//s9
			{
				mode2++;
				if(mode2==6)
					mode2=5;
				
				if(mode2==1)
					shuru1=4;
				if(mode2==2)
					shuru2=4;
				if(mode2==3)
					shuru3=4;
				if(mode2==4)
					shuru4=4;
			}
			if(trg16==0x24)		//s13
			{
				mode2++;
				if(mode2==6)
					mode2=5;
				
				if(mode2==1)
					shuru1=5;
				if(mode2==2)
					shuru2=5;
				if(mode2==3)
					shuru3=5;
				if(mode2==4)
					shuru4=5;
			}
			if(trg16==0x14)		//s17
			{
				mode2++;
				if(mode2==6)
					mode2=5;
				
				if(mode2==1)
					shuru1=6;
				if(mode2==2)
					shuru2=6;
				if(mode2==3)
					shuru3=6;
				if(mode2==4)
					shuru4=6;
			}
			if(trg16==0x48)		//s8
			{
				mode2++;
				if(mode2==6)
					mode2=5;
				
				if(mode2==1)
					shuru1=7;
				if(mode2==2)
					shuru2=7;
				if(mode2==3)
					shuru3=7;
				if(mode2==4)
					shuru4=7;
			}
			if(trg16==0x28)		//s12
			{
				mode2++;
				if(mode2==6)
					mode2=5;
				
				if(mode2==1)
					shuru1=8;
				if(mode2==2)
					shuru2=8;
				if(mode2==3)
					shuru3=8;
				if(mode2==4)
					shuru4=8;
			}
			if(trg16==0x18)		//s16
			{
				mode2++;
				if(mode2==6)
					mode2=5;
				
				if(mode2==1)
					shuru1=9;
				if(mode2==2)
					shuru2=9;
				if(mode2==3)
					shuru3=9;
				if(mode2==4)
					shuru4=9;
			}
		}
	}
}
u8 led_count;
void led_process()
{
	if(led_count>=10)
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
		if(shuru!=0)
		{
			if(shuru>shurujiu)
				led.b.b3=0;
			else
				led.b.b3=1;
		}
	}
	drive(led.hex,0x80);
}
void ds1302_process()
{
	sec = hextoshi(Read_Ds1302_Byte(0x81));
	min = hextoshi(Read_Ds1302_Byte(0x83));
	hour = hextoshi(Read_Ds1302_Byte(0x85));
}
void main()
{
	system_init();
	Timer2_Init();
	ds1302_set(16,23,30);
	while(1)
	{
		key16_process();
		ds1302_process();
		if(mode1==1)
		{
			smg_shijian();
		}
		if(mode1==2)
		{
			smg_shuru();
		}
		if(mode1==3)
		{
			smg_jilu();
			mode2=0;
		}
	}
}

void Timer2_Isr(void) interrupt 12
{
	key_count++;
	led_count++;
	
	smg_display();
	led_process();
}