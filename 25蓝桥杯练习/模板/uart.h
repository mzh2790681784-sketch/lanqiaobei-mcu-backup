#ifndef __UART_H__
#define __UART_H__

extern u8 ck_time,ck_flag,ck_string[10];
void ck_relax();
void SendString(char *s);
#endif