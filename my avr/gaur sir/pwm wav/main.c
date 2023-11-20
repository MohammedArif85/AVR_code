#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include<compat/deprecated.h>






void setup()
{
    
	/*start timer:

without presscaler

Non inverted mode in OC2 pin;

FAST PWM*/
TCCR2=(1<<FOC2)|(1<<COM21)|(1<<COM20)|(1<<WGM20)|(1<<WGM21)|(1<<CS00);

   
}

int main(void)
{unsigned char c;
 setup();
 DDRD=0xFF;
 
 while(1)
 {  
  for(c=0;c<256;c++)
  {
   OCR2=c;
   _delay_ms(100);
  }
  for(c=255;c>=0;c--)
  {
   OCR2=c;
   _delay_ms(100);
  }
  
 }
 return 0;
} 

