#include "system.h"
void delay12us(void)	//@12.000MHz
{
	unsigned char data i;

	_nop_();
	_nop_();
	i = 33;
	while (--i);
}

void CSB_send()
{
	u8 i;
	for(i=0;i<2;i++)		//发送2个40khz方波
	{
		P10=1;
		delay12us();
		P10=0;
		delay12us();
	}
}
u16 CSB_get()
{
	u16 distance;
	CSB_send();
	TH1=0;TL1=0;						//清零定时器1
	TR1=1;								//开启定时器1
	while((P11==1)&&(TF1==0));			//等待P11变成低电平
	TF1=0;
	TR1=0;								//关闭定时器1
	distance=(TH1<<8|TL1)*0.017;		//单位为cm   340/2/1000000*100
	return distance;
}