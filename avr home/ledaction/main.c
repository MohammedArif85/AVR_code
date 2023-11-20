#include<avr/io.h>
#include<util/delay.h>



void pattern1(unsigned char d)
{
 unsigned char c;
 for(c=0;c<4;c++)
 {
 PORTA=0x80>>c;
 _delay_ms(d);
 }
}
void pattern2(unsigned char d)
{
 unsigned char c;
 for(c=0;c<4;c++)
 {
 PORTA=~(0x10<<c);
 _delay_ms(d);
 }
}

void main()
{
  DDRA=0xF0;
  DDRD=0x00;
  PORTD=0x0F;
  while(1)
  { 
  
   while((PIND&0x01)!=0)
   {
   pattern1(500);
   }
   while((PIND&0x01)!=1);
   
   while((PIND&0x01)!=0)
   {
   pattern2(500);
   }
   while((PIND&0x01)!=1);
  
 
   }

}