#ifndef RUNNINGAVERAGEFILTER_HEADER_GUARD
#define RUNNINGAVERAGEFILTER_HEADER_GUARD

#include <stdint.h>

class RunningAverageFilter
{
public:
    RunningAverageFilter();
    void push(uint16_t newValue);
    uint16_t currentAverage();
    bool primed(); //Returns true if the currentAverage is based on a full history
    void flush(); //empty the filter
    
private:
    static const uint8_t BUFFER_LENGTH = 10;
    uint16_t m_pBuffer[BUFFER_LENGTH];
    uint8_t m_writeIndex;
    uint8_t m_count; // How many numbers we are averaging
    uint32_t m_total; //we could use a uint16_t if we need to free up ram
};

#endif //RUNNINGAVERAGEFILTER_HEADER_GUARD