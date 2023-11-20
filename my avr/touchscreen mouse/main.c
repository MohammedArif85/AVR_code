/*	
 * Project: USB Touchscreen Mouse
 * Author: Aaron Weiss
 * Date: 7-13-2009
 * copyright: Spark Fun Electronics© 2009
 * Released under the Creative Commons Attribution Share-Alike 3.0 License
 * http://creativecommons.org/licenses/by-sa/3.0
 * External 16MHz crystal. Fuse bits: LFuse 0xdf, HFuse 0xde
*/

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#define FOSC 16000000
#define BAUD 9600
#define MYUBRR 103 

#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask))
#define BUFFER 5        //how many samples to take from the ADC before moving the mouse
#define LOWACCURACY 1   //if values are higher than this move mouse
#define HIGHACCURACY 300 //if values are lower than this move mouse
#define SCALE 1         //speed of mouse


//Global Vars
//======================
uint16_t  l;// xlow register
uint16_t  h;// xhigh register
uint16_t  l1;// ylow register
uint16_t  h1;// yhigh register
volatile uint16_t  x_buffer[BUFFER + 1];
volatile uint16_t  y_buffer[BUFFER + 1];
unsigned char cpx,cpy;
int  XDIFF,YDIFF;

int i = 0;
char movex;
char movey;
//Define functions
//======================
void ioinit(void);      // initializes IO
static int uart_putchar(char c, FILE *stream);
uint8_t uart_getchar(void);
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
void delay_ms(uint16_t x); // general purpose delay

void read_x(void);
void read_y(void);

//===========================================================


#include "lcd.h"

int main (void)

{

  lcd_init();//initialise the lcd
  lcd_cmd(0x01);//clear screen
  ioinit();
  lcd_puts("Touchpad");
    
   
	while(1)
    {	
	
	calc();
//	_delay_ms(100);
    }
    return(0);
}
void ioinit (void)
{
    //1 = output, 0 = input
    DDRD = 0b11111110; //PORTD (RX on PD0)

    //USART Baud rate: 9600
    UBRRH = MYUBRR >> 8;
    UBRRL = MYUBRR;
    
	UCSRC=(1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|(0<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);
/*
===================================================================
ENABLE TRANSMITTER AND RECEIVER	
===================================================================
*/
	UCSRB=(1<<RXEN)|(1<<TXEN); 
	
	stdout = &mystdout; //Required for printf init
}

static int uart_putchar(char c, FILE *stream)
{
    if (c == '\n') uart_putchar('\r', stream);
  
    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = c;
    
    return 0;
}

uint8_t uart_getchar(void)
{
    while( !(UCSRA & (1<<RXC)) );
    return(UDR);
}
unsigned char adc(unsigned char channel)
{  unsigned int val;
   unsigned char c;
   ADMUX=0x60+channel;
   ADCSRA=(1<<ADEN);
   c=8;val=0;
   while(c--)
   {
   ADCSRA|=(1<<ADSC);
    ADCSRA|=0x07;
   while(bit_is_clear(ADCSRA,ADIF));
   sbi(ADCSRA,ADIF);
   val=val+ADCH; 
   }
   val=val/8;
   val=val&0x00ff;
   return val;
}

void Xread()
{
 DDRA=0b11110101;
 
 sbi(PORTA,0);
 cbi(PORTA,2);
 cpx=adc(1);
// lcd_cmd(0xc0);
// lcd_puts("X="); 
// itoa(cpx);
}
void Yread()
{
 DDRA=0b11111010;
 sbi(PORTA,1);
 cbi(PORTA,3);
 cpy=adc(0);
// lcd_cmd(0xc7);
// lcd_puts("Y="); 
// itoa(cpy);
}

void calc(void)
{  char dtoo[5];
		Xread();
		Yread();
		x_buffer[i] = cpx;
		y_buffer[i] = cpy;
	  ++i;
	 if(i>BUFFER+1)
	 {
	  i=0;
	  XDIFF= (x_buffer[0]-x_buffer[BUFFER-1]); 
	  YDIFF= (y_buffer[0]-y_buffer[BUFFER-1]);
	 }
	 printf("%d",XDIFF);
	 printf(",%d\n",YDIFF);
	 lcd_cmd(0xc1);
	 sprintf(dtoo,"%5d",XDIFF);
	 lcd_puts(dtoo);
	 sprintf(dtoo,"%5d",YDIFF);
	 lcd_puts(dtoo);
	
     

	
		
		
	}


