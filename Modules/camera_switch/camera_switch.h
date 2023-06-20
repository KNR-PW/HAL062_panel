/**
 ******************************************************************************
 * @file           camera_switch.h
 * @author         K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          Switches to cameras functionality
 ******************************************************************************
 */


#ifndef CAMERA_SWITCH_CAMERA_SWITCH_H_
#define CAMERA_SWITCH_CAMERA_SWITCH_H_

/* Includes --------------------------------------------------------- */
#include <stm32h7xx_hal.h>


/**
 * @brief 
 * Struct representing single camera screen with switch to changing image (one screen handles 3 cam).
 * 
 * @details
 * A,B,C,D channels are simply read pin state connected with 4-position switch.
 * It allows to contain information with camera image is currently shown on screen.
 * cameraNumber - number of screen that struct is connected to
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
