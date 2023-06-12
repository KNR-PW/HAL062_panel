/**
 ******************************************************************************
 * @file           : LED_switch.h
 * @author         : K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          : Switch do LEDs - headers file
 ******************************************************************************
 */

#ifndef LED_SWITCH_LED_SWITCH_H_
#define LED_SWITCH_LED_SWITCH_H_

#include <stm32h7xx_hal.h>


// @brief Initializing I2C module: GPIO
// @param None
// @returns void
void LED_Init(void);

// @brief Setting/Reseting LED
// @param lightCode, predefined values "LIGHTn"
// @param state, 1 - set LED, other - reset LED
// @returns void
void LED_Set(uint32_t lightCode, uint8_t state);

#endif //LED_SWITCH_LED_SWITCH_H
