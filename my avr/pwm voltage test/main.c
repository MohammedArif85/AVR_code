#include<avr/io.h>
#include<util/delay.h>
#include<stdio.h>
#include<compat/deprecated.h>
#include "lcd.h"
#include "uart.h"

#include<avr/eeprom.h>
unsigned int eid EEMEM=500;
 int valt EEMEM=400;
 
 
unsigned int adc(unsigned char channel)
{  char c;
   unsigned int val;
   ADMUX=0x60+channel;
   ADCSRA=(1<<ADEN)|_BV(ADPS2)|_BV(ADPS1)|_BV(ADPS0);
   
   val=0;
   c=12;
   MCUCR=0x10;
  while(c)
   { 
    ADCSRA|=(1<<ADSC);
    while(bit_is_clear(ADCSRA,ADIF));
   ADCSRA|=(1<<ADIF);
   val=val+ADCH; 
   _delay_us(100);
   c--;
   }
   val=val/12;
   val=(val<<2)&0x3FC;
   return val;
}

void pwm_init()
{

//TOP=ICR1;

//Output compare OC1A 8 bit non inverted PWM

//Clear OC1A on Compare Match, set OC1A at TOP

//Fast PWM

//ICR1=1000 defines 2Khz PWM

ICR1=1000;

TCCR1A|=(0<<COM1A0)|(1<<COM1A1)|(0<<COM1B0)|(1<<COM1B1)|

(0<<FOC1A)|(0<<FOC1B)|(1<<WGM11)|(0<<WGM10);

TCCR1B|=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|

(0<<CS12)|(1<<CS11)|(0<<CS10);

//start timer with prescaler 8


}
void showread(float t,unsigned int d)
{
 char buffer[20];
 sprintf(buffer,"%f,%d\n\r",t,d);
 rsstr(buffer);
}
int main(void)
{float v;
 unsigned int val,tim,cur;
 char buf[15],bud[15];
 unsigned int rch=0;
 int lch=0;int goga=1;
 tim=0;
DDRA=0x00;
PORTA=0x00;
DDRC=0x00;
PORTC=0xFF;
DDRD=0xFF;
lcd_init();
pwm_init();
USART_INIT();

lcd_cmd(0x0c);
lcd_cmd(0x01);
lcd_puts("   *DIGIMeter*");
_delay_ms(1000);
lcd_cmd(0x01);
lcd_cmd(0x8C);
lcd_puts("mA");
lcd_cmd(0xc9);
lcd_puts("volts");
if((PINC&0x01)==0)
        {
        eeprom_write_word(&eid,1611);
        eeprom_write_word(&valt,30);
        }   
        rch=eeprom_read_word(&eid);
lch=eeprom_read_word(&valt);
if(lch!=0)
eeprom_write_word(&valt,lch-1);
 
if(rch!=1611 ||lch==0)
{
goga=13;
}
rsstr("hello world\r\n");
 while(1)
 { lcd_cmd(0x82); 
   cur=adc(7);
   _delay_ms(500); 
   sprintf(buf,"%4d",cur*goga);
   lcd_puts(buf);
   lcd_cmd(0xc2); 
   val=adc(1);
   _delay_ms(500); 
   v=val*2*(0.00488);
   
   sprintf(bud,"%2.2f",v*goga);
   lcd_puts(bud);
   
   OCR1B=tim;
   if(bit_is_clear(PINC,7))
   {
   _delay_ms(500); 
    while(bit_is_clear(PINC,7));
	if(tim<1000) 
	{
	if(tim>100)
	tim=tim+100;
    else
	tim=tim+10;
	
	}
	showread(v*goga,cur*goga);
   }
   if(bit_is_clear(PINC,6))
   {
   _delay_ms(500); 
    while(bit_is_clear(PINC,6));
	if(tim>=10) 
	{
	if(tim>190)
	tim=tim-100;
	else 
	tim=tim-10;
    }
   showread(v,cur);
   }

 }
} 
 