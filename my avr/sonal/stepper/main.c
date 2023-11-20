#include<avr/io.h>
#include<util/delay.h>
#include<sfr_defs.h>
unsigned char fstep[]={0x80,0x20,0x40,0x10};
void stepper(int steps,char dir,char speed)
{
int i;
for(i=0;i<steps;i++)
{
if(dir==0)
PORTC=fstep[i%4];
else
PORTC=fstep[4-i%4-1];

_delay_ms(2*speed);
}
}

int main(void)
{DDRD=0x00;
PORTD=0xFF;
DDRC=0XFF;
PORTC=0x00;
while(1)
{

if(!(PIND&0x01))
{
_delay_ms(500);
while(!(PIND&0x01));
stepper(100,1,5);
}
else if(!(PIND&0x02))
{
_delay_ms(500);
while(!(PIND&0x02));
stepper(200,0,1);
}
else
{
 PORTC=0x00;
}
}
}