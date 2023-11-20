#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include<avr/pgmspace.h>
// . d e c g f a b
// 0 1 1 1 0 1 1 1 
  unsigned char segdisp[]=
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
 0b00011111
}; 
volatile int count=0;
volatile unsigned char d1,d2,d3,d4,dot=0;

ISR(TIMER0_OVF_vect)
{TCNT0=0;
    count++;
	if(count==488)
	{
     d1++;
	if(d1>9)
	{d1=0;d2++;
	}
	if(d2>5)
	{d2=0;d3++;
	}
	if(d3>9)
	{d3=0;d4++;
	}
	if(d4>5)
	{d4=0;
	}
 	 count=0;
	}
 


 
}


int main(void)
{int i;
  DDRB=0x0F;
 DDRA=0xFF;
 PORTB=0XFF;
 PORTA=0x00;
 TCCR0=(1<<CS00)|(1<<CS01);//64
 TCNT0=0;
 TIMSK=(1<<TOIE0);
 
 d1=0;
 d2=0;
 d3=0;
 d4=0;
 sei();
 
   while(1)
   {
     PORTB=0X01;
	 PORTA=segdisp[d1];
	 _delay_ms(1);
	 
     PORTB=0X02;
	 PORTA=segdisp[d2];
	 _delay_ms(1);
	 
     PORTB=0X04;
	 PORTA=segdisp[d3];
	 _delay_ms(1);
	 
	 
     PORTB=0X08;
	 PORTA=segdisp[d4];
	 _delay_ms(1);
	 
	 
	
	
   }return 0;

}