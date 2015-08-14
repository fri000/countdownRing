#include "RunningAverageFilter.h"
#include <string.h> //for memeset

RunningAverageFilter::RunningAverageFilter()
{
    flush();
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

    // Keep track of how many valid numbers we have in the buffer
    m_count++;
    if (m_count > BUFFER_LENGTH)
    {
        m_count = BUFFER_LENGTH;
    }
}//end push


uint16_t RunningAverageFilter::currentAverage()
{
    return (uint16_t)(m_total / BUFFER_LENGTH);
}//end currentAverage


bool RunningAverageFilter::primed()
{
    // we are primed if we have a full history.
    if (m_count >= BUFFER_LENGTH)
    {
        return true;
    }
    return false;
}//end primed


void RunningAverageFilter::flush()
{
    m_writeIndex = 0;
    m_total = 0;
    m_count = 0;
    memset(m_pBuffer, 0, sizeof(uint16_t) * BUFFER_LENGTH);
}//end flush