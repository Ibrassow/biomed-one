#ifndef _SIMULATION_H_
#define _SIMULATION_H_



#include "typevector.h"






class ISimulation {


public:

	virtual vec1D GetTimeSeries() = 0;
	virtual ~ISimulation() {};




private:

	virtual void GenerateTimeSeries() = 0;
	double mean;
	double sd;
	int N_points;




};


class AggravatorModel {

public:
    AggravatorModel(double heigth, double total_duration_h, double transition_period_h, double recovery_period_h, bool reverse_dir);

    vec1D generateModel();
private:

    vec1D logistic_model(double duration_h, double sign_evolution);
    vec1D create_constant_phase();

    

    double K_heigth;

    //Ensure that total > recov + transition
    double recovery_time;
    double transition_time;

    double constant_phase_duration;
    double variation_direction;

    vec1D model_series;
    
};













































#endif
