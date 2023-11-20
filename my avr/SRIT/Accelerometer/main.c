   #include<avr/io.h>
   #include<util/delay.h>
   #include "lcd.h"
    void adc(unsigned char channel)
	 
	{  int y;
	   ADMUX=(1<<REFS0)|channel;
	   ADCSRA=(1<<ADEN)|(1<<ADPS0);
	   ADCSRA|=(1<<ADSC);
	   loop_until_bit_is_set(ADCSRA,ADIF);
       y=ADCW;
	   itoa(y);
	}   
	
	
	 
   int main (void)
   { 
     DDRA=0X00;
     PORTA=0XFF;
	 lcd_init();
	 lcd_cmd(0x01); 
	 lcd_puts("Accellerometer");
	 while(1)
 	 {
	  lcd_cmd (0xc2);
	  adc(0);
	  lcd_data(' ');
	  adc(1);
	  lcd_data(' ');
	  adc(2);
	
	 }return 0;  
	}
	 
	