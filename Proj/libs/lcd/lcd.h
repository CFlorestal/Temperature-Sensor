#ifndef LCD_H
#define LCD_H

#include <avr/io.h>

//INSTRUCTIONS
#define LCD_CLEARDISPLAY 	0x01
#define LCD_RETURNHOME 		0x02
#define LCD_ENTRYMODESET 	0x04
#define LCD_DISPLAYCONTROL 	0x08
#define LCD_CURSORSHIFT 	0x10
#define LCD_FUNCTIONSET 	0x20
#define LCD_SET_CGRAMADDR 	0x40
#define LCD_SET_DDRAMADDR 	0x80

//DISPLAY ENTRY MODE   
#define LCD_ENTRYLEFT 		0x02
#define LCD_ENTRYRIGHT		0x00  
#define LCD_ENTRY_SHIFTON 	0x01
#define LCD_ENTRY_SHIFTOFF 	0x00

//DISPLAY CONTROL 
#define LCD_DISPLAYON 		0x04 
#define LCD_DISPLAYOFF 		0x00 
#define LCD_CURSORON 		0x02
#define LCD_CURSOROFF 		0x00
#define LCD_BLINKON 		0x01
#define LCD_BLINKOFF 		0x00

//DISPLAY & CURSOR MOTION
#define LCD_DISPLAYMOVE 	0x08
#define LCD_CURSORMOVE 		0x00
#define LCD_SCROLLRIGHT 	0x04
#define LCD_SCROLLLEFT 		0x00

//FUNCTION SET
#define LCD_8BITMODE 		0x10
#define LCD_4BITMODE 		0x00
#define LCD_2LINE 		0x08
#define LCD_1LINE 		0x00
#define LCD_5x10DOTS 		0x04
#define LCD_5x8DOTS 		0x00

//BACKLIGHT BIT
#define LCD_BACKLIGHT_ON 	0x08
#define LCD_BACKLIGHT_OFF 	0x00

//CONTROL SELECTION BITS
#define LCD_EN			0x04
#define LCD_RW			0x02
#define LCD_RS 			0x01

//********** DATA STRUCTURES *********
typedef struct {} LCD_CTRL;

//********** GLOBAL VARIABLES **********
unsigned char _displayControl;
unsigned char _displayMode;
unsigned char _displayFunction;

unsigned char _backlight;
unsigned char _rw, _rs, _en;		//Read/white, Register Select, and Enable.

volatile unsigned char * _controlPort;	//AVR port that will be used to control LCD. R/W, RS, and EN pins are used on this port. 
volatile unsigned char * _dataPort;	//AVR 8-bit port that will transmit data to LCD, D0,D1,D2,D3...D7, LSB - MSB

//************ FUNCTIONS ***************
extern void setDataPort(volatile unsigned char * dataPort);
extern void setControlPort(volatile unsigned char * controlPort, unsigned char rw,unsigned char rs, unsigned char en);

extern void set_1602_lcd_default_config();
extern void lcdInitialization();

extern void flashEnable();
extern void writeNibble(const unsigned char data);
extern void writeByte(const unsigned char data);
extern void transmit(const unsigned char data, const unsigned char mode);
extern void sendInstruction(const unsigned char command);
extern void sendCharacter(const unsigned char character);
extern void printString(char * string);

extern void backlightOn();
extern void backlightOff();
extern void clearDisplay();
extern void returnHome();
extern void displayOn();
extern void displayOff();
extern void blinkOn();
extern void blinkOff();
extern void cursorOn();
extern void cursorOff();
extern void cursorScrollLeft();
extern void cursorScrollRight();
extern void displayScrollLeft();
extern void displayScrollRight();
extern void printRightToLeft();
extern void printLeftToRight();
extern void shiftOn();
extern void shiftOff();
extern void setCursorPosition(const unsigned char row, const unsigned char col);
extern void create_char(const unsigned char position, const unsigned char  char_matrix[]);


#endif //LCD_H
