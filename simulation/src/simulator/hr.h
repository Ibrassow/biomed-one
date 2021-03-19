#include "typevector.h"
#include "simulation.h"


class HR : public ISimulation {

public:

    


    HR(int age, int duration_hr, double hr, double hr_std, std::string bpms);

    //Returns Evenly Sampled (interpolated) time series
    vec1D GetTimeSeries();

    vec1D GetRawTimeSeries();


    void aggrave(double hr_max, int start_h, int total_duration_h, int transition_period_h, int recovory_period_h);

    void setInternalSampleFreq(double sfint);



    



private:

    
    
    //Returns rr data points evenly spaced of 1 second
    vec1D sample_to_even(vec1D tp, vec1D yp);

    


    void getTimeOccurences();








    int giveBeatsNumber(int duration_h);



    void AdjustExtremaParam();

    void CalcTimeScales();

    vec1D ReplicateSpectralProperties();

    void GenerateTimeSeries();







    int duration_h;
    vec1D time_occurences;
    vec1D sampled_rr_series;
    vec1D temp_rr_series;


    std::string mode;

    int age;
    double N_beats;
    double hrmean;
    double hrstd;
 

    //TODEFINE HR_data

    double lfhfratio;
    double rrmean;
    double rrstd;

    bool worsen;
    int aggravation_start_h;


    vec1D rr_series;

    AggravatorModel* AggravModel;


    //morphological parameters 
    vec1D ti = { -70, -15, 0, 15, 100 };
    vec1D ai = { 1.2, -5, 30, -7.5, 0.75 };
    vec1D bi = { 0.25, 0.1, 0.1, 0.1, 0.4 };


    //define frequency parameters for rr process, flo and fhi correspond to the Mayer wavesand respiratory rate respectively
    const double flo = 0.1;
    const double fhi = 0.25;
    const double flostd = 0.01;
    const double fhistd = 0.01;


    //calculate time scales for rrand total output
    double sampfreqrr;
    double trr;
    double Nrr;
    double sfint;
};

