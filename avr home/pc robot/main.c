/*
===================================================================
PROGRAM TO PRINT A STRING ON COMPUTER SCREEN
===================================================================
*/

#include<avr/io.h>
#include "uart.h"
#include<util/delay.h>
#include <compat/deprecated.h>
#define forw 0xA0
#define back 0x50
#define right 0x60
#define left 0x90

int main(void)
{
	unsigned char t;
	USART_INIT();
	DDRA=0xF0;
	DDRA=0x0F;
	DDRC=0xFF;
	rsstr("hello i m robot\r\n");
	while(1)
	{
		t=rsreadchar();
		switch(t)
		{
		 case 'f':PORTC=forw;rsstr("i m going forward\r\n");break;
		 case 'b':PORTC=back;rsstr("i m going bacward\r\n");break;
		 case 'r':PORTC=right;rsstr("i m going right\r\n");break;
		 case 'l':PORTC=left;rsstr("i m going left\r\n");break;
		}
		_delay_ms(10);
		PORTC=0x00;
	}
	return 0;
}