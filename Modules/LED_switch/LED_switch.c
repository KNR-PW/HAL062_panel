/**
 ******************************************************************************
 * @file           : LED_switch.c
 * @author         : K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          : Switch do LEDs - initialization, function to set/reset LED
 ******************************************************************************
 */

//INCLUDATION:
#include "LED_switch.h"
#include "error_handlers/error_handlers.h"
#include "LED_const.h"
#include <stdbool.h>

//VARIABLES DEFINITIONS:
I2C_HandleTypeDef hi2c1;
static struct currentLEDstate currentState;
uint8_t pinNum = 0x00;
uint8_t devAddr;
uint8_t memAddr;

uint8_t boundryLed = 0;

bool i2cLedLineOpen = false;

//FUNCTIONS DEFINITIONS:
void LED_Init(void) {

	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x10707DBC;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE)
			!= HAL_OK) {
		Error_Handler();
	}

	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
		Error_Handler();
	}

	uint8_t configAsOutput = 0x00;
	uint8_t config = 0x3C;

	HAL_Delay(200);
	HAL_I2C_Mem_Write_IT(&hi2c1, DEV_1, 0x0A, 1, (uint8_t*) &config, 1);
	HAL_Delay(200);
	HAL_I2C_Mem_Write_IT(&hi2c1, DEV_1, 0x0B, 1, (uint8_t*) &config, 1);
	HAL_Delay(200);
	HAL_I2C_Mem_Write_IT(&hi2c1, DEV_1, 0x00, 1, (uint8_t*) &configAsOutput, 1);
	HAL_Delay(200);
	HAL_I2C_Mem_Write_IT(&hi2c1, DEV_1, 0x01, 1, (uint8_t*) &configAsOutput, 1);
	HAL_Delay(200);
	HAL_I2C_Mem_Write_IT(&hi2c1, DEV_2, 0x0A, 1, (uint8_t*) &config, 1);
	HAL_Delay(200);
	HAL_I2C_Mem_Write_IT(&hi2c1, DEV_2, 0x0B, 1, (uint8_t*) &config, 1);
	HAL_Delay(200);
	HAL_I2C_Mem_Write_IT(&hi2c1, DEV_2, 0x00, 1, (uint8_t*) &configAsOutput, 1);
	HAL_Delay(200);
	HAL_I2C_Mem_Write_IT(&hi2c1, DEV_2, 0x01, 1, (uint8_t*) &configAsOutput, 1);
	HAL_Delay(200);
}

void LED_Set(uint32_t lightCode, uint8_t state) {

	uint8_t devAddr = (lightCode & 0xFF0000) >> 16;
	uint8_t memAddr = (lightCode & 0x00FF00) >> 8;

	if (state != 0U) {
		if (devAddr == DEV_1 && memAddr == PORT_A) {
			pinNum = (lightCode & 0x0000FF) | currentState.dev1portA;
			currentState.dev1portA = pinNum;
		} else if (devAddr == DEV_1 && memAddr == PORT_B) {
			pinNum = (lightCode & 0x0000FF) | currentState.dev1portB;
			currentState.dev1portB = pinNum;
		} else if (devAddr == DEV_2 && memAddr == PORT_A) {
			pinNum = (lightCode & 0x0000FF) | currentState.dev2portA;
			currentState.dev2portA = pinNum;
		} else if (devAddr == DEV_2 && memAddr == PORT_B) {
			pinNum = (lightCode & 0x0000FF) | currentState.dev2portB;
			currentState.dev2portB = pinNum;
		}
	} else {
		if (devAddr == DEV_1 && memAddr == PORT_A) {
			pinNum = ~(lightCode & 0x0000FF) & currentState.dev1portA;
			currentState.dev1portA = pinNum;
		} else if (devAddr == DEV_1 && memAddr == PORT_B) {
			pinNum = ~(lightCode & 0x0000FF) & currentState.dev1portB;
			currentState.dev1portB = pinNum;
		} else if (devAddr == DEV_2 && memAddr == PORT_A) {
			pinNum = ~(lightCode & 0x0000FF) & currentState.dev2portA;
			currentState.dev2portA = pinNum;
		} else if (devAddr == DEV_2 && memAddr == PORT_B) {
			pinNum = ~(lightCode & 0x0000FF) & currentState.dev2portB;
			currentState.dev2portB = pinNum;
		}
	}
	HAL_I2C_Mem_Write_IT(&hi2c1, devAddr, memAddr, 1, &pinNum, 1);

}

