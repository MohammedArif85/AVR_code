#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "global.h"		// globals from the AVRlib library
#include "servo.h"              // include RC servo driver library
#include "servoconf.h"
#include "rprintf.h"
#include "timer.h"
/** Global variable, contains the angle of the servo. */
//static uint8_t channel;
static int angle;
static int channel;

void main()
{ DDRC = 0xff;                /* all outputs */
    PORTC = 0x00;

 // this bit is copy and pasted from AVRlib
	// 
	// do some examples
	// initialize RC servo system
	servoInit();
	// setup servo output channel-to-I/Opin mapping
	// format is servoSetChannelIO( CHANNEL#, PORT, PIN );
	servoSetChannelIO(0, _SFR_IO_ADDR(PORTC), PC0);
	servoSetChannelIO(1, _SFR_IO_ADDR(PORTC), PC1);
	servoSetChannelIO(2, _SFR_IO_ADDR(PORTC), PC2);
	servoSetChannelIO(3, _SFR_IO_ADDR(PORTC), PC3);
        servoSetChannelIO(4, _SFR_IO_ADDR(PORTC), PC4);
        servoSetChannelIO(5, _SFR_IO_ADDR(PORTC), PC5);
   sei();
	// set port pins to output
        outb(DDRC, 0x0F);
        sbi(DDRB, PB1);
        sbi(DDRB, PB2);

    channel = 0;		
    angle = 100;

 while(1)
 {
  
  
 }
}