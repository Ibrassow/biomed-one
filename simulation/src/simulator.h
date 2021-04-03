#pragma once

#include "model/hr.h"
#include "model/spo2.h"
#include "model/temperature.h"
#include "config.h"
#include <fstream>

#include <map>


class Simulator {


public:


	Simulator(int age, int entire_duration, std::string filename);


	void start(); 

	void loadSettings();

	//void pause();


	//void stop();



	void activateAggravator(bool fever, bool resp_trbl, int severity, int start_h, int total_duration_h, int transition_period_h, int recovory_period_h);

	vec1D giveTempSeries();
	vec1D giveHRSeries();
	vec1D giveSpO2Series();


private:

	//std::string gender;

	std::map<std::string, int> settings;

	int age;

	int simulation_duration_h;

	HR HRGen;
	Temperature TemperatureGen;
	SPO2 SpO2Gen;

	std::fstream resultsFileHandler;

	Config config;



};





