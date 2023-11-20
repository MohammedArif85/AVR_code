#include<avr/io.h>
#include<util/delay.h>

unsigned char seg[]={0x77,0x24,0x6b,0x6e,0x5c,0x3c,0x3f,0x64,0x7f,0x7d};

int main(void)
{
 DDRC=0xff;
 PORTC=0x00;
 DDRD=0xff;
 PORTD=0xff;
 unsigned int i;
 while(1)
 {
  for(i=0;i<8;i++)
  {
   PORTC=seg[i];
   _delay_ms(1000);
  }
 }
}