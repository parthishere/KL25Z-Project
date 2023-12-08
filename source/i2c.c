#include "i2c.h"


#define ICR 0x00
#define MULT 0x04

#define I2C_WRITE	0	// Master write
#define I2C_READ	1	// Master read

void initI2C(void){
    SIM->SCGC4 |= SIM_SCGC4_I2C0(1);
    SIM->SCGC5 |= SIM_SCGC5_PORTB(1); // FOR I2C

    SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL(1) | SIM_SOPT2_TPMSRC(1);
    SIM->SCGC6 |= SIM_SCGC6_TPM0(1);

    PORTB->PCR[0] = PORT_PCR_MUX(0x2);    		//Set PTB0 to mux 2 [I2C_SCL]
    PORTB->PCR[1] = PORT_PCR_MUX(0x2);    		//Set PTB1 to mux 2 [I2C_SDA]

    I2C0->F = I2C_F_ICR(ICR) | I2C_F_MULT(MULT); // Set I2C divider to 48 and multiplier to 4


    I2C0->C1 |= I2C_C1_IICEN(1); // Enable the I2C module 
}


void disableAck()
{
	I2C0->C1 |= I2C_C1_TXAK_MASK;
}

void enableAck()
{
	I2C0->C1 &= ~I2C_C1_TXAK_MASK;
}

void repeatedStart()
{
	I2C0->C1 |= I2C_C1_RSTA_MASK;
}

void enterRxMode()
{
	I2C0->C1 &= ~I2C_C1_TX_MASK;
}

void startI2C()
{
	I2C0->C1 |= I2C_C1_TX_MASK;
	I2C0->C1 |= I2C_C1_MST_MASK;
}

void stopI2C()
{
	I2C0->C1 &= ~I2C_C1_MST_MASK;
	I2C0->C1 &= ~I2C_C1_TX_MASK;
}

void waitI2C()
{
	uint32_t i = 0;
	i = 1000000;

	while (((I2C0->S & I2C_S_RXAK_MASK) == I2C_S_RXAK_MASK) &
              (i != 0))
	{
		i--;
	}

	if (i != 0)
    {
        i = 4000000UL;
        while (((I2C0->S & I2C_S_TCF_MASK) == 0) &
                        (i != 0))
        {
            /* wait until transmit is in progress or timeout occurs */
            i--;
        }
    }
     
    
	//I2C0->S |= I2C_S_IICIF_MASK;
}

void writeI2C(uint8_t data)
{
	I2C0->D = (uint8_t)data;
}

uint8_t readI2C()
{
	return (uint8_t)( I2C0->D );
}

void writeRegisterI2C(uint8_t SlaveAddress,uint8_t RegisterAddress, uint8_t data)
{
	startI2C();
	writeI2C(((SlaveAddress << 1) | I2C_WRITE));
	waitI2C();

	writeI2C(RegisterAddress);
	waitI2C();

	writeI2C(data);
	waitI2C();

	stopI2C();

	delay(50);
}


uint8_t readRegisterI2C(uint8_t SlaveAddress,uint8_t RegisterAddress)
{
	uint8_t res = 0;

	startI2C();
	writeI2C(((SlaveAddress << 1) | I2C_WRITE));
	waitI2C();

	writeI2C(RegisterAddress);
	waitI2C();

	repeatedStart();

	writeI2C(((SlaveAddress << 1) | I2C_READ));
	waitI2C();

	enterRxMode();
	disableAck();

	res = readI2C();
	waitI2C();
	stopI2C();
	res = readI2C();
	delay(50);

	return res;
}

void writeMultRegisterI2C(uint8_t SlaveAddress,uint8_t RegisterAddress, uint8_t n_data, uint8_t *data)
{
	uint8_t i = 0;

	startI2C();
	writeI2C(((SlaveAddress << 1) | I2C_WRITE));
	waitI2C();

	writeI2C(RegisterAddress);
	waitI2C();

	for(i=0;i<n_data;i++)
	{
		writeI2C(*data);
		waitI2C();
		data++;
	}
	stopI2C();

	delay(50);
}

void delay(uint32_t t)
{
	uint32_t cnt = 0;
	for(cnt=0; cnt<t; cnt++)
	{
		__asm("nop");
	};
}
