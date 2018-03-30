#include "rcservo.h"

#include "./driver/Servo/Servo.h"


static Servo servo[_PIN_SERVO_MAX]; 



RcServo::RcServo()
{
  is_init = false;
}

RcServo::~RcServo()
{

}

err_t RcServo::begin(void)
{
  err_t err = 0;
  uint16_t i;


  servo[0].attach(_PIN_SERVO_1, 500, 1500, 2500);  
  servo[1].attach(_PIN_SERVO_2, 500, 1500, 2500);  
  servo[2].attach(_PIN_SERVO_3, 500, 1500, 2500);  
  servo[3].attach(_PIN_SERVO_4, 500, 1500, 2500);  
  servo[4].attach(_PIN_SERVO_5, 500, 1500, 2500);  
  servo[5].attach(_PIN_SERVO_6, 500, 1500, 2500);  
  servo[6].attach(_PIN_SERVO_7, 500, 1500, 2500);  
  servo[7].attach(_PIN_SERVO_8, 500, 1500, 2500);  


  for (i=0; i<_PIN_SERVO_MAX; i++)
  {  
    servo[i].writeAngle(0, 0);
  }

  is_init = true;

  return err;
}

bool RcServo::isInit(void)
{
  return is_init;  
}

void RcServo::writeAngle(uint8_t ch, int value, uint8_t speed)
{
  if (ch < _PIN_SERVO_MAX)
  {
    servo[ch].writeAngle(value, speed);
  }
}

int RcServo::readAngle(uint8_t ch)
{
  if (ch >= _PIN_SERVO_MAX) return 0;
  
  return servo[ch].readAngle();
}
