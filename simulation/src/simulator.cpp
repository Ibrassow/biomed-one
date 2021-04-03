#include "simulator.h"
#include "model/simulation.h"
#include "model/typevector.h"
#include <string>
#include <random>
#include <chrono>
#include <iterator>

typedef std::string String;




unsigned sd = std::chrono::system_clock::now().time_since_epoch().count();
static std::default_random_engine gene(sd);




Simulator::Simulator(int age, int entire_duration, std::string filename) 
	: age(age), HRGen(age, entire_duration, 70., 5., "BPM"), TemperatureGen(age, entire_duration, 36.9), SpO2Gen(age, entire_duration, 96.)
{
	simulation_duration_h = entire_duration;

}


void Simulator::start() {

	resultsFileHandler = std::fstream("results_simu.txt", std::ios::out);


	vec1D TT = giveTempSeries();
	vec1D HR = giveHRSeries();
	vec1D SpO2 = giveSpO2Series();


	for (unsigned i = 0; i < simulation_duration_h * 3600; ++i) {

		//std::cout << TT[i] << ' ' << HR[i] << ' ' <<  SpO2[i] << std::endl;
		resultsFileHandler << TT[i] << ' ' << HR[i] << ' ' << SpO2[i] << std::endl;


	}


	resultsFileHandler.close();

}


void Simulator::loadSettings() {

	config.loadConfig();


	settings = config.getSettings();
	std::map<std::string, int>::iterator itr;

	for (itr = settings.begin(); itr != settings.end(); ++itr) {


		if (itr->first == "age") {
			age = itr->second;
		}
		else if (itr->first == "duration") {
			simulation_duration_h = itr->second;
		}
		else if ((itr->first == "fever") || (itr->first == "resp_trbl")) {

			if (itr->second == true) {
				activateAggravator(settings["fever"], settings["resp_trb"], settings["severity"], settings["start_h"], settings["total_duration_h"],
					settings["transition_period_h"], settings["recov_p"]);

			}
			break;
		}
	}
};







vec1D Simulator::giveSpO2Series() {

	return SpO2Gen.GetTimeSeries();

}

vec1D Simulator::giveTempSeries() {

	return TemperatureGen.GetTimeSeries();
}


vec1D Simulator::giveHRSeries() {

	return HRGen.GetTimeSeries();

}


void Simulator::activateAggravator(bool fever, bool resp_trbl, int severity, int start_h, int total_duration_h, int transition_period_h, int recovery_period_h) {

	double target_temp = 0;
	double target_spo2 = 0;
	double target_hr = 0;


	//Put a random process here


	switch (severity) {
	case 1:
		{std::uniform_real_distribution<> dis_temp_1(38., 39);
		target_temp = dis_temp_1(gene);

		std::uniform_real_distribution<> dis_spo2_1(91., 92.5);
		target_spo2 = dis_spo2_1(gene);

		std::uniform_real_distribution<> dis_hr_1(95., 105.);
		target_hr = dis_hr_1(gene); }
		break;
	case 2:

	{std::uniform_real_distribution<> dis_temp_2(37.5, 38.);
		target_temp = dis_temp_2(gene);

		std::uniform_real_distribution<> dis_spo2_2(92.5, 94);
		target_spo2 = dis_spo2_2(gene);

		std::uniform_real_distribution<> dis_hr_2(85., 92);
		target_hr = dis_hr_2(gene); }
		break;
	case 3:

	{std::uniform_real_distribution<> dis_temp_3(37., 37.5);
		target_temp = dis_temp_3(gene);

		std::uniform_real_distribution<> dis_spo2_3(94., 95.);
		target_spo2 = dis_spo2_3(gene);

		std::uniform_real_distribution<> dis_hr_3(77., 85.);
		target_hr = dis_hr_3(gene); }
		break;
	default:
		break;
	}

	if (fever == true && resp_trbl == true) {


		TemperatureGen.aggrave(target_temp, start_h, total_duration_h, transition_period_h, recovery_period_h);
		SpO2Gen.aggrave(target_spo2, start_h, total_duration_h, transition_period_h, recovery_period_h);
		HRGen.aggrave(target_hr, start_h, total_duration_h, transition_period_h, recovery_period_h);

	}

	else if (fever == true && resp_trbl == false) {


		TemperatureGen.aggrave(target_temp, start_h, total_duration_h, transition_period_h, recovery_period_h);
		HRGen.aggrave(target_hr, start_h, total_duration_h, transition_period_h, recovery_period_h);
	}

	else if (resp_trbl == true) {

		SpO2Gen.aggrave(target_spo2, start_h, total_duration_h, transition_period_h, recovery_period_h);
		HRGen.aggrave(target_hr, start_h, total_duration_h, transition_period_h, recovery_period_h);

	}



};


