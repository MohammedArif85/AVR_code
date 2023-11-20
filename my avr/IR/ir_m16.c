/*#########################################################

Demo program for testing the IR Remote control libraries.
-----------------------------------------------------------

Copyright Avinash Gupta 2008 avinash@eXtremeElectronics.co.in

Web: www.eXtremeElectronics.co.in

Please give credit or reference to www.eXtremeElectronics.co.in
if you use this in your projects and find it useful.


##########################################################*/

#include <avr/io.h>
#include <compat/deprecated.h>

#include "rckeys.h"
#include "remote.c"
#include "lcd.h"

void main()
{   
	uint8_t cmd=0;
	DDRA=0XFF;
	PORTA=0X00;

	RemoteInit();

	//LCDInit(LS_BLINK);

//	LCDClear();

//	LCDWriteString("IR RemoteDecoder");
//	LCDWriteStringXY(0,1,"Key Code:");

	while(1)
	{
		cmd=GetRemoteCmd(1);
		 
		 switch(cmd)
		 {
		  case 19:PORTA=PORTA^0x10;break;
		  case 18:PORTA=PORTA^0x20;break;
		  case 17:PORTA=PORTA^0x40;break;
		  case 16:PORTA=PORTA^0x80;break;
		  
		}
		
	
//		LCDWriteIntXY(10,1,cmd,3);
	}
}
