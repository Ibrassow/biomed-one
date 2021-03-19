#ifndef SENSORS_H
#define SENSORS_H

#include <OneWire.h> 
#include <DallasTemperature.h>


#define TEMPERATURE_SENSOR "ds18b20"
#define ONE_WIRE_BUS 21



int readBattery();
float getTemp(DallasTemperature sensor);











  














#endif
