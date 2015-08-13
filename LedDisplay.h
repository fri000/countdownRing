#ifndef LEDDISPLAY_HEADERGUARD
#define LEDDISPLAY_HEADERGUARD

#include <inttypes.h>
#include "Adafruit_NeoPixel.h"


class LedDisplay
{
public:

    LedDisplay(Adafruit_NeoPixel* strip);
    ~LedDisplay();
    bool setOffset(uint8_t newOffset);
    uint32_t grabFocus(); //hands out a token that is used to draw on the display. Token expires when this function is called to generate a new one.
    void refresh(uint32_t drawToken);  //write out the buffer to the physical leds
    void setPixel(uint32_t drawToken, int16_t whichPixel, uint32_t color); // set a pixel in the buffer
    void clearDisplay(uint32_t drawToken); //reset the display buffer to all zeros. Call refresh() afterwards to update physical leds.
    uint8_t numOfPixels();

private:

	uint8_t m_offset;
    uint32_t m_validToken;  //The current valid draw token
    Adafruit_NeoPixel* m_pStrip;
};






#endif //LEDDISPLAY_HEADERGUARD