#pragma once
#include "simulation.h"

class Temperature : public ISimulation {

public:



	Temperature(int age_, double time_hours, double mean);

	vec1D GetTimeSeries();


	void aggrave(double temp_max, int start_h, int total_duration_h, int transition_period_h, int recovory_period_h);


private:



	AggravatorModel* AggravModel;



	void GenerateTimeSeries();

	double CalculateModelTemp(double t);

	double AddVariation(double prev_temp);

	int age;
	double time_window;
	double temp_mean;
	double temp_sd;
	int N_points;
	vec1D temperature_series;

	double max_deviation = 0.1112;


	bool worsen;

	int aggravation_start_h;
	

};