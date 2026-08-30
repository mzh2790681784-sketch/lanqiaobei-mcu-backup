#ifndef __DS1302_H__
#define __DS1302_H__

void ds1302_set(u8 sec,u8 min,u8 hou,u8 date,u8 mon,u8 day,u8 year);
unsigned char Read_Ds1302_Byte ( unsigned char address );

#endif