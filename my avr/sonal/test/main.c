 #include<avr/io.h>
   #include<util/delay.h>
   #include<compat/deprecated.h>
    //volatile unsigned char sense;
	volatile unsigned int readc;
    int eyeread(unsigned char channel)
	 
	{  
	   ADMUX=(1<<REFS0)|channel;
	   ADCSRA=(1<<ADEN)|(1<<ADPS0);
	   ADCSRA|=(1<<ADSC);
	   loop_until_bit_is_set(ADCSRA,ADIF);
	   _delay_ms(10);
       return ADCW;
	}   
	 
	 
	 
	 void detectth(unsigned char ch, unsigned int *tune)
     {   
      while((PIND&0x01))
	  {
	    ADMUX=(1<<REFS0)|ch;
	   ADCSRA=(1<<ADEN)|(1<<ADPS0);
	   ADCSRA|=(1<<ADSC);
	   loop_until_bit_is_set(ADCSRA,ADIF);
	   readc=ADCW;
	   _delay_ms(10);
	   ADMUX=(1<<REFS0)|0x03;
	   ADCSRA=(1<<ADEN)|(1<<ADPS0);
	   ADCSRA|=(1<<ADSC);
	   loop_until_bit_is_set(ADCSRA,ADIF);
	   _delay_ms(10);
	   *tune=ADCW;
	   if(*tune<readc)
	   {
	    sbi(PORTA,4);
	   }
	   else
	   {cbi(PORTA,4);
	   }
	  }
	  
     } 
   int main (void)
   
   { unsigned static int tune;
     DDRA=0XF0;
	 PORTA=0X0F;
     DDRD=0X00;
	 PORTD=0XFF;
     detectth(0,&tune);
	 //sense=0x00;
	 PORTA=0xFF;
	 _delay_ms(1000);              	 
	 PORTA=0x0F;
	 _delay_ms(1000);
	 
	 while(1){
	 readc=eyeread(0);
	  if(tune<readc)
	   {
	    sbi(PORTA,4);
	   }
	   else
	   {cbi(PORTA,4);
	   }	  
	 }
	 return 0;  
	}