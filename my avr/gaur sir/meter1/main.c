#include<avr/io.h>
#include<util/delay.h>
#include<stdio.h>
#include<compat/deprecated.h>
#include "uart.h"
  float correcta,correctb;
 unsigned int val;
 unsigned char buf[15];
unsigned int adc(unsigned char channel)
{  unsigned int val;
   ADMUX=0x40+channel;
   ADCSRA=(1<<ADEN);
   ADCSRA|=(1<<ADSC);
    ADCSRA|=0x07;
   while(bit_is_clear(ADCSRA,ADIF));
   val=ADCW; 
   _delay_ms(100);
   return val;
}
void menu()
{
 rsstr("  **Menu**\r\n");
 rsstr("(1) Voltage Measurement\r\n");
 rsstr("(2) Current Measurement\r\n");
 rsstr("(3) Voltage & Current Measurement\r\n");
 rsstr("(4) No. of samples\r\n");
}
void voltage_show()
{
   val=adc(0);
   correcta=0.00488*val;
   sprintf(buf,"%1.2fV",correcta);
   rsstr(buf);
   rswritechar('\r');
   rswritechar('\n');
}
void current_show()
{
   val=adc(0);
   correcta=0.00488*val;
   val=adc(1);
   correctb=0.00488*val;
   correctb=((correcta-correctb)/5.00)*1000;
   sprintf(buf,"%.2fma",correctb);
   rsstr(buf);
   rswritechar('\r');
   rswritechar('\n');
}

int main(void)
{unsigned char t,sample;
DDRA=0xF0;
PORTA=0x00;
DDRD=0x00;
PORTD=0x01;
USART_INIT();
sample=0;
 while(1)
 { 
   t=rsreadchar();
   switch(t)
   {
    case '?':menu();break;
	case '1':
	while(sample!=0)
	{
	voltage_show();
	sample--;
	}
	break;
	case '2':
	while(sample!=0)
	{
	current_show();
	sample--;
	}
	break;
	case '3':
	while(sample!=0)
	{
	voltage_show();
	current_show();
	sample--;
	}
	break;
    case '4':rsstr("Input No. of Samples-");sample=rsreadchar();rsstr("ok\r\n");sample=sample-0x30;break;


   }  
 }
} 
 