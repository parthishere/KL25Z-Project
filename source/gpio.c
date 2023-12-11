#include "gpio.h"

// Defines the switch position.
#define HALL_SENSOR_POS 6
// Defines a macro to create a mask for a given bit.
#define MASK(X) (uint32_t)(1 << X)

#define THRITYTWO_BIT 0xffffffff

volatile uint32_t count = 0;
volatile uint32_t distance = 0;

#define R 10 //cm
#define PERIMETER 2*3.14*R // cm


void init_GPIO_External_IRQ(){
	// Enable the clock for the PORTD module.
	SIM->SCGC5 |= SIM_SCGC5_PORTD(1);

	// Configure the switch pin as an input with an interrupt on rising edge.
	PORTD->PCR[HALL_SENSOR_POS] |= PORT_PCR_MUX(1) | PORT_PCR_IRQC(0B1001) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK;

	PTD->PDDR &= ~MASK(HALL_SENSOR_POS);
	
	// Set the priority of the PORTD interrupt.
	NVIC_SetPriority(PORTD_IRQn, 4);

	// Clear the pending PORTD interrupt.
	NVIC_ClearPendingIRQ(PORTD_IRQn);

	// Enable the PORTD interrupt.
	NVIC_EnableIRQ(PORTD_IRQn);
}


void PORTD_IRQHandler(){
	// Clear the pending PORTD interrupt.
	NVIC_ClearPendingIRQ(PORTD_IRQn);

	// If the switch is pressed, transition to the emergency state.
	if ((PORTD->ISFR & MASK(HALL_SENSOR_POS))) {
		PRINTF("Got rising edge!");
		count++; // one revolution complete
		distance += PERIMETER;
	}

	// Clear the PORTD interrupt status flag.
	PORTD->ISFR = THRITYTWO_BIT;
}


uint32_t return_distance(){
	return (distance);
}

uint32_t clear_distance(){
	count = 0;
	distance = 0;
}

