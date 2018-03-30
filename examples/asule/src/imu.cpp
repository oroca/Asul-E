#include "imu.h"
#include "./driver/mpu9250/MPU9250-DMP.h"


static const signed char orientationMatrix[9] = {
   0,  1,  0,
  -1,  0,  0,
   0,  0,  1
};



static MPU9250_DMP imu;



Imu::Imu()
{
  is_init = false;
}

Imu::~Imu()
{

}

err_t Imu::begin(void)
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
    is_init = true;
  } 

  return 0; 
}

bool Imu::isInit(void)
{
  return is_init;  
}

err_t Imu::update(void)
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

float Imu::roll()
{
  return (float)imu.roll;
}

float Imu::pitch()
{
  return (float)imu.pitch;
}

float Imu::yaw()
{
  return (float)imu.yaw;
}

float Imu::heading()
{
  return (float)imu.heading;
}


