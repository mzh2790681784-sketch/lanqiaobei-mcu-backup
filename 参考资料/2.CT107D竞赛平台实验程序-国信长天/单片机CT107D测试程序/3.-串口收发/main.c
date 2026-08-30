#include "reg52.h"

#define SYSCLK  	12000000	//System clock

sfr P4 		= 0xC0;
sfr AUXR = 0x8E;
sfr T2H	 = 0XD6;	
sfr T2L  = 0xD7;            

volatile unsigned char RxC = 0;

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
        return 0;   //×Ö·û´®¿Õ
    }
    return 1;
}


//Ö÷º¯Êý
void main(void)
{
	UARTInit(14400);
	
	UARTSendString("UART test program.\r\n");
	UARTSendString("1->LED1 .\r\n");
	UARTSendString("2->LED2 .\r\n");
	UARTSendString("3->LED3 .\r\n");
	UARTSendString("4->LED4 .\r\n");
	
	while(1){
		if(RxC){
			switch(RxC){
				case '1':
					P4 = 0xFD;
					break;
				case '2':
					P4 = 0xFB;
					break;
				case '3':
					P4 = 0xF7;
					break;
				case '4':
					P4 = 0xEF;
					break;
				default:
					break;
			}
			RxC = 0;
		}
	}
}

//
void ISR_UART(void) interrupt 4
{
	if(RI){
		RI = 0;
		RxC = SBUF;
	}
}

