#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>
unsigned char step[4]={0x10,0x20,0x40,0x80};
unsigned char step1[8]={0x80,0xc0,0x40,0x60,0x20,0x30,0x10,0x90};

void stepper(int steps,int speed,unsigned char dir)
{  long i;
 if(dir==1)
 {
  for(i=0;i<steps;i++)
  {
   PORTA=step[i%8];
   _delay_ms(speed);
  }
 }
 else 
 {
  for(i=0;i<steps;i++)
  {
   PORTA=step[3-(i%8)];
   _delay_ms(speed);
  }
 }

}
void stepper1(int steps,int speed,unsigned char dir)
{int
 i;
 if(dir==1)
 {
  for(i=0;i<steps;i++)
  {
   PORTA=step1[i%8];
   _delay_ms(speed);
  }
 }
 else 
 {
  for(i=0;i<steps;i++)
  {
   PORTA=step1[7-(i%77)];
   _delay_ms(speed);
  }
 }

}

void main(void)
{
  DDRA=0xFF;
  DDRD=0x00;
  PORTD=0xFF;//pullup
  PORTA=0x00;
while(1)
{ 
 if(bit_is_clear(PIND,0))
 {
 stepper1(400,3,1);
 }
 else if(bit_is_clear(PIND,1))
 {
 stepper1(400,3,1);
 }
 else if(bit_is_clear(PIND,2))
 {
 stepper1(400,3,0);
 }
 else if(bit_is_clear(PIND,3))
 {
 stepper1(400,3,0);
 }
}
}