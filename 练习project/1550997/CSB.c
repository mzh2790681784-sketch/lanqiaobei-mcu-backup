#include "system.h"

void CSB_send()
{
	u8 i;
	for(i=0;i<2;i++)
	{
		P10=1;
		delay12us();
		P10=0;
		delay12us();
	}
}
u16 CSB_get()
{
	u16 distance;
	CSB_send();
	TH1=0;TL1=0;
	TR1=1;
	while((P11==1)&&(TF1==0));
	TF1=0;
	TR1=0;
	distance=(TH1<<8|TL1)*0.017;
	return distance;
}