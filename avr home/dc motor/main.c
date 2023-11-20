#include<avr/io.h>
#include<compat/deprectaed.h>
#include<util/delay.h>




int main(void)
{DDRD=0b11110000;
 DDRC=0b00000011;//1-output 0-input
 PORTD=0x0f;
  while(1)
  {
   if(bit_is_clear(PIND,0))
   {
    PORTC=0x01;//turn all led on
    _delay_ms(5);
    PORTC=0x00;//turn all led on
    _delay_ms(5);
   
   } 
   if(bit_is_clear(PIND,1))
   {
    PORTC=0x01;
   } 
   if(bit_is_clear(PIND,2))
   {
    PORTC=0x02;//turn all led on
    _delay_ms(5);
    PORTC=0x00;//turn all led on
    _delay_ms(5);
   } 
   if(bit_is_clear(PIND,3))
   {
    PORTC=0x02;
   } 
  
  
  }

}