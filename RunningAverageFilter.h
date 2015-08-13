#ifndef RUNNINGAVERAGEFILTER_HEADER_GUARD
#define RUNNINGAVERAGEFILTER_HEADER_GUARD

#include <stdint.h>

class RunningAverageFilter
{
public:
    RunningAverageFilter();
    void push(uint16_t newValue);
    uint16_t currentAverage();
    bool primed();
    
private:
    static const uint8_t BUFFER_LENGTH = 10;
    uint16_t m_pBuffer[BUFFER_LENGTH];
    uint16_t m_writeIndex;
    uint32_t m_total; //we could use a uint16_t if we need to free up ram
};

#endif //RUNNINGAVERAGEFILTER_HEADER_GUARD