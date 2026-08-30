#ifndef __I2C_H__
#define __I2C_H__

void AT24C02_write(u8 add, u8 dat);
u8 AT24C02_read(u8 add);

void DA_write(u8 dat);
u8 AD_read(u8 add);
#endif