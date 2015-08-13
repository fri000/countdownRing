#include "LedFuse.h"


// Public Functions
// ============================================================================

LedFuse::LedFuse(TaskScheduler& scheduler, LedDisplay& display):
    m_scheduler(scheduler),
    m_display(display),
    m_drawToken(0),
    m_ledIndex(0),
    m_numLedsOff(0),
    m_delayBetweenTicks(0),
    m_currentScheduledTask(0)
{
}//end constructor




LedFuse::~LedFuse()
{
    if(m_currentScheduledTask)
    {
        m_scheduler.removeScheduledTask(m_currentScheduledTask);
    }
}//end destructor


bool LedFuse::ignite( uint16_t burnTime, uint32_t color )
{
    if(m_currentScheduledTask == NULL)
    {
        m_delayBetweenTicks = burnTime / m_display.numOfPixels();

        //Serial.print("LED fuse ignited with m_delayBetweenTicks of ");  //debug
        //Serial.println(m_delayBetweenTicks); delay(20);    //debug

        //Grab focus on the screen so we can draw to it.
        m_drawToken = m_display.grabFocus();
        
        //Turn ON all the leds
        for(int i = 0; i < m_display.numOfPixels(); i++)
        {
            m_display.setPixel(m_drawToken, i, color);
        }
        
        //Push the new pixels on to the display.
        m_display.refresh(m_drawToken);
        
        m_ledIndex = 0;
        m_numLedsOff = 0;
        m_currentScheduledTask = m_scheduler.scheduleDelayedTask(decrementFuse0, this, m_delayBetweenTicks);
        return true;
    }
    else
    {
        //we are already ignited!
        // Need to wait for the led fuse to finish burning first
        //Serial.println("ERROR: trying to ignite a fuse that is already lit");
        return false;
    }
}//end ignite




void LedFuse::decrementFuse0(void* clientData)
{
    ((LedFuse*)clientData)->decrementFuse();
}//end decrementFuse0






// Private functions
// ============================================================================

void LedFuse::decrementFuse()
{
    if(m_numLedsOff < m_display.numOfPixels())
    {
        //turn off the current led
        
        const int LED_OFF_VALUE = 0;
        m_display.setPixel(m_drawToken, m_ledIndex, LED_OFF_VALUE);
        
        //Push the new pixels on to the display.
        m_display.refresh(m_drawToken);
        
        //update the index to the next led down the line.
        m_ledIndex++;
        if(m_ledIndex == m_display.numOfPixels())
        {
            //wrap around
            m_ledIndex = 0;
        }
        m_numLedsOff++;

        //schedule this function to be called again when it's time to decrement the next LED
        m_currentScheduledTask = m_scheduler.scheduleDelayedTask(decrementFuse0, this, m_delayBetweenTicks);
    }
    else
    {
        //all leds are already off, there is nothing to decrement
        m_currentScheduledTask = NULL;
    }
}//end decrementFuse


