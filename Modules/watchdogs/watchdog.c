/**
 ******************************************************************************
 * @file           watchdog.c
 * @author         K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          Watchdog header file
 ******************************************************************************
 */

/* Includes -----------------------------------*/
#include "error_handlers/error_handlers.h"
#include "watchdog.h"


IWDG_HandleTypeDef hiwdg1; /*!< Watchdog handler*/

/**
 * @brief Initializing watchdog module.
 * @details
 *        Watchdog is being refreshed provided that the system is not halted for
 *        more than 3 seconds. Failing this condition causes system reset.
 * @param void
 * @returns void
*/
void MX_IWDG1_Init(void)
{
  hiwdg1.Instance = IWDG1;
  hiwdg1.Init.Prescaler = IWDG_PRESCALER_32;
  hiwdg1.Init.Window = 4095;
  hiwdg1.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg1) != HAL_OK)
  {
    Error_Handler();
  }

}
