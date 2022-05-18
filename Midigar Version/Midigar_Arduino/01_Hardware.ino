//---------------------------------------------------------------
//   SCREEN
//---------------------------------------------------------------

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 64, 17, 19, 18, 15, 16);

void screenInitialize() {
	display.begin(SSD1306_SWITCHCAPVCC);
	display.clearDisplay();
    display.setTextSize(1);             
  	display.setTextColor(SSD1306_WHITE);        
  	display.setCursor(0,32);  
  	display.println(F(" MIDIGAR"));
  	display.display();
}

void screenPrint(String text) {
	display.clearDisplay();
	display.setCursor(0,32);  
  	display.println(text);
	display.display();
}


//---------------------------------------------------------------
//   LEDS
//---------------------------------------------------------------

#include <Max72xxPanel.h>

byte csPin = 21;
byte horiDisplays = 3;
byte vertiDisplays = 1;
Max72xxPanel leds = Max72xxPanel (csPin, horiDisplays, vertiDisplays);

void ledsInitialize() {
	leds.setIntensity(10);
    leds.setPosition(0, 2, 0);
    leds.setPosition(1, 1, 0);
    leds.setPosition(2, 0, 0);
    leds.setRotation(0, 3);
    leds.setRotation(1, 3);
    leds.setRotation(2, 3);
    leds.write();	
}


//---------------------------------------------------------------
//   BUTTONS
//---------------------------------------------------------------

#include <Keypad.h>

const byte ROWS = 8;
const byte COLS = 18;
byte rowPins[ROWS] = {38, 36, 37, 32, 35, 33, 31, 34};
byte colPins[COLS] = {26, 10, 29, 9, 8, 30, 5, 12, 27, 28, 6, 4, 40, 7, 2, 3, 24, 25};

char keys[ROWS][COLS] { 
    {  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17},
    { 18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35}, 
    { 36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53},
    { 54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71}, 
    { 72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89}, 
    { 90,  91,  92,  93,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107}, 
    {108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125},
    {126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143}
};

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void buttonsUpdate() {
	if (kpd.getKeys()) {
    	for (int i=0; i<10; i++) {
      		if (kpd.key[i].stateChanged) {
      			byte location = kpd.key[i].kchar;
                switch (kpd.key[i].kstate) {  
                	case PRESSED:
                    nodes[location].physicalState = 1;
            	break;
                	case RELEASED:
                    nodes[location].physicalState = 0;
            	break;
        			case HOLD:
        		break;
    				case IDLE:
    			break;
                }
      		}
   	 	}
  	}
}
