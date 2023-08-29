/**
 ******************************************************************************
 * @file           ethernet.c
 * @author         K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          Ethernet communication via UART functionality
 * @details
 * Ethernet module: W7500S2E-R1\n
 * Panelboard has pinout connected with external ethernet module. 
 * This module convert serial data (UART without flow control) to
 * ethernet standart. This module requires configuration as TCP client (or TCP server
 * in case of connecting with computer). \n
 * There are implemented UART using DMA.
 *  @see W7500S2E-R1 ethernet module documentation
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------ */
#include "ethernet.h"
#include "error_handlers/error_handlers.h"
#include <stdbool.h>
#include "LED_switch/LED_const.h"
#include "LED_switch/LED_switch.h"


UART_HandleTypeDef huart3; /*!< uart handler structure (HAL library) */
DMA_HandleTypeDef hdma_usart3_rx; /*!< uart receive dma handler  (HAL library) */
DMA_HandleTypeDef hdma_usart3_tx; /*!< uart transmit dma handler (HAL library) */

static uint8_t ethTxBuffer[19]; /*!< buffer to transmittion 19 bytes of data. @see UART frame documentation */
static uint8_t ethRxBuffer[19]; /*!< buffer to reception 19 bytes of data. @see UART frame documentation */

bool ethTxLineOpen = true; /*!< flag to allow data transmition */

/**
 * @brief 
 * Function to initialization ethernet module - UART and DMA
 * 
 * @details
 * Used ethernet module W7500S2E-R1 requires data in UART protocol. \n
 * This function configure this UART and DMA. It also sets NVIC interruptions
 * 
 * @see stm32h743 reference manual - additional information about DMA and UART
*/
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

/**
 * @brief 
 * Function to send message via ethernet (UART).
 * 
 * @param uint8_t *frameID - information of message ID (UART frame documentation)
 * @param uint8_t *msgData - information of message data (UART frame documentation)
 * @details
 * Entire rover software use previously prepared data frame. It contains 2 bytes
 * of ID connected with concrete rover module (f.e motors, manipulator, etc.) and 16 bytes
 * of data. If there are fewer data required to send other bytes are field with 'x'.
 * First byte is always '#' and this function add this at the beggining of message. \n
 * Example:
 * @verbatim
 [#][id1][id2][data1][data2][data3][x][x][x][x][x][x][x][x][x][x][x][x][x]
 @endverbatim
 * @see
 * UART frame documentation (in order to know how frame works and which ID's should be sent)
*/
void Eth_Send_Massage(uint8_t *frameID, uint8_t *msgData) {

	ethTxLineOpen = false;

	ethTxBuffer[0] = '#';
	for (uint8_t i = 0; i < 2; i++)
		ethTxBuffer[i + 1] = frameID[i];
	for (uint8_t i = 0; i < 16; i++)
		ethTxBuffer[i + 3] = msgData[i];


	HAL_UART_Transmit_DMA(&huart3, ethTxBuffer, 19);
}

/**
 * @brief 
 * Function to begin listening of data. 
 * @details
 * After data arrives, HAL_UART_RxCpltCallback() will be
 * immediatelly burst. In that callback listening is setting again.
*/
void Eth_Receive_Massage() {
	HAL_UART_Receive_DMA(&huart3, ethRxBuffer, 19);
}

/**
 * @brief 
 * UART reception complete callback.
 * @details
 * After data arrives,this function handles carried information and set listening again.
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

	HAL_UART_Receive_DMA(&huart3, ethRxBuffer, 19);

}

/**
 * @brief 
 * UART transmition complete callback.
 * @details
 * After data transmition this function set flags required to transmit other data.
*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	huart->gState = HAL_UART_STATE_READY;
	ethTxLineOpen = true;
}

void ETH_Code_UART(const uint8_t number, uint8_t* hex_in_ascii){
	uint8_t first_hex = (uint8_t)(((number & 0xF0)>>4) & 0x0F);
	uint8_t second_hex = (uint8_t)(number & 0x0F);


	if (first_hex >= 0x00 && first_hex <= 0x09){
		hex_in_ascii[0] = first_hex + 48;
	}
	else if (first_hex >= 0x0A && first_hex <= 0x0F){
		hex_in_ascii[0] = first_hex + 55;
	}

	if (second_hex >= 0x00 && second_hex <= 0x09){
		hex_in_ascii[1] = second_hex + 48;
	}
	else if (first_hex >= 0x0A && first_hex <= 0x0F){
		hex_in_ascii[1] = second_hex + 55;
		}

}

