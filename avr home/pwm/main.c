#include<avr/io.h>
#include<compat/deprectaed.h>
#include<util/delay.h>



int main(void)
{DDRD=0b11110000;
 DDRA=0b11110000;//1-output 0-input
 PORTD=0x0f;
  while(1)
  {
   if(bit_is_clear(PIND,0))
   {
    PORTA=0xF0;//turn all led on
    _delay_ms(5);
    PORTA=0x00;//turn all led off
    _delay_ms(5);
   } 
   if(bit_is_clear(PIND,1))
   {
    PORTA=0xF0;//turn all led on
    _delay_ms(3);
    PORTA=0x00;//turn all led off
    _delay_ms(7);
   } 
   if(bit_is_clear(PIND,2))
   {
    PORTA=0xF0;//turn all led on
    _delay_ms(1);
    PORTA=0x00;//turn all led off
    _delay_ms(9);
   } 
    if(bit_is_clear(PIND,3))
   {
    PORTA=0xF0;//turn all led on
   } 
  
  
  }

}