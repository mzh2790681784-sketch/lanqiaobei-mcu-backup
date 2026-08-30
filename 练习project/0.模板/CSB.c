#include "system.h"

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
	TH1=0;TL1=0;						//清零定时器1
	CSB_send();	
	TR1=1;								//开启定时器1
	while((P11==1)&&(TF1==0));			//等待P11变成低电平
	TR1=0;								//关闭定时器1
	TF1=0;
	distance=(TH1<<8|TL1)*0.017;		//单位为cm   340/2/1000000*100
	return distance;
}

//u16 CSB_get()
//{
//	u16 distance;
//	CMOD=0x00;
//	CH=CL=0;
//	CSB_send();
//	CR=1;
//	while((P11==1)&&(CF==0));			//等待P11变成低电平
//	CR=0;
//	CF=0;
//	distance=(CH<<8|CL)*0.017;		//单位为cm   340/2/1000000*100
//	return distance;
//}