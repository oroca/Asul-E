#ifndef _ASULE_H_
#define _ASULE_H_


#include <Arduino.h>
#include "error.h"
#include "setup.h"




#include "./src/imu.h"
#include "./src/rcservo.h"
#include "./src/sonic.h"




class Asule
{
public:
  Imu      imu;
  RcServo  servo;
  Sonic    sonic;

public:
  Asule();
  ~Asule();


  err_t begin(void);  
  err_t update(void);
  
private:
};


extern Asule asule;

#endif