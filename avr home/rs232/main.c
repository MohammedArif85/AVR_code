/*
===================================================================
PROGRAM TO PRINT A STRING ON COMPUTER SCREEN
===================================================================
*/

#include<avr/io.h>
#include<util/delay.h>
#include <compat/deprecated.h>
#include "uart.h"
#include "lcd.h"


int main(void)
{
	unsigned char t;
	USART_INIT();
	lcd_init();
	lcd_cmd(0x0c);
	lcd_cmd(0x01);
	while(1)
	{
		t=rsreadchar();
		if(t==27)
		lcd_cmd(0x01);
		else if(t==0x0d)
		lcd_cmd(0xc0);
		else
		lcd_data(t);
	}
	return 0;
}