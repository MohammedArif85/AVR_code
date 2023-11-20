#include<avr/io.h>
#include<util/delay.h>
#include "uart.h"
#include "lcd.h"

int main(void)

{
 unsigned char t;
 lcd_init();
 lcd_cmd(0x01);
 USART_Init();
 DDRB=0xFF;
 PORTB=0X00;
while(1)
{
 t=rsreadchar();
 if(t==0x0d)
 {
 lcd_cmd(0x01);
 }
 
 else
 { 
  lcd_data(t);
 }
}
return 0;

}
