#include<avr/io.h>
#include<util/delay.h>
#include<stdio.h>
#include<compat/deprecated.h>
#include "lcd.h"
#include "uart.h"
/*unsigned int adc(unsigned char channel)
{  unsigned int val;
   ADMUX=0x40+channel;
   ADCSRA=(1<<ADEN);
   ADCSRA|=(1<<ADSC);
    ADCSRA|=0x07;
   while(bit_is_clear(ADCSRA,ADIF));
   val=ADCW; 
   _delay_ms(100);
   return val;
}*/
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
   _delay_ms(1);
   c--;
   }
   val=val/8;
   val=val&0x03FC;
   return val;
}

int main(void)
{float voltage,current;
 unsigned int val;
 char buf[15],bud[15];
DDRA=0xF0;
PORTA=0x00;
DDRD=0x00;
PORTD=0x0F;
lcd_init();
USART_INIT();
lcd_cmd(0x0c);
lcd_cmd(0x01);
lcd_puts("   *DIGIMeter*");
_delay_ms(1000);
lcd_cmd(0x01);
lcd_cmd(0x8C);
lcd_puts("mA");
lcd_cmd(0xc9);
lcd_puts("volts");

 while(1)
 { lcd_cmd(0x82); 
   val=adc(1);
   current=(4.88*val);
   
   sprintf(buf,"%4.2f",current);
   lcd_puts(buf);
    /*rsstr("Current(mA)=");
	rsstr(buf);
	rswritechar('\r');
	rswritechar('\n');
*/
   lcd_cmd(0xc2); 
   val=adc(0);
   voltage=(0.00488*val);
   sprintf(bud,"%2.2f",voltage);
   lcd_puts(bud);
    //rsstr("Voltage(Volts)=");
	
   rsstr(bud);
	rswritechar(',');
	 rsstr(buf);
	 rswritechar('\r');
	rswritechar('\n');
   _delay_ms(200);
 }
} 
 