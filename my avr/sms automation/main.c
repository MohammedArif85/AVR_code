/*
===================================================================
PROGRAM TO PRINT A STRING ON COMPUTER SCREEN
===================================================================
*/

#include<avr/io.h>
#include<avr/interrupt.h>
#include <avr/sleep.h>
#include "uart.h"
#include<util/delay.h>
#include<compat/deprecated.h>



volatile unsigned char temp,cnt=0,str;
void readsms()
{rsstr("at");
 rswritechar(0x0d);
 _delay_ms(2000);
 rsstr("at+cmgf=1");
 rswritechar(0x0d);
 _delay_ms(2000);
rsstr("at+cnmi=2,2,0,0,0");
 rswritechar(0x0d);
 _delay_ms(2000); 
rsstr("at+cnmi=2,2,0,0,0");
 rswritechar(0x0d);
 _delay_ms(2000); 
}
void sendsms()
{
 rsstr("at\r\n");
 //rswritechar(0x0d);
 _delay_ms(1000);
 rsstr("at+cmgf=1\r\n");
 //rswritechar(0x0d);
 _delay_ms(1000);
 rsstr("at+cmgs=");
 _delay_ms(1000);
 rswritechar(0x22);
 rsstr("9826541718");
 rswritechar(0x22);
 _delay_ms(1000);
 rswritechar('\r');
 _delay_ms(1000);
 rswritechar('\n');
 rsstr("Intruder alert ..GSM Watchdog");
 rswritechar(0x1A);
 _delay_ms(2000);

}


int main(void)
{
	USART_INIT();
	DDRA=0xFF;
	PORTA=0x00;
	DDRC=0xFF;
	PORTC=0x00;
	//sei();
	PORTA=0xFF;
	_delay_ms(4000);
	_delay_ms(4000);
    _delay_ms(4000);
	_delay_ms(4000);
		
	PORTA=0x00;
	//readsms();
	sendsms();
    while(1)
	{
	
	
	
		
	}
	return 0;
}





