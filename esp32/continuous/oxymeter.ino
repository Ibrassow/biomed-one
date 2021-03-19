#include "oxymeter.h"


//callback with the SpO2 and Pulse intensity values
static void spo2piCallback(int _spo2) {
  spo2_current = _spo2;
  CURRENT_spo2_index += 1;
  
}


static void bpmCallback(int _hr) {
  hr_current = _hr;
  CURRENT_hr_index += 1;
}




//callback when the oxymeter is on or off
static void onoffCallback(boolean is_on,long duration) {
  if (is_on) {
    Serial.print("The oxymeter is now on and was off during ");
    Serial.print(-duration/1000);
  } else {
    Serial.print("The oxymeter is now off and was on during ");
    Serial.print(duration/1000);
  }
  Serial.println("sec");
}




//Notification callback from the oxymeter 
static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  dbgout("raw data notification : ");
  for (int i = 0; i < length; i++) {
    dbgout(pData[i]);
    dbgout(", ");
  }
  dbgoutln();
  switch (pData[0]) {
    case 10:
      bpmCallback(pData[12]);
      break;
    case 18:
      spo2piCallback(pData[12]);
      break;
    case 252:
      if (pData[2] == 2) laston = millis();
      else lastoff=millis();
      onoffCallback(pData[2] == 2,lastoff-laston);
      break;    
  }  
}







//Connection to the oxymeter
bool connectToServer() {
  dbgout("Forming a connection to ");
  dbgoutln(myDevice->getAddress().toString().c_str());


  BLEClient*  pClient  = BLEDevice::createClient();
  dbgoutln(" - Created client");

  pClient->setClientCallbacks(new MyClientCallback());

  // Connect to the remove BLE Server.
  pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
  dbgoutln(" - Connected to server");

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    dbgout("Failed to find our service UUID: ");
    dbgoutln(serviceUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  dbgoutln(" - Found our service");

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr) {
    dbgout("Failed to find our characteristic UUID: ");
    dbgoutln(charUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  dbgoutln(" - Found our characteristic");


  // Read the value of the characteristic.
  if (pRemoteCharacteristic->canRead()) {
    std::string value = pRemoteCharacteristic->readValue();
    dbgout("The characteristic value was: ");
    dbgoutln(value.c_str());
  }


  if (pRemoteCharacteristic->canNotify())
    pRemoteCharacteristic->registerForNotify(notifyCallback);

  connected = true;
  return true;
}
/**
   Scan for BLE servers and find the first one that advertises the service we are looking for.
*/

    /**
        Called for each advertising BLE server.
    */
void MyAdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice) {
      dbgout("BLE Advertised Device found: ");
      dbgoutln(advertisedDevice.toString().c_str());


      // We have found a device, let us now see if it is the oxymeter we are looking for
      // If the ble address is specified, it has priority vs. the oxy name and also, in case of disconnection, reconnect to the same one
      if ((OxyAddr.toString()==BLENULLADDRESS && advertisedDevice.getName() == OXYNAME) || OxyAddr.equals(advertisedDevice.getAddress())) {
        BLEDevice::getScan()->stop();
        myDevice = new BLEAdvertisedDevice(advertisedDevice);
        doConnect = true;
        doScan = true;
        OxyAddr =  advertisedDevice.getAddress(); 
        OxyRSSI =  advertisedDevice.getRSSI();      
      } // Found our server
    } // onResult








//callbacks on connection and disconnection of the oxymeter

void MyClientCallback::onConnect(BLEClient* pclient) {
  Serial.println("Connected to the oxymeter");
}


void MyClientCallback::onDisconnect(BLEClient* pclient) {
  connected = false; //do not remove this !!      
  Serial.println("Disconnected from the oxymeter");
}
