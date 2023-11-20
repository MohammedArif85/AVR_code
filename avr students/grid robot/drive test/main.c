#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>
#include "lcd.h"
/*
  8 4 2 1 8 4 2 1
  7 6 5 4 3 2 1 0 
  + - + -
  1 0 1 0      =0xA0
  0 1 0 1      =0x50 

*/

/*
  L1 L2 C R2 R1
   4  3 2 0 1
*/
volatile int eye_mid[5];
volatile unsigned char eyeval;
int adcread(unsigned char channel)
{
 ADMUX=0x40|channel;
 ADCSRA|=(1<<ADEN);
 ADCSRA|=(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
 while(!(ADCSRA&(1<<ADIF)));
 return ADC;
}
void eye_check()
{
 uint8_t i;
 int max[5];
 int min[5];
 lcd_cmd(0x01);
 lcd_puts("TEST Black");
 while(bit_is_set(PIND,0));
 _delay_ms(1000);
 min[0]=adcread(0); 
 min[1]=adcread(1); 
 min[2]=adcread(2); 
 min[3]=adcread(3); 
 min[4]=adcread(4); 
 lcd_cmd(0x01);
 lcd_puts("TEST white");
 while(bit_is_set(PIND,0)); 
 _delay_ms(1000);
 max[0]=adcread(0); 
 max[1]=adcread(1); 
 max[2]=adcread(2); 
 max[3]=adcread(3); 
 max[4]=adcread(4); 
 for(i=0;i<5;i++)
 {
  eye_mid[i]=(max[i]+min[i])/2;
 }
 while(bit_is_set(PIND,0)); 
 lcd_cmd(0x01);
}
void store_eye()
{ int t;
  t=adcread(0);
  eyeval=0;
  if(t>eye_mid[0])
  {
   sbi(eyeval,1);
  } 
  else
  {
   cbi(eyeval,1);
  }
  t=adcread(1);
  if(t>eye_mid[1])
  {
   sbi(eyeval,0);
  } 
  else
  {
   cbi(eyeval,0); 
  }
  t=adcread(2);
  if(t>eye_mid[2])
  {
   sbi(eyeval,2);
  } 
  else
  {
   cbi(eyeval,2);
  }
  t=adcread(3);
  if(t>eye_mid[3])
  {
   sbi(eyeval,3);
  } 
  else
  {
   cbi(eyeval,3);
  }
  t=adcread(4);
  if(t>eye_mid[4])
  {
   sbi(eyeval,4);
  } 
  else
  {
   cbi(eyeval,4);
  }

}
int main(void)
{  
 unsigned char r;
 lcd_init();
 lcd_cmd(0x0c);//display on curser off 
 lcd_cmd(0x01);//display on curser off 
 DDRA=0x00;//input
 DDRC=0xFF;//output 
 DDRD=0x00;
 PORTD=0xFF;
 PORTC=0x00;
 PORTC=0xA0;
  _delay_ms(2000);
 PORTC=0x50;
  _delay_ms(2000);
 PORTC=0x00;
 while(1)
 {
 }
}