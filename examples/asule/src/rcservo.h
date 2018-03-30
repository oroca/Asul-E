#ifndef _RCSERVO_H_
#define _RCSERVO_H_


#include <Arduino.h>
#include "../error.h"
#include "../setup.h"





class RcServo
{
public:


public:
  RcServo();
  ~RcServo();

  bool  isInit(void);
  err_t begin(void);  

  void writeAngle(uint8_t ch, int value, uint8_t speed);
  int  readAngle(uint8_t ch);
private:
  bool is_init;
};


#endif