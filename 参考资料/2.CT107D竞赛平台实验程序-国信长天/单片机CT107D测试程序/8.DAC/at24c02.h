#ifndef _at24c02_h_
#define _at24c02_h_

void x24c02_init();
unsigned char x24c02_read(unsigned char address);
void x24c02_write(unsigned char address,unsigned char info);

void dac(unsigned char value);
unsigned char adc( unsigned char channel);

void init_pcf8591(void);


float adc_pcf8591(void);

void dac_pcf8591(unsigned char dat);

#endif