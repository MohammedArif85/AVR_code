  #include<avr/io.h>
   #include<util/delay.h>
   #include<compat/deprecated.h>
  int main (void)
   
   {unsigned char c;
    DDRC=0xFF;
    DDRA=0xF0;
	PORTA=0x0F;
	PORTC=0x00;
	
	
	while(1)
	{
	 c=PINA&0x0D;
	 if(c==0x09)// forward
	 PORTC=0x50;
	 else if (c==0x01)//simple right
	 PORTC=0x10;
	 else if (c==0x08)//simple left
	 PORTC=0x40;
	 else if (c==0x05)//extreme right
	 {PORTC=0x90;
	  _delay_ms(10);
      PORTC=0x00;    
     _delay_ms(10);
	 } 
	 else if (c==0x0C)//extreme left
	 {
	 PORTC=0x60;
	 _delay_ms(10);
	 PORTC=0x00;
	 _delay_ms(10);
	 }
	 else if (c==0x00)
	 PORTC=0x50;
	  
	 else
	 PORTC=0x00;
	 
	}
	return 0;
	}
	