#ifndef __GPIO_H__
#define __GPIO_H__
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

void init_GPIO_External_IRQ();
uint32_t return_distance();
uint32_t clear_distance();

#endif // !__GPIO_H__