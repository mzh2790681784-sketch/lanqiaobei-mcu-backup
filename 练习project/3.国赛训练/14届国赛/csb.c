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
char jiaozhun=0;
float jiezhi=340.0;
u16 csb_get()
{
	u16 distance;
	csb_send();
	TH1=0;TL1=0;
	TR1=1;
	while((P11==1)&&(TF1==0));
	TF1=0;
	TR1=0;
	distance=(TH1<<8|TL1)*jiezhi/20000.0+jiaozhun;
	
	return distance;
	
}