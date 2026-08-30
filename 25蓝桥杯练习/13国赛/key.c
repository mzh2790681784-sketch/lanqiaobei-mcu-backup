#include "system.h"

u8 trg4,cont4;
void key4()
{
	u8 dat=P3^0XFF;
	trg4=dat&(dat^cont4);
	cont4=dat;
}