/**
 ******************************************************************************
 * @file           buttons.c
 * @author         K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          Buttons functionality
 ******************************************************************************
 */

/* Includes --------------------------------------------------------- */
#include "buttons.h"
#include "buttons_const.h"
#include "LED_switch/LED_switch.h"
#include "LED_switch/LED_const.h"
#include <stm32h7xx_hal_gpio.h>
#include <stdbool.h>


static uint8_t buttonsState = 0x00; /*!< 8bit variable contains information of witch LED is set*/
extern I2C_HandleTypeDef hi2c1; /*!< extern - declared in LED_switch.c*/
ADC_HandleTypeDef hadc1; /*!< ADC converter - handler to struct*/
double val = 0.0625; /*!< Upper value for joystick raw values devider. Default: 1/16 (max raw value 1600 scaled to 100)*/
int on, off = 0;

/**
 * @brief Initializes GPIO ports and inputs for buttons
 */
void Buttons_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStruct = {0};

	  __HAL_RCC_GPIOD_CLK_ENABLE();
	  __HAL_RCC_GPIOG_CLK_ENABLE();
	  __HAL_RCC_GPIOE_CLK_ENABLE();
	  __HAL_RCC_GPIOB_CLK_ENABLE();
	  __HAL_RCC_GPIOC_CLK_ENABLE();
	  __HAL_RCC_GPIOF_CLK_ENABLE();

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

	  /*Configure GPIO pins : PF6 - 12V switch 1*/
	  GPIO_InitStruct.Pin = GPIO_PIN_6;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);


	  /* Sets intterupt priority to the highest*/
	  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**
 * @brief Sets LED lights state depending on bistable button
 * @details
 * This functionality is used to indicate on the panel, 
 * whether bistable button is pressed. \n
 * The function uses bit masks to read button state and sets proper 
 * I2C signal (according to I2C module documentation) which at the end 
 * is being written by the interrupt. \n
 * There are basic bit operation to save led state on correct position. 
 * First there are AND operatation with mask on correct position, then 
 * OR operation with 1/0 (SET/RESET) shifted to position that we already check.
 */

void Set_LED_For_Bistable(void){
/*doxygen declaration *//** @verbatim */

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

 /** @endverbatim *//*end of doxygen declaration*/

	HAL_I2C_Mem_Write_IT(&hi2c1, DEV_2, PORT_B, 1, &buttonsState, 1);
}

/**
 * @brief Initializes ADC module to read value from potentiometer.
 */
void ADC1_Init(void)
{

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};


  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_16B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }


  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }


  sConfig.Channel = ADC_CHANNEL_18;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSignedSaturation = DISABLE;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }


}

/**
 * @brief Reads value from potentiometer.
 * @details 
 * This function will read value from potentiometer and prepered it to use.
 * Potentiometer is used to change range of joystick. In order to do that
 * value from potentiometer should be scaled to: \n
 * 0.0625 - joystick has full range (from -100 to 100)
 * 0.0125 - joystick has minimum range (from -20 to 20)
 */
void ADC_Try_Read(void){

	GPIO_PinState shouldReadPot = HAL_GPIO_ReadPin(BI_BUTTON_RED_1_GPIO_Port, BI_BUTTON_RED_1_Pin);
	if(shouldReadPot == GPIO_PIN_SET){
	HAL_ADC_Start(&hadc1);

	uint32_t temp_val = HAL_ADC_GetValue(&hadc1);
		double temp_val_double = (double)temp_val;

		if( temp_val <= 0 ){
		val = 1;
		}
		else{
		val = temp_val_double/(880000); /* scaling potetiometr raw value*/
		}
		HAL_ADC_Stop(&hadc1);
		}
	else
	{
		val = 0.0625;
	}
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(MONO_BUTTON_GREEN_1_Pin);
  HAL_GPIO_EXTI_IRQHandler(MONO_BUTTON_BLUE_1_Pin);
  HAL_GPIO_EXTI_IRQHandler(MONO_BUTTON_RED_2_Pin);
  HAL_GPIO_EXTI_IRQHandler(MONO_BUTTON_BLACK_2_Pin);
  HAL_GPIO_EXTI_IRQHandler(MONO_BUTTON_GREEN_2_Pin);
  HAL_GPIO_EXTI_IRQHandler(MONO_BUTTON_BLUE_2_Pin);

}
/**
  * @brief This function handles EXTI line[4] interrupts.
  */
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

/**
  * @brief This function handles EXTI callback for every pin
  * @details 
  * 	-MONO_BUTTON_BLACK_1_Pin - restart button, 
  * 	pushing causes going to inf loop, that fire watchdog \n
  * 
  * 	-MONO_BUTTON_JOY_BLUE_Pin - after pressing, message to mainboard is send
  * 	to make manipulator come to home position
  * 
  * @see uart frame documentation
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == MONO_BUTTON_BLACK_1_Pin){
		while(1){}
	}
	else if(GPIO_Pin == MONO_BUTTON_JOY_BLUE_Pin){
		uint8_t msgData[16] = {0x49, 0x78, 0x78, 0x78, 0x78,0x78,0x78,0x78,0x78,
				0x78,0x78,0x78,0x78,0x78,0x78,0x78};
		uint8_t msgID[2] = {'3', '0'};
	}
	if(GPIO_Pin == MONO_BUTTON_JOY_RED_Pin){
		off = 1;
		LED_Set(LIGHT1, 0);
	}
	if(GPIO_Pin == MONO_BUTTON_JOY_BLUE_Pin){
		on = 1;
		LED_Set(LIGHT1, 1);
	}

//		Eth_Send_Massage(msgID, msgData);
}
