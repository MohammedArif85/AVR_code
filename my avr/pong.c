#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>

#include "font.h"

int pong_mode;

/***************************************\
 * Ports:
 * A0-6: Dot matrix rows
 * D0-4: Dot matrix columns
 * B0:   Player 2 UP
 * B1:   Player 2 DOWN
 * B2:   Player 1 UP
 * B3:   Player 1 DOWN
 * B4:   Player 2 score LED
 * B5:   Player 1 score LED
 \***************************************/

/***************************************\
 * Framebuffer(5x7 dot matrix):
 * 6 x x x x O
 *   x x x x O <- p2_pos = 5
 *   x x x x x
 *   x x x x x
 *   x x x x x
 *   x x x x O
 * 0 x x x x O <- p2_pos = 0
 *   0       4
 \***************************************/

volatile unsigned char framebuffer[5];
volatile unsigned char framebuffer_pos = 0x00;

volatile char ball_x;
volatile char ball_y;
volatile char ball_dx;
volatile char ball_dy;
volatile char p1_pos; /* Position of player 1 paddle */
volatile char p2_pos; /* Position of player 2 paddle */
volatile char ballClock; /* Move ball only every to tick */
volatile char redraw_needed;
volatile char speed_counter; /* increase speed as game goes on */
const char speed_bumper = 10; /* increase speed after this many movement_timer ticks */
volatile char current_speed;
volatile char score; 
const unsigned char d1[] = {0x00, 0x10, 0x20, 0x7f, 0x00}; /* the number 1 */
const unsigned char d2[] = {0x47, 0x49, 0x49, 0x49, 0x31}; /* the number 2 */


#define BV(x) _BV(x)
#define outw(a, b) (a = (b))
#define outp(a, b) (b = (a))
#define inp(a) (a)


/* Draw paddles and ball */
void drawScene()
{
	framebuffer[0] = BV(p1_pos) | BV(p1_pos+1);
	framebuffer[1] = 0;
	framebuffer[2] = 0;
	framebuffer[3] = 0;
	framebuffer[4] = BV(p2_pos) | BV(p2_pos+1);
	framebuffer[(unsigned char)ball_x] |= BV(ball_y);
}

void variable_init(void)
{
	ball_x  = 2;
	ball_y  = 3;
	ball_dx = 1;
	ball_dy = 0;
	p1_pos = 2;
	p2_pos = 2;
	ballClock = 0;
	redraw_needed = 1;
	speed_counter = 0;
        current_speed = 3;
	outw(OCR1A,40000);
}

void ioinit(void)
{
	/* Set port C and B0-3, B6-7, to internal pullup, B4-5 output */
	outp(0x30, DDRB);
	outp(0x00, DDRC);
	outp(0xcf, PORTB);
	outp(0xff, PORTC);

	/* Set port A and D to output */
	outp(0xff, DDRA);
	outp(0xff, DDRD);

	/* Enable 8-bit counter1, running on clk speed */
	outp(BV(CS01), TCCR0);

	/* Set OCR1A as max value and Enable 16-bit counter,
	 * running on clk/8 speed */
	outp(BV(WGM12)|BV(CS11), TCCR1B);
	
	/*  */
	outw(OCR1A,40000);

	/* Enable overlow when counter1 reaches OCR1A */
	/*outp(BV(TOV1), TIFR);*/

	/* Enable overflow interrupt for counter 0 and counter 1*/
	timer_enable_int(BV(TOIE0)|BV(OCIE1A));

	/* Enable interrupts */
	sei();
}

/* Switch leds to indicate who's leading */
void update_score_output(void) {
	if (score == 1) 
	{
		outp(0xef, PORTB); /* turn on led for player 1 */
	}
	else if(score == -1)
	{
		outp(0xdf, PORTB); /* turn on led for player 2 */
	}
	else
	{
		outp(0xcf, PORTB); /* turn off leds */
	}
}

void victory(void)
{
	unsigned char buttons;
	int i;
	int j;
	
	/* turn off counter */
	outp(0x00, TCCR1B);
	
	/* turn on both leds */
	/* outp(0xff, PORTB); */

	/* Display winner */
	if (score == 1) 
	{
		framebuffer[0] = d1[0];
		framebuffer[1] = d1[1];
		framebuffer[2] = d1[2];
		framebuffer[3] = d1[3];
		framebuffer[4] = d1[4];
	}
	else 
	{
		framebuffer[0] = d2[0];
		framebuffer[1] = d2[1];
		framebuffer[2] = d2[2];
		framebuffer[3] = d2[3];
		framebuffer[4] = d2[4];
	}
	sei();

        /* wait a little */
	for (i = 0; i < 5000; i++)
	  for (j = 0; j < 1000; j++) j=j;
	/* wait for buttonpress to start again */
	buttons = ~inp(PINB);
	while( (buttons & 0x0f) == 0x00 ) {
		unsigned char wait_count;
		for (wait_count = 0; wait_count < 0xff; ++wait_count);
		buttons = ~inp(PINB);
	}

	cli();

	/* Reinit */
	variable_init();
	ioinit();
	score = 0;
        speed_counter = 0;
}

/* Movement */
void timer_movement(unsigned char portB)
{
	unsigned char p1_up; /* Player 1  up  button pressed */
	unsigned char p1_dn; /* Player 1 down button pressed */
	unsigned char p2_up; /* Player 2  up  button pressed */
	unsigned char p2_dn; /* Player 2 down button pressed */
	char pos;
	char loose;
	char paddle_bounce;
	int i;
	int j;

	/* Read buttons */
	p1_up = portB & 0x04; /* Pin B2 */
	p1_dn = portB & 0x08; /* Pin B3 */
	p2_up = portB & 0x01; /* Pin B0 */
	p2_dn = portB & 0x02; /* Pin B1 */

	/* Stop paddle movement at wall */
	p1_up = (p1_up && p1_pos < 5);
	p1_dn = (p1_dn && p1_pos > 0);
	p2_up = (p2_up && p2_pos < 5);
	p2_dn = (p2_dn && p2_pos > 0);

	/* Move paddles */
	if (p1_up) p1_pos++;
	if (p1_dn) p1_pos--;
	if (p2_up) p2_pos++;
	if (p2_dn) p2_pos--;

	/* Move ball only every two clock tick */
	if ((ballClock = !ballClock))
	{
		/* Bounce ball on wall */
		if (ball_y == 0) ball_dy = -ball_dy;
		if (ball_y == 6) ball_dy = -ball_dy;

		/* Bounce ball on paddle */
		loose = 0;
		paddle_bounce = 0;
		pos = (ball_x == 1)?p1_pos:p2_pos;
		if (ball_x == 1 || ball_x == 3)
		{
			loose = 1;
			if (ball_y == pos || ball_y == pos+1)
			{
				ball_dx = -ball_dx;
				loose = 0;
				paddle_bounce = 1;
			}
			else if (ball_y == pos-1 && ball_dy == 1)
			{
				ball_dx = -ball_dx;
				ball_dy = -ball_dy;
				loose = 0;
				paddle_bounce = 1;
			}
			else if (ball_y == pos+2 && ball_dy == -1)
			{
				ball_dx = -ball_dx;
				ball_dy = -ball_dy;
				loose = 0;
				paddle_bounce = 1;
			}

		}
		if (loose)
		{
			if (ball_x == 1)  /* player 2 won */
			{
				if (score == -1) /* total victory! */
				{
					victory();
					return;
				}
				else
				{
					score--;
					update_score_output();

					ball_x += ball_dx;
					ball_y += ball_dy;
					redraw_needed = 1;

					outp(0x00, TCCR1B); /* turn off counter */
					sei(); /* Re-enable interrupts */
					/* Wait a little */
					for (i = 0; i < 5000; i++)
						for (j = 0; j < 1000; j++) j=j;
					cli(); /* Disable interrupts again */
					outp(BV(WGM12)|BV(CS11), TCCR1B); /* turn on counter */
				}
			}
			else { /* player 1 won */
				if (score == 1) /* total victory! */
				{
					victory();
					return;
				}
				else
				{
					score++;
					update_score_output();

					ball_x += ball_dx;
					ball_y += ball_dy;
					redraw_needed = 1;

					outp(0x00, TCCR1B); /* turn off counter */
					sei(); /* Re-enable interrupts */
					/* Wait a little */
					for (i = 0; i < 5000; i++)
						for (j = 0; j < 1000; j++) j=j;
					cli(); /* Disable interrupts again */
					outp(BV(WGM12)|BV(CS11), TCCR1B); /* turn on counter */
				}
			}
			variable_init();
			update_score_output();
		}

		/* Change vertical speed of ball according to paddle speed */
		if (paddle_bounce)
		{
			if (ball_x == 1 && p1_up && ball_dy <  1) ball_dy++;
			if (ball_x == 1 && p1_dn && ball_dy > -1) ball_dy--;
			if (ball_x == 3 && p2_up && ball_dy <  1) ball_dy++;
			if (ball_x == 3 && p2_dn && ball_dy > -1) ball_dy--;
		}

		/* Wall bounce after paddle bounce */
		if (ball_y == 0 && ball_dy == -1) ball_dy = -ball_dy;
		if (ball_y == 6 && ball_dy ==  1) ball_dy = -ball_dy;

		/* Move ball */
		ball_x += ball_dx;
		ball_y += ball_dy;
	}

	redraw_needed = 1;
}

/* Framebuffer output to led matrix */
SIGNAL(SIG_OVERFLOW0)
{
	if (++framebuffer_pos > 4) framebuffer_pos = 0;

	/* Change output to dot matrix */
	outp(0xff, PORTA); /* Avoid ghosting */
	outp(BV(framebuffer_pos), PORTD);
	outp(~framebuffer[framebuffer_pos], PORTA);

	if (!pong_mode) return;
	/* Redraw scene during "vertical retrace" */
	if (framebuffer_pos == 4 && redraw_needed)
	{
		drawScene();
		redraw_needed = 0;
	}
}

/* Movement */
SIGNAL(SIG_OUTPUT_COMPARE1A)
{
	if (!pong_mode) return;
	if (++speed_counter == speed_bumper) {
		speed_counter = 0;
		outw(OCR1A, (40000 / ++current_speed) * 3);
		if (current_speed > 100) current_speed = 100;
	}

	timer_movement(~inp(PINB));
}

int main_pong(void)
{
	score = 0;
	variable_init();
	ioinit();
	for(;;);
}

int main_font(void)
{
	unsigned char portB;
	unsigned char nextPressed;
	unsigned char prevPressed;
	int cf; /* Current font */

	nextPressed = 0;
	prevPressed = 0;
	cf = 0;
	framebuffer[0] = font[cf][0];
	framebuffer[1] = font[cf][1];
	framebuffer[2] = font[cf][2];
	framebuffer[3] = font[cf][3];
	framebuffer[4] = font[cf][4];

	ioinit();

	for(;;)
	{
		portB = ~inp(PINB);

		/* Prev button */
		if (portB & 0x01)
		{
			if (!prevPressed)
			{
				cf--;
				if (cf < 0) cf = 0;
				framebuffer[0] = font[cf][0];
				framebuffer[1] = font[cf][1];
				framebuffer[2] = font[cf][2];
				framebuffer[3] = font[cf][3];
				framebuffer[4] = font[cf][4];
			}
			prevPressed = 1;
		}
		else
			prevPressed = 0;

		/* Next button */
		if (portB & 0x02)
		{
			if (!nextPressed)
			{
				cf++;
				if (cf >= font_count) cf = font_count - 1;
				framebuffer[0] = font[cf][0];
				framebuffer[1] = font[cf][1];
				framebuffer[2] = font[cf][2];
				framebuffer[3] = font[cf][3];
				framebuffer[4] = font[cf][4];
			}
			nextPressed = 1;
		}
		else
			nextPressed = 0;
	}
}

int main(void)
{
	unsigned char portB;
        
        framebuffer[0] = 0;
        framebuffer[1] = 0;
        framebuffer[2] = 0;
        framebuffer[3] = 0;
        framebuffer[4] = 0;
	pong_mode = 0;
        ioinit();
        
        for (;;)
        {
          portB = ~inp(PINB);
          if (portB & 0x01) {pong_mode = 1; main_pong();}
          if (portB & 0x02) {pong_mode = 0; main_font();}
        }
}
