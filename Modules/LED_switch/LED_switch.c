
#include "LED_switch.h"
#include "error_handlers/error_handlers.h"

I2C_HandleTypeDef hi2c1;

typedef enum LEDS{
	LIGHT25 = 0x1901,
	LIGHT26 = 0x1902,
	LIGHT27 = 0x1904,
	LIGHT28 = 0x1908,
	LIGHT29 = 0x1910,
	LIGHT30 = 0x1920,
	LIGHT31 = 0x1940,
	LIGHT32 = 0x1980,

	LIGHT24 = 0x0901,
	LIGHT23 = 0x0902,
	LIGHT22 = 0x0904,
	LIGHT21 = 0x0908,
	LIGHT20 = 0x0910,
	LIGHT19 = 0x0920,
	LIGHT18 = 0x0940,
	LIGHT17 = 0x0980,
} addressLED;

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

}

/*
 * BANK = 1
 * GPIOB7 : 0x1980
 * GPIOB6 : 0x1940
 * GPIOB5 : 0x1920
 * GPIOB4 : 0x1910
 * GPIOB3 : 0x1908
 * GPIOB2 : 0x1904
 * GPIOB1 : 0x1902
 * GPIOB0 : 0x1901
 *
 * GPIOA7 : 0x0980
 * GPIOA6 : 0x0940
 * GPIOA5 : 0x0920
 * GPIOA4 : 0x0910
 * GPIOA3 : 0x0908
 * GPIOA2 : 0x0904
 * GPIOA1 : 0x0902
 * GPIOA0 : 0x0901
 */

uint16_t getAddressLED(addressLED LED){
	switch(LED){
		case LIGHT25: return 0x1901;
		case LIGHT26: return 0x1902;
		case LIGHT27: return 0x1904;
		case LIGHT28: return 0x1908;
		case LIGHT29: return 0x1910;
		case LIGHT30: return 0x1920;
		case LIGHT31: return 0x1940;
		case LIGHT32: return 0x1980;

		case LIGHT24: return 0x0901;
		case LIGHT23: return 0x0902;
		case LIGHT22: return 0x0904;
		case LIGHT21: return 0x0908;
		case LIGHT20: return 0x0910;
		case LIGHT19: return 0x0920;
		case LIGHT18: return 0x0940;
		case LIGHT17: return 0x0980;
		default: return 0x0000;
	}
}


void setLED(addressLED LED, uint8_t setState){
	uint16_t address = getAddressLED(LED);
	if(setState == 1){
		// nie mom pojecia jak sie zapala leda xddd
		uint8_t setOn = 0x08;
		HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x19, 1, (uint8_t*)&setOn, 1, HAL_MAX_DELAY);;
	}
	else{
		uint8_t setOff = 0x0000;
		HAL_I2C_Mem_Write(&hi2c1, 0x42, address, 1, (uint8_t*)&setOff, 8, HAL_MAX_DELAY);
	}
}

void dupa(void){
	uint8_t setOn = 0x08;
	HAL_I2C_Mem_Write_IT(&hi2c1, 0x42, 0x19, 1, (uint8_t*)&setOn, 1);
}

