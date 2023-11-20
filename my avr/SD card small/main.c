/***********************************************************************************************
*	  			  demo display bmp 24 bit from SD card with circuitidea.com BL-TFT240320PLUS
* File name		: main.h
* Programmer 	: jaruwit supa
* Web           : www.circuitidea.com
* Note			: demo
* Language		: avrGCC
* Hardware		: atmega16
* Date			: 01/05/2009
*************************************************************************************************
* DESCRIPTION
* ===========
* 1. read sd card
* 2. open bmp file
* 3. display picture
*
* HISTORY
* 26/09/2009 first release
* 01/10/2009 using stream to improve speed.
************************************************************************************************/

/* _____STANDARD INCLUDES____________________________________________________ */
#include <avr/io.h>
#include<util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>

/* _____PROJECT INCLUDES_____________________________________________________ */
#include "diskio.h"
#include "pff.h"
#include "lcd.h"



/* _____LOCAL VARIABLES______________________________________________________ */

/* _____PRIVATE FUNCTIONS____________________________________________________ */

/* _____PUBLICE FUNCTIONS____________________________________________________ */

int main (void)
{
	BYTE res;
	FATFS fs;			/* File system object */
	DIR dir;			/* Directory object */
	FILINFO fno;		/* File information */
//	unsigned int h = -1, w=-1;

//	LCD_BL_ON();
//	LCD_Reset();
    lcd_init();
	lcd_cmd(0x01);
	lcd_puts("SDCARD");
	res = disk_initialize();
	if (res == FR_OK)
	{
		res = pf_mount(&fs);
		if (res == FR_OK)
		{
			for(;;)
			{
				res = pf_opendir(&dir, "/");
				if (res == FR_OK)
				{
					for(;;)
					{
						res = pf_readdir(&dir, &fno);
						if (res != FR_OK)
							break;
						if (!fno.fname[0])	// end of file name/dir
							break;
                         lcd_cmd(0xc0);
                        lcd_puts(fno.fname); 
					
					}
				}
			}
		}
	}

	pf_mount(0);

	while(1);
}

