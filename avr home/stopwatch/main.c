#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include "lcd.h"
volatile unsigned int min,sec,msec;
ISR(TIMER0_OVF_vect)
{    msec++;
	if(msec>9999)
	{
	PORTA=~PORTA;
	msec=0;
	sec++;
	}
	if(sec>59)
	{
	sec=0;
	min++;
	}
	 TCNT0=56;
	
}
void ascii(unsigned char c)
{
  unsigned char a,b;
  a=c%10;
  c=c/10;
  b=c%10;
  lcd_data(0x30+b);
  lcd_data(0x30+a); 
}
int main(void)
{ DDRA=0xFF;
  DDRD=0x00;//port input
  PORTD=0xFF;//pull up
  
  lcd_init();//initialise the lcd
  lcd_cmd(0x01);//clear screen
   lcd_puts("  *STOPWATCH*   ");
   min=0;
   sec=0;
   msec=0;
   //selecting FCPU/8
   TCCR0|=(1<<CS01);
   //enable interrupt
   TIMSK|=(1<<TOIE0);
   TCNT0=56;
   sei();
   while(1)
   {
    lcd_cmd(0xc2);
	ascii(min);
	lcd_puts(":");
	ascii(sec);
	lcd_puts(":");
	ascii(msec);
	if(bit_is_clear(PIND,0))
    {
	 cli();	
	}
	if(bit_is_clear(PIND,1))
    {
	 min=0;
   sec=0;
   msec=0;
  
	}
	if(bit_is_clear(PIND,2))
    {
	sei();
	}
	}

 







}