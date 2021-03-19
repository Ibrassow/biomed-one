#include <EEPROM.h>


#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define PIN_RESET_BUTTON 13
#define EEPROM_SIZE 4096



void detect_reset();

void deep_sleep(int time_in_sec);
