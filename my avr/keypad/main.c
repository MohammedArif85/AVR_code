#include<avr/io.h>
#include<util/delay.h>
#include "lcd.h"
/*
row1  PD.4
row2  PD.5
row3  PD.6
row4  PD.7

col1  PD.0
col2  PD.1
col3  PD.2
col4  PD.3
*/

unsigned char getkey()
{unsigned char i,k=0;
 for(i=0;i<4;i++)
 {
 PORTD=~(0x80>>i); 
 if(!(PIND&0x01))
 {k=k+1;
  while(!(PIND&0x01));//wait untill released
  return k;
 }
 else if(!(PIND&0x02))
 {k=k+2;
  while(!(PIND&0x02));//wait untill released
  return k;
 }
 else if(!(PIND&0x04))
 {k=k+3;
  while(!(PIND&0x04));//wait untill released
  return k;
 }
 else if(!(PIND&0x08))
 {k=k+4;
  while(!(PIND&0x08));//wait untill released
  return k;
 }
 k=k+4;
 }
  return 0;
 
}
int main(void)
{unsigned char key;
 DDRD=0xF0;
 PORTD=0x0F;
 lcd_init();
 lcd_cmd(0x0c);
 lcd_cmd(0x01);//lcd clear
 lcd_puts("Keypad Matrix");     
  while(1)
  {
   while(!(key=getkey()));
   lcd_cmd(0xc4);
   lcd_data('0'+key);
  }
}
