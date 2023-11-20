#include<avr/io.h>
#include<util/delay.h>
#include<stdio.h>
#include<compat/deprecated.h>
#include "lcd.h"
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
{float correcta;
 unsigned int val;
 unsigned char buf[15];
DDRA=0xF0;
PORTA=0x00;
DDRD=0x00;
PORTD=0x01;
lcd_init();
USART_INIT();
lcd_cmd(0x0c);
lcd_cmd(0x01);
lcd_puts("MultiMeter");
 while(1)
 { lcd_cmd(0xc5); 
   val=adc(0);
   correcta=0.00488*val;
   sprintf(buf,"%1.2f",correcta);
   lcd_puts(buf);
   if(bit_is_clear(PIND,0))
   {
    rsstr(buf);
	rswritechar('\r');
	rswritechar('\n');
	
   }
   
 }
} 
 