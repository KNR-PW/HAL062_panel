/**
 ******************************************************************************
 * @file           buttons_timer.c
 * @author         K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          Timer used mainly for buttons - initialization function
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------- */
#include "buttons_timer.h"
#include "error_handlers/error_handlers.h"

TIM_HandleTypeDef htim6; /*!< Timer TIM6 handler */

/**
 * @brief Buttons_Timer_Init() initializes timer TIM6, which is used for handling
 * 		  interrupts for buttons. The timer is set to cause an interrupt every 100ms.
 */
void Buttons_Timer_Init(void){

	  TIM_MasterConfigTypeDef sMasterConfig = {0};

	  htim6.Instance = TIM6;
	  htim6.Init.Prescaler = 6399;
	  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim6.Init.Period = 99;
	  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  HAL_TIM_Base_Start_IT(&htim6);
}

