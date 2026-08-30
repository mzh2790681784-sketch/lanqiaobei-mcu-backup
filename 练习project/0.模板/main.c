#include "system.h"

hextobin led,buzzer;

void SMG_int()			//设置数码管显示的内容,想在哪一位加小数点就在后面加上   &0x7f
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
	SMG_int();
	SMG_display();
}



void SMG_set()			//设置数码管显示的内容,想在哪一位加小数点就在后面加上   &0x7f
{
	smg[0]=Seg_Table[0];
	smg[1]=Seg_Table[1];
	smg[2]=Seg_Table[2];
	smg[3]=Seg_Table[3];
	smg[4]=Seg_Table[4];
	smg[5]=Seg_Table[5];
	smg[6]=Seg_Table[6];
	smg[7]=0xff;
}



u8 key4_count;
void key4_process()		//独立按键处理函数，10ms执行一次就行
{
	if(key4_count>=10)
	{
		key4_count=0;
		key4();
		if(trg4&0x08)	//s4
		{
		
		}
		if(trg4&0x04)	//s5
		{
		
		}
		if(trg4&0x02)	//s6
		{
		
		}
		if(trg4&0x01)	//s7
		{
		
		}
	}
}


u8 key16_count;
void key16_process()				//矩阵键盘处理函数，10ms执行一次就行
{
	if(key16_count>=10)
	{
		key16_count=0;
		key16();
		if(trg16==0x88)		//s4		以右上角为原点，向左高四位依次1248，向下低四位依次1248
		{
		
		}
	}
}



u8 temp_count;
u32 temp;
void temp_process()		//温度处理函数，100ms执行一次就行
{
	if(temp_count>=100)
	{
		temp_count=0;
		temp=(u32)(temp_read()*10000);		//数码管六位依次显示temp	/100000，	/10000%10（后面&0x7f加小数点），/1000%10，	/100%10，	/10%10，	%10
	}
}



union AT24C02_u16
{
	u16 a;
	u8 b[2];
}u16_write,u16_read;

union AT24C02_s16
{
	s16 a;
	u8 b[2];
}s16_write,s16_read;

union AT24C02_float
{
	float a;
	u8 b[4];
}float_write,float_read;

union AT24C02_u32
{
	u32 a;
	u8 b[4];
}u32_write,u32_read;

u8 string_write[11]={"HELLO WORLD"};
u8 string_read[11];
void AT24C02_process()						//AT24C02写入、读出，读写的范围是0x00~0xff
{
	u8 i=0;
	
	u16_write.a=1234;						//两个字节数据的读写
	for(i=0;i<sizeof(u16);i++)
		AT24C02_write(0x30+i,u16_write.b[i]);
	for(i=0;i<sizeof(u16);i++)
		u16_read.b[i]=AT24C02_read(0x30+i);
	
	s16_write.a=-1234;						//负数的读写
	for(i=0;i<sizeof(s16);i++)
		AT24C02_write(0x20+i,s16_write.b[i]);
	for(i=0;i<sizeof(s16);i++)
		s16_read.b[i]=AT24C02_read(0x20+i);
	
	float_write.a=3.1415926;				//浮点数的读写
	for(i=0;i<sizeof(float);i++)
		AT24C02_write(0x10+i,float_write.b[i]);
	for(i=0;i<sizeof(float);i++)
		float_read.b[i]=AT24C02_read(0x10+i);
	
	u32_write.a=70000;						//四个字节数据的读写
	for(i=0;i<sizeof(u32);i++)
		AT24C02_write(0x00+i,u32_write.b[i]);
	for(i=0;i<sizeof(u32);i++)
		u32_read.b[i]=AT24C02_read(0x00+i);
		
	for(i=0;i<sizeof(string_write);i++)		//字符串的读写，字符以ASCII码存储
		AT24C02_write(0x40+i,string_write[i]);
	for(i=0;i<sizeof(string_write);i++)
		string_read[i]=AT24C02_read(0x40+i);
}



u8 AD_count;
u8 ch0,ch1,ch3;			//通道零0x40（排针），通道一0x41（光敏电阻），通道三0x43（旋转电位器）
u16 ch3_05;
u8 ch3_0_99,ch3_1_5;
void AD_process()		//AD采样，10ms执行一次就行
{
	if(AD_count>=10)
	{
		AD_count=0;
		AD_read(0x41);
		ch1=AD_read(0x41);				//耗时0.75ms左右
		AD_read(0x43);
		ch3=AD_read(0x43);
		
		ch3_05=ch3*100/51;				//		255/5=51
		ch3_0_99=ch3/2.57;  			//  	255/99=2.57;
		ch3_1_5=ch3/51.1+1;				//		256/5=51.1
	}
}


u8 DA_count;
void DA_process()		//DA输出，2ms执行一次就行
{
	static u8 haha;
	if(DA_count>=2)
	{
		DA_count=0;
		DA_write(haha++);				//耗时0.6ms左右
	}
}



u8 hour,minute,second,year,month,day,date;
void DS1302_process()					//DS1302时间处理函数，将BCD码转为十进制
{
	second = BCDToDec(Read_Ds1302_Byte(0x81));		//数码管依次读		/10，	%10就行
	minute = BCDToDec(Read_Ds1302_Byte(0x83));
	hour = BCDToDec(Read_Ds1302_Byte(0x85));
	date = BCDToDec(Read_Ds1302_Byte(0x87));
	month = BCDToDec(Read_Ds1302_Byte(0x89));
	day = BCDToDec(Read_Ds1302_Byte(0x8b));
	year = BCDToDec(Read_Ds1302_Byte(0x8d));
	
}



u16 NE555_count;
u16 NE555_date;					//频率在90~32000左右
void NE555_process()			//频率测量,配合counter0使用
{
	if(NE555_count>=1000)		//精确的1s，1s更新一次数据，若想改变更新时间，如100ms一次，只需将1000改成100，NE555_date*10
	{
		NE555_count=0;
		NE555_date=(TH0<<8)|TL0;		//1s时误差为1hz,100ms误差就*10，500ms误差就*2
		TL0=0;
		TH0=0;
	}
}



u8 CSB_count;
u16 CSB_date; 
void CSB_process()				//超声波测距
{
	if(CSB_count>=100)			//100ms执行一次就行
	{
		CSB_count=0;
		CSB_date=CSB_get();		//单位cm
	}
}



void CK_process()
{
	if(CK_flag)
	{
		drive(CK_string[0],0x80);				//用串口接受的数据执行某些操作
		drive(CK_string[1],0x80);
		drive(CK_string[2],0x80);
		CK_flag=0;
	}
}



u8 pwm_count=1;
u8 compare = 90;
void PWM_prcocess()
{
	pwm_count=pwm_count%10+1;	//cnt_pwm的范围是1~10，共10个数，即定义了PWM的周期为10ms
	if(pwm_count<=(compare/10))
		drive(0xff,0x80);	
	else
		drive(0x00,0x80);
}

u8 led_count;
void led_process()
{
	if(led_count>=10)
	{
		led_count=0;
		
		
	}
}

void main()
{ 
	system_init();
	Timer2_Init();
	Counter0_Init();
	Uart1_Init();				//不能和超声波同时使用
	temp_read();				//防止出现读出85℃,也可以写为while(temp_read()==85);temp=(u32)(temp_get()*10000);
	delay_ms(750);				//实测550ms即可
	
	DS1302_set(11,25,25,24,2,12,1);
	AT24C02_process();
	while(1)
	{
		SMG_set();
		key4_process();
		key16_process();
		temp_process();
		AD_process();
		DA_process();
		DS1302_process();
		CSB_process();
		CK_process();
	}
}

void Timer2_Isr(void) interrupt 12
{
	static u16 count;			//在中断里定义的变量要记得加static（但好像不加也没出错，不造为啥）
	count++;
	
	key4_count++;
	key16_count++;
	temp_count++;
	AD_count++;
	DA_count++;
	NE555_count++;
	CSB_count++;
	led_count++;
	
	SMG_display();
	led_process();
	NE555_process();
	PWM_prcocess();
	ck_relax();
}