
#include "sensors.h"



int readBattery(){
  float percentage_ = 100;
  //float voltage = analogRead(36) * 0.0010688;  
  float voltage_ = analogRead(36) * 0.001047363;
  Serial.println("Voltage = " + String(voltage_));
  percentage_ = 2808.3808 * pow(voltage_, 4) - 43560.9157 * pow(voltage_, 3) + 252848.5888 * pow(voltage_, 2) - 650767.4615 * voltage_ + 626532.5703;
  if (voltage_ > 4.19) percentage_ = 100;
  else if (voltage_ <= 3.50) percentage_ = 0;
  return int(percentage_);
}

float getTemp(DallasTemperature sensor){
  sensor.requestTemperatures();
  float celsius_temp = sensor.getTempCByIndex(0);
  return celsius_temp; 
  }
