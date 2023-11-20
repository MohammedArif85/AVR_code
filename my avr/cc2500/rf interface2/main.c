/* interfacing CC2500 RF module
	PORTS USED																	*
*																				*	
*	CC2500……………….(RF trans receiver module)										*
*	PB3-MOSI																	*
*	PB4-MISO																	*
*	PB5-SCK																		*	
*	PD1-SS'																		*
*	PD2-GD0																		*		
*	PD3-GD2	
*/


#include<avr/io.h>
#include<util/delay.h>
#include"uart.h"
#include "cc2500.h"

int main(void)
{
rf_init();
rf_transmit();
 while(1);
  
}