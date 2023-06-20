/**
 ******************************************************************************
 * @file           joystick_timer.c
 * @author         K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          Timer for joystick interrupt - file for initialization function
 ******************************************************************************
 */

/* Includes ----------------------------------------------------------------- */
#include "joystick_timer.h"
#include "joystick/joystick.h"
#include "stdbool.h"
#include "joystick/joystick_const.h"

TIM_HandleTypeDef htim7; /*!< Timer TIM7 handler*/

//10ms

/**
 * @brief Joystick_Timer_Init() initializes timer TIM7, which is used for handling
 * 		  interrupts for joystick (it checks their readings every interrupt).
 * 		  The timer is set to cause an interrupt every 10ms.
 */
void Joystick_Timer_Init(void) {

	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 6400-1;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = 1000-1;
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim7) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	HAL_TIM_Base_Start_IT(&htim7);

}
