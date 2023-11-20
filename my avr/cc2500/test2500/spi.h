/******************************************************************************
    Filename: spi.h

    Copyright 2007 Texas Instruments, Inc.
******************************************************************************/

#ifndef SPI_H
#define SPI_H

#include <avr/io.h>
#include <stdint.h>
//----------------------------------------------------------------------------------
//   Generic SPI plugin module. Supports all known MSP430 SPI interfaces.
//----------------------------------------------------------------------------------




//----------------------------------------------------------------------------------
//   Common Macros
//----------------------------------------------------------------------------------
#define SPI_BEGIN        sbi(SPCR,SPE)
#define SPI_END          cbi(SPCR,SPE)


//----------------------------------------------------------------------------------
//  SPI
//----------------------------------------------------------------------------------

#define SPI_WAIT_RXFIN   while((SPSR&0x80)==0);
#define SPI_WAIT_TXFIN   while((SPSR&0x80)==0);
#define SPI_RXBUF        SPDR
#define SPI_TXBUF        SPDR
#define SPI_TXBUF_SET(x) SPI_TXBUF = (x)

void Init_spi(void);
uint8_t SpiWrite(uint8_t ,const uint8_t* ,uint16_t);
uint8_t SpiRead(uint8_t,uint8_t*,uint16_t);
uint8_t SpiStrobe(uint8_t);


#endif
