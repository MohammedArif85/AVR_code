#include<avr/io.h>
#include<util/delay.h>
#include<sfr_defs.h>
void pwm(int time)
{
for(i=1;i<=time;i++)
_delay_ms(10);
}

int main(void)
{
DDRA=0xF0;
PORTA=0x00;
while(1)
{
PORTA=0xF0;
pwm(50);
PORTA=0x00;
pwm(50);
} 
} 