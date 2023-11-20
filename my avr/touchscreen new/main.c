#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>
#include "lcd.h"
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
 
  while(1)
 {
 Xread();
 Yread();
 if(cpx>210 && cpx<230 && cpy<60&&cpy>40)
 sbi(PORTA,4);
 else
 cbi(PORTA,4);
 if(cpx>40&&cpx<60 && cpy>40&&cpy<67)
 sbi(PORTA,5);
 else
 cbi(PORTA,5);
}
}