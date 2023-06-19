/**
 ******************************************************************************
 * @file           ethernet.h
 * @author         K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          Ethernet - headers file
 ******************************************************************************
 */

#ifndef ETHERNET_ETHERNET_H_
#define ETHERNET_ETHERNET_H_

#include <stm32h7xx_hal.h>

/**
 * @brief Initializing ethernet module: GPIO, UART
 * @param None
 * @returns void
*/
void Eth_Init();


/**
 * @brief sending message using special frame
 * @param frameID array of uint8_t, specifying rover modules
 * @param msgData array of uint8_t, contains data connected to ID
 * @returns void
 */
void Eth_Send_Massage(uint8_t *frameID, uint8_t *msgData);


/** 
 * @brief Begins to listening of data
 * @returns void
 */
void Eth_Receive_Massage();


#endif /* ETHERNET_ETHERNET_H_ */
