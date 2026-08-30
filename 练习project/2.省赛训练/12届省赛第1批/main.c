#include "system.h"

hextobin led,buzzer;

void delay_ms(u16 xms)	//@12.000MHz
{
	unsigned char data i, j;
	u16 k;
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

void Timer2_Init(void)		//100微秒@11.0592MHz
{
	AUXR |= 0x04;			//定时器时钟1T模式
	T2L = 0xAE;				//设置定时初始值
	T2H = 0xFB;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时
	IE2 |= 0x04;			//使能定时器2中断
	EA=1;
}

void SMG_set_int()			//设置数码管显示的内容,想在哪一位加小数点就在后面加上   &0x7f
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
void system_init()		//上电初始状态
{
	drive(0,0xa0);
	led.hex=0xff;
	drive(led.hex,0x80);
	SMG_set_int();
}


u8 temp_count;
u32 temp_float;
void temp_process()		//温度处理函数，100ms执行一次就行
{
	if(temp_count>=100)
	{
		temp_count=0;
		temp_float=(u32)(temp_read()*10000);		//数码管六位依次显示temp_float	/100000，	/10000%10（后面&0x7f加小数点），/1000%10，	/100%10，	/10%10，	%10
	}
}


u8 DA_count;
u8 DA_set;
void DA_process()		//DA输出，2ms执行一次就行
{
	if(DA_count>=2)
	{
		DA_count=0;
		DA_write(DA_set);				//耗时0.6ms左右
	}
}


void SMG_wendu()			//设置数码管显示的内容,想在哪一位加小数点就在后面加上   &0x7f
{
	smg[0]=Seg_Table[12];
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=Seg_Table[temp_float/100000];
	smg[5]=Seg_Table[temp_float/10000%10]&0x7f;
	smg[6]=Seg_Table[temp_float/1000%10];
	smg[7]=Seg_Table[temp_float/100%10];
}

u8 temp_set=25;
u8 temp_set_temp=25;
void SMG_wenduset()			//设置数码管显示的内容,想在哪一位加小数点就在后面加上   &0x7f
{
	smg[0]=0x8c;
	smg[1]=0xff;
	smg[2]=0xff;
	smg[3]=0xff;
	smg[4]=0xff;
	smg[5]=0xff;
	smg[6]=Seg_Table[temp_set_temp/10];
	smg[7]=Seg_Table[temp_set_temp%10];
}
u8 flag=1;
void SMG_DA()			//设置数码管显示的内容,想在哪一位加小数点就在后面加上   &0x7f
{
	temp_set=temp_set_temp;
	if(flag==1)
	{
		if((temp_float/10000)<temp_set)
		{
			smg[0]=0x88;
			smg[1]=0xff;
			smg[2]=0xff;
			smg[3]=0xff;
			smg[4]=0xff;
			smg[5]=Seg_Table[0]&0x7f;
			smg[6]=Seg_Table[0];
			smg[7]=Seg_Table[0];
			DA_set=0;
		}
		else
		{
			smg[0]=0x88;
			smg[1]=0xff;
			smg[2]=0xff;
			smg[3]=0xff;
			smg[4]=0xff;
			smg[5]=Seg_Table[5]&0x7f;
			smg[6]=Seg_Table[0];
			smg[7]=Seg_Table[0];
			DA_set=255;
		}
	}
	if(flag==2)
	{
		if((temp_float/10000)<20)
		{
			DA_set=51;
			smg[0]=0x88;
			smg[1]=0xff;
			smg[2]=0xff;
			smg[3]=0xff;
			smg[4]=0xff;
			smg[5]=Seg_Table[1]&0x7f;
			smg[6]=Seg_Table[0];
			smg[7]=Seg_Table[0];
		}
		else if((temp_float/10000)>40)
		{
			DA_set=204;
			smg[0]=0x88;
			smg[1]=0xff;
			smg[2]=0xff;
			smg[3]=0xff;
			smg[4]=0xff;
			smg[5]=Seg_Table[4]&0x7f;
			smg[6]=Seg_Table[0];
			smg[7]=Seg_Table[0];
		}
		else
		{
		DA_set=7.65*temp_float/10000-102;
		smg[0]=0x88;
		smg[1]=0xff;
		smg[2]=0xff;
		smg[3]=0xff;
		smg[4]=0xff;
		smg[5]=Seg_Table[DA_set/51]&0x7f;
		smg[6]=Seg_Table[DA_set*100/51/10%10];
		smg[7]=Seg_Table[DA_set*100/51%10];
		}
	}
}

u8 mode=1;
u8 key16_count;
void key16_process()				//矩阵键盘处理函数，10ms执行一次就行
{

	if(key16_count>=10)
	{	
		key16();
		key16_count=0;
		if(trg16==0x88)		//s4		以右上角为原点，向左高四位依次1248，向下低四位依次1248
		{
			mode++;
			if(mode==4)
				mode=1;
		}
		if(trg16==0x84)		//s5
		{
			flag++;
			if(flag==3)
				flag=1;
		}
	}
	if(mode==2&&trg16==0x48)		//s8
	{
		if(temp_set_temp>0)
			temp_set_temp--;
	}
	if(mode==2&&trg16==0x44)		//s9
	{
		if(temp_set_temp<99)
			temp_set_temp++;
	}
	if(mode==3&&(temp_set_temp!=temp_set))
		temp_set=temp_set_temp;
}
u8 ready;
void main()
{	
	system_init();
	Timer2_Init();
	temp_read();
	delay_ms(550);
	ready=1;
	while(1)
	{
		temp_process();
		key16_process();
		DA_process();
		if(mode==1)
		{
			SMG_wendu();
			led.b.b1=0;
		}
		else
			led.b.b1=1;
		
		
		if(mode==2)
		{
			SMG_wenduset();
			led.b.b2=0;
		}
		else
			led.b.b2=1;
		
		
		if(mode==3)
		{
			SMG_DA();
			led.b.b3=0;
		}
		else
			led.b.b3=1;
		
		
	}
}
u8 led_count;
void Timer2_Isr(void) interrupt 12
{
	key16_count++;
	temp_count++;
	DA_count++;
	led_count++;
	if(led_count>=200&&ready==1)
	{
		led_count=0;
		if(flag==1)		
			led.b.b0=0;
		else			
			led.b.b0=1;
		drive(led.hex,0x80);
	}
	
	SMG_display();
	
}