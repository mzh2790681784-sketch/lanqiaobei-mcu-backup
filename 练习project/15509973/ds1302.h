#ifndef __DS1302_H__
#define __DS1302_H__

#define dectobcd(dec) (dec/10*16)+(dec%10)
#define bcdtodec(bcd) (bcd/16*10)+(bcd%16)

unsigned char Read_Ds1302_Byte( unsigned char address );
void ds1302_set(u8 hour,u8 min,u8 sec);

#endif