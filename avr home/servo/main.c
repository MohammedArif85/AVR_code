#include <avr\io.h>
#include<compat/deprecated.h>

int main(void) {

//Port D pins as input

DDRD=0x00;

//Enable internal pull ups

PORTD=0xFF;

//Set PORTB1 pin as output

DDRB=0xFF;

//TOP=ICR1;

//Output compare OC1A 8 bit non inverted PWM

//Clear OC1A on Compare Match, set OC1A at TOP

//Fast PWM

//ICR1=20000 defines 50Hz PWM

ICR1=20000;

TCCR1A|=(0<<COM1A0)|(1<<COM1A1)|(0<<COM1B0)|(1<<COM1B1)|

(0<<FOC1A)|(0<<FOC1B)|(1<<WGM11)|(0<<WGM10);

TCCR1B|=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|

(0<<CS12)|(1<<CS11)|(0<<CS10);

//start timer with prescaler 8

for (;;) {

if(bit_is_clear(PIND, 0)){

//increase duty cycle

OCR1A+=10;
OCR1B-=10;
loop_until_bit_is_set(PIND, 0);

}

if(bit_is_clear(PIND, 1)) {

//decease duty cycle

OCR1A-=10;
OCR1B+=10;
loop_until_bit_is_set(PIND, 1);

}

}

}