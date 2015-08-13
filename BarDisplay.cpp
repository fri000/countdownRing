#include "BarDisplay.h"


// Public Functions
// ============================================================================

BarDisplay::BarDisplay(LedDisplay& display):
    m_display(display),
    m_drawToken(0),
    m_color(0x20) //default to the color blue
{
}//end constructor



BarDisplay::~BarDisplay()
{

}//end destructor


bool BarDisplay::activate()
{
    m_drawToken = m_display.grabFocus();
    return true;
}//end activate


bool BarDisplay::setLength(uint16_t newLength)
{
    if(newLength > m_display.numOfPixels())
    {
        newLength = m_display.numOfPixels();
    }
    
    //Draw the new bar on the display of the specified length.
    for(uint16_t i = 0; i < newLength; i++)
    {
        m_display.setPixel(m_drawToken, i, m_color);
    }
    
    //Turn off the remaining leds.
    for(uint16_t i = newLength; i < m_display.numOfPixels(); i++)
    {
        m_display.setPixel(m_drawToken, i, 0);
    }
    
    //Push the new pixels on to the display.
    m_display.refresh(m_drawToken);
    
    return true;
}//end setLength


bool BarDisplay::setColor(uint32_t newColor)
{
    m_color = newColor;
    return true;
}//end setColor


uint16_t BarDisplay::maxLength()
{
    return m_display.numOfPixels();
}//end setColor


