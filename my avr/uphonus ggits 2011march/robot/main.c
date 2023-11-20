#include<avr/io.h>
#include<util/delay.h>
#include<avr/pgmspace.h>
#include<compat/deprecated.h>

#define uchar unsigned char
#define forw 0x50
#define back 0xA0
#define xright 0x60
#define xleft 0x90
#define left 0x40
#define right 0x10
#define brake 0xF0
#define stop 0x00

#define pwmmaxspeed 240

volatile uint8_t Reye;
volatile unsigned char current_cell,next_cell;
uint8_t midpoint[5] = { 0,0,0,0,0 };    /* sensor trip points */
/*
PD0 - switch 1 set
PD1 - switch 2 start
PD2 - switch 3 ok 
*/

#include "move.h"
#include "algo.h"


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
void toggle()
{
 if(PORTB&0x40)
 {
  cbi(PORTB,6);
 }
 else
 {
  sbi(PORTB,6);
 
 }
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
//start timer with prescaler 8
//start timer with prescaler 8
//TOP=ICR1;

//Output compare OC1A 8 bit non inverted PWM

//Clear OC1A on Compare Match, set OC1A at TOP

//Fast PWM

//ICR1=20000 defines 50Hz PWM

ICR1=40000;

TCCR1A|=(0<<COM1A0)|(1<<COM1A1)|(0<<COM1B0)|(1<<COM1B1)|

(0<<FOC1A)|(0<<FOC1B)|(1<<WGM11)|(0<<WGM10);

TCCR1B|=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|

(0<<CS12)|(1<<CS11)|(0<<CS10);
OCR1A=4500;//arm down
OCR1B=2810;//gripper open

 PORTC=forw;
 TCCR0=0x6D;
 TCNT0=0x00;
 TCCR2=0x6F;
 TCNT2=0x00;
}

uint8_t readobstacle()
{uint8_t p;
 p=(PINA&0xc0);
 if(p==0x00)
 return 0;//both white
 else if(p==0x40)
 return 2;//upblack 
 else
 return 1; //no block  
}
void pullback()
{
steer(60,60);
PORTC=back;
_delay_ms(300);

}
void detectandpick()
{
unsigned char r,t;
eye_read();
r=(Reye & 0b00011111);
 t=readobstacle();
 while(t!=0x00) 
 {
 switch(r)
 {case 0x01:drive_motor(0,pwmmaxspeed);break;//xrightmost95
  case 0x02:drive_motor(20,pwmmaxspeed);break;//xrightmost95
  case 0x03:drive_motor(pwmmaxspeed-210,pwmmaxspeed);break;//xrightmost95
  case 0x06:drive_motor(pwmmaxspeed-80,pwmmaxspeed);break;//slight right35
  case 0x04:drive_motor(pwmmaxspeed,pwmmaxspeed);break;//center
  case 0x0C:drive_motor(pwmmaxspeed,pwmmaxspeed-80);break;//sl left35
  case 0x18:drive_motor(pwmmaxspeed,pwmmaxspeed-210);break;//xleftmost95
  case 0x08:drive_motor(pwmmaxspeed,20);break;
  case 0x10:drive_motor(pwmmaxspeed,0);break;//xleftmost95

 } 
 eye_read();
r=(Reye & 0b00011111);
 t=readobstacle();
}

PORTC=brake;
OCR1A=2500;//arm down

_delay_ms(1000);
_delay_ms(1000);

OCR1B=4010;//gripper open
_delay_ms(1000);
_delay_ms(1000);

OCR1A=4500;//arm down
_delay_ms(1000);
_delay_ms(1000);
pullback();
}

void steer(unsigned char l,unsigned char r)
{
 OCR2=l;
 OCR0=r;
 
}
void pos_calculate()
{unsigned char r,side,t,tempo;
 side=(Reye & 0b00010001);
 r=(Reye & 0b00011111);
 t=readobstacle();
 if(side!=0b00010001 && t==1) 
 {
 switch(r)
 {case 0x01:drive_motor(0,pwmmaxspeed);break;//xrightmost95
  case 0x02:drive_motor(20,pwmmaxspeed);break;//xrightmost95
  case 0x03:drive_motor(pwmmaxspeed-210,pwmmaxspeed);break;//xrightmost95
  case 0x06:drive_motor(pwmmaxspeed-80,pwmmaxspeed);break;//slight right35
  case 0x04:drive_motor(pwmmaxspeed,pwmmaxspeed);break;//center
  case 0x0C:drive_motor(pwmmaxspeed,pwmmaxspeed-80);break;//sl left35
  case 0x18:drive_motor(pwmmaxspeed,pwmmaxspeed-210);break;//xleftmost95
  case 0x08:drive_motor(pwmmaxspeed,20);break;
  case 0x10:drive_motor(pwmmaxspeed,0);break;//xleftmost95

 } 
}
else
{
//toggle();
if(t==1)
decide();
else
{
PORTC=stop;
_delay_ms(2000);
t=readobstacle();
if(t==0)
{ 
 
 notify_danger();
 tempo=search_next_move(next_cell);
 algorun(current_cell,tempo);
 modify_trace_route();
 decideafterupdate();
// while(bit_is_set(PIND,0));
// while(bit_is_set(PIND,0));
// display_path();
/*turnright();
turnright();
*/
}
else
{
 detectandpick();
 tempo=search_next_move(next_cell);
 finalrun(current_cell,0x00);
 //algorun(current_cell,tempo);
// modify_trace_route();
 modify_trace_route_for_final_run();
 decideafterupdate();
}
//while(bit_is_set(PIND,0));

}
}
 
}
void pos_run()
{unsigned char r;
 
 r=(Reye & 0b00011111);
 
 
 switch(r)
 {case 0x01:drive_motor(0,pwmmaxspeed);break;//xrightmost95
  case 0x02:drive_motor(20,pwmmaxspeed);break;//xrightmost95
  case 0x03:drive_motor(pwmmaxspeed-210,pwmmaxspeed);break;//xrightmost95
  case 0x06:drive_motor(pwmmaxspeed-80,pwmmaxspeed);break;//slight right35
  case 0x04:drive_motor(pwmmaxspeed,pwmmaxspeed);break;//center
  case 0x0C:drive_motor(pwmmaxspeed,pwmmaxspeed-80);break;//sl left35
  case 0x18:drive_motor(pwmmaxspeed,pwmmaxspeed-210);break;//xleftmost95
  case 0x08:drive_motor(pwmmaxspeed,20);break;
  case 0x10:drive_motor(pwmmaxspeed,0);break;//xleftmost95

 } 

}

void avoid_zun()
{unsigned char r,side;
 eye_read();
 side=(Reye & 0b00010001);
 r=(Reye & 0b00001110);
 while(side!=0x00)
 {eye_read();
  side=(Reye & 0b00010001);
 r=(Reye & 0b00011111);
 switch(r) 
 {                                                                              
  case 0x01:drive_motor(pwmmaxspeed-120,pwmmaxspeed);break;//xrightmost95
  case 0x03:drive_motor(pwmmaxspeed-80,pwmmaxspeed);break;//xrightmost95
  case 0x06:drive_motor(pwmmaxspeed-60,pwmmaxspeed);break;//slight right35
  case 0x04:drive_motor(pwmmaxspeed,pwmmaxspeed);break;//center
  case 0x0C:drive_motor(pwmmaxspeed,pwmmaxspeed-60);break;//sl left35
  case 0x18:drive_motor(pwmmaxspeed,pwmmaxspeed-80);break;//xleftmost95
  case 0x10:drive_motor(pwmmaxspeed,pwmmaxspeed-120);break;//xleftmost95 
 }
 
 } 

 
}

void first_zun()
{unsigned char r,side;
 eye_read();
 side=(Reye & 0b00010001);
 r=(Reye & 0b00001110);
 while(side!=0x11)
 {eye_read();
  side=(Reye & 0b00010001);
 r=(Reye & 0b00011111);
 switch(r) 
 {                                                                              
  case 0x01:drive_motor(0,pwmmaxspeed);break;//xrightmost95
  case 0x02:drive_motor(20,pwmmaxspeed);break;//xrightmost95
  case 0x03:drive_motor(pwmmaxspeed-210,pwmmaxspeed);break;//xrightmost95
  case 0x06:drive_motor(pwmmaxspeed-80,pwmmaxspeed);break;//slight right35
  case 0x04:drive_motor(pwmmaxspeed,pwmmaxspeed);break;//center
  case 0x0C:drive_motor(pwmmaxspeed,pwmmaxspeed-80);break;//sl left35
  case 0x18:drive_motor(pwmmaxspeed,pwmmaxspeed-210);break;//xleftmost95
  case 0x08:drive_motor(pwmmaxspeed,20);break;
  case 0x10:drive_motor(pwmmaxspeed,0);break;//xleftmost95
 }
 
 } 
avoid_zun();
 
}
void drive_motor(unsigned char Lspeed,unsigned char Rspeed) 
{
 PORTC=forw;
 steer(Rspeed,Lspeed);//L-R
 
}

void notify_danger()
{
 unsigned char hb,lb;
 hb=(next_cell>>4)&0x0f;
 lb=(next_cell&0x0f);
 maze[hb][lb].temp=99;
 


}
void update_robot()
{
 unsigned char hb,lb;
 hb=(current_cell>>4)&0x0f;
 lb=(current_cell&0x0f);
 maze[hb][lb].visited=1;
 
}
void finalramp()
{unsigned char r,side;
 if(pos==4)
 {
 turnleft();
 }
 eye_read();
 side=(Reye & 0b00010001);
 r=(Reye & 0b00001110);
 while(side!=0x11)
 {eye_read();
  side=(Reye & 0b00010001);
 r=(Reye & 0b00011111);
 switch(r) 
 {                                                                              
  case 0x01:drive_motor(0,pwmmaxspeed);break;//xrightmost95
  case 0x02:drive_motor(20,pwmmaxspeed);break;//xrightmost95
  case 0x03:drive_motor(pwmmaxspeed-210,pwmmaxspeed);break;//xrightmost95
  case 0x06:drive_motor(pwmmaxspeed-80,pwmmaxspeed);break;//slight right35
  case 0x04:drive_motor(pwmmaxspeed,pwmmaxspeed);break;//center
  case 0x0C:drive_motor(pwmmaxspeed,pwmmaxspeed-80);break;//sl left35
  case 0x18:drive_motor(pwmmaxspeed,pwmmaxspeed-210);break;//xleftmost95
  case 0x08:drive_motor(pwmmaxspeed,20);break;
  case 0x10:drive_motor(pwmmaxspeed,0);break;//xleftmost95
 }
 
 } 
}
void decideafterupdate()
{
 
 tracepath(current_cell);
next_cell=search_next_move(current_cell);
}
void decide()
{
 current_cell=next_cell;
 update_robot();
 avoid_zun();
 tracepath(current_cell);
 next_cell=search_next_move(current_cell);
 if(next_cell==0xEE)
 {
  
  finalramp();
  PORTC=stop;
  while(bit_is_set(PIND,0));
 } 
 
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
                midpoint[i] = (thresh_low[i] + (thresh_high[i] - thresh_low[i]) / 2);
        }
       PORTC=stop;
}

void motor_test()
{
 PORTC=forw;
 steer(64,64);//right
 _delay_ms(5000);
 steer(128,128);//equal
 _delay_ms(5000);
 steer(192,192);//right
 _delay_ms(5000);
 steer(255,255);//left
  _delay_ms(5000);
 PORTC=0x00;
}  

int main(void)
{

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
 while(bit_is_set(PIND,0));
 current_cell=0x00;
 next_cell=0x00;
 fflush();
 //_delay_ms(2000);
// first_zun();
 while(1)
 {

 eye_read();

 pos_run();
 
 }
}

  
