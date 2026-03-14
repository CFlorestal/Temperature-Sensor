#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define slp(void) __asm__ __volatile("sleep");
#define UP_BTN PORTB5
#define DOWN_BTN PORTB4
#define SET_BTN PORTB3
#define PWR_BTN PORTB2

//**************** GLOBAL VARIABLES ************
volatile unsigned char pwr_btn_active = 0;
volatile unsigned char set_btn_active = 0;

uint16_t temp = 0; 

void disableADC();
void enableADC();


//****************** BUTTONS ********************
void gpioConfig(){
	DDRB &= ~(1 << SET_BTN) | ~(1 << UP_BTN) | ~(1 << DOWN_BTN) | ~(1 << PWR_BTN);	//set the buttons pins as inputs.
	PORTB |= 1 << SET_BTN | 1 << UP_BTN | 1 << DOWN_BTN | 1 << PWR_BTN;		//set the buttons pins HIGH, pullup resistor.
}

//**************** SLEEP FUNCTIONS *********************
void sleepConfig(){
	//power down mode
	SMCR |= 1 << SM1;
	SMCR &= ~(1 << SM2) | ~(1 << SM0);
}
void sleep(){
	//update necessary variables 
	pwr_btn_active = 0;
	set_btn_active = 0;
	
	//turn off ADC, Timers, GPIO, BOD, etc., then sleep.
	disableADC();

	PRR |= 1 << PRTWI | 1 << PRTIM2 | 1 << PRTIM0 | 1 << PRTIM1 | 1 << PRSPI | 1 << PRUSART0 | 1 << PRADC; //setting power reduction for peripherals 
	//Disabling BOD
	MCUCR |=  1 << BODSE | 1 << BODS; 
	MCUCR |= 1 << BODS  | (MCUCR & ~( 1 << BODSE));
	
	SMCR |= 1 << SE;		//enable sleep 
	slp();				//sleep instruction
}
void wakeUp(){
	//turn on peripherals
	SMCR &= ~(1 << SE);	//disable sleep mode
	PRR &= ~(1 << PRTWI) | ~(1 << PRTIM2) | ~(1 << PRTIM0) | ~(1 << PRTIM1) | ~(1 << PRSPI) | ~(1 << PRUSART0) | ~(1 << PRADC); //removing power reduction for peripherals 
	enableADC();
}



//********** INTERRUPT FUNCTIONS ****************

void interruptConfig(){
	PCICR |= 1 << PCIE0;				//set pin change interrupt enable 0
	PCMSK0 |= 1 << PCINT3 | 1 << PCINT2;		//enabling pin change interrupts for specific pins 3,2
	//PCMSK0 |= 1 << PCINT0;
	sei(); 	//enabling global interrupts
}

//**************** ADC FUNCTIONS ****************
void adcConfig(){
	ADCSRA |= 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0; 		//Prescaler 128, 16MHz clock / 128 = 150kHz sample rate, between recommended 50k - 200k sample rate 
	ADCSRB &= ~(1 << ADTS2) | ~(1 << ADTS1) | ~(1 << ADTS0); 	//Free running mode 
	ADMUX |= 1 << REFS0; 						//AVcc as voltage reference
	ADCSRA |= 1 << ADEN | 1 << ADATE;				//ADC enabled, Auto Triggering 
}

void enableADC(){
	ADCSRA |= 1 << ADEN;						//ADC enabled 
}
void disableADC(){
	ADCSRA &= ~(1 << ADEN);						//ADC disabled  
}
void startADC(){
	ADCSRA |= 1 << ADSC;						//start adc conversion  
}

void captureADCValues(){
	//when conversion is complete, ADC interrupt flag goes high
	//capture conversion result 
	if(ADCSRA & ADIF) {
		temp = (ADCH << 8) | ADCL; 		
	}
}

//**************** LCD FUNCTIONS ****************
void lcdBacklight(){

}
void displayMenu(){

}
void displayTemp(){

}

//************** TEMP FUNCTIONS *****************
float getCelsius(){
	return 0.0;
}
float getFaherenheit(){
	return 0.0;
}

//************** OTHER FUNCTIONS ****************
void _init(){
	gpioConfig();
	interruptConfig();
	sleepConfig();
	adcConfig();
}

void powerBtnPress(){
	//if the power button is not pressed, leave;
	if(PINB & (1 << PWR_BTN)){
		pwr_btn_active = 0;
		return;
	}
	//if it hasn't been at least 3 seconds or more leave 
	
	//put device in sleep (power down) mode 
	sleep();
}
void program(){

}

void run(){
	if(pwr_btn_active)
		powerBtnPress();
	else 
		program();
}

//************** MAIN FUNCTION ****************

int main(){
	_init();
	while(1)
		program();
	return 0;
}

ISR(PCINT0_vect){
	pwr_btn_active = (!(PINB & PWR_BTN)) ? 1 : 0;
	set_btn_active = (!(PINB & SET_BTN)) ? 1 : 0; 
}
