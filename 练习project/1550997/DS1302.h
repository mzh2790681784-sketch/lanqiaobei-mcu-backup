#ifndef __DS1302_H__
#define __DS1302_H__

#define DecToBCD(dec) (dec/10*16)+(dec%10)
#define BCDToDec(bcd) (bcd/16*10)+(bcd%16)

unsigned char Read_Ds1302_Byte( unsigned char address );
void DS1302_set(u8 hour,u8 minute,u8 second,u8 year,u8 month,u8 day,u8 date);

#endif