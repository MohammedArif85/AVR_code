/*
===================================================================
PROGRAM TO PRINT A STRING ON COMPUTER SCREEN
===================================================================
*/

#include<avr/io.h>
#include<avr/interrupt.h>
#include <avr/sleep.h>
#include "uart.h"
#include<util/delay.h>
#include<compat/deprecated.h>
#include "lcd.h"


volatile unsigned char temp,cnt=0,str;
void readsms()
{rsstr("at");
 rswritechar(0x0d);
 _delay_ms(1000);
 rsstr("at+cmgf=1");
 rswritechar(0x0d);
 _delay_ms(1000);
rsstr("at+cnmi=1,2,0,0,0");
 rswritechar(0x0d);
 _delay_ms(1000); 
}
void sendsms()
{
 rsstr("at");
 rswritechar(0x0d);
 _delay_ms(1000);
 rsstr("at+cmgf=1");
 rswritechar(0x0d);
 _delay_ms(1000);
 rsstr("at+cmgs=");
 rswritechar(0x22);//"
 rsstr("9926366672");
 rswritechar(0x22);//"
 rswritechar(0x0d);
 rsstr("Intruder alert ..GSM Watchdog");
 rswritechar(0x1A);
 _delay_ms(2000);

}
ISR(USART_RXC_vect)
{

	//define temp value for storing received byte

   //Store data to temp
  //readsms();
	temp=UDR;
	if(temp=='a'&& cnt==0)
	cnt++;
    else if (temp=='s'&& cnt==1)
	cnt++;
	else if (temp=='k'&& cnt==2)
	cnt++;
	else if (cnt==3)
	{
	str=temp;
	cnt=0;
	}
	else
	cnt=0;


}


int main(void)
{
	USART_INIT();
	DDRA=0xFF;
	PORTA=0x00;
	sei();
	lcd_init();
	lcd_cmd(0x0c);
	lcd_cmd(0x01);
	lcd_puts("hello dear..");
	_delay_ms(1000);
	lcd_cmd(0x01);
	readsms();
	
    while(1)
	{
	lcd_cmd(0x81);
    lcd_data(temp);
    lcd_data(str);
	
	 switch(str)
     {
      case '1':sbi(PORTA,4);break;
      case '2':cbi(PORTA,4);break;
      case '3':sbi(PORTA,5);break;
      case '4':cbi(PORTA,5);break;
	  
      }	
		
	}
	return 0;
}





