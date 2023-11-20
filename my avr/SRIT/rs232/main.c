/*
===================================================================
PROGRAM TO PRINT A STRING ON COMPUTER SCREEN
===================================================================
*/

#include<avr/io.h>
#include "uart.h"
#include<util/delay.h>
#include "lcd.h"

int main(void)
{
	unsigned char t;
	USART_INIT();
	DDRA=0xFF;
	lcd_init();
	lcd_cmd(0X01);
	rsstr("HI... ITZ JATIN\r\n");
	while(1)
	{
		t=rsreadchar();
		rswritechar(t);
		lcd_cmd(0XC2);
		lcd_data(t);
		t=t-0x30;
		PORTA=t<<4;
	}
	return 0;
}