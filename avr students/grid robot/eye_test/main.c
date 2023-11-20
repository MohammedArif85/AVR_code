#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>
#include "lcd.h"
volatile int eye_mid[5];
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

int main(void)
{ int t; 
  unsigned char r;
 lcd_init();
 lcd_cmd(0x0c);//display on curser off 
 lcd_cmd(0x01);//display on curser off 
 DDRA=0x00;//input
 DDRC=0xFF;//output 
 DDRD=0x00;
 PORTD=0xFF;
 PORTC=0x00;
 eye_check();
 while(1)
 {lcd_cmd(0x80);
  t=adcread(0);
  if(t>eye_mid[0])
  {
   lcd_puts("1 ");
  } 
  else
  {
   lcd_puts("0 ");
  }
  t=adcread(1);
  if(t>eye_mid[1])
  {
   lcd_puts("1 ");
  } 
  else
  {
   lcd_puts("0 ");
  }
  t=adcread(2);
  if(t>eye_mid[2])
  {
   lcd_puts("1 ");
  } 
  else
  {
   lcd_puts("0 ");
  }
  t=adcread(3);
  if(t>eye_mid[3])
  {
   lcd_puts("1 ");
  } 
  else
  {
   lcd_puts("0 ");
  }
  t=adcread(4);
  if(t>eye_mid[4])
  {
   lcd_puts("1 ");
  } 
  else
  {
   lcd_puts("0 ");
  }

  }

}