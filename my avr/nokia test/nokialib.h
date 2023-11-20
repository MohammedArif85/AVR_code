#include<compat/deprecated.h>
#include<avr/pgmspace.h>
#define LCDPORT PORTB


#define SCK_en sbi(LCDPORT,PB4)
#define SCK_db cbi(LCDPORT,PB4)
#define DC_en sbi(LCDPORT,PB2)
#define DC_db cbi(LCDPORT,PB2)
#define RES_en sbi(LCDPORT,PB0)
#define RES_db cbi(LCDPORT,PB0)
#define DAT_en sbi(LCDPORT,PB3)
#define DAT_db cbi(LCDPORT,PB3)
#define CE_en sbi(LCDPORT,PB1)
#define CE_db cbi(LCDPORT,PB1)
//global variable for remembering where to start writing the next text string on 3310 LCD
unsigned char char_start;

void spisend(unsigned char);
void wrdata(unsigned char);
void wrcmd(unsigned char);
void pixelxy(unsigned char, unsigned char);
void cursorxy(unsigned char, unsigned char);
void clearram(void);
void initlcd(void);
void putchr(unsigned char);
void putstr (unsigned char*);

/* Lookup Table for Nokia LCD */
const unsigned char FontLookup [][5] PROGMEM=
{
{ 0x00, 0x00, 0x00, 0x00, 0x00 }, // sp
{ 0x00, 0x00, 0x2f, 0x00, 0x00 }, // !
{ 0x00, 0x07, 0x00, 0x07, 0x00 }, // ”
{ 0x14, 0x7f, 0x14, 0x7f, 0x14 }, // #
{ 0x24, 0x2a, 0x7f, 0x2a, 0x12 }, // $
{ 0xc4, 0xc8, 0x10, 0x26, 0x46 }, // %
{ 0x36, 0x49, 0x55, 0x22, 0x50 }, // &
{ 0x00, 0x05, 0x03, 0x00, 0x00 }, // ‘
{ 0x00, 0x1c, 0x22, 0x41, 0x00 }, // (
{ 0x00, 0x41, 0x22, 0x1c, 0x00 }, // )
{ 0x14, 0x08, 0x3E, 0x08, 0x14 }, // *
{ 0x08, 0x08, 0x3E, 0x08, 0x08 }, // +
{ 0x00, 0x00, 0x50, 0x30, 0x00 }, // ,
{ 0x10, 0x10, 0x10, 0x10, 0x10 }, // -
{ 0x00, 0x60, 0x60, 0x00, 0x00 }, // .
{ 0x20, 0x10, 0x08, 0x04, 0x02 }, // /
{ 0x3E, 0x51, 0x49, 0x45, 0x3E }, // 0
{ 0x00, 0x42, 0x7F, 0x40, 0x00 }, // 1
{ 0x42, 0x61, 0x51, 0x49, 0x46 }, // 2
{ 0x21, 0x41, 0x45, 0x4B, 0x31 }, // 3
{ 0x18, 0x14, 0x12, 0x7F, 0x10 }, // 4
{ 0x27, 0x45, 0x45, 0x45, 0x39 }, // 5
{ 0x3C, 0x4A, 0x49, 0x49, 0x30 }, // 6
{ 0x01, 0x71, 0x09, 0x05, 0x03 }, // 7
{ 0x36, 0x49, 0x49, 0x49, 0x36 }, // 8
{ 0x06, 0x49, 0x49, 0x29, 0x1E }, // 9
{ 0x00, 0x36, 0x36, 0x00, 0x00 }, // :
{ 0x00, 0x56, 0x36, 0x00, 0x00 }, // ;
{ 0x08, 0x14, 0x22, 0x41, 0x00 }, // <
{ 0x14, 0x14, 0x14, 0x14, 0x14 }, // =
{ 0x00, 0x41, 0x22, 0x14, 0x08 }, // >
{ 0x02, 0x01, 0x51, 0x09, 0x06 }, // ?
{ 0x32, 0x49, 0x59, 0x51, 0x3E }, // @
{ 0x7E, 0x11, 0x11, 0x11, 0x7E }, // A
{ 0x7F, 0x49, 0x49, 0x49, 0x36 }, // B
{ 0x3E, 0x41, 0x41, 0x41, 0x22 }, // C
{ 0x7F, 0x41, 0x41, 0x22, 0x1C }, // D
{ 0x7F, 0x49, 0x49, 0x49, 0x41 }, // E
{ 0x7F, 0x09, 0x09, 0x09, 0x01 }, // F
{ 0x3E, 0x41, 0x49, 0x49, 0x7A }, // G
{ 0x7F, 0x08, 0x08, 0x08, 0x7F }, // H
{ 0x00, 0x41, 0x7F, 0x41, 0x00 }, // I
{ 0x20, 0x40, 0x41, 0x3F, 0x01 }, // J
{ 0x7F, 0x08, 0x14, 0x22, 0x41 }, // K
{ 0x7F, 0x40, 0x40, 0x40, 0x40 }, // L
{ 0x7F, 0x02, 0x0C, 0x02, 0x7F }, // M
{ 0x7F, 0x04, 0x08, 0x10, 0x7F }, // N
{ 0x3E, 0x41, 0x41, 0x41, 0x3E }, // O
{ 0x7F, 0x09, 0x09, 0x09, 0x06 }, // P
{ 0x3E, 0x41, 0x51, 0x21, 0x5E }, // Q
{ 0x7F, 0x09, 0x19, 0x29, 0x46 }, // R
{ 0x46, 0x49, 0x49, 0x49, 0x31 }, // S
{ 0x01, 0x01, 0x7F, 0x01, 0x01 }, // T
{ 0x3F, 0x40, 0x40, 0x40, 0x3F }, // U
{ 0x1F, 0x20, 0x40, 0x20, 0x1F }, // V
{ 0x3F, 0x40, 0x38, 0x40, 0x3F }, // W
{ 0x63, 0x14, 0x08, 0x14, 0x63 }, // X
{ 0x07, 0x08, 0x70, 0x08, 0x07 }, // Y
{ 0x61, 0x51, 0x49, 0x45, 0x43 }, // Z
{ 0x00, 0x7F, 0x41, 0x41, 0x00 }, // [
{ 0x55, 0x2A, 0x55, 0x2A, 0x55 }, // 55
{ 0x00, 0x41, 0x41, 0x7F, 0x00 }, // ]
{ 0x04, 0x02, 0x01, 0x02, 0x04 }, // ^
{ 0x40, 0x40, 0x40, 0x40, 0x40 }, // _
{ 0x00, 0x01, 0x02, 0x04, 0x00 }, // ‘
{ 0x20, 0x54, 0x54, 0x54, 0x78 }, // a
{ 0x7F, 0x48, 0x44, 0x44, 0x38 }, // b
{ 0x38, 0x44, 0x44, 0x44, 0x20 }, // c
{ 0x38, 0x44, 0x44, 0x48, 0x7F }, // d
{ 0x38, 0x54, 0x54, 0x54, 0x18 }, // e
{ 0x08, 0x7E, 0x09, 0x01, 0x02 }, // f
{ 0x0C, 0x52, 0x52, 0x52, 0x3E }, // g
{ 0x7F, 0x08, 0x04, 0x04, 0x78 }, // h
{ 0x00, 0x44, 0x7D, 0x40, 0x00 }, // i
{ 0x20, 0x40, 0x44, 0x3D, 0x00 }, // j
{ 0x7F, 0x10, 0x28, 0x44, 0x00 }, // k
{ 0x00, 0x41, 0x7F, 0x40, 0x00 }, // l
{ 0x7C, 0x04, 0x18, 0x04, 0x78 }, // m
{ 0x7C, 0x08, 0x04, 0x04, 0x78 }, // n
{ 0x38, 0x44, 0x44, 0x44, 0x38 }, // o
{ 0x7C, 0x14, 0x14, 0x14, 0x08 }, // p
{ 0x08, 0x14, 0x14, 0x18, 0x7C }, // q
{ 0x7C, 0x08, 0x04, 0x04, 0x08 }, // r
{ 0x48, 0x54, 0x54, 0x54, 0x20 }, // s
{ 0x04, 0x3F, 0x44, 0x40, 0x20 }, // t
{ 0x3C, 0x40, 0x40, 0x20, 0x7C }, // u
{ 0x1C, 0x20, 0x40, 0x20, 0x1C }, // v
{ 0x3C, 0x40, 0x30, 0x40, 0x3C }, // w
{ 0x44, 0x28, 0x10, 0x28, 0x44 }, // x
{ 0x0C, 0x50, 0x50, 0x50, 0x3C }, // y
{ 0x44, 0x64, 0x54, 0x4C, 0x44 } // z
};

//******* VERY LARGE FONTS ********** 
//used here for displaying temperature

const unsigned char number[13][3][16] PROGMEM= {

0,128,192,224,224,96,224,224,  //'0'
192,128,0,0,0,0,0,0
,
112,255,255,1,0,0,0,0,
255,255,254,0,0,0,0,0
,
0,15,31,60,56,48,56,56,
31,15,3,0,0,0,0,0
,

0,0,0,0,128,224,224,0, 		   //'1'
0,0,0,0,0,0,0,0
,
0,0,3,3,3,255,255,0,
0,0,0,0,0,0,0,0
,
0,0,56,56,56,63,63,56,
56,56,0,0,0,0,0,0
,

0,192,192,224,96,96,224,224,   //'2'
192,128,0,0,0,0,0,0
,
0,1,0,0,128,192,224,249,
63,31,0,0,0,0,0,0
,
0,60,62,63,63,59,57,56,
56,56,56,0,0,0,0,0
,

0,192,224,224,96,96,224,224,   //'3'
192,192,0,0,0,0,0,0
,
0,1,0,0,48,48,56,125,
239,207,0,0,0,0,0,0
,
0,28,56,56,48,48,56,60,
31,15,1,0,0,0,0,0
,

0,0,0,0,0,128,192,224, 		   //'4'
224,0,0,0,0,0,0,0
,
224,240,248,222,207,199,193,255,
255,192,192,0,0,0,0,0
,
0,0,0,0,0,0,0,63,
63,0,0,0,0,0,0,0
,

0,224,224,224,224,224,224,224,	//'5'
224,224,224,0,0,0,0,0
,
0,63,63,63,56,56,48,112,
240,224,0,0,0,0,0,0
,
0,28,56,56,48,48,56,60,
31,15,1,0,0,0,0,0
,

0,0,128,192,192,224,96,96,		//'6'
224,224,0,0,0,0,0,0
,
224,254,255,55,57,24,24,56,
240,240,192,0,0,0,0,0
,
0,15,31,28,56,48,48,56,
31,15,7,0,0,0,0,0
,

0,224,224,224,224,224,224,224,		 //'7'
224,224,224,0,0,0,0,0
,
0,0,0,0,128,224,248,126,
31,7,1,0,0,0,0,0
,
0,0,56,62,31,7,1,0,
0,0,0,0,0,0,0,0
,

0,128,192,224,224,96,96,224,		 //'8'
192,192,0,0,0,0,0,0
,
0,207,255,127,56,48,112,112,
255,239,199,0,0,0,0,0
,
3,15,31,60,56,48,48,56,
31,31,15,0,0,0,0,0
,

0,128,192,224,224,96,224,224,		 //'9'
192,128,0,0,0,0,0,0
,
12,63,127,241,224,192,192,225,
255,255,254,0,0,0,0,0
,
0,0,56,48,48,56,56,30,
15,7,0,0,0,0,0,0
,


0,0,0,0,0,0,0,0,	  		  		 //'.'
0,0,0,0,0,0,0,0
,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0
,
60,60,60,0,0,0,0,0,
0,0,0,0,0,0,0,0
,

0,0,0,0,0,0,0,0,   					 //'+'
0,0,0,0,0,0,0,0
,
0,0,64,64,64,64,64,254,
254,64,64,64,64,64,0,0
,
0,0,0,0,0,0,0,15,
15,0,0,0,0,0,0,0
,

0,0,0,0,0,0,0,0, 	   				 //'-'
0,0,0,0,0,0,0,0
,
0,64,64,64,64,64,64,0,
0,0,0,0,0,0,0,0
,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0
};



static void Delay ()
{
 _delay_us(10);
}

/* Send single byte on SPI Bus (software) */
/* Input Arguments: x -> Byte to send */

void spisend(unsigned char x)
{
unsigned char i;
for(i=0;i<8;i++)
{
SCK_db;
_delay_us(10);
if((x&0x80)){
DAT_en;
} else {
DAT_db;
}
SCK_en;
x <<= 1;
}
}

/* Write Data Byte to Nokia LCD */
/* Input Arguments: x-> Data Byte to be sent */

void wrdata(unsigned char x){
DC_en;
CE_db;
spisend(x);
CE_en;
}

/* Write Command Byte to Nokia LCD */
/* Input Arguments: x -> Command Byte to be sent */
void wrcmd(unsigned char x){
DC_db;
CE_db;
spisend(x);
CE_en;
}

/* Brings ram pointer to X,Y pixel position */
/* Input Arguments: x-> X cordinate range from 0 to 83 */
/* Input Arguments: y-> Y cordinate range from 0 to 5 */
void pixelxy(unsigned char x, unsigned char y)
{
wrcmd(0x40|(y&0x07)); // Y axis
wrcmd(0x80|(x&0x7f)); // X axis
}

/* Clears the screen */
void clearram(void)
{
int ddram;
pixelxy(0,0); // Cursor Home.
for (ddram=0;ddram<700;ddram++) {wrdata(0x00);} // 6*84 = 504 DDRAM addresses.
}

/* Nokia3310 LCD Initialization */
void initlcd(void)
{
RES_en; // Set _RES HIGH.
CE_en; // Disable Chip.
RES_db; // Reset the LCD.
Delay(); // Wait 100ms.
RES_en; // Awake LCD from RESET state.

wrcmd(0x21); // Activate Chip and H=1. Extended instruction set mode.
wrcmd(0xc2); // Set LCD Voltage to about 7V. wrcmd(0xc7);
wrcmd(0x20); // Horizontal addressing and H=0.
clearram(); // Erase all pixel on the DDRAM.
wrcmd(0x0C); // Display Normal.
pixelxy(0,0); // Cursor Home.
}

/* Bring cursor to Line and character specified */
/* Input Arguments: row -> Line number range from 1 to 6 */
/* Input Arguments: Column -> character position range from 1 to 14 */
/* You can have maximum of 6 lines of text on LCD and each line
containing 14 characters. Address is auto increment */

void cursorxy(unsigned char row, unsigned char col){
if((row>6) || (row<1) || (col<1) || (col>14))
return;
pixelxy(((col-1)*6),(row-1));
}

/* Writes single character on LCD */
/* Input Arguments: ch -> Character to be displayed */
void putchr(unsigned char ch){
unsigned char i, temp;
if ( (ch < 0x20) || (ch > 0x7b) ){
ch = 92;
}
for(i=0;i<5;i++){
temp = pgm_read_byte(&FontLookup[ch - 32][i]) << 1;
wrdata(temp);
}
wrdata(0x00);
}

/*--------------------------------------------------------------------------------------------------
  Name         :  LCD_writeChar_megaFont
  Description  :  Displays a character in large fonts, used here for displatying temperature 
		  (for displaying '.', '+', '-', and '0' to '9', stored 
		  in 3310_routines.h as three dimensional array, number[][][])
  Argument(s)  :  ch   -> Character to write.
  Return value :  None.
--------------------------------------------------------------------------------------------------*/
void LCD_writeChar_megaFont (unsigned char ch)
{
   unsigned char i, j, k;
   
   if(ch == '.')
    ch = 10;
   else if (ch == '+')
    ch = 11;
   else if (ch == '-')
    ch = 12;
   else
    ch = ch & 0x0f;
	
    
   for(i=0;i<3;i++)
   {	
	 pixelxy (4 + char_start, i+2);
 
     for(j=0; j<16; j++)
       wrdata( pgm_read_byte(&number[ch][i][j]));
   } 
   
   if(ch == '.') 
     char_start += 5;
   else
     char_start += 12;
}

/*--------------------------------------------------------------------------------------------------
  Name         :  LCD_writeString_megaFont
  Description  :  Displays a string at current location, in large fonts
  Argument(s)  :  string -> Pointer to ASCII string (stored in RAM)
  Return value :  None.
--------------------------------------------------------------------------------------------------*/
void lcdstr_megaFont ( unsigned char *string )
{
    char_start = 0;
	
    while ( *string )
        LCD_writeChar_megaFont( *string++ );
	
	//pixelxy(char_start+6, 3);
//	LCD_writeChar('z'+1); 			  //symbol of Degree
//	LCD_writeChar('C');
}




/* Writes character string on LCD */
/* Input Arguments: str -> Pointer to string to be displayed */
void putstr (unsigned char *str){
while(*str)
putchr(*str++);
}
/* draw border on LCD */
void lcd_drawborder (void )
{
  unsigned char i, j;  
    
  for(i=0; i<7; i++)
  {
    pixelxy (0,i);
	
	for(j=0; j<84; j++)
	{
	  if(j == 0 || j == 83)
		wrdata (0xff);		// first and last column solid fill to make line
	  else if(i == 0)
	   wrdata (0x08);		// row 0 is having only 5 bits (not 8)
	  else if(i == 6)
	    wrdata (0x04);		// row 6 is having only 3 bits (not 8)
	  else
	    wrdata(0x00);
	}
  }
}	
void lcd_clear()
{int i;
for(i=0;i<504;i++) {
pixelxy((i%84)-1,i/84);
wrdata(0x00);
}

}


void LcdLine ( unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2 )
{
    int dx, dy,i,j;

    dy = y2 - y1;
    dx = x2 - x1;

    if ( dy < 0 )
    {
        dy    = -dy;
        
    }
   
    if ( dx < 0 )
    {
        dx    = -dx;
        
    }
   
 for(i=0;i<dx;i++)
 {
  for(j=0;j<dy;j++)  
  {
    pixelxy(i,j);
	wrdata(0x80);
  }
 }   
}
