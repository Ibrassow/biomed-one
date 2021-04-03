#include "typevector.h"
#include "temperature.h"
#include <numeric>
#include <random>
#include "utilities/tools.h"


Temperature::Temperature(int age_, double time_hours, double mean) : worsen(false), AggravModel(NULL)
{

	temp_mean = mean;
	//temp_sd = GetHealthySD(mean);
	age = age_;
	N_points = time_hours * 3600.;

	temperature_series = vec1D(N_points, 0.);



}




vec1D Temperature::GetTimeSeries() {

	GenerateTimeSeries();

	return temperature_series;


}




void Temperature::GenerateTimeSeries() {

	if (worsen == true) {

		vec1D worsening_values = AggravModel->generateModel();
		//int time_w = worsening_values.size();

		vec1D::iterator it_w = worsening_values.begin();

		for (double i = 0; i < N_points; ++i) {

			
			if (i >= (double)aggravation_start_h*3600 && it_w != worsening_values.end()) {
				temperature_series[i] = CalculateModelTemp(i) + AddVariation(temperature_series[i]) + *it_w;
				++it_w;
			}
			else {

				temperature_series[i] = CalculateModelTemp(i) + AddVariation(temperature_series[i]);
			}
			
			

		}
	}


	else {

		for (double i = 0; i < N_points; ++i) {

			temperature_series[i] = CalculateModelTemp(i) + AddVariation(temperature_series[i]);

		}


	}


}


double Temperature::CalculateModelTemp(double t) {

	t = t / 3600;
	return 0.6122 * sin(0.2303 * t - 2.302) + 36.984; //the last term is the mean 

}

std::random_device rd;
std::mt19937 e2(rd());


double Temperature::AddVariation(double prev_temp) {

	std::uniform_real_distribution<> dist(-max_deviation, max_deviation);

	double variation = dist(e2);

	return variation;

}


void Temperature::aggrave(double temp_max, int start_h, int total_duration_h, int transition_period_h, int recovery_period_h) {


	worsen = true;
	aggravation_start_h = start_h;
	double k_heigth = temp_max - temp_mean;




	AggravModel = new AggravatorModel(k_heigth, total_duration_h, transition_period_h, recovery_period_h, false);


}

