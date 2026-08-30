#ifndef __DS1302_H__
#define __DS1302_H__


#define shitobcd(shi)	(shi/10*16)+(shi%10)
#define bcdtoshi(bcd)	(bcd/16*10)+(bcd%16)

unsigned char Read_Ds1302_Byte ( unsigned char address );
void ds1302_set(u8 sec,u8 min,u8 hour);

#endif