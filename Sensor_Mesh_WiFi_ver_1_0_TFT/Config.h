/* Configuration and Initialization elements */
#include "painlessMesh.h"
#include "DHTesp.h"
#include <AHT10.h>
#include <Wire.h>

#define   SENSOR_NUMBER      0
#define   VERBOSE         false
#define   BUZZER          12
#define   BUZPWR          13
#define   AHT10_PWR       17
#define   ALARM_LENGTH    3000    // seconds * 1000 !!!!!!!!!!!!
#define   ALARM_CHECK     250
#define   JSON_DOC_SZ     128
#define   TASK_SEND       5000
#define   TASK_TEMP       2000
#define   TASK_UI         1000
#define   MESH_PREFIX     "sensor_net"
#define   MESH_PASSWORD   "Pecan-Neurotic-Equipment9"
#define   MESH_PORT       5050
#define   RADIO_CH        11

size_t 	MainnodeId    = 0;
int     counter 	  = 0;
long    WIFI_RSSI     = 52;
bool    setAlarm[7]   = {0,0,0,0,0,0,0};    // !!!!!!!!!!!!!
int     bCounter      = 0;
int     bCastIntvl    = 2;
int     dhtPin        = 27;              /* Pin number for DHT11 data pin */
float   ALARMTEMP     = 40.5;           // !! Celsius!! Temperature for alarm
int     ALARMHUM      = 96;
float   pod_temp[7]   = {15,15,15,15,15,15,15};
float   pod_hum [7]   = {15,15,15,15,15,15,15};

TaskHandle_t TaskA, TaskB, TaskC;
DHTesp  dht;
AHT10 aht(AHT10_ADDRESS_0X38);
painlessMesh  mesh;

Scheduler userScheduler; // to control user tasks

void sendMessage() ;    // Send Data out on network
void readTemp();        // Collect local sensor data  
void alarmMonitor();    // Monitor for Alarm condition
void showEnv();         // Display output to TFT

Task taskSendMessage( TASK_SEND,  TASK_FOREVER, &sendMessage );
//Task taskshowEnv (    TASK_UI,    TASK_FOREVER, &showEnv);
//Task taskalarmMonitor(ALARM_CHECK, TASK_FOREVER, &alarmMonitor);
//Task taskReadTemp(    TASK_TEMP,  TASK_FOREVER, &readTemp);

void configPinModes() {
  
  Serial.println("Configuring PinModes");
  pinMode(BUZZER, OUTPUT);
  pinMode(BUZPWR, OUTPUT);
  pinMode(AHT10_PWR,OUTPUT);
  Serial.println("pinMode: " + String(BUZZER) + " configured");
  Serial.println("Configured PinModes");
}

bool initTemp() {
  dht.setup(dhtPin, DHTesp::DHT11);      // Initialize temperature sensor
  Serial.println("DHT initiated");
  if (aht.begin() != true){
    Serial.println(F("AHT10 Fail")); //(F()) save string to flash free RAM    
  }
  aht.setCycleMode();
}

void initIO() {  
  Serial.println("Initializing and Testing Outputs");
  digitalWrite(BUZZER, LOW);
  digitalWrite(BUZPWR, HIGH);
  Serial.println("Channel: " + String(BUZZER) + " Testing");
  delay(1000);
  digitalWrite(BUZZER, HIGH); 
  digitalWrite(AHT10_PWR, HIGH);
  Serial.println("Initialized Outputs");
}

void initScheduler(){
  Serial.println("Initializing Scheduler");
  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
  Serial.println("...taskSendMessage");
  // userScheduler.addTask( taskReadTemp );
  // taskReadTemp.enable();
  // Serial.println("...taskReadTemp");
  // userScheduler.addTask( taskalarmMonitor );
  // taskalarmMonitor.enable();
  // Serial.println("...taskalarmMonitor");
  // userScheduler.addTask( taskshowEnv );
  // taskshowEnv.enable();
  // Serial.println("...taskshowEnv");
  Serial.println("Initialized Scheduler");
}
