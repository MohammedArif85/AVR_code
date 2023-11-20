 #include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include<avr/pgmspace.h>
// . d e c g f a b
// 0 1 1 1 0 1 1 1 
volatile unsigned char segdisp[]=
{
 0b01110111,
 0b00010001,
 0b01101011,
 0b01011011,
 0b00011101,
 0b01011110,
 0b01111110,
 0b00010011,
 0b01111111,
 0b00011111,
 0b00000000
};

int main(void)
{
 unsigned char c,d;
 DDRA=0xFF;
 DDRB=0xf0;
 PORTB=0x00;
 PORTA=0x00;
 
 while(1)
 {
 PORTA=segdisp[1];
 PORTB=0x10;
 _delay_ms(1);
 PORTA=segdisp[1];
 PORTB=0x20;
 _delay_ms(1);
 PORTA=segdisp[1];
 PORTB=0x40;
 _delay_ms(1);
 PORTA=segdisp[1];
 PORTB=0x80;
 _delay_ms(1);
 
 }


}
 