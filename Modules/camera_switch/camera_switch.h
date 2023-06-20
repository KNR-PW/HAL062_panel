/**
 ******************************************************************************
 * @file           camera_switch.h
 * @author         K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          Switches to cameras functionality
 ******************************************************************************
 */


#ifndef CAMERA_SWITCH_CAMERA_SWITCH_H_
#define CAMERA_SWITCH_CAMERA_SWITCH_H_

/* Includes --------------------------------------------------------- */
#include <stm32h7xx_hal.h>


/**
 * @brief 
 * Struct representing single camera screen with switch to changing image (one screen handles 3 cam).
 * 
 * @details
 * A,B,C,D channels are simply read pin state connected with 4-position switch.
 * It allows to contain information with camera image is currently shown on screen.
 * cameraNumber - number of screen that struct is connected to
*/
struct cameraSwitch{
	uint8_t cameraNumber;
	GPIO_PinState channel_A;
	GPIO_PinState channel_B;
	GPIO_PinState channel_C;
	GPIO_PinState channel_D;
};

/**
 * @brief 
 * Function to check is there more than one channel set to 1 (SET). 
 * 
 * @param struct CameraSwitch camSw
 * Functions takes CameraSwitch structure as parameter.
 * 
 * @details
 * As cameraSwitch structure represents switch it cannot have set state on more than one pin.
 * This function was created to check correctness of pins readings.
 * If there are situation that there were more than one pin read as set, it throws error.
 * Function is importatnt to have mechanizm blocking sending message to mainboard with
 * incorrect information - it could make errors in rover driver.
*/
HAL_StatusTypeDef Check_Camera_State(struct cameraSwitch camSw);

/**
 * @brief 
 * Function to read from GPIO 4-position switch state.
 * 
 * @details
 * Function scans gpio connected with 4-position switches (3 switches to one of camera screens)
 * Then it will overwrite correct cameraSwitch structure atrubute (state of channels) for every
 * camera switch structure.
*/
void Read_Camera_Switch_Value(void);

/**
 * @brief 
 * Function to send via Ethernet (UART) state of camera switches
 * 
 * @details
 * As rover has to know with camera image should be transmitted to screens,
 * this function send that information in previously defined frame.
 * 
 * @see 
 * UART frame documentation (camera switch message frame).
*/
void Send_Cameras_State(void);

/**
 * @brief 
 * Function to set LED informing of current switch position
 * 
 * @details
 * LED connected with 4-position switch could be connected to switch in two ways.
 * If fully electronical connection aren't implemented this function can make it 
 * entirely by software. After scanning 4-pos switch pins (GPIO) this function
 * will read current position and light up only this led that is required.
*/
void Set_Camera_LED(void);

#endif //CAMERA_SWITCH_CAMERA_SWITCH_H_
