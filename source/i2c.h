#ifndef __I2C_H__
#define __I2C_H__

#include "MKL25Z4.h"


void initI2C();
void disableAck();
void enableAck();
void repeatedStart();
void enterRxMode();
void startI2C();
void stopI2C();
void waitI2C();
void writeI2C(uint8_t data);
void writeRegisterI2C(uint8_t SlaveAddress,uint8_t RegisterAddress, uint8_t data);
void writeMultRegisterI2C(uint8_t SlaveAddress,uint8_t RegisterAddress, uint8_t n_data, uint8_t *data);

uint8_t readI2C();
uint8_t readRegisterI2C(uint8_t SlaveAddress,uint8_t RegisterAddress);
void i2c_read_setup(uint8_t SlaveAddress, uint8_t address);
uint8_t i2c_repeated_read(uint8_t isLastRead);

void delay(uint32_t t);

#endif // !__I2C_H__