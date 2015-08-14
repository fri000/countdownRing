#include "PhotoCountSelectMonitor.h"
/*
static const uint8_t  POLL_DELAY_MS = 33;
static const uint8_t  MAX_PHOTO_COUNT = 16;
static const uint8_t  MIN_PHOTO_COUNT = 1;
static const uint16_t AUTO_HIDE_DELAY_MS = 3000;
static const uint8_t  READING_THRESHOLD = 2; //in ADC units
static const int16_t  INVALID_ANALOG_VALUE = -1;
*/

// #define POLL_DELAY_MS 33
#define POLL_DELAY_MS 20
#define MAX_PHOTO_COUNT 8
#define MIN_PHOTO_COUNT 1
#define AUTO_HIDE_DELAY_MS 3000
// #define READING_THRESHOLD 2
#define READING_THRESHOLD 10 
#define INVALID_ANALOG_VALUE -1

// Public Functions
// ============================================================================

PhotoCountSelectMonitor::PhotoCountSelectMonitor(TaskScheduler& scheduler, LedDisplay& display, uint8_t monitorPin, ColorSelectMonitor& colorSelector):
    m_scheduler(scheduler),
    m_currentScheduledTask(NULL),
    m_barDisplay(display),
    m_pinToMonitor(monitorPin),
    m_previousReading(INVALID_ANALOG_VALUE),
    m_timeIdleMs(0),
    m_showingBarDisplay(false),
    m_selectedPhotoCount(1), //1 second as the default until we are able to take our first analog reading
    m_colorSelector(colorSelector)
{    
    //start enabled by default.
    enable(); 
}//end constructor


PhotoCountSelectMonitor::~PhotoCountSelectMonitor()
{
    if(m_currentScheduledTask)
    {
        m_scheduler.removeScheduledTask(m_currentScheduledTask);
    }
}//end destructor


//Enable (if disabled) monitoring of the analog pin
void PhotoCountSelectMonitor::enable()
{
    if(m_currentScheduledTask == NULL)
    {
        m_currentScheduledTask = m_scheduler.scheduleDelayedTask(checkAnalogPin0, this, POLL_DELAY_MS);
    }
}//end enable


//Disable monitoring of the analog pin.
void PhotoCountSelectMonitor::disable()
{
    if(m_currentScheduledTask)
    {
        m_scheduler.removeScheduledTask(m_currentScheduledTask);
        m_currentScheduledTask = 0;
    }
}//end disable


uint8_t PhotoCountSelectMonitor::getCount()
{
    takeReading(); //update m_selectedPhotoCount
    return m_selectedPhotoCount;
}//end getCount


void PhotoCountSelectMonitor::checkAnalogPin0(void* clientData)
{
    ((PhotoCountSelectMonitor*)clientData)->checkAnalogPin();
}//end checkAnalogPin0




// Private functions
// ============================================================================
void PhotoCountSelectMonitor::checkAnalogPin()
{
    //Take an analog reading on the pin
    m_lowPassFilter.push( analogRead(m_pinToMonitor) );
    int16_t newReading = m_lowPassFilter.currentAverage();
    
    //Did the voltage change passed our threshold?
    if( (newReading > (m_previousReading + READING_THRESHOLD)) ||
        (newReading < (m_previousReading - READING_THRESHOLD))
       )
    {
        //Yes the voltage changed.
        //Draw the bar display to show the user how many photos they have selected.

        //map the current analog value to the number of photos to take.
        //                      value,    fromLow, fromHigh, toLow, toHigh
        m_selectedPhotoCount = map(newReading, 0, 1000, MIN_PHOTO_COUNT, MAX_PHOTO_COUNT);
        //1000 instead of 1023 to make sure the last leds lights up

        // Grab focus so we can light up the leds!
        m_barDisplay.activate();

        m_timeIdleMs = 0;
        m_barDisplay.setColor( m_colorSelector.getColor() );
        m_barDisplay.setLength(m_selectedPhotoCount);
        m_showingBarDisplay = true;
        
        // update the previous reading
        m_previousReading = newReading;
    }
    else
    {
        //No, it did not change.         
        //Do we have the bar display visible right now?
        if(m_showingBarDisplay)
        {
            //How long have we been idle? Longer than our auto-hide timeout?
            m_timeIdleMs += POLL_DELAY_MS;
            if(m_timeIdleMs > AUTO_HIDE_DELAY_MS)
            {
                //Yes, the user has not changed the knob for some time now.
                //Hide our bar display.
                hideDisplay();
            }
        }
    }
        
    //set up a delayed task so that we will check again in a little bit.
    m_currentScheduledTask = m_scheduler.scheduleDelayedTask(checkAnalogPin0, this, POLL_DELAY_MS);
    
}//end checkAnalogPin


//Read the analog pin and update m_selectedPhotoCount 
void PhotoCountSelectMonitor::takeReading()
{
    m_lowPassFilter.push( analogRead(m_pinToMonitor) );
    int16_t newReading = m_lowPassFilter.currentAverage();
    m_selectedPhotoCount = map(newReading, 0, 1023, MIN_PHOTO_COUNT, MAX_PHOTO_COUNT);
}//end takeReading


void PhotoCountSelectMonitor::hideDisplay()
{
    //Turn off all the leds
    m_barDisplay.setLength(0);
    m_showingBarDisplay = false;
}//end hideDisplay

