#include<avr/io.h>
#include<stdlib.h>
#include<util/delay.h>
#include "nokialib.h"


int main(void)
{
int i;
DDRB=0xFF;
PORTB=0x00;
initlcd();
 
lcd_clear();
lcd_drawborder();
cursorxy(2,2);
putstr("  **Aims**");
cursorxy(3,2);
putstr(" Interactive");
cursorxy(4,2);
putstr("   Counter");
_delay_ms(5000);
lcd_clear();
lcd_drawborder();
while(1)
{char buffer[4];
 for(i=0;i<1000;i++)
 {
  itoa(i,buffer,10);
  lcdstr_megaFont(buffer);
  _delay_ms(900);
 } 
}

return 0;
}
