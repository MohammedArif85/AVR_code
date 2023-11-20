#include<avr/io.h>
#include<util/delay.h>






int main(void)
{
 DDRA=0b11110000;
 PORTA=0b00000000;
 while(1)
 {
  PORTA=0b11000000;
   _delay_ms(2500);
   
   PORTA=0b10010000;
   _delay_ms(2500);
  
 }
}