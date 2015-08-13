#ifndef COLORSELECTMONITOR_HEADERGUARD
#define COLORSELECTMONITOR_HEADERGUARD

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include <pins_arduino.h>
#endif
#include "TaskScheduler.h"
#include "ColorWheelDisplay.h"
#include "RunningAverageFilter.h"

class ColorSelectMonitor
{
public:
    ColorSelectMonitor(TaskScheduler& scheduler, LedDisplay& display, uint8_t monitorPin);
    ~ColorSelectMonitor();
    void enable();
    void disable();
    uint32_t getColor();
    
    static void checkAnalogPin0(void* clientData); //should only be called by the taskScheduler.
     
private:
    // Private functions
    void checkAnalogPin();

    // Private variables
    TaskScheduler&       m_scheduler; 
    taskToken_t          m_currentScheduledTask;
    ColorWheelDisplay    m_colorWheelDisplay;
    uint8_t              m_pinToMonitor;
    int16_t              m_previousReading;
    uint32_t             m_timeIdleMs;
    bool                 m_showingColorWheel;
    RunningAverageFilter m_lowPassFilter;
};//end ColorSelectMonitor



#endif //COLORSELECTMONITOR_HEADERGUARD