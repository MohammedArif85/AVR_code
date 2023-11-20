#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

typedef union { 
	uint16_t raw;
	struct {
		unsigned cmd    : 6;	// LSB
		unsigned addr   : 5;
		unsigned toggle : 1;
		unsigned start  : 2;
		unsigned        : 2;	// MSB
	};
} rc5data;

static volatile int flag = 0;
static volatile rc5data data;

void init_int0(void)
{
	// Interrupt on INT0 pin going low
	MCUCR = (1 << ISC01);

	// Turn on INT0!
	GICR |= (1 << INT0);
}

ISR (INT0_vect)
{	
	int pos = 0;

	if (!flag) {
		data.raw = 0;

		// triggers on falling edge of start bit 1.
		_delay_ms(0.2);

		while (pos < 14) {
			data.raw = (data.raw << 1) | ((~PIND & 0x04) >> 2);
			_delay_ms(1.8);
			pos++;
		}

		flag = 1;
	}
}

int main()
{   unsigned char bittu;
	DDRA=0XFF;
	PORTA=0X00;
	
	
	init_int0();
	sei(); 		// Enable interrupts

	while(1) {	

		while(!flag);
		if(data.addr==0x00)
		{
		 if((data.cmd==0x01) &&(bittu!=data.toggle))
		 {
		  PORTA=~PORTA;
		 }
		
		bittu=data.toggle;
	
		// Do with data from the remote control here what you like.
		}
		
		flag = 0;
		
	}

	return 0;
}
