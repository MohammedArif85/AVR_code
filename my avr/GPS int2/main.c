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
volatile unsigned char timeString[20]; 		/* Buffer collects chars read from GPS */
volatile unsigned char charRead,GPScap=0,Latcard,Lngcard;  
volatile unsigned char   i,hour,min,sec,deg,mm,latdeg,latmin,lngdeg,lngmin,Pfix,nSV,latdsec_8,lngdsec_8;
float latdsec,lngdsec;	

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
  while (*str)
    ret = 10 * ret + *str++ - '0';
  return ret;
}
void gpstime()
{     
      hour=(timeString[0]-'0')*10+(timeString[1]-'0')*1;    
      min=(timeString[2]-'0')*10+(timeString[3]-'0')*1;    
      sec=(timeString[4]-'0')*10+(timeString[5]-'0')*1;    
         
}

void gpsposlat()
{     latdeg=(timeString[0]-'0')*10+(timeString[1]-'0')*1;    
      latmin=(timeString[2]-'0')*10+(timeString[3]-'0')*1;    
      latdsec=(timeString[5]-'0')*1000+(timeString[6]-'0')*100+(timeString[7]-'0')*10+(timeString[8]-'0')*1;    
        /*
          lcd_cmd(0xc5);
		  itoa(latdeg);
		  lcd_data(',');
		  itoa(latmin);
		  lcd_data('.');
		  itoal(latdsec);
		 */
}
void gpsposlong()
{     lngdeg=(timeString[0]-'0')*100+(timeString[1]-'0')*10+(timeString[2]-'0')*1;    
      lngmin=(timeString[3]-'0')*10+(timeString[4]-'0')*1;    
      lngdsec=(timeString[6]-'0')*1000+(timeString[7]-'0')*100+(timeString[8]-'0')*10+(timeString[9]-'0')*1;    
       /*
          lcd_cmd(0x85);
		  itoa(lngdeg);
		  lcd_data(',');
		  itoa(lngmin);
		  lcd_data('.');
		  itoal(lngdsec);*/
		 
}
void parseGPS()
{
unsigned char  *pChar,j,k,lastcommaPosition;
while(GPScap!=3);
 
if(stringRead[3] == 'G' && stringRead[4] == 'G' && stringRead[5] == 'A')
 {  PORTA=(PORTA^0x01)&0x01;
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
		  gpstime(); 
		
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
         // lcd_data(Latcard); 
		  
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
          //lcd_data(Lngcard); 
        // showlatlong();
          latdsec=latdsec*60;
          latdsec=latdsec/10000; 
          latdsec_8=(unsigned char)latdsec;
		  lngdsec=lngdsec*60;
          lngdsec=lngdsec/10000;
          lngdsec_8=(unsigned char)lngdsec;
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
		  Pfix=timeString[0]-'0';
		 
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
		  nSV=(timeString[0]-'0')*10+(timeString[1]-'0')*1;  
		    
 }
GPScap=0;

}
void showlatlong()
{
 
parseGPS();
 if(Pfix>0)
 {
 lcd_cmd(0x80);
 lcd_puts("LAT:");
 itoa(latdeg);
 lcd_data('-');
 itoa(latmin);
 lcd_data('-');
 itoa(latdsec_8);
 lcd_data(Latcard); 
 lcd_cmd(0xC0);
 lcd_puts("LON:");
 itoa(lngdeg);
 lcd_data('-');
 itoa(lngmin);
 lcd_data('-');
 itoa(lngdsec_8);
 lcd_data(Lngcard); 
 }
 else
 {
  lcd_cmd(0x80);
  lcd_puts("No coverage");
  lcd_cmd(0xc0);
  lcd_puts("Sat no.=");
  itoa(nSV);
 }
}
void gpsshowtime()
{
     parseGPS();
      lcd_cmd(0x82);
		  lcd_puts("GPS TIME");
		  lcd_cmd(0xC2);
		  itoa(hour);
		  lcd_data(':');
		  itoa(min);
		  lcd_data(':');
		  itoa(sec);
		 

}

int main(void)
{   char usr=2;
	USART_INIT();
	DDRA=0xFF;
	PORTA=0x00;
	DDRC=0x0F;
	PORTC=0xF0;
	sei();
	lcd_init();
	lcd_cmd(0x0c);
	lcd_cmd(0x01);
	lcd_puts("GPS Engine..");
	_delay_ms(2000);
	lcd_cmd(0x01);
    while(1)
	{ if((PINC&0x80)==0)
     {lcd_cmd(0x01);
 		while((PINC&0x80)==0);
		_delay_ms(100);
		
		if(usr==1)
		{
		 usr=2;
		}
		else
		{
		 usr=1;
		}
     }
     if(usr==1)
	 {
	  showlatlong();
	 }
	 if(usr==2)
	 {
	 gpsshowtime();
	 }	
	}
	return 0;
}





