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

void Eth_Init();

/**
 * @param frameID array of uint8_t, specifying rover modules
 * @param msgData array of uint8_t, contains data connected to ID
 */
void Eth_Send_Massage(uint8_t *frameID, uint8_t *msgData);

void Eth_Receive_Massage();

void ETH_Code_UART(const uint8_t number, uint8_t* hex_in_ascii);

#endif /* ETHERNET_ETHERNET_H_ */
