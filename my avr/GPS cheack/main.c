/*
===================================================================
PROGRAM TO PRINT A STRING ON COMPUTER SCREEN
===================================================================
*/

#include<avr/io.h>
#include<avr/interrupt.h>
#include <avr/sleep.h>
#include "uart.h"
#include<util/delay.h>
#include<compat/deprecated.h>
#include "lcd.h"
volatile unsigned char stringRead[100]; 		/* Buffer collects chars read from GPS */
volatile unsigned char timeString[]="4335.9079"; 		/* Buffer collects chars read from GPS */
volatile unsigned char charRead,GPScap=0,Latcard,Lngcard;  
volatile unsigned char   i,hour,min,sec,deg,mm,latdeg,latmin,lngdeg,lngmin;
volatile float latdsec,lngdsec;	

ISR(USART_RXC_vect)
{

	charRead=UDR;
	if(charRead == '$' && GPScap==0) 
	{  
   /* GPS messages start with $ char */
	  i = 0;
	  stringRead[i] = charRead;
	  GPScap=1;
	  i++;
	 }
	 else if(GPScap==1 && charRead !=0x0d)
	 {
	  stringRead[i] = charRead;
	  i++;  
	 
	 }
	   /* By this point, a complete GPS string has been read*/
	  /* Append the null terminator to the string read */
     else if(charRead ==0x0d)
	 {
	  stringRead[i] = '\0';
	  GPScap=3;	 
	 }
     
}
long gpsatol(const char *str)
{
  long ret = 0;
  while (*str >= '0' && *str <= '9')
    ret = 10 * ret + *str++ - '0';
  return ret;
}
void gpstime()
{     hour=(timeString[0]-'0')*10+(timeString[1]-'0')*1;    
      min=(timeString[2]-'0')*10+(timeString[3]-'0')*1;    
      sec=(timeString[4]-'0')*10+(timeString[5]-'0')*1;    

          lcd_cmd(0x82);
		  itoa(hour);
		  lcd_data(':');
		  itoa(min);
		  lcd_data(':');
		  itoa(sec);
		 
}
void gpsposlat()
{     latdeg=(timeString[0]-'0')*10+(timeString[1]-'0')*1;    
      latmin=(timeString[2]-'0')*10+(timeString[3]-'0')*1;    
      latdsec=(timeString[5]-'0')*1000+(timeString[6]-'0')*100+(timeString[7]-'0')*10+(timeString[8]-'0')*1;    
        
          lcd_cmd(0xc5);
		  itoa(latdeg);
		  lcd_data(',');
		  itoa(latmin);
		  lcd_data('.');
		  itoal(latdsec);
		 
}
void gpsposlong()
{     lngdeg=(timeString[0]-'0')*100+(timeString[1]-'0')*10+(timeString[2]-'0')*1;    
      lngmin=(timeString[3]-'0')*10+(timeString[4]-'0')*1;    
      lngdsec=(timeString[6]-'0')*1000+(timeString[7]-'0')*100+(timeString[8]-'0')*10+(timeString[9]-'0')*1;    
       
          lcd_cmd(0x85);
		  itoa(lngdeg);
		  lcd_data(',');
		  itoa(lngmin);
		  lcd_data('.');
		  itoal(lngdsec);
		 
}

void parseGPS()
{
unsigned char  *pChar,j,k,lastcommaPosition;
while(GPScap!=3);
 
if(stringRead[3] == 'G' && stringRead[4] == 'G' && stringRead[5] == 'A')
 {  PORTA=(PORTA^0x10)&0x10;
	      /*
		 Found GPGGA string.  It has 14 commas total.  Its NMEA sentence structure is:

		 $GPGGA,hhmmss.ss,ddmm.mmmm,n,dddmm.mmmm,e,q,ss,y.y,a.a,z,g.g,z,t.t,iii*CC
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
		//  gpstime(); 
		lastcommaPosition = j;
         pChar = stringRead;
	      j = lastcommaPosition + 1;
	      k = 0;
	      while(*(pChar+j) != ',') 
		  {
		   timeString[k] = *(pChar+j);
		   j++;
		   k++;
	      }
	      lastcommaPosition = j;
	      timeString[k] = '\0';
          gpsposlat();
		lastcommaPosition = j;
         pChar = stringRead;
	      j = lastcommaPosition + 1;
	      k = 0;
	      while(*(pChar+j) != ',') 
		  {
		   timeString[k] = *(pChar+j);
		   j++;
		   k++;
	      }
	      lastcommaPosition = j;
	      timeString[k] = '\0';
          Latcard=timeString[0];
          lcd_data(Latcard); 
		   pChar = stringRead;
	      j = lastcommaPosition + 1;
	      k = 0;
	      while(*(pChar+j) != ',') 
		  {
		   timeString[k] = *(pChar+j);
		   j++;
		   k++;
	      }
	      lastcommaPosition = j;
	      timeString[k] = '\0';
          gpsposlong();
		  pChar = stringRead;
	      j = lastcommaPosition + 1;
	      k = 0;
	      while(*(pChar+j) != ',') 
		  {
		   timeString[k] = *(pChar+j);
		   j++;
		   k++;
	      }
	      lastcommaPosition = j;
	      timeString[k] = '\0';
          Lngcard=timeString[0];
          lcd_data(Lngcard); 
 }

GPScap=0;


}

unsigned long parse_degrees()
{ 
  char *p;
  unsigned long left = gpsatol(timeString);
  unsigned long tenk_minutes = (left % 100UL) * 10000UL;
  for (p=timeString; (*p) >= '0' && (*p) <= '9'; ++p);
  if (*p == '.')
  {
    unsigned long mult = 1000;
    while ((*++p) >= '0' && (*++p) <= '9')
    {
      tenk_minutes += mult * (*p - '0');
      mult /= 10;
    }
  }
  return (left / 100) * 100000 + tenk_minutes / 6;
}

int main(void)
{   unsigned char buf[16];
    long test;
	USART_INIT();
	DDRA=0xFF;
	PORTA=0x00;
	//sei();
	lcd_init();
	lcd_cmd(0x0c);
	lcd_cmd(0x01);
	lcd_puts("GPS Engine..");
	_delay_ms(1000);
	lcd_cmd(0x01);
	test=parse_degrees();
	sprintf(buf,"%ld",test);
	lcd_puts(buf);
    while(1)
	{
	
		
	}
	return 0;
}





