
#include "LED_switch.h"
#include "error_handlers/error_handlers.h"
#include "LED_const.h"

I2C_HandleTypeDef hi2c1;
static uint8_t setOn = 0xFF;
static struct currentLEDstate currentState;

void LED_Init(void)
{

	  hi2c1.Instance = I2C1;
	  hi2c1.Init.Timing = 0x10707DBC;
	  hi2c1.Init.OwnAddress1 = 0;
	  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	  hi2c1.Init.OwnAddress2 = 0;
	  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  /** Configure Analogue filter
	  */
	  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  /** Configure Digital filter
	  */
	  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  /* USER CODE BEGIN I2C1_Init 2 */

	  /* USER CODE END I2C1_Init 2 */
		uint8_t configAsOutput = 0x00;
		uint8_t config = 0x3C;
		HAL_StatusTypeDef s1;
		uint8_t data = 19;
		HAL_I2C_Mem_Read(&hi2c1, 0x40, 0x00,1, &data, 1, 1000);


		HAL_Delay(200);
		s1 = HAL_I2C_Mem_Write_IT(&hi2c1, 0x40, 0x0A, 1, (uint8_t*)&config, 1);
		HAL_Delay(200);
		s1 = HAL_I2C_Mem_Write_IT(&hi2c1, 0x40, 0x0B, 1, (uint8_t*)&config, 1);
		HAL_Delay(200);
		s1 = HAL_I2C_Mem_Write_IT(&hi2c1, 0x40, 0x00, 1, (uint8_t*)&configAsOutput, 1);
		HAL_Delay(200);
		s1 = HAL_I2C_Mem_Write_IT(&hi2c1, 0x40, 0x01, 1, (uint8_t*)&configAsOutput, 1);
		HAL_Delay(200);
		HAL_I2C_Mem_Read(&hi2c1, 0x40, 0x0A,1, &data, 1, 1000);
}



void setLED(uint32_t lightCode, uint8_t state){

	uint8_t devAddr = (lightCode & 0xFF0000)>>16;
	uint8_t memAddr = (lightCode & 0x00FF00)>>8;
	uint8_t pinNum = 0x00;

	if(state == 1){
		if(devAddr == 0x40 && memAddr == 0x12){
			uint8_t pinNum = (lightCode & 0x0000FF) | currentState.dev40portA;
			currentState.dev40portA = pinNum;
		}
		else if(devAddr == 0x40 && memAddr == 0x13){
			uint8_t pinNum = (lightCode & 0x0000FF) | currentState.dev40portB;
			currentState.dev40portB = pinNum;
				}
		else if(devAddr == 0x42 && memAddr == 0x12){
			uint8_t pinNum = (lightCode & 0x0000FF) | currentState.dev42portA;
			currentState.dev42portA = pinNum;
				}
		else if(devAddr == 0x42 && memAddr == 0x13){
			uint8_t pinNum = (lightCode & 0x0000FF) | currentState.dev42portB;
			currentState.dev42portB = pinNum;
				}
	}
	else{
		if(devAddr == 0x40 && memAddr == 0x12){
			uint8_t pinNum = ~(lightCode & 0x0000FF) & currentState.dev40portA;
			currentState.dev40portA = pinNum;
		}
		else if(devAddr == 0x40 && memAddr == 0x13){
			uint8_t pinNum = ~(lightCode & 0x0000FF) & currentState.dev40portB;
			currentState.dev40portB = pinNum;
				}
		else if(devAddr == 0x42 && memAddr == 0x12){
			uint8_t pinNum = ~(lightCode & 0x0000FF) & currentState.dev42portA;
			currentState.dev42portA = pinNum;
				}
		else if(devAddr == 0x42 && memAddr == 0x13){
			uint8_t pinNum = ~(lightCode & 0x0000FF) & currentState.dev42portB;
			currentState.dev42portB = pinNum;
				}
	}
	if(HAL_I2C_Mem_Write_IT(&hi2c1, devAddr, memAddr, 1, &pinNum, 1)!=HAL_OK)
		{
		Error_Handler();
		}
}

