#include "StartButtonMonitor.h"

#define POLL_DELAY_MS 33

#include "AttinySoftwareSerial.h" //for debug, hard coded to write serial to PB1 @ 9600 baud
// extern AttinySoftwareSerial mySerial;//debug!

// Public Functions
// ============================================================================

StartButtonMonitor::StartButtonMonitor(TaskScheduler& scheduler, LedFuse& fuse, PhotoCountSelectMonitor& photoCountMonitor, ColorSelectMonitor& colorMonitor,uint8_t monitorPin, uint8_t triggerPin):
    m_scheduler(scheduler),
    m_currentScheduledTask(NULL),
    m_ledFuse(fuse),
    m_photoCountSelector(photoCountMonitor),
    m_colorSelector(colorMonitor),
    m_burnTimeMs(5000), //default the burn time to 5 seconds
    m_ledColor(0xFF), //default to blue
    m_photosToTake(1), //take 1 photo by default
    m_photosTaken(0),
    m_pinToMonitor(monitorPin),
    m_triggerPin(triggerPin)
{
    //start enabled by default.
    enable(); 
}//end constructor


StartButtonMonitor::~StartButtonMonitor()
{
    if(m_currentScheduledTask)
    {
        m_scheduler.removeScheduledTask(m_currentScheduledTask);
    }
}//end destructor


//Enable (if disabled) monitoring of the digital pin
void StartButtonMonitor::enable()
{
    if(m_currentScheduledTask == 0)
    {
        m_currentScheduledTask = m_scheduler.scheduleDelayedTask(checkDigitalPin0, this, POLL_DELAY_MS);
    }
}//end enable


void StartButtonMonitor::setCountDownTime(uint16_t newBurnTimeMs)
{
    m_burnTimeMs = newBurnTimeMs;
}//end setCountDownTime


//Disable monitoring of the digital pin.
void StartButtonMonitor::disable()
{
    if(m_currentScheduledTask)
    {
        m_scheduler.removeScheduledTask(m_currentScheduledTask);
        m_currentScheduledTask = 0;
    }
}//end disable



void StartButtonMonitor::checkDigitalPin0(void* clientData)
{
    ((StartButtonMonitor*)clientData)->checkDigitalPin();
}//end checkDigitalPin0


void StartButtonMonitor::triggerCamera0(void* clientData)
{
    ((StartButtonMonitor*)clientData)->triggerCamera();
}//end triggerCamera0


void StartButtonMonitor::reactivate0(void* clientData)
{
    ((StartButtonMonitor*)clientData)->reactivate();
}//end reactivate0




// Private functions
// ============================================================================
void StartButtonMonitor::checkDigitalPin()
{

    //Check if the button is being pressed
    if(digitalRead(m_pinToMonitor) == LOW)
    {

        //The button is pressed!
        //Tell the photoCountSelector and the colorSelectMonitor to stop monitoring their pins
        m_photoCountSelector.disable();
        m_colorSelector.disable();
        
        m_ledColor     = m_colorSelector.getColor();
        m_photosToTake = m_photoCountSelector.getCount();
        m_photosTaken  = 0; //reset how many shots we've taken
        
        // mySerial.print("m_photosToTake = ");
        // mySerial.print(m_photosToTake);
        // mySerial.print(", m_ledColor = ");
        // mySerial.print(m_ledColor);
        
        m_ledFuse.ignite(m_burnTimeMs, m_ledColor);
        
        //Schedule the camera to be triggered a little bit after the countdown animation ends.
        m_currentScheduledTask = m_scheduler.scheduleDelayedTask(triggerCamera0, this, m_burnTimeMs + 10);
    }
    else
    {
        //The button is not being pressed
        //Set up a delayed task so that we will check again in a little bit.
        m_currentScheduledTask = m_scheduler.scheduleDelayedTask(checkDigitalPin0, this, POLL_DELAY_MS);
    }
}//end checkDigitalPin


void StartButtonMonitor::triggerCamera()
{
    digitalWrite(m_triggerPin, HIGH); 
    const uint16_t TRIGGER_HIGH_TIME_MS = 2000; //Length of trigger pulse
    // We assume 2 seconds is long enough for all cameras to auto-focus and take the picture.
    // If the pin goes low before auto-focus is finished, some cameras will abort taking the picture.
    m_currentScheduledTask = m_scheduler.scheduleDelayedTask(reactivate0, this, TRIGGER_HIGH_TIME_MS);
}//end triggerCamera


void StartButtonMonitor::reactivate()
{
    digitalWrite(m_triggerPin, LOW); 
    m_photosTaken++;
    
    // Do we have more photos to take?
    if(m_photosTaken < m_photosToTake)
    {
        //Take one more photo.
        //Start the led count down again.
        m_ledFuse.ignite(m_burnTimeMs, m_ledColor);
        //schedule ourselves to be reactivated after the burnTime + coolDownDelay
        m_currentScheduledTask = m_scheduler.scheduleDelayedTask(triggerCamera0, this, m_burnTimeMs + 10);
    }
    else
    {
        //We are done taking all the pictures, reactive monitoring of the start button
        m_currentScheduledTask = 0; //Needed for our enable function to work.
        enable(); 
        m_photoCountSelector.enable();
        m_colorSelector.enable();
    }
}//end reactivate


