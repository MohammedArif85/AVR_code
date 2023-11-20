#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>

#define cw cbi(PORTB,0); sbi(PORTB,1)
#define ccw cbi(PORTB,1); sbi(PORTB,0)
#define stop cbi(PORTB,0); cbi(PORTB,1)

int main(void)
{
unsigned char c=0;
DDRA=0XFF;
DDRB=0XFF;
DDRC=0X00;
DDRD=0X00;
PORTC=0XF0;
PORTD=0xf0;
stop;
PORTA=0XFF;
_delay_ms(1000);

PORTA=0X00;
_delay_ms(1000);

 
while(1)
{
 
 cw;
 _delay_ms(7);
 
 stop;
 _delay_ms(7);
 if(bit_is_clear(PINC,4))
 {
   
   while(bit_is_clear(PINC,4));
   PORTA^=0x01;
   c++;
 
 }
 if(c>30)
  {
   ccw;
   _delay_ms(4);
  stop;
   while(1);
  }
}

}
