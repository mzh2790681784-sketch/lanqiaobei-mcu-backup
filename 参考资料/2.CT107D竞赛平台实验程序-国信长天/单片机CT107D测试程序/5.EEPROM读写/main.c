#include "reg52.h"
#include "at24c02.h"
#include "stdio.h"

#define SYSCLK  	12000000	//System clock

sfr P4 		= 0xC0;
sfr AUXR = 0x8E;
sfr T2H	 = 0XD6;	
sfr T2L  = 0xD7;            

volatile unsigned char RxC = 0;

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

	ES = 1;
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
	unsigned char occount = 0;
	unsigned char tempstr[40];
	
	UARTInit(14400);
	
	UARTSendString("EEPROM test program.\r\n");
	x24c02_init();
	Delay();
	
	occount = x24c02_read(0x00);
	UARTSendString("EEPROM Counter:");	//每次开关机EEPROM COUNTER后面的数值+1

	tempstr[0] = occount/10+'0';
	tempstr[1] = occount%10+'0';
	tempstr[2] = '\r';
	tempstr[3] = '\n';
	tempstr[4] = '\0';	//结束标志
	UARTSendString(tempstr);
	
	occount++;
	x24c02_write(0x00,occount);
	Delay();
	
	while(1){

	}
}

