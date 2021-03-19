#include "utilities.h"



void deep_sleep(int time_in_sec){

 esp_sleep_enable_timer_wakeup(time_in_sec * uS_TO_S_FACTOR);

 WiFi.disconnect();
 //WiFi.mode(WIFI_OFF);


 btStop();
 esp_bt_controller_disable();
  
 Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");
 Serial.println("Going to sleep now");
 delay(1000);
 Serial.flush(); 
 esp_deep_sleep_start();

  }


 

void detect_reset(){

int RESET = digitalRead(PIN_RESET_BUTTON);
  if (RESET == HIGH){
    Serial.println("Efface les paramètres de connexion et redémarre...");
    delay(1000);
    EEPROM.begin(EEPROM_SIZE);
    for (int i = 0 ; i < EEPROM_SIZE ; i++) {
      EEPROM.write(i, 0);
    }
    EEPROM.commit();
    Serial.println("Cleared EEPROM");
    ESP.restart();
    }
}




    
