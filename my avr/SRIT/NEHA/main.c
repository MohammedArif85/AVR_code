#include<avr/io.h>
#include<util/delay.h>
void pwm (int time);
int main (void)
{
 DDRA=0xF0;
 PORTA=0X00;
 DDRD=0x00;
 PORTD=0XF0;
 
 
 while(1)
 {
 if (!(PIND&0X04))
 {PORTA=0XF0;
 pwm(30);
 PORTA=0X00;
 pwm(50);
 }
 }return 0;
 



}
void pwm(int time)
{
int i;
for(i=0;i<time;i++);
 
 _delay_
}
