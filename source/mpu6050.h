#ifndef __MPU6050_H__
#define __MPU6050_H__
#include <stdint.h>
#include "i2c.h"
#include "fsl_debug_console.h"

typedef enum
{
    Gyro_Range_250 = 0b00000,
    Gyro_Range_500 = 0b01000,
    Gyro_Range_1000 = 0b10000,
    Gyro_Range_2000 = 0b11000
} GyroRange_t;

typedef enum
{
    Accel_Range_2G  = 0b00000,
    Accel_Range_4G  = 0b01000,
    Accel_Range_8G  = 0b10000,
    Accel_Range_16G = 0b11000
} AccelRange_t;

void init_MPU();
void gyroConfig(GyroRange_t range);
void accelConfig(AccelRange_t range);
uint8_t calibrate_MPU(void);


float accX(void);
float accY(void);
float accZ(void);

float tempC(void);

float gyroX(void);
float gyroY(void);
float gyroZ(void);

int16_t gyroXraw(void);
int16_t gyroYraw(void);
int16_t gyroZraw(void);

int16_t accelXraw(void);
int16_t accelYraw(void);
int16_t accelZraw(void);

float get_accelRange(void);
float get_gyroRange(void);

void delay_MPU(uint32_t t);

void read_full_xyz(int16_t * x,int16_t * y, int16_t * z);

#endif // !__MPU6050_H__