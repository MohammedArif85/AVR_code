/*SPI master to interface CC2500 with atmega 16
PORTS USED																	*
*																				*	
*	CC2500……………….(RF trans receiver module)										*
*	PB3-MOSI																	*
*	PB4-GET_MISO_HIGH																	*
*	PB5-SCK																		*	
*	PD1-SS'																		*
*	PD2-GD0																		*		
*	PD3-GD2																				*
*********************************************************************************/

#include<stdlib.h>
#include<avr/interrupt.h>
#include<compat/deprecated.h>
#include"cc2500.h"


#define SELF_ADDR 0x02
#define SELF_CHANNEL 0X03
 
void writefifo(uint8_t *,uint8_t);				//for initialization of rf device
void readfifo(uint8_t *);
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

//uint8_t txdata[60]="jabalpur city is beautiful city with good tech skills";
uint8_t txdata[60]="jangan man adhi nayak jaya hai bharat bhagya vidhata";
uint8_t rxdata[64];





ISR (INT2_vect) 
{ 
      rf_receive();
}





void inline rf_init(void) 
{
 spi_master_init();
 uart_init(9600);
 cc_reset();
 cc_config();
 Wait(100);   
 cc_strobe(CC2500_SRX); //receive mode
 MCUCSR=0x40;
 GICR=0x20;
 sei();	 
}


void  writefifo(uint8_t *buf,uint8_t len)
{
	cc_writereg(CC2500_TXFIFO,len);
	cc_writereg(CC2500_TXFIFO,SELF_ADDR);
	cc_writeregbust(CC2500_TXFIFO,buf,60);
}


void inline readfifo(uint8_t *buf)
{
	length=0;
	length=cc_readreg(CC2500_RXFIFO);
	cc_readregbust(CC2500_RXFIFO,buf,length);
	
}

void  rf_transmit()//**************************************************************************2
{
 cli();
 cc_strobe(CC2500_SIDLE); //ideale mode
 writefifo(txdata,64);
 cc_strobe(CC2500_STX);	 //transmitter mode
 while(GET_GDO2_LOW);
 while(GET_GDO2_HIGH);
 cc_strobe(CC2500_SFRX);  //flux rxfifo
 Wait(1);
 cc_strobe(CC2500_SFTX);  //flux txfifo
 Wait(1);
 cc_strobe(CC2500_SIDLE); //ideale mode
 cc_strobe(CC2500_SRX);	//receive mode
 Wait(1); 
 sei();
}

void inline rf_receive() //**************************************************************************3
{
 readfifo(rxdata);
 uart_puts("receive>>>");
 uart_puts(rxdata);
  cc_strobe(CC2500_SFRX);  //flux rxfifo
 Wait(1);
 cc_strobe(CC2500_SIDLE);  //IDLE MODE
 Wait(1);
 cc_strobe(CC2500_SRX);	//receive mode
 Wait(1);	
}

void inline spi_master_init(void)//******************************************************************4
{
  CC2500_DDR=0xB0;				//spi pins (mosi, sck, ss) configure as output  important ss must shod be configured  even if it is not in use
  CC2500_PORT=0X4F;
  SPCR = 0x5D;				//spe=1(spi enable),DDRD=1(MSB first),mstr=1(master)
  SPSR|=_BV(SPI2X);
}

void inline cc_reset(void)//*************************************************************************5
{
	CS_HIGH; 
    Wait(1); 
    CS_LOW; 
    Wait(1); 
    CS_HIGH; 
    Wait(41);
	CS_LOW;
    while (GET_MISO_HIGH);
    SPDR = CC2500_SRES; 
    while((SPSR&0x80)==0);
	while (GET_MISO_HIGH);
    CS_HIGH; 
}

void cc_config(void) //***********************************************************************6
{
 cc_writereg(CC2500_FSCTRL1,0x07);
 cc_writereg(CC2500_FSCTRL0,0x00);
 cc_writereg(CC2500_FREQ2,0x5D);
 cc_writereg(CC2500_FREQ1,0x93);
 cc_writereg(CC2500_FREQ0,0xB1);//93
 
 cc_writereg(CC2500_MDMCFG4,0x2D);
 cc_writereg(CC2500_MDMCFG3,0x3B);
 cc_writereg(CC2500_MDMCFG2,0x73);
 cc_writereg(CC2500_MDMCFG1,0x22);
 cc_writereg(CC2500_MDMCFG0,0xF8);
 
 cc_writereg(CC2500_CHANNR,SELF_CHANNEL);//Channel No. 
 
 cc_writereg(CC2500_DEVIATN,0x00);//
 
 cc_writereg(CC2500_FREND1,0xB6);
 cc_writereg(CC2500_FREND0,0x10);
 
 cc_writereg(CC2500_MCSM0,0x18);
 cc_writereg(CC2500_FOCCFG,0x1D);
 
 cc_writereg(CC2500_BSCFG,0x1C);
 cc_writereg(CC2500_AGCCTRL2,0xC7);
 cc_writereg(CC2500_AGCCTRL1,0x00);
 cc_writereg(CC2500_AGCCTRL0,0xB2);//f8
 
 cc_writereg(CC2500_FSCAL3,0xEA);
 cc_writereg(CC2500_FSCAL2,0x0A);
 cc_writereg(CC2500_FSCAL1,0x00);
 cc_writereg(CC2500_FSCAL0,0x11);
 cc_writereg(CC2500_FSTEST,0x59);
 
 cc_writereg(CC2500_TEST2,0x88);
 cc_writereg(CC2500_TEST1,0x31);
 cc_writereg(CC2500_TEST0,0x0B);
 
 cc_writereg(CC2500_IOCFG2,0x06);  //iocfg2
 
 cc_writereg(CC2500_IOCFG0,0x06);  //iocfg0
 
 cc_writereg(CC2500_PKTCTRL1,0x0D); //pktctrl1
 cc_writereg(CC2500_PKTCTRL0,0x45);  //pktctrl 0 
 cc_writereg(CC2500_ADDR,SELF_ADDR);//device address
 cc_writereg(CC2500_PKTLEN,0xFF);
 
 //sync bits
 cc_writereg(CC2500_SYNC1,0x12);
 cc_writereg(CC2500_SYNC0,0x34);
 
 
 
 Wait(10);
 cc_strobe(CC2500_SFRX);  //flux rxfifo
 Wait(1);
 cc_strobe(CC2500_SFTX);  //flux txfifo
 Wait(1);
 cc_strobe(CC2500_SIDLE); //ideale mode
 
}

uint8_t cc_readreg(uint8_t add)//*******************************************************************7
{
  CS_LOW;
  while(GET_MISO_HIGH);
  SPDR=(add|CC2500_READ_SINGLE);
  Wait(1);	
  while((SPSR&0x80)==0);			//pull the ss line to down to activate slave 
  SPDR=0;							//place data on spi resistor
  while((SPSR&0x80)==0);			//wait foe the spi transmittion complit
  CS_HIGH;								//pull the ss line to up to deactivate the slave
  return SPDR;
}


void cc_readregbust(uint8_t add,uint8_t *dat,uint8_t leng)//*******************************************************************7
{
  CS_LOW;
  while(GET_MISO_HIGH);
  SPDR=(add|CC2500_READ_BURST);
  Wait(1);	
  while((SPSR&0x80)==0);			//pull the ss line to down to activate slave 
  
  while(leng--)
  {
  SPDR=0;							//place data on spi resistor
  while((SPSR&0x80)==0);			//wait foe the spi transmittion complit
  *dat=SPDR;
  dat++;
  }
  
  CS_HIGH;								//pull the ss line to up to deactivate the slave
  
}

void cc_writereg(uint8_t add,uint8_t data)//********************************************************8
{
  CS_LOW;
  while(GET_MISO_HIGH);
  SPDR=add;
  while((SPSR&0x80)==0);			//pull the ss line to down to activate slave 
  SPDR=data;				//place data on spi resistor
  while((SPSR&0x80)==0);	//wait foe the spi transmittion complit
  CS_HIGH;
  Wait(1);
}

void cc_writeregbust(uint8_t addr,uint8_t* data,uint8_t lengt)
{
  CS_LOW;
  while(GET_MISO_HIGH);
  SPDR=addr|CC2500_WRITE_BURST;
  while((SPSR&0x80)==0);			//pull the ss line to down to activate slave 
  while(lengt--)
  {
	  SPDR=*data;				//place data on spi resistor
	  while((SPSR&0x80)==0);	//wait foe the spi transmittion complit
	  data++;
  }
  CS_HIGH;
  
}

uint8_t cc_strobe(uint8_t data) //***********************************************************************9
{
  CS_LOW;								//pull the ss line to down to activate slave 
  while(GET_MISO_HIGH);
  SPDR=data;						//place data on spi resistor
  while((SPSR&0x80)==0);			//wait foe the spi transmittion complit
  CS_HIGH;								//pull the ss line to up to deactivate the slave
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

