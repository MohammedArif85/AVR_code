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
   return val;
}

int main(void)
{unsigned int xaxis,yaxis;
 unsigned char direction;
DDRA=0xF0;
DDRB=0xFF;
PORTB=0xFF;
PORTA=0x00;
 direction=0x00;
 while(1)
 {  
   xaxis=adc(0);
   yaxis=adc(1);
   if(xaxis>250)
   {
   sbi(PORTA,4);
   sbi(direction,0);
   
   }
   else
   {
   cbi(PORTA,4);
   cbi(direction,0);
   }
   if(yaxis>250)
   {
   sbi(PORTA,5);
   sbi(direction,1);
   
   }
   else
   {
   cbi(PORTA,5);
   cbi(direction,1);
   }
  switch(direction)
  {
   case 0x01:PORTB=0x0A;break;
   case 0x02:PORTB=0x05;break;
   case 0x03:PORTB=0x0E;break;
   default:PORTB=0x0F;
  }
 }
} 
 