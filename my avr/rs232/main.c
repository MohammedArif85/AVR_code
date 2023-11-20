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
	DDRA=0xFF;
	while(1)
	{
		t=rsreadchar();
		switch(t)
		{
		 case '5':PORTA=0xFF;break;
		 case '6':PORTA=0x00;break;
		 case '7':PORTA=0x30;break;
		 case '8':PORTA=0xC0;break;
		
		
		}
	}
	return 0;
}