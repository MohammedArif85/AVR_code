#include<avr/io.h>
#include<util/delay.h>

void stepper (int steps, int dir, int speed);

const unsigned fstep[4]={0x80,0x20,0x40,0x10};

int main (void)
{
 DDRC=0xFF;
 PORTC=0x00;
 DDRD=0x00;
 PORTD=0xFF;
 while(1)
 {
  if(!(PIND & 0x01))
  stepper(100,1,4);
  else if(!(PIND & 0x02))
  stepper(200,0,3);
  else if(!(PIND & 0x04))
  stepper(100,0,2);
  else if(!(PIND & 0x08))
  stepper(200,1,1);
 }

   return 0;

}


void stepper(int steps, int dir, int speed)
{
 int i=0;
  
 for(i=0;i<steps;i++)
 {
   if(dir==1)
   PORTC=fstep[i%4];
   else
   PORTC=fstep[4-(i%4)-1];
   _delay_ms(5*speed);
 }
}