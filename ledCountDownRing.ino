// LED count down ring controller
// Ilya Brutman
// 2014

// --------------------------------------------------------------------
// This code is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this code.  If not, see
// <http://www.gnu.org/licenses/>.
// --------------------------------------------------------------------

#include "TaskScheduler.h"
#include "LedDisplay.h"
#include "LedFuse.h"
#include "PhotoCountSelectMonitor.h"
#include "ColorSelectMonitor.h"
#include "StartButtonMonitor.h"
#include "config.h"

#define A3 3
#define A4 2

// Pins
// ============================================================================
const uint8_t ledRingPin          = 0; //led ring out
const uint8_t outputTriggerPin	  = 1; //Output trigger
const uint8_t startButtonPin	  = 2; //Start Button
const uint8_t colorSelectPin 	  = A3; //pot color select (PB3)
const uint8_t photoCountSelectPin = A4; //pot photo count select (PB4)

void setup()
{	
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
}//end setup



void loop()
{
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMBER_OF_LEDS, ledRingPin, NEO_GRB + NEO_KHZ800); 
    
    TaskScheduler           scheduler;
    LedDisplay              ringDisplay(&strip);
    LedFuse                 fuse(scheduler, ringDisplay);
    ColorSelectMonitor      colorSelector(scheduler, ringDisplay, colorSelectPin);
    PhotoCountSelectMonitor photoCountSelector(scheduler, ringDisplay, photoCountSelectPin, colorSelector);
    StartButtonMonitor      theStartButtonMonitor(scheduler, fuse, photoCountSelector, colorSelector, startButtonPin, outputTriggerPin);
    
    ringDisplay.setOffset(LED_START_OFFSET);
    theStartButtonMonitor.setCountDownTime(COUNT_DOWN_SECONDS * 1000);
    
    // Start the task scheduler loop. Does not return.
    scheduler.doEventLoop();

}//end loop






