#include "asule.h"





Asule::Asule()
{
}

Asule::~Asule()
{
}

err_t Asule::begin(void)
{
  err_t err = 0;

  err |= imu.begin();
  err |= servo.begin();
  err |= sonic.begin();

  return err;
}



err_t Asule::update(void)
{
  err_t err = 0;


  if (imu.isInit() == true)
  {
    err |= imu.update();
  }
  if (sonic.isInit() == true)
  {
    err |= sonic.update();
  }
  return err;
}














Asule asule;


