#include "system.h"
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

void delay12us(void)	//@12.000MHz
{
	unsigned char data i;

	_nop_();
	_nop_();
	i = 33;
	while (--i);
}
