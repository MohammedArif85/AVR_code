int main(void)
{ DDRA=0x00;
  PORTA=0xFF;
  DDRC=0xFF;
  PORTC=0x00;
  unsigned char c;
  while(1)
  { c=PINA & 0x05;
    if(c==0x00)
	//move forward
	PORTC=0xA0;
	else if(c==0x05)
	//move backward
	{
	 PORTC=0x20;
	 _delay_ms(1000);
	}
	else if(c==0x04)
	//move left
	PORTC=0x40;
	else if(c==0x01)
	//move right
	PORTC=0x20;
	else
	PORTC=0x00;
    
  }
   return 0;



