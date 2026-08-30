#include "system.h"

hextobin led,buzzer;
u8 maxwendu,maxshidu;
u16 sumwendu,sumshidu;
u8 chufa;
u8 chufahour,chufamin;
u8 sec,min,hour;
u8 wendu1,shidu1;
u8 wendu1temp[2],shidu1temp[2];
u8 shidu;
u8 mode1=1;
u8 mode2=1;
u8 flag;
u16 wenshidu_count;
u8 wenducanshu=30;
u16 ch1_count;
u8 ch1_temp;

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
void smg_wenduhuixian()
{
	smg[0]=Seg_Table[12];
	smg[1]=0xff;
	if(chufa!=0)
	{
		smg[2]=Seg_Table[maxwendu/10];
		smg[3]=Seg_Table[maxwendu%10];
		smg[4]=0xbf;
		smg[5]=Seg_Table[(sumwendu*10/chufa)/100];
		smg[6]=Seg_Table[(sumwendu*10/chufa)/10%10]&0x7f;
		smg[7]=Seg_Table[(sumwendu*10/chufa)%10];
	}
	else
	{
		smg[2]=0xff;
		smg[3]=0xff;
		smg[4]=0xff;
		smg[5]=0xff;
		smg[6]=0xff;
		smg[7]=0xff;
	}
}
void smg_shiduhuixian()
{
	smg[0]=0x89;
	smg[1]=0xff;
	if(chufa!=0)
	{
		smg[2]=Seg_Table[maxshidu/10];
		smg[3]=Seg_Table[maxshidu%10];
		smg[4]=0xbf;
		smg[5]=Seg_Table[(sumshidu*10/chufa)/100];
		smg[6]=Seg_Table[(sumshidu*10/chufa)/10%10]&0x7f;
		smg[7]=Seg_Table[(sumshidu*10/chufa)%10];
	}
	else
	{
		smg[2]=0xff;
		smg[3]=0xff;
		smg[4]=0xff;
		smg[5]=0xff;
		smg[6]=0xff;
		smg[7]=0xff;
	}
}
void smg_shijianhuixian()
{
	smg[0]=Seg_Table[15];
	smg[1]=Seg_Table[chufa/10];
	smg[2]=Seg_Table[chufa%10];
	if(chufa!=0)
	{
		smg[3]=Seg_Table[chufahour/10];
		smg[4]=Seg_Table[chufahour%10];
		smg[5]=0xbf;
		smg[6]=Seg_Table[chufamin/10];
		smg[7]=Seg_Table[chufamin%10];
	}
	else
	{
		smg[3]=0xff;
		smg[4]=0xff;
		smg[5]=0xff;
		smg[6]=0xff;
		smg[7]=0xff;
	}
}
void smg_canshu()
{
	smg[0]=0x8c;
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=0xff;
	smg[6]=Seg_Table[wenducanshu/10];
	smg[7]=Seg_Table[wenducanshu%10];
}
void smg_wenshidu()
{
	smg[0]=0x86;
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=Seg_Table[wendu1/10];
	smg[4]=Seg_Table[wendu1%10];
	smg[5]=0xbf;
	if(shidu!=0)
	{
		smg[6]=Seg_Table[shidu1/10];
		smg[7]=Seg_Table[shidu1%10];
	}
	else
	{
		smg[6]=0x88;
		smg[7]=0x88;
	}
}

void system_int()
{
	drive(0,0xa0);
	led.hex=0xff;
	drive(led.hex,0x80);
	smg_int();
	smg_display();
}
u32 temp;
u8 temp_count;
void temp_process()
{
	if(temp_count>=100)
	{
		temp_count=0;
		temp=(u32)(temp_read()*10000);
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

u8 ad_count;
u8 ch1,ch3;
void ad_process()
{
	if(ad_count>=10)
	{
		ad_count=0;
		AD_read(0x41);
		ch1=AD_read(0x41);
		AD_read(0x43);
		ch3=AD_read(0x43);
	}
}
u16 ne555_count;
u16 ne555_date;
void ne555_process()
{
	if(ne555_count>=1000)
	{
		ne555_count=0;
		ne555_date=(TH0<<8)|TL0;
		TL0=0;
		TH0=0;
		
		if(ne555_date<=200||ne555_date>=2000)
			shidu=0;
		else
			shidu=0.04*ne555_date+2;
	}
}

u8 key16_count;
u8 s9time;
void key16_process()
{
	if(key16_count>=1)
	{
		key16_count=0;
		if(flag==0)
		{
			key16();
			if(trg16==0x88)		//s4
			{
				mode1++;
				if(mode1==4)
					mode1=1;
			}
			if(trg16==0x84)		//s5
			{
				if(mode1==2)
				{
					mode2++;
					if(mode2==4)
						mode2=1;
				}
			}
			if(trg16==0x48)		//s8
			{
				if(mode1==3)
					wenducanshu++;
			}
			if(trg16==0x44)		//s9
			{
				if(mode1==3)
					wenducanshu--;
			}
			if(mode1==2&&mode2==3&&cont16==0x44)
				s9time++;
			if(cont16==0&&trg16==0&&s9time>=150)
			{
				s9time=0;
				chufa=0;
				maxwendu=0;
				maxshidu=0;
				sumwendu=0;
				sumshidu=0;
			}
		}
	}
}
void ch1_process()
{
	if(ch1_count>=200)
	{
		ch1_count=0;
		if(flag==0)
		{
			if(ch1_temp>=130&&ch1<=100)
			{
				flag=1;
				wendu1=temp/10000;
				shidu1=shidu;
				if(shidu1!=0)
				{
					wendu1temp[0]=wendu1temp[1];
					wendu1temp[1]=wendu1;
					
					shidu1temp[0]=shidu1temp[1];
					shidu1temp[1]=shidu1;
					chufahour=hour;
					chufamin=min;
					chufa++;
					sumwendu+=wendu1;
					sumshidu+=shidu1;
					if(wendu1>maxwendu)
						maxwendu=wendu1;
					if(shidu1>maxshidu)
						maxshidu=shidu1;
				}
			}
		}
		ch1_temp=ch1;
	}
	if(flag==1)
	{
		wenshidu_count++;
		if(wenshidu_count>=3000)
		{
			flag=0;
			wenshidu_count=0;
		}
	}
}
u8 led_count;
void led_process()
{
	if(led_count>=100)
	{
		led_count=0;
		if(flag==0)
		{
			led.b.b2=1;
			if(mode1==1)
				led.b.b0=0;
			else
				led.b.b0=1;
			
			if(mode1==2)
				led.b.b1=0;
			else
				led.b.b1=1;
		}
		
		if(flag==1)
		{
			led.b.b0=1;
			led.b.b1=1;
			led.b.b2=0;
		}
		
		if(temp/10000>wenducanshu)
			led.b.b3=!led.b.b3;
		else
			led.b.b3=1;
		
		if(flag==1&&shidu1==0)
			led.b.b4=0;
		if(flag==1&&shidu1!=0)
			led.b.b4=1;
		if(chufa>=2&&wendu1temp[1]>wendu1temp[0]&&shidu1temp[1]>shidu1temp[0])
			led.b.b5=0;
		else
			led.b.b5=1;
		drive(led.hex,0x80);
	}
}
void main()
{
	system_int();
	Timer2_Init();
	Counter0_Init();
	ds1302_set(5,3,13);
	while(temp_read()==85);
	
	sec=bcdtodec(Read_Ds1302_Byte(0x81));
	min=bcdtodec(Read_Ds1302_Byte(0x83));
	hour=bcdtodec(Read_Ds1302_Byte(0x85));
	while(1)
	{
		key16_process();
		ad_process();
		ds1302_process();
		temp_process();
		led_process();
		
		if(flag==0)
		{
			if(mode1==1)
			{
				smg_shijian();
			}
			if(mode1==2)
			{
				if(mode2==1)
				{
					smg_wenduhuixian();
				}
				if(mode2==2)
				{
					smg_shiduhuixian();
				}
				if(mode2==3)
				{
					smg_shijianhuixian();
				}
			}
			if(mode1==3)
			{
				mode2=1;
				smg_canshu();
			}
		}
		if(flag==1)
		{
			smg_wenshidu();
		}
	}
}


void Timer2_Isr(void) interrupt 12
{
	key16_count++;
	ds1302_count++;
	ad_count++;
	ne555_count++;
	ch1_count++;
	temp_count++;
	led_count++;
	
//	if(led_count>=100)
//	{
//		led_count=0;
//		if(temp/10000>wenducanshu)
//			led.b.b3=!led.b.b3;
//		drive(led.hex,0x80);
//	}
	smg_display();
	ne555_process();
	ch1_process();
}