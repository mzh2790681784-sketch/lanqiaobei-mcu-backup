#ifndef __IIC_H__
#define __IIC_H__

void at24c02_wirte(u8 add,u8 dat);
u8 at24c02_read(u8 add);
void da_write(u8 dat);
u8 ad_read(u8 add);
#endif