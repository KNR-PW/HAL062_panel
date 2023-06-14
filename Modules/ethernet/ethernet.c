/**
 ******************************************************************************
 * @file           : error_handlers.h
 * @author         : K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          : Ethernet - functionality
 ******************************************************************************
 */

#include "ethernet.h"
#include "error_handlers/error_handlers.h"
#include <stdbool.h>
#include "LED_switch/LED_const.h"
#include "LED_switch/LED_switch.h"

UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

static uint8_t ethTxBuffer[19];
static uint8_t ethRxBuffer;

volatile uint8_t boudryButtonStates[3];

bool ethTxLineOpen = true;
bool test = false;
uint8_t taken = 0;
bool foundHash;
bool toSend = false;

void Eth_Init() {

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

	/* UART interrupt enabling*/
	HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART3_IRQn);

	/* UART configuration*/
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
	if (HAL_UARTEx_EnableFifoMode(&huart3) != HAL_OK) {
		Error_Handler();
	}
}

void Eth_Send_Massage(uint8_t *frameID, uint8_t *msgData) {

	ethTxLineOpen = false;

	ethTxBuffer[0] = '#';
	for (uint8_t i = 0; i < 2; i++)
		ethTxBuffer[i + 1] = frameID[i];
	for (uint8_t i = 0; i < 16; i++)
		ethTxBuffer[i + 3] = msgData[i];


	HAL_UART_Transmit_DMA(&huart3, ethTxBuffer, 19);
}


void Eth_Receive_Massage() {
	HAL_UART_Receive_DMA(&huart3, &ethRxBuffer, 1);

}

void decode_UART() {
	// TODO: implement functionality of decoding message
//	if(ethRxBuffer[0] != '#') Error_Handler();
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {


	HAL_UART_Receive_DMA(&huart3, &ethRxBuffer, 1);


}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	huart->gState = HAL_UART_STATE_READY;
	if(toSend){
		HAL_UART_Transmit_DMA(&huart3, ethRxBuffer, 19);
		toSend = false;
	}else{
	ethTxLineOpen = true;
	}
}

