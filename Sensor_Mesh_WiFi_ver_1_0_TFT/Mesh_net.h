/* Wireless Mesh functionality */
void sendMessage() {
  //WIFI_RSSI = WiFi.RSSI();
  DynamicJsonDocument jsonBuffer(JSON_DOC_SZ);            // Instantiate JSON doc and set size (128)
  JsonObject msg = jsonBuffer.to<JsonObject>();   // Instantiate JSON object
  String str;                                     // String that JSON object is cast to for transmission
                              // Add current data to the JSON object for transmission
  msg["station"]  = SENSOR_NUMBER;       // send Local Sensor#
  msg["nodeid"]   = mesh.getNodeId(); // send Local net NodeId
  msg["temp"]     = pod_temp[SENSOR_NUMBER];          // send Local Temp
  msg["humid"]    = pod_hum[SENSOR_NUMBER];           // send Local Humidity
    
  serializeJson(msg, str);            // Serialize the data into JSON String to be transmitted 
  if (bCounter >= bCastIntvl){
    MainnodeId = 0;
    bCounter = 0;   
  }
  if (MainnodeId == 0){                 // If we don't know the MainnodeId yet broadcast the data to all nodes
    mesh.sendBroadcast(str); 
  } else {                             // If we know MainnodeId just send data to it. 
    mesh.sendSingle(MainnodeId, str);
    bCounter++;       
  }                                     // Randomize send period to reduce collisions 
  taskSendMessage.setInterval( random( TASK_SECOND * .83, TASK_SECOND * 1.27 ));
}

void receivedCallback( uint32_t from, String &msg ) {
  if (VERBOSE == true){
    Serial.printf("Rcv src %u msg= %s\n", from, msg.c_str());
  }
  DynamicJsonDocument jsonBuffer(JSON_DOC_SZ + msg.length());
  DeserializationError error = deserializeJson(jsonBuffer, msg);
  if (error) {
    Serial.printf("DeserializationError\n");
    return;
  }
  JsonObject root = jsonBuffer.as<JsonObject>();
  pod_temp[size_t(root["station"])]  = root["temp"];
  pod_hum[size_t(root["station"])]   = root["humid"];
  if (root["station"] == 0) {         // Check if we recieved data from the Main node
    MainnodeId = root["nodeid"];       // If it is record the Main nodes net ID for use later
    if (VERBOSE == true){
      Serial.printf("Main node detected!\n");    
      Serial.printf("Rcvd src %u msg=%s\n", from, msg.c_str());
    }
  }
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void initMesh(){
        // all types ( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE )
        //mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA, RADIO_CH );   // Set channel
  if (VERBOSE == true){
    mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE );  // set before init() so that you can see startup messages 
  } else {
    mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages   
  } 
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, RADIO_CH );   // Set channel
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);  
}
