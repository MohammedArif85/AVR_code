  #include<avr/io.h>
   #include<util/delay.h>
   #include "lcd.h"
   #include<stdio.h>
     float temp(unsigned char channel)
	 
	{  float val; char ch[8];
	   ADMUX=(1<<REFS0)|channel;
	   ADCSRA=(1<<ADEN)|(1<<ADPS0);
	   ADCSRA|=(1<<ADSC);
	   loop_until_bit_is_set(ADCSRA,ADIF);
       val= (ADCW*0.00488);
	   val=val*100;
	   val=val-273;
	   sprintf(ch,"%.2fC",val);
	   lcd_cmd(0xC2);
	   lcd_puts(ch);
	   return val;
	}   
	float setting(unsigned char channel)
	 
	{  float val; 
	   ADMUX=(1<<REFS0)|channel;
	   ADCSRA=(1<<ADEN)|(1<<ADPS0);
	   ADCSRA|=(1<<ADSC);
	   loop_until_bit_is_set(ADCSRA,ADIF);
       val= (ADCW*0.00488);
	   val=val*100;
	   val=val-273;
	   return val;
	}   
	
	 float mode(void)
	 { float a;
	  lcd_cmd(0x01);
	   lcd_cmd(0x80);
	   lcd_puts("SET LIMIT");
	  while((PIND&0X01))
       a=temp(1); 
	  _delay_ms(1000);
	  
	  return a;
	 }
	 
	 
   int main (void)
   
   {float current,set; 
     DDRD=0X00;
     PORTD=0XFF;
	 DDRA=0XF0;
	 PORTA=0X00;
     lcd_init();
	 lcd_cmd(0x01); 
	 lcd_puts("TEMPERATURE");
	 lcd_cmd(0xC2);
	 lcd_puts("CONTROLLER");
	 _delay_ms(2000);
	  lcd_cmd(0x01);
	   
	 while(1)
	 {
	  if(!(PIND&0X01))
	  {
	   _delay_ms(1);
	  
	   while(!(PIND&0X01));
       set= mode();
	   
	    lcd_cmd(0x01);
	  lcd_cmd(0x82);
      lcd_puts("Current Temp");
	  }
	  else
	  {
	  
	  current=temp(0);
	  set= setting(1);
	   if(current>=set)
	  PORTA=0XF0;
	  else
	  PORTA=0X00;
	
	  }	
	 }return 0;  
	}
	 
	