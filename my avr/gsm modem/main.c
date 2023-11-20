/*
===================================================================
PROGRAM TO PRINT A STRING ON COMPUTER SCREEN
===================================================================
*/

#include<avr/io.h>
#include "uart.h"
#include<util/delay.h>
#include "lcd.h"
void readsms()
{rsstr("at");
 rswritechar(0x0d);
 _delay_ms(1000);
 rsstr("at+cmgf=1");
 rswritechar(0x0d);
 _delay_ms(1000);
}

void sendsms()
{
 rsstr("at");
 rswritechar(0x0d);
 _delay_ms(1000);
 rsstr("at+cmgf=1");
 rswritechar(0x0d);
 _delay_ms(1000);
 rsstr("at+cmgs=");
 rswritechar(0x22);//"
 rsstr("9826541718");
 rswritechar(0x22);//"
 rswritechar(0x0d);
 rsstr("Alert~! Soil Moisture Low ");
 rswritechar(0x1A);
 _delay_ms(2000);

}
int main(void)
{
	unsigned char t;
	USART_INIT();
	DDRA=0xFF;
	PORTA=0x00;
	DDRD=0x00;
	PORTD=0x0F;
	
	readsms();
	while(1)
	{
	if(!(PIND&0x01))
    {
     while(!(PIND&0x01));
	 sendsms();
	 PORTA=PORTA^0x10;
    }	
	}
	return 0;
}