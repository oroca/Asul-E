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

  
private:
  bool is_init;
};


#endif