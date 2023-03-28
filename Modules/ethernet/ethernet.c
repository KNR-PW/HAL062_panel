/**
 ******************************************************************************
 * @file           : error_handlers.h
 * @author         : K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          : Ethernet - functionality
 ******************************************************************************
 */

#include "ethernet.h"
#include "error_handlers/error_handlers.h"

UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

uint8_t ethTxBuffer[19];
uint8_t ethRxBuffer[19];

void eth_init() {
	// TODO: test
	/* DMA controller clock enable */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Stream0_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
	/* DMA1_Stream1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);


	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart3) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK) {
		Error_Handler();
	}
}

void eth_send_massage(uint8_t *frameID, uint8_t *msgData) {

	//TODO: checking size of sent data:

	ethTxBuffer[0] = '#';
	for(uint8_t i = 0; i<2; i++) ethTxBuffer[i+1] = frameID[i];
	for(uint8_t i = 0; i< 16; i++) ethTxBuffer[i+3] = msgData[i];

	HAL_UART_Transmit(&huart3, ethTxBuffer, 19,1000);
}

void eth_receive_massage() {
	HAL_UART_Receive_DMA(&huart3,ethRxBuffer,19);
}

void decode_UART() {
	// TODO: implement functionality of decoding message
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	//obsługa ramek
	uint8_t data[16];
	uint8_t id[2];

	for(uint8_t i=0;i<2;i++) id[i]=ethRxBuffer[i+1];
	for(uint8_t i=0;i<16;i++) data[i]=ethRxBuffer[i+3];
	eth_send_massage(id, data);
//	HAL_UART_DMAResume(&huart3);
//	HAL_DMA_Abort_IT(&hdma_usart3_rx);
//	HAL_UART_Receive_DMA(&huart3,ethRxBuffer,19);
;

//	HAL_UART_Receive_DMA(&huart3, ethRxBuffer,19);

}
