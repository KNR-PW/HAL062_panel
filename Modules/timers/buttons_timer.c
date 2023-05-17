//#include <stm32h7xx_hal_tim.h>
#include "buttons_timer.h"
#include "error_handlers/error_handlers.h"

TIM_HandleTypeDef htim6;

void Buttons_Timer_Init(void){

	  TIM_MasterConfigTypeDef sMasterConfig = {0};

	  htim6.Instance = TIM6;
	  htim6.Init.Prescaler = 6399;
	  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim6.Init.Period = 999;
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

