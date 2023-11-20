#include<avr/io.h>
#include<util/delay.h>
void pwm (int time);
int main (void)
{int i,j;
 DDRA=0XF0;
 PORTA=0X00;
 DDRD=0X00;
 PORTD=0XF0;
 while(1)
 {
  
   for(i=1;i<=100;i++)
  {
   for(j=0;j<50;j++)
   {
    PORTA=0XF0;
	pwm(i);
	PORTA=0X00;
	pwm(100-i);
   }
   }
   for(i=1;i<=100;i++)
  {
   for(j=0;j<50;j++)
   {
    PORTA=0X00;
	pwm(i);
	PORTA=0XF0;
	pwm(100-i);
   }
   
   
  }
  
 }return 0;
} 
 void pwm (int time)
{int k;
 for(k=0;k<time;k++)
 {
  _delay_us(10);
 }
}