        /* TFT Display functionality */
#include <TFT_eSPI.h>
#include "Hershey135_240.h"
#include "Free_Fonts.h"
#include "WiFi_30px.h"

TFT_eSPI tft = TFT_eSPI(135, 240); // configure 135*240 pixel resolution ST2289V

String nodeSelector(int sensor){
    if (sensor == 0){
    return "Node 1";    
  } else if (sensor == 7) {
    return "Node 2";
  } else {
    return "Sensor " + String(sensor);
  }
  return "Unk"; 
}

void meshStatus(){ // WiFi.RSSI()
  if (mesh.getNodeList().size() > 0) {
      if (abs(WIFI_RSSI) >= 90){
        tft.drawXBitmap(tft.width()-30,tft.height()-134, WiFi_Con1_30px_bits, 30, 30, TFT_WHITE, TFT_BLACK);
        return ; // Show Unusable signal strength icon
      } else if (abs(WIFI_RSSI) >= 70) {
        tft.drawXBitmap(tft.width()-30,tft.height()-134, WiFi_Con2_30px_bits, 30, 30, TFT_WHITE, TFT_BLACK);
        return ; // Show Weak signal strength icon
      } else if (abs(WIFI_RSSI) >= 50) {
        tft.drawXBitmap(tft.width()-30,tft.height()-134, WiFi_Con3_30px_bits, 30, 30, TFT_WHITE, TFT_BLACK);
        return ; // Show Good signal strength icon
      } else if (abs(WIFI_RSSI) >= 1) {
        tft.drawXBitmap(tft.width()-30,tft.height()-134, WiFi_Con4_30px_bits, 30, 30, TFT_WHITE, TFT_BLACK);
        return ; // Show Amazing signal strength icon
      } else {
        tft.drawXBitmap(tft.width()-30,tft.height()-134, WiFi_Con1_30px_bits, 30, 30, TFT_WHITE, TFT_BLACK);
        return ;
      }
  } else{
    tft.drawXBitmap(tft.width()-30,tft.height()-134, WiFi_Dis_30px_bits, 30, 30, TFT_WHITE, TFT_BLACK);
    return ;  // Show Disconnect Icon when Wirelss is disconnected
  }
}

void singleNode(int sensor){
  String str; 
  tft.setTextSize(1);  
  tft.setFreeFont(FSSB24);
  //tft.setTextDatum(MC_DATUM);  
  tft.setTextDatum(C_BASELINE); 
  tft.drawString(nodeSelector(sensor),  tft.width() / 2, tft.height() / 3, GFXFF);
  str = String(oneDecCtoF(pod_temp[sensor]) + "F  ");
  tft.drawString(str, tft.width() / 2, tft.height() / 1.5, GFXFF);
  tft.setFreeFont(FSSB12);
  tft.setTextColor(TFT_WHITE);  // TFT_WHITE
  str = " " + oneDecCtoF(pod_temp[SENSOR_NUMBER]) + "F  " + oneDecimal(pod_hum[SENSOR_NUMBER]) + "% ";
  tft.drawString(nodeSelector(SENSOR_NUMBER) + str, 0, tft.height() / 1.01, GFXFF);
  meshStatus();              // Show Signal Strength  
}

void setScreen(int i){
    if (setAlarm[i]== true) {
      tft.fillScreen(TFT_BLACK);       // TFT_BLACK
      tft.setTextColor(TFT_RED);  // TFT_WHITE
      tft.drawXBitmap(tft.width()-25,tft.height()-98, thermblk_bits, 18, 45, TFT_RED, TFT_BLACK);
  } else {
      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_WHITE);  // TFT_WHITE 
      tft.drawXBitmap(tft.width()-25,tft.height()-98, thermblk_bits, 18, 45, TFT_WHITE, TFT_BLACK);
  }
}

void showEnv(){   
  int increment = 5;
  tft.setRotation(3);
  if ( counter < 35) {
    int i = (counter/increment);       
        setScreen(i);
        singleNode(i);            
  } else {
    counter = 0;
  }
  counter++;
}

void initTFT(){
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);

  tft.setSwapBytes(true);
  tft.pushImage(0, 0,  240, 135, Hershey135_240);
  delay(2000);

  tft.setRotation(0);
  tft.fillScreen(TFT_RED);
  delay(100);
  tft.fillScreen(TFT_GREEN);
  delay(100);
  tft.fillScreen(TFT_BLUE);
  delay(100);
}
