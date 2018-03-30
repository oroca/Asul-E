#include "sonic.h"

#include "./driver/Servo/Servo.h"
#include "./driver/Filter/MedianFilter.h"



static MedianFilter sonic_filter(16, 0);

static const int sonic_trigger_pin = _PIN_SONIC_TRIG; //Trig pin
static const int sonic_echo_pin    = _PIN_SONIC_ECHO; //Echo pin
static uint32_t  distance_mm       = 0;

static volatile bool     started = false;
static volatile uint8_t  pos_cnt = 0;
static volatile uint32_t pos_tbl[2] = {0, 0};




void sonicISR() 
{
  if (started == true)
  {
    if (digitalRead(sonic_echo_pin) == HIGH && pos_cnt == 0)
    {
      pos_tbl[pos_cnt] = micros();
      pos_cnt++;
    }
    if (digitalRead(sonic_echo_pin) == LOW && pos_cnt == 1)
    {
      pos_tbl[pos_cnt] = micros();
      pos_cnt++;
    }

    if (pos_cnt >= 2)
    {
      started = false;
    }
  }  
}

Sonic::Sonic()
{
  is_init = false;
}

Sonic::~Sonic()
{

}

err_t Sonic::begin(void)
{
  err_t err = 0;

  pinMode(sonic_trigger_pin, OUTPUT); // Trigger is an output pin
  pinMode(sonic_echo_pin,    INPUT);  // Echo is an input pin


  attachInterrupt(digitalPinToInterrupt(sonic_echo_pin),sonicISR, CHANGE);	

  is_init = true;
  return err;
}

bool Sonic::isInit(void)
{
  return is_init;  
}

uint32_t Sonic::distance(void)
{
  return distance_mm;
}

err_t Sonic::update(void)
{
  err_t err = 0;
  static uint32_t pre_time;  


  if (millis()-pre_time >= 40)
  {
    uint32_t  duration;


    pre_time = millis();

    if (started == false && pos_cnt == 2)
    {
      duration = constrain(pos_tbl[1]-pos_tbl[0], 0, 18000);

      if (duration == 0)
      {
        duration = 18000;
      }  
    }
    else
    {
      duration = 18000;
    }


    sonic_filter.in(duration);
    duration = sonic_filter.out();
    distance_mm = (((double)duration / 2.9) / 2.0);     // Actual calculation in mm    


    pos_cnt = 0;
    started = true;

    digitalWrite(sonic_trigger_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(sonic_trigger_pin, HIGH);    // Trigger pin to HIGH
    delayMicroseconds(10); // 10us high
    digitalWrite(sonic_trigger_pin, LOW);     // Trigger pin to HIGH    
  }


  return err;
}