#ifndef _ASULE_H_
#define _ASULE_H_


#include <Arduino.h>
#include "error.h"
#include "setup.h"

#include "./src/Servo/Servo.h"
#include "./src/mpu9250/MPU9250-DMP.h"







class Asule
{
public:
  MPU9250_DMP imu;
  Servo       servo[_PIN_SERVO_MAX]; 

public:
  Asule();
  ~Asule();


  err_t begin(void);  
  err_t update(void);

  
private:
  bool is_imu_init;

  err_t beginImu(void);
  err_t updateImu(void);
  err_t beginServo(void);
};


extern Asule asule;

#endif