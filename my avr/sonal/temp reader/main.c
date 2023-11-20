#include<avr/io.h>
#include<util/delay.h>
#include<avr/sfr_defs.h>
#include "lcd.h"
#include<stdio.h>
float temp(char channel)
{float val;
 char ch[8];
  lcd_cmd(0xC2);
   ADMUX=0x40|channel;
   ADCSRA=(1<<ADEN);
   ADCSRA|=(1<<ADSC);
   while(bit_is_clear(ADCSRA,ADIF));
   val=ADCW;
val*=0.00488;
val*=100;
val=val-273;
sprintf(ch,"%.2f",val);
lcd_puts(ch);
return val;
}

int main(void)
{ float val,set=0;
DDRA=0xF0;
DDRC=0xFF;
PORTC=0x00;
PORTA=0x0F;
DDRD=0x00;
PORTD=0xFF;
 lcd_init();
 set=temp(1); 
 lcd_cmd(0x01);
 lcd_cmd(0x80);
 lcd_puts("ADC Test");
 while(1)
 { 
 if(bit_is_clear(PIND,0))
 {
 _delay_ms(1000);
 lcd_cmd(0x01);
 lcd_cmd(0x80);
 lcd_puts("  Set Limit     ");
  while(bit_is_set(PIND,0))
  {
  set=temp(1);  
  }
 _delay_ms(1000);
 lcd_cmd(0x01);
 lcd_cmd(0x80);
 lcd_puts("**Temperature** ");
 }
 else
 {
  val=temp(0);
  if(val>=set)
  PORTA=0xF0;
  else
  PORTA=0x00;
 }

}
 }
   