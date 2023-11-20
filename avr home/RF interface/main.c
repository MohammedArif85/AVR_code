/* interfacing CC2500 RF module
*********************************************************************************
*	COPYRIGHT(C)2009 Technophilia												*
*	date:-12 th september 2009													*
*	Author:																		*
*	Organization: Technophilia													*
*	Target system-SWARM devlopment board s/n-98020								*
*	manufacturer: Technophilia													*
*	microcontroller: ATmega-8													*
*	clock: 8mhz internal oscillator												*
*																				*	
*	H fuse=0xD9																	*
*	L fuse=0xE4																	*
*																				*
*	compiler win avr															*		
*																				*
*	PORTS USED																	*
*																				*	
*	CC2500……………….(RF trans receiver module)										*
*	PB5-MOSI																	*
*	PB6-MISO																	*
*	PB7-SCK																		*	
*	PB4-SS'																		*
*	PD2-GD0																		*		
*	PD3-GD2																		*
*																				*
*	LCD……………………….(PCD 8544, 48X84 pixel graphics LCD)							*
*	PB0-SCK																		*
*	PB1-SDIN																	*
*	PB2-D/C																		*
*	PB6-CS																		*		
*	PB7-RESET																	*
*																				*
*	MOTOR………………….																*
*	D4																			*
*	D5																			*
*	D6																			*
*	D7																			*
*																				*	
*	SENSOR……………….																*
*	PC0																			*		
*	PC1																			*
*	PC2																			*
*	PC3																			*
*	PC4																			*
*	PD0																			*	
*																				*
*	SWITCHES…………….																*
*	PC5-4 SWITCHES READ THROUGH ADC	 											*		
*																				*
*	LED……………………….																*	
*	LED1-PB0																	*
	LED2-PB1																	*
*	LED3-PB2																	*
*	LED4-PB3																	*
*																				*	
*																				*
*********************************************************************************/
#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>
#include "cc2500.h"
int main(void)
{
int i=5;
_delay_ms(500);
DDRA=0xFF;
rf_init();

 while(1)
  {
    sbi(PORTA,4); 
    rf_transmit(i);
	_delay_ms(1000);
	cbi(PORTA,4);
	_delay_ms(1000);
	
  }
}