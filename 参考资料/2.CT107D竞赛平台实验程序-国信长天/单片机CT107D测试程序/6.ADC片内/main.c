#include "reg52.h"
#include "intrins.h"

#define SYSCLK  	12000000	//System clock

sfr T2H   		= 0xd6;          
sfr T2L   		= 0xd7;             

sfr AUXR       	=   0x8e;                      

sfr ADC_CONTR   =   0xBC;         
sfr ADC_RES     =   0xBD;         
sfr ADC_LOW2    =   0xBE;       
sfr P1ASF       =   0x9D;   

#define ADC_POWER   0x80         
#define ADC_FLAG    0x10           
#define ADC_START   0x08           
#define ADC_SPEEDLL 0x00           
#define ADC_SPEEDL  0x20           
#define ADC_SPEEDH  0x40          
#define ADC_SPEEDHH 0x60            

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
void InitADC()
{
    P1ASF = 0xff;                   //P1 ->adc
    ADC_RES = 0;                    
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
    Delay();                      
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

//
unsigned char GetADCResult(unsigned char ch)
{
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //Delay
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//等待转换完成
    ADC_CONTR &= ~ADC_FLAG;        

    return ADC_RES;                 
}


//主函数
void main(void)
{
	unsigned char x = 0;
	unsigned char tempstr[40];
	
	UARTInit(14400);
	InitADC();
	
	
	
	UARTSendString("ADC test program.\r\n");
	
	while(1){
		x = GetADCResult(5);
		tempstr[0] = x/100+'0';
		tempstr[1] = x%100/10+'0';
		tempstr[2] = x%10+'0';
		tempstr[3] = '\r';
		tempstr[4] = '\n';
		tempstr[5] = '\0';
		Delay();
		UARTSendString("ADC result:");
		UARTSendString(tempstr);

	}
}

