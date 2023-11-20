#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include<compat/deprecated.h>
unsigned int i=0;

int main(void)
{
DDRD=0xFB;         // PORTD declared as output port  PORTD_2 input
PORTD=0xFF;        // PORTD pulls enabled, initialized
GICR=1<<INT0;   // INT0 enabled
sbi(MCUCR,1);       // falling edge at INT0 generates interrupt
cbi(MCUCR,0);
sei();  // Interrupt flag set
while(1);

return 0;

}

SIGNAL(	SIG_INTERRUPT0 )  
{

cli();
DDRD=0xFF;
_delay_us(10);
PORTD=0xFF;
_delay_ms(5000);
PORTD=0x00;
sei();
DDRD=0x00;

}

