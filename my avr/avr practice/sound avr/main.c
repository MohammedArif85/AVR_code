
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"


#define C6  661
#define D6  588
#define E6  524
#define F6  495
#define G6  441
#define A6  393
#define B6  350
#define C7  330

#define PAUSE 80






void play_note(unsigned int note,unsigned int duration)
{
   // Reset the 16 bit Counter
   TCNT1H = 0;
   TCNT1L = 0;

   // Set the Counter TOP
   ICR1H = ( note >> 8 ) & 0x00FF;
   ICR1L = note;

   // Turn on the Prescaler
   TCCR1B |= (1<<CS11);
   _delay_ms(duration); 

   // Turn off the Prescaler
   TCCR1B &= ~(1<<CS11);
   _delay_ms(PAUSE);
}

int main(void)
{
   unsigned char repeat,PlayStatus;
   int iCount;

   DDRB = 0xFE;                  // Set PB0 as input others as Output
   PORTB = 0x00;
   DDRC = 0x00;                  // Set PORTC as Input
   PORTC = 0xFF;
   DDRD = 0xFF;                  // Set PORTD as Output
   PORTD = 0x00;

   // Initial LCD using 4 bits data interface
   lcd_init();

   // Set LCD Display: D=1; Display On, C=0; Cursor Off, B=0; Blinking Off
   lcd_cmd(0x01);lcd_cmd(0x0c);  // Display On, Cursor Off

   // Initial PWM (using Timer/Counter2)
   TCCR2A=0b10000011;            // Fast PWM Mode, Clear on OCRA
   TCCR2B=0b00000100;            // Used fclk/64 prescaller
   OCR2A=0;                      // Initial the OC2A (PB3) Out to 0  

   // Set ADMUX Channel for Volume
   ADMUX=0x00;

   // Initial the ADC Circuit
   ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1);

   // Free running Mode
   ADCSRB = 0x00;

   // Disable digital input on ADC0
   DIDR0 = 0x01; 

   // Set the Timer/Counter Control Register
   TCCR1A = (1<<COM1A1)|(1<<COM1A0); // Set OC1A when upcounting, clear when downcounting
   TCCR1B = (1<<WGM13);              // Phase/Freq-correct PWM, top value = ICR1

   repeat=0;
   PlayStatus=0;                 // Initial Play Status

   for(;;) {

 	   // Start conversion by setting ADSC on ADCSRA Register
	   ADCSRA |= (1<<ADSC);

	   // wait until convertion complete ADSC=0 -> Complete
           while (ADCSRA & (1<<ADSC));

	   // Get the 8 bit ADC Result
	   OCR1AH=0;
	   OCR1AL=ADCL;

 	   // Display Clear, Home
          lcd_cmd(0x01);lcd_cmd(0x0c);	  

	   lcd_puts("Twin"); play_note(C6,200);
	   lcd_puts("kle"); play_note(C6,200);
	   lcd_puts(" Twin"); play_note(G6,200);
	   lcd_puts("kle"); play_note(G6,200);
	   lcd_cmd(0xc0);
	   lcd_puts("Lit"); play_note(A6,200);
	   lcd_puts("tle"); play_note(A6,200);
	   lcd_puts(" Star"); play_note(G6,500);

           // Display Clear, Home
          lcd_cmd(0x01);lcd_cmd(0x0c);

	   lcd_puts("How"); play_note(F6,200);
	   lcd_puts(" I"); play_note(F6,200);
	   lcd_puts(" Won"); play_note(E6,200);
	   lcd_puts("der"); play_note(E6,200);
	   lcd_cmd(0xc0);
	   lcd_puts("What"); play_note(D6,200);
	   lcd_puts(" You"); play_note(D6,200);

	   if (!repeat) {
	     LCD_puts(" Are!"); play_note(C6,500);
           } else {
	     LCD_puts(" Are!"); play_note(C6,1000);
           }

	   if (!repeat) {
	     // Display Clear
           lcd_cmd(0x01);lcd_cmd(0x0c);

	     lcd_puts("Up"); play_note(G6,200);
	     lcd_puts(" A"); play_note(G6,200);
	     lcd_puts("bove"); play_note(F6,200);
	     lcd_puts(" The"); play_note(F6,200);
	     lcd_cmd(0xc0);
	     lcd_puts("World"); play_note(E6,200);
	     lcd_puts(" So"); play_note(E6,200);
	     lcd_puts(" High"); play_note(D6,500); 

	     // Display Clear
           lcd_cmd(0x01);lcd_cmd(0x0c);

	     lcd_puts("Like"); play_note(G6,200);
	     lcd_puts(" A"); play_note(G6,200);
	     lcd_puts(" Dia"); play_note(F6,200);
	     lcd_puts("mond"); play_note(F6,200);
	     lcd_cmd(0xc0);
	     lcd_puts("In"); play_note(E6,200);
	     lcd_puts(" The"); play_note(E6,200);
	     lcd_puts(" Sky"); play_note(D6,500);
	   }

	   if(repeat) {
  	     _delay_ms(1000);
	     repeat=0;

	     lcd_cmd(0x01);lcd_cmd(0x0c);
	     LCD_puts("Twinkle Twinkle");
	     lcd_cmd(0x01);lcd_cmd(0x0c);
	     LCD_puts("Sound by AVR PWM");
	     play_note(C7,100); play_note(C7,1500); // Tada
	     _delay_ms(5000);
                
             // Switch Off the Backlight
	     for (iCount=255;iCount > 0;iCount--) {
	       OCR2A=iCount;			           // Decrease OCR2A
	       _delay_ms(3);
	     }

           } else {
	     repeat=1;
	   }
	   
	 
  }
  return 0;	                  // Standard Return Code
}