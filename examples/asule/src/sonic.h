#ifndef _SONIC_H_
#define _SONIC_H_


#include <Arduino.h>
#include "../error.h"
#include "../setup.h"





class Sonic
{
public:


public:
  Sonic();
  ~Sonic();

  bool  isInit(void);
  err_t begin(void);  
  err_t update(void);

  uint32_t distance(void);
  
private:
  bool is_init;  
};


#endif