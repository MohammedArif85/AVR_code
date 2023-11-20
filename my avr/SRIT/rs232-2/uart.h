//set desired baud rate

#define BAUDRATE 19200

//calculate UBRR value

#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)

void USART_Init()

{

//Set baud rate

UBRRL=UBRRVAL; //low byte

UBRRH=(UBRRVAL>>8); //high byte

//Set data frame format: asynchronous mode,no parity, 1 stop bit, 8 bit size

UCSRC=(1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|

(0<<USBS)|(0<<UCSZ2)|(1<<UCSZ1)|(1<<UCSZ0);

//Enable Transmitter and Receiver

UCSRB=(1<<RXEN)|(1<<TXEN);

}

void rswritechar(unsigned char c)

{


//wait for empty transmit buffer

while (!(UCSRA&(1<<UDRE))){};

//send received data back

UDR=c;

}

unsigned char rsreadchar()
{
   //Wait untill a data is available

   while(!(UCSRA & (1<<RXC)))
   {
      //Do nothing
   }
return UDR;
}

void rsstr(char *s)
{
  while(*s)
  {
  
   rswritechar(*s);
   s++;
  }
}



