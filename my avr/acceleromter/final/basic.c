#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>
unsigned int adc(unsigned char channel)
{  unsigned char c=0;
unsigned int val; 
 ADMUX=0x60|channel;//for channel selection
 ADCSRA|=(1<<ADEN);//to enable adc
 
 ADCSRA|=(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//adsc- start conversion
 
 for(c=0;c<8;c++)
 {
 ADCSRA|=_BV(ADSC);
 while(!(ADCSRA&(1<<ADIF)));//poll data untill ADIF adc interrupt flag=1 or conversion completed
 val+=ADCH;
 _delay_us(20);
 
 }
 val=val/8;
 
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
   if(xaxis>80&& xaxis<94 && yaxis >100 &&yaxis<115)
   {
    cbi(PORTA,0);cbi(PORTA,1);cbi(PORTA,2);cbi(PORTA,3); 
   }
   /*else if(xaxis>165&& xaxis<220 && yaxis >95 &&yaxis<120)
   {
   sbi(PORTA,0);cbi(PORTA,1);sbi(PORTA,2);cbi(PORTA,3);  
   }*/
   else if(xaxis>40&& xaxis<70 && yaxis >100 &&yaxis<120)
   {
   cbi(PORTA,0);sbi(PORTA,1);cbi(PORTA,2);sbi(PORTA,3);  
   }
    else if(xaxis>60&& xaxis<110 && yaxis >110 &&yaxis<150)
   {
   sbi(PORTA,0);cbi(PORTA,1);cbi(PORTA,2);sbi(PORTA,3);  
   }
   else if(xaxis>75&& xaxis<120  &&yaxis<92 &&yaxis>42)
   {
   cbi(PORTA,0);sbi(PORTA,1);sbi(PORTA,2);cbi(PORTA,3);  
   }
  
   else
   {
    sbi(PORTA,0);cbi(PORTA,1);sbi(PORTA,2);cbi(PORTA,3);
   
   }
  // _delay_ms(500);
  
 }
} 
 