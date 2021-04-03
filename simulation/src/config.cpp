#include "config.h""

#include <fstream>
#include <sstream>

Config::Config() : config_file_name("config.txt"), settings{} {
};


int Config::changeString2Bool(std::string var) {
    if ((var == "true") || (var == "True")) { return true; }
    else if ((var == "false") || (var == "False")) { return false; }
}

void Config::loadConfig() {

    std::fstream config_file(config_file_name, std::ios::in);

    //Handle file not opening

    std::string line;

    int temp;
    std::string temp_string;

    while (getline(config_file, line)) {

        std::istringstream strin(line.substr(line.find("=") + 1));

        if (line.find("age") != -1) {
            strin >> temp;
            settings.insert({ "age", temp });
        }

        else if (line.find("duration") != -1) {
            strin >> temp;
            settings.insert({ "duration", temp });
        }
        else if (line.find("fever") != -1) {
            strin >> temp_string;
            temp = changeString2Bool(temp_string);
            settings.insert({ "fever", temp });
        }
        else if (line.find("resp_trbl") != -1) {
            strin >> temp;
            settings.insert({ "resp_trbl", temp });
        }
        else if (line.find("severity") != -1) {
            strin >> temp;
            settings.insert({ "severity", temp });
        }
        else if (line.find("start_h") != -1) {
            strin >> temp;
            settings.insert({ "start_h", temp });
        }
        else if (line.find("total_duration_h") != -1) {
            strin >> temp;
            settings.insert({ "total_duration_h", temp });
        }
        else if (line.find("transition_period_h") != -1) {
            strin >> temp;
            settings.insert({ "transition_period_h", temp });
        }
        else if (line.find("recovery_period_h") != -1) {
            strin >> temp;
            settings.insert({ "recovery_period_h",temp });
        }

    }


    config_file.close();


}


std::map<std::string, int> Config::getSettings() {

    return settings;
};
