
#include<avr/io.h>		//This is the header for AVR Microcontroller.
#include<avr/interrupt.h>
#include<util/delay.h> 
#define clegc 90  //center leg center
#define rlegc 150 //right leg center
#define llegc 80  //left leg center
#define llegf 60  //left leg forw  
#define rlegf 170  //right leg forw
#define llegb 100  //left leg back
#define rlegb 130  //right leg back
#define clegr 80  //center leg right
#define clegl 100  //center leg left


int i,j,k;   
unsigned char m0h,m0l,m1h,m1l,m2h,m2l,m3h,m3l,x,y;
int time=300;
unsigned char temp;
float mid1=90,mid2=90,mid3=90;   
ISR (TIMER1_COMPA_vect) //void timer1_compa_isr(void)
{
PORTC=0X00;
TCNT1H=0x00;
TCNT1L=0x00;
}

ISR (TIMER1_COMPB_vect) // void timer1_compb_isr(void)
{
switch(i)
{
case 0:
 OCR1BH=m0h;     
 OCR1BL=m0l;     
 PORTC=0X01;
 i=1;
 break ;  
 
case 1:
 OCR1BH=m1h;       
 OCR1BL=m1l;      
 PORTC=0X02;
 i=2;
 break ; 
 
case 2:
 OCR1BH=m2h;              
 OCR1BL=m2l;                
 PORTC=0X04;
 i=3;
 break ;
 
case 3:
 OCR1BH=m3h;   
 OCR1BL=m3l;      
 PORTC=0X08;
 i=0;
 break ; 
 } 
}

void deg(float value,int m)
{
        unsigned int va;
        va=5060+value*20.411;
        if(m==2)              
        {
                m1l=va;
                temp=va>>8;
                m1h=temp;
        }
        if(m==3)            
        {
                m2l=va;
                temp=va>>8;
                m2h=temp;
        }
        if(m==0)       
        {
                m3l=va;
                temp=va>>8;
                m3h=temp;
        }
        if(m==1)     
        {
                m0l=va;
                temp=va>>8;
                m0h=temp;
        }
}


int main()
{ 
int i;

PORTD=0xFF;
DDRD=0x00;
PORTC=0x00;
DDRC=0xFF;


// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: Timer 1 Stopped
// Mode: Normal top=FFFFh
// OC1A output: Discon.
// OC1B output: Discon.
TCCR1A=0x03;
TCCR1B=0x1A;
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x27;
OCR1AL=0x0F;
OCR1BH=0x1D;
OCR1BL=0x4B;
TIMSK=0x18;  // Timer(s)/Counter(s) Interrupt(s) initialization

sei();       //to set global interrupts enable

m0h=0x1A;
m0l=0xF1;

m1h=0x1A;
m1l=0xF1;

m2h=0x1A;
m2l=0xF1;

m3h=0x1A;
m3l=0xF1; 
deg(llegb,0);
deg(rlegb,1);
deg(clegl,2);
while (1)
      {
       if((PIND&0x01)==0x00)		//if sw1 is pressed //rotate the servo to minimum degree
	 
		{
		  deg(0,0);
		  deg(0,1);
		  deg(0,2);
		  deg(0,3);
		  
		}	 
	
	   if((PIND&0x02)==0x00)		//if sw2 is pressed //rotate the servo to maximum degree
		{
		  deg(60,0);
		  deg(60,1);
		  deg(60,2);
		  deg(60,3);
		}	 
	 
	   if((PIND&0x04)==0x00)			//if sw2 is pressed //rotate the servo to middle degree
		{
		  deg(90,0);
		  deg(90,1);
		  deg(90,2);
		  deg(90,3);
		}	  
	}   
}
