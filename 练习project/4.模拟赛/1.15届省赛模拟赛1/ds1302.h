#ifndef __ds1302_H__
#define __ds1302_H__

#define hextoshi(hex)	(hex/16*10)+(hex%16)
#define shitohex(shi)	(shi/10*16)+(shi%10)

unsigned char Read_Ds1302_Byte(unsigned char address);
void ds1302_set(u8 hour,u8 min,u8 sec);

#endif