#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>
#include "lcd.h"

#define tbi(x,y) x^=(1<<y);


unsigned char cpx,cpy;
unsigned char adc(unsigned char channel)
{  unsigned int val;
   unsigned char c;
   ADMUX=0x60+channel;
   ADCSRA=(1<<ADEN);
   c=8;val=0;
   while(c--)
   {
   ADCSRA|=(1<<ADSC);
    ADCSRA|=0x07;
   while(bit_is_clear(ADCSRA,ADIF));
   sbi(ADCSRA,ADIF);
   val=val+ADCH; 
   }
   val=val/8;
   val=val&0x00ff;
   return val;
}

void Xread()
{
 DDRA=0b11110101;
 
 sbi(PORTA,0);
 cbi(PORTA,2);
 cpx=adc(1);
 lcd_cmd(0xc0);
 lcd_puts("X="); 
 itoa(cpx);
}
void Yread()
{
 DDRA=0b11111010;
 sbi(PORTA,1);
 cbi(PORTA,3);
 cpy=adc(0);
 lcd_cmd(0xc7);
 lcd_puts("Y="); 
 itoa(cpy);
}
int main(void)
{

 lcd_init();
 lcd_cmd(0x0c);//display on curser off 
 lcd_cmd(0x01);//display on curser off 
 lcd_puts("ADC TEST"); 
 DDRC=0xFF;
  while(1)
 {
 Xread();
 Yread();
 if(cpx>157 && cpx<230 && cpy<202&&cpy>142)
 {
 Xread();
 Yread();
 
 _delay_ms(10);
 if(cpx>157 && cpx<230 && cpy<202&&cpy>142)
 {
 tbi(PORTC,0);_delay_ms(800);
 }
 }
 
 
 if(cpx>33 && cpx<101 && cpy<201&&cpy>143)
 {
 Xread();
 Yread();
 
 _delay_ms(10);
 if(cpx>33 && cpx<101 && cpy<201&&cpy>143)
 {
 tbi(PORTC,1);_delay_ms(800);
 }
 }
 
 
 if(cpx>41 && cpx<108 && cpy<119&&cpy>52)
 {
 Xread();
 Yread();
 
 _delay_ms(10);
 if(cpx>41 && cpx<108 && cpy<119&&cpy>52)
 {
 tbi(PORTC,2);_delay_ms(800);
 }
 }
 
 if(cpx>136 && cpx<220 && cpy<113&&cpy>46)
 {
 Xread();
 Yread();
 
 _delay_ms(10);
 if(cpx>136 && cpx<220 && cpy<113&&cpy>46)
 {
 tbi(PORTC,3);_delay_ms(800);
 }
 }
 
 
}
}