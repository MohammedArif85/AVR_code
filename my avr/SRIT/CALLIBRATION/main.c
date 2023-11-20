  #include<avr/io.h>
   #include<util/delay.h>
   #include "lcd.h"
   #include<stdio.h>
   int main (void)
   { char ch[8];
      float val;
     lcd_init();
	  lcd_cmd(0x01); 
	 while(1)
	 {
	  ADMUX=(1<<REFS0);
	  ADCSRA=(1<<ADEN)|(1<<ADSC)|(1<<ADPS0);
	  loop_until_bit_is_set(ADCSRA,ADIF);
       val= (ADCW*0.00488);
	   val=val*100;
	   val=val-273;
	   sprintf(ch,"%2f C",val);
	   lcd_cmd(0x82);
	   lcd_puts(ch);
	 }
   } 