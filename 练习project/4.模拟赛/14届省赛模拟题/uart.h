#ifndef __uart_H__
#define __uart_H__

extern u8 ck_string[1];
extern u8 ck_date[1];
extern bit ck_flag;

void Uart1Init(void);
void ck_relax();
void SendData(u8 dat);
void SendString(char *s);

#endif