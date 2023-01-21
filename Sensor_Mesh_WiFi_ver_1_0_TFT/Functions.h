          /* Standard Functions */


void alarmBuzzer(bool test){  // example: alarmBuzzer(alarmStatus());
  if(test == true) {
    digitalWrite(BUZZER, LOW); // Sound buzzer for overtemp 
    digitalWrite(BUZPWR, HIGH);
  } else {
    digitalWrite(BUZZER, HIGH); // Silence buzzer for undertemp
  }
}

bool alarmStatus(){    
  for (int i = 0; i < 7; i++){  // int((&pod_temp + 1) - pod_temp)
    if (pod_temp[i] > ALARMTEMP && i == SENSOR_NUMBER){
      setAlarm[i] = true;
      return true;   // This unit has overtemp then alarm
    } else if (pod_temp[i] > ALARMTEMP && SENSOR_NUMBER == 0){  
      setAlarm[i] = true;
      return true;    // Main node unit detects overtemp on another node
    }
    setAlarm[i] = false;
  }    
  return false;   // No alarm if no criteria met
}

void alarmMonitor(){
  alarmBuzzer(alarmStatus());
}

bool meshConn(){
  if (mesh.getNodeList().size() > 0) {
    return true;   // Turn on Connected Icon when Wirelss connects
  } else{
    return false;  // turn on Disconnect Icon when Wirelss is disconnected
  }
}

void readTemp() {
  TempAndHumidity newValues = dht.getTempAndHumidity();
  if (dht.getStatus() != 0) {   // Check if any reads failed and exit early (to try again).
    Serial.println("DHT11 error status: " + String(dht.getStatusString()));
  } else {
  pod_temp[SENSOR_NUMBER]= newValues.temperature;
  pod_hum[SENSOR_NUMBER] = newValues.humidity;
  }
  if (aht.readRawData() != AHT10_ERROR){
   pod_temp[SENSOR_NUMBER]= aht.readTemperature(AHT10_USE_READ_DATA);
   pod_hum[SENSOR_NUMBER] = aht.readHumidity(AHT10_USE_READ_DATA);    
  } else {
    Serial.print(F("Failed to read - reset: ")); 
    Serial.println(aht.softReset());         //reset 1-success, 0-failed
  }
  alarmBuzzer(alarmStatus());    // set leds and buzzer based on latest sensor input   
}

void codeForTask1( void * parameter )
{
  for (;;) {
    Serial.print("This Task runs on Core: ");
    Serial.println(xPortGetCoreID());
  }
}  

String twoDigits(int digits){             // utility function for digital clock display: prints leading 0
  if(digits < 10) {
    String i = '0'+String(digits);
    return i;
  }
  else {
    return String(digits);
  }
}

String floattostring(float in){           // toString
  char out[15];
  dtostrf(in, 4, 0, out);
  return String(out);
}

String oneDecimal(float f){               // round a float to one decimal
  char str[6];
  dtostrf(f, 5, 1, str);
  return String(str);
}

String oneDecCtoF(float f){
  char str[6];
  dtostrf(((f*1.8)+32), 5, 1, str);
  return String(str);
}
