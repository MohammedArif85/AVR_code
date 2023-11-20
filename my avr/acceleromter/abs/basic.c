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
{unsigned int t;
DDRA=0xF0;
DDRB=0xFF;
PORTB=0xFF;
DDRC=0xFF; 
PORTC=0x00;
while(1)
 {  
   t=adc(0);
   if(t<200)
   {PORTC=0x55;
    sbi(PORTA,4);  sbi(PORTA,6); 
   }
   else
   {
   PORTC=0x00;
     cbi(PORTA,4);  cbi(PORTA,6); 
  
   }
  // _delay_ms(500);
  
 }
} 
 