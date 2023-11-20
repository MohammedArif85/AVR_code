//********************************************************
// *********** PWM DC MOTOR CONTROL with IR *************
//********************************************************
//Controller:	ATmega8 (1MHz internal Crystal)
//Compiler:		ICCAVR
//Author:		CC Dharmani, Chennai(India)
//Date:			Dec 2008
//********************************************************
//this program uses two of the three PWM channels (OC1A & OC1B)
//of the ATmega8, for controlling speed & direction of DC motor
//The remote of Sony TV was used for sending IR codes
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<compat/deprecated.h>
#include "PWM_main.h"

//*********************************************************************
//   Initializing functions for ports, timer0 & timer1
//*********************************************************************	 
void port_init(void)
{
 PORTB = 0x00; 
 DDRB  = 0x06; //PWM pins OC1A & OC1B defined as outputs

 DDRC  = 0xFF; //LED for IR detection indication
 PORTC = 0x00; 
 DDRD  = 0x01; //LED, for testing purpose
 PORTD = 0x00; 
 DDRA=0xFF;
}

//timer0 init
void timer0_init(void)
{
    //8-bit timer for measuring delay between IR pulses
	TCCR0 = 0x05; //CLK / 64/1024
	TCNT0 = 0; //reset the timer
}



/**************************************************************************
* Interrupt Service Routine for INT0
* Executed whenever a remote code is detected
**************************************************************************/
//#pragma interrupt_handler int0_isr:2
ISR (INT0_vect)
{ 		
	unsigned char count, code, address;
	unsigned int IR_input;
	
	TCNT0 = 0;
	while(!(PIND & 0x04));
	count = TCNT0;
	
	if(count < 30) 		  //to verify start pulse (2.4 ms long)
	{
	  _delay_ms(20);	
	  ENABLE_INT0;
	  return;
	}
	  
	PORTA |= 0x20;
	
	IR_input = read_IR ();
	
	code = (unsigned char) ((IR_input & 0xff00) >> 8);
	address = (unsigned char) (IR_input & 0x00ff);
	
	//motorControl ( code, address );
	
	
	_delay_ms(100);
	PORTA = (code<<4);//PORTC=0;
}

//*********************************************************************
//Function to read IR message from the detector
//Return value contains code in upper byte and address in lower byte
//*********************************************************************	 
unsigned int read_IR (void)
{
    unsigned char pulseCount=0,  code = 0, address = 0, timerCount;
	unsigned int IR_input;
	 
	while(pulseCount < 7)
	{
	   while(PIND & 0x04);
	   TCNT0 = 0;
	   
	   while(!(PIND & 0x04));
	   pulseCount++;
	 
	   timerCount = TCNT0;
	 
	   if(timerCount > 14)
	      code = code | (1 << (pulseCount-1));
	   else
	 	  code = code & ~(1 << (pulseCount-1));  
	 }

	 pulseCount = 0;
	 while(pulseCount < 5)
	 {
	   while(PIND & 0x04);
	   TCNT0 = 0;
	   
	   while(!(PIND & 0x04));
	   pulseCount++;
	 
	   timerCount = TCNT0;
	 
	   if(timerCount > 14)
	      address = address | (1 << (pulseCount-1));
	   else
	 	  address = address & ~(1 << (pulseCount-1));  
	 }
	 
	 IR_input = (((unsigned int)code) << 8) | address;
	 
	 return(IR_input);
}

//****************************************************************************
//Function to control motor speed & direction depending onthe IR code rceived
//Argumets are the code and address values received from IR detector
//****************************************************************************	 
void motorControl (unsigned char code, unsigned char address)
{
	 static unsigned char counter, dir, dir1;
	 
	 if (address != 1)	//detect only TV remote, other signals rejected
	   return;

	 if((code == 16) ) //Channel+ or Channel- button is pressed
     {	PORTC=0xAA;
   	 sbi(PORTA,4);
	 }
	 if(code == 17)
	 {
	 PORTC=0x55;
	 }
	 if((code == 18) || (code == 19)) //Channel+ or Channel- button is pressed
     {	PORTC=0x00;
   	  cbi(PORTA,4);
	 }
	
	 
	 
}
	
//************************************************************
//*** call this routine to initialize all peripherals
//************************************************************
void init_devices(void)
{
 //stop errant interrupts until set up
 cli(); //disable all interrupts
 port_init();
 timer0_init();
 MCUCR = 0x02;
 GICR  = 0x40;
 sei(); //re-enable interrupts
 //all peripherals are now initialized
}


//*******************************************************
//	************  MAIN FUNCTION  *************
//*******************************************************	 
void main(void)
{
 init_devices();
  
 while(1); //infinite loop, waiting for interrups from IR detector

}

//******************************  END  ***************************************
