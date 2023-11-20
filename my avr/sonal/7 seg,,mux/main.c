#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>

//a b g f e d c .
//1 1 
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
volatile unsigned char d1,d2,d3,d4,count=0,dot=0;
ISR(TIMER0_OVF_vect)
{
 count++;
 if(count%15==0)
 {
   if(dot)
   dot=0x00;
   else
   dot=0x80;
 }
 if(count==31)
 {
   count=0;
   d1++;
   if(d1>9)
    {
      d1=0;
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
	 if(d4>5)
	 {
	  d4=0;
	 }

 } 
}
int main(void)
{
  DDRB=0x0F;
 DDRA=0xFF;
 PORTB=0x00;
 PORTA=0x00;
 TCCR0=(1<<CS00)|(1<<CS02);//1024
 TCNT0=0;
 TIMSK=(1<<TOIE0);
 
 d1=0;
 d2=0;
 d3=0;
 d4=0;
 sei();
 
   while(1)
   {
    PORTB=0x01;
	PORTA=segdisp[d1];
	_delay_ms(1);
    PORTB=0x02;
	PORTA=segdisp[d2];
	_delay_ms(1);
    PORTB=0x04;
	PORTA=segdisp[d3]|dot;
	_delay_ms(1);
    PORTB=0x08;
	PORTA=segdisp[d4];
	_delay_ms(1);
	
  }









}