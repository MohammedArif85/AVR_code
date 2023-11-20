#include<avr/io.h>
#include<util/delay.h>
#include<lcd.h>

int main (void)
{
int m=0,s=0;
DDRD=0x00;
PORTD=0xFF;
while(1)
{
if(!(PIND&0x01))
{
_delay_ms(100);
while(!(bit_is_clear(PIND,0))
while(!(PIND&0x02))
{
lcd_cmd(0x82);
itoa(m);
lcd_data(':');
itoa(s);
s++;
if(s>59)
s=0;
m++;
if(m>10)
m=0;

_delay_ms(1000);
}
}
if(bit_is_clear(PIND,3))
{
s=m=0;
lcd_cmd(0x82);
itoa(m);
lcd_data(':');
itoa(s);
}
}
}

