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
#include "gpio.h"


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

    init_MPU();
    gyroConfig(Gyro_Range_250);
    accelConfig(Accel_Range_8G);

    calibrate_MPU();
    
    init_GPIO_External_IRQ();

    int16_t x, y, z;
    char buffer[50];

    while(1) {
        read_full_xyz_calibrated(&x, &y, &z);
        PRINTF("X: %04d Y : %04d Z: %04d  | Gyro X %04d Y %04d Z %0d Tempreture %04d  \n\r", x, y, z, gyroXraw(), gyroYraw(), gyroZraw(), tempC());
        // IMPLEMENT DELAY
        snprintf(buffer, sizeof(buffer), "%04d, %04d, %04d", x, y, z);
        gotoXY_SSD1306 (0,5);
        puts_SSD1306 (buffer, &Font_7x10, SSD1306_COLOR_WHITE);
        updateScreen_SSD1306();
    }
    return 0 ;
}
