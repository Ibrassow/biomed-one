

#include <iostream>
#include <algorithm>
#include <numeric>
#include "simulation.h"


#include "typevector.h"



class SPO2 : public ISimulation {

public:


	SPO2(int age, double time_hours, double mean);

	vec1D GetTimeSeries();
	void aggrave(double spo2_max, int start_h, int total_duration_h, int transition_period_h, int recovory_period_h);

private:

	int age;
	int N_points; //nb of measures per second
	double spo2_mean;
	double spo2_sd;
	vec1D spo2_series;

	bool worsen;
	int aggravation_start_h;
	AggravatorModel* AggravModel;


	void GenerateTimeSeries();


	vec1D CreateInterval();



	/*
	@param /
	@return True when SpO2(n+1)=SpO2(n) for 87 percent of the time
	*/
	bool randomnizer();

	/*
	@param the safe interval
	@return A randomly chosen value from that interval
	*/
	double GetSafeValue(); //vec1D& elem


	/*
	@param
	@return
	*/
	void CalculateNewMean(int i);


	/*
	Consequence of Tchebytchev inequality : triple the interval to obtain 90% of safe values (according to the regression line obtained via Python)
	@param the acurrent spo2
	@return the new standard deviation
	*/
	double GetHealthySD(double spo2_value);

};

