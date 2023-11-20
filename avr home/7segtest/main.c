#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include<avr/pgmspace.h>
// . d e c g f a b
// 0 1 1 1 0 1 1 1 
  unsigned char segdisp[]=
{
 0b01110111,
 0b00010001,
 0b01101011,
 0b01011011,
 0b00011101,
 0b01011110,
 0b01111110,
 0b00010011,
 0b01111111,
 0b00011111
}; 
volatile unsigned char d1,d2,d3,d4,count=0,dot=0,sec=0;
ISR(TIMER0_OVF_vect)
{
 count++;
 if(count==61)
 {
  count=0;
  
  if(dot==0)
  {
   dot=0x80;
  }
  else
  {
   dot=0x00;
  }
  
  d1++;
  if(d1>9)
  {d1=0;
   d2++;
  }
  if(d2>5)
  {
   d2=0;
   d3++;
  }
  if(d3>9)
  {
   d3=0;
   d4++;
  }
  
 }
}
int main(void)
{
  DDRB=0x0F;
 DDRA=0xFF;

 PORTA=0x00;
 //selecting FCPU/1024
   TCCR0|=(1<<CS02)|(CS01);
   //enable interrupt
   TIMSK|=(1<<TOIE0);
   TCNT0=0;
   
 d1=0;
 d2=0;
 d3=0;
 d4=0;sec=0;
 sei();
 
   while(1)
   { 
    PORTB=0x01;
	PORTA=segdisp[d1]|dot;
	_delay_ms(1);
    PORTB=0x02;
	PORTA=segdisp[d2]|dot;
	_delay_ms(1);
    PORTB=0x04;
	PORTA=segdisp[d3]|dot;
	_delay_ms(1);
    PORTB=0x08;
	PORTA=segdisp[d4]|dot;
	_delay_ms(1);
    PORTB=0xFF; 
	
  }









}