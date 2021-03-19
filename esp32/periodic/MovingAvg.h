
#ifndef MOVINGAVG_H_INCLUDED
#define MOVINGAVG_H_INCLUDED

class MovingAvg
{
    public:
        MovingAvg(float interval)
            : m_interval(interval), m_nbrReadings(0), m_sum(0), m_next(0) {}
        void begin();
        double reading(const double newReading);
        double getAvg();
        void reset();

    private:
        int m_interval;     // number of data points for the moving average
        double m_nbrReadings;  // number of readings
        double m_sum;         // sum of the m_readings array
        int m_next;         // index to the next reading
        double *m_readings;// pointer to the dynamically allocated interval array
        //float m_readings;
};
#endif
