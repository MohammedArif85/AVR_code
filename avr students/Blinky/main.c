#include<avr/io.h>
#include<compat/deprecated.h>
#include<util/delay.h>








int main(void)
{
 DDRA=0x0F;
 while(1)
 {
  PORTA=0x0F;
  _delay_ms(100);
  PORTA=0x00;
  _delay_ms(100);
 
 }
 return 0;
}