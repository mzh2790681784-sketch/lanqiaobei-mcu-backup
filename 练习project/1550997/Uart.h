#ifndef __UART_H__
#define __UART_H__

extern u8 CK_string[3];
extern bit CK_flag;

void Uart1_Init(void);
void SendData(u8 dat);
void SendString(char *s);
void ck_relax();
#endif