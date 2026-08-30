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
	u8 i;
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
	u16 distance;
	csb_send();
	TH0=0;TL0=0;
	TR0=1;
	while((P11==1)&&(TF0==0));
	TF0=0;
	TR0=0;
	distance=(TH0<<8|TL0)*0.017;
	
	return distance;
}