#include "system.h"

hextobin led,buzzer;


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

u16 ne555_date;

void smg_pinlv()
{
	u8 weishu;
	u8 i=7;
	smg[0]=Seg_Table[15];
	smg[1]=0xff;
	smg[2]=0xff;
	
	if(ne555_date/100==0)		weishu=2;
	else if(ne555_date/1000==0)		weishu=3;
	else if(ne555_date/10000==0)		weishu=4;
	else weishu=5;
	switch(weishu)
	{
		case 5:
		smg[3]=Seg_Table[ne555_date/10000%10];
		smg[4]=Seg_Table[ne555_date/1000%10];
		smg[5]=Seg_Table[ne555_date/100%10];
		smg[6]=Seg_Table[ne555_date/10%10];
		smg[7]=Seg_Table[ne555_date%10];
		break;
		
		case 4:
		smg[3]=0xff;
		smg[4]=Seg_Table[ne555_date/1000%10];
		smg[5]=Seg_Table[ne555_date/100%10];
		smg[6]=Seg_Table[ne555_date/10%10];
		smg[7]=Seg_Table[ne555_date%10];
		break;
		
		case 3:
		smg[3]=0xff;
		smg[4]=0xff;
		smg[5]=Seg_Table[ne555_date/100%10];
		smg[6]=Seg_Table[ne555_date/10%10];
		smg[7]=Seg_Table[ne555_date%10];
		break;
		case 2:
		smg[3]=0xff;
		smg[4]=0xff;
		smg[5]=0xff;
		smg[6]=Seg_Table[ne555_date/10%10];
		smg[7]=Seg_Table[ne555_date%10];
		break;
	}
}
void smg_zhouqi()
{
	u8 weishu;
	u16 zq=1000000/ne555_date;
	smg[0]=0xc8;
	smg[1]=0xff;
	smg[2]=0xff;
	
	if(zq/100==0)		weishu=2;
	else if(zq/1000==0)		weishu=3;
	else if(zq/10000==0)		weishu=4;
	else weishu=5;
	switch(weishu)
	{
		case 5:
			smg[3]=Seg_Table[zq/10000%10];
			smg[4]=Seg_Table[zq/1000%10];
			smg[5]=Seg_Table[zq/100%10];
			smg[6]=Seg_Table[zq/10%10];
			smg[7]=Seg_Table[zq%10];
			break;
		case 4:
			smg[3]=0xff;
			smg[4]=Seg_Table[zq/1000%10];
			smg[5]=Seg_Table[zq/100%10];
			smg[6]=Seg_Table[zq/10%10];
			smg[7]=Seg_Table[zq%10];
			break;
		case 3:
			smg[3]=0xff;
			smg[4]=0xff;
			smg[5]=Seg_Table[zq/100%10];
			smg[6]=Seg_Table[zq/10%10];
			smg[7]=Seg_Table[zq%10];
			break;
		case 2:
			smg[3]=0xff;
			smg[4]=0xff;
			smg[5]=0xff;
			smg[6]=Seg_Table[zq/10%10];
			smg[7]=Seg_Table[zq%10];
			break;
	}
	
}
u8 flag=1;
u16 ad_05;
void smg_dianya()
{
	smg[0]=0xc1;
	smg[1]=0xbf;
	smg[2]=Seg_Table[flag];
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=Seg_Table[ad_05/100]&0x7f;
	smg[6]=Seg_Table[ad_05/10%10];
	smg[7]=Seg_Table[ad_05%10];
}
u8 key4_count;
u8 mode=1;
u16 time,pinlv_temp;
u8 dianya_temp;
u8 ch1,ch3;
u8 haha;
void key4_process()
{
	if(key4_count>=10)
	{
		key4_count=0;
		key4();
		if(trg4&0x08)					//s4
		{
			mode++;
			if(mode==4)
				mode=1;
		}
		if(trg4&0x04)		//s5
		{
			if(mode==3)
			{
				flag+=2;
				if(flag==5)
					flag=1;
			}
		}
		if(trg4&0x02)					//s6
		{
			dianya_temp=ch3;
		}
		if(trg4&0x01)					//s7
		{
			pinlv_temp=ne555_date;
		}
		if(cont4&0x01)
		{
			time++;
		}
		if(cont4==0&&trg4==0)
		{
			if(time>=100)
			{
				haha=!haha;
			}
			time=0;
		}
	}
}
void led_process()
{
	if(haha==0)
	{
		if(mode==1)
			led.b.b2=0;
		else
			led.b.b2=1;
		
		
		if(mode==2)
			led.b.b3=0;
		else
			led.b.b3=1;
		
		
		if(mode==3)
			led.b.b4=0;
		else
			led.b.b4=1;
		
		if(pinlv_temp<ne555_date)
			led.b.b1=0;
		else
			led.b.b1=1;
		
		if(dianya_temp<ch3)
			led.b.b0=0;
		else
			led.b.b0=1;
	}
	else
		led.hex=0xff;
	
	drive(led.hex,0x80);
}
void system_int()
{
	drive(0,0xa0);
	led.hex=0xff;
	drive(led.hex,0x80);
	smg_int();
	smg_display();
	
}
u8 ad_count;
void AD_process()
{
	if(ad_count>=10)
	{
		ad_count=0;
		AD_read(0x41);
		ch1=AD_read(0x41);
		AD_read(0x43);
		ch3=AD_read(0x43);
		if(flag==1)
			ad_05=ch1*100/51;
		if(flag==3)
			ad_05=ch3*100/51;
	}
}
u16 ne555_count;
void NE555_process()
{
	if(ne555_count>=1000)
	{
		ne555_count=0;
		ne555_date=(TH0<<8)|TL0;
		TL0=0;
		TH0=0;
	}
}

void main()
{
	system_int();
	Timer2_Init();
	Counter0_Init();
	NE555_process();

	while(1)
	{
		key4_process();
		AD_process();
		NE555_process();
		if(mode==1)
		{
			smg_pinlv();
		}
		
		if(mode==2)
		{
			smg_zhouqi();
		}
		
		if(mode==3)
		{
			smg_dianya();
		}
		
	}
}
u8 led_count;
void Timer2_Isr(void) interrupt 12
{
	ad_count++;
	ne555_count++;
	key4_count++;
	
	
	led_count++;
	if(ne555_date)
	{
		if(led_count>=100)
		{
			led_count=0;
			led_process();
		}
			smg_display();
	}
}

