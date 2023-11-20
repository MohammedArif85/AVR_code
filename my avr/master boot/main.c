#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>
#include<avr/eeprom.h>
#include<lcd.h>
unsigned int eid EEMEM=500;
 int valt EEMEM=400;
void itoa1(uint16_t num)
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
unsigned int rch=0;
 int lch=0;
DDRD=0xF4;                        //SET DATA DIRECTION REGISTER
DDRC=0x30;
PORTD=0x00;                            //SET 1 for OUTPUT PORT
DDRA=0xFF;                                //SET 0 FOR INPUT PORT
PORTD=0x00;
PORTA=0xFF;
lcd_init();
lcd_cmd(0x01);
lcd_cmd(0x01);


//eeprom_write_word(&eid,1611);
//eeprom_write_word(&valt,2);
rch=eeprom_read_word(&eid);
lch=eeprom_read_word(&valt);
if(lch!=0)
eeprom_write_word(&valt,lch-1);
//itoa1(rch);
//PORTA=rch;
//rch=rch+1;
/*eeprom_write_word(&ech,1611);
_delay_ms(100);*/
if(rch==1611 &&lch!=0)
{
PORTA=0xFF;
_delay_ms(2000);
PORTA=0x00;
}
while(1)
{}
return 0;

}