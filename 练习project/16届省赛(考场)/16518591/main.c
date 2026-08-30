#include "system.h"

hextobin led,buzzer;

u8 mode1=0,mode2=0;
u8 guangqiang;


u8 wenducanshu=30;
u8 gaowen=0;

u8 yundongzhuangtai=0;
u8 yundongzhuangtai_last=0;

u8 julicanshu=30;
u8 jiejin=0;

u16 xihe=0;

void Delayms(u16 xms)	//@12.000MHz
{
	u16 k;
	unsigned char data i, j;
	for(k=0;k<xms;k++)
	{
		i = 12;
		j = 169;
		do
		{
			while (--j);
		} while (--i);
	}
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

void Counter0_Init(void)
{
	TMOD|=0x05;			
	TL0=0x00;
	TH0=0x00;			
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
	buzzer.hex=0x00;
	drive(buzzer.hex,0xa0);
	led.hex=0xff;
	drive(led.hex,0x80);
	smg_init();
}

u16 csb_count;
u16 csb_date; 
u16 csb_date_last; 
u8 flag=0;
u8 time=0;
void csb_process()
{
	if(csb_count>=1000)
	{
		csb_count=0;
		csb_date_last=csb_date;
		csb_date=csb_get();
		
		if(csb_date<julicanshu)
			jiejin=1;
		else
			jiejin=0;
		
		yundongzhuangtai_last=yundongzhuangtai;
		if(flag==0)
		{
			if(csb_date_last>=csb_date)
			{
				if((csb_date_last-csb_date)<5)
				{
					yundongzhuangtai=1;
				}
				else if((csb_date_last-csb_date)<10)
				{
					yundongzhuangtai=2;
				}
				else
				{
					yundongzhuangtai=3;
				}
			}
			else
			{
				if((csb_date-csb_date_last)<5)
				{
					yundongzhuangtai=1;
				}
				else if((csb_date-csb_date_last)<10)
				{
					yundongzhuangtai=2;
				}
				else
				{
					yundongzhuangtai=3;
				}
			}
		}
		if(yundongzhuangtai_last!=yundongzhuangtai)
		{
			flag=1;
		}
		if(flag==1)
		{
			time++;
			if(time==3)
			{
				flag=0;
				time=0;
			}
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
		if(temp/10000>wenducanshu)
			gaowen=1;
		else
			gaowen=0;
	}
}

u8 ad_count;
u8 ch1;	
void ad_process()
{
	if(ad_count>=50)
	{
		ad_count=0;
		ad_read(0x41);
		ch1=ad_read(0x41);
		if(ch1>=153)
			guangqiang=1;
		else if(ch1>=102)
			guangqiang=2;
		else if(ch1>=26)
			guangqiang=3;
		else
			guangqiang=4;
	}
}

u8 key16_count;
u8 s8s9=0;
void key16_process()
{
	if(key16_count>=10)
	{
		key16_count=0;
		key16();
		
		if(trg16==0x88)		//s4
		{
			mode1++;
			if(mode1==2)
				mode2=0;
			if(mode1==4)
				mode1=0;
		}
		if(trg16==0x84)		//s5
		{
			if(mode1==2)
			{
				mode2++;
				if(mode2==2)
					mode2=0;
			}
		}
		if(trg16==0x48)		//s8
		{
			if(mode1==2)
			{
				if(mode2==0)
				{
					wenducanshu+=1;
					if(wenducanshu>=81)
						wenducanshu=80;
				}
				else if(mode2==1)
				{
					julicanshu+=5;
					if(julicanshu>80)
						julicanshu=80;
				}
			}
		}
		if(trg16==0x44)		//s9
		{
			if(mode1==2)
			{
				if(mode2==0)
				{
						wenducanshu-=1;
					if(wenducanshu<=19)
						wenducanshu=20;
				}
				else if(mode2==1)
				{
					julicanshu-=5;
					if(julicanshu<20)
						julicanshu=20;
				}
			}
		}
		if(cont16==0x40)		//s8s9	
		{
			if(mode1==3)
			{
				s8s9++;
				if(s8s9>=200)
				{
					s8s9=201;
					xihe=0;
				}
			}
		}
		if(trg16==0&&cont16==0)
		{
			s8s9=0;
		}
	}
}

void smg_huanjing()
{
	smg[0]=Seg_Table[12];
	smg[1]=Seg_Table[temp/100000];
	smg[2]=Seg_Table[temp/10000%10];
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=0xff;
	smg[6]=0xc8;
	smg[7]=Seg_Table[guangqiang];
}

void smg_yundong()
{
	smg[0]=0xc7;
	smg[1]=Seg_Table[yundongzhuangtai];
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=Seg_Table[csb_date/100];
	smg[6]=Seg_Table[csb_date/10%10];
	smg[7]=Seg_Table[csb_date%10];
}

void smg_canshu()
{
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=0xff;
	
	if(mode2==0)
	{
		smg[0]=0x8c;
		smg[1]=Seg_Table[12];
		smg[6]=Seg_Table[wenducanshu/10];
		smg[7]=Seg_Table[wenducanshu%10];
	}
	else if(mode2==1)
	{
		smg[0]=0x8c;
		smg[1]=0xc7;
		smg[6]=Seg_Table[julicanshu/10];
		smg[7]=Seg_Table[julicanshu%10];
	}
}

void smg_tongji()
{
	smg[0]=0xc8;
	smg[1]=Seg_Table[12];
	smg[2]=0xff;
	smg[3]=0xff;
	if(xihe>=1000)
	{
		smg[4]=Seg_Table[xihe/1000];
		smg[5]=Seg_Table[xihe/100%10];
		smg[6]=Seg_Table[xihe/10%10];
		smg[7]=Seg_Table[xihe%10];
	}
	else if(xihe>=100)
	{
		smg[4]=0xff;
		smg[5]=Seg_Table[xihe/100];
		smg[6]=Seg_Table[xihe/10%10];
		smg[7]=Seg_Table[xihe%10];
	}
	else if(xihe>=10)
	{
		smg[4]=0xff;
		smg[5]=0xff;
		smg[6]=Seg_Table[xihe/10];
		smg[7]=Seg_Table[xihe%10];
	}
	else if(xihe>=0)
	{
		smg[4]=0xff;
		smg[5]=0xff;
		smg[6]=0xff;
		smg[7]=Seg_Table[xihe];
	}
}

void smg_process()
{
	if(mode1==0)
		smg_huanjing();
	else if(mode1==1)
		smg_yundong();
	else if(mode1==2)
		smg_canshu();
	else if(mode1==3)
		smg_tongji();
}

u8 led_count;
u8 xihe_flag=0;
u8 time100ms=0;
void led_process()
{
	if(led_count>=10)
	{
		led_count=0;
		
		if(mode1!=2)
		{
			if(jiejin==1)
			{
				if(guangqiang==1)
				{
					led.b.b0=0;
					led.b.b1=1;
					led.b.b2=1;
					led.b.b3=1;			
				}
				else if(guangqiang==2)
				{
					led.b.b0=0;
					led.b.b1=0;
					led.b.b2=1;
					led.b.b3=1;	
				}
				else if(guangqiang==3)
				{
					led.b.b0=0;
					led.b.b1=0;
					led.b.b2=0;
					led.b.b3=1;	
				}
				else if(guangqiang==4)
				{
					led.b.b0=0;
					led.b.b1=0;
					led.b.b2=0;
					led.b.b3=0;	
				}
			}
			else
			{
				led.b.b0=1;
				led.b.b1=1;
				led.b.b2=1;
				led.b.b3=1;
			}
			
			if(yundongzhuangtai==1)
			{
				led.b.b7=1;
				time100ms=0;
			}
			else if(yundongzhuangtai==2)
			{
				time100ms=0;
				led.b.b7=0;
			}
			else if(yundongzhuangtai==3)
			{
				time100ms++;
				if(time100ms>=10)
				{
					led.b.b7=!led.b.b7;
					time100ms=0;
				}
			}
			
			if(jiejin==1&&gaowen==1)
			{
				buzzer.b.b4=1;
				if(xihe_flag==0)
				{
					xihe++;
					xihe_flag=1;
				}
			}
			else
			{
				xihe_flag=0;
				buzzer.b.b4=0;
			}
			
			drive(buzzer.hex,0xa0);
			drive(led.hex,0x80);
		}
	}
}

void main()
{
	system_init();
	Timer2_Init();
	Counter0_Init();
	
	temp=(u32)(temp_read()*10000);
	Delayms(850);

	while(1)
	{
		ad_process();
		temp_process();
		key16_process();
		csb_process();
		smg_process();
		led_process();
	}
}

void Timer2_Isr(void) interrupt 12
{
	key16_count++;
	ad_count++;
	temp_count++;
	csb_count++;
	led_count++;
	
	smg_display();
}