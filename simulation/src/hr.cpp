#include <numeric>
#include <complex>
#include <algorithm>
#include <functional>


#include "hr.h"
#include "typevector.h"
#include "utilities/fft.h"
#include "utilities/tools.h"






/*Constructor

@param
@return

*/
HR::HR(int age, int duration_h, double hr, double hr_std, std::string bpms) : rr_series(NULL), time_occurences(NULL), worsen(false), AggravModel(NULL), sampfreqrr(2.), 
hrmean(hr), hrstd(hr_std), lfhfratio(0.5), duration_h(duration_h), sampled_rr_series(NULL), temp_rr_series(NULL), mode(bpms)
{
    //Get data from database based on age (rr mean, lhlf ratio, etc ...)
    

    AdjustExtremaParam();
    CalcTimeScales();

    

    //Set the sampling frequency => initializaztion
    //sampfreqrr = 2.;



}



void HR::setInternalSampleFreq(double sfint) {

    sampfreqrr = sfint;

}



vec1D HR::GetTimeSeries() {

    if (duration_h > 24) {

        int daily_cycle = int(duration_h/24);

        N_beats = giveBeatsNumber(24);

        for (int i = 0; i < daily_cycle; ++i) {
            temp_rr_series = vec1D(N_beats * sampfreqrr, 0.);
            GenerateTimeSeries();
            rr_series.insert(rr_series.end(), temp_rr_series.begin(), temp_rr_series.end());
            temp_rr_series.clear();
        }
    }

    int remainder = duration_h % 24;


    //In any case
    N_beats = giveBeatsNumber((int)(remainder));
    temp_rr_series = vec1D(N_beats * sampfreqrr, 0.);
    GenerateTimeSeries();
    rr_series.insert(rr_series.end(), temp_rr_series.begin(), temp_rr_series.end());



    //Resampling
    getTimeOccurences();
    vec1D sampled_rr_series = sample_to_even(time_occurences, rr_series);
    sampled_rr_series.resize(duration_h*3600); //Get only what's needed*/

    if (mode == "BPM") {
    
        std::transform(sampled_rr_series.begin(), sampled_rr_series.end(), sampled_rr_series.begin(), [](double x) {return (60. / x); });

    
    }

    if (worsen == true) {
    
        vec1D worsening_values = AggravModel->generateModel();
        vec1D::iterator it_w = worsening_values.begin();

        for (double i = 0; i < duration_h * 3600; ++i) {

            if (i >= (double)aggravation_start_h * 3600 && it_w != worsening_values.end()) {
                sampled_rr_series[i] += *it_w;
                ++it_w;
            }

        }
    }


    return sampled_rr_series;
}










vec1D HR::GetRawTimeSeries() {

    rr_series = vec1D(N_beats * sampfreqrr, 0.);

    GenerateTimeSeries();

    return rr_series;

}






int HR::giveBeatsNumber(int duration_h) { //due to fft

    int duration_sec = duration_h * 3600;

    int beats_number = 2;

    while (duration_sec >= beats_number) {

        beats_number *= 2;

    }

    return beats_number*sampfreqrr;


}

void HR::aggrave(double hr_max, int start_h, int total_duration_h, int transition_period_h, int recovery_period_h) {


    worsen = true;
    aggravation_start_h = start_h;
    double k_heigth = hr_max - hrmean;

    AggravModel = new AggravatorModel(k_heigth, total_duration_h, transition_period_h, recovery_period_h, false);


}



vec1D HR::sample_to_even(vec1D tp, vec1D yp) {

    int last = floor(tp.back()); //last timestamp
    vecInt time_interval = arange(0, last, 1); //Generate even timestamps
    vec1D even_sample(last, 0.);//vector containing the interpolated values

    int time_to_interpolate = 1;

    for (int i = 1; i < yp.size(); ++i) {

        if (tp[i] >= time_to_interpolate) { //sometimes may break due to the first beat taking more than one second => to fix

            double slope = (yp[i] - yp[i - 1]) / (tp[i] - tp[i - 1]);
            even_sample[time_to_interpolate - 1] = slope * (time_to_interpolate - tp[i - 1]) + yp[i - 1];

            time_to_interpolate++;

        }

    }

    return even_sample;

}


//Get the time of occurences for each points generated in the time series taking into acount the internal sample frequency
void HR::getTimeOccurences() {

    time_occurences.clear();

    time_occurences.push_back(0.);
    int sz = rr_series.size();


    for (int i = 0; i < sz; ++i) {

        time_occurences.push_back((time_occurences[i] + rr_series[i]) / sampfreqrr);

    }

    time_occurences.erase(time_occurences.begin());
}













void HR::AdjustExtremaParam() {
    //adjust extrema parameters for mean heart rate
    double hrfact = sqrt(hrmean / 60.);
    double hrfact2 = sqrt(hrfact);
    vec1D hr_arr{ hrfact2, hrfact, 1., hrfact, hrfact2 };
    std::transform(bi.begin(), bi.end(), bi.begin(), [hrfact](double b) {return b * hrfact; });
    for (int i = 0; i < ti.size(); ++i) { ti[i] = (ti[i] * PI / 180) * hr_arr[i]; }
};


void HR::CalcTimeScales() {
    //calculate time scales for rrand total output
    sampfreqrr = 1.;
    trr = 1. / sampfreqrr;
    Nrr = pow(2, (std::ceil(std::log2(N_beats * rrmean / trr))));
    rrmean = 60 / hrmean;
    rrstd = 60 * hrstd / (hrmean * hrmean);

}



vec1D HR::ReplicateSpectralProperties() {


    double w1 = 2. * PI * flo;
    double w2 = 2. * PI * fhi;
    double c1 = 2. * PI * flostd;
    double c2 = 2. * PI * fhistd;
    double sig1 = lfhfratio;
    double sig2 = 1;
    double df = sampfreqrr / N_beats; //sfrr influence here

    vec1D w_temp = arange(0., N_beats); //maybe make the sfrr/N calculation as well ?
    vec1D Hw(N_beats, 0.);

    for (int i = 0; i < N_beats; ++i) {
        double dw1 = (w_temp[i] * 2. * PI * df) - w1;  //angular freq diff 
        double dw2 = (w_temp[i] * 2. * PI * df) - w2;

        //Sum of two gaussian functions

        Hw[i] = (sig1 * exp(-0.5 * pow((dw1 / c1), 2)) / sqrt(2. * PI * pow(c1, 2))) + (sig2 * exp(-0.5 * pow((dw2 / c2), 2)) / sqrt(2. * PI * pow(c2, 2)));
    }

    vec1D Hw0 = slicing(Hw, 0, (N_beats / 2) - 1);
    //vec1D Hw02 = slicing(Hw, 0, (N_beats / 2) - 1);
    vec1D Hw02((N_beats / 2));
    std::reverse_copy(Hw0.begin(), Hw0.end(), Hw02.begin());
    Hw0.insert(Hw0.end(), Hw02.begin(), Hw02.end()); //concatenating

    vec1D Sw(Hw0.size(), 0.);
    float sfrr = sampfreqrr;
    std::transform(Hw0.begin(), Hw0.end(), Sw.begin(), [sfrr](double x) {return (sfrr / 2.) * sqrt(x); });

    return Sw;

}


void HR::GenerateTimeSeries() {

    /*A time seriesT(t) with power spectrumS(f) is generated by taking the inverse Fourier transform of a sequence of complex numbers with amplitudes √S(f)
    and phases which are randomly distributed between 0 and 2π */

    vec1D Sw = ReplicateSpectralProperties();

    vec1D ph0 = random_nb_array(0.0, 1.0, (N_beats / 2) - 1);
    std::transform(ph0.begin(), ph0.end(), ph0.begin(), [](double rd) {return 2 * PI * rd; });


    /*ph = [0; ph0; 0; -flipud(ph0)]*/

    vec1D ph = { 0. };
    ph.insert(ph.end(), ph0.begin(), ph0.end());
    ph.push_back(0.);
    std::reverse(ph0.begin(), ph0.end());
    std::transform(ph0.begin(), ph0.end(), ph0.begin(), std::negate<double>());
    ph.insert(ph.end(), ph0.begin(), ph0.end());


    //associate modules Sw to phases ph to form complex numbers
    std::vector<std::complex<double>> SwC(Sw.size(), 0.);
    std::transform(Sw.begin(), Sw.end(), SwC.begin(), [Sw, ph](double sw) {
        auto tt = std::find(Sw.begin(), Sw.end(), sw);
        return std::polar(sw, ph[tt - Sw.begin()]);
        });


    fft::ifft(SwC); //Compute inverse Fast Fourier Transform
    vec1D x(SwC.size(), 0.);
    std::transform(SwC.begin(), SwC.end(), x.begin(), [this](std::complex<double> y) {return (1 / N_beats) * y.real(); });


    double xstd = standard_deviation(x);
    double ratio = rrstd / xstd;

    std::transform(x.begin(), x.end(), temp_rr_series.begin(), [this, ratio](double y) {return (rrmean + y * ratio); });

}