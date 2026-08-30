#ifndef __I2C_H__
#define __I2C_H__

void AT24C02_write(u8 add,u8 date);
u8 AT24CO2_read(u8 add);

u8 AD_read(u8 add);

#endif