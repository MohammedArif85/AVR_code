/***********************************************************************************
    Filename: rf.h

    Copyright 2007 Texas Instruments, Inc.
***********************************************************************************/

#ifndef RF_H
#define RF_H

#include <stdint.h>

//----------------------------------------------------------------------------------
// Type declarations
//----------------------------------------------------------------------------------

// The following structure can be used when configuring the RF chip. SmartRF Studio
// can be used to export a predefined instance of this structure.
typedef struct {
    uint8_t fsctrl1;    // Frequency synthesizer control.
    uint8_t fsctrl0;    // Frequency synthesizer control.
    uint8_t freq2;      // Frequency control word, high byte.
    uint8_t freq1;      // Frequency control word, middle byte.
    uint8_t freq0;      // Frequency control word, low byte.
    uint8_t mdmcfg4;    // Modem configuration.
    uint8_t mdmcfg3;    // Modem configuration.
    uint8_t mdmcfg2;    // Modem configuration.
    uint8_t mdmcfg1;    // Modem configuration.
    uint8_t mdmcfg0;    // Modem configuration.
    uint8_t channr;     // Channel number.
    uint8_t deviatn;    // Modem deviation setting (when FSK modulation is enabled).
    uint8_t frend1;     // Front end RX configuration.
    uint8_t frend0;     // Front end RX configuration.
    uint8_t mcsm0;      // Main Radio Control State Machine configuration.
    uint8_t foccfg;     // Frequency Offset Compensation Configuration.
    uint8_t bscfg;      // Bit synchronization Configuration.
    uint8_t agcctrl2;   // AGC control.
    uint8_t agcctrl1;   // AGC control.
    uint8_t agcctrl0;   // AGC control.
    uint8_t fscal3;     // Frequency synthesizer calibration.
    uint8_t fscal2;     // Frequency synthesizer calibration.
    uint8_t fscal1;     // Frequency synthesizer calibration.
    uint8_t fscal0;     // Frequency synthesizer calibration.
    uint8_t fstest;     // Frequency synthesizer calibration.
    uint8_t test2;      // Various test settings.
    uint8_t test1;      // Various test settings.
    uint8_t test0;      // Various test settings.
    uint8_t iocfg2;     // GDO2 output pin configuration.
    uint8_t iocfg0;     // GDO0 output pin configuration.
    uint8_t pktctrl1;   // Packet automation control.
    uint8_t pktctrl0;   // Packet automation control.
    uint8_t addr;       // Device address.
    uint8_t pktlen;     // Packet length.
} RF_CONFIG;

// For rapid chip configuration with a minimum of function overhead.
// The array has to be set up with predefined values for all registers.
typedef uint8_t RF_BURST_CONFIG[47];

// The chip status byte, returned by chip for all SPI accesses
typedef uint8_t RF_STATUS;



//----------------------------------------------------------------------------------
// Function declarations
//----------------------------------------------------------------------------------

void  RfConfig(const RF_CONFIG* ,const uint8_t* ,uint8_t);
void  RfBurstConfig(const RF_BURST_CONFIG , const uint8_t* , uint8_t);
void  RfResetChip(void);
uint8_t RfGetChipId(void);
uint8_t RfGetChipVer(void);
uint8_t RfReadStatusReg(uint8_t);
uint8_t RfReadReg(uint8_t);

RF_STATUS RfWriteReg(uint8_t,uint8_t);
RF_STATUS RfWriteFifo(const uint8_t* ,uint8_t );
RF_STATUS RfReadFifo(uint8_t* ,uint8_t);
RF_STATUS RfStrobe(uint8_t);
RF_STATUS RfGetTxStatus(void);
RF_STATUS RfGetRxStatus(void);
void Rf_tx_data(uint8_t);
void Rf_rx_data(void);


/**********************************************************************************/
#endif
