#ifndef __MPU6050_H__
#define __MPU6050_H__
#include <stdint.h>
#include "i2c.h"
#include "fsl_debug_console.h"

typedef enum
{
    MPU_Gyro_Range_250 = 0x00,
    MPU_Gyro_Range_500 = (1 << 3),
    MPU_Gyro_Range_1000 = (1 << 4),
    MPU_Gyro_Range_2000 = ((1 << 4) | (1 << 3))
} MPU_GyroRange;

typedef enum
{
    MPU_Accel_Range_2G = 0x00,
    MPU_Accel_Range_4G = (1 << 3),
    MPU_Accel_Range_8G = (1 << 4),
    MPU_Accel_Range_16G = ((1 << 4) | (1 << 3))
} MPU_AccelRange;

void init_MPU();
void MPU_gyroConfig(MPU_GyroRange range);
void MPU_accelConfig(MPU_AccelRange range);
uint8_t MPU_calibrate(void);


float MPU_accelX(void);
float MPU_accelY(void);
float MPU_accelZ(void);

float MPU_tempC(void);

float MPU_gyroX(void);
float MPU_gyroY(void);
float MPU_gyroZ(void);

int16_t MPU_gyroXraw(void);
int16_t MPU_gyroYraw(void);
int16_t MPU_gyroZraw(void);

int16_t MPU_accelXraw(void);
int16_t MPU_accelYraw(void);
int16_t MPU_accelZraw(void);

float MPU_get_accelRange(void);
float MPU_get_gyroRange(void);

void delay_mpu(uint32_t t);

#endif // !__MPU6050_H__