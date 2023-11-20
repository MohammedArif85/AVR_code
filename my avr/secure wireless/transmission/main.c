/*
===================================================================
PROGRAM TO PRINT A STRING ON COMPUTER SCREEN
===================================================================
*/

#include<avr/io.h>
#include "uart.h"
#include<util/delay.h>
#include <compat/deprecated.h>

int main(void)
{
	unsigned char t;
	USART_INIT();
	rsstr("Secure wireless terminal\r\n");
	DDRA=0xFF;
	while(1)
	{
		t=rsreadchar();
		switch(t)
		{
		 case '5':PORTA=0x10;break;
		 case '6':PORTA=0x20;break;
		 case '7':PORTA=0x40;break;
		 case '8':PORTA=0x80;break;
		
		
		}
	}
	return 0;
}