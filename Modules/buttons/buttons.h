/**
 ******************************************************************************
 * @file           buttons.h
 * @author         K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          Buttons header file
 ******************************************************************************
 */


#ifndef BUTTONS_BUTTONS_H_
#define BUTTONS_BUTTONS_H_


/**
 * @brief Initializes GPIO ports and inputs for buttons
 */
void Buttons_Init(void);

/**
 * @brief Sets LED lights state depending on bistable button
 * @details
 *        This functionality is used to indicate on the panel, 
 *        whether bistable button is pressed.
 *        The function uses bit masks to read button state and sets proper
 *        I2C signal (according to I2C module documentation) which at the end
 *        is being written by the interrupt.
 */
void Set_LED_For_Bistable(void);


/**
 * @brief Initializes ADC module
 */
void ADC1_Init(void);

/**
 * @brief Reads value from potentiometer
 */
void ADC_Try_Read(void);

#endif //BUTTON_BUTTON_H_
