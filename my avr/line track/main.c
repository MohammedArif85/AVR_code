  #include<avr/io.h>
   #include<util/delay.h>
   #include<compat/deprecated.h>
   #define left 0x40
   #define xleft 0x60
   #define right 0x10
   #define xright 0x90
   #define forw 0x50
/*
 voilet -left -PA0
 white -center -PA2
 Grey -right -PA3
 00001101
*/
 
   int main (void)
   
   {unsigned char c;
    DDRC=0xFF;
    DDRA=0xF0;
	DDRD=0x00;
	PORTD=0xFF;
	PORTC=0x00;
	
	while(PIND&0x01);
	while(1)
	{
	 c=PINA&0x0D;
	 if(c==0x09)//center detect
     {
      PORTC=forw;
     } 
	 else if(c==0x01)//slight right
	 {
	  PORTC=right;
       _delay_us(70);
	  PORTC=0x00;
	  _delay_us(30);
	 
	  
	 }
	 else if(c==0x08)
	 {
	  PORTC=left;
	    _delay_us(70);
	  PORTC=0x00;
	  _delay_us(30);
	 
	 }
	 else if(c==0x05)
	 {
	  PORTC=xright;
	  _delay_us(10);
	  PORTC=0x00;
	  _delay_us(10);
	 }
	 else if(c==0x0c)
	 {
	  PORTC=xleft;
	  _delay_us(10);
	  PORTC=0x00;
	  _delay_us(10);
	 }
	 else if(c==0x00)
	 {
	  PORTC=forw;
	   _delay_us(10);
	  PORTC=0x00;
	  _delay_us(10);
	 }
	 else
	 {
	 PORTC=0x00;
	 }
	}
	 return 0;  
	}