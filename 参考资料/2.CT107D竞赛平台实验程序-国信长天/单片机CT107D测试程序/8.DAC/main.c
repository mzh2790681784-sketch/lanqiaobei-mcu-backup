#include "reg52.h"
#include "at24c02.h"
#include "stdio.h"

#define SYSCLK  	12000000	//System clock
#define VREF		5.0	
#define OUTPUT		0xAA

sfr T2H   		= 0xd6;          
sfr T2L   		= 0xd7;             

sfr AUXR       	= 0x8e;                            


//延时函数
void Delay(void){
	unsigned char i,j,k;
	
	for(i=0;i<40;i++){
		for(j=40;j>0;j--){
			for(k=248;k>0;k--);
		}
	}
}


//
void UARTInit(unsigned int baud)
{
	SCON = 0x50;
	
	T2L = (65536-(SYSCLK/4/baud));
	T2H = (65536-(SYSCLK/4/baud))>>8;

	AUXR = 0x14;
	AUXR |= 0x01;
	
	TI = 0;
	RI = 0;

//	ES = 1;
	EA = 1;
}

//
bit UARTSendString(char *str)
{
    unsigned char *temp;

    temp = str;
    if (*str != '\0') {
        do {
            TI = 0; 
            SBUF = *temp++;
            while(TI == 0);
            TI = 0;
        }while (*temp != '\0');
    }
    else{
        return 0;   //字符串空
    }
    return 1;
}

//主函数
void main(void)
{
	unsigned int x = 0;
	unsigned char tempstr[40];
	
	UARTInit(14400);
	
	Delay();
	Delay();
	
	init_pcf8591();
	
	UARTSendString("DAC-PCF8591 test program.\r\n");
	Delay();
	dac_pcf8591(OUTPUT);	//输出电压接近3.33V 结果与输入电压相关

	x = (unsigned int)(OUTPUT*VREF/0xFF*100);	//OUTPUT<VREF
	
	tempstr[0] = x/100+'0';
	tempstr[1] = '.';
	tempstr[2] = x%100/10+'0';
	tempstr[3] = x%10+'0';
	tempstr[4] = '\r';
	tempstr[5] = '\n';
	tempstr[6] = '\0';

	UARTSendString("DAC output:");
	UARTSendString(tempstr);
	
	//使用万用表测量J11-PORT1标识为'A'的引脚的电压值，与串口输出显示的电压值比较。
	while(1){
		;	
	}
}

