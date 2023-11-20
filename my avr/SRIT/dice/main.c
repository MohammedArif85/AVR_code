#include<avr/io.h>
#include<util/delay.h>
int main(void)
{unsigned char i=1;
char A[6] = {0X79,0X24,0X30,0X19,0X12,0X02};
DDRA=0XFF;
DDRB=0X04;
PORTA=0X00;
PORTB=0X04;
while(1)
{if(!(PINB&0X04))
 {
  while(!(PINB&0x04))
   {
     PORTA=A[i-1];
	 _delay_ms(100);
	 i++;
	 if(i>6)
	  i=1;
   
   
   }

 }
}return(0);
}

