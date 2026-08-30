#ifndef __UART_H__
#define __UART_H__

extern u8 ck_flag;
extern u8 ck_string[20];

void ck_relax();

void Uart1_Init(void);
void SendData(u8 dat);
void SendString(char *s);

#endif