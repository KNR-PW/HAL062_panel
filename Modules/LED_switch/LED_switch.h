/**
 ******************************************************************************
 * @file           LED_switch.h
 * @author         K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          LED lights handling - headers file
 ******************************************************************************
 */

#ifndef LED_SWITCH_LED_SWITCH_H_
#define LED_SWITCH_LED_SWITCH_H_

#include <stm32h7xx_hal.h>


void LED_Init(void);

void LED_Set(uint32_t lightCode, uint8_t state);

#endif //LED_SWITCH_LED_SWITCH_H
