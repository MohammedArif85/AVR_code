#include<avr/io.h>
#include<util/delay.h>
 int main(void)
{
 DDRD=0X00;
 PORTD=0XFF;
 DDRA=0xF0;
 PORTA=0x00;
 while(1)
 {
   PORTA=0x10;
   _delay_ms(250);
   PORTA=0x20;
   _delay_ms(250);
   PORTA=0x40;
   _delay_ms(250);
   PORTA=0x80;
   _delay_ms(250);
   
 }return 0;
}
