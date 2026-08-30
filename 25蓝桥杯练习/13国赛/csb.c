#include "system.h"

void Delay12us(void)	//@12.000MHz
{
	unsigned char data i;

	_nop_();
	_nop_();
	i = 33;
	while (--i);
}

void csb_send()
{
	u8 i=0;
	for(i=0;i<2;i++)
	{
		P10=1;
		Delay12us();
		P10=0;
		Delay12us();
	}
}

u16 csb_get()
{
	u16 dis;
	CMOD=0;
	CH=CL=0;
	csb_send();
	CR=1;
	while(P11==1&&CF==0);
	CR=0;
	CF=0;
	dis=((CH<<8)|CL)*0.017;
	return dis;
}