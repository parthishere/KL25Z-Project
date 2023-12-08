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


static MPU_GyroRange gyroRange;
static MPU_AccelRange accelRange;

int MPU_accel_offset[3];
int MPU_gyro_offset[3];


void init_MPU()
{   
    initI2C();
    MPU_SEND_COMMAND(0x6B, 0x00);
    delay_mpu(1000);
    return;
}

/**
 * Configure the range of the gyro
 * @param range gyro range
 */
void MPU_gyroConfig(MPU_GyroRange range) {
	gyroRange = range;
	MPU_SEND_COMMAND(0x1B, gyroRange);
	delay_mpu(2);
}

/**
 * Configure the Accelerometer range
 * @param range accelerometer range
 */
void MPU_accelConfig(MPU_AccelRange range) {
	accelRange = range;
	MPU_SEND_COMMAND(0x1C, accelRange);
	delay_mpu(2);
}


uint8_t MPU_calibrate(void) {
	int i;
	for (int j = 0; j < 3; j++) {
		MPU_accel_offset[j] = 0;
		MPU_gyro_offset[j] = 0;
	}
	PRINTF("MPU 6050 Calibration");
	for (i = 0; i < NUM_CALIBRATIONS; i++) {
		if (i % 50 == 0) {
			PRINTF(".");
		}
		MPU_accel_offset[0] += MPU_accelXraw();
		MPU_accel_offset[1] += MPU_accelYraw();
		MPU_accel_offset[2] += MPU_accelZraw();
		MPU_gyro_offset[0] += MPU_gyroXraw();
		MPU_gyro_offset[1] += MPU_gyroYraw();
		MPU_gyro_offset[2] += MPU_gyroZraw();
	}
	PRINTF("\r\n");
	for (int j = 0; j < 3; j++) {
		MPU_accel_offset[j] /= NUM_CALIBRATIONS;
		MPU_gyro_offset[j] /= NUM_CALIBRATIONS;
	}
	PRINTF("Offset: %d | %d | %d \r\n", MPU_gyro_offset[0], MPU_gyro_offset[1], MPU_gyro_offset[2]);
	delay_mpu(2000);
	return 1;
}

/**
 * Read Raw values of the Accelerometer on the X-axis
 */
int16_t MPU_accelXraw(void) {
	int16_t accelX;
	accelX = (int8_t)MPU_READ(MPU_ACCEL_X_REG);
	delay_mpu(2000);
	return accelX;
}
/**
 * Read Raw values of the Accelerometer on the Y-axis
 */
int16_t MPU_accelYraw(void) {
	int16_t accelY;
	accelY = (int8_t)MPU_READ(MPU_ACCEL_Y_REG);
	delay_mpu(2000);
	return accelY;
}
/**
 * Read Raw values of the Accelerometer on the Z-axis
 */
int16_t MPU_accelZraw(void) {
	int16_t accelZ;
	accelZ = (int8_t)MPU_READ(MPU_ACCEL_Z_REG);
	delay_mpu(2000);
	return accelZ;
}
/**
 * Read Raw values of the Gyroscope on the X-axis
 */
int16_t MPU_gyroXraw(void) {
	int16_t gyroX;
	gyroX = (int8_t)MPU_READ(MPU_GYRO_X_REG);
	delay_mpu(2000);
	return gyroX;
}
/**
 * Read Raw values of the Gyroscope on the Y-axis
 */
int16_t MPU_gyroYraw(void) {
	int16_t gyroY;
	gyroY = (int8_t)MPU_READ(MPU_GYRO_Y_REG);
	delay_mpu(2000);
	return gyroY;
}
/**
 * Read Raw values of the Gyroscope on the Z-axis
 */
int16_t MPU_gyroZraw(void) {
	int16_t gyroZ;
	gyroZ = (int8_t)MPU_READ(MPU_GYRO_Z_REG);
	delay_mpu(2000);
	return gyroZ;
}


/**
 * Read the calculated Accelerometer value on X-axis
 */
float MPU_accelX(void) {
	int16_t accelX;
	accelX = (int8_t)MPU_READ(MPU_ACCEL_X_REG);
	accelX -= MPU_accel_offset[0];
	float accelX_calc;
	accelX_calc = accelX / (MPU_get_accelRange());
	delay_mpu(1000);
	return accelX_calc;
}
/**
 * Read the calculated Accelerometer value on Y-axis
 */
float MPU_accelY(void) {
	int16_t accelY;
	accelY = (int8_t)MPU_READ(MPU_ACCEL_Y_REG);
	float accelY_calc;
	accelY_calc = accelY / (MPU_get_accelRange());
	delay_mpu(1000);
	return accelY_calc;
}
/**
 * Read the calculated Accelerometer value on Z-axis
 */
float MPU_accelZ(void) {
	int16_t accelZ;
	accelZ = (int8_t)MPU_READ(MPU_ACCEL_Z_REG);
	float accelZ_calc;
	accelZ_calc = accelZ / (MPU_get_accelRange());
	delay_mpu(1000);
	return accelZ_calc;
}
/**
 * Read the Temperature sensor in degrees Celcius
 */
float MPU_tempC(void) {
	int16_t temp;
	temp = (int8_t)MPU_READ(MPU_TEMP_REG);
	float tempC;
	tempC = (temp / 340) + 36.53;
	delay_mpu(1000);
	return temp;
}



/**
 * Get the Accelerometer range value
 */
float MPU_get_accelRange(void) {
	switch (accelRange) {
	case MPU_Accel_Range_2G:
		return 16384.0f;
	case MPU_Accel_Range_4G:
		return 8192.0f;
	case MPU_Accel_Range_8G:
		return 4096.0f;
	case MPU_Accel_Range_16G:
		return 2048.0f;
	default:
		return 2048.0f;
	}
}
/**
 * Get the Gyroscope range value
 */
float MPU_get_gyroRange(void) {
	switch (gyroRange) {
	case MPU_Gyro_Range_250:
		return 131;
	case MPU_Gyro_Range_500:
		return 65.5;
	case MPU_Gyro_Range_1000:
		return 32.8;
	case MPU_Gyro_Range_2000:
		return 16.4;
	default:
		return 16.4;
	}
}

/**
 * Read calculated Gyroscope values on X-axis
 */
float MPU_gyroX(void) {
	int16_t gyroX;
	gyroX = (int8_t)MPU_READ(MPU_GYRO_X_REG);
	gyroX -= MPU_gyro_offset[0];
	float gyroX_calc;
	gyroX_calc = gyroX / (MPU_get_gyroRange());
	delay_mpu(2);
	return gyroX_calc;
}
/**
 * Read calculated Gyroscope values on Y-axis
 */
float MPU_gyroY(void) {
	int16_t gyroY;
	gyroY = (int8_t)MPU_READ(MPU_GYRO_Y_REG);
	gyroY -= MPU_gyro_offset[1];
	float gyroY_calc;
	gyroY_calc = gyroY / (MPU_get_gyroRange());
	delay_mpu(2);
	return gyroY_calc;
}
/**
 * Read calculated Gyroscope values on Z-axis
 */
float MPU_gyroZ(void) {
	int16_t gyroZ;
	gyroZ = MPU_READ(MPU_GYRO_Z_REG);
	gyroZ -= MPU_gyro_offset[2];
	float gyroZ_calc;
	gyroZ_calc = gyroZ / (MPU_get_gyroRange());
	delay_mpu(2);
	return gyroZ_calc;
}

void delay_mpu(uint32_t t)
{
	uint32_t cnt = 0;
	for(cnt=0; cnt<t; cnt++)
	{
		__asm("nop");
	};
}
