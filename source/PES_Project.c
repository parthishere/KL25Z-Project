/*
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    PES_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "oled.h"
#include "fonts.h"
#include "mpu6050.h"


int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    BOARD_InitDebugConsole();
#endif

    PRINTF("Hello World\r\n");

    init_SSD1306();

//    gotoXY_SSD1306 (0,0);
//	puts_SSD1306 ("HELLO", &Font_11x18, 1);

    init_MPU();
    MPU_gyroConfig(MPU_Gyro_Range_250);
    MPU_accelConfig(MPU_Accel_Range_8G);

    MPU_calibrate();

    while(1) {
        PRINTF("X: %d Y : %d Z: %d Gyro X %d Y %d Z %d Tempreture %d  \n\r", MPU_accelXraw(), MPU_accelYraw(), MPU_accelZraw(), MPU_gyroXraw(), MPU_gyroYraw(), MPU_gyroZraw(), MPU_tempC());
    }
    return 0 ;
}
