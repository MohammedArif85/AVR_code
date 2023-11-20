#include<avr/io.h>
#include<util/delay.h>
#include "lcd.h"







int main(void)
{
  lcd_init();//initialise the lcd
  lcd_cmd(0x01);//clear screen
   lcd_puts("AIms Interactive");
   lcd_cmd(0xC0);
   lcd_puts("LCD Demo");
   
   while(1)
   {
     lcd_cmd(0x18);
	 _delay_ms(250);
	
   

    }









}