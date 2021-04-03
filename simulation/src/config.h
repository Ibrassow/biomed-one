#ifndef CONFIG_H_
#define CONFIG_H_


#include <string>
#include <map>



class Config {

public:


    Config();


    void loadConfig();

    std::map<std::string, int> getSettings();


private:


    int changeString2Bool(std::string var);


    std::string config_file_name;
    std::map<std::string, int> settings;




};




#endif