#include<avr/io.h>
#include<util/delay.h>
#include<avr/sfr_defs.h>
#include "lcd.h"

int main(void)
{unsigned int val;
DDRA=0xF0;
DDRC=0xFF;
PORTC=0x00;
PORTA=0x00;
 lcd_init();
 lcd_cmd(0x01);
 lcd_cmd(0x80);
 lcd_puts("ADC Test");
 while(1)
 { lcd_cmd(0xC2);
   ADMUX=0x40;
   ADCSRA=(1<<ADEN);
   ADCSRA|=(1<<ADSC);
   while(bit_is_clear(ADCSRA,ADIF));
   val=ADCW;  
   itoa(val);
 if(val>=300)
 {
 PORTA=0xC0;
 PORTC=0x80;
 }
 
 else
 {
 PORTA=0x30;
 PORTC=0x10;
 }
 }
} 
 