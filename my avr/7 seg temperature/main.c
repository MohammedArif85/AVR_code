  #include<avr/io.h>
   #include<util/delay.h>
   #include <avr/interrupt.h>
   #include<stdio.h>
   
   
    volatile char ch[8];
    volatile unsigned char d1[4];
   void convert()
   {
    int i=0,j=0;
    while(ch[i]!=0)
    {
     if((ch[i]!='.')&&(j<4))
	  {
	   d1[j]=ch[i]-0x30;
	   j++;
	  }
     i++;
	}    
   }
   //a b g f e d c .
//1 1 
volatile unsigned char segdisp[]=
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
volatile unsigned char count=0,temp=0;
ISR(TIMER0_OVF_vect)
{   temp=d1[3-count];
    PORTB=(1<<count);
	if(count!=2)
	{PORTD=segdisp[temp];}
	else
	{PORTD=segdisp[temp]|0x80;
	 
	}
	count++;
	if(count>3)
    count=0; 
}

   int main (void)
   {  float val;
      DDRD=0xFF;
      PORTD=0x00;
      DDRB=0xFF;
      PORTB=0x00; 
	   TCNT0=0;
 TCCR0=(1<<CS00)|(1<<CS01);
 TIMSK=(1<<TOIE0);
 sei();
	 while(1)
	 {
	  ADMUX=(1<<REFS0);
	  ADCSRA=(1<<ADEN)|(1<<ADSC)|(1<<ADPS0);
	  loop_until_bit_is_set(ADCSRA,ADIF);
       val= (ADCW*0.00488);
	   val=val*100;
	   val=val-273;
	   sprintf(ch,"%2.1f",val);
	   convert();
	 }
   } 