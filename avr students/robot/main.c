#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>

int adcread(unsigned char channel)
{
 ADMUX=0x40|channel;
 ADCSRA|=(1<<ADEN);
 ADCSRA|=(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
 while(!(ADCSRA&(1<<ADIF)));
 return ADC;
}
void forward()
{
 PORTC=0b01100000;
}
void backward()
{
 PORTC=0b10010000;
}
void left()
{
 PORTC=0b00100000;
}
void right()
{
 
 PORTC=0b01000000;
}

void stop()
{
 PORTC=0x00;
}
int main(void)
{
int a1d,a2d,a3d,a1l,a2l,a3l,a1t,a2t,a3t; 
 unsigned char eyeval;
 DDRA=0xF0;
 DDRC=0xFF;
 PORTC=0x00;
 DDRD=0x00;
 PORTD=0x03;
 PORTA=0xF0;
 _delay_ms(2000);
 PORTA=0x00;
 eyeval=0x00;
   while(bit_is_set(PIND,0));
   _delay_ms(500);
   while(bit_is_clear(PIND,0));
   PORTA=0x10;
  _delay_ms(1000);
  a1d=adcread(0);
  a2d=adcread(1);
  a3d=adcread(2);
   while(bit_is_set(PIND,0));
   _delay_ms(500);
   while(bit_is_clear(PIND,0));
   PORTA=0x20;
   _delay_ms(1000);
  a1l=adcread(0);
  a2l=adcread(1);
  a3l=adcread(2);
  a1t=(a1d+a1l)/2;
  a2t=(a2d+a2l)/2;
  a3t=(a3d+a3l)/2;
   PORTA=0xF0;
   _delay_ms(400);
   PORTA=0x00;
   _delay_ms(400);
   PORTA=0xF0;
   _delay_ms(400);
   PORTA=0x00;
   _delay_ms(400);
   
 while(1)
 {
   a1d=adcread(0);
  a2d=adcread(1);
  a3d=adcread(2);
   if(a1d>a1t)
   {
   sbi(PORTA,4);
    sbi(eyeval,4);
   }
   else
   {
   cbi(PORTA,4);
   cbi(eyeval,4);
   }
   if(a2d>a2t)
   {
   sbi(PORTA,5);
   sbi(eyeval,5);
   }
   else
   {
   cbi(PORTA,5);
   cbi(eyeval,5);
   }
   if(a3d>a3t)
   {
   sbi(PORTA,6);
   sbi(eyeval,6);
   }
   else
   {
   cbi(PORTA,6);
   cbi(eyeval,6);
   }
   eyeval=eyeval&0x70;
   switch(eyeval)
   {
    case 0x50:forward();_delay_ms(7);stop();_delay_ms(3);break;
	case 0x10:left();_delay_ms(4);stop();_delay_ms(6);break;
	case 0x30:left();_delay_ms(5);stop();_delay_ms(5);break;
	case 0x40:right();_delay_ms(4);stop();_delay_ms(6);break;
	case 0x60:right();_delay_ms(5);stop();_delay_ms(5);break;
	default: stop();
   }
   
 }

}