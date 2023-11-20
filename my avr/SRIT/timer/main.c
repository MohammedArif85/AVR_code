#include<avr/io.h>
#include<util/delay.h>
#include<stdio.h>
#include<avr/interrupt.h>
volatile unsigned int count=0;
int main (void)
{
 TCCR0|= (1<<CS02)|(0<<CS00);
 TIMSK|=(1<<TOIE0);
 TCNT0=0;
 count=0;
 DDRA=0XF0;
 PORTA=0X00;
 sei();
 while(1)
 {
 }
}
 ISR(TIMER0_OVF_vect)
 {
  count++;
  if(count==122)
  {
   PORTA=~PORTA;
   count=0;
  }
 } 