/*
 Servo.cpp - Interrupt driven Servo library for Arduino using 16 bit timers- Version 2
 Copyright (c) 2009 Michael Margolis.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#define ARDUINO_ARCH_AVR



#if defined(ARDUINO_ARCH_AVR)

#include <avr/interrupt.h>
#include <Arduino.h>

#include "Servo.h"

#define usToTicks(_us)    (( clockCyclesPerMicrosecond()* _us) / 8)     // converts microseconds to tick (assumes prescale of 8)  // 12 Aug 2009
#define ticksToUs(_ticks) (( (unsigned)_ticks * 8)/ clockCyclesPerMicrosecond() ) // converts from ticks back to microseconds


#define TRIM_DURATION       2                               // compensation ticks to trim adjust for digitalWrite delays // 12 August 2009

//#define NBR_TIMERS        (MAX_SERVOS / SERVOS_PER_TIMER)

static servo_t servos[MAX_SERVOS];                          // static array of servo structures
static volatile int8_t Channel[_Nbr_16timers ];             // counter for the servo being pulsed for each timer (or -1 if refresh interval)

volatile uint8_t ServoCount = 0;                                     // the total number of attached servos


static volatile void (*servo_isr_func)(void) = NULL;
static void handle_speed(void);


// convenience macros
#define SERVO_INDEX_TO_TIMER(_servo_nbr) ((timer16_Sequence_t)(_servo_nbr / SERVOS_PER_TIMER)) // returns the timer controlling this servo
#define SERVO_INDEX_TO_CHANNEL(_servo_nbr) (_servo_nbr % SERVOS_PER_TIMER)       // returns the index of the servo on this timer
#define SERVO_INDEX(_timer,_channel)  ((_timer*SERVOS_PER_TIMER) + _channel)     // macro to access servo index by timer and channel
#define SERVO(_timer,_channel)  (servos[SERVO_INDEX(_timer,_channel)])            // macro to access servo class by timer and channel

#define SERVO_MIN() (MIN_PULSE_WIDTH - this->min * 4)  // minimum value in uS for this servo
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->max * 4)  // maximum value in uS for this servo

/************ static functions common to all instances ***********************/

extern void servoISR(void);

static inline void handle_interrupts(timer16_Sequence_t timer, volatile uint16_t *TCNTn, volatile uint16_t* OCRnA)
{
  if( Channel[timer] < 0 )
    *TCNTn = 0; // channel set to -1 indicated that refresh interval completed so reset the timer
  else{
    if( SERVO_INDEX(timer,Channel[timer]) < ServoCount && SERVO(timer,Channel[timer]).Pin.isActive == true )
      digitalWrite( SERVO(timer,Channel[timer]).Pin.nbr,LOW); // pulse this channel low if activated
  }

  Channel[timer]++;    // increment to the next channel
  if( SERVO_INDEX(timer,Channel[timer]) < ServoCount && Channel[timer] < SERVOS_PER_TIMER) {
    *OCRnA = *TCNTn + SERVO(timer,Channel[timer]).ticks;
    if(SERVO(timer,Channel[timer]).Pin.isActive == true)     // check if activated
      digitalWrite( SERVO(timer,Channel[timer]).Pin.nbr,HIGH); // its an active channel so pulse it high
  }
  else {
    // finished all channels so wait for the refresh period to expire before starting over
    if( ((unsigned)*TCNTn) + 4 < usToTicks(REFRESH_INTERVAL) )  // allow a few ticks to ensure the next OCR1A not missed
      *OCRnA = (unsigned int)usToTicks(REFRESH_INTERVAL);
    else
      *OCRnA = *TCNTn + 4;  // at least REFRESH_INTERVAL has elapsed
    Channel[timer] = -1; // this will get incremented at the end of the refresh period to start again at the first channel
    if (servo_isr_func != NULL)
    {
      (*servo_isr_func)();
    }
    else
    {
      handle_speed();
    }
  }
}

static void handle_speed(void)
{
  for (int i=0; i<ServoCount; i++)
  {
    if (servos[i].Pin.isActive == true)
    {
      if (servos[i].ticks_cur != servos[i].ticks_goal)
      {
        if (servos[i].ticks_goal > servos[i].ticks_cur)
        {
          servos[i].ticks_cur += servos[i].ticks_speed;
          if (servos[i].ticks_goal < servos[i].ticks_cur)
          {
            servos[i].ticks_cur = servos[i].ticks_goal;
          }
          servos[i].ticks = servos[i].ticks_cur>>8;
        }
        else if (servos[i].ticks_goal < servos[i].ticks_cur)
        {
          servos[i].ticks_cur -= servos[i].ticks_speed;
          if (servos[i].ticks_goal > servos[i].ticks_cur)
          {
            servos[i].ticks_cur = servos[i].ticks_goal;
          }      
          servos[i].ticks = servos[i].ticks_cur>>8;    
        }                
      }
    }
  }
}

void servoSetISR(void (*func)())
{
  servo_isr_func = func;
}

#ifndef WIRING // Wiring pre-defines signal handlers so don't define any if compiling for the Wiring platform
// Interrupt handlers for Arduino
#if defined(_useTimer1)
SIGNAL (TIMER1_COMPA_vect)
{
  handle_interrupts(_timer1, &TCNT1, &OCR1A);
}
#endif

#if defined(_useTimer3)
SIGNAL (TIMER3_COMPA_vect)
{
  handle_interrupts(_timer3, &TCNT3, &OCR3A);
}
#endif

#if defined(_useTimer4)
SIGNAL (TIMER4_COMPA_vect)
{
  handle_interrupts(_timer4, &TCNT4, &OCR4A);
}
#endif

#if defined(_useTimer5)
SIGNAL (TIMER5_COMPA_vect)
{
  handle_interrupts(_timer5, &TCNT5, &OCR5A);
}
#endif

#elif defined WIRING
// Interrupt handlers for Wiring
#if defined(_useTimer1)
void Timer1Service()
{
  handle_interrupts(_timer1, &TCNT1, &OCR1A);
}
#endif
#if defined(_useTimer3)
void Timer3Service()
{
  handle_interrupts(_timer3, &TCNT3, &OCR3A);
}
#endif
#endif


static void initISR(timer16_Sequence_t timer)
{
#if defined (_useTimer1)
  if(timer == _timer1) {
    TCCR1A = 0;             // normal counting mode
    TCCR1B = _BV(CS11);     // set prescaler of 8
    TCNT1 = 0;              // clear the timer count
#if defined(__AVR_ATmega8__)|| defined(__AVR_ATmega128__)
    TIFR |= _BV(OCF1A);      // clear any pending interrupts;
    TIMSK |=  _BV(OCIE1A) ;  // enable the output compare interrupt
#else
    // here if not ATmega8 or ATmega128
    TIFR1 |= _BV(OCF1A);     // clear any pending interrupts;
    TIMSK1 |=  _BV(OCIE1A) ; // enable the output compare interrupt
#endif
#if defined(WIRING)
    timerAttach(TIMER1OUTCOMPAREA_INT, Timer1Service);
#endif
  }
#endif

#if defined (_useTimer3)
  if(timer == _timer3) {
    TCCR3A = 0;             // normal counting mode
    TCCR3B = _BV(CS31);     // set prescaler of 8
    TCNT3 = 0;              // clear the timer count
#if defined(__AVR_ATmega128__)
    TIFR |= _BV(OCF3A);     // clear any pending interrupts;
	ETIMSK |= _BV(OCIE3A);  // enable the output compare interrupt
#else
    TIFR3 = _BV(OCF3A);     // clear any pending interrupts;
    TIMSK3 =  _BV(OCIE3A) ; // enable the output compare interrupt
#endif
#if defined(WIRING)
    timerAttach(TIMER3OUTCOMPAREA_INT, Timer3Service);  // for Wiring platform only
#endif
  }
#endif

#if defined (_useTimer4)
  if(timer == _timer4) {
    TCCR4A = 0;             // normal counting mode
    TCCR4B = _BV(CS41);     // set prescaler of 8
    TCNT4 = 0;              // clear the timer count
    TIFR4 = _BV(OCF4A);     // clear any pending interrupts;
    TIMSK4 =  _BV(OCIE4A) ; // enable the output compare interrupt
  }
#endif

#if defined (_useTimer5)
  if(timer == _timer5) {
    TCCR5A = 0;             // normal counting mode
    TCCR5B = _BV(CS51);     // set prescaler of 8
    TCNT5 = 0;              // clear the timer count
    TIFR5 = _BV(OCF5A);     // clear any pending interrupts;
    TIMSK5 =  _BV(OCIE5A) ; // enable the output compare interrupt
  }
#endif
}

static void finISR(timer16_Sequence_t timer)
{
    //disable use of the given timer
#if defined WIRING   // Wiring
  if(timer == _timer1) {
    #if defined(__AVR_ATmega1281__)||defined(__AVR_ATmega2561__)
    TIMSK1 &=  ~_BV(OCIE1A) ;  // disable timer 1 output compare interrupt
    #else
    TIMSK &=  ~_BV(OCIE1A) ;  // disable timer 1 output compare interrupt
    #endif
    timerDetach(TIMER1OUTCOMPAREA_INT);
  }
  else if(timer == _timer3) {
    #if defined(__AVR_ATmega1281__)||defined(__AVR_ATmega2561__)
    TIMSK3 &= ~_BV(OCIE3A);    // disable the timer3 output compare A interrupt
    #else
    ETIMSK &= ~_BV(OCIE3A);    // disable the timer3 output compare A interrupt
    #endif
    timerDetach(TIMER3OUTCOMPAREA_INT);
  }
#else
  //For arduino - in future: call here to a currently undefined function to reset the timer
  (void) timer;  // squash "unused parameter 'timer' [-Wunused-parameter]" warning
#endif
}

static boolean isTimerActive(timer16_Sequence_t timer)
{
  // returns true if any servo is active on this timer
  for(uint8_t channel=0; channel < SERVOS_PER_TIMER; channel++) {
    if(SERVO(timer,channel).Pin.isActive == true)
      return true;
  }
  return false;
}


/****************** end of static functions ******************************/

Servo::Servo()
{
  if( ServoCount < MAX_SERVOS) {
    this->servoIndex = ServoCount++;                    // assign a servo index to this instance
	servos[this->servoIndex].ticks = usToTicks(DEFAULT_PULSE_WIDTH);   // store default values  - 12 Aug 2009
  servos[this->servoIndex].ticks_cur = usToTicks(DEFAULT_PULSE_WIDTH)<<8;
  servos[this->servoIndex].ticks_goal = usToTicks(DEFAULT_PULSE_WIDTH)<<8;
  
  }
  else
    this->servoIndex = INVALID_SERVO ;  // too many servos
}

uint8_t Servo::attach(int pin)
{
  return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo::attach(int pin, int min, int max)
{
  return attach(pin, min, (max+min)/2, max);
}

uint8_t Servo::attach(int pin, int min, int mid, int max)
{  
  if(this->servoIndex < MAX_SERVOS ) {
    pinMode( pin, OUTPUT) ;                                   // set servo pin to output
    servos[this->servoIndex].Pin.nbr = pin;
    min_tick = usToTicks(min);
    mid_tick = usToTicks(mid);
    max_tick = usToTicks(max);

    servos[this->servoIndex].ticks_speed = 0;

    // todo min/max check: abs(min - MIN_PULSE_WIDTH) /4 < 128
    this->min  = (MIN_PULSE_WIDTH - min)/4; //resolution of min/max is 4 uS
    this->max  = (MAX_PULSE_WIDTH - max)/4;
    // initialize the timer if it has not already been initialized
    timer16_Sequence_t timer = SERVO_INDEX_TO_TIMER(servoIndex);
    if(isTimerActive(timer) == false)
      initISR(timer);
    servos[this->servoIndex].Pin.isActive = true;  // this must be set after the check for isTimerActive
  }
  return this->servoIndex ;  
}

void Servo::detach()
{
  servos[this->servoIndex].Pin.isActive = false;
  timer16_Sequence_t timer = SERVO_INDEX_TO_TIMER(servoIndex);
  if(isTimerActive(timer) == false) {
    finISR(timer);
  }
}

void Servo::write(int value)
{
  if(value < MIN_PULSE_WIDTH)
  {  // treat values less than 544 as angles in degrees (valid values in microseconds are handled as microseconds)
    if(value < 0) value = 0;
    if(value > 180) value = 180;
    value = map(value, 0, 180, SERVO_MIN(),  SERVO_MAX());
  }
  this->writeMicroseconds(value);
}

void Servo::writeMicroseconds(int value)
{
  // calculate and store the values for the given channel
  byte channel = this->servoIndex;
  if( (channel < MAX_SERVOS) )   // ensure channel is valid
  {
    if( value < SERVO_MIN() )          // ensure pulse width is valid
      value = SERVO_MIN();
    else if( value > SERVO_MAX() )
      value = SERVO_MAX();

    value = value - TRIM_DURATION;
    value = usToTicks(value);  // convert to ticks after compensating for interrupt overhead - 12 Aug 2009

    uint8_t oldSREG = SREG;
    cli();
    servos[channel].ticks_speed = 0;
    servos[channel].ticks = value;
    servos[channel].ticks_cur = (uint32_t)value<<8;
    servos[channel].ticks_goal = (uint32_t)value<<8;    
    SREG = oldSREG;
  }
}

void Servo::writeAngle(int value)
{
  int servo_out;


  value = constrain(value, -900, 900);
  
  byte channel = this->servoIndex;
  if( (channel < MAX_SERVOS) )   // ensure channel is valid
  {
    if (servos[channel].ticks_speed > 0)
    {
      servos[channel].ticks_goal = (uint32_t)getAngleToTick(value)<<8;
    }
    else
    {
      if (value > 0)
      {
        servo_out = map(value, 0, 900, mid_tick, max_tick);
      }
      else
      {
        servo_out = map(value, 0, -900, mid_tick, min_tick);
      }
      servos[this->servoIndex].ticks = servo_out;      
    }
  }   
}

void Servo::writeAngle(int value, uint8_t speed)
{
  value = constrain(value, -900, 900);

  // calculate and store the values for the given channel
  byte channel = this->servoIndex;
  if( (channel < MAX_SERVOS) )   // ensure channel is valid
  {
    servos[channel].ticks_goal = (uint32_t)getAngleToTick(value)<<8;

    if (speed == 0)
    {
      servos[channel].ticks_speed = 0xFFFF;
    }
    else
    {
      uint32_t angle_tick;

      angle_tick = getAngleToTick(1<<8) - getAngleToTick(0<<8);

      servos[channel].ticks_speed = speed * angle_tick;
    }
  }  
}

void Servo::writeSpeed(uint8_t speed)
{
  uint32_t angle_tick;

  byte channel = this->servoIndex;

  angle_tick = getAngleToTick(1<<8) - getAngleToTick(0<<8);
  servos[channel].ticks_speed = speed * angle_tick;
}

int Servo::readAngle(void)
{
  return getTickToAngle(servos[this->servoIndex].ticks);  
}
void Servo::wait(void)
{
  while(1)
  {
    if (servos[this->servoIndex].ticks_cur == servos[this->servoIndex].ticks_goal)
    {
      break;
    }
  }
}

int Servo::getAngleToTick(int value)
{
  int servo_out;


  value = constrain(value, -900, 900);
  if (value > 0)
  {
    servo_out = map(value, 0, 900, mid_tick, max_tick);
  }
  else
  {
    servo_out = map(value, 0, -900, mid_tick, min_tick);
  }

  return servo_out;
}

int Servo::getTickToAngle(int value)
{
  int ret;

  if (value >= mid_tick)
  {
    ret = map(value, mid_tick, max_tick, 0, 900);
  }
  else
  {
    ret = map(value, mid_tick, min_tick, 0, -900);
  }

  return ret;
}

int Servo::read() // return the value as degrees
{
  return  map( this->readMicroseconds()+1, SERVO_MIN(), SERVO_MAX(), 0, 180);
}

int Servo::readMicroseconds()
{
  unsigned int pulsewidth;
  if( this->servoIndex != INVALID_SERVO )
    pulsewidth = ticksToUs(servos[this->servoIndex].ticks)  + TRIM_DURATION ;   // 12 aug 2009
  else
    pulsewidth  = 0;

  return pulsewidth;
}

bool Servo::attached()
{
  return servos[this->servoIndex].Pin.isActive ;
}

#endif // ARDUINO_ARCH_AVR

