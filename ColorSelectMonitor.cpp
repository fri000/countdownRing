#include "ColorSelectMonitor.h"

// #define POLL_DELAY_MS 33
#define POLL_DELAY_MS 20
#define MAX_WHEEL_POS 255
#define MIN_WHEEL_POS 0
#define AUTO_HIDE_DELAY_MS 3000
// #define READING_THRESHOLD 2
#define READING_THRESHOLD 4 //this was a test
#define INVALID_ANALOG_VALUE -1

// Public Functions
// ============================================================================

ColorSelectMonitor::ColorSelectMonitor(TaskScheduler& scheduler, LedDisplay& display, uint8_t monitorPin):
    m_scheduler(scheduler),
    m_currentScheduledTask(NULL),
    m_colorWheelDisplay(display),
    m_pinToMonitor(monitorPin),
    m_previousReading(INVALID_ANALOG_VALUE),
    m_timeIdleMs(0),
    m_showingColorWheel(false)
{
    //start enabled by default.
    enable(); 
}//end constructor


ColorSelectMonitor::~ColorSelectMonitor()
{
    if(m_currentScheduledTask)
    {
        m_scheduler.removeScheduledTask(m_currentScheduledTask);
    }
}//end destructor


//Enable (if disabled) monitoring of the analog pin
void ColorSelectMonitor::enable()
{
    if(m_currentScheduledTask == NULL)
    {
        m_currentScheduledTask = m_scheduler.scheduleDelayedTask(checkAnalogPin0, this, POLL_DELAY_MS);
    }
}//end enable


//Disable monitoring of the analog pin.
void ColorSelectMonitor::disable()
{
    if(m_currentScheduledTask)
    {
        m_scheduler.removeScheduledTask(m_currentScheduledTask);
        m_currentScheduledTask = 0;
    }
}//end disable


uint32_t ColorSelectMonitor::getColor()
{
    //todo: get the value from our lowpass filter instead of reading the pin directly.
    //read the analog pin here to get the most up to date value
    int16_t newReading = analogRead(m_pinToMonitor);
    uint8_t colorWheelPosition = 0;
    colorWheelPosition = map(newReading, 0, 1023, MIN_WHEEL_POS, MAX_WHEEL_POS);
    m_colorWheelDisplay.setWheelPosition(colorWheelPosition);
    return m_colorWheelDisplay.getActiveColor();
}//end getDelayMs


void ColorSelectMonitor::checkAnalogPin0(void* clientData)
{
    ((ColorSelectMonitor*)clientData)->checkAnalogPin();
}//end checkAnalogPin0




// Private functions
// ============================================================================
void ColorSelectMonitor::checkAnalogPin()
{
    //Take an analog reading on the pin
    m_lowPassFilter.push( analogRead(m_pinToMonitor) );
    int16_t newReading = m_lowPassFilter.currentAverage();
    uint8_t colorWheelPosition = 0;
    
    //Did the voltage change passed our threshold?
    if( (newReading > (m_previousReading + READING_THRESHOLD)) ||
        (newReading < (m_previousReading - READING_THRESHOLD))
       )
    {
        //Yes the voltage changed.
        //map the current analog value to the number of seconds of delay.
        //                      value,    fromLow, fromHigh, toLow, toHigh
        colorWheelPosition = map(newReading, 0, 1023, MIN_WHEEL_POS, MAX_WHEEL_POS);
        
        //Draw the bar display to show the user how many seconds they have selected.
        if(m_previousReading == INVALID_ANALOG_VALUE)
        {
            //This is our very first reading since we powered up.
            //Don't active the display for this reading,
            //Just prime our internal variables with an initial reading.
        }
        else
        {
            //Light up the leds!
            m_colorWheelDisplay.activate();
        }
        
        m_timeIdleMs = 0;
        m_colorWheelDisplay.setWheelPosition(colorWheelPosition);
        m_showingColorWheel = true;
        
        //update the previous reading
        m_previousReading = newReading;
    }
    else
    {
        //No, the voltage on the analog pin did not change.         
        //Do we have the color wheel display visible right now?
        if(m_showingColorWheel)
        {
            //How long have we been idle? Longer than our auto-hide timeout?
            m_timeIdleMs += POLL_DELAY_MS;
            if(m_timeIdleMs > AUTO_HIDE_DELAY_MS)
            {
                //Yes, the user has not changed the knob for some time now.
                //Hide our color wheel display.
                m_colorWheelDisplay.blank();
                m_showingColorWheel = false;
            }
        }
    }
        
    //set up a delayed task so that we will check again in a little bit.
    m_currentScheduledTask = m_scheduler.scheduleDelayedTask(checkAnalogPin0, this, POLL_DELAY_MS);
    
}//end checkAnalogPin




