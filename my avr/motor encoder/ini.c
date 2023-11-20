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

PORTA=0XFF;
_delay_ms(1000);

PORTA=0X00;
_delay_ms(1000);

do
 {
	cw;
	_delay_ms(5);
	stop;
	_delay_ms(5);
	if(bit_is_clear(PINC,4))
	{
	 _delay_ms(10);
	loop_until_bit_is_set(PINC,4); 
	c++;
	PORTA=0XFF;

	}
	else
	{
	PORTA=0x00;
	}
 }
 while(c==15);
 

while(1)
{
 
}

}
