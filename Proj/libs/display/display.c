#include <stdbool.h>
#include <stdio.h>
#define MAX_CHARS	20
#define MAX_VAL		5


typedef struct {
	char str[MAX_CHARS];		//array of characters, string
	unsigned char size; 		//number of elements in the string 
} STRING;

typedef struct {
	STRING opt; 
	STRING val[MAX_VAL];
	unsigned char index;
	unsigned char size;
} OPTION;

typedef struct {
	OPTION * opt;
} LINE;

typedef struct {
	LINE * l0;
	LINE * l1;
} DISPLAY;


//****************GLOBAL VARIABLES *************
LINE line[2];
DISPLAY monitor;

unsigned char valScrollPosition = 0;
unsigned char optScrollPosition = 0;
unsigned char displayPage = 0;
unsigned char cursorPosition = 0;


bool valueMode = false; 

char tempcfg[2] = {'c','c'};

//***************  FUNCTIONS ******************
void assignString(STRING * sStr, char cStr[], unsigned char size){
	for(int i = 0; i < size; i++)
		sStr->str[i] = cStr[i];
	sStr->size = size;
}
void deleteString(STRING * str){
	for(int i = 0; i < str->size; i++)
		str->str[i] = 0;
	str->size = 0;
}

void createOption(OPTION * opt, STRING * str){
	opt->opt = *str;
	opt->index = 0;
	opt->size = 0;
}
void addValueToOption(OPTION * opt, STRING * str){
	opt->val[opt->size] = *str;
	opt->size++;
}
void mapOptionValue(OPTION * opt, unsigned char index){
	opt->index = index;
}
void addToLine(LINE * line, OPTION * opt){
	line->opt = opt;
}




void mainScreenDisplay(){
	char options [2][MAX_CHARS] = {"TEMP 1:","TEMP 2:"};
	unsigned char optionsSizes [2] = {7,7};
}

void settingsMenuDisplay(){
	char options [5][MAX_CHARS] = {"CONFIGURATION","TEMPERATURE","CALIBRATION","POWER","BACK"};
	unsigned char optionsSizes [5] = {13,11,11,5,4};
}





void configurationMenuDisplay(){
	char options [3][MAX_CHARS] = {"TEMP 1:", "TEMP 2:", "BACK"}; 
	char values [2][MAX_CHARS] = {"ENABLED","DISABLED"};
	unsigned char optionsSizes [3] = {7,7,4};
	unsigned char valuesSizes  [2] = {7,8};

}

void temperatureMenuDisplay(bool set, bool up, bool down){
	char options [2][MAX_CHARS] = {"UNIT:","BACK"};
	char values [2][MAX_CHARS] = {"FAHRENHEIT","CELSIUS"};

	unsigned char optionsSizes [2] = {5,4};
	unsigned char valuesSizes  [2] = {10,7};
}

void calibrationMenuDisplay(){
	char options [3][MAX_CHARS] = {"CONVERSION","MAPPING","BACK"};
	char conversionVal [] = {};
	char mappingVal [] = {};
}


void printString(STRING * str){
	for(int i = 0; i < str->size; i++)
		printf("%c",str->str[i]);
}

void printLine(LINE * line){
	if(line->opt)
		printString(&line->opt->opt);
	if(line->opt->size){
		printf(" ");
		printString(&line->opt->val[line->opt->index]);
		printf(" Size: %hhu, Index: %hhu ", line->opt->size,line->opt->index);
	}
	printf("\n");
}

void printDisplay(){
	printLine(&line[0]);
	printLine(&line[1]);
	printf("{oSPos: %hhu} {vSPos : %hhu} {cPos: %hhu} {valMode : %s}\n", optScrollPosition,valScrollPosition,cursorPosition,valueMode ? "true":"false");
}

void test1(){

	STRING str[6];
	OPTION opt[3];

	assignString(&str[0],"TEMP1:",6);
	assignString(&str[1],"TEMP2:",6);
	assignString(&str[2],"BACK",4);
	assignString(&str[3],"FAHRENHEIT",10);
	assignString(&str[4],"CELSIUS",7);
	assignString(&str[5],"KELVIN",6);

	createOption(&opt[0],&str[0]);
	addValueToOption(&opt[0],&str[3]);
	addValueToOption(&opt[0],&str[4]);
	
	createOption(&opt[1],&str[1]);
	addValueToOption(&opt[1],&str[3]);
	addValueToOption(&opt[1],&str[4]);
	addValueToOption(&opt[1],&str[5]);
	
	createOption(&opt[2],&str[2]);

	mapOptionValue(&opt[0],(tempcfg[0] == 'c') ? 1 : 0);
	mapOptionValue(&opt[1],(tempcfg[1] == 'f') ? 1 : 0);
	
	addToLine(&line[0],&opt[0]);
	addToLine(&line[1],&opt[1]);

	printDisplay();
	printf("0 = {SET}, 1 = {UP}, 2 = {DOWN}, 3 = {POWER}\n");
	
	unsigned char input = 0;
	while(input < 3){
		printf("Input: ");
		scanf("%hhu",&input);

		switch(input){
			case 0:
				if(valueMode){
					if(optScrollPosition < 2) 
						tempcfg[optScrollPosition] = (valScrollPosition) ? 'f' : 'c';
					valScrollPosition = 0;
					cursorPosition = 0;
					valueMode = false;
					//updateConfigFile
				}else{
					cursorPosition = opt[optScrollPosition].opt.size + 2;
					valScrollPosition = opt[optScrollPosition].index;
					valueMode = true;
				}
				break;
			case 1: 
				if(valueMode){
					if(valScrollPosition > 0) {
						valScrollPosition--;
						unsigned char index = opt[optScrollPosition].index - 1;
						mapOptionValue(&opt[optScrollPosition],index);
					}
				}else{
					if(optScrollPosition > 0) optScrollPosition--;
				}
				break;
			case 2: 
				if(valueMode){
					if(valScrollPosition < 2) {
						valScrollPosition++;
						unsigned char index = opt[optScrollPosition].index + 1;
						mapOptionValue(&opt[optScrollPosition],index);
					}
				} else {
					if(optScrollPosition < 1) 
						optScrollPosition ++;
					
				}
				break;
			default: 
				break;

		}
		addToLine(&line[0],&opt[optScrollPosition]);
		addToLine(&line[1],&opt[optScrollPosition+1]);
		
		printDisplay();
	}
}

void display_init(){
	monitor.l0 = &line[0];
	monitor.l1 = &line[1];
}




int main() {
	test1();
	return 0;
}
