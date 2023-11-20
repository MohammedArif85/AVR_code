#include <avr\io.h>
#include<compat/deprecated.h>
#include<util/delay.h>
void pick()
{
OCR1A=2200;//arm down
OCR1B=2200;//gripper open
_delay_ms(1000);
_delay_ms(1000);
_delay_ms(1000);
_delay_ms(1000);

OCR1A=1010;//arm down
OCR1B=1010;//gripper open
}
void initial()
{
OCR1A=1010;//arm down
OCR1B=1010;//gripper open
}


int main(void) {

//Port D pins as output

DDRD=0xF0;
PORTD=0x0F;

//Set PORTB pin as input
DDRB=0x00;
//Enable internal pull ups
PORTB=0xFF;

//TOP=ICR1;

//Output compare OC1A 8 bit non inverted PWM

//Clear OC1A on Compare Match, set OC1A at TOP

//Fast PWM

//ICR1=20000 defines 50Hz PWM

ICR1=40000;

TCCR1A|=(0<<COM1A0)|(1<<COM1A1)|(0<<COM1B0)|(1<<COM1B1)|

(0<<FOC1A)|(0<<FOC1B)|(1<<WGM11)|(0<<WGM10);

TCCR1B|=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|

(0<<CS12)|(1<<CS11)|(0<<CS10);

//start timer with prescaler 8
initial();
for (;;) {

if(bit_is_clear(PIND, 0)){

pick();
loop_until_bit_is_set(PIND, 0);

}

}

}