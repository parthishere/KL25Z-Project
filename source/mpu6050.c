#include "mpu6050.h"

#define MPU_ADDR_DEFAULT 0x68
#define MPU_ADDR_SCND 0x69

#define MPU_ACCEL_X_REG 0x3B
#define MPU_ACCEL_Y_REG 0x3D
#define MPU_ACCEL_Z_REG 0x3F
#define MPU_GYRO_X_REG 0x43
#define MPU_GYRO_Y_REG 0x45
#define MPU_GYRO_Z_REG 0x47
#define MPU_TEMP_REG 0x41
#define MPU_GYRO_CONFIG_REG 0x1B
#define MPU_ACCEL_CONFIG_REG 0x1C
#define MPU_TEMP_CONFIG_REG

#define NUM_CALIBRATIONS 1000

#define MPU_SEND_COMMAND(register, command) writeRegisterI2C(MPU_ADDR_DEFAULT, (register), (command))
#define MPU_READ(register) readRegisterI2C(MPU_ADDR_DEFAULT, (register))


static GyroRange_t gyroRange;
static AccelRange_t accelRange;

static int MPU_accel_offset[3];
static int MPU_gyro_offset[3];


void init_MPU()
{   
    initI2C();
    MPU_SEND_COMMAND(0x6B, 0x00);
    delay_MPU(1000);
    return;
}

/**
 * Configure the range of the gyro
 * @param range gyro range
 */
void gyroConfig(GyroRange_t range) {
	gyroRange = range;
	MPU_SEND_COMMAND(0x1B, gyroRange);
	delay_MPU(2);
}

/**
 * Configure the Accelerometer range
 * @param range accelerometer range
 */
void accelConfig(AccelRange_t range) {
	accelRange = range;
	MPU_SEND_COMMAND(0x1C, accelRange);
	delay_MPU(2);
}


uint8_t calibrate_MPU(void) {
	int i;
	int16_t temp_x, temp_y, temp_z;

	for (int j = 0; j < 3; j++) {
		MPU_accel_offset[j] = 0;
		MPU_gyro_offset[j] = 0;
	}
	PRINTF("MPU 6050 Calibration");
	for (i = 0; i < NUM_CALIBRATIONS; i++) {
		if (i % 50 == 0) {
			PRINTF(".");
		}
		read_full_xyz(&temp_x, &temp_y, &temp_z);

		MPU_accel_offset[0] += temp_x;
		MPU_accel_offset[1] += temp_y;
		MPU_accel_offset[2] += temp_z;
		MPU_gyro_offset[0] += gyroXraw();
		MPU_gyro_offset[1] += gyroYraw();
		MPU_gyro_offset[2] += gyroZraw();
	}
	PRINTF("\r\n");
	for (int j = 0; j < 3; j++) {
		MPU_accel_offset[j] /= NUM_CALIBRATIONS;
		MPU_gyro_offset[j] /= NUM_CALIBRATIONS;
	}
	PRINTF("Offset: %d | %d | %d \r\n", MPU_gyro_offset[0], MPU_gyro_offset[1], MPU_gyro_offset[2]);
	PRINTF("Accel Offset: %d | %d | %d \r\n", MPU_accel_offset[0], MPU_accel_offset[1], MPU_accel_offset[2]);
	delay_MPU(2000);
	return 1;
}

void read_full_xyz(int16_t * x,int16_t * y, int16_t * z)
{
	int i;
		
	int16_t temp[3];
	int8_t data[6];
	
	startI2C();
	i2c_read_setup(MPU_ADDR_DEFAULT , 0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
	
	// Read five bytes in repeated mode
	for( i=0; i<5; i++)	{
		data[i] = (int8_t)i2c_repeated_read(0);
	}
	// Read last byte ending repeated mode
	data[i] = (int8_t)i2c_repeated_read(1);
	
	for ( i=0; i<3; i++ ) {
		temp[i] = (int16_t) ((data[2*i]<<8) | data[2*i+1]);
	}

	// Align for 14 bits
	*x = temp[0]/4;
	*y = temp[1]/4;
	*z = temp[2]/4;
}

void read_full_xyz_calibrated(int16_t * x,int16_t * y, int16_t * z)
{
	int i;
		
	int16_t temp[3];
	int8_t data[6];
	
	startI2C();
	i2c_read_setup(MPU_ADDR_DEFAULT , 0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
	
	// Read five bytes in repeated mode
	for( i=0; i<5; i++)	{
		data[i] = (int8_t)i2c_repeated_read(0);
	}
	// Read last byte ending repeated mode
	data[i] = (int8_t)i2c_repeated_read(1);
	
	for ( i=0; i<3; i++ ) {
		temp[i] = (int16_t) ((data[2*i]<<8) | data[2*i+1]);
	}

	// Align for 14 bits
	*x = temp[0]/4;
	*y = temp[1]/4;
	*z = temp[2]/4;

	*x -= (MPU_accel_offset[0]);
	*y = *y - (MPU_accel_offset[1]);
	*z = *z - (MPU_accel_offset[2]);
}


/**
 * Read Raw values of the Accelerometer on the X-axis
 */
int16_t accelXraw(void) {
	int16_t accelX;
	accelX = (int8_t)MPU_READ(MPU_ACCEL_X_REG);
	delay_MPU(2000);
	return accelX;
}
/**
 * Read Raw values of the Accelerometer on the Y-axis
 */
int16_t accelYraw(void) {
	int16_t accelY;
	accelY = (int8_t)MPU_READ(MPU_ACCEL_Y_REG);
	delay_MPU(2000);
	return accelY;
}
/**
 * Read Raw values of the Accelerometer on the Z-axis
 */
int16_t accelZraw(void) {
	int16_t accelZ;
	accelZ = (int8_t)MPU_READ(MPU_ACCEL_Z_REG);
	delay_MPU(2000);
	return accelZ;
}
/**
 * Read Raw values of the Gyroscope on the X-axis
 */
int16_t gyroXraw(void) {
	int16_t gyroX;
	gyroX = (int8_t)MPU_READ(MPU_GYRO_X_REG);
	delay_MPU(2000);
	return gyroX;
}
/**
 * Read Raw values of the Gyroscope on the Y-axis
 */
int16_t gyroYraw(void) {
	int16_t gyroY;
	gyroY = (int8_t)MPU_READ(MPU_GYRO_Y_REG);
	delay_MPU(2000);
	return gyroY;
}
/**
 * Read Raw values of the Gyroscope on the Z-axis
 */
int16_t gyroZraw(void) {
	int16_t gyroZ;
	gyroZ = (int8_t)MPU_READ(MPU_GYRO_Z_REG);
	delay_MPU(2000);
	return gyroZ;
}


/**
 * Read the calculated Accelerometer value on X-axis
 */
float accX(void) {
	int16_t accelX;
	accelX = (int8_t)MPU_READ(MPU_ACCEL_X_REG);
	accelX -= MPU_accel_offset[0];
	float accelX_calc;
	accelX_calc = accelX / (get_accelRange());
	delay_MPU(1000);
	return accelX_calc;
}
/**
 * Read the calculated Accelerometer value on Y-axis
 */
float accY(void) {
	int16_t accelY;
	accelY = (int8_t)MPU_READ(MPU_ACCEL_Y_REG);
	float accelY_calc;
	accelY_calc = accelY / (get_accelRange());
	delay_MPU(1000);
	return accelY_calc;
}
/**
 * Read the calculated Accelerometer value on Z-axis
 */
float accZ(void) {
	int16_t accelZ;
	accelZ = (int8_t)MPU_READ(MPU_ACCEL_Z_REG);
	float accelZ_calc;
	accelZ_calc = accelZ / (get_accelRange());
	delay_MPU(1000);
	return accelZ_calc;
}
/**
 * Read the Temperature sensor in degrees Celcius
 */
float tempC(void) {
	int16_t temp;
	temp = (int8_t)MPU_READ(MPU_TEMP_REG);
	float tempC;
	tempC = (temp / 340) + 36.53;
	delay_MPU(1000);
	return temp;
}



/**
 * Get the Accelerometer range value
 */
float get_accelRange(void) {
	switch (accelRange) {
	case Accel_Range_2G:
		return 16384.0f;
	case Accel_Range_4G:
		return 8192.0f;
	case Accel_Range_8G:
		return 4096.0f;
	case Accel_Range_16G:
		return 2048.0f;
	default:
		return 2048.0f;
	}
}
/**
 * Get the Gyroscope range value
 */
float get_gyroRange(void) {
	switch (gyroRange) {
	case Gyro_Range_250:
		return 131;
	case Gyro_Range_500:
		return 65.5;
	case Gyro_Range_1000:
		return 32.8;
	case Gyro_Range_2000:
		return 16.4;
	default:
		return 16.4;
	}
}

/**
 * Read calculated Gyroscope values on X-axis
 */
float gyroX(void) {
	int16_t gyroX;
	gyroX = (int8_t)MPU_READ(MPU_GYRO_X_REG);
	gyroX -= MPU_gyro_offset[0];
	float gyroX_calc;
	gyroX_calc = gyroX / (get_gyroRange());
	delay_MPU(2);
	return gyroX_calc;
}
/**
 * Read calculated Gyroscope values on Y-axis
 */
float gyroY(void) {
	int16_t gyroY;
	gyroY = (int8_t)MPU_READ(MPU_GYRO_Y_REG);
	gyroY -= MPU_gyro_offset[1];
	float gyroY_calc;
	gyroY_calc = gyroY / (get_gyroRange());
	delay_MPU(2);
	return gyroY_calc;
}
/**
 * Read calculated Gyroscope values on Z-axis
 */
float gyroZ(void) {
	int16_t gyroZ;
	gyroZ = MPU_READ(MPU_GYRO_Z_REG);
	gyroZ -= MPU_gyro_offset[2];
	float gyroZ_calc;
	gyroZ_calc = gyroZ / (get_gyroRange());
	delay_MPU(2);
	return gyroZ_calc;
}

void delay_MPU(uint32_t t)
{
	uint32_t cnt = 0;
	for(cnt=0; cnt<t; cnt++)
	{
		__asm("nop");
	};
}
