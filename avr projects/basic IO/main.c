#include<avr/io.h>
#include<util/delay.h>


void main()
{
 DDRC=0b00001111;
 while(1)
 {
  PORTC=0b00000001;
  _delay_ms(100);
  PORTC=0b00000010;
  _delay_ms(100); 
  PORTC=0b00000100;
  _delay_ms(100);
  PORTC=0b00001000;
  _delay_ms(100);  
 }
}
