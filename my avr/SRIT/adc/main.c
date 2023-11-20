  #include<avr/io.h>
   #include<util/delay.h>
   #include "lcd.h"
   int main (void)
   {unsigned int i;
     DDRA=0xF0;
	 lcd_init();
	 lcd_puts("ADC TEST");
	 while(1)
	 {
	  ADMUX=(1<<REFS0);
	  ADCSRA=(1<<ADEN)|(1<<ADSC)|(1<<ADPS0);
	  loop_until_bit_is_set(ADCSRA,ADIF);
	  lcd_cmd (0xc2);
	  i=ADCW;
	  itoa(i);
	  
	  if(i<=7)
	  {
	   PORTA=0XF0;
	   lcd_cmd(0x80);
	  }
	   else if(i<=96)
	   {PORTA=0X70;
	   lcd_cmd(0x80);
	   }
        else if(i<=152) 
		{PORTA=0X30;
	     lcd_cmd(0x80);
	    }
		else if(i<=200) 
		{PORTA=0X10;
	     lcd_cmd(0x80);
	    }
		else
		{
		 PORTA=0X00;
		 lcd_cmd(0x80);
		
		}
	 
   }
}   