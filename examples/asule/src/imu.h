#ifndef _IMU_H_
#define _IMU_H_


#include <Arduino.h>
#include "../error.h"
#include "../setup.h"





class Imu
{
public:


public:
  Imu();
  ~Imu();

  bool  isInit(void);
  err_t begin(void);  
  err_t update(void);

	float pitch();
  float roll();
  float yaw();
	float heading();
  
  
private:
  bool is_init;
};


#endif