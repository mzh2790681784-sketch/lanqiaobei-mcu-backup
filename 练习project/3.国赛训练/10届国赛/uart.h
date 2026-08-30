#ifndef __UART_H__
#define __UART_H__

extern bit ck_flag;
extern u8 ck_string[6];

void Uart1Init(void);
void SendData(u8 dat);
void SendString(char *s);
void ck_relax();

#endif