#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include"spi.h"
#include"cc2500.hpp"
#include"cc2500.h"

 
volatile uint8_t data;



void rf_init(void)
{
	CC2500_DDR=(_BV(SCK)|_BV(MOSI)|_BV(CS));
	CC2500_PORT=(_BV(MISO)|_BV(GDO2));

	

}


//----------------------------------------------------------------------------------
//  void RfResetChip(void)
//
//  DESCRIPTION:
//    Resets the chip using the procedure described in the datasheet.
//----------------------------------------------------------------------------------
void RfResetChip(void)
{
	rf_init();
	CS_HIGH;
	_delay_us(5);
	SCK_LOW;
	_delay_us(5);
	MOSI_LOW;
	_delay_us(5);
    // Toggle chip select signal
    CS_LOW;
    _delay_us(30);
    CS_HIGH;
    _delay_us(30);
    CS_LOW;
    _delay_us(45);

    // Send SRES command
    CS_HIGH;
    while(GET_MISO_HIGH);
    RfStrobe(CC2500_SRES);
    // Wait for chip to finish internal reset
    while(GET_MISO_HIGH);
    CS_LOW;
}

//----------------------------------------------------------------------------------
//  void RfConfig(const RF_CONFIG* rfConfig, const uint8_t* rfPaTable, uint8_t rfPaTableLen)
//
//  DESCRIPTION:
//    Used to configure the CC1100/CC2500 registers with exported register
//    settings from SmartRF Studio.
//
//  ARGUMENTS:
//    rfConfig     - register settings (as exported from SmartRF Studio)
//    rfPaTable    - array of PA table values (from SmartRF Studio)
//    rfPaTableLen - length of PA table
//
//----------------------------------------------------------------------------------
void RfConfig(const RF_CONFIG* rfConfig, const uint8_t* rfPaTable, uint8_t rfPaTableLen)
{  RfWriteReg(CC2500_FSCTRL1,  rfConfig->fsctrl1);    // Frequency synthesizer control.
    RfWriteReg(CC2500_FSCTRL0,  rfConfig->fsctrl0);    // Frequency synthesizer control.
    RfWriteReg(CC2500_FREQ2,    rfConfig->freq2);      // Frequency control word, high byte.
    RfWriteReg(CC2500_FREQ1,    rfConfig->freq1);      // Frequency control word, middle byte.
    RfWriteReg(CC2500_FREQ0,    rfConfig->freq0);      // Frequency control word, low byte.
    RfWriteReg(CC2500_MDMCFG4,  rfConfig->mdmcfg4);    // Modem configuration.
    RfWriteReg(CC2500_MDMCFG3,  rfConfig->mdmcfg3);    // Modem configuration.
    RfWriteReg(CC2500_MDMCFG2,  rfConfig->mdmcfg2);    // Modem configuration.
    RfWriteReg(CC2500_MDMCFG1,  rfConfig->mdmcfg1);    // Modem configuration.
    RfWriteReg(CC2500_MDMCFG0,  rfConfig->mdmcfg0);    // Modem configuration.
    RfWriteReg(CC2500_CHANNR,   rfConfig->channr);     // Channel number.
    RfWriteReg(CC2500_DEVIATN,  rfConfig->deviatn);    // Modem deviation setting (when FSK modulation is enabled).
    RfWriteReg(CC2500_FREND1,   rfConfig->frend1);     // Front end RX configuration.
    RfWriteReg(CC2500_FREND0,   rfConfig->frend0);     // Front end RX configuration.
    RfWriteReg(CC2500_MCSM0,    rfConfig->mcsm0);      // Main Radio Control State Machine configuration.
    RfWriteReg(CC2500_FOCCFG,   rfConfig->foccfg);     // Frequency Offset Compensation Configuration.
    RfWriteReg(CC2500_BSCFG,    rfConfig->bscfg);      // Bit synchronization Configuration.
    RfWriteReg(CC2500_AGCCTRL2, rfConfig->agcctrl2);   // AGC control.
    RfWriteReg(CC2500_AGCCTRL1, rfConfig->agcctrl1);   // AGC control.
    RfWriteReg(CC2500_AGCCTRL0, rfConfig->agcctrl0);   // AGC control.
    RfWriteReg(CC2500_FSCAL3,   rfConfig->fscal3);     // Frequency synthesizer calibration.
    RfWriteReg(CC2500_FSCAL2,   rfConfig->fscal2);     // Frequency synthesizer calibration.
    RfWriteReg(CC2500_FSCAL1,   rfConfig->fscal1);     // Frequency synthesizer calibration.
    RfWriteReg(CC2500_FSCAL0,   rfConfig->fscal0);     // Frequency synthesizer calibration.
    RfWriteReg(CC2500_FSTEST,   rfConfig->fstest);     // Frequency synthesizer calibration.
    RfWriteReg(CC2500_TEST2,    rfConfig->test2);      // Various test settings.
    RfWriteReg(CC2500_TEST1,    rfConfig->test1);      // Various test settings.
    RfWriteReg(CC2500_TEST0,    rfConfig->test0);      // Various test settings.
    RfWriteReg(CC2500_IOCFG2,   rfConfig->iocfg2);     // GDO2 output pin configuration.
    RfWriteReg(CC2500_IOCFG0,   rfConfig->iocfg0);     // GDO0 output pin configuration.
    RfWriteReg(CC2500_PKTCTRL1, rfConfig->pktctrl1);   // Packet automation control.
    RfWriteReg(CC2500_PKTCTRL0, rfConfig->pktctrl0);   // Packet automation control.
    RfWriteReg(CC2500_ADDR,     rfConfig->addr);       // Device address.
    RfWriteReg(CC2500_PKTLEN,   rfConfig->pktlen);     // Packet length.
	CS_HIGH;
	while(GET_MISO_HIGH);
    SpiWrite(CC2500_PATABLE | CC2500_WRITE_BURST, rfPaTable, rfPaTableLen);
	 CS_LOW;
	 RfStrobe(CC2500_SRX); //receiver mode
	
}


//----------------------------------------------------------------------------------
//  void  RfBurstConfig(const RF_BURST_CONFIG rfConfig, const uint8_t* rfPaTable, uint8_t rfPaTableLen)
//
//  DESCRIPTION:
//    Used to configure all of the CC1100/CC2500 registers in one burst write.
//
//  ARGUMENTS:
//    rfConfig     - register settings
//    rfPaTable    - array of PA table values (from SmartRF Studio)
//    rfPaTableLen - length of PA table
//
//----------------------------------------------------------------------------------
void  RfBurstConfig(const RF_BURST_CONFIG rfConfig, const uint8_t* rfPaTable, uint8_t rfPaTableLen)
{
    CS_HIGH;
	while(GET_MISO_HIGH);
	SpiWrite(CC2500_IOCFG2  | CC2500_WRITE_BURST, rfConfig, sizeof(rfConfig));
    SpiWrite(CC2500_PATABLE | CC2500_WRITE_BURST, rfPaTable, rfPaTableLen);
	CS_LOW;
}

//----------------------------------------------------------------------------------
//  uint8_t RfGetChipId(void)
//----------------------------------------------------------------------------------
uint8_t RfGetChipId(void)
{
    return(RfReadStatusReg(CC2500_PARTNUM));
}

//----------------------------------------------------------------------------------
//  uint8_t RfGetChipVer(void)
//----------------------------------------------------------------------------------
uint8_t RfGetChipVer(void)
{
    return(RfReadStatusReg(CC2500_VERSION));
}

//----------------------------------------------------------------------------------
//  RF_STATUS RfStrobe(uint8_t cmd)
//----------------------------------------------------------------------------------
RF_STATUS RfStrobe(uint8_t cmd)
{
	uint8_t rc;
	CS_HIGH;
	while(GET_MISO_HIGH);
	rc=SpiStrobe(cmd);
	CS_LOW;
    return(rc);
}

//----------------------------------------------------------------------------------
//  uint8_t RfReadStatusReg(uint8_t addr)
//
//  NOTE:
//      When reading a status register over the SPI interface while the register
//      is updated by the radio hardware, there is a small, but finite, probability
//      that the result is corrupt. The CC1100 and CC2500 errata notes explain the
//      problem and propose several workarounds.
//
//----------------------------------------------------------------------------------
uint8_t RfReadStatusReg(uint8_t addr)
{
    uint8_t reg;
	CS_HIGH;
	while(GET_MISO_HIGH);
    SpiRead(addr | CC2500_READ_BURST, &reg, 1);
	CS_LOW;
    return(reg);
}

//----------------------------------------------------------------------------------
//  uint8_t RfReadReg(uint8_t addr)
//----------------------------------------------------------------------------------
uint8_t RfReadReg(uint8_t addr)
{
    uint8_t reg;
	CS_HIGH;
	while(GET_MISO_HIGH);
    SpiRead(addr | CC2500_READ_SINGLE, &reg, 1);
	CS_LOW;
    return(reg);
}

//----------------------------------------------------------------------------------
//  RF_STATUS RfWriteReg(uint8_t addr, uint8_t data)
//----------------------------------------------------------------------------------
RF_STATUS RfWriteReg(uint8_t addr, uint8_t data)
{
    uint8_t rc;
	CS_HIGH;
	while(GET_MISO_HIGH);
    rc = SpiWrite(addr, &data, 1);
	CS_LOW;
    return(rc);
}

//----------------------------------------------------------------------------------
//  RF_STATUS RfWriteFifo(uint8_t* data, uint8_t length)
//----------------------------------------------------------------------------------
RF_STATUS RfWriteFifo(const uint8_t* data, uint8_t length)
{
    uint8_t rc;
	CS_HIGH;
	while(GET_MISO_HIGH);
	rc=SpiWrite(CC2500_TXFIFO | CC2500_WRITE_BURST, data, length);
	CS_LOW;
	return(rc);
	
}

//----------------------------------------------------------------------------------
//  RF_STATUS RfReadFifo(uint8_t* data, uint8_t length)
//----------------------------------------------------------------------------------
RF_STATUS RfReadFifo(uint8_t* data, uint8_t length)
{
	uint8_t rc;
	CS_HIGH;
	while(GET_MISO_HIGH);
	rc=SpiRead(CC2500_RXFIFO | CC2500_READ_BURST, data, length);
	CS_LOW;
	return(rc);
}

//----------------------------------------------------------------------------------
//  uint8_t RfGetTxStatus(void)
//
//  DESCRIPTION:
//      This function transmits a No Operation Strobe (SNOP) to get the status of
//      the radio and the number of free bytes in the TX FIFO
//
//      Status byte:
//
//      ---------------------------------------------------------------------------
//      |          |            |                                                 |
//      | CHIP_RDY | STATE[2:0] | FIFO_BYTES_AVAILABLE (free bytes in the TX FIFO |
//      |          |            |                                                 |
//      ---------------------------------------------------------------------------
//
//  NOTE:
//      When reading a status register over the SPI interface while the register
//      is updated by the radio hardware, there is a small, but finite, probability
//      that the result is corrupt. This also applies to the chip status byte. The
//      CC1100 and CC2500 errata notes explain the problem and propose several
//      workarounds.
//
//----------------------------------------------------------------------------------
RF_STATUS RfGetTxStatus(void)
{
    return(RfReadReg(CC2500_SNOP));
}

//----------------------------------------------------------------------------------
//  uint8_t RfGetRxStatus(void)
//
//  DESCRIPTION:
//      This function transmits a No Operation Strobe (SNOP) with the read bit set
//      to get the status of the radio and the number of available bytes in the RX
//      FIFO.
//
//      Status byte:
//
//      --------------------------------------------------------------------------------
//      |          |            |                                                      |
//      | CHIP_RDY | STATE[2:0] | FIFO_BYTES_AVAILABLE (available bytes in the RX FIFO |
//      |          |            |                                                      |
//      --------------------------------------------------------------------------------
//
//  NOTE:
//      When reading a status register over the SPI interface while the register
//      is updated by the radio hardware, there is a small, but finite, probability
//      that the result is corrupt. This also applies to the chip status byte. The
//      CC1100 and CC2500 errata notes explain the problem and propose several
//      workarounds.
//
//----------------------------------------------------------------------------------
RF_STATUS RfGetRxStatus(void)
{
    return(RfReadReg(CC2500_SNOP | CC2500_READ_SINGLE));
}



void Rf_tx_data(uint8_t data)
{
	
	RfWriteReg(0x3F,data);
	RfStrobe(CC2500_STX);
	while(GET_GDO0_LOW);
	while(GET_GDO0_HIGH);
	RfStrobe(CC2500_SFRX);  //flux rxfifo
	RfStrobe(CC2500_SFTX);  //flux txfifo
	RfStrobe(CC2500_SIDLE); //ideale mode 
	RfStrobe(CC2500_SRX); //receiver mode
	
}
//---------------------------------------------------------------
//
//---------------------------------------------------------------
void Rf_rx_data(void)
{
	
	data=RfReadReg(0x3F);
	if(data==5)
	PORTA=0x0A;
	else
	PORTA=0x05;
	RfStrobe(0x3A);  //flux rxfifo
	RfStrobe(0x36); //ideale mode 
	RfStrobe(0x34);	//receive mode
}

