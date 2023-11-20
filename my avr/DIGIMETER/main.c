#include<avr/io.h>
#include<util/delay.h>
#include<stdio.h>
#include<compat/deprecated.h>
#include "lcd.h"
unsigned int adc(unsigned char channel)
{  unsigned char c;
   unsigned int val;
   ADMUX=0x40+channel;
   ADCSRA=(1<<ADEN);
   val=0;
   c=8;
  while(c)
   { 
   ADCSRA|=(1<<ADSC);
    ADCSRA|=0x07;
   while(bit_is_clear(ADCSRA,ADIF));
   ADCSRA|=(1<<ADIF);

   ADCSRA|=(1<<ADSC);
    ADCSRA|=0x07;
   while(bit_is_clear(ADCSRA,ADIF));
   ADCSRA|=(1<<ADIF);
   
   val=val+ADCW; 
   _delay_ms(10);
   c--;
   }
   val=val/8;
   val=val&0x03FC;
   return val;
}

int main(void)
{double voltage,current;
 unsigned int val;
 char buf[15];
DDRA=0xF0;
PORTA=0x00;
DDRD=0x00;
PORTD=0x0F;
lcd_init();
//USART_INIT();
lcd_cmd(0x0c);
lcd_cmd(0x01);
lcd_puts("   *DIGIMeter*");
_delay_ms(1000);
lcd_cmd(0x01);
lcd_cmd(0x89);
lcd_puts("mA");
lcd_cmd(0xc9);
lcd_puts("volts");

 while(1)
 { lcd_cmd(0x82); 
   val=adc(0);
   current=(0.00488*val);
   current=current/0.9;
   current=current*23;
   sprintf(buf,"%4.2f",current);
   lcd_puts(buf);
   lcd_cmd(0xc2); 
   val=adc(1);
   voltage=(0.00488*val);
   sprintf(buf,"%1.2f",voltage);
   lcd_puts(buf);
   if((PIND&0x01)==0)
   {
    lcd_cmd(0x01);
	lcd_puts("Resistance");
	lcd_cmd(0xc2);
    sprintf(buf,"%1.3fKohm",(voltage/current));
	lcd_puts(buf);
	_delay_ms(3000);
	lcd_cmd(0x01);
	lcd_cmd(0x89);
lcd_puts("mA");
lcd_cmd(0xc9);
lcd_puts("volts");
   }
 }
} 
 