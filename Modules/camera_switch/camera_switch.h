/**
 ******************************************************************************
 * @file           camera_switch.h
 * @author         K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          Buttons functionality
 ******************************************************************************
 */


#ifndef CAMERA_SWITCH_CAMERA_SWITCH_H_
#define CAMERA_SWITCH_CAMERA_SWITCH_H_

#include <stm32h7xx_hal.h>


/**
 * @brief 
 * abcd to sczytywanie wartosci ze switcha
*/

struct cameraSwitch{
	uint8_t cameraNumber;
	GPIO_PinState channel_A;
	GPIO_PinState channel_B;
	GPIO_PinState channel_C;
	GPIO_PinState channel_D;
};

HAL_StatusTypeDef Check_Camera_State(struct cameraSwitch camSw);

void Read_Camera_Switch_Value(void);

void Send_Cameras_State(void);

void Set_Camera_LED(void);

#endif //CAMERA_SWITCH_CAMERA_SWITCH_H_
