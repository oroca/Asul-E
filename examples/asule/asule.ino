
#include "asule.h"




#define _DEBUG_IMU        1
#define _DEBUG_SONIC      1
#define _DEBUG_SERVO      1


uint32_t pre_time;


void setup() {  
  err_t err;
  
  Serial.begin(115200);

  err = asule.begin();

  Serial.println("err = " + String(err));

  pre_time = millis();

  pinMode(13, OUTPUT);
}

void loop() {

  asule.update();

  if (millis()-pre_time >= 100)
  {
    digitalWrite(13, !digitalRead(13));

    pre_time = millis();
    #if _DEBUG_IMU
    Serial.print("R/P/Y: " + String(asule.imu.roll()) 
                        + " " + String(asule.imu.pitch()) 
                        + " " + String(asule.imu.yaw()) 
                        + " " + String(asule.imu.heading()));  
    Serial.print(" ");
    #endif

    #if _DEBUG_SONIC
    Serial.print("Sonic: " + String(asule.sonic.distance()) + " mm ");  
    Serial.print(" ");
    #endif

    #if _DEBUG_SERVO
    if (Serial.available())
    {
      uint8_t ch;

      ch = Serial.read();

      if (ch == '1')
      {
        asule.servo.writeAngle(0, 0, 8);
      }
      if (ch == '2')
      {
        asule.servo.writeAngle(0, 900, 8);
      }      
    }

    Serial.print("Angle: " + String(asule.servo.readAngle(0)));      
    #endif

    Serial.println("");    
  }
}

