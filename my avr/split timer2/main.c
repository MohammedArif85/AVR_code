#include <avr\io.h>
#include <avr/interrupt.h>
#include<util/delay.h>
#include "lcd.h"

//Global variable for the clock system
volatile unsigned int clock_ms1=0,clock_ms2=0;
volatile unsigned char clock_s1=0,clock_s2=0;

//The output compate interrupt handler
//We set up the timer in such a way that
//this ISR is called exactly at 1ms interval
ISR(TIMER1_COMPA_vect)
{
   clock_ms1++;
   if(clock_ms1==1000)
   {
      clock_s1++;
      clock_ms1=0;
      if(clock_s1==60)
      {
         clock_ms1++;
         clock_s1=0;
      }
   }
}
ISR(TIMER1_COMPB_vect)
{
   clock_ms2++;
   if(clock_ms2==1000)
   {
      clock_s2++;
      clock_ms2=0;
      if(clock_s2==60)
      {
         clock_ms2++;
         clock_s2=0;
      }
   }
}

int main(void) {

//Port D pins as input

DDRD=0x00;

//Enable internal pull ups

PORTD=0xFF;


   //Initialize the LCD Subsystem
   lcd_init();
//TOP=ICR1;

//Output compare OC1A 8 bit non inverted PWM

//Clear OC1A on Compare Match, set OC1A at TOP

//Fast PWM

//ICR1=20000 defines 50Hz PWM

ICR1=20000;

TCCR1A|=(0<<COM1A0)|(1<<COM1A1)|(0<<COM1B0)|(0<<COM1B1)|

(0<<FOC1A)|(0<<FOC1B)|(1<<WGM11)|(0<<WGM10);

TCCR1B|=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|

(0<<CS12)|(1<<CS11)|(0<<CS10);

//start timer with prescaler 8
   //Enable the Output Compare A interrupt
   TIMSK|=(1<<OCIE1A)|(1<<OCIE1B);
    
 //Enable interrupts globally

   sei();

for (;;) {

if(bit_is_clear(PIND, 0)){

//increase duty cycle

OCR1A+=10;

loop_until_bit_is_set(PIND, 0);

}

if(bit_is_clear(PIND, 1)) {

//decease duty cycle

OCR1A-=10;

loop_until_bit_is_set(PIND, 1);

}
 lcd_cmd(0x81);
	  itoa(clock_s1);
	  lcd_data(':');
     lcd_cmd(0x86);
	  itoa(clock_ms1);
	  lcd_cmd(0xC1);
	  itoa(clock_s2);
	  lcd_data(':');
     lcd_cmd(0xC6);
	  itoa(clock_ms2);



}

}