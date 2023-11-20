#include<avr/io.h>
#include<util/delay.h>
void pwm (int time);
int main (void)
{
 DDRA=0XF0;
 PORTA=0X00;
 DDRD=0X00;
 PORTD=0XF0;
 while(1)
 {
  if(!(PIND&0X01))
  {
   PORTA=0XF0;
 
  }
 if(!(PIND&0X02))
 {
  PORTA=0XF0;
  pwm(50);
  PORTA=0X00;
  pwm(50);
 }
 if(!(PIND&0X04))
 {
  PORTA=0XF0;
  pwm(30);
  PORTA=0X00;
  pwm(70);
 
 }
 }
}
void pwm(int time)
{int i;
 for(i=0;i<time;i++)
 {
  _delay_us(100);
 }
}