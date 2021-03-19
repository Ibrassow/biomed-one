#include "spo2.h"
#include "typevector.h"
#include "utilities/tools.h"

#include <chrono>
#include <random>



unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
static std::default_random_engine gen(seed);




SPO2::SPO2(int age_, double time_hours, double mean) : worsen(false), AggravModel(NULL)

{ //Constructor
    spo2_mean = mean;
    spo2_sd = GetHealthySD(mean);
    age = age_;
    N_points = time_hours * 3600.;

    spo2_series = vec1D(N_points, 0.);

};




vec1D SPO2::GetTimeSeries() {
    GenerateTimeSeries();
    return spo2_series;
}








/*
* Generate the entire time series given the time window
@param
@return
*/
void SPO2::GenerateTimeSeries()
{
    spo2_series[0] = spo2_mean;

    if (worsen == true) {

        vec1D worsening_values = AggravModel->generateModel();
        //int time_w = worsening_values.size();
        vec1D::iterator it_w = worsening_values.begin();
    
        for (int i = 1; i < N_points; ++i) {

            if (i >= (double)aggravation_start_h * 3600 && it_w != worsening_values.end()) {
                
                if (randomnizer())
                {
                    spo2_series[i] = spo2_series[i - 1] + *it_w;
                }
                else
                {
                    spo2_series[i] = GetSafeValue() + *it_w;; //model_interval
                }
                //recalculates the mean after 1 hour
                if (i % 3600 == 0) {
                    CalculateNewMean(i);
                }

                ++it_w;
            }
            else {

                if (randomnizer())
                {
                    spo2_series[i] = spo2_series[i - 1];
                }
                else
                {
                    spo2_series[i] = GetSafeValue(); //model_interval
                }

                //recalculates the mean after 1 hour
                if (i % 3600 == 0) {
                    CalculateNewMean(i);
                }
            }
        }
    }


    else {    
        /*//vec1D model_interval = CreateInterval();
        spo2_series[0] = spo2_mean;*/

        for (int i = 1; i < N_points; ++i) {

            if (randomnizer())
            {
                spo2_series[i] = spo2_series[i - 1];
            }
            else
            {
                spo2_series[i] = GetSafeValue(); //model_interval
            }

            //recalculates the mean after 1 hour
            if (i % 3600 == 0) {

                CalculateNewMean(i);

            }
        }
    }
    
    
    
    
    /*//vec1D model_interval = CreateInterval();

    spo2_series[0] = spo2_mean;

    for (int i = 1; i < N_points; ++i) {

        if (randomnizer())
        {
            spo2_series[i] = spo2_series[i-1];
        }
        else
        {
            spo2_series[i] = GetSafeValue(); //model_interval
        }

        //recalculates the mean after 1 hour
        if (i % 3600 == 0) {

            CalculateNewMean(i);

        }
    }*/


}







double SPO2::GetHealthySD(double mean)
{
    return ((round((-0.146 * mean + 14.998) * 10.0)) / 10.0);
}






vec1D SPO2::CreateInterval() {
    vec1D interv = arange(spo2_mean - spo2_sd, spo2_mean + spo2_sd, 0.1);
    return interv;
}




bool SPO2::randomnizer()
{
    std::uniform_real_distribution<> dis(0, 100);
    int result=round(dis(gen));
    if (result<87){return true;}
    else{return false;}
}




double SPO2::GetSafeValue() //vec1D& elem
{
    std::normal_distribution<double> dis(spo2_mean, spo2_sd);
    return dis(gen);
}




void SPO2::CalculateNewMean(int i)
{//retourne la moyenne de la période suivante

    std::vector<double> last_hour_data(spo2_series.begin() + i - 3600, spo2_series.begin() + i);
    double sum = std::accumulate(last_hour_data.begin(), last_hour_data.end(), 0.);
    spo2_mean = round((sum/3600)*10.)/10.;

}



///Need a fonction to determine the mean


void SPO2::aggrave(double spo2_max, int start_h, int total_duration_h, int transition_period_h, int recovery_period_h) {

    worsen = true;
    aggravation_start_h = start_h;
    double k_heigth = spo2_max - spo2_mean;

    AggravModel = new AggravatorModel(k_heigth, total_duration_h, transition_period_h, recovery_period_h, false);


}