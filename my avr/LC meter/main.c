#include<avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "uart.h"

volatile char  Data_Ready =0;					// Flag for timer0 interrupt to tell freq. measurement when time is up.
volatile int counter =0;						// Counts the number of timer0 interrupts.
volatile double	counter1_overflow = 0;  		// Counts the number of timer1 overflow events.
volatile double ioffset;						// The measurement offset value.
volatile char add_decimal_point;				// Flag to tell sendstring routine when to add a decimal point to the display.


ISR(TIMER0_OVF_vect)								// TIMER0 Interrupt Service
{  
		if  (counter ==61 )							//  Count N+1 number of 1/((fosc/1024)*256) interrupts. 62 Iterations is 1 seconds.
			Data_Ready = 0xFF;
		else;
{			counter++;
	}
}

ISR(TIMER1_OVF_vect)							// Counter1 Interrupt Service
{
	counter1_overflow++;							// Only keep track of the quantity of counter1 overflows.
}


void Timers_Setup (void)
{
	// Initialize Timers.
	TCCR0	 = 0b00000101;							//Set up Timer0.
	TCCR1B =  0b00000110; 							//Enable External T1 input (pin 9 on AT90S2313/TINY2313).
	TIMSK =   0x05;							//Counter/Timer Interrupt Mask -enable these two interrupts.	
}

void Timers_stop (void)								//Set the prescalers so the timers stop.
{
	TCCR0	 = 0b00000000;
	TCCR1B =  0b00000000; //       
}


double measure_frequency (void)						//Returns incoming frequency in Hz.
{   
	double 	frequency = 0;  						//Hz	
	TCNT1 = 0;										// Set up registers for the next measurement.
	frequency = 0;									
	counter = 0;									//Used by timer0 to count number of 65.536 ms interrupts.
	 Data_Ready =0;									// Flag from Timer0 interrupt to let this routine know when time is up.
	TCNT0=0;										// Clear timero data register.
	counter1_overflow=0;							// Clear the count of timer1 overflow events.
	Timers_Setup ();								// Restart clocks to the timers.
	sei ();											// Re-enable interrupts.
	while (!Data_Ready) {PORTA =0b11110000;};		//Wait for timer0 to count to 0, Light LED on B6.
	cli();											// Stop interrupts then compute and display results.
	Timers_stop();									//Stop clocks to the timers.
	PORTA=0b00000000;								//Turn off LED on B6.
	frequency = (1000 * (frequency + TCNT1 + (counter1_overflow  * 65536)))/1000; //Calculate the frequency for 1 second sampling.
	return frequency;
}
void measure_and_display_inductance (void)
/* The same formula is used to display inductance in tenths of microhenries and capacitance in picofarads when
a 0.0100 uf capacitor is used to measure inductance and a 1.00 milihenry inductor is used to measure capacitance. 
The formula below calculates "inductance" and displays it as inductance when measuring inductance and as capacitance 
when measuring capacitance. When displaying inductance, a decimal point is written to the left of the last digit so the
display can be read directly in microhenries and thenths of microhenries. The capacitance display does not have a 
decimal point and it reads directly in picofarags. 

When PORTD, bit 4 is found to be low during a measurement cycle, indicating that the "Zero" button has been pressed,
the result of the calculation, whether it is inductance or capacitance is stored as the offset value and then the offset
value is subtracted from subsequent readings.
*/

{
	double capacitance =   7.25;					//  Capacitance of .0100 uf capacitor reads 100 nh ls digit for inductance measurement. 
	double inductance =0;                              //  This corresponds to an inductor of 1.00 mh to get 1 pf ls digit for capacitance measurement.
	double numerator = 1e11;						// For proper scaling of results in 100 nh/1pf resolution.
	double frequency =0;							
	char numstring[16];									// Array for strings to be displayed.
	
	frequency = measure_frequency();				//Measure the incoming frequency.	
	inductance = (((numerator/(628*frequency))*(numerator/(628*frequency)))/capacitance) ; //The main event.
	
	if ((!(PIND & 0b00000001)))	//If D4 is low, store inductance reading as offset instead of displaying it.
			{
			ioffset = inductance;
			rsstr("ZERO SET\r\n");
			}
		else
	
			{		
			if(ioffset<inductance)
			inductance = inductance - ioffset;							// Compensate for the offset.				
			else
			inductance = ioffset-inductance;							// Compensate for the offset.				
			
			if ( (unsigned long) ((double) inductance) > 1000000000) 		// Error message if inductance is over 100H or 100uF.		
			{
				
				rsstr("RANGE");
				}
			else
			{
					ultoa( (unsigned long) ((double) inductance), numstring, 10);  // unsigned long to string
					  rsstr("C(pf)= ");
					  rsstr(numstring);	
                      rsstr("\r\n");
										// Send the inductance value.
					}
			}

}	

int main(void)
{
 USART_INIT();
 DDRA=0xFF;
 DDRB=0x00;
 PORTB=0xFF;
 rsstr("LC METER\r\n");
 
 while(1)
 { 
  measure_and_display_inductance ();
 }
   
 }
 
 