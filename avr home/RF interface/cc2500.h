/*SPI master to interface CC2500 with atmega 8 

*********************************************************************************
*	COPYRIGHT(C)2009 Technophilia												*
*	date:-11 th september 2009													*
*	Author: Prasanta Kumar Nayak												*
*	Organization: Technophilia													*
*	Target system-SWARM devlopment board s/n-98020								*
*	manufacturer: Technophilia													*
*	microcontroller: ATmega-8													*
*	clock: 8mhz internal oscillator												*
*																				*	
*	H fuse=0xD9																	*
*	L fuse=0xE4																	*
*																				*
*	compiler win avr															*		
*																				*
*	PORTS USED																	*
*																				*	
*	CC2500……………….(RF trans receiver module)										*
*	PB3-MOSI																	*
*	PB4-MISO																	*
*	PB5-SCK																		*	
*	PD1-SS'																		*
*	PD2-GD0																		*		
*	PD3-GD2																		*
*																				*
*	LCD……………………….(PCD 8544, 48X84 pixel graphics LCD)							*
*	PB0-SCK																		*
*	PB1-SDIN																	*
*	PB2-D/C																		*
*	PB6-CS																		*		
*	PB7-RESET																	*
*	MOTOR…………………																*
*	D4																			*
*	D5																			*
*	D6																			*
*	D7																			*
*																				*	
*	SENSOR………………																*
*	PC0																			*		
*	PC1																			*
*	PC2																			*
*	PC3																			*
*	PC4																			*
*	PD0																			*	
*																				*
*	SWITCHES……………																*
*	PC5-4 SWITCHES READ THROUGH ADC												*		
*																				*
*	LED………………………																*	
*	LED1-PB0																	*
	LED2-PB1																	*
*	LED3-PB2																	*
*	LED4-PB3																	*
*																				*	
*																				*
*********************************************************************************/
#include<avr/interrupt.h>


#ifndef _CC2500_H_
#define _CC2500_H_

#define csn1 (PORTD|=0x02)
#define csn0 (PORTD&=0xFD)

#define miso (PINB & 0x20)

#define gd0 (PIND & 0x04)

void rf_init();				//for initialization of rf device
void rf_transmit(unsigned char CCdata);

void rf_receive();
void spi_master_init(void);
void cc_reset(void);
void cc_config(void);
void cc_readreg(char add);
void cc_writereg(char add,char data);
void cc_strobe(char data);
void Wait(char timeout);


unsigned char retval,flag,cc_data;

ISR (INT0_vect) //****************************************************************************0
{ 
  if(flag==0)
   {
    // LCD_gotoXY(4,3);
    // LCD_writeString_F ("inter ");
    // LCD_showvalue(count);
     rf_receive();
   }
  else
   {
     flag=0;
   }
   
}

void rf_init() //*****************************************************************************1
{
 spi_master_init();
 cc_reset();
 cc_config();
 Wait(100);   
 cc_strobe(0x34); //receive mode
 MCUCR=0x02;
 GICR=0x40;
 sei();	 
}

void rf_transmit(unsigned char CCdata)//**************************************************************************2
{
 cli();
 flag=1;
 cc_writereg(0x3f,CCdata);
 cc_strobe(0x35);	 //transmitter mode
 while(!gd0);
 while(gd0);
 cc_strobe(0x3A);  //flux rxfifo
 cc_strobe(0x3B);  //flux txfifo
 Wait(1);
 cc_strobe(0x36); //ideale mode 
 cc_strobe(0x34); //receiver mode
 Wait(1); 
 sei();
}

void rf_receive() //**************************************************************************3
{
 cc_readreg(0x3F);
 cc_data=retval;
 if(retval==5)
 sbi(PORTA,5);
 else
 cbi(PORTA,5);
 Wait(5);
 cc_strobe(0x3A);  //flux rxfifo
 Wait(1);
 cc_strobe(0x36); //ideale mode 
 Wait(1);
 cc_strobe(0x34);	//receive mode
 Wait(1);	
}

void spi_master_init(void)//******************************************************************4
{
  DDRD|=0x02;				//spi slave select pin configure as output
  DDRB|=0xF0;				//spi pins (mosi, sck, ss) configure as output  important ss must shod be configured  even if it is not in use
  SPCR = 0x51;				//spe=1(spi enable),DDRD=1(MSB first),mstr=1(master)
}

void cc_reset(void)//*************************************************************************5
{
	csn1; 
    Wait(1); 
    csn0; 
    Wait(1); 
    csn1; 
    Wait(41);
	csn0;
    while (miso);//((PINB&0x08)); 
    SPDR = 0x30; 
    while((SPSR&0x80)==0);			 //SPI_WAIT(); 
	while (miso);//((PINB&0x08)); 
    csn1; 
}

void cc_config(void) //***********************************************************************6
{
 cc_writereg(0X0B,0x07);
 cc_writereg(0x0c,0x00);
 cc_writereg(0x0D,0x5D);
 cc_writereg(0x0E,0x93);
 cc_writereg(0x0F,0xB1);//93
 
 cc_writereg(0x10,0x2D);
 cc_writereg(0x11,0x3B);
 cc_writereg(0x12,0x73);
 cc_writereg(0x13,0x22);
 cc_writereg(0x14,0xF8);
 
 cc_writereg(0x0A,0x00);//Channel No. 
 
 cc_writereg(0x15,0x00);//
 
 cc_writereg(0x21,0xB6);
 cc_writereg(0x22,0x10);
 
 cc_writereg(0x18,0x18);
 cc_writereg(0x19,0x1D);
 
 cc_writereg(0x1A,0x1C);
 cc_writereg(0x1B,0xC7);
 cc_writereg(0x1C,0x00);
 cc_writereg(0x1D,0xB2);//f8
 
 cc_writereg(0x23,0xEA);
 cc_writereg(0x24,0x0A);
 cc_writereg(0x25,0x00);
 cc_writereg(0x26,0x11);
 cc_writereg(0x29,0x59);
 
 cc_writereg(0x2C,0x88);
 cc_writereg(0x2D,0x31);
 cc_writereg(0x2E,0x0B);
 
 cc_writereg(0x00,0x06);  //iocfg2
 
 cc_writereg(0x02,0x06);  //iocfg0
 
 cc_writereg(0x07,0x04);
 cc_writereg(0x08,0x05);  //pktctrl 0 
 cc_writereg(0x09,0x00);//device address
 cc_writereg(0x06,0xFF);
 
 //sync bits
 cc_writereg(0x04,0x12);
 cc_writereg(0x05,0x34);
 
}

void cc_readreg(char add)//*******************************************************************7
{
  csn0;
  while(miso);
  SPDR=(add|0x80);
  Wait(1);	
  while((SPSR&0x80)==0);			//pull the ss line to down to activate slave 
  SPDR=0;							//place data on spi resistor
  while((SPSR&0x80)==0);			//wait foe the spi transmittion complit
  csn1;								//pull the ss line to up to deactivate the slave
  retval = SPDR;
}

void cc_writereg(char add,char data)//********************************************************8
{
  csn0;
  while(miso);
  SPDR=add;
  while((SPSR&0x80)==0);			//pull the ss line to down to activate slave 
  SPDR=data;				//place data on spi resistor
  while((SPSR&0x80)==0);	//wait foe the spi transmittion complit
  csn1;
  Wait(1);
}

cc_strobe(char data) //***********************************************************************9
{
  csn0;								//pull the ss line to down to activate slave 
  while(miso);
  SPDR=data;						//place data on spi resistor
  while((SPSR&0x80)==0);			//wait foe the spi transmittion complit
  csn1;								//pull the ss line to up to deactivate the slave
  retval = SPDR;
}

Wait(char timeout) //*************************************************************************10
{
	// This sequence uses exactly 8 clock cycle for each round
    do {
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
	} while (--timeout);
}

#endif