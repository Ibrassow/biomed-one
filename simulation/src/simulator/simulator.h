#pragma once

#include "hr.h"
#include "spo2.h"
#include "temperature.h"
#include <fstream>




class Simulator {


public:


	Simulator(int age, int entire_duration, std::string filename);



	/*
		Start the entire simulator
		@param XXXX
	*/
	void start();


	void pause();


	void stop();


	void activateAggravator(bool fever, bool resp_trbl, int severity, int start_h, int total_duration_h, int transition_period_h, int recovory_period_h);

	vec1D giveTempSeries();
	vec1D giveHRSeries();
	vec1D giveSpO2Series();


private:

	//std::string gender;

	int simulation_duration_h;


	HR HRGen;
	Temperature TemperatureGen;
	SPO2 SpO2Gen;

	std::fstream resultsFileHandler;



};





