#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<compat/deprecated.h>
//static volatile uint8_t led;
ISR (INT0_vect)
{ if (bit_is_clear(PINA,6))
sbi(PORTA,6);
else
cbi(PORTA,6);
}
int main(void)
{
DDRA=0xF0;
DDRB=0xFB;
DDRD=0xF1;
PORTA=0x00;
PORTB=0x04;
PORTD=0x0D;
sbi(PORTA,4);
sbi(PORTB,2);
sbi(PORTD,1);
sbi(PORTD,2);
sbi(PORTD,3);
GICR=(1<<INT0);
MCUCR=(1<<ISC01);
sei();
for(;;)
{
 sbi(PORTA,5);
 _delay_ms(250);
 cbi(PORTA,5);
 _delay_ms(250);
 
}
 
return 0;

}