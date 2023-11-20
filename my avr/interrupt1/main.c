#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<compat/deprecated.h>
#include "lcd.h"

 volatile unsigned char count;
ISR (INT0_vect)
{ if (bit_is_clear(PINA,6))
  {sbi(PORTA,6);
   count++;
  } 
else
cbi(PORTA,6);
}
int main(void)
{
lcd_init();
lcd_cmd(0x01);
DDRA=0xF0;
DDRD=0xF1;
PORTA=0x00;
PORTD=0x0D;
sbi(PORTA,4);
GICR=(1<<INT0);
MCUCR=(1<<ISC01);
sei();
for(;;)
{count=0;
sei();
 _delay_ms(5000);
 cli();
 lcd_cmd(0x82);
 itoa(count);
 
 
 
}
 
return 0;

}