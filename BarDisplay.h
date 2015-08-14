#ifndef BARDISPLAY_HEADERGUARD
#define BARDISPLAY_HEADERGUARD

#include "LedDisplay.h"

// Draws a bar graph on the LED display
class BarDisplay
{
public:
    BarDisplay(LedDisplay& display);
    ~BarDisplay();
    bool activate(); //Grab the focus of the led display
    bool setLength(uint16_t newLength); //Draw a new bar graph of the specified length
    bool setColor(uint32_t newColor); // Sets the color, takes effect on next call to setLength()
    uint16_t maxLength(); 
    
private:
    // Private variables
    LedDisplay&     m_display;
    uint32_t        m_drawToken;    
    uint32_t        m_color;
};//end BarDisplay



#endif //BARDISPLAY_HEADERGUARD