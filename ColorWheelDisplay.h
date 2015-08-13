#ifndef COLORWHEELDISPLAY_HEADERGUARD
#define COLORWHEELDISPLAY_HEADERGUARD

#include "LedDisplay.h"

class ColorWheelDisplay
{
public:
    ColorWheelDisplay(LedDisplay& display);
    ~ColorWheelDisplay();
    bool activate();
    bool setWheelPosition(uint8_t color); //Input a value 0 to 255 to get a color value.
    bool blank();
    uint32_t getActiveColor(); //returns the RGB color that the color wheel is displaying.
    
private:
    //private functions
    uint32_t wheel(uint8_t WheelPos);
    uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b);
    
    // Private variables
    LedDisplay& m_display;
    uint32_t    m_drawToken;    
    uint32_t    m_activeColor;
};//end ColorWheelDisplay



#endif //COLORWHEELDISPLAY_HEADERGUARD