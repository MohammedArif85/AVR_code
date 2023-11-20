#include<avr/io.h>
#include<util/delay.h>
#include <compat/deprecated.h>

#define en_off sbi(PORTC,2)//enable
#define en_on  cbi(PORTC,2)

#define clkwise     sbi(PORTC,1)//orange
#define anticlkwise cbi(PORTC,1)

#define clk_on sbi(PORTC,0)
#define clk_off sbi(PORTC,0)//red
 

void clock(unsigned  int step)
{
unsigned int i;
 en_on;
 _delay_us(5);
 clkwise;
 _delay_us(5);
 for(i=0;i<step;i++)
 {
	 clk_on;
	 _delay_ms(1);
	 clk_off;
	 _delay_ms(1);
 }
en_off; 
 
 

} 

void anticlock(unsigned  int step)
{
unsigned int i;
 en_on;
 _delay_us(5);
 anticlkwise;
 _delay_us(5);
 for(i=0;i<step;i++)
 {
	 clk_on;
	 _delay_ms(1);
	 clk_off;
	 _delay_ms(1);
 }
en_off; 
 
 

}
void stop(void)
{
 en_off;
 clk_off;
 anticlkwise;
}


int main(void)
{
DDRA=0XFF;
DDRC=0XFF;
DDRD=0X00;

PORTA=0X00;
PORTC=0X00;
PORTD=0XFF;

	while(1)
	{
		 if(bit_is_clear(PIND,4))
		 {
		  
		  clock(20);
		 }
		 else if(bit_is_clear(PIND,5))
		 {
		  anticlock(20);
		 }
		 else
		 {
		  stop();
		 }
	}



 return 0;
}