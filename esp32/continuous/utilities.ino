#include "utilities.h"




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




    
