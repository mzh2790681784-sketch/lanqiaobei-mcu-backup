#include "system.h"

hextobin led,buzzer;

u8 setjuli=40;
u8 setwendu=30;
u8 da=10;
u16 jilujuli;

u8 flag;
u8 mode1=1;
u8 mode2,mode3;
u8 mode4=1;
u8 time2;
u16 haha;
void Timer2_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xFB;			//定时器时钟12T模式
	T2L = 0x18;				//设置定时初始值
	T2H = 0xFC;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时
	IE2 |= 0x04;			//使能定时器2中断
	EA=1;
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
u32 temp;
void temp_process()
{
	if(temp_count>=100)
	{
		temp_count=0;
		temp=(u32)(temp_read()*10000);
	}
}

u16 csb_count;
u16 distance;
void csb_process()
{
	if(csb_count>=200)
	{
		csb_count=0;
		distance=csb_get();
	}
}

u8 da_count;
void da_process()
{
	if(da_count>=2)
	{
		da_count=0;
		DA_write(51*haha);
	}
}

void chushi()
{
	mode2=0;
	setjuli=40;
	setwendu=30;
	jiaozhun=0;
	jiezhi=340.0;
	da=10;
}

u8 key16_count;
void key16_process()
{
	if(key16_count>=10)
	{
		key16_count=0;
		key16();
		if(flag==0)
		{
			if(trg16==0x88)		//s4
			{
				mode1++;
				if(mode1==4)
					mode1=1;
			}
			if(trg16==0x84)		//s5
			{
				if(mode1==1)
				{
					mode2=!mode2;
				}
				if(mode1==2)
				{
					mode3=!mode3;
				}
				if(mode1==3)
				{
					mode4++;
					if(mode4==4)
						mode4=1;
				}
			}
			if(trg16==0x48)		//s8
			{
				if(mode1==2)
				{
					if(mode3==0)
						setjuli+=10;
					if(mode3==1)
						setwendu+=1;
					if(setjuli==100)
						setjuli=90;
					if(setwendu==81)
						setwendu=80;
				}
				if(mode1==3)
				{
					if(mode4==1)
					{
						jiaozhun+=5;
						if(jiaozhun==95)
							jiaozhun=90;
					}
					if(mode4==2)
					{
						jiezhi+=10;
						if(jiezhi==10000)
							jiezhi=9990;
					}
					if(mode4==3)
					{
						da+=1;
						if(da==21)
							da=20;
					}
				}
				if(mode1==1)
				{
					flag=1;
				}
			}
			if(trg16==0x44)		//s9
			{
				if(mode1==2)
				{
					
					if(mode3==0)
						setjuli-=10;
					if(mode3==1)
					{
						if(setwendu==0)
							setwendu=1;
						setwendu-=1;
					}
					if(setjuli==0)
						setjuli=10;
					
				}
				if(mode1==3)
				{
					if(mode4==1)
					{
						jiaozhun-=5;
						if(jiaozhun==-95)
							jiaozhun=-90;
					}
					if(mode4==2)
					{
						jiezhi-=10;
						if(jiezhi==0)
							jiezhi=10;
					}
					if(mode4==3)
					{
						da-=1;
						if(da==0)
							da=1;
					}
				}
				if(mode1==1)
				{
					if(jilujuli!=0)
					{
						if(jilujuli<=10)
							haha=da*255/10;
						else if(jilujuli>=90)
							haha=255;
						else
							haha=(50-da)*(jilujuli-10)/800+da/10;
					}
				}
			}
			if(cont16==0x40)
			{
				time2++;
				if(time2>=200)
				{
					time2=201;
					chushi();
				}
			}
			if(trg16==0&&cont16==0)
				time2=0;
		}
	}
}
void smg_ceju()
{
	smg[0]=Seg_Table[temp/100000];
	smg[1]=Seg_Table[temp/10000%10]&0x7f;
	smg[2]=Seg_Table[temp/1000%10];
	smg[3]=0xbf;
	if(mode2==0)
	{
		if(distance>=1000)
		{
			smg[4]=Seg_Table[distance/1000];
			smg[5]=Seg_Table[distance/100%10];
			smg[6]=Seg_Table[distance/10%10];
			smg[7]=Seg_Table[distance%10];
		}
		else if(distance>=100)
		{
			smg[4]=0xff;
			smg[5]=Seg_Table[distance/100];
			smg[6]=Seg_Table[distance/10%10];
			smg[7]=Seg_Table[distance%10];
		}
		else if(distance>=10)
		{
			smg[4]=0xff;
			smg[5]=0xff;
			smg[6]=Seg_Table[distance/10];
			smg[7]=Seg_Table[distance%10];
		}
		else
		{
			smg[4]=0xff;
			smg[5]=0xff;
			smg[6]=0xff;
			smg[7]=Seg_Table[distance];
		}
	}
	if(mode2==1)
	{
		if(distance>=1000)
		{
			smg[4]=Seg_Table[distance/1000];
			smg[5]=Seg_Table[distance/100%10]&0x7f;
			smg[6]=Seg_Table[distance/10%10];
			smg[7]=Seg_Table[distance%10];
		}
		else if(distance>=100)
		{
			smg[4]=0xff;
			smg[5]=Seg_Table[distance/100]&0x7f;
			smg[6]=Seg_Table[distance/10%10];
			smg[7]=Seg_Table[distance%10];
		}
		else if(distance>=10)
		{
			smg[4]=0xff;
			smg[5]=Seg_Table[0]&0x7f;
			smg[6]=Seg_Table[distance/10];
			smg[7]=Seg_Table[distance%10];
		}
		else
		{
			smg[4]=0xff;
			smg[5]=Seg_Table[0]&0x7f;
			smg[6]=Seg_Table[0];
			smg[7]=Seg_Table[distance];
		}
	}
}
void smg_canshu1()
{
	smg[0]=0x8c;
	smg[1]=Seg_Table[1];
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=0xff;
	smg[6]=Seg_Table[setjuli/10];
	smg[7]=Seg_Table[setjuli%10];
}
void smg_canshu2()
{
	smg[0]=0x8c;
	smg[1]=Seg_Table[2];
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=0xff;
	smg[6]=Seg_Table[setwendu/10];
	smg[7]=Seg_Table[setwendu%10];
}
void smg_jiaozhun()
{
	smg[0]=0x8e;
	smg[1]=Seg_Table[1];
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	if(jiaozhun>=0)
	{
		if(jiaozhun>=100)
		{
			smg[5]=Seg_Table[jiaozhun/100];
			smg[6]=Seg_Table[jiaozhun/10%10];
			smg[7]=Seg_Table[jiaozhun%10];
		}
		else if(jiaozhun>=10)
		{
			smg[5]=0xff;
			smg[6]=Seg_Table[jiaozhun/10];
			smg[7]=Seg_Table[jiaozhun%10];
		}
		else if(jiaozhun>=0)
		{
			smg[5]=0xff;
			smg[6]=0xff;
			smg[7]=Seg_Table[jiaozhun];
		}
	}
	if(jiaozhun<0)
	{
		jiaozhun=-jiaozhun;
		if(jiaozhun>=10)
		{
			smg[5]=0xbf;
			smg[6]=Seg_Table[jiaozhun/10];
			smg[7]=Seg_Table[jiaozhun%10];
		}
		else if(jiaozhun>=0)
		{
			smg[5]=0xff;
			smg[6]=0xbf;
			smg[7]=Seg_Table[jiaozhun];
		}
		jiaozhun=-jiaozhun;
	}
}
void smg_jiezhi()
{
	smg[0]=0x8e;
	smg[1]=Seg_Table[2];
	smg[2]=0xff;
	smg[3]=0xff;
	if(jiezhi>=1000)
	{
		smg[4]=Seg_Table[(u16)jiezhi/1000];
		smg[5]=Seg_Table[(u16)jiezhi/100%10];
		smg[6]=Seg_Table[(u16)jiezhi/10%10];
		smg[7]=Seg_Table[(u16)jiezhi%10];
	}
	else if(jiezhi>=100)
	{
		smg[4]=0xff;
		smg[5]=Seg_Table[(u16)(jiezhi/100)];
		smg[6]=Seg_Table[(u16)jiezhi/10%10];
		smg[7]=Seg_Table[(u16)jiezhi%10];
	}
	else if(jiezhi>=10)
	{
		smg[4]=0xff;
		smg[5]=0xff;
		smg[6]=Seg_Table[(u16)jiezhi/10];
		smg[7]=Seg_Table[(u16)jiezhi%10];
	}
}
void smg_da()
{
	smg[0]=0x8e;
	smg[1]=Seg_Table[3];
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=0xff;
	if(da>=10)
	{
		smg[6]=Seg_Table[da/10]&0x7f;
		smg[7]=Seg_Table[da%10];
	}
	else if(da>=0)
	{
		smg[6]=Seg_Table[0]&0x7f;
		smg[7]=Seg_Table[da];
	}
}
u8 led_count;
void led_process()
{
	if(led_count>=10)
	{
		led_count=0;

		if(mode1==1)
		{
			if(distance>=255)
				led.hex=0;
			else
				led.hex=~distance;
		}
		if(mode1==2)
		{
			led.hex=0xff;
			led.b.b7=0;
		}
		if(mode1==3)
		{
			led.hex|=0xfe;
			led.b.b0=!led.b.b0;
		}
		if((setjuli-5)<=distance&&distance<=(setjuli+5)&&temp/10000<=setwendu)
			buzzer.b.b4=1;
		else
			buzzer.b.b4=0;
		drive(led.hex,0x80);
		drive(buzzer.hex,0xa0);
	}
}
void main()
{
	system_int();
	while(temp_read()==85);
	Timer2_Init();
	while(1)
	{
		key16_process();
		temp_process();
		csb_process();
		da_process();
		if(mode1==1)
		{
			mode4=1;
			smg_ceju();
		}
		if(mode1==2)
		{
			mode2=0;
			if(mode3==0)
				smg_canshu1();
			if(mode3==1)
				smg_canshu2();
		}
		if(mode1==3)
		{
			mode3=0;
			if(mode4==1)
			{
				smg_jiaozhun();
			}
			if(mode4==2)
			{
				smg_jiezhi();
			}
			if(mode4==3)
			{
				smg_da();
			}
		}
	}
}
u16 time1;
void Timer2_Isr(void) interrupt 12
{
	key16_count++;
	temp_count++;
	csb_count++;
	da_count++;
	led_count++;
	if(flag==1)
	{
		time1++;
		if(time1>=6000)
		{
			flag=0;
			time1=0;
			jilujuli=distance;
		}
	}
	smg_display();
	led_process();
}
