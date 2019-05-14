#ifndef __LEDBLINKING_H__
#define __LEDBLINKING_H__

#include <Arduino.h>

/*  HOW TO USE:
    FOR USE LedBlinking(), JUST USE VAR.UpdateLed() FOR LED_BUILTIN BLINK ONE SECOND ON AND ONE SECOND OFF UNLIMITED TIME
    
    FOR USE LedBlinking(int pin, unsigned long on, unsigned long off)
    JUST USE VAR.UpdateLed()
    
    pin = pin to blinking
    on = statement "on" of pin
    off = statement "off" of pin
    FOR UNLIMITED TIME

    FOR USE LedBlinking(int pin, unsigned long timeOn, unsigned long timeOff, int numberOfBlink)
    USE VAR.UpdateLed()
    AND TRIGER WITH VAR.Blink()

    pin = pin to blinking
    timeOn = statement "on" of pin
    timeOff = statement "off" of pin
    numberOfBlink = FOR numberOfBlink TIME

    LedBlinking(int pin, unsigned long timeOn, unsigned long timeOff, unsigned long delayToBlink);
    TO DO
*/

typedef char KeypadEvent; 

class LedBlinking{
    // Class Member Variables
	// These are initialized at startup
	private:
    int ledPin;      // the number of the LED pin
	unsigned long OnTimeLed;     // milliseconds of on-time
	unsigned long OffTimeLed;    // milliseconds of off-time
 
	// These maintain the current state
	int ledState;             		// ledState used to set the LED
	unsigned long previousMillisLed;  	// will store last time LED was updated
    unsigned long delayToBlinkLocal;
    int nbrOfBlink;

    bool blink;
    bool unlimitedBlink;
    int count;

    
    void (*keypadEventListener)(char);
    

    public:
    LedBlinking();
    LedBlinking(int pin, unsigned long on, unsigned long off);
    LedBlinking(int pin, unsigned long timeOn, unsigned long timeOff, unsigned long delayToBlink);
    LedBlinking(int pin, unsigned long timeOn, unsigned long timeOff, int numberOfBlink);
    void UpdateLed();
    void Blink();

    
    void addEventListener(void (*listener)(char));
};

#endif 