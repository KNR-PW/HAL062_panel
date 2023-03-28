/**
 ******************************************************************************
 * @file           : error_handlers.h
 * @author         : K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          : Ethernet - headers file
 ******************************************************************************
 */

#ifndef ETHERNET_ETHERNET_H_
#define ETHERNET_ETHERNET_H_

#include <stm32h7xx_hal.h>




// @brief Initializing ethernet module: GPIO, UART
// @param None
// @returns void
void eth_init();


// @brief sending massage using special frame
// @param frameID array of uint8_t, specifying rover modules
// @param msgData array of uint8_t, contains data connected to ID
// @returns void
void eth_send_massage(uint8_t *frameID, uint8_t *msgData);


// @brief Begins to listening of data
// @returns TODO: failure feedback
void eth_receive_massage();


// @brief Decoding UART received massage
// @returns TODO: decoded massage
void decode_UART();

#endif /* ETHERNET_ETHERNET_H_ */