#include "stm32h7xx_hal.h"
#include "stm32h7xx_it.h"
#include "ethernet/ethernet.h"

extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;

void NMI_Handler(void) {
	while (1) {
	}
}

void HardFault_Handler(void) {
	while (1) {
	}
}

void MemManage_Handler(void) {
	while (1) {
	}
}

void BusFault_Handler(void) {
	while (1) {
	}
}

void UsageFault_Handler(void) {
	while (1) {
	}
}

void SVC_Handler(void) {
}

void DebugMon_Handler(void) {
}

void PendSV_Handler(void) {
}

void DMA_STR0_IRQHandler(void)
{

  HAL_DMA_IRQHandler(&hdma_usart3_rx);

}
void DMA_STR1_IRQHandler (void)
{
  HAL_DMA_IRQHandler(&hdma_usart3_tx);
}

void SysTick_Handler(void)
{
  HAL_IncTick();
}
