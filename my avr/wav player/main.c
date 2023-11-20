#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include<compat/deprecated.h>
#include<avr/pgmspace.h>
#include "sounddata.h"

volatile unsigned int sample;
unsigned char lastSample;

 

// This is called at 8000 Hz to load the next sample.
ISR(TIMER1_COMPA_vect) {
    if (sample >= sounddata_length) {
        if (sample == sounddata_length + lastSample) {
            stopPlayback();
        }
        else {
            // Ramp down to zero to reduce the click at the end of playback.
            OCR2 = sounddata_length + lastSample - sample;
        }
    }
    else {
        OCR2 = pgm_read_byte(&sounddata_data[sample]);
    }

    ++sample;
}

void startPlayback()
{
    DDRD=0xFF;
 PORTD=0x00;

    /*set timer counter initial value*/

TCNT2=0x00;

/*Set timer output compare register*/

OCR2=0x80;

/*start timer:

without presscaler

Non inverted mode in OC2 pin;

FAST PWM*/
TCCR2=(1<<FOC2)|(1<<COM21)|(1<<COM20)|(1<<WGM20)|(1<<WGM21)|(1<<CS00);

    // Set initial pulse width to the first sample.
    OCR2 = pgm_read_byte(&sounddata_data[0]);


    // Set up Timer 1 to send a sample every interrupt.

    cli();

    // Set CTC mode (Clear Timer on Compare Match) (p.133)
    // Have to set OCR1A *after*, otherwise it gets reset to 0!
    TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
    TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));

    // No prescaler (p.134)
    TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);

    // Set the compare register (OCR1A).
    // OCR1A is a 16-bit register, so we have to do this with
    // interrupts disabled to be safe.
    OCR1A = F_CPU / SAMPLE_RATE;    // 16e6 / 8000 = 2000

    // Enable interrupt when TCNT1 == OCR1A (p.136)
    TIMSK |= _BV(OCIE1A);

    lastSample = pgm_read_byte(&sounddata_data[sounddata_length-1]);
    sample = 0;
    sei();
}

void stopPlayback()
{
    // Disable playback per-sample interrupt.
    TIMSK &= ~_BV(OCIE1A);

    // Disable the per-sample timer completely.
    TCCR1A &= ~_BV(CS10);

    // Disable the PWM timer.
    TCCR2 &= ~_BV(CS00);

    PORTD=0x00;
}

void setup()
{
   startPlayback();
}
int main(void)
{setup();
 while(1)
 {  
 
  
 }
 return 0;
} 

