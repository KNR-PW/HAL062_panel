/**
 ******************************************************************************
 * @file           : main.c
 * @author         : K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          : Rover's operational body main program body
 * version 1.0
 ******************************************************************************
 * @details
 * 			This is the main program body of HAL-062 rover's operational panel.
 * 			The panel constitutes of several switches, potentiometers, lights and
 * 			3 joysticks. The system uses communications standards like I2C, CAN, UART,
 * 			and bluetooth and ethernet modules, which are supposed to engage communication
 * 			with the rover.
 *
 * 			Project consists of files specifying operation of each module, which
 * 			are located in /Modules folder.

 *			Used communication standards and modules:
 *				- I2C
 *				- UART
 *				- bluetooth
 *				- ethernet - W7500S2E-R1
 *
 *			Used modules
 *				- MAX11616EEE+ GPIO expander
 *				- MCP23017 GPIO expander
 *
 ******************************************************************************
 *
 */
/* Includes ------------------------------------------ */

#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_gpio.h>
#include <stdbool.h>
#include "error_handlers/error_handlers.h"
#include "ethernet/ethernet.h"
#include "LED_switch/LED_switch.h"
#include "LED_switch/LED_const.h"
#include "joystick/joystick.h"
#include "timers/joystick_timer.h"
#include "buttons/buttons.h"
#include "timers/buttons_timer.h"
#include "watchdogs/watchdog.h"

void SystemClock_Config(void);
extern bool joyInitFinished;

extern IWDG_HandleTypeDef hiwdg1;

int main(void) {

	//system init
	HAL_Init();
	SystemClock_Config();

	//modules init
	Joystick_I2C_Init();
	Buttons_Init();
	Eth_Init();
	LED_Init();
	Joystick_Timer_Init();
	Buttons_Timer_Init();

	//starting functionality
	Joystick_Write_Conditions();
	Eth_Receive_Massage();
  
	//watchdog init
	MX_IWDG1_Init();
//	LED_Set(LIGHT1, 1);
	while (1)
	{
		// refreshing watchdog to prevent reset
		HAL_IWDG_Refresh(&hiwdg1);

	}
}

// configuration of clock
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Supply configuration update enable
	 */
	HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

	/** Configure the main internal regulator output voltage
	 */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

	while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {
	}

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1
			| RCC_CLOCKTYPE_D1PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
	RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
		Error_Handler();
	}
}

