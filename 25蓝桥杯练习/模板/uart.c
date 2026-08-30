#include "SYSTEM.H"

bit busy;
u8 ck_time,ck_count,ck_flag,ck_string[10];

void ck_relax()
{
	if(ck_time>=50)
	{
		ck_time=ck_count=0;
		memset(ck_string,'\0',sizeof(ck_string));
	}
}
/*----------------------------
UART 中断服务程序
-----------------------------*/
void Uart() interrupt 4
{
    if (RI)
    {
        RI = 0;                 //清除RI位
		ck_time=0;
        ck_string[ck_count++] = SBUF;              //P0显示串口数据
		if(ck_string[ck_count-1]=='\n')
		{
			ck_count=0;
			ck_flag=1;
		}
    }
    if (TI)
    {
        TI = 0;                 //清除TI位
        busy = 0;               //清忙标志
    }
}

/*----------------------------
发送串口数据
----------------------------*/
void SendData(u8 dat)
{
    while (busy);               //等待前面的数据发送完成
    busy = 1;
    SBUF = dat;                 //写数据到UART数据寄存器
}

/*----------------------------
发送字符串
----------------------------*/
void SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        SendData(*s++);         //发送当前字符
    }
}