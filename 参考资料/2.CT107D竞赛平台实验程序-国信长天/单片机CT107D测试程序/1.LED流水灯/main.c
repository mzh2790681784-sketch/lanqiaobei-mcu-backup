#include "reg52.h"

sfr P4 =  0xC0;

sbit LED1 = P4^1;
sbit LED2 = P4^2;
sbit LED3 = P4^3;
sbit LED4 = P4^4;

/*
11111101
11111011
11110111
11101111
*/
const unsigned char table[] = {0xFD, 0xFB, 0xF7, 0xEF};

//延时函数
void Delay(void){
	unsigned char i,j,k;
	
	for(i=0;i<40;i++){
		for(j=40;j>0;j--){
			for(k=248;k>0;k--);
		}
	}
}

//主函数
void main(void)
{
	unsigned char i;
	
	P4 = 0xFF;	//状态初始化
	
	do{
		P4 = table[i];	
		Delay();
		if(++i == 4){
			i = 0;
		}
	}while(1);
}