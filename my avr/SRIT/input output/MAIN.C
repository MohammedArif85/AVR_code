# include<avr/io.h>
# include<util/delay.h>

int main(void)
{
 unsigned char i=0;
 char A[10] ={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
 DDRA=0XFF;
 DDRB=0X04;
 PORTB=0X04;
 PORTA=0X00;
while(1)
{
 if(!(PINB & 0x04))//if(PORTB_2==0)
 {
 _delay_ms (1);
 while(!(PINB & 0x04))
 {
   for(i=1;i<7;i++)
   {   
    PORTA=A[i];
	_delay_ms(1000);
	if(!(PINB & 0x04))
	break;
   
   }
 }

 }
 
 }
 
return 0;
}
