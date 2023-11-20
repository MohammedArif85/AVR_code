#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>

int main(void)
{unsigned char c;
 DDRD=0xFF;
 PORTD=0x00;
 /*set timer counter initial value*/

TCNT2=0x00;

/*Set timer output compare register*/

OCR2=0x80;

/*start timer:

without presscaler

Non inverted mode in OC2 pin;

FAST PWM*/
TCCR2=(1<<FOC2)|(1<<COM21)|(1<<COM20)|(1<<WGM20)|(1<<WGM21)|(1<<CS00);
 while(1)
 {  
  OCR2=255;
  _delay_ms(2000);
  OCR2=128;
  _delay_ms(2000);
  OCR2=0;
  _delay_ms(2000);
 
  
 }
} 
 