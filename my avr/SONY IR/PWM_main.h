//***********************************************************************
//   **************  HEADER FILE FOR PWM_MAIN  *****************
//***********************************************************************


#define ENABLE_INT0							GICR |= 0x40
#define DISABLE_INT0						GICR &= ~0x40



void port_init(void);
void timer0_init(void);
unsigned int read_IR (void);
void motorControl (unsigned char code, unsigned char address);
void init_devices(void);
