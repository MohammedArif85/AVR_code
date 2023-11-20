#include<avr/io.h>
#include<avr/interrupt.h>
#include<compat/deprecated.h>
#include<util/delay.h>
#include"cc2500.h"
#include"cc2500.hpp"
#include"spi.h"
#include"my_rfsetting.h"
#include"uart.h"







int main(void)
{
	uint8_t i,data;
	
	Init_spi();
	uart_init(9600);
	DDRA=0xFF;
	PORTA=0X00;
	RfResetChip();
	
	RfConfig(&myRfConfig, myPaTable, myPaTableLen);
	_delay_ms(1000);
	PORTA=0XFF;
	_delay_ms(1000);
	
	PORTA=0X00;
	uart_tx_char('k');
	uart_tx_char('e');
	uart_tx_char('t');
	uart_tx_char('a');
	uart_tx_char('n');
	
	while(1)
	{
	
		data=RfStrobe(0x3D);
		uart_tx_char(data);
		_delay_ms(1000);
		
		_delay_ms(1000);
		_delay_ms(1000);
		_delay_ms(1000);
	
	}
	
	return 0;
}
