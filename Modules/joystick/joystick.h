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

union F2I{
	uint32_t uint;
	float f;
};

struct manipData{
	uint8_t byte0;
	uint8_t byte1;
	uint8_t byte2;
	uint8_t byte3;
};

void Joystick_I2C_Init(void);

void Joystick_Write_Conditions(void);

void Joystick_Read_Value_Start(void);

void Joystick_Send_Readings(void);

void Jostick_Read_Value(void);

#endif // JOYSTICK_JOYSTICK_H

