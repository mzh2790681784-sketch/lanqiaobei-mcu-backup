#include "SYSTEM.H"

u8 trg4,cont4;

void key4()
{
	u8 date=~P3;
	trg4=date&(date^cont4);
	cont4=date;
}