#include "system.h"

hextobin led,buzzer;
u8 mode=1;
u32 temp_date;
u8 hour,min,sec;
u8 canshu=23;
u8 flag;

u8 key_count;
void key_process()
{
	if(key_count>=10)
	{
		key_count=0;
		key16();
		if(trg16==0x28)			//s12
		{
			mode++;
			if(mode==4)
				mode=1;
		}
		if(trg16==0x24)			//s13
		{
			flag=!flag;
			buzzer.b.b4=0;
		}	
		if(trg16==0x18)			//s16
		{
			if(mode==3)
				canshu++;
		}
		if(trg16==0x14)			//s17
		{
			if(mode==3)
				canshu--;
		}		
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

void smg_wendu()
{
	smg[0]=0xc1;
	smg[1]=Seg_Table[mode];
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=Seg_Table[temp_date/100000];
	smg[6]=Seg_Table[temp_date/10000%10]&0x7f;
	smg[7]=Seg_Table[temp_date/1000%10];
}
void smg_shijian1()
{
	smg[0]=0xc1;
	smg[1]=Seg_Table[mode];
	smg[2]=0xff;
	smg[3]=Seg_Table[hour/10];
	smg[4]=Seg_Table[hour%10];
	smg[5]=0xbf;
	smg[6]=Seg_Table[min/10];
	smg[7]=Seg_Table[min%10];
}
void smg_shijian2()
{
	smg[0]=0xc1;
	smg[1]=Seg_Table[mode];
	smg[2]=0xff;
	smg[3]=Seg_Table[min/10];
	smg[4]=Seg_Table[min%10];
	smg[5]=0xbf;
	smg[6]=Seg_Table[sec/10];
	smg[7]=Seg_Table[sec%10];
}

void smg_canshu()
{
	smg[0]=0xc1;
	smg[1]=Seg_Table[mode];
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=0xff;
	smg[6]=Seg_Table[canshu/10];
	smg[7]=Seg_Table[canshu%10];
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
		temp_date=(u32)(temp_read()*10000);
	}
}
u8 ds1302_count;
void ds1302_process()
{
	if(ds1302_count>=100)
	{
		ds1302_count=0;
		sec=bcdtodec(Read_Ds1302_Byte(0x81));
		min=bcdtodec(Read_Ds1302_Byte(0x83));
		hour=bcdtodec(Read_Ds1302_Byte(0x85));
	}
	
}
u8 buzzer_count;
void buzzer_process()
{
	if(buzzer_count>=50)
	{
		buzzer_count=0;
		if(flag==0)
		{
			led.b.b1=0;
			if(temp_date/10000>=canshu)
				buzzer.b.b4=1;
			else
				buzzer.b.b4=0;
		}
		else
		{
			led.b.b1=1;
			if(sec==0&&min==0)
				buzzer.b.b4=1;
			if(sec==5)
				buzzer.b.b4=0;
		}
		drive(buzzer.hex,0xa0);
		drive(led.hex,0x80);
	}
}
u8 led_count;
void led_process()
{
	if(led_count>=100)
	{
		led_count=0;
		if(buzzer.b.b4==1)
		{
			led.b.b2=!led.b.b2;
		}
		else
			led.b.b2=1;
		
		if(sec==0&&min==0)
			led.b.b0=0;
		if(sec==5)
			led.b.b0=1;
		
	}
	drive(led.hex,0x80);
}
void main()
{
	system_int();
	DS1302_set(23,59,45);
	sec=bcdtodec(Read_Ds1302_Byte(0x81));
	min=bcdtodec(Read_Ds1302_Byte(0x83));
	hour=bcdtodec(Read_Ds1302_Byte(0x85));
	Timer2_Init();
	temp_read();
	Delay_ms(700);
	while(1)
	{
		key_process();
		temp_process();
		ds1302_process();
		buzzer_process();
		
		if(mode==1)
		{
			smg_wendu();
		}
		if(mode==2)
		{
			if(cont16==0x14)
				smg_shijian2();
			else
				smg_shijian1();
		}
		if(mode==3)
		{
			smg_canshu();
		}
	}
}

void Timer2_Isr(void) interrupt 12
{
	key_count++;
	temp_count++;
	ds1302_count++;
	buzzer_count++;
	led_count++;
	
	smg_display();
	led_process();
}