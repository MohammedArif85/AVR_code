/*
===================================================================
PROGRAM TO PRINT A STRING ON COMPUTER SCREEN
===================================================================
*/

#include<avr/io.h>
#include<util/delay.h>
#include "uart.h"
#include"lcd.h"

#include<compat/deprecated.h>
volatile unsigned char stringRead[100]; 		/* Buffer collects chars read from GPS */
volatile unsigned char timeString[20]; 		/* Buffer collects chars read from GPS */

int main(void)
{   unsigned char charRead;  
	unsigned char  *pChar;
    unsigned char   i,j, k;	
	USART_INIT();
	lcd_init();//initialise the lcd
   lcd_cmd(0x01);//clear screen
   lcd_puts("GPS ENGINE");
   _delay_ms(2000);
   lcd_cmd(0x01);
    DDRA=0xFF;
	DDRD=0x00;
	while(1)
	{
	charRead=com_rx();
	if(charRead == '$') 
	{  
   /* GPS messages start with $ char */
	  i = 0;
	  stringRead[i] = charRead;
	  do 
	  {
	     charRead = com_rx();
	     i++;
		stringRead[i] = charRead;
	    
	  } 
	  while(charRead != 0x0d);
     /* By this point, a complete GPS string has been read so save it to file */
	  /* Append the null terminator to the string read */
	  stringRead[i+1] = '\0';
      if(stringRead[3] == 'G' && stringRead[4] == 'G' && stringRead[5] == 'A')
 {  PORTA=(PORTA^0x10)&0x10;
	      /*
		 Found GPGGA string.  It has 14 commas total.  Its NMEA sentence structure is:

		 $GPGAA,hhmmss.ss,ddmm.mmmm,n,dddmm.mmmm,e,q,ss,y.y,a.a,z,g.g,z,t.t,iii*CC
		 |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
		 0   	   1         2         3         4         5         6         7
		 0123456789012345678901234567890123456789012345678901234567890123456789012

		 where:

		 GPGAA		: GPS fixed data identifier
		 hhmmss.ss	: Coordinated Universal Time (UTC), also known as GMT
		 ddmm.mmmm,n	: Latitude in degrees, minutes and cardinal sign
		 dddmm.mmmm,e	: Longitude in degrees, minutes and cardinal sign
		 q		: Quality of fix.  1 = there is a fix
		 ss		: Number of satellites being used
		 y.y		: Horizontal dilution of precision
		 a.a,M		: GPS antenna altitude in meters
		 g.g,M		: geoidal separation in meters
		 t.t		: Age of the defferential correction data
		 iiii		: Deferential station's ID
		 *CC		: checksum for the sentence
	      */
          pChar = stringRead;
	      /* Get UTC time */
	      j = 7;  /* start of time field */
	      k = 0;
	      while(*(pChar+j) != ',') 
		  {
		   timeString[k] = *(pChar+j);
		   j++;
		   k++;
	      }
	      timeString[k] = '\0';
		  lcd_cmd(0xc0);
          lcd_puts(timeString);

 }
 }
 }
	return 0;
}

/*
long TinyGPS::gpsatol(const char *str)
{
  long ret = 0;
  while (gpsisdigit(*str))
    ret = 10 * ret + *str++ - '0';
  return ret;
}

*/

