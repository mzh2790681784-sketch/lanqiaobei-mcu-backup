#include "SYSTEM.H"
hextobin led,buzzer;
u8 pwm_compare=5;
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
	TL0=TH0=0;
	TR0=1;
}
void Uart1Init(void)		//9600bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器时钟1T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xC7;			//设置定时初始值
	TH1 = 0xFE;			//设置定时初始值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
	ES=1;
	EA=1;
}

void system_init()
{
	led.hex=0xff;
	buzzer.hex=0;
	drive(led.hex,0x80);
	drive(buzzer.hex,0xa0);
	smg_display();
}

u8 key4_count;
u8 num;
void key4_process()
{
	if(key4_count>=10)
	{
		key4_count=0;
		key4();
		if(trg4&0x01)	//s7
		{
			num++;
			at24c02_wirte(0x00,num);
		}
		else if(trg4&0x02)	//s6
		{
			pwm_compare++;
			if(pwm_compare>10)
				pwm_compare=0;
		}
		else if(trg4&0x04)	//s5
		{
			num++;
		}
		else if(trg4&0x08)	//s4
		{
			num++;
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
		if(trg16==0x88)	//s7
		{
			num++;
		}
		if(trg16==0x12)	//s6
		{
			num++;
		}
		if(trg16==0x14)	//s5
		{
			num++;
		}
		if(trg16==0x18)	//s4
		{
			num++;
		}
	}
}
u8 csb_count;
u16 csb_dat;
void csb_process()
{
	if(csb_count>=100)
	{
		csb_count=0;
		csb_dat=csb_get();
	}
}
u8 ds18b20_count;
u32 temp;
void ds18b20_process()
{
	if(ds18b20_count>=10)
	{
		ds18b20_count=0;
		temp=(u32)(ds18b20_read()*10000);
	}
}

u8 ad_count;
u8 ch0,ch1,ch3;
void ad_process()
{
	if(ad_count>=10)
	{
		ad_count=0;
		ad_read(0x40);
		ch0=ad_read(0x40);
		ad_read(0x41);
		ch1=ad_read(0x41);
		ad_read(0x43);
		ch3=ad_read(0x43);
	}
}
u8 da_count;
void da_process()
{
	if(da_count>=2)
	{
		da_count=0;
		da_write(200);
	}
}
u8 ds1302_count;
u8 sec,min,hou;
void ds1302_process()
{
	if(ds1302_count>=200)
	{
		ds1302_count=0;
		sec=Read_Ds1302_Byte(0x81);
		min=Read_Ds1302_Byte(0x83);
		hou=Read_Ds1302_Byte(0x85);
	}
}
u16 ne555_dat;
u16 ne555_count;
void ne555_process()
{
	if(ne555_count>=1000)
	{
		ne555_count=0;
		ne555_dat=(TH0<<8)|TL0;
		TL0=TH0=0;
	}
}
void smg_process()
{
	smg[0]=smg_set[ne555_dat/10000];
	smg[1]=smg_set[ne555_dat/1000%10];
	smg[2]=smg_set[ne555_dat/100%10];
	smg[3]=smg_set[ne555_dat/10%10];
	smg[4]=smg_set[ne555_dat%10];
	smg[5]=0xbf;
	smg[6]=smg_set[sec/16];;
	smg[7]=smg_set[sec%16];;
}
//void smg_process()
//{
//	smg[0]=smg_set[hou/16];
//	smg[1]=smg_set[hou%16];
//	smg[2]=0xbf;
//	smg[3]=smg_set[min/16];;
//	smg[4]=smg_set[min%16];;
//	smg[5]=0xbf;
//	smg[6]=smg_set[sec/16];;
//	smg[7]=smg_set[sec%16];;
//}
void ck_process()
{
	if(ck_flag)
	{
		ck_flag=0;
		SendString(ck_string);
	}
}
u8 pwm_count=1;
void pwm_process()
{
	pwm_count=pwm_count%10+1;
	if(pwm_count<=pwm_compare)
		drive(0x00,0x80);
	else
		drive(0xff,0x80);
}
void main()
{
	system_init();
	Timer2_Init();
	counter0_init();
	Uart1Init();
	num=at24c02_read(0x00);
	ds1302_set(0x55,0x59,0x23,0x01,0x01,0x01,0x01);
	while(1)
	{
		key4_process();
//		key16_process();
		
		csb_process();
		ds18b20_process();
		ad_process();
		da_process();
		ds1302_process();
		smg_process();	
		ck_relax();
		ck_process();
	}
}

void Timer2_Isr(void) interrupt 12
{
	
	key4_count++;
	key16_count++;
	csb_count++;
	ds18b20_count++;
	ad_count++;
	da_count++;
	ds1302_count++;
	ne555_count++;
	ck_time++;
	
	
	ne555_process();
	smg_display();
	pwm_process();
}