
#include "asule.h"





uint32_t pre_time;


void setup() {  
  err_t err;
  
  Serial.begin(115200);

  err = asule.begin();

  Serial.println("err = " + String(err));

  pre_time = millis();
}

void loop() {

  asule.update();

  if (millis()-pre_time >= 100)
  {
    pre_time = millis();
    Serial.println("R/P/Y: " + String(asule.imu.roll) 
                        + " " + String(asule.imu.pitch) 
                        + " " + String(asule.imu.yaw) 
                        + " " + String(asule.imu.heading));  
  }
}

