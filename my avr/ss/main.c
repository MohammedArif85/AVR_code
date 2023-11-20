#include<avr/io.h>
#include<util/delay.h>
int  main (void)
{ 
  DDRA=0xF0;
  PORTA=0X50;
  while(1)
  {
   PORTA=~PORTA;
  _delay_ms(200);
  }

 return 0 ;
}

