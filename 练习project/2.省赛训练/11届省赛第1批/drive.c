#include "system.h"

void drive(u8 p0,u8 p2)
{
	P0=p0;
	P2=P2&0X1F|p2;
	P2=P2&0X1F;
}