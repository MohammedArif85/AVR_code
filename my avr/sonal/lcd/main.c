#include<avr/io.h>
#include<util/delay.h>
#include "lcd.h"

int main(void)
{ int s,m;
  lcd_init();
  lcd_cmd(0x01);
  while(1)
  {
 for(m=0;m<=59;m++)
 {
 for( s=1;s<=59;s++)
 {
 lcd_cmd(0x82);
  itoa(m);
  lcd_data(':');
  itoa(s);
 _delay_ms(1000);

}
}
}
}
