#include "reg52.h"
#include "at24c02.h"
#include "stdio.h"

#define SYSCLK  	12000000	//System clock

sfr T2H   		= 0xd6;          
sfr T2L   		= 0xd7;             

sfr AUXR       	= 0x8e;                            


//ÑÓÊ±º¯Êý
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
        return 0;   //×Ö·û´®¿Õ
    }
    return 1;
}

//Ö÷º¯Êý
void main(void)
{
	float x = 0.0;
	unsigned char tempstr[40];
	
	UARTInit(14400);
	
	Delay();
	init_pcf8591();
	Delay();
	
	
	UARTSendString("ADC-PCF8591 test program.\r\n");
	
	while(1){
		x = adc_pcf8591();
		sprintf(tempstr,"%s:%0.2f%c%c","ADC value",x,'\r','\n');
		UARTSendString(tempstr);

		Delay();
		Delay();
	}
}

