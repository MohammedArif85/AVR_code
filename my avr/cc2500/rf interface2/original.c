/*SPI master to interface CC2500 with atmega 16
PORTS USED																	*
*																				*	
*	CC2500……………….(RF trans receiver module)										*
*	PB3-MOSI																	*
*	PB4-MISO																	*
*	PB5-SCK																		*	
*	PD1-SS'																		*
*	PD2-GD0																		*		
*	PD3-GD2																				*
*********************************************************************************/

#include<stdlib.h>
#include<avr/interrupt.h>
#include<compat/deprecated.h>
#include"uart.h"

#define csn1 sbi(PORTB,4)//(PORTB|=0x10)
#define csn0 cbi(PORTB,4)//(PORTB&=0xEF)

#define miso (bit_is_set(PINB,6))

#define gd0 (bit_is_set(PINB,2))
#define gd2 (bit_is_set(PINB,1))

 uint8_t data1[60]={"ketankotharicc2500xshaherkkufghjklomnhybctgfhjkloplmkiuytr"};
 //uint8_t data1[60]={"jabalpur hamara shaher haiasdfghjklomnhybctgfhjkloplmkiuytr"};
 uint8_t data2[64];
void rf_init(void);
void writefifo(void);				//for initialization of rf device
void readfifo(void);
void rf_transmit(void);

void rf_receive(void);
void spi_master_init(void);
void cc_reset(void);
void cc_config(void);
uint8_t cc_readreg(uint8_t);
void cc_writereg(uint8_t,uint8_t);
void cc_readregbust(uint8_t,uint8_t*,uint8_t);
void cc_writeregbust(uint8_t,uint8_t*,uint8_t);
uint8_t cc_strobe(uint8_t);
void Wait(uint8_t);


volatile uint8_t length,address;
uint8_t i;

ISR (INT2_vect) //****************************************************************************0
{ 
  
     rf_receive();
  
   
}

void inline rf_init(void) //*****************************************************************************1
{
 spi_master_init();
 uart_init(9600);
 cc_reset();
 cc_config();
 Wait(100);   
 cc_strobe(0x34); //receive mode
 MCUCSR=0x40;
 GICR=0x20;
 sei();	 
}
void inline writefifo()
{
	cc_writereg(0x3f,64);
	cc_writereg(0x3f,0x02);
	cc_writeregbust(0x3f,data1,60);
	//uart_puts("size of data");
	//uart_tx_char(sizeof(data1));
}


void inline readfifo()
{
	length=0;
	for(i=0;i<64;i++)
	{data2[i]=0;}
	length=cc_readreg(0x3f);
	cc_readregbust(0x3f,data2,length);
	uart_puts("Recieved Data");
	uart_puts(data2);
	
	
}

void inline rf_transmit()//uint8_t* data,uint16_t length)//**************************************************************************2
{
 cli();
 writefifo();
 //while((cc_readreg(0xf8)&0x10)!=1);//check for channel to be clear for tx
 cc_strobe(0x35);	 //transmitter mode
 while(!gd2);
 while(gd2);
 cc_strobe(0x3A);  //flux rxfifo
 cc_strobe(0x3B);  //flux txfifo
 Wait(1);
 cc_strobe(0x36); //ideale mode 
 cc_strobe(0x34); //receiver mode
 Wait(1); 
 sei();
}

void inline rf_receive() //**************************************************************************3
{
 readfifo();
  cc_strobe(0x3A);  //flux rxfifo
 Wait(1);
 cc_strobe(0x36); //ideale mode 
 Wait(1);
 cc_strobe(0x34);	//receive mode
 Wait(1);	
}

void inline spi_master_init(void)//******************************************************************4
{
  DDRB=0xB0;				//spi pins (mosi, sck, ss) configure as output  important ss must shod be configured  even if it is not in use
  PORTB=0X4F;
  SPCR = 0x5D;				//spe=1(spi enable),DDRD=1(MSB first),mstr=1(master)
  SPSR|=_BV(SPI2X);
}

void inline cc_reset(void)//*************************************************************************5
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
 
 cc_writereg(0x07,0x0D); //pktctrl1
 cc_writereg(0x08,0x45);  //pktctrl 0 
 cc_writereg(0x09,0x02);//device address
 cc_writereg(0x06,0xFF);
 
 //sync bits
 cc_writereg(0x04,0x12);
 cc_writereg(0x05,0x34);
 Wait(10);
 cc_strobe(0x3A);  //flux rxfifo
 Wait(1);
 cc_strobe(0x3B);  //flux txfifo
 Wait(1);
 cc_strobe(0x36); //ideale mode
 
}

uint8_t cc_readreg(uint8_t add)//*******************************************************************7
{
  csn0;
  while(miso);
  SPDR=(add|0x80);
  Wait(1);	
  while((SPSR&0x80)==0);			//pull the ss line to down to activate slave 
  SPDR=0;							//place data on spi resistor
  while((SPSR&0x80)==0);			//wait foe the spi transmittion complit
  csn1;								//pull the ss line to up to deactivate the slave
  return SPDR;
}


void cc_readregbust(uint8_t add,uint8_t *dat,uint8_t leng)//*******************************************************************7
{
  csn0;
  while(miso);
  SPDR=(add|0xc0);
  Wait(1);	
  while((SPSR&0x80)==0);			//pull the ss line to down to activate slave 
  
  while(leng--)
  {
  SPDR=0;							//place data on spi resistor
  while((SPSR&0x80)==0);			//wait foe the spi transmittion complit
  *dat=SPDR;
  dat++;
  }
  
  csn1;								//pull the ss line to up to deactivate the slave
  
}

void cc_writereg(uint8_t add,uint8_t data)//********************************************************8
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

void cc_writeregbust(uint8_t addr,uint8_t* data,uint8_t lengt)
{
  csn0;
  while(miso);
  SPDR=addr|0x40;
  while((SPSR&0x80)==0);			//pull the ss line to down to activate slave 
  while(lengt--)
  {
	  SPDR=*data;				//place data on spi resistor
	  while((SPSR&0x80)==0);	//wait foe the spi transmittion complit
	  data++;
  }
  csn1;
  
}

uint8_t cc_strobe(uint8_t data) //***********************************************************************9
{
  csn0;								//pull the ss line to down to activate slave 
  while(miso);
  SPDR=data;						//place data on spi resistor
  while((SPSR&0x80)==0);			//wait foe the spi transmittion complit
  csn1;								//pull the ss line to up to deactivate the slave
  return SPDR;
}

void Wait(uint8_t timeout) //*************************************************************************10
{
	// This sequence uses exactly 8 clock cycle for each round
    do {
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
		
	} while (--timeout);
}

