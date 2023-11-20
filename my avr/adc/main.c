#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>
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
{
 unsigned int val;
DDRA=0xF0;
DDRB=0xFF;
PORTB=0xFF;
PORTA=0x00;

 while(1)
 {  
   val=adc(1);
   
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
 