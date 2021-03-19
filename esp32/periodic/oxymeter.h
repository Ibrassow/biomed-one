#ifndef OXYMETER_H
#define OXYMETER_H



#include "BLEDevice.h"
#ifndef _DBG   
  #define dbgoutln(a)  
  #define dbgout(a)
  #define dbgoutw(a)
#else
  #define dbgoutln(a) Serial.println(a)
  #define dbgout(a) Serial.print(a)
  #define dbgoutw(a) Serial.write(a)
#endif

//If you know the oxy BLE address, you can replace by it, so it will always connect to a specific one
#define OXYADDRESS "00:00:00:00:00:00"

//Device name of the oxymeter (don't change it)
#define OXYNAME "SP001"

//NULL BLUTOOTH MAC ADDRESS
#define BLENULLADDRESS "00:00:00:00:00:00"


int spo2_current;
int hr_current;


int CURRENT_spo2_index = 0;
int CURRENT_hr_index = 0;


// BLE Address of the oxymeter
static BLEAddress OxyAddr(OXYADDRESS);
static BLEUUID serviceUUID("f000efe0-0451-4000-0000-00000000b000");
static BLEUUID    charUUID("f000efe3-0451-4000-0000-00000000b000");


static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;
static int OxyRSSI = 0;
//timestamps when oxymeter was respectively on and off
unsigned long laston = 0, lastoff = 0;

static void spo2piCallback(int _spo2);
static void bpmCallback(int _bpm);
static void onoffCallback(boolean is_on,long duration);


class MyClientCallback : public BLEClientCallbacks {

	void onConnect(BLEClient* pclient);
	void onDisconnect(BLEClient* pclient);

};

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify);
  
  bool connectToServer();
  
 
  class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    /**Called for each advertising BLE server.*/
    void onResult(BLEAdvertisedDevice advertisedDevice);
}; 



//void InitOxymeter();

//void CheckdoConnect();




#endif
