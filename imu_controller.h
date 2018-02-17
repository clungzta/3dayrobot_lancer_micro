#include <Wire.h>

#define    MPU9250_ADDRESS            0x68
#define    MAG_ADDRESS                0x0C

#define    GYRO_FULL_SCALE_250_DPS    0x00  
#define    GYRO_FULL_SCALE_500_DPS    0x08
#define    GYRO_FULL_SCALE_1000_DPS   0x10
#define    GYRO_FULL_SCALE_2000_DPS   0x18

#define    ACC_FULL_SCALE_2_G        0x00  
#define    ACC_FULL_SCALE_4_G        0x08
#define    ACC_FULL_SCALE_8_G        0x10
#define    ACC_FULL_SCALE_16_G       0x18

class ImuController
{
	public:
		ImuController();
		void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data);
		void RunLoop();

	private:
		// Accelerometer
		int16_t ax;
		int16_t ay;
		int16_t az;

	  // Gyroscope
	  	int16_t gx;
	  	int16_t gy;
		int16_t gz;

		int16_t mx;
		int16_t my;
		int16_t mz;
	  

}

ImuController::ImuController(uint8_t debug)
{
	this->debug = debug;
	if (debug) {
    	Serial.print(CLASS_NAME);
    	Serial.println(": initialised");
    }
}

ImuController::setup()
{
	Wire.begin();
 	Serial.begin(115200);

  // Configure gyroscope range
  I2CwriteByte(MPU9250_ADDRESS,27,GYRO_FULL_SCALE_2000_DPS);
  // Configure accelerometers range
  I2CwriteByte(MPU9250_ADDRESS,28,ACC_FULL_SCALE_16_G);
  // Set by pass mode for the magnetometers
  I2CwriteByte(MPU9250_ADDRESS,0x37,0x02);
  
  // Request first magnetometer single measurement
   I2CwriteByte(MAG_ADDRESS,0x0A,0x01);
}

ImuController::I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
	  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();
  
  // Read Nbytes
  Wire.requestFrom(Address, Nbytes); 
  uint8_t index=0;
  while (Wire.available())
    Data[index++]=Wire.read();
}

ImuController::loop()
{
	// Read Accelerometer and Gyro 
	uint8_t Buf[14];
	I2Cread(MPU9250_ADDRESS,0x3B,14,Buf);
	 // Create 16 bits values from 8 bits data

	// Accelerometer
	ax=-(Buf[0]<<8 | Buf[1]);
	ay=-(Buf[2]<<8 | Buf[3]);
	az=Buf[4]<<8 | Buf[5];

	// Gyroscope
	gx=-(Buf[8]<<8 | Buf[9]);
	gy=-(Buf[10]<<8 | Buf[11]);
	gz=Buf[12]<<8 | Buf[13];
	// Read register Status 1 and wait for the DRDY: Data Ready

	uint8_t ST1;
	do
	{
		ImuController::I2Cread(MAG_ADDRESS,0x02,1,&ST1);
	}
	while (!(ST1&0x01));

	// Read magnetometer data  
	uint8_t Mag[7];  
	ImuController::I2Cread(MAG_ADDRESS,0x03,7,Mag);


	// Create 16 bits values from 8 bits data

	// Magnetometer
	mx=-(Mag[3]<<8 | Mag[2]);
	my=-(Mag[1]<<8 | Mag[0]);
	mz=-(Mag[5]<<8 | Mag[4]);
	mx +=200;
	my -=70;
	mz -= 700;



}