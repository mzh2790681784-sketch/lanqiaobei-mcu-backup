#include "system.h"

hextobin led,buzzer;
u8 cishu;
u8 wenduset1=30,juliset1=35;
u8 wenduset2=30,juliset2=35;
u8 mode1,mode2=1,mode3;
u8 da_flag;
void Timer2_Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x04;			//定时器时钟1T模式
	T2L = 0x20;				//设置定时初始值
	T2H = 0xD1;				//设置定时初始值
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
u8 csb_count;
u16 distance;
void csb_process()
{
	if(csb_count>=100)
	{
		csb_count=0;
		distance=csb_get();
		if(distance>=99)
			distance=99;
	}
}
u8 temp_count;
u32 temp;
void temp_process()
{
	if(temp_count>=100)
	{
		temp_count=0;
		temp=(u32)(temp_get()*10000);
	}
}

u8 da_count;
void da_process()
{
	if(da_count>=2)
	{
		da_count=0;
		if(da_flag==0)
		{
			if(distance<=juliset2)
				da_write(102);
			else
				da_write(204);
		}
		if(da_flag==1)
			da_write(20);
			
	}
}

void smg_wendu()
{
	smg[0]=Seg_Table[12];
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=Seg_Table[temp/100000];
	smg[5]=Seg_Table[temp/10000%10]&0x7f;
	smg[6]=Seg_Table[temp/1000%10];
	smg[7]=Seg_Table[temp/100%10];
}
void smg_juli()
{
	smg[0]=0xc7;
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=0xff;
	smg[6]=Seg_Table[distance/10];
	smg[7]=Seg_Table[distance%10];
}
void smg_biangeng()
{
	smg[0]=0x98;
	smg[1]=0xff;
	smg[2]=0xff;
	if(cishu>=10000)
	{
		smg[3]=Seg_Table[cishu/10000];
		smg[4]=Seg_Table[cishu/1000%10];
		smg[5]=Seg_Table[cishu/100%10];
		smg[6]=Seg_Table[cishu/10%10];
		smg[7]=Seg_Table[cishu%10];
	}
	else if(cishu>=1000)
	{
		smg[3]=0xff;
		smg[4]=Seg_Table[cishu/1000];
		smg[5]=Seg_Table[cishu/100%10];
		smg[6]=Seg_Table[cishu/10%10];
		smg[7]=Seg_Table[cishu%10];
	}
	else if(cishu>=100)
	{
		smg[3]=0xff;
		smg[4]=0xff;
		smg[5]=Seg_Table[cishu/100];
		smg[6]=Seg_Table[cishu/10%10];
		smg[7]=Seg_Table[cishu%10];
	}
	else if(cishu>=10)
	{
		smg[3]=0xff;
		smg[4]=0xff;
		smg[5]=0xff;
		smg[6]=Seg_Table[cishu/10];
		smg[7]=Seg_Table[cishu%10];
	}
	else
	{
		smg[3]=0xff;
		smg[4]=0xff;
		smg[5]=0xff;
		smg[6]=0xff;
		smg[7]=Seg_Table[cishu];
	}
}
void smg_wenduset()
{
	smg[0]=0x8c;
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=Seg_Table[1];
	smg[4]=0xff;
	smg[5]=0xff;
	smg[6]=Seg_Table[wenduset1/10];
	smg[7]=Seg_Table[wenduset1%10];
}
void smg_juliset()
{
	smg[0]=0x8c;
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=Seg_Table[2];
	smg[4]=0xff;
	smg[5]=0xff;
	smg[6]=Seg_Table[juliset1/10];
	smg[7]=Seg_Table[juliset1%10];
}
u8 key16_count;
u8 time1,time2;
void key16_process()
{
	if(key16_count>=10)
	{
		key16_count=0;
		key16();
		if(trg16==0x18)		//s16
		{			
			
			if(mode1==1&&mode3==0)
			{
				if(wenduset1==0)
					wenduset1=2;
				wenduset1-=2;
				
			}
			if(mode1==1&&mode3==1)
			{
				if(juliset1==0)
					juliset1=5;
				juliset1-=5;
			}

		}
		if(trg16==0x14)		//s17
		{
			if(mode1==1&&mode3==0)
				wenduset1+=2;
			if(mode1==1&&mode3==1)
				juliset1+=5;
			if(wenduset1==100)
				wenduset1=98;
			if(juliset1==100)
				juliset1=95;
		}
		if(cont16==0x28)		//s12
		{
			time1++;
			if(time1>=100)
			{
				if(time1==100)
				{
					cishu=0;
					at24c02_write(cishu,0x00);
				}
				if(time1>=110)
					time1=110;
			}
		}
		if(cont16==0x24)		//s13
		{
			time2++;
			if(time2>=100)
			{
				if(time2==100)
					da_flag=!da_flag;
				if(time2>=110)
					time2=110;
			}
		}
		if(trg16==0&&cont16==0)
		{
			if(time1!=0&&time1<100)
			{
				if(mode1==0)
				{
					mode2++;
					if(mode2==4)
						mode2=1;
				}
				if(mode1==1)
				{
					mode3=!mode3;
				}
			}
			if(time2!=0&&time2<100)
				mode1=!mode1;
			time1=0;
			time2=0;
		}
	}
}
void ck_process()
{
	if(ck_flag)
	{
		if(ck_string[0]=='S'&&ck_string[1]=='T'&&ck_string[2]=='\r'&&ck_string[3]=='\n')
		{
			SendData('$');
			SendData(distance/10+'0');
			SendData(distance%10+'0');
			SendData(',');
			SendData(temp/100000+'0');
			SendData(temp/10000%10+'0');
			SendData('.');
			SendData(temp/1000%10+'0');
			SendData(temp/100%10+'0');
			SendData('\r');
			SendData('\n');
		}
		else if(ck_string[0]=='P'&&ck_string[1]=='A'&&ck_string[2]=='R'&&ck_string[3]=='A'&&ck_string[4]=='\r'&&ck_string[5]=='\n')
		{
			SendData('#');
			SendData(wenduset2/10+'0');
			SendData(wenduset2%10+'0');
			SendData(',');
			SendData(juliset2/10+'0');
			SendData(juliset2%10+'0');
			SendData('\r');
			SendData('\n');
		}
		else
		{
			SendString("ERROR\r\n");
		}
		ck_flag=0;
	}
}
u8 led_count;
void led_process()
{
	if(led_count>=10)
	{
		led_count=0;
		if(temp/10000>=wenduset2)
			led.b.b0=0;
		else
			led.b.b0=1;
		if(distance>=juliset2)
			led.b.b1=0;
		else
			led.b.b1=1;
		if(da_flag==0)
			led.b.b2=0;
		else
			led.b.b2=1;
		
		drive(led.hex,0x80);
	}
}
void main()
{
	while(temp_get()==85);
	temp=(u32)(temp_get()*10000);
	system_int();
	Timer2_Init();
	Uart1Init();
	while(1)
	{
		da_process();
		ck_process();
		key16_process();
		csb_process();
		temp_process();
		if(mode1==0)
		{
			mode3=0;
			if(wenduset2!=wenduset1&&juliset2!=juliset1)
			{
				cishu++;
				wenduset2=wenduset1;
				juliset2=juliset1;
				at24c02_write(cishu,0x00);
			}
			else if(wenduset2!=wenduset1)
			{
				cishu++;
				wenduset2=wenduset1;
				at24c02_write(cishu,0x00);
			}
			else if(juliset2!=juliset1)
			{
				cishu++;
				juliset2=juliset1;
				at24c02_write(cishu,0x00);
			}
			
			if(mode2==1)
			{
				smg_wendu();
			}
			if(mode2==2)
			{
				smg_juli();
			}
			if(mode2==3)
			{
				smg_biangeng();
			}
		}
		if(mode1==1)
		{
			mode2=1;
			if(mode3==0)
			{
				smg_wenduset();
			}
			if(mode3==1)
			{
				smg_juliset();
			}
		}
	}
}

void Timer2_Isr(void) interrupt 12
{
	csb_count++;
	temp_count++;
	da_count++;
	key16_count++;
	led_count++;
	
	smg_display();
	led_process();
	ck_relax();
}