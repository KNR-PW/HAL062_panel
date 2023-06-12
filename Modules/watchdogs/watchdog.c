
#include "error_handlers/error_handlers.h"
#include "watchdog.h"


IWDG_HandleTypeDef hiwdg1;

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
