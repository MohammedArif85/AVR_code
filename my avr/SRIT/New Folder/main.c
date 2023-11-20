#include<avr/io.h>
#include<util/delay.h>
#include "lcd.h"

void display(int h,int m, int s);

int main(void)
{ int h=0,m=0,s=0;
  DDRD=0x00;
  PORTD=0xFF;
  lcd_init();//initialise the lcd
  lcd_cmd(0x01);//clear screen
   while(1)
   {
     if(!(PIND & 0x04))
	 {
	 while(PIND & 0x08)
	 {
	  display(h,m,s);
	  _delay_ms(250);
	  s++;
	  if(s>59)
	  {s=0;
	  m++;
	  }
	  if(m>59)
	  {
	  m=0;
	  h++;
	  }
      }
	 }
	 if(!(PIND &0x02)) 
	 {
	  m=0;
	  s=0;
	  h=0;
	  display(h,m,s);
	 }

    }
	return 0;
}


void display (int h, int m, int s)
{
lcd_cmd(0xc2);
itoa (h);
lcd_data(':');
itoa(m);
lcd_data(':');
itoa(s);
}
