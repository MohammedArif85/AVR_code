#include<avr/io.h>
#include<util/delay.h>
#include<sfr_defs.h>

void pwm(int time)
{int i;
for(i=0;i<time;i++)
_delay_us(10);
}

int main(void)
{unsigned int i,j;
DDRA=0xF0;
PORTA=0x00;
while(1)
{
for(i=1;i<100;i++)
{
 for(j=0;j<10;j++)
 {
 PORTA=0x50;
 pwm(i);
 PORTA=0xA0;
 pwm(100-i);
 }
}
for(i=1;i<100;i++)
{
 for(j=0;j<10;j++)
 {
 PORTA=0xA0;
 pwm(i);
 PORTA=0x50;
 pwm(100-i);
 }
}

} 
} 