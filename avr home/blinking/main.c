#include<avr/io.h>
#include<util/delay.h>








int main()
{DDRD=0x00;
 PORTD=0xFF;
 DDRA=0b11110000;//1-output 0-input
 PORTA=0b00000000;
 while(1)
 {
   if((PIND&0x01)==0)
   {PORTA=0b10000000;
   _delay_ms(50);
   PORTA=0b01000000;
   _delay_ms(50);
   PORTA=0b00100000;
   _delay_ms(50);
   PORTA=0b00010000;
   _delay_ms(50);
   }
else if((PIND&0x02)==0)
   {PORTA=0x10;
   _delay_ms(50);
   PORTA=0x20;
   _delay_ms(50);
   PORTA=0x40;
   _delay_ms(50);
   PORTA=0x80;
   _delay_ms(50);
   }
   else
   {
    PORTA=0x00;
   }





 }
 return 0;
}