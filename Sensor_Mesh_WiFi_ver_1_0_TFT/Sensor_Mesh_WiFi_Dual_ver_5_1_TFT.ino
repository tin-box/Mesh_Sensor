//************************************************************
// Temperature Sensor network
//
// 1. Checks Temp and Humidity at a time between 4.5 and 6.5 seconds
// 2. Transmits data to Mesh, to node 0 if it exists, if not to all nodes
//  mtaft
//************************************************************
#include "Config.h"
#include "Functions.h"
#include "Mesh_net.h"
#include "Tft.h"

void taskReadTempF(void * parameter){
  for (;;) {
    readTemp();
    vTaskDelay( TASK_TEMP );
  }
}

void taskalarmMonitor(void * parameter){
  for (;;) {
    alarmMonitor();
    vTaskDelay( ALARM_CHECK );
  }
}

void taskshowEnv(void * parameter){
  for (;;) {
    showEnv();
    vTaskDelay( TASK_UI );
  }
}

void setup() {
  Serial.begin(115200);   // Initialize Serial port for monitoring
  configPinModes();       // Initialize IO pins
  initTemp();             // Initialize Temp Sensor
  initIO();               // OpCheck lights and buzzer at startup
  initMesh();             // Start Mesh network
  initScheduler();        // Setup Scheduler Tasks
  initTFT(); 
   //xTaskCreatePinnedToCore( taskReadTempF, "Task_1", 1000, NULL,  1, &Task1, 0);  
  xTaskCreate( taskReadTempF, "ReadTempF", 10000, NULL,  1, &TaskA);                      /* Core */
  xTaskCreate( taskalarmMonitor, "AlarmMon", 10000, NULL,  2, &TaskB);
  xTaskCreate( taskshowEnv, "ShowEnv", 10000, NULL,  0, &TaskC);
  Serial.println("Completed Setup routine");
  delay(500);
}  

void loop() {
  mesh.update();           // Runs the user scheduler as well
}
