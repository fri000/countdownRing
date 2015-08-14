#ifndef PHOTOCOUNTSELECTMONITOR_HEADERGUARD
#define PHOTOCOUNTSELECTMONITOR_HEADERGUARD

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include <pins_arduino.h>
#endif
#include "TaskScheduler.h"
#include "BarDisplay.h"
#include "ColorSelectMonitor.h"
#include "RunningAverageFilter.h"

// Constantly monitors an analog pin and interprets its value as the numbers of photos to take.
// Displays the current selected count when the analog pin changes to a new value.
class PhotoCountSelectMonitor
{
public:
    PhotoCountSelectMonitor(TaskScheduler& scheduler, LedDisplay& display, uint8_t monitorPin, ColorSelectMonitor& colorSelector);
    ~PhotoCountSelectMonitor();
    void enable();
    void disable();
    uint8_t getCount();
    
    static void checkAnalogPin0(void* clientData); //should only be called by the taskScheduler.
     
private:
    // Private functions
    void checkAnalogPin();
    void takeReading();
    void hideDisplay();

    // Private variables
    TaskScheduler&  m_scheduler; 
    taskToken_t     m_currentScheduledTask;
    BarDisplay      m_barDisplay;
    uint8_t         m_pinToMonitor;
    int16_t         m_previousReading;
    uint32_t        m_timeIdleMs;
    bool            m_showingBarDisplay;
    uint8_t         m_selectedPhotoCount;
    ColorSelectMonitor& m_colorSelector; //We ask it what is the current selected color
    RunningAverageFilter m_lowPassFilter; //Filter to smooth out analog readings
};//end PhotoCountSelectMonitor



#endif //PHOTOCOUNTSELECTMONITOR_HEADERGUARD