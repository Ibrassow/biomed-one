
#include "simulator.h"

#include <iostream>
#include <fstream>







int main() {


	/*std::vector<std::string> answer;

	std::fstream file("config.txt", std::ios::in);
	std::string str;


	while (std::getline(file, str)) {
		
		answer.push_back(str);
		std::cout << str << std::endl;
	}

	int age = std::stoi(answer[0]);
	int duration_t = std::stoi(answer[1]);
	

	bool fever = (answer[2] == "True");
	bool resp_trb = (answer[3] == "True");

	int severity = std::stoi(answer[4]);
	int start = std::stoi(answer[5]);
	int tot_dur = std::stoi(answer[6]);
	int trans_p = std::stoi(answer[7]);
	int recov_p = std::stoi(answer[8]);*/

	//Simulator mainSimu(age, duration_t, "results_simu.txt");
	//mainSimu.activateAggravator(fever, resp_trb, severity, start, tot_dur, trans_p, recov_p);
	Simulator mainSimu(14, 22, "results_simu.txt");
	mainSimu.loadSettings();

	mainSimu.start();



	return 0;

}


