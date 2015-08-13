#ifndef BARDISPLAY_HEADERGUARD
#define BARDISPLAY_HEADERGUARD

#include "LedDisplay.h"

class BarDisplay
{
public:
    BarDisplay(LedDisplay& display);
    ~BarDisplay();
    bool activate();
    bool setLength(uint16_t newLength);
    bool setColor(uint32_t newColor);
    uint16_t maxLength();
    
private:
    // Private variables
    LedDisplay&     m_display;
    uint32_t        m_drawToken;    
    uint32_t        m_color;
};//end BarDisplay



#endif //BARDISPLAY_HEADERGUARD