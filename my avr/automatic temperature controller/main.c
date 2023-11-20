  #include<avr/io.h>
   #include<util/delay.h>
   #include<avr/interrupt.h>
   #include <compat/deprecated.h>
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
      unsigned int p;  
      DDRD=0xFF;
      PORTD=0x00;
      DDRB=0xFF;
	  DDRA=0x00;
	  DDRC=0xFF;
	  PORTC=0x00;
      PORTB=0x00; 
	   TCNT0=0;
 TCCR0=(1<<CS00)|(1<<CS01);
 TIMSK=(1<<TOIE0);
 sei();
	 while(1)
	 {
	  ADMUX=0x40;
	  ADCSRA=(1<<ADEN)|(1<<ADSC)|(1<<ADPS0);
	  loop_until_bit_is_set(ADCSRA,ADIF);
	   p=ADCW;
       val= (ADCW*0.00488);
	   val=val*100;
	 //  val=val-273;
	   sprintf(ch,"%2.1f",val);
	   convert();
	   if(p>100)
	   sbi(PORTC,0);
	   else
	   cbi(PORTC,0);
	  /*d1[0]=1;
	  d1[1]=2;
	  d1[2]=3;
	  d1[3]=4;
	  */
	 }
   } 