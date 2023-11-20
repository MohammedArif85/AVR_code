#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include"lcd.h"
volatile  unsigned char seg[]=
{
 0b01111011,
 0b01000010,
 0b00110111,
 0b01100111,
 0b01001110,
 0b01101101,
 0b01111101,
 0b01000011,
 0b01111111,
 0b01101111
};
  
volatile unsigned char d1,d2,d3,d4,count=0;
ISR(TIMER0_OVF_vect)
{  TCNT0=131;  
    count++; 
    if(count==7500)
	{
    count=0;
	d1++;
	}
	if(d1>9)
	{d1=0;d2++;
	}
	if(d2>5)
	{d2=0;d3++;
	}
	if(d3>9)
	{d3=0;d4++;
	}
	if((d4==1)&(d3>1))
	d1=d2=d3=d4=0;
	
}

  
int main(void)
{DDRD=0x00;
PORTD=0xFF;
  DDRB=0x0F;
 DDRA=0xFF;
 PORTB=0x00;
 PORTA=0x00;
 count=0;
 TCNT0=131;
 TCCR0=((1<<CS00)|(1<<CS02));
 TIMSK=(1<<TOIE0);
 d1=d2=d3=d4=0;
 lcd_init();
 lcd_cmd(0x01);
 lcd_puts("DIGITAL CLOCK");
 _delay_ms(1000);
 

  while(1)
   {
    PORTB=0x01;
	PORTA=seg[d1];
	_delay_ms(1);
    PORTB=0x02;
	PORTA=seg[d2];
	_delay_ms(1);
    PORTB=0x04;
	PORTA=seg[d3];
	_delay_ms(1);
    PORTB=0x08;
	PORTA=seg[d4];
	_delay_ms(1);
	
	if(!(PIND&0x01))    //set hour
 {
 while(!(PIND&0x01));
 d3++;
 if(d3>9)
 {d4++;
 d3=0;
 }
 if((d4==1)&(d3>1))
 d4=d3=0;
 }
 if(!(PIND&0x02))    //set minute
 {
 while(!(PIND&0x02));
 d1++;
 if(d1>9)
 {d2++;
 d1=0;
 }
 if(d2>59)
 d1=d2=0;
 }
  if(!(PIND&0x04))   //start
 {
 while(!(PIND&0x04));
  count=0;
 TCNT0=131;
 TCCR0=((1<<CS00)|(1<<CS02));
 TIMSK=(1<<TOIE0);
 sei();
  }
  if(!(PIND&0x08))   //stop
 {
 while(!(PIND&0x08));
 cli();
  }
   }
 }