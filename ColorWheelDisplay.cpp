#include "ColorWheelDisplay.h"


// Public Functions
// ============================================================================
ColorWheelDisplay::ColorWheelDisplay(LedDisplay& display):
    m_display(display),
    m_drawToken(0),
    m_activeColor(0xFF) //we have to pick a default color, so lets pick blue.
{
}//end constructor



ColorWheelDisplay::~ColorWheelDisplay()
{
}//end destructor


bool ColorWheelDisplay::activate()
{
    m_drawToken = m_display.grabFocus();
    //blank out the display to erase the stuff someone else might have drawn 
    blank();
    return true;
}//end activate


bool ColorWheelDisplay::setWheelPosition(uint8_t color)
{
    m_activeColor = wheel(color);
    //Draw all the pixels with the selected color
    
    //every pixel
    //for(uint8_t i = 0; i < m_display.numOfPixels(); i++)
    //{
    //    m_display.setPixel(m_drawToken, i, m_activeColor);
    //}
    
    //every 2nd pixel
    for(uint8_t i = 0; i < m_display.numOfPixels(); i+=2)
    {
        m_display.setPixel(m_drawToken, i, m_activeColor);
    }
    
    //just 1 pixel
    // m_display.setPixel(m_drawToken, 14, m_activeColor);
    
    //Push the new pixels on to the display.
    m_display.refresh(m_drawToken);
    return true;
}//end setColor


bool ColorWheelDisplay::blank()
{
    m_display.clearDisplay(m_drawToken);
    //Push the new pixels on to the display.
    m_display.refresh(m_drawToken);
    return true;
}//end blank


uint32_t ColorWheelDisplay::getActiveColor()
{
    return m_activeColor;
}//end getActiveColor



// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t ColorWheelDisplay::wheel(uint8_t WheelPos)
{
    if(WheelPos < 85)
    {
        return makeColor(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
    else if(WheelPos < 170)
    {
        WheelPos -= 85;
        return makeColor(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else
    {
        WheelPos -= 170;
        return makeColor(0, WheelPos * 3, 255 - WheelPos * 3);
    }
}//end wheel


// Convert separate R,G,B into packed 32-bit RGB color.
// Packed format is always RGB, regardless of LED strand color order.
uint32_t ColorWheelDisplay::makeColor(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}//end makeColor


