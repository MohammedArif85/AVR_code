#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>

volatile unsigned char segdisp[]=
{
 0b01111011, 
 0b01000010,
 0b00110111,
 0b01100111,
 0b01001110,
 0b01101101,
 0b01111101,
 0b01000011,
 0b01111111,
 0b01101111
 
}; 

int main (void)
{
 int i;
 DDRA=0xFF;
 PORTA=0x00;
 while(1)
 {
 for(i=0;i<10;i++)
 PORTA=segdisp[i];
 _delay_ms(1000);
 }return 0;
}