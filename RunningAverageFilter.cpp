#include "RunningAverageFilter.h"
#include <string.h> //for memeset

RunningAverageFilter::RunningAverageFilter()
{
    m_writeIndex = 0;
    m_total = 0;
    memset(m_pBuffer, 0, sizeof(uint16_t) * BUFFER_LENGTH);
}//end constructor



void RunningAverageFilter::push(uint16_t newValue)
{
    //Subtract the old value from the total.
    m_total -= m_pBuffer[m_writeIndex];
    
    //Save the new value to our buffer and add it to the total.
    m_pBuffer[m_writeIndex] = newValue;
    m_total += newValue;
    
    //Advance the writeIndex. Wrap around if needed.
    m_writeIndex++;
    if(m_writeIndex == BUFFER_LENGTH)
    {
        m_writeIndex = 0;
    }
}//end push


uint16_t RunningAverageFilter::currentAverage()
{
    return (uint16_t)(m_total / BUFFER_LENGTH);
}//end currentAverage

