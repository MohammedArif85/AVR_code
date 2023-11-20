#include<avr/io.h>
#include<util/delay.h>

unsigned char seven[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

void main()
{ unsigned char c;
  DDRA=0xFF;
  PORTA=0x00;
  while(1)
  {
   for(c=0;c<10;c++)
   {
    PORTA=seven[c];
	_delay_ms(1000);
    }
  }
  
}  

