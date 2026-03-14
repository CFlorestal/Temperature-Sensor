#define F_CPU 1000000UL
#include "lcd.h"
#include <util/delay.h>

//****** FUNCTIONS IMPLEMENTATIONS *********

void setDataPort(volatile unsigned char * dataPort){
	_dataPort = dataPort;
}
void setControlPort(volatile unsigned char * controlPort, unsigned char rw,unsigned char rs, unsigned char en){
	_controlPort = controlPort;
	_rw = rw;
	_rs = rs;
	_en = en;
}

void set_1602_lcd_default_config(){
}
void lcdInitialization(){
	_delay_ms(50);								//1) Wait 50ms, after LCD initial power up.
	sendInstruction(0x30);							//2) Acknowledging 8-bit mode.
	_delay_ms(5);								//3) Wait 5ms > 4.1ms.
	sendInstruction(0x30);							//4) Acknowledging 8-bit mode.
	_delay_ms(5);								//5) Wait 5ms > 4.1ms.
	sendInstruction(0x30);							//6) Acknowledging 8-bit mode.
	_delay_ms(1);								//7) Wait 1ms > 100us.
	sendInstruction(0x02);							//8) Set to 4-bit mode.

	sendInstruction(_displayFunction);					//9) Setting function set.
	sendInstruction(_displayControl);					//10) Setting display control.
	sendInstruction(_displayMode);						//11) Setting display entry mode set.

	_backlight = LCD_BACKLIGHT_ON;						//12) LCD backlight turned on, after configuration.

	clearDisplay();								//13) Clear display to finish initialization.
}

void flashEnable(){
	*_controlPort &= ~(1 << _en);
	_delay_ms(1);
	*_controlPort |= 1 << _en;
	_delay_ms(1);
	*_controlPort &= ~(1 << _en);
	_delay_ms(1);
}
void writeNibble(const unsigned char data){
	//we are only worried about the lower 4 bits
	for(int i = 0; i < 4; i++)
		//write output to the lower 4 elements in the data array
		flashEnable();	//pulse enable pin for chip to update the information
}
void writeByte(const unsigned char data){
	for(int i = 0; i < 8; i++)
		//write output to all the 8 elements in the data array
		flashEnable();//pulse enable pin for chip to update the information
}

void transmit(const unsigned char data, const unsigned char mode){
	//PORTC = (mode) ? (PORTB | rs) : (PORTC & ~rs);
	if(_displayControl & LCD_4BITMODE)
		for(int i = 0; i < 4; i++){
			//PORTB
			//
			for(int i = 0; i < 8; i++){
				//PORTB
			}
		}
}
void sendInstruction(const unsigned char command){
	transmit(command,0);
}
void sendCharacter(const unsigned char character){
	transmit(character,1);
}
void printString(char * string){
	while(*string)
		sendCharacter(*string++);
}
void backlightOn(){
	sendInstruction(LCD_BACKLIGHT_ON);
}
void backlightOff(){
	sendInstruction(LCD_BACKLIGHT_OFF);
}
void clearDisplay(){
	sendInstruction(LCD_CLEARDISPLAY);
	//delay needs to be added for instructions
}
void returnHome(){
	sendInstruction(LCD_RETURNHOME);
	//delay needs to be added for instructions
}
void displayOn(){
	_displayControl |= LCD_DISPLAYON;
	sendInstruction(_displayControl);
}
void displayOff(){
	_displayControl &= ~LCD_DISPLAYON;
	sendInstruction(_displayControl);
}
void blinkOn(){
	_displayControl |= LCD_BLINKON;
	sendInstruction(_displayControl);
}
void blinkOff(){
	_displayControl &= ~LCD_BLINKON;
	sendInstruction(_displayControl);
}
void cursorOn(){
	_displayControl |= LCD_CURSORON;
	sendInstruction(_displayControl);
}
void cursorOff(){
	_displayControl &= ~LCD_CURSORON;
	sendInstruction(_displayControl);
}
void cursorScrollLeft(){
	sendInstruction(LCD_SCROLLLEFT | LCD_CURSORMOVE | LCD_CURSORSHIFT);
}
void cursorScrollRight(){
	sendInstruction(LCD_SCROLLRIGHT | LCD_CURSORMOVE | LCD_CURSORSHIFT);
}
void displayScrollLeft(){
	sendInstruction(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_SCROLLLEFT);
}
void displayScrollRight(){
	sendInstruction(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_SCROLLRIGHT);
}
void printRightToLeft(){
	_displayMode |= LCD_ENTRYLEFT;
	sendInstruction(_displayMode);
}
void printLeftToRight(){
	_displayMode |= LCD_ENTRYRIGHT;
	sendInstruction(_displayMode);
}
void shiftOn(){
	_displayMode |= LCD_ENTRY_SHIFTON;
	sendInstruction(_displayMode);
}
void shiftOff(){
	_displayMode &= ~LCD_ENTRY_SHIFTON;
	sendInstruction(_displayMode);
}
void setCursorPosition(const unsigned char row, const unsigned char col){
	const unsigned char row_offset[4] = {0x00,0x40,0x14,0x54};
	sendInstruction(LCD_SET_DDRAMADDR | ((col-1) + row_offset[row-1]));
}
void create_char(unsigned char position, const unsigned char  char_matrix[]){
	position &= 7;
	sendInstruction(LCD_SET_CGRAMADDR | (position << 3));
	for(unsigned char i = 0; i < 8; i++)
		transmit(char_matrix[i],0);
}
