#include<avr/io.h>
#include<util/delay.h>
#include "uart.h"
#include "lcd.h"

int main(void)

{
 unsigned char t;
 lcd_init();
 lcd_cmd(0x01);
 lcd_cmd(0x80);
 USART_Init();
 DDRA=0xFF;
 PORTA=0X00;
 rsstr("welcome to rs232\r\n");
while(1)
{
 t=rsreadchar();
 PORTA=(t<<4);
 
 
 }
 return 0;

}
