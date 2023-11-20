#include<avr/io.h>
#include<compat/deprecated.h>
#include<util/delay.h>
#include "lcd.h"
int adcread(unsigned char channel)
{
unsigned char c=0;
unsigned int val; 
 ADMUX=0x60|channel;//for channel selection
 ADCSRA|=(1<<ADEN);//to enable adc
 
 ADCSRA|=(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//adsc- start conversion
 
 for(c=0;c<8;c++)
 {
 ADCSRA|=_BV(ADSC);
 while(!(ADCSRA&(1<<ADIF)));//poll data untill ADIF adc interrupt flag=1 or conversion completed
 val+=ADCH;
 _delay_us(20);
 
 }
 val=val/8;
 
 return val;// return value
}



void main()
{unsigned int t;
  char buffer[10];
 lcd_init();
 DDRA=0b00000000;
 DDRC=0xFF;
 lcd_cmd(0x01);
 lcd_puts("Hello Dear");
 lcd_cmd(0xc0);
 lcd_puts("16X2 LCD DEMO");
 _delay_ms(2000);
 lcd_cmd(0x01);
 while(1)
 {
    lcd_cmd(0xc2);
    t=adcread(0);//read adc channel 0
    sprintf(buffer,"x=%d  ",t);
	lcd_puts(buffer);
    t=adcread(1);//read adc channel 0
    sprintf(buffer,"y=%d  ",t);
	lcd_puts(buffer);
	if(t>400)
	PORTC=0xFF;
	else
	PORTC=0x00;
  
 }
}
