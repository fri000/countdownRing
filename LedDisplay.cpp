#include "LedDisplay.h"

//arduino include for digitalWrite and associated constants.
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include <pins_arduino.h>
#endif

LedDisplay::LedDisplay(Adafruit_NeoPixel* pStrip):
    // Parameter 1 = number of pixels in strip
    // Parameter 2 = pin number (most are valid)
    // Parameter 3 = pixel type flags, add together as needed:
    //   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
    //   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
    //   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
    //   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
    m_offset(0),
    m_validToken(0),
    m_pStrip(pStrip)
{   
}//end constructor


LedDisplay::~LedDisplay()
{
}//end destructor


// Use the software offset when the first visual pixel is not wired as the first led in the chain.
bool LedDisplay::setOffset(uint8_t newOffset)
{
    if(newOffset < m_pStrip->numPixels())
    {
        m_offset = newOffset;
        return true;
    }
    return false;   
}//end setOffset


//hands out a token that is used to draw on the display.
// Token expires when this function is called to generate a new one.
uint32_t LedDisplay::grabFocus()
{
    m_validToken++;
    return m_validToken;
}//end grabFocus


void LedDisplay::refresh(uint32_t drawToken)
{
    if(drawToken != m_validToken)
    {
        return; //we got an expired token. Do nothing.
    }
    
    m_pStrip->show();    
}//end refresh



void LedDisplay::setPixel(uint32_t drawToken, int16_t whichPixel, uint32_t color)
{
    if(drawToken != m_validToken)
    {
        return; //we got an expired token. Do nothing.
    }

    uint8_t numPixels = m_pStrip->numPixels(); //cache

    // Make sure the requested pixel is within the valid range
    if( (whichPixel >=0) && (whichPixel < numPixels) )
    {
        // Keep going with the function
    }
    else
    {
        // The pixel position requested doesn't match up with a physical pixel.
        // Do nothing.
        return;
    }

    // Apply our pixel offset
    whichPixel += m_offset;

    uint8_t lastIndex = numPixels - 1;
    
    // Wrap around if we need to
    if (whichPixel > lastIndex)
    {
        whichPixel -= numPixels;
    }

    m_pStrip->setPixelColor(whichPixel, color);
    
}//end setPixel


// Old version
// todo: delete this if the new version works
// void LedDisplay::setPixel(uint32_t drawToken, int16_t whichPixel, uint32_t color)
// {
//     if(drawToken != m_validToken)
//     {
//         return; //we got an expired token. Do nothing.
//     }

//     // Apply our pixel offeset
//     whichPixel += m_offset;
    
//     // we accept negative numbers, which means that we want to address the pixel position from the end
//     // Convert the negative number to a positive pixel position.
//     if(whichPixel < 0)
//     {
//         //negate the number to make it positive
//         whichPixel = 0 - whichPixel;

//         //make sure that this new positive number is within range
//         if ( whichPixel < m_pStrip->numPixels())
//         {
//             whichPixel = m_pStrip->numPixels() - whichPixel;
//             //If we have 24 leds then
//             // position 0 is the first led.
//             // position -1 is the last, 24th, led. In our case this is index 23.
//         }
//     }

//     // Now that we have a pixel position, make sure it is within the valid range.
//     //if bigger than 0 and less than the max
//     if( (whichPixel >=0) && (whichPixel < m_pStrip->numPixels()) )
//     {
//         m_pStrip->setPixelColor(whichPixel, color);
//     }
//     else
//     {
//         // The pixel position requested doesn't match up with a physical pixel.
//         // Do nothing.
//     }
// }//end setPixel


// Clears the internal buffer.
// Does not change the state of the physical leds.
void LedDisplay::clearDisplay(uint32_t drawToken)
{
    if(drawToken != m_validToken)
    {
        return; //we got an expired token. Do nothing.
    }
    
    uint32_t blackColor = 0;
    
    for(int i = 0; i < m_pStrip->numPixels(); i++)
    {
        m_pStrip->setPixelColor(i, blackColor);
    }
}//end clearDisplay


uint8_t LedDisplay::numOfPixels()
{
    return m_pStrip->numPixels();
}//end numOfPixels