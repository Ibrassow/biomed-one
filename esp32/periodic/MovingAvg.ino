#include "MovingAvg.h"
 
// initialize - allocate the interval array
void MovingAvg::begin()
{
    m_readings = new double[m_interval];
}
 
// add a new reading and return the new moving average
double MovingAvg::reading(const double new_reading)
{
    // add each new data point to the sum until the m_readings array is filled
    if (m_nbrReadings < m_interval)
    {
        ++m_nbrReadings;
        m_sum = m_sum + new_reading;
    }
    // once the array is filled, subtract the oldest data point and add the new one
    else
    {
        m_sum = m_sum - m_readings[m_next] + new_reading;
    }
 
    m_readings[m_next] = new_reading;
    if (++m_next >= m_interval) m_next = 0;
    return (m_sum + static_cast<double>(m_nbrReadings) / 2.0) / static_cast<double>(m_nbrReadings);
}
 
// just return the current moving average
double MovingAvg::getAvg()
{
    return (m_sum + static_cast<double>(m_nbrReadings) / 2.0) / static_cast<double>(m_nbrReadings);
}
 

void MovingAvg::reset()
{
    m_nbrReadings = 0;
    m_sum = 0;
    m_next = 0;
 
    delete m_readings;
}
