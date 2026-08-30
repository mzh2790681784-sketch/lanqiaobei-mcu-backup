#ifndef __iic_h__
#define __iic_h__

void at24c02_write(u8 add,u8 dat);
u8 at24c02_read(u8 add);
void da_write(u8 dat);
u8 ad_read(u8 add);
#endif