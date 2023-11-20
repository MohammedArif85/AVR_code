#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include "lcd.h"
volatile unsigned char scalar=0,hr,min,sec,mer=0;
ISR(TIMER0_OVF_vect)
{
 scalar=scalar+1;
 if(scalar==244)
 {
  scalar=0;

  sec++;
  if(sec>59)
  {
   min++;
   sec=0;
  }
  if(min>59)
  {
   min=0;
   hr++;
  }
  if(hr>12)
  {
   hr=1;
   if(mer==0)
   mer=1;
   else
   mer=0; 
  }
 } 

}

void ascii(unsigned char c)
{
  unsigned char a,b;
  a=c%10;
  c=c/10;
  b=c%10;
  lcd_data('0'+b);
  lcd_data('0'+a); 
}
int main(void)
{ 
  // Prescaler = FCPU/256
   TCCR0|=(1<<CS02);

   //Enable Overflow Interrupt Enable
   TIMSK|=(1<<TOIE0);
   sei();

   //Initialize Counter
   TCNT0=0;
   DDRA=0xFF;
   PORTA=0x00;  
   hr=12;
   min=58;
   sec=0;mer=0;
  lcd_init();//initialise the lcd
  lcd_cmd(0x0c);
  lcd_cmd(0x01);
  lcd_puts("    **Clock**");
 
 while(1)
 {
  lcd_cmd(0xc3);
  ascii(hr);
  lcd_data(':');  
  ascii(min);
  lcd_data(':');  
  ascii(sec);
  if(mer==0)
  lcd_puts(" AM");
  else
  lcd_puts(" PM");
  
  
 }

}