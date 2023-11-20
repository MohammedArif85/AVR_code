#include<avr/io.h>
#include<util/delay.h>
#include "lcd.h"
int adcread(unsigned char channel)
{
 ADMUX=0x40|channel;
 ADCSRA|=(1<<ADEN);
 ADCSRA|=(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
 while(!(ADCSRA&(1<<ADIF)));
 return ADC;

}
int main(void)
{int t; 
  unsigned char r;
 lcd_init();
 lcd_cmd(0x0c);//display on curser off 
 lcd_cmd(0x01);//display on curser off 
 lcd_puts("ADC TEST"); 
 DDRA=0xF0;
 DDRC=0xFF;
 PORTC=0x00;
 while(1)
 {
  lcd_cmd(0xc2);
  t=adcread(0);
  itoa(t);
  if(t>30)
  PORTA=0xF0;
  else
  PORTA=0x00;
  
 }

}