#include "system.h"

/*
	独立按键都是&，矩阵键盘都是==，长按判断分别为cont4&和cont16==，
	从右到左同一列同时按下，cont16值分别为0x10,0x20,0x40,0x80
*/



//独立按键
u8 trg4,cont4;
void key4()
{
	u8 date=P3^0xff;
	trg4=date&(date^cont4);
	cont4=date;
}



//矩阵键盘
u8 key16_io()
{
	u8 key_io=0xff;
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


u8 trg16,cont16;
void key16()
{
	u8 date=key16_io()^0xff;
	trg16=date&(date^cont16);
	cont16=date;
}