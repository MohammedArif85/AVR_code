// Black and white NTSC video generation with fixed point animation
// D.6 is sync:330 ohm + diode to 75 ohm resistor
// D.5 is video:1000 ohm + diode to 75 ohm resistor  
// Mega644 version by Shane Pryor 142h x 200v resolution
// mods by brl4@cornell.edu

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h> 
#include <math.h> 
#include <util/delay.h>  
#include <avr/sleep.h>


// optional, if preferred///
//#define begin {
//#define end   }
////////////////////////////			

//cycles = 63.625 * 16 Note NTSC is 63.55 
//but this line duration makes each frame exactly 1/60 sec
//which is nice for keeping a realtime clock 
#define lineTime 1018
#define ScreenTop 30
#define ScreenBot 230

//sync
char syncON, syncOFF;

//current line number in the current frame
volatile int LineCount;

//200 x 144 - screen buffer and pointer
char screen[3600];
int* screenindex;

//One bit masks
char pos[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

//================================ 
//3x5 font numbers, then letters
//packed two per definition for fast 
//copy to the screen at x-position divisible by 4
prog_char smallbitmap[39][5] = { 
	//0
    0b11101110,
	0b10101010,
	0b10101010,
	0b10101010,
	0b11101110,
	//1
	0b01000100,
	0b11001100,
	0b01000100,
	0b01000100,
	0b11101110,
	//2
	0b11101110,
	0b00100010,
	0b11101110,
	0b10001000,
	0b11101110,
	//3
	0b11101110,
	0b00100010,
	0b11101110,
	0b00100010,
	0b11101110,
	//4
	0b10101010,
	0b10101010,
	0b11101110,
	0b00100010,
	0b00100010,
	//5
	0b11101110,
	0b10001000,
	0b11101110,
	0b00100010,
	0b11101110,
	//6
	0b11001100,
	0b10001000,
	0b11101110,
	0b10101010,
	0b11101110,
	//7
	0b11101110,
	0b00100010,
	0b01000100,
	0b10001000,
	0b10001000,
	//8
	0b11101110,
	0b10101010,
	0b11101110,
	0b10101010,
	0b11101110,
	//9
	0b11101110,
	0b10101010,
	0b11101110,
	0b00100010,
	0b01100110,
	//:
	0b00000000,
	0b01000100,
	0b00000000,
	0b01000100,
	0b00000000,
	//=
	0b00000000,
	0b11101110,
	0b00000000,
	0b11101110,
	0b00000000,
	//blank
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//A
	0b11101110,
	0b10101010,
	0b11101110,
	0b10101010,
	0b10101010,
	//B
	0b11001100,
	0b10101010,
	0b11101110,
	0b10101010,
	0b11001100,
	//C
	0b11101110,
	0b10001000,
	0b10001000,
	0b10001000,
	0b11101110,
	//D
	0b11001100,
	0b10101010,
	0b10101010,
	0b10101010,
	0b11001100,
	//E
	0b11101110,
	0b10001000,
	0b11101110,
	0b10001000,
	0b11101110,
	//F
	0b11101110,
	0b10001000,
	0b11101110,
	0b10001000,
	0b10001000,
	//G
	0b11101110,
	0b10001000,
	0b10001000,
	0b10101010,
	0b11101110,
	//H
	0b10101010,
	0b10101010,
	0b11101110,
	0b10101010,
	0b10101010,
	//I
	0b11101110,
	0b01000100,
	0b01000100,
	0b01000100,
	0b11101110,
	//J
	0b00100010,
	0b00100010,
	0b00100010,
	0b10101010,
	0b11101110,
	//K
	0b10001000,
	0b10101010,
	0b11001100,
	0b11001100,
	0b10101010,
	//L
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b11101110,
	//M
	0b10101010,
	0b11101110,
	0b11101110,
	0b10101010,
	0b10101010,
	//N
	0b00000000,
	0b11001100,
	0b10101010,
	0b10101010,
	0b10101010,
	//O
	0b01000100,
	0b10101010,
	0b10101010,
	0b10101010,
	0b01000100,
	//P
	0b11101110,
	0b10101010,
	0b11101110,
	0b10001000,
	0b10001000,
	//Q
	0b01000100,
	0b10101010,
	0b10101010,
	0b11101110,
	0b01100110,
	//R
	0b11101110,
	0b10101010,
	0b11001100,
	0b11101110,
	0b10101010,
	//S
	0b11101110,
	0b10001000,
	0b11101110,
	0b00100010,
	0b11101110,
	//T
	0b11101110,
	0b01000100,
	0b01000100,
	0b01000100,
	0b01000100, 
	//U
	0b10101010,
	0b10101010,
	0b10101010,
	0b10101010,
	0b11101110, 
	//V
	0b10101010,
	0b10101010,
	0b10101010,
	0b10101010,
	0b01000100,
	//W
	0b10101010,
	0b10101010,
	0b11101110,
	0b11101110,
	0b10101010,
	//X
	0b00000000,
	0b10101010,
	0b01000100,
	0b01000100,
	0b10101010,
	//Y
	0b10101010,
	0b10101010,
	0b01000100,
	0b01000100,
	0b01000100,
	//Z
	0b11101110,
	0b00100010,
	0b01000100,
	0b10001000,
	0b11101110
};

//===============================================
// Full ascii 5x7 char set
// Designed by: David Perez de la Cruz,and Ed Lau	  
// see: http://instruct1.cit.cornell.edu/courses/ee476/FinalProjects/s2005/dp93/index.html

prog_char ascii[128][7] = {
	//0
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//1
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//2
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//3
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//4
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//5
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//6
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//7
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//8
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//9
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//10
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//11
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//12
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//13
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//14
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//15
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//16
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//17
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//18
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//19
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//20
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//21
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//22
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//23
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//24
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//25
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//26
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//27
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//28
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//29
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//30
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//31
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//32 Space
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//33 Exclamation !
	0b01100000,
	0b01100000,
	0b01100000,
	0b01100000,
	0b00000000,
	0b00000000,
	0b01100000,
	//34 Quotes "
	0b01010000,
	0b01010000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//35 Number #
	0b00000000,
	0b01010000,
	0b11111000,
	0b01010000,
	0b11111000,
	0b01010000,
	0b00000000,
	//36 Dollars $
	0b01110000,
	0b10100000,
	0b10100000,
	0b01110000,
	0b00101000,
	0b00101000,
	0b01110000,
	//37 Percent %
	0b01000000,
	0b10101000,
	0b01010000,
	0b00100000,
	0b01010000,
	0b10101000,
	0b00010000,
	//38 Ampersand &
	0b00100000,
	0b01010000,
	0b10100000,
	0b01000000,
	0b10101000,
	0b10010000,
	0b01101000,
	//39 Single Quote '
	0b01000000,
	0b01000000,
	0b01000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//40 Left Parenthesis (
	0b00010000,
	0b00100000,
	0b01000000,	
	0b01000000,
	0b01000000,
	0b00100000,
	0b00010000,
	//41 Right Parenthesis )
	0b01000000,
	0b00100000,
	0b00010000,
	0b00010000,
	0b00010000,
	0b00100000,
	0b01000000,
	//42 Star *
	0b00010000,
	0b00111000,
	0b00010000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//43 Plus +
	0b00000000,
	0b00100000,
	0b00100000,
	0b11111000,
	0b00100000,
	0b00100000,
	0b00000000,
	//44 Comma ,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00010000,
	0b00010000,
	//45 Minus -
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b11111000,
	0b00000000,
	0b00000000,
	//46 Period .
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00010000,
	// 47 Backslash /
	0b00000000,
	0b00001000,
	0b00010000,
	0b00100000,
	0b01000000,
	0b10000000,
	0b00000000,
	// 48 Zero
	0b01110000,
	0b10001000,
	0b10011000,
	0b10101000,
	0b11001000,
	0b10001000,
	0b01110000,
	//49 One
	0b00100000,
	0b01100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b01110000,  
	//50 two
	0b01110000,
	0b10001000,
	0b00001000,
	0b00010000,
	0b00100000,
	0b01000000,
	0b11111000,
	 //51 Three
	0b11111000,
	0b00010000,
	0b00100000,
	0b00010000,
	0b00001000,
	0b10001000,
	0b01110000,
	//52 Four
	0b00010000,
	0b00110000,
	0b01010000,
	0b10010000,
	0b11111000,
	0b00010000,
	0b00010000,
	//53 Five
	0b11111000,
	0b10000000,
	0b11110000,
	0b00001000,
	0b00001000,
	0b10001000,
	0b01110000,
	//54 Six
	0b01000000,
	0b10000000,
	0b10000000,
	0b11110000,
	0b10001000,
	0b10001000,
	0b01110000,
	//55 Seven
	0b11111000,
	0b00001000,
	0b00010000,
	0b00100000,
	0b01000000,
	0b10000000,
	0b10000000,
	//56 Eight
	0b01110000,
	0b10001000,
	0b10001000,
	0b01110000,
	0b10001000,
	0b10001000,
	0b01110000,
	//57 Nine
	0b01110000,
	0b10001000,
	0b10001000,
	0b01111000,
	0b00001000,
	0b00001000,
	0b00010000,
	//58 :
	0b00000000,
	0b00000000,
	0b00100000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00100000,
	//59 ;
	0b00000000,
	0b00000000,
	0b00100000,
	0b00000000,
	0b00000000,
	0b00100000,
	0b00100000,
	//60 <
	0b00000000,
	0b00011000,
	0b01100000,
	0b10000000,
	0b01100000,
	0b00011000,
	0b00000000,
	//61 =
	0b00000000,
	0b00000000,
	0b01111000,
	0b00000000,
	0b01111000,
	0b00000000,
	0b00000000,
	//62 >
	0b00000000,
	0b11000000,
	0b00110000,
	0b00001000,
	0b00110000,
	0b11000000,
	0b00000000,
	//63 ?
	0b00110000,
	0b01001000,
	0b00010000,
	0b00100000,
	0b00100000,
	0b00000000,
	0b00100000,
	//64 @
	0b01110000,
	0b10001000,
	0b10111000,
	0b10101000,
	0b10010000,
	0b10001000,
	0b01110000,
	//65 A
	0b01110000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b11111000,
	0b10001000,
	0b10001000,
	//B
	0b11110000,
	0b10001000,
	0b10001000,
	0b11110000,
	0b10001000,
	0b10001000,
	0b11110000,
	//C
	0b01110000,
	0b10001000,
	0b10000000,
	0b10000000,
	0b10000000,
	0b10001000,
	0b01110000,
	//D
	0b11110000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b11110000,
	//E
	0b11111000,
	0b10000000,
	0b10000000,
	0b11111000,
	0b10000000,
	0b10000000,
	0b11111000,
	//F
	0b11111000,
	0b10000000,
	0b10000000,
	0b11111000,
	0b10000000,
	0b10000000,
	0b10000000,
	//G
	0b01110000,
	0b10001000,
	0b10000000,
	0b10011000,
	0b10001000,
	0b10001000,
	0b01110000,
	//H
	0b10001000,
	0b10001000,
	0b10001000,
	0b11111000,
	0b10001000,
	0b10001000,
	0b10001000,
	//I
	0b01110000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b01110000,
	//J
	0b00111000,
	0b00010000,
	0b00010000,
	0b00010000,
	0b00010000,
	0b10010000,
	0b01100000,
	//K
	0b10001000,
	0b10010000,
	0b10100000,
	0b11000000,
	0b10100000,
	0b10010000,
	0b10001000,
	//L
	0b10000000,
	0b10000000,
	0b10000000,
	0b10000000,
	0b10000000,
	0b10000000,
	0b11111000,
	//M
	0b10001000,
	0b11011000,
	0b10101000,
	0b10101000,
	0b10001000,
	0b10001000,
	0b10001000,
	//N
	0b10001000,
	0b10001000,
	0b11001000,
	0b10101000,
	0b10011000,
	0b10001000,
	0b10001000,
	//O
	0b01110000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b01110000,
	//P
	0b11110000,
	0b10001000,
	0b10001000,
	0b11110000,
	0b10000000,
	0b10000000,
	0b10000000,
	//Q
	0b01110000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b10101000,
	0b10010000,
	0b01101000,
	//R
	0b11110000,
	0b10001000,
	0b10001000,
	0b11110000,
	0b10100000,
	0b10010000,
	0b10001000,
	//S
	0b01111000,
	0b10000000,
	0b10000000,
	0b01110000,
	0b00001000,
	0b00001000,
	0b11110000,
	//T
	0b11111000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	//U
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b01110000,
	//V
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b01010000,
	0b00100000,
	//W
	0b10001000,
	0b10001000,
	0b10001000,
	0b10101000,
	0b10101000,
	0b10101000,
	0b01010000,
	//X
	0b10001000,
	0b10001000,
	0b01010000,
	0b00100000,
	0b01010000,
	0b10001000,
	0b10001000,
	//Y
	0b10001000,
	0b10001000,
	0b10001000,
	0b01010000,
	0b00100000,
	0b00100000,
	0b00100000,
	//Z
	0b11111000,
	0b00001000,
	0b00010000,
	0b00100000,
	0b01000000,
	0b10000000,
	0b11111000,
	//91 [
	0b11100000,
	0b10000000,
	0b10000000,
	0b10000000,
	0b10000000,
	0b10000000,
	0b11100000,
	//92 (backslash)
	0b00000000,
	0b10000000,
	0b01000000,
	0b00100000,
	0b00010000,
	0b00001000,
	0b00000000,
	//93 ]
	0b00111000,
	0b00001000,
	0b00001000,
	0b00001000,
	0b00001000,
	0b00001000,
	0b00111000,
	//94 ^
	0b00100000,
	0b01010000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//95 _
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b11111000,
	//96 `
	0b10000000,
	0b01000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	//97 a
	0b00000000,
	0b01100000,
	0b00010000,
	0b01110000,
	0b10010000,
	0b01100000,
	0b00000000,
	//98 b
	0b10000000,
	0b10000000,
	0b11100000,
	0b10010000,
	0b10010000,
	0b11100000,
	0b00000000,
	//99 c
	0b00000000,
	0b00000000,
	0b01110000,
	0b10000000,
	0b10000000,
	0b01110000,
	0b00000000,
	// 100 d
	0b00010000,
	0b00010000,
	0b01110000,
	0b10010000,
	0b10010000,
	0b01110000,
	0b00000000,
	//101 e
	0b00000000,
	0b01100000,
	0b10010000,
	0b11110000,
	0b10000000,
	0b01110000,
	0b00000000,
	//102 f
	0b00110000,
	0b01000000,
	0b11100000,
	0b01000000,
	0b01000000,
	0b01000000,
	0b00000000,
	//103 g
	0b00000000,
	0b01100000,
	0b10010000,
	0b01110000,
	0b00010000,
	0b00010000,
	0b01100000,
	//104 h
	0b10000000,
	0b10000000,
	0b11100000,
	0b10010000,
	0b10010000,
	0b10010000,
	0b00000000,
	//105 i
	0b00000000,
	0b00100000,
	0b00000000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00000000,
	//106 j
	0b00000000,
	0b00010000,
	0b00000000,
	0b00010000,
	0b00010000,
	0b00010000,
	0b01100000,
	//107 k
	0b10000000,
	0b10010000,
	0b10100000,
	0b11000000,
	0b10100000,
	0b10010000,
	0b00000000,
	//108 l
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00000000,
	//109 m
	0b00000000,
	0b00000000,
	0b01010000,
	0b10101000,
	0b10101000,
	0b10101000,
	0b00000000,
	//110 n
	0b00000000,
	0b00000000,
	0b01100000,
	0b10010000,
	0b10010000,
	0b10010000,
	0b00000000,
	//111 o
	0b00000000,
	0b00000000,
	0b01100000,
	0b10010000,
	0b10010000,
	0b01100000,
	0b00000000,
	//112 p
	0b00000000,
	0b00000000,
	0b01100000,
	0b10010000,
	0b11110000,
	0b10000000,
	0b10000000,
	//113 q
	0b00000000,
	0b00000000,
	0b01100000,
	0b10010000,
	0b11110000,
	0b00010000,
	0b00010000,
	//114 r
	0b00000000,
	0b00000000,
	0b10111000,
	0b01000000,
	0b01000000,
	0b01000000,
	0b00000000,
	//115 s
	0b00000000,
	0b00000000,
	0b01110000,
	0b01000000,
	0b00010000,
	0b01110000,
	0b00000000,
	//116 t
	0b01000000,
	0b01000000,
	0b11100000,
	0b01000000,
	0b01000000,
	0b01000000,
	0b00000000,
	// 117u
	0b00000000,
	0b00000000,
	0b10010000,
	0b10010000,
	0b10010000,
	0b01100000,
	0b00000000,
	//118 v
	0b00000000,
	0b00000000,
	0b10001000,
	0b10001000,
	0b01010000,
	0b00100000,
	0b00000000,
	//119 w
	0b00000000,
	0b00000000,
	0b10101000,
	0b10101000,
	0b01010000,
	0b01010000,
	0b00000000,
	//120 x
	0b00000000,
	0b00000000,
	0b10010000,
	0b01100000,
	0b01100000,
	0b10010000,
	0b00000000,
	//121 y
	0b00000000,
	0b00000000,
	0b10010000,
	0b10010000,
	0b01100000,
	0b01000000,
	0b10000000,
	//122 z
	0b00000000,
	0b00000000,
	0b11110000,
	0b00100000,
	0b01000000,
	0b11110000,
	0b00000000,
	//123 {
	0b00100000,
	0b01000000,
	0b01000000,
	0b10000000,
	0b01000000,
	0b01000000,
	0b00100000,
	//124 |
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	//125 }
	0b00100000,
	0b00010000,
	0b00010000,
	0b00001000,
	0b00010000,	
	0b00010000,
	0b00100000,
	//126 ~
	0b00000000,
	0b00000000,
	0b01000000,
	0b10101000,
	0b00010000,
	0b00000000,
	0b00000000,
	//127 DEL
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000
};

//macro to put a byte to the screen
asm(".MACRO videobits\n\t"
  		
	"BST  R4,7\n\t"
	"BLD  R30,5\n\t"
	"OUT  0x0b,R30\n\t"

	//"NOP \n\t" 
	"NOP \n\t"
	"BST  R4,6\n\t"
	"BLD  R30,5\n\t"
	"OUT  0x0b,R30\n\t"

	//"NOP \n\t" 
	"NOP \n\t"
	"BST  R4,5\n\t"
	"BLD  R30,5\n\t"
	"OUT  0x0b,R30\n\t"

	//"NOP \n\t" 
	"NOP \n\t"
	"BST  R4,4\n\t"
	"BLD  R30,5\n\t"
	"OUT  0x0b,R30\n\t"

	//"NOP \n\t" 
	"NOP \n\t"
	"BST  R4,3\n\t"
	"BLD  R30,5\n\t"
	"OUT  0x0b,R30\n\t"

	//"NOP \n\t" 
	"NOP \n\t"
	"BST  R4,2\n\t"
	"BLD  R30,5\n\t"
	"OUT  0x0b,R30\n\t"

	//"NOP \n\t" 
	"NOP \n\t"
	"BST  R4,1\n\t"
	"BLD  R30,5\n\t"
	"OUT  0x0b,R30\n\t"

	//"NOP \n\t" 
	"NOP \n\t"
	"BST  R4,0\n\t"
	"BLD  R30,5\n\t"
	"OUT  0x0b,R30\n\t"

	".ENDM\n\t"
   );

// puts 18 bytes (1 line) to the screen
void byteblast() {

	asm("LDS  R26,screenindex\n\t"
        "LDS  R27,screenindex+1\n\t"
			
		"LDI R30, 0x40\n\t"		

		"LD  R4,X+\n\t"
		"videobits\n\t"

		"LD  R4,X+\n\t"
		"videobits\n\t"
	
		"LD  R4,X+\n\t"
		"videobits\n\t"

		"LD  R4,X+\n\t"
		"videobits\n\t"

		"LD  R4,X+\n\t"
		"videobits\n\t"

		"LD  R4,X+\n\t"
		"videobits\n\t"

		"LD  R4,X+\n\t"
		"videobits\n\t"

		"LD  R4,X+\n\t"
		"videobits\n\t"

		"LD  R4,X+\n\t"
		"videobits\n\t"

		"LD  R4,X+\n\t"
		"videobits\n\t"

		"LD  R4,X+\n\t"
		"videobits\n\t"

		"LD  R4,X+\n\t"
		"videobits\n\t"

		"LD  R4,X+\n\t"
		"videobits\n\t"

		"LD  R4,X+\n\t"
		"videobits\n\t"

		"LD  R4,X+\n\t"
		"videobits\n\t"

		"LD  R4,X+\n\t"
		"videobits\n\t"

		"LD  R4,X+\n\t"
		"videobits\n\t"

		"LD  R4,X\n\t"
		"videobits\n\t"

		"CLT\n\t"
		"LDI R30, 0x40\n\t"

		"BLD  R30,5\n\t"
		"OUT  0x0b,R30\n\t"	
	   );
}

//==================================
//This is the sync generator and raster generator. It MUST be entered from 
//sleep mode to get accurate timing of the sync pulses

ISR (TIMER1_COMPA_vect) {
	//start the Horizontal sync pulse    
	PORTD = syncON;

	//update the current scanline number
	LineCount++;   
  
	//begin inverted (Vertical) synch after line 247
	if (LineCount==248) { 
    	syncON = 0b01000000;
    	syncOFF = 0;
  	}
  
	//back to regular sync after line 250
	if (LineCount==251)	{
		syncON = 0;
		syncOFF = 0b01000000;
	}  
  
  	//start new frame after line 262
	if (LineCount==263)
		LineCount = 1;
      
	//adjust to make 5 us pulses
	_delay_us(2);

	//end sync pulse
	PORTD = syncOFF;   

	if (LineCount < ScreenBot && LineCount >= ScreenTop) {
		//compute offset into screen array
		screenindex = screen + ((LineCount - ScreenTop) << 4) + ((LineCount - ScreenTop) << 1);

		//center image on screen
		_delay_us(8); //12

		//blast the data to the screen
		byteblast();
	}         
}

//==================================
//plot one point 
//at x,y with color 1=white 0=black 2=invert 
void video_pt(char x, char y, char c) {
	//each line has 18 bytes
	//calculate i based upon this and x,y
	// the byte with the pixel in it
	int i = (x >> 3) + ((int)y<<4) + ((int)y<<1);

	if (c==1)
	  screen[i] = screen[i] | pos[x & 7];
    else if (c==0)
	  screen[i] = screen[i] & ~pos[x & 7];
    else
	  screen[i] = screen[i] ^ pos[x & 7];
}

//==================================
//plot a line 
//at x1,y1 to x2,y2 with color 1=white 0=black 2=invert 
//NOTE: this function requires signed chars   
//Code is from David Rodgers,
//"Procedural Elements of Computer Graphics",1985
void video_line(char x1, char y1, char x2, char y2, char c) {
	int e;
	signed int dx,dy,j, temp;
	signed char s1,s2, xchange;
    signed int x,y;
        
	x = x1;
	y = y1;
	
	//take absolute value
	if (x2 < x1) {
		dx = x1 - x2;
		s1 = -1;
	}

	else if (x2 == x1) {
		dx = 0;
		s1 = 0;
	}

	else {
		dx = x2 - x1;
		s1 = 1;
	}

	if (y2 < y1) {
		dy = y1 - y2;
		s2 = -1;
	}

	else if (y2 == y1) {
		dy = 0;
		s2 = 0;
	}

	else {
		dy = y2 - y1;
		s2 = 1;
	}

	xchange = 0;   

	if (dy>dx) {
		temp = dx;
		dx = dy;
		dy = temp;
		xchange = 1;
	} 

	e = ((int)dy<<1) - dx;  
	 
	for (j=0; j<=dx; j++) {
		video_pt(x,y,c);
		 
		if (e>=0) {
			if (xchange==1) x = x + s1;
			else y = y + s2;
			e = e - ((int)dx<<1);
		}

		if (xchange==1) y = y + s2;
		else x = x + s1;

		e = e + ((int)dy<<1);
	}
}

//==================================
// put a big character on the screen
// c is index into bitmap
void video_putchar(char x, char y, char c) { 
    char i;
	char y_pos;
	uint8_t j;

	for (i=0;i<7;i++) {
        y_pos = y + i;

		j = pgm_read_byte(((uint32_t)(ascii)) + c*7 + i);

        video_pt(x,   y_pos, (j & 0x80)==0x80);  
        video_pt(x+1, y_pos, (j & 0x40)==0x40); 
        video_pt(x+2, y_pos, (j & 0x20)==0x20);
        video_pt(x+3, y_pos, (j & 0x10)==0x10);
        video_pt(x+4, y_pos, (j & 0x08)==0x08);
    }
}

//==================================
// put a string of big characters on the screen
void video_puts(char x, char y, char *str) {
	char i;
	for (i=0; str[i]!=0; i++) { 
		video_putchar(x,y,str[i]);
		x = x+6;	
	}
}
      
//==================================
// put a small character on the screen
// x-coord must be on divisible by 4 
// c is index into bitmap
void video_smallchar(char x, char y, char c) { 
	char mask;
	int i=((int)x>>3) + ((int)y<<4) + (((int)y)<<1);

	if (x == (x & 0xf8)) mask = 0x0f;     //f8
	else mask = 0xf0;
	
	uint8_t j = pgm_read_byte(((uint32_t)(smallbitmap)) + c*5);
	screen[i]    =    (screen[i] & mask) | (j & ~mask);

	j = pgm_read_byte(((uint32_t)(smallbitmap)) + c*5 + 1);
   	screen[i+18] = (screen[i+18] & mask) | (j & ~mask);

	j = pgm_read_byte(((uint32_t)(smallbitmap)) + c*5 + 2);
    screen[i+36] = (screen[i+36] & mask) | (j & ~mask);
    
	j = pgm_read_byte(((uint32_t)(smallbitmap)) + c*5 + 3);
	screen[i+54] = (screen[i+54] & mask) | (j & ~mask);
   	
	j = pgm_read_byte(((uint32_t)(smallbitmap)) + c*5 + 4);
	screen[i+72] = (screen[i+72] & mask) | (j & ~mask); 
}

//==================================
// put a string of small characters on the screen
// x-cood must be on divisible by 4 
void video_putsmalls(char x, char y, char *str) {
	char i;
	x = x & 0b1111100; //make it divisible by 4
	for (i = 0; str[i] != 0; i++) {
		if (str[i] >= 0x30 && str[i] <= 0x3a) 
			video_smallchar(x, y, str[i] - 0x30);

        else video_smallchar(x, y, str[i]-0x40+12);
		x += 4;	
	}
}

//==================================
//return the value of one point 
//at x,y with color 1=white 0=black 2=invert
char video_set(char x, char y) {
	//The following construction 
  	//detects exactly one bit at the x,y location
	int i = (x>>3) + ((int)y<<4) + ((int)y<<1);

    return (screen[i] & 1<<(7-(x & 0x7)));   	
}

//=== fixed point mult ===============================
int multfix(int a, int b) {
  int result1 = a * b;
  int result2 = (a>>8) * (b>>8);

  return (result2 << 8) | (result1 >> 8);
} 


//=== fixed conversion macros ========================================= 
#define int2fix(a)   (((int)(a))<<8)            //Convert char to fix. a is a char
#define fix2int(a)   ((signed char)((a)>>8))    //Convert fix to int. a is an int
#define float2fix(a) ((int)((a)*256.0))         //Convert float to fix. a is a float
#define fix2float(a) (((float)(a))/256.0)       //Convert fix to float. a is an int   

//=== animation stuff ==================================================
unsigned int x, y;
int vx, vy, g, drag;
char cu1[]="Cornell  ECE 4760";
char cu2[]="COPYRIGHT:2008";
///////////////

//==================================         
// set up the ports and timers
int main() {
  //init timer 1 to generate sync
  OCR1A = lineTime; 	//One NTSC line
  
  TCCR1B = 0x09; 	//full speed; clear-on-match
  TCCR1A = 0x00;	//turn off pwm and oc lines
  TIMSK = 0x02;	//enable interrupt T1 cmp A

  //init ports
  DDRD = 0xf0;		//video out

  DDRB = 0x00;      //switches

  DDRA = 0xff;      //LEDs

  //initialize synch constants 
  LineCount = 1;

  syncON = 0b00000000;
  syncOFF = 0b01000000;
  
  //Print "CORNELL" message
  video_puts(13,2,cu1);

  //Print "Copyright" message
  video_putsmalls(86,192,cu2);

  //side lines
  #define width 142
  #define height 199

  video_line(0,0,0,height,1);
  video_line(width,0,width,height,1);

  video_line(0,10,width,10,1);
  video_line(0,0,width,0,1);
  video_line(0,height,width,height,1);

  //video_line(0,0,width,height,1); // tst diagonal

  //init animation
  // initial position
  x = int2fix(20);
  y = int2fix(40);

  // initial velocity
  vx = float2fix(2.5);
  vy = float2fix(0.0);

  // gravity and drag
  g = float2fix(0.03);  
  drag = float2fix(0.004);

  video_pt(fix2int(x),fix2int(y),2); 
  ///////////////////////
  
  // Set up single video line timing
  sei();
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();


  //The following loop executes once/video line during lines
  //1-230, then does all of the frame-end processing
  for(;;) {
  
    //stall here until next line starts  
    //use sleep to make entry into sync ISR uniform time  
	
	sleep_cpu();

	//The following code executes during the vertical blanking
    //Code here can be as long as  
    //a total of 60 lines x 63.5 uSec/line x 8 cycles/uSec 
    
	if (LineCount == 231) { 

        //animation
		//erase old ball using XOR mode
        video_pt(fix2int(x),fix2int(y),2);
		 
        // new vel = old vel + acceleration*dt (dt=1)
        vy = vy + g - multfix(drag,vy);
        vx = vx - multfix(drag,vx);
                
		// new pos = old pos + velocity*dt (dt=1)
        x = x + vx;
        y = y + vy;
		
		// Bounds check -- fixed notation
		if (x>0x8d00) { vx = -vx; x=0x8d00; }    // right edge
        if (x<0x0100) { vx = -vx; x=0x0100; }    // left edge
        if (y>0xc600) { vy = -vy; y=0xc600; }    // bottom edge

		video_pt(fix2int(x),fix2int(y),2);
		
		/////////////////////////////////////////

		
     }  //line 231
  }  //for
}  //main