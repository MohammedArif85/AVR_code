#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>
void count()
{
 unsigned char c;
 for(c=0;c<16;c++)
 {
  PORTA=c<<4;
  _delay_ms(500);
 } 

}

int  main (void)
{ unsigned char c;
  DDRA=0xF0;
  DDRB=0x00;
  PORTB=0xFF;
  PORTA=0X00;
  while(1)
  {
   if(!(PINB&0x04))
   count();
   else
   PORTA=0x00;
  }

 return 0 ;
}

