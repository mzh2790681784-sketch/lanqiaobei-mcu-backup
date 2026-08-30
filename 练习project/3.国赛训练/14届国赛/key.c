#include "system.h"

u8 trg16,cont16;

u8 key16_io()
{
	u8 io=0xff;
	P3=0XF0;P4=0XFF;
	if(P44==0)			io=0X70;
	if(P42==0)			io=0Xb0;
	if(P35==0)			io=0Xd0;
	
	P3=0X1F;P4=0X00;
	if(P33==0)			io|=0x07;
	if(P32==0)			io|=0x0b;
	if(P31==0)			io|=0x0d;
	if(P30==0)			io|=0x0e;
	
	return io;
}

void key16()
{
	u8 date=~key16_io();
	trg16=date&(date^cont16);
	cont16=date;
}