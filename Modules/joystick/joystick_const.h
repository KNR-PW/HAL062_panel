/**
 ******************************************************************************
 * @file           joystick_const.h
 * @author         K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          Joystick variables structure and ADC converter configuration variables
 * @see 		   MAX.... module configuration in datasheet
 ******************************************************************************
 */

#ifndef JOYSTICK_JOYSTICK_CONST_H
#define JOYSTICK_JOYSTICK_CONST_H

#include <stm32h7xx_hal.h>

#define SLAVE_ADDRESS 0x35 /*!< I2C slave address for ADC converter*/
#define CONFIG_DATA 0x17 /*!< Configuration data for ADC converter */
#define SETUP_DATA 0x82 /*!< Setup data for ADC converter*/

struct Joystick{
	uint8_t number; /*!< Joystick number (1, 2 or 3)*/
	uint16_t xVal; /*!< Read joystick raw value in x axis*/
	uint16_t yVal; /*!< Read joystick raw value in y axis*/
	uint16_t zVal; /*!< Read joystick raw value in z axis*/
	uint16_t midVal; /*!< Reference value used to set middle value based on raw value*/
	int16_t xPos; /*!< Calculated final x value, which is sent in frame*/
	int16_t yPos; /*!< Calculated final y value, which is sent in frame*/
	int16_t zPos; /*!< Calculated final z value, which is sent in frame*/

};

#endif // JOYSTICK_JOYSTICK_CONST_H
