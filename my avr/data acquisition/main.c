#include<avr/io.h>
#include<util/delay.h>
#include<stdio.h>
#include<compat/deprecated.h>
#include "uart.h"
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

int main(void)
{unsigned char buf[15];
 unsigned int val,temp,light;
DDRA=0xF0;
DDRB=0xFF;
PORTB=0xFF;
PORTA=0x00;
USART_INIT();
rsstr("DATA acquisition\r\n");
 while(1)
 { light=adc(0);
   temp=adc(1); 
   sprintf(buf,"Light=%d\r\n",light);
   rsstr(buf);
   sprintf(buf,"TEMP=%d\r\n",temp);
   rsstr(buf);
   val=light;
   
   if(val<512)
   {
   sbi(PORTA,4);
    cbi(PORTA,5);
    cbi(PORTA,6);
    cbi(PORTA,7);
   }
   else if(val<600)
   {
   sbi(PORTA,5);
    cbi(PORTA,4);
    cbi(PORTA,6);
    cbi(PORTA,7);
   }
   else if(val<700)
   {
   sbi(PORTA,6); cbi(PORTA,5);
    cbi(PORTA,4);
    cbi(PORTA,7);
   }
   else if(val<1000)
   {
   sbi(PORTA,7);
    cbi(PORTA,5);
    cbi(PORTA,6);
    cbi(PORTA,4);
   }
   
 }
} 
 