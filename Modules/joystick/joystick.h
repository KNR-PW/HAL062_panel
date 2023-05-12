/**
 ******************************************************************************
 * @file           : joystick.h
 * @author         : K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          : Joystick module - header file
 ******************************************************************************
 */

#ifndef JOYSTICK_JOYSTICK_H_
#define JOYSTICK_JOYSTICK_H_

#include <stm32h7xx_hal.h>


// @brief Initializing I2C module: GPIO
void I2C_Init(void);

// @brief configuring ADC inverter
void I2C_Write_Conditions(void);

// @brief setting flag - start receive joysticks value
void I2C_Read_Joystick_Value_Start(void);

#endif // JOYSTICK_JOYSTICK_H


