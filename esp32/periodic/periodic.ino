/*** PERIODIC VERSIONS ***/

#include <IotWebConf.h>
#include "oxymeter.h"
#include "webserver.h"
#include "sensors.h"
#include <InfluxDbClient.h>
#include "utilities.h"
#include "time.h"
#include "esp_timer.h"
#include "MovingAvg.h"
#include <Wifi.h>

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;


void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

#define MEASUREMENT_TIME  45.   // in seconds
#define TIME_TO_SLEEP 250.       // in seconds
float timer_sleep = 0;




///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// -- Temperature sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature TEMP_sensor(&oneWire);
float temperature = 0;

// -- indices for oxymeter data comparison
int prev_spo2_index = 0;
int prev_hr_index = 0;


const char deviceName[] = "MyTest";
const char deviceID[] = "XPBIO100"; //used as a password
int use_of = 0; //Variable destined to indicate if the config has already been done

// -- Battery life variable
int battery_percentage; 
const int greenpin = 32;
const int redpin = 33;

// -- moving averages
MovingAvg moving_temperature(45);
MovingAvg moving_hr(15);
MovingAvg moving_spo2(15);
MovingAvg moving_battery(30);


// -- averages
double avg_temperature = 0;
double avg_spo2 = 0;
double avg_hr = 0;
double avg_battery = 100;
long avg_rssi = 0;



/////////////////////////////////// WI-FI ////////////////////////////////////////////////////
DNSServer dnsServer;
WebServer server(80);
IotWebConf iotWebConf(deviceName, &dnsServer, &server, deviceID);

// -- Method declarations.
void handleRoot();
void handleParam();
void handleRedirect();

// -- Patient information details to send once recuperated
String name_ = ""; 
String surname_ = ""; 
String age_ = ""; 
String gender_ = ""; 
String mailadress_ = ""; 
String info_add_ = ""; 

//////////////////////////////////////////////////////////////////////////////////////////////


// -- InfluxDB
#define INFLUXDB_URL "http://influx.biomed.ulb.ovh"
#define INFLUXDB_DB_NAME "biomed1"
#define INFLUXDB_DB_USERNAME "biomed1"
#define INFLUXDB_DB_PASSWORD "M5xsTyS4fS0EYsFN"
InfluxDBClient Iclient(INFLUXDB_URL, INFLUXDB_DB_NAME);

RTC_DATA_ATTR int personalData_sent = 0;

String ID_patient = deviceID;

Point PatientDetails(ID_patient);
Point PatientData(ID_patient);




void setup() {

  setCpuFrequencyMhz(80);
  
  Serial.begin(115200);  
  
  Serial.println("Starting up...");


  Serial.println(getCpuFrequencyMhz());

  //--Init the temperature sensor
  TEMP_sensor.begin();  

  // -- LED battery
  pinMode(greenpin, OUTPUT);
  pinMode(redpin, OUTPUT);

  // -- Set up required URL handlers on the web server.

  server.on("/", handleRoot);
  server.on("/param", handleParam);
  server.on("/wait", handleRedirect);
  server.on("/config", []{ iotWebConf.handleConfig(); });
  server.onNotFound([](){ iotWebConf.handleNotFound(); });

  

  




  if (personalData_sent != 1) {

    // -- Initializing the configuration.
    iotWebConf.init();

    while (WiFi.status() != WL_CONNECTED){
    iotWebConf.doLoop();
    //Serial.println("Waiting for the wifi");
    delay(50);
    detect_reset();
    }

  // -- Database connection
  Iclient.setConnectionParamsV1(INFLUXDB_URL, INFLUXDB_DB_NAME, INFLUXDB_DB_USERNAME, INFLUXDB_DB_PASSWORD);
  
  PatientDetails.addTag("deviceID", deviceID);
  PatientDetails.addField("resp", 1);
  PatientDetails.addField("name", name_);
  PatientDetails.addField("surname", surname_);
  PatientDetails.addField("age", age_);
  PatientDetails.addField("gender", gender_);
  PatientDetails.addField("mail", mailadress_);
  PatientDetails.addField("info", info_add_);

  if (Iclient.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(Iclient.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(Iclient.getLastErrorMessage());
  }

    
   if (!Iclient.writePoint(PatientDetails)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(Iclient.getLastErrorMessage());
    iotWebConf.doLoop();
    Serial.println("Trying to send details...");
    Iclient.writePoint(PatientDetails);

    WiFi.disconnect(true);

  }
  else {
    
    personalData_sent = 1;
    
    }}




      //////////////////////Init oxymeter//////////////////////
  dbgoutln("Starting Arduino BLE Client application...");
  BLEDevice::init("");
  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 10 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  dbgoutln("BLE intensive scan for 10sec");
  pBLEScan->start(10, false); 
      //////////////////////End Init oxymeter//////////////////////
  
  

  Serial.println("Ready.");

  moving_temperature.begin();
  moving_hr.begin();
  moving_spo2.begin();
  moving_battery.begin();


  timer_sleep = esp_timer_get_time()/1000000; //esp_... return an uint64_t in microseconds
  //the time is converted in seconds and then rounded off by the type conversion


} 





void loop() {

  // -- doLoop should be called as frequently as possible.
  //iotWebConf.doLoop();

  ///////////////////////////////////////// OXYMETER CHECK /////////////////////////////////////////
  
  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server (i.e. Oxymeter) with which we wish to connect.  Now we connect to it.  Once we are connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
      dbgoutln("We are now connected to the BLE Server.");
    } else {
      dbgoutln("We have failed to connect to the server; there is nothing more we will do.");
    }
    doConnect = false;
  }
  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {  
    //we may do something here since we know we are connected to the oxymeter
  } else if (doScan) {
    //not connected anymore --> infinite scan 
    BLEDevice::getScan()->start(0); // this is just eample to start scan after disconnect (infinite scan !), most likely there is better way to do it in arduino
    Serial.println("Error from getscan");
  }  

  ///////////////////////////////////////// END OXYMETER CHECK /////////////////////////////////////////




  // -- Read sensor data and updates running averages
    
    temperature = getTemp(TEMP_sensor);
    Serial.println(temperature);
    avg_temperature = moving_temperature.reading(temperature);

    // -- Conditions to update averages only if they are new values of spo2 and hr
    if (CURRENT_spo2_index != prev_spo2_index and spo2_current != 0){ 
      Serial.println(spo2_current);
      prev_spo2_index = CURRENT_spo2_index;
      avg_spo2 = moving_spo2.reading(spo2_current);
      }

    if (CURRENT_hr_index != prev_hr_index and hr_current != 0){
      Serial.println(hr_current);
      prev_hr_index = CURRENT_hr_index;
      avg_hr = moving_hr.reading(hr_current);
      }

    battery_percentage = readBattery();
    Serial.println(battery_percentage);
    avg_battery = moving_battery.reading(battery_percentage);

    
  // -- Light the LED
  if (battery_percentage > 30){
    digitalWrite(greenpin, HIGH);
    Serial.println("Green on"); // turn on the LED
    }
  else if (10 > battery_percentage) {
    digitalWrite(redpin, HIGH);  // turn on the LED
    delay(50); // wait for half a second or 500 milliseconds
    digitalWrite(redpin, LOW); // turn off the LED
    delay(50); // wait for half a second or 500 milliseconds
    }
  else {
    digitalWrite(redpin, HIGH);  // turn on the LED
    }

  
  delay(500);
  detect_reset();

  if (((esp_timer_get_time()/1000000) - timer_sleep) > MEASUREMENT_TIME) {

    iotWebConf.init();
    iotWebConf.skipApStartup();
  
    while (WiFi.status() != WL_CONNECTED){
    iotWebConf.doLoop();
    //Serial.println("Waiting for the wifi");
    WiFi.reconnect();
    delay(1000);
    detect_reset();
    
    }
    
      // -- Database connection
  Iclient.setConnectionParamsV1(INFLUXDB_URL, INFLUXDB_DB_NAME, INFLUXDB_DB_USERNAME, INFLUXDB_DB_PASSWORD);
if (Iclient.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(Iclient.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(Iclient.getLastErrorMessage());
  }


    // -- mean value for wifi signal
    int n = 2;
    for (int i = 0; i < n; ++i){
      avg_rssi += WiFi.RSSI();
      delay(300);
      }
      avg_rssi /= n;
      Serial.println(avg_rssi);

      
  PatientData.addTag("deviceID", deviceID);
  
  PatientData.addField("resp", 2);
  PatientData.addField("battery", avg_battery);

  if (avg_temperature > 15) {
  PatientData.addField("temperature", avg_temperature);
    }
  PatientData.addField("rssi", avg_rssi);
  if (avg_hr != 0){
    PatientData.addField("hr", avg_hr);
    }
  if (avg_spo2 != 0){
    PatientData.addField("spo2", avg_spo2);
    }
  Iclient.writePoint(PatientData);

  if (!Iclient.writePoint(PatientData)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(Iclient.getLastErrorMessage());
    iotWebConf.doLoop();
    Iclient.writePoint(PatientData);
  }

  moving_temperature.reset();
  moving_hr.reset();
  moving_spo2.reset();
  moving_battery.reset();
  
  Serial.print("It's time!");
  deep_sleep(TIME_TO_SLEEP);   
    
    }
  
} 



  
  

















//////////////////////////////////////////////////// Handle web requests ////////////////////////////////////////////////////

void handleRoot()
{
  // -- Let IotWebConf test and handle captive portal requests.
  if (iotWebConf.handleCaptivePortal())
  {
    // -- Captive portal request were already served.
    //return;
  }

if (use_of == 0){
  server.send(200, "text/html", init_page);
}

else {
  server.send(200, "text/html", root_page);
}
 

}

void handleParam(){
  server.send(200, "text/html", param_page);
  };

void handleRedirect()
{
   name_ = String(server.arg("name")); 
   surname_ = String(server.arg("surname")); 
   age_ = String(server.arg("age")); 
   gender_ = String(server.arg("gender")); 
   mailadress_ = String(server.arg("mailadress")); 
   info_add_ = String(server.arg("info_add")); 
  
   Serial.println(name_);
   Serial.println(surname_);
   Serial.println(age_);
   Serial.println(gender_);
   Serial.println(mailadress_); 
   Serial.println(info_add_);

   if (use_of == 0){
  server.send(200, "text/html", redirect_page);
  use_of += 1;
    }
  else {
    server.send(200, "text/html", root_page);
  }

}
