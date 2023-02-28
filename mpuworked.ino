


#include <Wire.h>

#define MPU_ADDRESS 0b1101000
// Gyroscope modes
#define pm250 0b00000
#define pm500 0b01000
#define pm1000 0b10000
#define pm2000 0b11000

#define gyroMode pm1000

#if gyroMode == pm2000
#define converter 131
#elif gyroMode == pm1000
#define converter 65.5
#elif gyroMode == pm500
#define converter 32.8
#else
#define converter 16.4
#endif

// needed Registers
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48
#define GYRO_CONFIG 0x1B

// Other Registers
#define MPU_POWER 0x6B
//Variables for Mpu Functions
float milliOld_mpu;
float milliNew_mpu;
float dt_mpu;
int flag = 0;

inline int16_t gyroZ_raw();
void MPU_reg_write(byte address, byte data);
float readYaw();
void rotate_90(float yaw1, char dir);


void setup() {

    Wire.begin();
    Serial.begin(9600);
  MPU_reg_write(MPU_POWER, 0x00); // MPU Reset
  MPU_reg_write(GYRO_CONFIG, gyroMode);
}
float oldread = 0;
int fl=0;

void loop()
{
  oldread=readYaw();
   
  if (oldread==readYaw()+0.01 || oldread==readYaw() ){  
   // oldread=readYaw();
    Serial.println("rrr");
  }
  else if (oldread==readYaw())
  {
    oldread=readYaw();
    Serial.println("Second if ");
      Serial.println(oldread);
  }
    else if (oldread!=readYaw())
  {
    
     Serial.println("change in read is ");
      Serial.println(oldread);
      oldread=readYaw();
  }
 //oldread=readYaw();
  //delay(500);
  // rotate_90( readYaw(), 'l' );

  
}
/*********************************Functions*******************************************/
inline int16_t gyroZ_raw()
{
  int16_t data = 0;
  Wire.beginTransmission(MPU_ADDRESS); // Orders MPU to communicate to send data
  Wire.write(GYRO_ZOUT_H);             // Sends Register Address to MPU, then the MPU responds.
  Wire.endTransmission(false);         // Doesn't send a stop bit at the end of the transmission

  Wire.requestFrom(MPU_ADDRESS, 2); // Request 2 bytes from MPU. GYRO_ZOUT_L
  // The MPU will send the data in GYRO_ZOUT_H then GYRO_ZOUT_L
  data = Wire.read() << 8 | Wire.read();

  return data;
}

void MPU_reg_write(byte address, byte data) // inline means the function is inserted at the call place when compiled,
{                                           //  and not sent to this function address as normal functions.
  Wire.beginTransmission(MPU_ADDRESS);      // Start communication with MPU6050 // MPU=0x68
  Wire.write(address);                      // Talk to the register 6B
  Wire.write(data);                         // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);               // end the transmission
}

float readYaw()
{ // gyroZ angle
  milliOld_mpu = milliNew_mpu;
  milliNew_mpu = millis();
  dt_mpu = (milliOld_mpu - milliNew_mpu) / 1000;
  float reading = (float)gyroZ_raw();
  static float yaw = 0;
  reading /= converter;
  yaw += reading * (dt_mpu);
  return yaw * 2;
}

void rotate_90(float yaw1, char dir)
{
  // start rotation in a certian direction
  if (dir == 'l')
  {
   //left();
    while (readYaw() <= yaw1 + 65)
    {
     // Serial.println(readYaw());
      Serial.println(yaw1);
    }
  }
  else if (dir == 'r')
  {
   // right();
    while (readYaw() >= yaw1 - 65)
    {
      
    }
  }
  // spins until the following condition is false
  //STOP(); // end rotation
}
