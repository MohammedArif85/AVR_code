#include<avr/io.h>
#include<util/delay.h>
int main(void)
{ unsigned char count=0;
  
   TCCR0=0x06;
   TCNT0=0x00;
   OCR0=0x00;
   DDRB=0x00;
   DDRA=0xFF;
   PORTB=0x01;
   while(1)
   {
   TCCR0=0x06;
   //TCNT0=0x00;
   
   _delay_ms(1000);
   count=TCNT0;
   if(count==1)
   {
   PORTA=0b00010000;
   }
   if(count==2)
   {
   PORTA=0b00100000;
   }
   if(count==3)
   {
   PORTA=0b01000000;
   }
   if(count==4)
   {
   PORTA=0b10000000;
   }
   //TCCR0=0x00;
   //TCNT0=0x00;
     }

}