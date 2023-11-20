#include<avr/io.h>
#include<util/delay.h>
#include<sfr_defs.h>
void main()
{
DDRA=0XF0;
DDRB=0x00;
PORTB=0xFF;
DDRD=0x00;
PORTD=0xFF;
PORTA=0x00;
while(1)
{
 if(bit_is_clear(PINB,2))
 {
  PORTA=0xF0;
 }
 else if(bit_is_clear(PIND,0))
 {
  PORTA=0x10;
  }
 else if(bit_is_clear(PIND,1))
 {
  PORTA=0x20;
  }
  else if(bit_is_clear(PIND,2))
 {
  PORTA=0x40;
  }
   else if(bit_is_clear(PIND,3))
 {
  PORTA=0x80;
  }
   else 
 {
  PORTA=0x00;
  }

 
 }
} 