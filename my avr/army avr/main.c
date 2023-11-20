  #include<avr/io.h>
   #include<util/delay.h>
   #include<avr/interrupt.h>
  
    volatile unsigned char d1[4];
  
   //a b g f e d c .
//1 1 
static unsigned char segdisp[]=
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


volatile unsigned char count=0,temp=0,turnon;



ISR(TIMER0_OVF_vect)
{   
 

    temp=d1[count];
    PORTB=(1<<count);
	PORTD=segdisp[temp];
	count++;
	if(count>3)
    count=0;
 
 if(turnon)
 {
   
   d1[0]++;
   if(d1[0]>9)
   {
    d1[0]=0;
	d1[1]++;
   }
  if(d1[1]>9)
   {
    d1[1]=0;
	d1[2]++;
   }   
   if(d1[2]>9)
   {
    d1[2]=0;
	d1[3]++;
   }
 if(d1[3]>9)
   {
    d1[3]=0;
	}
 
	
  }
  TCNT0=0x06;
   
}

   int main (void)
   { 
      
	 
	  turnon=0;
      DDRD=0xFF;
      PORTD=0x00;
      DDRB=0xFF;
	  DDRA=0x00;
	  PORTA=0xFF;
	  
      PORTB=0x00; 
	   
	   TCNT0=0x06;
	   TCCR0=(1<<CS00)|(1<<CS01);
	   TIMSK=(1<<TOIE0);
       sei();
 
 
	 while(1)
	 {
		 if(!(PINA&0x01))
		 {
		  turnon=1;
		 }
		 
		 else if(!(PINA&0x08))
		 {
		  turnon=0;
		 }
	  
   } 
   
}