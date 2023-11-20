#include<avr/io.h>
#include<util/delay.h>
#include<avr/pgmspace.h>
#include<compat/deprecated.h>

#define uchar unsigned char
#define forw 0x50
#define back 0xA0
#define xright 0x90
#define xleft 0x60
#define left 0x40
#define right 0x10
#define brake 0xF0
#define stop 0x00

#define pwmmaxspeed 150

volatile uint8_t Reye,nattu,pikku;

uint8_t midpoint[5] = { 0,0,0,0,0 };    /* sensor trip points */
/*
PD0 - switch 1 set
PD1 - switch 2 start
PD2 - switch 3 ok 
*/



uint8_t adc(unsigned char channel)
{uint8_t adc_value;
         ADMUX = 0x60|channel; // channel 0, left-justified result
        ADCSRA = _BV(ADEN) | _BV(ADPS2)| _BV(ADPS0);     
        /* read output from ADC */
        ADCSRA |= _BV(ADSC);
        while (!(ADCSRA & _BV(ADIF)));
        adc_value = ADCH;
        ADCSRA |= _BV(ADIF);
        
        return adc_value;

}


void eye_read()
{ uint8_t adc_value,i;
   Reye=0;
   for (i=0; i<5; i++)
                {
                        Reye = Reye << 1;
                        adc_value = adc(i);
                        if (adc_value >= midpoint[i]) 
                        {
                                Reye |= _BV(0);  
                        }
                        else
                        {
                                Reye &= ~_BV(0);
                        }
                }
 if(Reye&0x01)
 sbi(PORTB,0);
 else
 cbi(PORTB,0);
 if(Reye&0x02)
 sbi(PORTB,1);
 else
 cbi(PORTB,1);
 if(Reye&0x04)
 sbi(PORTB,2);
 else
 cbi(PORTB,2);
 if(Reye&0x08)
 sbi(PORTB,5);
 else
 cbi(PORTB,5);
 if(Reye&0x10)
 sbi(PORTB,6);
 else
 cbi(PORTB,6);

}
void motor_pwm_init()
{DDRC=0xFF;
 PORTC=0x00;
 DDRB=0xFF;
 DDRD=0xF0;
 PORTD=0xFF;

 PORTC=forw;
 TCCR0=0x6D;
 TCNT0=0x00;
 TCCR2=0x6F;
 TCNT2=0x00;
}



void steer(unsigned char l,unsigned char r)
{
 OCR2=l;
 OCR0=r;
 
}
void pos_calculate()
{unsigned char r,side;
 side=(Reye & 0b00011111);
 r=(Reye & 0b00011111);
 
 if(side!=0b00000000 && side!=0b00011111) 
 {
 switch(r)
 {
  case 0x07:drive_motor(20,pwmmaxspeed);break;//xrightmost95
  case 0x03:drive_motor(pwmmaxspeed-100,pwmmaxspeed);break;//xrightmost95
 
  
  case 0x10:drive_motor(pwmmaxspeed,pwmmaxspeed);break;//center
  case 0x11:drive_motor(pwmmaxspeed,pwmmaxspeed);break;//center
  case 0x13:drive_motor(pwmmaxspeed,pwmmaxspeed);break;//center
  case 0x19:drive_motor(pwmmaxspeed,pwmmaxspeed);break;//center
  
 
  case 0x18:drive_motor(pwmmaxspeed,pwmmaxspeed-100);break;//xleftmost95
  case 0x1C:drive_motor(pwmmaxspeed,20);break;


 } 
}
else if(side==0b00011111)
{PORTC=stop;
  _delay_ms(1000);
 if(nattu<1)
 {
 eye_read();
 r=(Reye & 0b00011111);
 while(r==0b00011111)
 {
 eye_read();
 r=(Reye & 0b00011111);
 drive_motor(110,110);
 if(pikku==0)
 {PORTC=xright;}
 else
 {PORTC=xleft;}
 
 }
 
// _delay_ms(1000);
 PORTC=forw;
 nattu++;
 }
}

else
{
 if(nattu<1)
 {
 PORTC=forw;
 }
 else
 {
 if(pikku==0)
 {
 PORTC=xleft;
  _delay_ms(100);
 }
 else
 {
 PORTC=xright;
  _delay_ms(100);


 } 
 PORTC=forw;
  _delay_ms(500);
 PORTC=stop;
 while(bit_is_set(PIND,0));
 }
 
}
//while(bit_is_set(PIND,0));

}



void drive_motor(unsigned char Lspeed,unsigned char Rspeed) 
{
 PORTC=forw;
 steer(Lspeed,Rspeed);//L-R
 
}
void calibrate_sensors()
{
        uint8_t adc_value;              /* ADC value */
        uint8_t i, j;                   /* loop counter */ 
        uint8_t thresh_high[5] = { 0,0,0,0,0 };
        uint8_t thresh_low[5] = { 255,255,255,255,255 };
         PORTC=xleft; steer(pwmmaxspeed-40,pwmmaxspeed-40);           
        for (i=0; i<100; i++) 
        { 
                for (j=0; j<5; j++)
                {
                        adc_value = adc(j);
                        if (adc_value < thresh_low[j]) thresh_low[j] = adc_value;
                        if (adc_value > thresh_high[j]) thresh_high[j] = adc_value;
                }
                _delay_ms(50); 
        }
        
        for (i=0; i<5; i++)
        {
                midpoint[i] =  (thresh_high[i] + thresh_low[i]) / 2;
        }
       PORTC=stop;
}

void motor_test()
{
 PORTC=forw;
 steer(64,64);//right
 _delay_ms(2000);

 steer(110,110);//right
 _delay_ms(1000);

 steer(150,150);//right
 _delay_ms(1000);

 steer(240,240);//right
 _delay_ms(2000);

 PORTC=0x00;
}  

int main(void)
{
 nattu=0;pikku=0;
 DDRD=0xF0;
 PORTD=0x0F;
 DDRA=0x00;
 PORTA=0xC0;
 motor_pwm_init();
 sbi(PORTB,7);
 _delay_ms(2000);
//motor_test();
 cbi(PORTB,7);
 calibrate_sensors();
 while(bit_is_set(PIND,0) && bit_is_set(PIND,1));
 if(bit_is_clear(PIND,1))
 pikku=1;
  _delay_ms(2000);
 while(1)
 {

eye_read();

 pos_calculate();
 
 }
}

  
