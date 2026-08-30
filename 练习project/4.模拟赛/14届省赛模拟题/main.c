#include "system.h"

hextobin led,buzzer;
u8 flag;
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
void smg_wendu()
{
	smg[0]=0xc1;
	smg[1]=Seg_Table[1];
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=Seg_Table[temp/100000];
	smg[6]=Seg_Table[temp/10000%10]&0x7f;
	smg[7]=Seg_Table[temp/1000%10];
	
}
u8 ad_count;
u8 ch3;
u16 ch3_05;
void ad_process()
{
	if(ad_count>=10)
	{
		ad_count=0;
		ch3=AD_read(0x43);
		ch3_05=ch3*100/51;
	}
}
void smg_dianya()
{
	smg[0]=0xc1;
	smg[1]=Seg_Table[2];
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=Seg_Table[ch3_05/100]&0x7f;
	smg[6]=Seg_Table[ch3_05/10%10];
	smg[7]=Seg_Table[ch3_05%10];
}
void system_init()
{
	drive(0,0xa0);
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
		if(cont16==0x88)	//s4
		{
			flag=1;
		}
		if(cont16==0x84)	//s5
		{
			flag=0;
		}
		if(cont16==0x28)	//s12
		{
			if(ck_date[0]=='A')
			{
				SendString("TEMP:");
				SendData(temp/100000+'0');
				SendData(temp/10000%10+'0');
				SendString(".");
				SendData(temp/1000%10+'0');
			}
			if(ck_date[0]=='B')
			{
				SendString("Voltage:");
				SendData(ch3_05/100+'0');
				SendString(".");
				SendData(ch3_05/10%10+'0');
				SendData(ch3_05%10+'0');
			}
		}
	}
}

void ck_process()
{
	if(ck_flag==1)
	{
		ck_date[0]=ck_string[0];
		ck_flag=0;
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


void main()
{
	Timer2_Init();
	Uart1Init();
	system_init();
	while(1)
	{
		key16_process();
		ck_process();
		ad_process();
		temp_process();
		if(ck_date[0]=='A')
		{
			smg_wendu();
		}
		if(ck_date[0]=='B')
		{
			smg_dianya();
		}
	}
}

void Timer2_Isr(void) interrupt 12
{
	key16_count++;
	temp_count++;
	ad_count++;
	
	smg_display();
}
