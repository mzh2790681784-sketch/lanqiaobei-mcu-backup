#ifndef __DS1302_H
#define __DS1302_H

#define REG_SECOND 0x80
#define REG_MINUTES 0x82
#define REG_HOUR    0x84
#define REG_DATE    0x86
#define REG_MONTH   0x88
#define REG_WEEK    0x8A
#define REG_YEAR    0x8C
#define REG_PROTECT 0x8E


void Write_Ds1302_Byte(unsigned  char temp);
void Write_Ds1302( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302 ( unsigned char address );


void DS1302_Init(void);
void Get_Time(void);

#endif
