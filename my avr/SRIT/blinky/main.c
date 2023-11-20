# include <avr/io.h>
# include <util/delay.h>
int main (void)
{ unsigned char i;
 DDRA= 0xF0;
 PORTA= 0X00;
 while (1)
 {
  PORTA=0XA0;
  _delay_ms(3000);
  PORTA=0X50;
  _delay_ms(5000);
  
 } return 0;
}