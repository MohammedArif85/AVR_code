/*	GLCD.h for SWARM
ATMEGA8 8MHZ
Technophilia embedded systems

	This header files have implementation of some common finctions of the LCD.
	Include this header file in your C code. Call the lcd_init() function before
	calling any other functions to initialize the lcd.


	lcd_init() ----- Call the lcd_init() function before calling any other functions
			  to initialize the lcd.
	lcd_cmd() ------ This function is used to give any command instructions to the
			  LCD. For e.g lcd_cmd(0x01) will give the clear command.
	lcd_char() ----- This function will display a single character on the LCD display.
			  For example lcd_char(0x61) will display A. Again lcd_char('b')
			  will display b.
	lcd_string()---- This function will display a string. An example of this would be
			  like lcd_string("This is AVR")
	lcd_showvalue()- This will show a 3-digit decimal value on the LCD. For example if
			  we give lcd_showvalue(0xFF) then 255 will be displayed.
	lcd_gotoxy()--- Will set the cursor at a particular position on of the LCD.
			  So lcd_gotoxy(1,4) will set the cursor at the 2d row 3rd column
			  row and column address are starts from(0,0)
	lcd_clear()---To clear the LCD
	
*/

//PB0-SCE	0x01
//PB1-SCK	0x02
//PB2-MOSI	0x04
//PB4-RST	0x10
//PB5-D/C	0x20


#ifndef _GLCD_H_
#define _GLCD_H_


//#include<avr/io.h>

#define SET_DC_PIN                 PORTB |= 0x04	//portb2  
#define CLEAR_DC_PIN               PORTB &= ~0x04 

#define SET_SCE_PIN                PORTB |= 0x40	//portb6
#define CLEAR_SCE_PIN              PORTB &= ~0x40

#define SET_RST_PIN                PORTB |= 0x80	//port b7
#define CLEAR_RST_PIN              PORTB &= 0x80

#define SET_SCK 				   PORTB |= 0x01  //portb0	
#define CLEAR_SCK				   PORTB &= ~0x01

#define SET_MOSI				   PORTB |= 0x02	//port b1
#define CLEAR_MOSI				   PORTB &= ~0x02 

void spi_init(void);
void LCD_init (unsigned char contrast);
void LCD_writeCommand ( unsigned char command );
void LCD_clear ( void);
void LCD_gotoXY ( unsigned char x, unsigned char y );
void LCD_writeData ( unsigned char Data );
void lcd_write(void);
void LCD_writeChar (unsigned char ch);
void LCD_writeString_F ( const unsigned char *string );
void sendspi(char command);
void delay_ms(unsigned int delaytime);

void LCD_showvalue(unsigned char num);

static const unsigned char smallFont[] =
{
     0x00, 0x00, 0x00, 0x00, 0x00,   // sp
     0x00, 0x00, 0x2f, 0x00, 0x00,    // !
     0x00, 0x07, 0x00, 0x07, 0x00,   // "
     0x14, 0x7f, 0x14, 0x7f, 0x14,   // #
     0x24, 0x2a, 0x7f, 0x2a, 0x12,   // $
     0xc4, 0xc8, 0x10, 0x26, 0x46,   // %
     0x36, 0x49, 0x55, 0x22, 0x50,   // &
     0x00, 0x05, 0x03, 0x00, 0x00,   // '
     0x00, 0x1c, 0x22, 0x41, 0x00,   // (
     0x00, 0x41, 0x22, 0x1c, 0x00,   // )
     0x14, 0x08, 0x3E, 0x08, 0x14,   // *
     0x08, 0x08, 0x3E, 0x08, 0x08,   // +
     0x00, 0x00, 0x50, 0x30, 0x00,   // ,
     0x10, 0x10, 0x10, 0x10, 0x10,   // -
     0x00, 0x60, 0x60, 0x00, 0x00,   // .
     0x20, 0x10, 0x08, 0x04, 0x02,   // /
     0x3E, 0x51, 0x49, 0x45, 0x3E,   // 0
     0x00, 0x42, 0x7F, 0x40, 0x00,   // 1
     0x42, 0x61, 0x51, 0x49, 0x46,   // 2
     0x21, 0x41, 0x45, 0x4B, 0x31,   // 3
     0x18, 0x14, 0x12, 0x7F, 0x10,   // 4
     0x27, 0x45, 0x45, 0x45, 0x39,   // 5
     0x3C, 0x4A, 0x49, 0x49, 0x30,   // 6
     0x01, 0x71, 0x09, 0x05, 0x03,   // 7
     0x36, 0x49, 0x49, 0x49, 0x36,   // 8
     0x06, 0x49, 0x49, 0x29, 0x1E,   // 9
     0x00, 0x36, 0x36, 0x00, 0x00,   // :
     0x00, 0x56, 0x36, 0x00, 0x00,   // ;
     0x08, 0x14, 0x22, 0x41, 0x00,   // <
     0x14, 0x14, 0x14, 0x14, 0x14,   // =
     0x00, 0x41, 0x22, 0x14, 0x08,   // >
     0x02, 0x01, 0x51, 0x09, 0x06,   // ?
     0x32, 0x49, 0x59, 0x51, 0x3E,   // @
     0x7E, 0x11, 0x11, 0x11, 0x7E,   // A
     0x7F, 0x49, 0x49, 0x49, 0x36,   // B
     0x3E, 0x41, 0x41, 0x41, 0x22,   // C
     0x7F, 0x41, 0x41, 0x22, 0x1C,   // D
     0x7F, 0x49, 0x49, 0x49, 0x41,   // E
     0x7F, 0x09, 0x09, 0x09, 0x01,   // F
     0x3E, 0x41, 0x49, 0x49, 0x7A,   // G
     0x7F, 0x08, 0x08, 0x08, 0x7F,   // H
     0x00, 0x41, 0x7F, 0x41, 0x00,   // I
     0x20, 0x40, 0x41, 0x3F, 0x01,   // J
     0x7F, 0x08, 0x14, 0x22, 0x41,   // K
     0x7F, 0x40, 0x40, 0x40, 0x40,   // L
     0x7F, 0x02, 0x0C, 0x02, 0x7F,   // M
     0x7F, 0x04, 0x08, 0x10, 0x7F,   // N
     0x3E, 0x41, 0x41, 0x41, 0x3E,   // O
     0x7F, 0x09, 0x09, 0x09, 0x06,   // P
     0x3E, 0x41, 0x51, 0x21, 0x5E,   // Q
     0x7F, 0x09, 0x19, 0x29, 0x46,   // R
     0x46, 0x49, 0x49, 0x49, 0x31,   // S
     0x01, 0x01, 0x7F, 0x01, 0x01,   // T
     0x3F, 0x40, 0x40, 0x40, 0x3F,   // U
     0x1F, 0x20, 0x40, 0x20, 0x1F,   // V
     0x3F, 0x40, 0x38, 0x40, 0x3F,   // W
     0x63, 0x14, 0x08, 0x14, 0x63,   // X
     0x07, 0x08, 0x70, 0x08, 0x07,   // Y
     0x61, 0x51, 0x49, 0x45, 0x43,   // Z
     0x00, 0x7F, 0x41, 0x41, 0x00,   // [
     0x55, 0x2A, 0x55, 0x2A, 0x55,   // 55
     0x00, 0x41, 0x41, 0x7F, 0x00,   // ]
     0x04, 0x02, 0x01, 0x02, 0x04,   // ^
     0x40, 0x40, 0x40, 0x40, 0x40,   // _
     0x00, 0x01, 0x02, 0x04, 0x00,   // '
     0x20, 0x54, 0x54, 0x54, 0x78,   // a
     0x7F, 0x48, 0x44, 0x44, 0x38,    // b
     0x38, 0x44, 0x44, 0x44, 0x20,   // c
     0x38, 0x44, 0x44, 0x48, 0x7F,   // d
     0x38, 0x54, 0x54, 0x54, 0x18,   // e
     0x08, 0x7E, 0x09, 0x01, 0x02,   // f
     0x0C, 0x52, 0x52, 0x52, 0x3E,   // g
     0x7F, 0x08, 0x04, 0x04, 0x78,   // h
     0x00, 0x44, 0x7D, 0x40, 0x00,   // i
     0x20, 0x40, 0x44, 0x3D, 0x00,   // j
     0x7F, 0x10, 0x28, 0x44, 0x00,   // k
     0x00, 0x41, 0x7F, 0x40, 0x00,   // l
     0x7C, 0x04, 0x18, 0x04, 0x78,   // m
     0x7C, 0x08, 0x04, 0x04, 0x78,   // n
     0x38, 0x44, 0x44, 0x44, 0x38,   // o
     0x7C, 0x14, 0x14, 0x14, 0x08,   // p
     0x08, 0x14, 0x14, 0x18, 0x7C,   // q
     0x7C, 0x08, 0x04, 0x04, 0x08,   // r
     0x48, 0x54, 0x54, 0x54, 0x20,   // s
     0x04, 0x3F, 0x44, 0x40, 0x20,   // t
     0x3C, 0x40, 0x40, 0x20, 0x7C,   // u
     0x1C, 0x20, 0x40, 0x20, 0x1C,   // v
     0x3C, 0x40, 0x30, 0x40, 0x3C,   // w
     0x44, 0x28, 0x10, 0x28, 0x44,   // x
     0x0C, 0x50, 0x50, 0x50, 0x3C,   // y
     0x44, 0x64, 0x54, 0x4C, 0x44,   // z
	 0x00, 0x06, 0x09, 0x09, 0x06    // �
};

void LCD_init (unsigned char contrast)
{
    PORTB=0x00;
	DDRB|=0xC7;
	SET_SCE_PIN;
	SET_RST_PIN;
	SET_DC_PIN;

	delay_ms(1);
	CLEAR_SCE_PIN;    //Enable LCD
    delay_ms(5);
	CLEAR_RST_PIN;	//reset LCD
    delay_ms(10);
    SET_RST_PIN;
	delay_ms(5);
	SET_SCE_PIN;	//disable LCD

    LCD_writeCommand( 0x21 );  // LCD Extended Commands.
    LCD_writeCommand(contrast);  // Set LCD Vop (Contrast).0xC0
    LCD_writeCommand( 0x04 );  // Set Temp coefficent.
    LCD_writeCommand( 0x13 );  // LCD bias mode 1:48.
    LCD_writeCommand( 0x20 );  // LCD Standard Commands, Horizontal addressing mode.
    LCD_writeCommand( 0x0c );  // LCD in normal mode.
	
	LCD_writeCommand (0x80);   //column
	LCD_writeCommand (0x40);   //row
	LCD_clear();

 

}


void LCD_writeCommand ( unsigned char command )
{
    CLEAR_SCE_PIN;	  //enable LCD
	
	CLEAR_DC_PIN;	  //set LCD in command mode
	
    //  Send data to display controller.
    sendspi(command);//SPDR = command;
//	delay_ms(1);

    //  Wait until Tx register empty.
  //  while ( !(SPSR & 0x80) );

    SET_SCE_PIN;   	 //disable LCD
}

void LCD_clear ( void )
{
    int i,j;
	
	LCD_gotoXY (0,0);  	//start with (0,0) position

    for(i=0; i<8; i++)
	  for(j=0; j<90; j++)
	     LCD_writeData( 0x00 );
   
    LCD_gotoXY (0,0);	//bring the XY position back to (0,0)
      
}

void LCD_gotoXY ( unsigned char x, unsigned char y )
{
    LCD_writeCommand (0x80 | x);   //column
	LCD_writeCommand (0x40 | y);   //row
}


void LCD_writeData ( unsigned char Data )
{
    CLEAR_SCE_PIN;	  //enable LCD
	
	SET_DC_PIN;	  //set LCD in Data mode
	
    //  Send data to display controller.
    sendspi(Data);//SPDR = Data;
//	delay_ms(1);

    //  Wait until Tx register empty.
   // while ( !(SPSR & 0x80) );

    SET_SCE_PIN;   	 //disable LCD
}

void lcd_write(void)
{
int i;
for(i=0;i<=504;i++)
LCD_writeData(0xc0);
}


void LCD_writeChar (unsigned char ch)
{
   unsigned char j;
  
   LCD_writeData(0x00);
   
   for(j=0; j<5; j++)
     LCD_writeData( smallFont [(ch-32)*5 + j] );
	 
   LCD_writeData( 0x00 );
}

void LCD_writeString_F ( const unsigned char *string )
{
    while ( *string )
        LCD_writeChar( *string++ );
}

void sendspi(char scommand) //spi with port pin
 {  
   unsigned char i;
   for(i=0;i<=7;i++)
   { 
		CLEAR_SCK; //sck=0;
			
		if(scommand & 0X80)
			{SET_MOSI;}
		else
			{CLEAR_MOSI;}//mosi=(SREG & 0X01);
		
		scommand<<=1;
		SET_SCK; //sck=1; 
	
   }  
// PORTB &=0xFE;
 }

void LCD_showvalue(unsigned char num)	//prints the decimal 3digit value of num
{
	unsigned char H=0,T=0,O=0;
	H=num/100;
	T=(num - (H*100))/10;
	O=(num - (H*100) - (T*10));
	
	LCD_writeChar(H+48);
	LCD_writeChar(T+48);
	LCD_writeChar(O+48);
}
 
 void delay_ms(unsigned int delaytime)
{
 unsigned int delaycounter1,delaycounter2;
	 for(delaycounter1=0;delaycounter1<=delaytime;delaycounter1++)
	 {
		 for(delaycounter2=0;delaycounter2<=800;delaycounter2++)
		  {
		    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
		    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
		  }
		
	 }
}


#endif 