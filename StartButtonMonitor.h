#ifndef STARTBUTTONMONITOR_HEADERGUARD
#define STARTBUTTONMONITOR_HEADERGUARD

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include <pins_arduino.h>
#endif
#include "TaskScheduler.h"
#include "PhotoCountSelectMonitor.h"
#include "ColorSelectMonitor.h"
#include "LedFuse.h"


class StartButtonMonitor
{
public:
    StartButtonMonitor(TaskScheduler& scheduler, LedFuse& fuse, PhotoCountSelectMonitor& photoCountMonitor, ColorSelectMonitor& colorMonitor, uint8_t monitorPin, uint8_t triggerPin);
    ~StartButtonMonitor();
    void enable();
    void setCountDownTime(uint16_t newBurnTimeMs);
    void disable();
    
    static void checkDigitalPin0(void* clientData); //should only be called by the taskScheduler.
    static void triggerCamera0(void* clientData); //should only be called by the taskScheduler.
    static void reactivate0(void* clientData); //should only be called by the taskScheduler.
private:
    // Private functions
    void checkDigitalPin();
    void triggerCamera();
    void reactivate();

    // Private variables
    TaskScheduler&           m_scheduler; 
    taskToken_t              m_currentScheduledTask;
    LedFuse&                 m_ledFuse;
    PhotoCountSelectMonitor& m_photoCountSelector;
    ColorSelectMonitor&      m_colorSelector;
    uint16_t                 m_burnTimeMs;
    uint32_t                 m_ledColor;
    uint8_t                  m_photosToTake;
    uint8_t                  m_photosTaken;
    uint8_t                  m_pinToMonitor;
    uint32_t                 m_timeIdleMs;
    uint8_t                  m_triggerPin;
};//end StartButtonMonitor



#endif //STARTBUTTONMONITOR_HEADERGUARD