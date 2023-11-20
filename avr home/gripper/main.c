#include <avr\io.h>

/*
Ocr  pulse values 
38      600us 0deg
66      1050  45
94      1500  90
122     1950  135
150     2400  180
*/

int main(void) {

//Port D pins as input

DDRD=0xF0;

//Enable internal pull ups

PORTD=0xFF;

//Set PORTB1 pin as output

//DDRB=0xFF;

//TOP=ICR1;

//Output compare OC1A 8 bit non inverted PWM

//Clear OC1A on Compare Match, set OC1A at TOP

//Fast PWM

//ICR1=1250 defines 50Hz PWM-16mhz/256

ICR1=1250;

TCCR1A|=(0<<COM1A0)|(1<<COM1A1)|(0<<COM1B0)|(1<<COM1B1)|

(0<<FOC1A)|(0<<FOC1B)|(1<<WGM11)|(0<<WGM10);

TCCR1B|=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|

(1<<CS12)|(0<<CS11)|(0<<CS10);

//start timer with prescaler 8

for (;;) {

if(bit_is_clear(PIND, 0)){

//increase duty cycle

OCR1A=38;
OCR1B=38;

loop_until_bit_is_set(PIND, 0);

}

if(bit_is_clear(PIND, 1)) {

//decease duty cycle

OCR1A=66;
OCR1B=66;

loop_until_bit_is_set(PIND, 1);

}

if(bit_is_clear(PIND, 2)){

//increase duty cycle

OCR1A=94;
OCR1B=94;

loop_until_bit_is_set(PIND, 2);

}

if(bit_is_clear(PIND, 3)) {

//decease duty cycle

OCR1A=150;
OCR1B=150;

loop_until_bit_is_set(PIND, 3);

}


}

}