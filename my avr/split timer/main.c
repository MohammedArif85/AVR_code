#include <avr/io.h>
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

int main(void)
{  DDRD=0x00;
   PORTD=0x0F;
   //Initialize the LCD Subsystem
   lcd_init();
   //Clear the display
   lcd_cmd(0x01);
 TCCR1B = 0x00; //Stop the timer
 OCR1A=250;
 OCR1B=125;
 ICR1H  = 0x00;
 ICR1L  = 0x00;
 TCCR1A = 0b00000001; // 0x01
 TCCR1B = 0b01001011; // 0x48  
 
   //Set up the timer1 as described in the
   //tutorial
/*   TCCR1A=(1<<WGM10);
   TCCR1B=(1<<CS11)|(1<<CS10);
   OCR1A=125;
   OCR1B=250;*/

   //Enable the Output Compare A interrupt
   TIMSK|=(1<<OCIE1A)|(1<<OCIE1B);



   //Enable interrupts globally

   sei();

   //Continuasly display the time
   while(1)
   {/*
     if(!(PIND&0x01))
	 {
	  while(!(PIND&0x01));
	  OCR1A+=20;
	  if(OCR1A>250)
	  OCR1A=250;	 
	 }
     if(!(PIND&0x02))
	 {
	  while(!(PIND&0x02));
	  OCR1B+=20;
	  if(OCR1B>250)
	  OCR1B=250;	 
	 }*/
    
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

