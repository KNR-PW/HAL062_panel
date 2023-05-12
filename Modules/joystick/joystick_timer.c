#include "joystick_timer.h"
#include "joystick.h"
#include "stdbool.h"
#include "joystick_const.h"

TIM_HandleTypeDef htim7;
//extern I2C_HandleTypeDef hi2c2;


void Joystick_Timer_Init(void) {

	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 6400-1;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = 10000-1;
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
