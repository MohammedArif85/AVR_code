#include<avr/io.h>
#include<stdint.h>
#include<compat/deprecated.h>
#include"spi.h"


void Init_spi(void) 
{
	SPCR=0X1D;
	SPSR=0X00;
}



//----------------------------------------------------------------------------------
//  void SpiWrite(uint8_t addr, const uint8_t *buffer, uint16_t length)
//
//  DESCRIPTION:
//    Write data to device, starting at internal device address "addr".
//    The device will increment the address internally for every new byte
//    that is written. For single byte write, set length to 1.
//----------------------------------------------------------------------------------
uint8_t SpiWrite(uint8_t addr, const uint8_t* data, uint16_t length)
{
    uint16_t i;
    uint8_t rc;

    SPI_BEGIN;
    SPI_TXBUF_SET(addr);
    SPI_WAIT_TXFIN;
    rc = SPI_RXBUF;
    for (i = 0; i < length; i++)
    {
      SPI_TXBUF_SET(data[i]);
      SPI_WAIT_TXFIN;
    }

    SPI_END;
    return(rc);

}

//----------------------------------------------------------------------------------
//  uint8_t SpiRead(uint8_t addr, uint8_t* data, uint16_t length)
//
//  DESCRIPTION:
//    Read data from device, starting at internal device address "addr".
//    The device will increment the address internally for every new byte
//    that is read. Note that the master device needs to write a dummy byte
//    (in this case 0) for every new byte in order to generate the clock to
//    clock out the data. For single byte read, set length to 1.
//----------------------------------------------------------------------------------
uint8_t SpiRead(uint8_t addr, uint8_t* data, uint16_t length)
{
    uint16_t i;
    uint8_t rc;

    SPI_BEGIN;

    SPI_TXBUF_SET(addr);
    SPI_WAIT_TXFIN;
    rc = SPI_RXBUF;
    for (i = 0; i < length; i++)
    {
        SPI_TXBUF_SET(0);        // Dummy write to read data byte
        SPI_WAIT_TXFIN;
        data[i] = SPI_RXBUF;     // Store data from last data RX
    }

    SPI_END;
    return(rc);
}


//----------------------------------------------------------------------------------
//  uint8_t SpiStrobe(uint8_t cmd)
//
//  DESCRIPTION:
//    Special write function, writing only one byte (cmd) to the device.
//----------------------------------------------------------------------------------
uint8_t SpiStrobe(uint8_t cmd)
{
    uint8_t rc;

    SPI_BEGIN;
    SPI_TXBUF_SET(cmd);
    SPI_WAIT_TXFIN;
    rc = SPI_RXBUF;
    rc = SPI_RXBUF;
    SPI_END;
    return(rc);
}

