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
void Joystick_I2C_Init(void);

// @brief configuring ADC inverter
void Joystick_Write_Conditions(void);

// @brief setting flag - start receive joysticks value
void Joystick_Read_Value_Start(void);

// @brief sending do rover joystick values
void Joystick_Send_Readings(void);

#endif // JOYSTICK_JOYSTICK_H

