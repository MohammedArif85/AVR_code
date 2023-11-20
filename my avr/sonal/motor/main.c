#include<avr/io.h>
#include<util/delay.h>
#include<sfr_defs.h>
int main(void)
{DDRD=0x00;
PORTD=0xFF;
DDRC=0XFF;
PORTC=0x00;
while(1)
{

if(!(PIND&0x01))
{
_delay_ms(500);
while(!(PIND&0x01))
PORTC=0x55;
}
else if(!(PIND&0x02))
{
_delay_ms(500);
while(!(PIND&0x02))
PORTC=0xAA;
}
else
{
 PORTC=0x00;
}
}
}