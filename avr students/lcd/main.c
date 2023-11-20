#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>
#include "lcd.h"





void main()
{int i;
 lcd_init();
 lcd_cmd(0x0c);//display on cursor off
 lcd_cmd(0x01);
 lcd_puts("Hello world ");
 lcd_cmd(0xc0);
 lcd_puts("LCD DEMO");
 _delay_ms(2000);
 lcd_cmd(0x01);
 while(1)
 {
  for(i=0;i<100;i++)
  {
   lcd_cmd(0xc5);
   itoa(i);
   _delay_ms(500);
  
  }
 }
}