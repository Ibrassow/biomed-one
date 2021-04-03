#include "simulation.h"
#include "utilities/tools.h"
#include <algorithm>
#include <math.h>  
#include <exception>
#include <iostream>

struct DurationException : public std::exception {
    const char* what() const throw () {
        return "ERROR: Total duration too low for the specified parameter";
    }
};


AggravatorModel::AggravatorModel(double heigth, double total_duration_h, double transition_period_h, double recovery_period_h, bool reverse_dir = false) 
    : K_heigth(heigth)
{


    //Ensure that total > recov + transition
    recovery_time = recovery_period_h;
    transition_time = transition_period_h;


    try {
        constant_phase_duration = total_duration_h - recovery_period_h - transition_period_h;

        if (constant_phase_duration < 0.) {
            DurationException dd;
            throw dd;
        }
    }

    catch (DurationException& e) {
        std::cout << e.what() << std::endl;
        exit(0);
    }



    
    //constant_phase_duration = total_duration_h - recovery_period_h - transition_period_h;



    //check if one or minus one
    if (reverse_dir == false)
    {
        variation_direction = 1.;
    }
    else if (reverse_dir == true) {

        variation_direction = -1.;
    }
    

};




vec1D AggravatorModel::logistic_model(double duration_h, double sign_evolution) {


    vec1D data = arange(0., duration_h * 3600.);

    double M_shift = duration_h * 3600 / 2;

    double charac_time =  sign_evolution * duration_h * 3600; //variation_direction *

    double gr = std::log(9801); // 1% to 99%

    std::transform(data.begin(), data.end(), data.begin(), [this, M_shift, charac_time, gr](double t) {return K_heigth / (1 + std::exp(- gr * (t - M_shift) / charac_time)); });
    

    return data;

};



vec1D AggravatorModel::create_constant_phase() {

    double HH;


    HH = K_heigth;

    vec1D points((static_cast<int> (constant_phase_duration)) * 3600, HH);

    return points;
};


vec1D AggravatorModel::generateModel() {

    vec1D phase1 = logistic_model(transition_time, 1.);
    model_series.insert(std::end(model_series), phase1.begin(), phase1.end());

    vec1D phase2 = create_constant_phase();
    model_series.insert(std::end(model_series), phase2.begin(), phase2.end());

    vec1D phase3 = logistic_model(recovery_time, -1.);
    model_series.insert(std::end(model_series), phase3.begin(), phase3.end());


    return model_series;

}