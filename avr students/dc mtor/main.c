#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>





void main()
{
 DDRC=0xFF;
 DDRD=0x00;
 PORTD=0x03;
 while(1)
 {
  if(bit_is_clear(PIND,0))
  {
    PORTC=0x80;
  
  }
  else if(bit_is_clear(PIND,1))
  {
   PORTC=0x40;
  
  }
  else
  {
   PORTC=0x00;
  }
  

 }
}