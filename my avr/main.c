#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>
#include<avr/eeprom.h>
#include<lcd.h>
unsigned int ech EEMEM=0;
void itoa(uint16_t num)
{
unsigned char a,b,c,d,e;
a=num%10;
num=num/10;
b=num%10;
num=num/10;
c=num%10;
num=num/10;
d=num%10;
num=num/10;
e=num%10;
lcd_data(0x30+e);

lcd_data(0x30+d);
lcd_data(0x30+c);
lcd_data(0x30+b);
lcd_data(0x30+a);

}

int main(void)
{
DDRD=0xF4;                        //SET DATA DIRECTION REGISTER
DDRC=0x30;
PORTD=0x00;                            //SET 1 for OUTPUT PORT
DDRB=0x00;                                //SET 0 FOR INPUT PORT
PORTD=0x00;
PORTB=0xFF;
lcd_init();
lcd_cmd(0x01);
while(1)
{
lcd_cmd(0x01);
unsigned int rch=eeprom_read_word(&ech);
itoa(rch);
rch=rch+100;
eeprom_write_word(&ech,rch);
_delay_ms(100);
}
return 0;

}