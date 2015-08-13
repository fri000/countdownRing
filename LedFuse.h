#ifndef LEDFUSE_HEADERGUARD
#define LEDFUSE_HEADERGUARD

#include "TaskScheduler.h"
#include "LedDisplay.h"

class LedFuse
{
public:
    LedFuse(TaskScheduler& scheduler, LedDisplay& display);
    ~LedFuse();
    bool ignite(uint16_t burnTime, uint32_t color);
    
    static void decrementFuse0(void* clientData); //should only be called by the taskScheduler.

private:
    // Private functions
    void decrementFuse();

    // Private variables
    TaskScheduler&  m_scheduler;
    LedDisplay&     m_display;
    uint32_t        m_drawToken;    
    uint8_t         m_ledIndex; //index of the next led to turn off
    uint8_t         m_numLedsOff; //The number of leds we have turned off so far
    uint16_t        m_delayBetweenTicks;
    taskToken_t     m_currentScheduledTask;

};//end LedFuse



#endif //LEDFUSE_HEADERGUARD