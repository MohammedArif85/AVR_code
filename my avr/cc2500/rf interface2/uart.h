#ifndef _uart_h_
#define _uart_h_
#include<stdlib.h>
#include<avr/io.h>
#include<compat/deprecated.h>		//HEADER FILE FOR FUNCTIONS LIKE SBI AND CBI

#define UART_BAUD_SELECT (F_CPU/(baud*16l)-1)

#define TX_NEWLINE {uart_tx_char(0x0d); uart_tx_char(0x0a);}





void uart_init(unsigned int baud)
{
 UBRRH=UART_BAUD_SELECT>>8;
 UBRRL=(uint8_t)UART_BAUD_SELECT;
 
 UCSRB=_BV(RXEN)|_BV(TXEN);
 UCSRC=_BV(URSEL)|_BV(UCSZ1)|_BV(UCSZ0);
 
}



void uart_tx_char(uint8_t s)
{
 loop_until_bit_is_set(UCSRA,UDRE);
 UDR=s;
}




unsigned char uart_rx_char(void)
{
 loop_until_bit_is_set(UCSRA,RXC);
 if(UCSRA&(_BV(PE)|_BV(DOR)|_BV(FE)))
 return(unsigned char)-1;
 
 return UDR;
}



void uart_puts(uint8_t *s )
{
    while (*s) 
    uart_tx_char(*s++);

}


#endif 
/* uart.h*/

