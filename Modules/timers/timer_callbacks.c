/**
 ******************************************************************************
 * @file           timer_callbacks.c
 * @author         K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          File containing functions handling timer interrupts
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------ */
#include <stdbool.h>
#include "joystick/joystick_const.h"
#include "camera_switch/camera_switch.h"
#include "joystick/joystick.h"
#include "buttons/buttons.h"
#include "LED_switch/LED_switch.h"
#include "ethernet/ethernet.h"
#include "buttons/buttons_const.h"

extern bool receiveIsReady; /*!< */
extern struct cameraSwitch yellowCamera; /*! cameraSwitch structure represents yellow camera screen*/
extern struct cameraSwitch redCamera; /*! cameraSwitch structure represents red camera screen*/
extern struct cameraSwitch blueCamera; /*! cameraSwitch structure represents blue camera screen*/


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM7) {
		if (receiveIsReady) {
		Jostick_Read_value_Done();
		}
	}
	if(htim->Instance == TIM6) {
		Set_LED_For_Bistable();
		Read_Camera_Switch_Value();
		Send_Cameras_State();

		ADC_Try_Read();

	}
}
