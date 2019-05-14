#include "LedBlinking.h"

  
  // Constructor - creates a Flasher 
  // and initializes the member variables and state
  LedBlinking::LedBlinking()
  {
	ledPin = 13;
	pinMode(ledPin, OUTPUT);     
	  
	OnTimeLed = 1000;
	OffTimeLed = 1000;
	
	ledState = LOW; 
	previousMillisLed = 0;
  
  
  }

  LedBlinking::LedBlinking(int pin, unsigned long timeOn, unsigned long timeOff)
  {
	ledPin = pin;
	pinMode(ledPin, OUTPUT);     
	  
	OnTimeLed = timeOn;
	OffTimeLed = timeOff;
	
	ledState = LOW; 
	previousMillisLed = 0;
  blink = true;
  unlimitedBlink = true;
  }

  LedBlinking::LedBlinking(int pin, unsigned long timeOn, unsigned long timeOff, unsigned long delayToBlink)
  {
    ledPin = pin;
	  pinMode(ledPin, OUTPUT);     
	  
	  OnTimeLed = timeOn;
	  OffTimeLed = timeOff;
	
	  ledState = LOW; 
	  previousMillisLed = 0;
    delayToBlinkLocal = delayToBlink;

  }
  LedBlinking::LedBlinking(int pin, unsigned long timeOn, unsigned long timeOff, int numberOfBlink){
    ledPin = pin;
	  pinMode(ledPin, OUTPUT);  
     
	  OnTimeLed = timeOn;
	  OffTimeLed = timeOff;
    if (numberOfBlink == 0){
      unlimitedBlink = true;
      blink = true;
    }
    else if(numberOfBlink > 1){
      unlimitedBlink = false;
      blink = false;
      nbrOfBlink = numberOfBlink;
    }
    else {
      unlimitedBlink = false;
      nbrOfBlink = numberOfBlink;
      blink = false;
    }
    
	  
	  ledState = LOW; 
	  previousMillisLed = 0;
    
    count = 0;
    
  }

   // temps on + temps off + nombre de clignotement => OK
   void LedBlinking::UpdateLed()
  {
    keypadEventListener(nbrOfBlink);
    // check to see if it's time to change the state of the LED
    unsigned long currentMillisLed = millis();
     
    if((ledState == HIGH) && (currentMillisLed - previousMillisLed >= OnTimeLed))
    {
    	ledState = LOW;  // Turn it off
      previousMillisLed = currentMillisLed;  // Remember the time
      
      //SerialUSB.println("count = " + String(count));
      //SerialUSB.println("nbrOfBlink = " + String(nbrOfBlink));
      //SerialUSB.println("unlimitedBlink = " + String(unlimitedBlink));
      
      if(blink){
        digitalWrite(ledPin, ledState);	  // Update the actual LED
      }
      
    }
    else if ((ledState == LOW) && (currentMillisLed - previousMillisLed >= OffTimeLed))
    {
      ledState = HIGH;  // turn it on
      previousMillisLed = currentMillisLed;   // Remember the time
      if(blink && ((count < nbrOfBlink) || unlimitedBlink)){
        digitalWrite(ledPin, ledState);	  // Update the actual LED
        count++;
      }else{
        blink = false;
      }
      
    }
  }


  void LedBlinking::Blink(){
    blink = true;
    count = 0;
  }


  void LedBlinking::addEventListener(void (*listener)(char)){
	keypadEventListener = listener;
}

  