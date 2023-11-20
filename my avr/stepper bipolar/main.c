#include<avr/io.h>
#include<util/delay.h>
#include<avr/pgmspace.h>
 const unsigned char fulln[4]PROGMEM={0x88,0x22,0x44,0x11};
 const unsigned char fulld[4]PROGMEM={0xAA,0x66,0x55,0x99};
 const unsigned char half[8]PROGMEM={0x88,0xAA,0x22,0x66,0x44,0x55,0x11,0x99};

int main(void)
{
 
 
 
 
 unsigned char c,d,e;
 DDRA=0xF0;
 DDRC=0xFF;
 DDRD=0x00;
 PORTA=0x00;
 PORTC=0x00;
 PORTD=0x0F;
 
 while(1)
 {
  c=PIND&0x0F;
  switch(c)
  {
   case 0x0E :
				for(d=0;d<4;d++)
				{
				 PORTA=PORTC=pgm_read_byte(&fulln[d]);
				 _delay_ms(3);
				};
   break;
   
   case 0x0D : 
				for(d=0;d<4;d++)
				{
				 PORTA=PORTC=pgm_read_byte(&fulld[d]);
				 _delay_ms(3);
				 
				};
   break;
   
   case 0x0B : 
				for(e=0;e<8;e++)
				{
				 PORTA=PORTC=pgm_read_byte(&half[e]);
				 _delay_ms(3);
				 				 
				};
   break;
   
   case 0x07 : 
				for(e=0;e<8;e++)
				{
				 PORTA=PORTC=pgm_read_byte(&half[e]);
				 _delay_ms(2);
				 
				};
   break;
   
   default : PORTA=0x00;PORTC=0x00;
 
  }
 }
 
 
 
 
 return 0;
 
}