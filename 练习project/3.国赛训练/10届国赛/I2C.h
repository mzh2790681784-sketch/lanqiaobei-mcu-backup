#ifndef __I2C_H__
#define __I2C_H__

u8 at24c02_read(u8 add);
void at24c02_write(u8 add,u8 date);
void da_write(u8 date);

#endif