#include<avr/io.h>
#include<util/delay.h>
#include "lcd.h"



int main(void)
{   int i;
    lcd_init();
    lcd_cmd(0x0c);//display on curser off
	lcd_cmd(0x01);//lcd clear
	lcd_puts("  Counter");
	while(1)
	{
	 for(i=0;i<60;i++)
	 {lcd_cmd(0xc5);//goto second line 6th character
	  itoa(i);
	  _delay_ms(1000);
	 }
	}
	return 0;
}