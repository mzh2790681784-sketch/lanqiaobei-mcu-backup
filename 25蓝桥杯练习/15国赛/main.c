#include "system.h"

hextobin led,buzzer;
u8 mode=0;
u8 smg_mode=0;
u8 mudidi_flag=0;
u8 canshu_mode=0;
u16 x=0,y=0;
u16 x1=0,y1=0;
float v,r=1.0;
float r1=1.0;
s8 b=0;
s8 b1=0;
u8 changjing=0;
u16 csb_date;
float dis=0;
float dis1=0;
u32 run_time=0;
u16 count_3000=0;
void Timer2_Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x04;			//定时器时钟1T模式
	T2L = 0x20;				//设置定时初始值
	T2H = 0xD1;				//设置定时初始值
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
	buzzer.hex=0;
	drive(buzzer.hex,0xa0);
	led.hex=0xff;
	drive(led.hex,0x80);
	smg_init();
	smg_display();
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
			if(mudidi_flag==1&&mode==0)
				mode=1;
			if(mode==2)
			{
				if(csb_date>=30)
					mode=1;
			}
		}
		if(trg16==0x84)		//s5
		{
			if(mode==0)
			{
				x1=0;
				y1=0;
			}
		}
		if(trg16==0x48)		//s8
		{
			smg_mode++;
			if(smg_mode==3)
			{
				canshu_mode=0;
				r1=r;
				b1=b;
				smg_mode=0;
			}
		}
		if(trg16==0x44)		//s9
		{
			if(smg_mode==2)
			{
				canshu_mode++;
				if(canshu_mode==2)
					canshu_mode=0;
			}
		}
		if(trg16==0x28)		//s12
		{
			if(smg_mode==2)
			{
				if(canshu_mode==0)
				{
					r+=0.1;
					if(r>=2)
						r=2.0;
				}
				else if(canshu_mode==1)
				{
					b+=5;
					if(b>=90)
						b=90;
				}
			}
		}
		if(trg16==0x24)		//s13
		{
			if(smg_mode==2)
			{
				if(canshu_mode==0)
				{
					r-=0.1;
					if(r<=1)
						r=1.0;
				}
				else if(canshu_mode==1)
				{
					b-=5;
					if(b<=-90)
						b=-90;
				}
			}
		}
	}
}

u16 ne555_date;
u16 ne555_count;
void ne555_process()
{
	if(ne555_count>=10)
	{
		ne555_count=0;
		ne555_date=((TH0<<8)|TL0)*101;
		TL0=0;TH0=0;
		
		if(mode==1)
			v=3.14*r1*(float)ne555_date/100+(float)b1;
		else
			v=0;
		if(v<=0)
			v=0;
		dis1+=v*0.01;
		if(dis1>=dis)
			dis1=dis;
		if(dis!=0&&dis1!=0)
		{
			x1=dis1*x/dis;
			y1=dis1*y/dis;
		}
		else
		{
			x1=0;
			y1=0;
		}
	}
}
u8 ad_count;
u8 ch1;
void ad_process()
{
	if(ad_count>=10)
	{
		ad_count=0;
		ch1=ad_read(0x41);
		if(ch1>=61.2)
			changjing=1;
		else
			changjing=0;
	}
}

u8 csb_count;
void csb_process()
{
	if(csb_count>=100)
	{
		csb_count=0;
		csb_date=csb_get();
		if(csb_date<30&&mode==1)
			mode=2;
	}
}

void ck_process()
{
	if(ck_flag)
	{
		if(ck_flag==1)
		{
			if(mode==0)
			{
				SendString("Got it");
				if(ck_string[2]==','&&ck_string[4]==')')
				{
					x=(ck_string[1]-'0');
					y=(ck_string[3]-'0');
				}
				else if(ck_string[2]==','&&ck_string[5]==')')
				{
					x=(ck_string[1]-'0');
					y=(ck_string[3]-'0')*10+(ck_string[4]-'0');
				}
				else if(ck_string[2]==','&&ck_string[6]==')')
				{
					x=(ck_string[1]-'0');
					y=(ck_string[3]-'0')*100+(ck_string[4]-'0')*10+(ck_string[5]-'0');
				}	


				else if(ck_string[3]==','&&ck_string[5]==')')
				{
					x=(ck_string[1]-'0')*10+(ck_string[2]-'0');
					y=(ck_string[4]-'0');
				}
				else if(ck_string[3]==','&&ck_string[6]==')')
				{
					x=(ck_string[1]-'0')*10+(ck_string[2]-'0');
					y=(ck_string[4]-'0')*10+(ck_string[5]-'0');
				}
				else if(ck_string[3]==','&&ck_string[7]==')')
				{
					x=(ck_string[1]-'0')*10+(ck_string[2]-'0');
					y=(ck_string[4]-'0')*100+(ck_string[5]-'0')*10+(ck_string[6]-'0');
				}

				
				else if(ck_string[4]==','&&ck_string[6]==')')
				{
					x=(ck_string[1]-'0')*100+(ck_string[2]-'0')*10+(ck_string[3]-'0');
					y=(ck_string[5]-'0');
				}
				else if(ck_string[4]==','&&ck_string[7]==')')
				{
					x=(ck_string[1]-'0')*100+(ck_string[2]-'0')*10+(ck_string[3]-'0');
					y=(ck_string[5]-'0')*10+(ck_string[6]-'0');
				}
				else if(ck_string[4]==','&&ck_string[8]==')')
				{
					x=(ck_string[1]-'0')*100+(ck_string[2]-'0')*10+(ck_string[3]-'0');
					y=(ck_string[5]-'0')*100+(ck_string[6]-'0')*10+(ck_string[7]-'0');
				}
				dis=sqrt(x*x+y*y);
			}
			else
				SendString("Busy");
		}
		else if(ck_flag==2)
		{
			if(mode==0)
				SendString("Idle");
			else if(mode==1)
				SendString("Busy");
			else if(mode==2)
				SendString("Wait");
		}
		else if(ck_flag==3)
		{
			SendData('(');
			if(x1>=100)
			{
				SendData((x1/100)+'0');
				SendData((x1/10%10)+'0');
				SendData((x1%10)+'0');
			}
			else if(x1>=10)
			{
				SendData((x1/10)+'0');
				SendData((x1%10)+'0');
			}
			else
			{
				SendData(x1+'0');
			}
			SendData(',');
			if(y1>=100)
			{
				SendData((y1/100)+'0');
				SendData((y1/10%10)+'0');
				SendData((y1%10)+'0');
			}
			else if(y1>=10)
			{
				SendData((y1/10)+'0');
				SendData((y1%10)+'0');
			}
			else
			{
				SendData(y1+'0');
			}
			SendData(')');
		}
		else if(ck_flag==4)
		{
			SendString("Error");
		}
		ck_flag=0;
	}
}

void smg_zuobiao()
{
	smg[0]=0xc7;
	if(mode==1||mode==2)
	{
		if(x>=100)
		{
			smg[1]=smg_num[x/100];
			smg[2]=smg_num[x/10%10];
			smg[3]=smg_num[x%10];
		}
		else if(x>=10)
		{
			smg[1]=0xff;
			smg[2]=smg_num[x/10];
			smg[3]=smg_num[x%10];
		}
		else
		{
			smg[1]=0xff;
			smg[2]=0xff;
			smg[3]=smg_num[x];
		}
		smg[4]=0xbf;
		if(y>=100)
		{
			smg[5]=smg_num[y/100];
			smg[6]=smg_num[y/10%10];
			smg[7]=smg_num[y%10];
		}
		else if(y>=10)
		{
			smg[5]=0xff;
			smg[6]=smg_num[y/10];
			smg[7]=smg_num[y%10];
		}
		else
		{
			smg[5]=0xff;
			smg[6]=0xff;
			smg[7]=smg_num[y];
		}
	}
	else if(mode==0)
	{
		if(x1>=100)
		{
			smg[1]=smg_num[x1/100];
			smg[2]=smg_num[x1/10%10];
			smg[3]=smg_num[x1%10];
		}
		else if(x1>=10)
		{
			smg[1]=0xff;
			smg[2]=smg_num[x1/10];
			smg[3]=smg_num[x1%10];
		}
		else
		{
			smg[1]=0xff;
			smg[2]=0xff;
			smg[3]=smg_num[x1];
		}
		smg[4]=0xbf;
		if(y1>=100)
		{
			smg[5]=smg_num[y1/100];
			smg[6]=smg_num[y1/10%10];
			smg[7]=smg_num[y1%10];
		}
		else if(y1>=10)
		{
			smg[5]=0xff;
			smg[6]=smg_num[y1/10];
			smg[7]=smg_num[y1%10];
		}
		else
		{
			smg[5]=0xff;
			smg[6]=0xff;
			smg[7]=smg_num[y1];
		}
	}
}
void smg_sudu()
{
	smg[0]=smg_num[14];
	if(mode==0)
	{
		smg[1]=smg_num[2];
		smg[2]=0xff;
		smg[3]=0xbf;
		smg[4]=0xbf;
		smg[5]=0xbf;
		smg[6]=0xbf;
		smg[7]=0xbf;
	}
	else if(mode==1)
	{
		smg[1]=smg_num[1];
		smg[2]=0xff;
		if(v>=1000)
		{
			smg[3]=smg_num[(u16)v/1000];
			smg[4]=smg_num[(u16)v/100%10];
			smg[5]=smg_num[(u16)v/10%10];
			smg[6]=smg_num[(u16)v%10]&0x7f;
			smg[7]=smg_num[(u16)(v*10)%10];
		}
		else if(v>=100)
		{
			smg[3]=0xff;
			smg[4]=smg_num[(u8)v/100];
			smg[5]=smg_num[(u8)v/10%10];
			smg[6]=smg_num[(u8)v%10]&0x7f;
			smg[7]=smg_num[(u8)(v*10)%10];
		}
		else if(v>=10)
		{
			smg[3]=0xff;
			smg[4]=0xff;
			smg[5]=smg_num[(u8)v/10];
			smg[6]=smg_num[(u8)v%10]&0x7f;
			smg[7]=smg_num[(u8)(v*10)%10];
		}
		else
		{
			smg[3]=0xff;
			smg[4]=0xff;
			smg[5]=0xff;
			smg[6]=smg_num[(u8)v]&0x7f;
			smg[7]=smg_num[(u8)(v*10)%10];
		}
	}
	else if(mode==2)
	{
		smg[1]=smg_num[3];
		smg[2]=0xff;
		if(csb_date>=10000)
		{
			smg[3]=smg_num[csb_date/10000];
			smg[4]=smg_num[csb_date/1000%10];
			smg[5]=smg_num[csb_date/100%10];
			smg[6]=smg_num[csb_date/10%10];
			smg[7]=smg_num[csb_date%10];
		}
		else if(csb_date>=1000)
		{
			smg[3]=0xff;
			smg[4]=smg_num[csb_date/1000];
			smg[5]=smg_num[csb_date/100%10];
			smg[6]=smg_num[csb_date/10%10];
			smg[7]=smg_num[csb_date%10];
		}
		else if(csb_date>=100)
		{
			smg[3]=0xff;
			smg[4]=0xff;
			smg[5]=smg_num[csb_date/100];
			smg[6]=smg_num[csb_date/10%10];
			smg[7]=smg_num[csb_date%10];
		}
		else if(csb_date>=10)
		{
			smg[3]=0xff;
			smg[4]=0xff;
			smg[5]=0xff;
			smg[6]=smg_num[csb_date/10];
			smg[7]=smg_num[csb_date%10];
		}
		else
		{
			smg[3]=0xff;
			smg[4]=0xff;
			smg[5]=0xff;
			smg[6]=0xff;
			smg[7]=smg_num[csb_date];
		}
	}
}
void smg_canshu()
{
	smg[0]=0x8c;
	smg[1]=0xff;
	smg[2]=smg_num[(u8)r]&0x7f;
	smg[3]=smg_num[(u8)(r*10)%10];
	smg[4]=0xff;
	if(b>=0)
	{
		smg[5]=0xff;
		if(b>=10)
		{
			smg[6]=smg_num[b/10];
			smg[7]=smg_num[b%10];
		}
		else
		{
			smg[6]=0xff;
			smg[7]=smg_num[b];
		}
	}
	else
	{
		if(b<=-10)
		{
			smg[5]=0xbf;
			smg[6]=smg_num[-b/10];
			smg[7]=smg_num[-b%10];
		}
		else
		{
			smg[5]=0xff;
			smg[6]=0xbf;
			smg[7]=smg_num[-b];
		}
	}
}

void smg_process()
{
	if(smg_mode==0)
		smg_zuobiao();
	else if(smg_mode==1)
		smg_sudu();
	else if(smg_mode==2)
		smg_canshu();
}

u8 led_count;
u8 count_100=0;
u8 arrive_flag=0;
void led_process()
{
	if(led_count>=10)
	{
		led_count=0;
		if(mode==1)
			buzzer.b.b4=1;
		else
			buzzer.b.b4=0;
		
		drive(buzzer.hex,0xa0);
		
		if(mode==1)
		{
			led.b.b0=0;
			if(changjing==1)
				led.b.b1=1;
			else
				led.b.b1=0;
		}
		else if(mode==0)
		{
			led.b.b0=1;
			led.b.b1=1;
		}
		else
		{
			led.b.b1=1;
			count_100++;
			if(count_100>=10)
			{
				count_100=0;
				led.b.b0=!led.b.b0;
			}
		}
		
		if(x1>=x&&y1>=y&&x1!=0&&y1!=0)
		{
			x1=x;
			y1=y;
			arrive_flag=1;
			mode=0;
			dis1=0;
		}
		if(arrive_flag==1)
		{
			count_3000++;
			if(count_3000<=300)
			{
				led.b.b2=0;
			}
			else
			{
				count_3000=0;
				led.b.b2=1;
				arrive_flag=0;
			}
		}
		
		drive(led.hex,0x80);
	}
}

void main()
{
	system_init();
	counter0_init();
	Uart1_Init();
	Timer2_Init();
	while(1)
	{
		ad_process();
		ck_process();
		csb_process();
		key16_process();
		smg_process();
	}
}

void Timer2_Isr(void) interrupt 12
{
	ne555_count++;
	key16_count++;
	ad_count++;
	csb_count++;
	led_count++;
	
	ck_relax();
	ne555_process();
	smg_display();
	led_process();
}