#include "system.h"
hextobin led,buzzer;
u16 haha;
u8 naozhongshi,naozhongfen,naozhongmiao;
u8 sec,min,hour;
u8 realhour,realmin,realsec;
u8 mode1,mode2,flag;
void Timer2_Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x04;			//定时器时钟1T模式
	T2L = 0x20;				//设置定时初始值
	T2H = 0xD1;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时
	IE2 |= 0x04;			//使能定时器2中断
	EA=1;
}
u8 key_count;
void key4_process()
{
	if(key_count>=10)
	{
		key_count=0;
		key4();
		if(trg4&0x08)		//s4
		{
			if(mode1==2)
			{
				if(hour==0)
					hour=1;
				hour-=1;	
			}
			
			if(mode2==2)
			{
				naozhongshi-=1;
				
			}
			if(naozhongshi==24)
				naozhongshi=0;
			if(mode1==3)
			{
				min+=1;	
			}
			if(min==60)
			{
				min=0;
				hour+=1;
			}
			if(mode2==3)
			{
				naozhongfen+=1;
			}
			if(naozhongfen==60)
			{
				naozhongfen=0;
				naozhongshi+=1;
			}
			if(mode1==4)
			{
				sec+=1;
			}
			if(mode2==4)
			{
				naozhongmiao+=1;
			}
			if(sec==60)
			{
				sec=0;
				min+=1;
			}
			if(naozhongmiao==60)
			{
				naozhongmiao=0;
				naozhongfen+=1;
			}
		}
		if(trg4&0x04)		//s5
		{
			if(mode1==2)
			{
				hour+=1;	
			}
			if(hour==24)
				hour=0;
			if(mode2==2)
			{
				naozhongshi+=1;
				
			}
			if(naozhongshi==24)
				naozhongshi=0;
			if(mode1==3)
			{
				min+=1;	
			}
			if(min==60)
			{
				min=0;
				hour+=1;
			}
			if(mode2==3)
			{
				naozhongfen+=1;
			}
			if(naozhongfen==60)
			{
				naozhongfen=0;
				naozhongshi+=1;
			}
			if(mode1==4)
			{
				sec+=1;
			}
			if(mode2==4)
			{
				naozhongmiao+=1;
			}
			if(sec==60)
			{
				sec=0;
				min+=1;
			}
			if(naozhongmiao==60)
			{
				naozhongmiao=0;
				naozhongfen+=1;
			}
		}
		if(trg4&0x02)		//s6
		{
			flag=2;
			mode1=0;
			mode2++;
			if(mode2==5)
				mode2=1;
		}
		if(trg4&0x01)		//s7
		{
			flag=0;
			mode2=0;
			mode1++;
			if(mode1==5)
				mode1=1;
		}
		if(cont4&0x08)
		{
			if(mode1==0&&mode2==0)
				flag=1;
		}
		if(cont4==0&&trg4==0&&flag==1)
		{
			flag=0;
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
u8 ds1302_count;
void ds1302_process()
{
	if(ds1302_count>=200)
	{
		ds1302_count=0;
		sec=bcdtoshi(Read_Ds1302_Byte(0x81));
		min=bcdtoshi(Read_Ds1302_Byte(0x83));
		hour=bcdtoshi(Read_Ds1302_Byte(0x85));
	}
}
void smg_shizhong()
{
	smg[2]=0xbf;
	smg[5]=0xbf;
	if(haha<1000)
	{
		smg[0]=Seg_Table[hour/10];
		smg[1]=Seg_Table[hour%10];
		smg[3]=Seg_Table[min/10];
		smg[4]=Seg_Table[min%10];
		smg[6]=Seg_Table[sec/10];
		smg[7]=Seg_Table[sec%10];
	}
	
	if(haha>1000&&haha<=2000)
	{
		if(mode1==2)
		{
			smg[6]=Seg_Table[sec/10];
			smg[7]=Seg_Table[sec%10];
			smg[0]=0xff;
			smg[1]=0xff;
		}
		if(mode1==3)
		{
			smg[0]=Seg_Table[hour/10];
			smg[1]=Seg_Table[hour%10];
			smg[3]=0xff;
			smg[4]=0xff;
		}
		if(mode1==4)
		{
			smg[3]=Seg_Table[min/10];
			smg[4]=Seg_Table[min%10];
			smg[6]=0xff;
			smg[7]=0xff;
		}
	}
}
void smg_naozhong()
{
	smg[2]=0xbf;
	smg[5]=0xbf;
	if(haha<1000)
	{
		smg[0]=Seg_Table[naozhongshi/10];
		smg[1]=Seg_Table[naozhongshi%10];
		smg[3]=Seg_Table[naozhongfen/10];
		smg[4]=Seg_Table[naozhongfen%10];
		smg[6]=Seg_Table[naozhongmiao/10];
		smg[7]=Seg_Table[naozhongmiao%10];
	}
	if(haha>1000&&haha<=2000)
	{
		if(mode2==2)
		{
			smg[6]=Seg_Table[naozhongmiao/10];
			smg[7]=Seg_Table[naozhongmiao%10];
			smg[0]=0xff;
			smg[1]=0xff;
		}
		if(mode2==3)
		{
			smg[0]=Seg_Table[naozhongshi/10];
			smg[1]=Seg_Table[naozhongshi%10];
			smg[3]=0xff;
			smg[4]=0xff;
		}
		if(mode2==4)
		{
			smg[3]=Seg_Table[naozhongfen/10];
			smg[4]=Seg_Table[naozhongfen%10];
			smg[6]=0xff;
			smg[7]=0xff;
		}
	}
}
void smg_wendu()
{
	smg[0]=0xff;
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=Seg_Table[temp/100000];
	smg[6]=Seg_Table[temp/10000%10];
	smg[7]=Seg_Table[12];
}
void main()
{	
	system_int();
	while(temp_read()==85);
	temp=(u32)(temp_read()*10000);
	ds1302_set(50,59,23);

	Timer2_Init();
	
	while(1)
	{
		key4_process();
		temp_process();
		ds1302_process();
		if(flag==0)
		{
			smg_shizhong();
		}
		if(flag==1)
		{
			smg_wendu();
		}
		if(flag==2)
		{
			smg_naozhong();
		}
		
	}
}

void Timer2_Isr(void) interrupt 12
{
	key_count++;
	temp_count++;
	ds1302_count++;
	if(mode1!=0||mode2!=0)
		haha++;
	else
		haha=0;
	if(haha>=2000)
		haha=0;
	smg_display();
}