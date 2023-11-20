#include<avr/io.h>
#include<compat/deprecated.h>
#include<util/delay.h>



void main()
{unsigned char c;
 PORTC=0XF0;
 PORTA=0X00;
 PORTA=0X05;
 
 while(1)
 
 {c=PINA&0x05;
  if(c==0x00) 
 { 

   PORTC=0xA0;

 }


  else if(c==0x01)
  { 

    PORTC=0x90;

  }
  else if(c==0x02)
  { 

    PORTC=0x60;

  }
  else if(c==0x05)
  { 

    PORTC=0x50;

  }
  else
  {
  PORTC=0x00;
  }
 }

}