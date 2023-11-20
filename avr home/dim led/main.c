#include<avr/io.h>
#include<compat/deprectaed.h>
#include<util/delay.h>




int main(void)
{int i,j;
 DDRD=0b11110000;
 DDRA=0b11110000;//1-output 0-input
 PORTD=0x0f;
  while(1)
  {
  for(i=0;i<=100;i++)
  {
  for(j=0;j<10;j++)
  {
  PORTA=0xF0;
  _delay_us(i*50);
  PORTA=0x00;
  _delay_us((100-i)*50);
  }
  }
   for(i=100;i>=0;i--)
  {
  for(j=0;j<10;j++)
  {
  PORTA=0xF0;
  _delay_us(i*50);
  PORTA=0x00;
  _delay_us((100-i)*50);
  }
  }
  _delay_ms(1000);
  }
  

}