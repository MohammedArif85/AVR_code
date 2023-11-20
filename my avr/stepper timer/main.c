#include<avr/io.h>
#include<util/delay.h>

#include<avr/pgmspace.h>
#include<avr/interrupt.h>
 const unsigned char fulln[4]PROGMEM={0x88,0x22,0x44,0x11};
 const unsigned char fulld[4]PROGMEM={0xAA,0x66,0x55,0x99};
 const unsigned char half[8]PROGMEM={0x88,0xAA,0x22,0x66,0x44,0x55,0x11,0x99};
 static volatile int speed=10;
 static volatile uint8_t d=0,c=0;
 //volatile unsigned char fulld[4]={0xA0,0x60,0x50,0x90};
ISR(TIMER0_OVF_vect)
{
 
 c++;
 if(c==speed)
 {
  PORTA=PORTC=pgm_read_byte(&half[d]);
  d++;
  if(d>7)
  {
   d=0;
   
  }
 
  c=0;
 }
 
}





int main(void)
{
 DDRA=0xF0;
 DDRC=0xFF;
 PORTA=0x00;
 PORTC=0x00;
  
 TCNT0=0x00;
 TCCR0=_BV(CS01)|_BV(CS00);
 cli();
 TIMSK=_BV(TOIE0);
 sei();
 while(1)
 {
   speed--;
   if(speed<1)
    speed=10;
   _delay_ms(500);
 }
 
 return 0;
}