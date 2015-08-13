// LED count down ring controller
// Ilya Brutman
// 2014

#include "TaskScheduler.h"
#include "LedDisplay.h"
#include "LedFuse.h"
#include "PhotoCountSelectMonitor.h"
#include "ColorSelectMonitor.h"
#include "StartButtonMonitor.h"
#include "config.h"
//#include <MemoryFree.h> //for debug
//#include "AttinySoftwareSerial.h" //for debug, hard coded to write serial to PB1 @ 9600 baud


#define A3 3
#define A4 2

// Pins
// ============================================================================
const uint8_t ledRingPin          = 0; //led ring out
const uint8_t outputTriggerPin	  = 1; //Output trigger
const uint8_t startButtonPin	  = 2; //Start Button
const uint8_t colorSelectPin 	  = A3; //pot color select (PB3)
const uint8_t photoCountSelectPin = A4; //pot photo count select (PB4)

// AttinySoftwareSerial mySerial(0);

int16_t previousReading = 0; //debug

void setup()
{	
    //DEBUG START
    //strip.begin();
    //strip.show(); // Initialize all pixels to 'off'
    //DEBUG END
    
    // Put the output pins into output mode.
    // GPIO pins start out in this state by default,
    // but tell them to go there anyway for good measure.
    pinMode(ledRingPin,         OUTPUT);
    pinMode(outputTriggerPin,   OUTPUT);

    
    // Put the input pins into input mode.
    pinMode(startButtonPin, INPUT);
    pinMode(colorSelectPin, INPUT);
    pinMode(photoCountSelectPin, INPUT);


    //Turn on the internal pull up resistor on the following input pins.
    //The startButtonPin gets pulled to ground when pressed.
    digitalWrite(startButtonPin, HIGH);
     

    //Start up the serial interface. This is used only for debugging.
    //Serial.begin(115200);
    //Serial.println("ready for action"); delay(20);
  
}//end setup




//DEBUG function
// Fill the dots one after the other with a color
//void colorWipe(uint32_t c, uint8_t wait) {
//  for(uint16_t i=0; i<strip.numPixels(); i++) {
//      strip.setPixelColor(i, c);
//      strip.show();
//      delay(wait);
//  }
//}

//void debugFunction(void* clientData)
//{
//    mySerial.print("Ram = ");
//    mySerial.println(freeMemory());
//    int16_t newReading = analogRead(colorSelectPin);
//    int numLeds = map(newReading, 0, 1023, 0, 16);
//    uint32_t color = strip.Color(128, 0, 0);
//
//    theBarDisplay.setColor(color);
//    if( (newReading > (previousReading + 2)) ||
//        (newReading < (previousReading - 2))
//       )
//    {
//        theBarDisplay.setLength(numLeds);
//        previousReading = newReading;
//    }
//    
//    theTaskScheduler.scheduleDelayedTask(debugFunction, 0, 33);
//}//end checkAnalogPin0


//END DEBUG FUNCTION
void loop()
{
    // mySerial.println("Start");
    // mySerial.println(freeMemory());
    
    // while(1)
    // {
        // digitalWrite(outputTriggerPin, HIGH);
        // delay(1000);
        // digitalWrite(outputTriggerPin, LOW);
        // delay(1000);
    // }
    
    
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMBER_OF_LEDS, ledRingPin, NEO_GRB + NEO_KHZ800); 
    
    TaskScheduler           theTaskScheduler;
    LedDisplay              theLedDisplay(&strip);
    LedFuse                 theLedFuse(theTaskScheduler, theLedDisplay);
    ColorSelectMonitor      theColorSelectMonitor(theTaskScheduler, theLedDisplay, colorSelectPin);
    PhotoCountSelectMonitor thePhotoCountSelectMonitor(theTaskScheduler, theLedDisplay, photoCountSelectPin, theColorSelectMonitor);
    StartButtonMonitor      theStartButtonMonitor(theTaskScheduler, theLedFuse, thePhotoCountSelectMonitor, theColorSelectMonitor, startButtonPin, outputTriggerPin);
    
    theLedDisplay.setOffset(LED_START_OFFSET);
    theStartButtonMonitor.setCountDownTime(COUNT_DOWN_SECONDS * 1000);
    //DEBUG START
     
    //int16_t newReading = analogRead(colorSelectPin);
    //int numLeds = map(newReading, 0, 1023, 0, 16);
    //uint32_t color = strip.Color(128, 0, 0);
    
    //Test out the adafruit library for the led ring   
    //-------------------------------------------------------------------------
    //for(uint16_t i=0; i<numLeds; i++) {
    //    strip.setPixelColor(i, color);
    //    //strip.show();
    //}
    //for(uint16_t i=numLeds; i<strip.numPixels(); i++) {
    //    strip.setPixelColor(i, 0);
    //    //strip.show();
    //}
    //strip.show();
    
    //Test out the led display class
    //-------------------------------------------------------------------------
    //uint32_t drawToken = theLedDisplay.grabFocus();
    //for(uint16_t i=0; i<numLeds; i++) {
    //    theLedDisplay.setPixel(drawToken, i, color);
    //}
    //for(uint16_t i=numLeds; i<strip.numPixels(); i++) {
    //    theLedDisplay.setPixel(drawToken, i, 0);
    //}
    //theLedDisplay.refresh(drawToken);
    
    //Test out the bar display class
    //-------------------------------------------------------------------------
    //theBarDisplay.setColor(color);
    //if( (newReading > (previousReading + 2)) ||
    //    (newReading < (previousReading - 2))
    //   )
    //{
    //    theBarDisplay.setLength(numLeds);
    //    previousReading = newReading;
    //}
    
    //theTaskScheduler.scheduleDelayedTask(debugFunction, 0, 33);
    
    
    //colorWipe(strip.Color(255, 0, 0), 50); // Red
    //colorWipe(strip.Color(0, 255, 0), 50); // Green
    //colorWipe(strip.Color(0, 0, 255), 50); // Blue
    //return;
    //DEBUG END
    
    // Start the task scheduler loop. Does not return.
    theTaskScheduler.doEventLoop();

}//end loop






