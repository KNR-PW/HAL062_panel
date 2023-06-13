#include "buttons.h"
#include "buttons_const.h"
#include "LED_switch/LED_switch.h"
#include "LED_switch/LED_const.h"
#include <stm32h7xx_hal_gpio.h>

static uint8_t buttonsState = 0x00;
extern I2C_HandleTypeDef hi2c1;

void Buttons_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStruct = {0};

	  __HAL_RCC_GPIOD_CLK_ENABLE();
	  __HAL_RCC_GPIOG_CLK_ENABLE();
	  __HAL_RCC_GPIOE_CLK_ENABLE();
	  __HAL_RCC_GPIOB_CLK_ENABLE();
	  __HAL_RCC_GPIOC_CLK_ENABLE();

	  /*Configure GPIO pins : BI_BUTTON_GREEN_3_Pin BI_BUTTON_GREEN_1_Pin BI_BUTTON_GREEN_2_Pin */
	  GPIO_InitStruct.Pin = BI_BUTTON_GREEN_3_Pin|BI_BUTTON_GREEN_1_Pin|BI_BUTTON_GREEN_2_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	  /*Configure GPIO pins :  BI_BUTTON_RED_1_Pin BI_BUTTON_RED_2_Pin BI_BUTTON_BLUE_1_Pin
	                           BI_BUTTON_BLUE_2_Pin BI_BUTTON_BLUE_3_Pin */
	  GPIO_InitStruct.Pin = BI_BUTTON_RED_1_Pin|BI_BUTTON_RED_2_Pin|BI_BUTTON_BLUE_1_Pin
	                         |BI_BUTTON_BLUE_2_Pin|BI_BUTTON_BLUE_3_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


	  /*Configure GPIO pins : MONO_BUTTON_JOY_RED_Pin MONO_BUTTON_JOY_BLUE_Pin MONO_BUTTON_JOY_GREEN_Pin MONO_BUTTON_RED_1_Pin */
	  GPIO_InitStruct.Pin = MONO_BUTTON_JOY_RED_Pin|MONO_BUTTON_JOY_BLUE_Pin|MONO_BUTTON_JOY_GREEN_Pin|MONO_BUTTON_RED_1_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	  /*Configure GPIO pins : MONO_BUTTON_BLACK_1_Pin MONO_BUTTON_GREEN_1_Pin MONO_BUTTON_BLUE_1_Pin MONO_BUTTON_RED_2_Pin
	                           MONO_BUTTON_BLACK_2_Pin MONO_BUTTON_GREEN_2_Pin MONO_BUTTON_BLUE_2_Pin */
	  GPIO_InitStruct.Pin = MONO_BUTTON_BLACK_1_Pin|MONO_BUTTON_GREEN_1_Pin|MONO_BUTTON_BLUE_1_Pin|MONO_BUTTON_RED_2_Pin
	                          |MONO_BUTTON_BLACK_2_Pin|MONO_BUTTON_GREEN_2_Pin|MONO_BUTTON_BLUE_2_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	  //cameras switches:

	  /*Configure GPIO pin : CAM_SWITCH_3_A_Pin */
	  GPIO_InitStruct.Pin = CAM_SWITCH_3_A_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	  HAL_GPIO_Init(CAM_SWITCH_3_A_GPIO_Port, &GPIO_InitStruct);

	  /*Configure GPIO pins : CAM_SWITCH_3_B_Pin CAM_SWITCH_3_C_Pin */
	  GPIO_InitStruct.Pin = CAM_SWITCH_3_B_Pin|CAM_SWITCH_3_C_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	  /*Configure GPIO pins : CAM_SWITCH_3_D_Pin CAM_SWITCH_2_A_Pin CAM_SWITCH_2_B_Pin CAM_SWITCH_2_C_Pin
	                           CAM_SWITCH_2_D_Pin CAM_SWITCH_1_A_Pin CAM_SWITCH_1_B_Pin */
	  GPIO_InitStruct.Pin = CAM_SWITCH_3_D_Pin|CAM_SWITCH_2_A_Pin|CAM_SWITCH_2_B_Pin|CAM_SWITCH_2_C_Pin
	                          |CAM_SWITCH_2_D_Pin|CAM_SWITCH_1_A_Pin|CAM_SWITCH_1_B_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	  /*Configure GPIO pins : CAM_SWITCH_1_C_Pin CAM_SWITCH_1_D_Pin */
	  GPIO_InitStruct.Pin = CAM_SWITCH_1_C_Pin|CAM_SWITCH_1_D_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}


void Set_LED_For_Bistable(void){

	uint8_t temp;
	temp = HAL_GPIO_ReadPin(BI_BUTTON_RED_1_GPIO_Port, BI_BUTTON_RED_1_Pin);
	buttonsState = (buttonsState & 0b01111111) | (temp<<7);

	temp = HAL_GPIO_ReadPin(BI_BUTTON_RED_2_GPIO_Port, BI_BUTTON_RED_2_Pin);
	buttonsState = (buttonsState & 0b10111111) |  (temp<<6);

	temp = HAL_GPIO_ReadPin(BI_BUTTON_BLUE_1_GPIO_Port, BI_BUTTON_BLUE_1_Pin);
	buttonsState = (buttonsState & 0b11011111) |(temp<<5);

	temp = HAL_GPIO_ReadPin(BI_BUTTON_BLUE_2_GPIO_Port, BI_BUTTON_BLUE_2_Pin);
	buttonsState = (buttonsState & 0b11101111) |(temp<<4);

	temp = HAL_GPIO_ReadPin(BI_BUTTON_BLUE_3_GPIO_Port, BI_BUTTON_BLUE_3_Pin);
	buttonsState = (buttonsState & 0b11110111) |(temp<<3);

	temp = HAL_GPIO_ReadPin(BI_BUTTON_GREEN_1_GPIO_Port, BI_BUTTON_GREEN_1_Pin);
	buttonsState = (buttonsState & 0b11111011) | (temp<<2);

	temp = HAL_GPIO_ReadPin(BI_BUTTON_GREEN_2_GPIO_Port, BI_BUTTON_GREEN_2_Pin);
	buttonsState = (buttonsState & 0b11111101) | (temp<<1);

	temp = HAL_GPIO_ReadPin(BI_BUTTON_GREEN_3_GPIO_Port, BI_BUTTON_GREEN_3_Pin);
	buttonsState = (buttonsState & 0b11111110) | temp;

	HAL_I2C_Mem_Write_IT(&hi2c1, DEV_2, PORT_B, 1, &buttonsState, 1);
//	LED_Set(LIGHT1, 1);


}

void EXTI15_10_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(MONO_BUTTON_GREEN_1_Pin);
  HAL_GPIO_EXTI_IRQHandler(MONO_BUTTON_BLUE_1_Pin);
  HAL_GPIO_EXTI_IRQHandler(MONO_BUTTON_RED_2_Pin);
  HAL_GPIO_EXTI_IRQHandler(MONO_BUTTON_BLACK_2_Pin);
  HAL_GPIO_EXTI_IRQHandler(MONO_BUTTON_GREEN_2_Pin);
  HAL_GPIO_EXTI_IRQHandler(MONO_BUTTON_BLUE_2_Pin);

}

void EXTI4_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(MONO_BUTTON_JOY_RED_Pin);
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(MONO_BUTTON_JOY_BLUE_Pin);
  HAL_GPIO_EXTI_IRQHandler(MONO_BUTTON_JOY_GREEN_Pin);
  HAL_GPIO_EXTI_IRQHandler(MONO_BUTTON_RED_1_Pin);
  HAL_GPIO_EXTI_IRQHandler(MONO_BUTTON_BLACK_1_Pin);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
//	if(GPIO_Pin == MONO_BUTTON_JOY_RED_Pin){
//		LED_Set(LIGHT1, 1);
//	}
}
