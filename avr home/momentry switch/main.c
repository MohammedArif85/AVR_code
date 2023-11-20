#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>


void main()
{
  DDRA=0xF0;
  DDRD=0x00;
  PORTD=0x0F;
  while(1)
  { 
  if(bit_is_clear(PIND,1))
  {_delay_ms(300);
  while(!bit_is_set(PIND,1));
  PORTA=~PORTA;
   
  }
 
   
 
 }

}