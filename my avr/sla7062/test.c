#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>

#define cw cbi(PORTB,0); sbi(PORTB,1)
#define ccw cbi(PORTB,1); sbi(PORTB,0)
#define stop cbi(PORTB,0); cbi(PORTB,1)

int main(void)
{
DDRA=0XFF;
DDRB=0XFF;
DDRD=0X00;
PORTD=0xf0;

PORTA=0XFF;
_delay_ms(1000);

PORTA=0X00;
_delay_ms(1000);



while(1)
{
 if(bit_is_clear(PIND,4))
 {
	cw;
	_delay_ms(10);
	stop;
	_delay_ms(20);
	
	if(bit_is_clear(PIND,6))
	{
	 
	 PORTA=0xff;
	}
	else
	{
	 PORTA=0x00;
	}

 }
 else if(bit_is_clear(PIND,5))
 {
  ccw;
  
	_delay_ms(10);
	stop;
	_delay_ms(20);
	if(bit_is_clear(PIND,6))
	{
	 
	 PORTA=0xff;
	 
	}
	else
	{
	 PORTA=0x00;
	}

 }
 else
 {
  stop;
 }
 if(bit_is_clear(PIND,6))
	{
	 
	 PORTA=0xff;
	 
	}
	else
	{
	 PORTA=0x00;
	}
 

}}
