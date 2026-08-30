#include <STC15F2K60S2.H>

unsigned char mode1=1,mode2=1;

void drive(unsigned char p0,unsigned char p2)
{
	P0=p0;
	P2=P2&0x1f|p2;
	P2=P2&0x1f;
}

void Timer2_Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x04;			//定时器时钟1T模式
	T2L = 0x20;				//设置定时初始值
	T2H = 0xD1;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时
	IE2 |= 0x04;			//使能定时器2中断
	EA=1;
}

//独立按键
unsigned char trg4,cont4;
void key4()
{
	unsigned char date=P3^0xff;
	trg4=date&(date^cont4);
	cont4=date;
}



//矩阵键盘
unsigned char key16_io()
{
	unsigned char key_io=0xff;
	P3=0xf0;P4=0xff;			//列扫描，低4位置0，检测高四位
	if(P44==0)	key_io=0x70;
	if(P42==0)	key_io=0xb0;
	if(P35==0)	key_io=0xd0;
	if(P34==0)	key_io=0xe0;
	
	P3=0x0f;P4=0x00;			//行扫描，高4位置0，检测低四位
	if(P33==0)	key_io=key_io|0x07;
	if(P32==0)	key_io=key_io|0x0b;
	if(P31==0)	key_io=key_io|0x0d;
	if(P30==0)	key_io=key_io|0x0e;
	
	return key_io;
}


unsigned char trg16,cont16;
void key16()
{
	unsigned char date=key16_io()^0xff;
	trg16=date&(date^cont16);
	cont16=date;
}

unsigned char key4_count;
void key4_process()		//独立按键处理函数，10ms执行一次就行
{
	if(key4_count>=10)
	{
		key4_count=0;
		key4();
		if(trg4&0x08)	//s4
		{
			mode1++;
			if(mode1==4)
			{
				mode1=1;
			}
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
unsigned char key16_count;
void key16_process()				//矩阵键盘处理函数，10ms执行一次就行
{
	if(key16_count>=10)
	{
		key16_count=0;
		key16();
		if(trg16==0x88)		//s4		以右上角为原点，向左高四位依次1248，向下低四位依次1248
		{
			mode2++;
			if(mode2==4)
			{
				mode2=1;
			}
		}
	}
}
code unsigned char Seg_Table[] = 
{
0xc0, //0
0xf9, //1
0xa4, //2
0xb0, //3
0x99, //4
0x92, //5
0x82, //6
0xf8, //7
0x80, //8
0x90, //9
0x88, //A
0x83, //b
0xc6, //C
0xa1, //d
0x86, //E
0x8e, //F
};


unsigned char smg[8];

void SMG_display()
{
	static unsigned char i=0;
	drive(0,0xc0);
	drive(smg[i],0xe0);
	drive(0x01<<i,0xc0);
	
	i++;
	if(i==8)
		i=0;
}
void smg_mode1()
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
void smg_mode2()
{
	smg[0]=Seg_Table[6];
	smg[1]=Seg_Table[5];
	smg[2]=Seg_Table[4];
	smg[3]=Seg_Table[3];
	smg[4]=Seg_Table[2];
	smg[5]=Seg_Table[1];
	smg[6]=Seg_Table[0];
	smg[7]=0xff;
}
void main()
{
	Timer2_Init();
	while(1)
	{
		if(mode1==1)
		{
			smg_mode1();
		}
		if(mode1==2)
		{
		
		}
		if(mode1==3)
		{
		
		}
	}
}
void Timer2_Isr(void) interrupt 12
{
	key4_count++;
	key16_count++;
	
	SMG_display();
}