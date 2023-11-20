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
DDRA=0x0F;
DDRB=0xFF;
PORTB=0xFF;
PORTA=0x00;
 direction=0x00;
 while(1)
 {  
   xaxis=adc(4);
   yaxis=adc(5);
   if(xaxis>250)
   {
   sbi(PORTA,0);
   sbi(PORTA,2);  
   }
   else
   {
    
   cbi(PORTA,0);
   cbi(PORTA,2);
   }
   if(yaxis>250)
   {
   sbi(PORTA,1);
   sbi(PORTA,3);

   }
   else
   {
   cbi(PORTA,1);
   cbi(PORTA,3);
    }
  
 }
} 
 