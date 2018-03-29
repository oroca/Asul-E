#include "asule.h"



const signed char orientationMatrix[9] = {
   0,  1,  0,
  -1,  0,  0,
   0,  0,  1
};






Asule::Asule()
{
  is_imu_init = false;
}

Asule::~Asule()
{

}

err_t Asule::begin(void)
{
  err_t err = 0;

  err |= beginImu();
  err |= beginServo();

  return err;
}

err_t Asule::beginImu(void)
{
  inv_error_t  err_code;


  if (imu.begin() != INV_SUCCESS)
  {
    return ERR_BEGIN_IMU;
  }  

  err_code = imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | // Enable 6-axis quat
                          DMP_FEATURE_SEND_RAW_ACCEL |
                          DMP_FEATURE_SEND_CAL_GYRO  |
                          DMP_FEATURE_GYRO_CAL, // Use gyro calibration
                          100); // Set DMP FIFO rate to 10 Hz                                                
  imu.dmpSetOrientation(orientationMatrix);
  if (err_code == 0)
  {
    is_imu_init = true;
  }      
}

err_t Asule::beginServo(void)
{
  err_t err = 0;
  uint16_t i;


  servo[0].attach(_PIN_SERVO_1, 530, 1490, 2500);  
  servo[1].attach(_PIN_SERVO_2, 480, 1371, 2381);  
  servo[2].attach(_PIN_SERVO_3, 480, 1371, 2381);  
  servo[3].attach(_PIN_SERVO_4, 480, 1371, 2381);  
  servo[4].attach(_PIN_SERVO_5, 480, 1371, 2381);  
  servo[5].attach(_PIN_SERVO_6, 480, 1371, 2381);  
  servo[6].attach(_PIN_SERVO_7, 480, 1371, 2381);  
  servo[7].attach(_PIN_SERVO_8, 480, 1371, 2381);  


  for (i=0; i<_PIN_SERVO_MAX; i++)
  {  
    servo[i].writeAngle(0, 0);
  }

  return err;
}

err_t Asule::update(void)
{
  err_t err = 0;


  if (is_imu_init == true)
  {
    err |= updateImu();
  }

  return err;
}

err_t Asule::updateImu(void)
{
  err_t err = 0;


  // Check for new data in the FIFO
  if ( imu.fifoAvailable() )
  {
    // Use dmpUpdateFifo to update the ax, gx, mx, etc. values
    if ( imu.dmpUpdateFifo() == INV_SUCCESS)
    {
      // computeEulerAngles can be used -- after updating the
      // quaternion values -- to estimate roll, pitch, and yaw
      imu.computeEulerAngles();
      if ( imu.updateCompass() == INV_SUCCESS )
      {
        imu.computeCompassHeading(); 
      }
    }
  }  

  return err;
}













Asule asule;


