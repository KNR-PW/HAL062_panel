/**
 ******************************************************************************
 * @file           camera_switch.c
 * @author         K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          Functionality for camera switch
 ******************************************************************************
 * @details
 * 		This file contains functions to handle camera switch, screens and
 * 		sending information to rover of witch camera should be activatied.
 * 		To proper functionality it is required to have ethernet module
 * 		configured. For any specifics of uart frame and electronical side
 * 		of camera switch there are separated documentations:
 * 		- UART frame documentation
 * 		- Mainboard electric schema
 * 		- Panel electric schema
 * 		- Mainboard software documentation
 * 
 ******************************************************************************
 */

/* Includes ------------------------ */
#include "camera_switch.h"
#include "buttons/buttons_const.h"
#include "error_handlers/error_handlers.h"
#include "ethernet/ethernet.h"
#include "LED_switch/LED_switch.h"
#include "LED_switch/LED_const.h"
#include <stdbool.h>
#include <stm32h7xx_hal_gpio.h>

static struct cameraSwitch yellowCamera = {1,0,0,0,0}; /*!< cameraSwitch structure represents yellow camera screen*/
static struct cameraSwitch blueCamera = {2,0,0,0,0}; /*!< cameraSwitch structure represents blue camera screen*/
static struct cameraSwitch redCamera = {3,0,0,0,0}; /*!< cameraSwitch structure represents red camera screen*/

extern bool ethTxLineOpen; /*!< contains information of UART trasmition state (true if nothing is being sent)*/

static uint8_t cameraMsgID[2] = {0x32,0x44}; /*!< camera switch message ID - see uart frame documentation */
static uint8_t cameraMsgData[16]; /*!< camera switch message data - see uart frame documentation */

uint8_t currentCameraLight = 0; /*!< vairable informing with camera light is currently being set */


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
 * 
 * @return HAL_StatusTypeDef - HAL varaible containing information of error.
*/
HAL_StatusTypeDef Check_Camera_State(struct cameraSwitch camSw){
	uint8_t pinAlreadySet = 0;
	if(camSw.channel_A == GPIO_PIN_SET) pinAlreadySet = 1;
	if(camSw.channel_B == GPIO_PIN_SET){
		if(pinAlreadySet == 1) return HAL_ERROR;
		else pinAlreadySet = 1;
	}
	if(camSw.channel_C == GPIO_PIN_SET){
		if(pinAlreadySet == 1) return HAL_ERROR;
		else pinAlreadySet = 1;
	}
	if(camSw.channel_D == GPIO_PIN_SET){
		if(pinAlreadySet == 1) return HAL_ERROR;
		else pinAlreadySet = 1;
	}
	return HAL_OK;

}

/**
 * @brief 
 * Function to read from GPIO 4-position switch state.
 * 
 * @details
 * Function scans gpio connected with 4-position switches (3 switches to one of camera screens)
 * Then it will overwrite correct cameraSwitch structure atrubute (state of channels) for every
 * camera switch structure.
*/
void Read_Camera_Switch_Value(void){

	yellowCamera.channel_A = HAL_GPIO_ReadPin(CAM_SWITCH_1_A_GPIO_Port, CAM_SWITCH_1_A_Pin);
	yellowCamera.channel_B = HAL_GPIO_ReadPin(CAM_SWITCH_1_B_GPIO_Port, CAM_SWITCH_1_B_Pin);
	yellowCamera.channel_C = HAL_GPIO_ReadPin(CAM_SWITCH_1_C_GPIO_Port, CAM_SWITCH_1_C_Pin);
	yellowCamera.channel_D = HAL_GPIO_ReadPin(CAM_SWITCH_1_D_GPIO_Port, CAM_SWITCH_1_D_Pin);

	blueCamera.channel_A = HAL_GPIO_ReadPin(CAM_SWITCH_2_A_GPIO_Port, CAM_SWITCH_2_A_Pin);
	blueCamera.channel_B = HAL_GPIO_ReadPin(CAM_SWITCH_2_B_GPIO_Port, CAM_SWITCH_2_B_Pin);
	blueCamera.channel_C = HAL_GPIO_ReadPin(CAM_SWITCH_2_C_GPIO_Port, CAM_SWITCH_2_C_Pin);
	blueCamera.channel_D = HAL_GPIO_ReadPin(CAM_SWITCH_2_D_GPIO_Port, CAM_SWITCH_2_D_Pin);

	redCamera.channel_A = HAL_GPIO_ReadPin(CAM_SWITCH_3_A_GPIO_Port, CAM_SWITCH_3_A_Pin);
	redCamera.channel_B = HAL_GPIO_ReadPin(CAM_SWITCH_3_B_GPIO_Port, CAM_SWITCH_3_B_Pin);
	redCamera.channel_C = HAL_GPIO_ReadPin(CAM_SWITCH_3_C_GPIO_Port, CAM_SWITCH_3_C_Pin);
	redCamera.channel_D = HAL_GPIO_ReadPin(CAM_SWITCH_3_D_GPIO_Port, CAM_SWITCH_3_D_Pin);

}

/**
 * @brief 
 * Function to send via Ethernet (UART) state of camera switches
 * 
 * @details
 * As rover has to know with camera image should be transmitted to screens,
 * this function send that information in previously defined frame. \n
 * Send_Cameras_State() creates a frame that sends \n
 * 
 * @see 
 * UART frame documentation (camera switch message frame).
*/
void Send_Cameras_State(void){

	// camera red
	if(redCamera.channel_A == GPIO_PIN_SET){
		cameraMsgData[0] = 0x30;
		cameraMsgData[1] = 0x31;
	}
	else if(redCamera.channel_B == GPIO_PIN_SET){
		cameraMsgData[0] = 0x30;
		cameraMsgData[1] = 0x32;
	}
	else if(redCamera.channel_C == GPIO_PIN_SET){
		cameraMsgData[0] = 0x30;
		cameraMsgData[1] = 0x33;
	}
	else{
		cameraMsgData[0] = 0x30;
		cameraMsgData[1] = 0x30;
	}

	// camera blue
	if(blueCamera.channel_A == GPIO_PIN_SET){
		cameraMsgData[2] = 0x30;
		cameraMsgData[3] = 0x31;
	}
	else if(blueCamera.channel_B == GPIO_PIN_SET){
		cameraMsgData[2] = 0x30;
		cameraMsgData[3] = 0x32;
	}
	else if(blueCamera.channel_C == GPIO_PIN_SET){
		cameraMsgData[2] = 0x30;
		cameraMsgData[3] = 0x33;
	}
	else{
		cameraMsgData[2] = 0x30;
		cameraMsgData[3] = 0x30;
	}

	// camera yellow
	if(yellowCamera.channel_A == GPIO_PIN_SET){
		cameraMsgData[4] = 0x30;
		cameraMsgData[5] = 0x31;
	}
	else if(yellowCamera.channel_B == GPIO_PIN_SET){
		cameraMsgData[4] = 0x30;
		cameraMsgData[5] = 0x32;
	}
	else if(yellowCamera.channel_C == GPIO_PIN_SET){
		cameraMsgData[4] = 0x30;
		cameraMsgData[5] = 0x33;
	}
	else{
		cameraMsgData[4] = 0x30;
		cameraMsgData[5] = 0x30;
	}

	for(int i = 6; i<16; ++i){
	cameraMsgData[i] = 'X';
	}

	if(ethTxLineOpen){
//	Eth_Send_Massage(cameraMsgID, cameraMsgData);
	}

}

/**
 * @brief 
 * Function to set LED informing of current switch position
 * 
 * @details
 * LED connected with 4-position switch could be connected to switch in two ways.
 * If fully electronical connection aren't implemented this function can make it 
 * entirely by software. After scanning 4-pos switch pins (GPIO) this function
 * will read current position and light up only this led that is required. \n 
 * In order not to have i2c line busy during attemption to light up
 * one led after another this function will set led on at time, so it is required
 * to use this function in loop with some delay (f.e timer interuption in every 10ms)
*/
void Set_Camera_LED(void){
	switch(currentCameraLight)
	{
	case 0:
		LED_Set(LIGHT7, yellowCamera.channel_C);
		currentCameraLight = 1;
		break;
	case 1:
		LED_Set(LIGHT9, yellowCamera.channel_B);
		currentCameraLight = 2;
		break;
	case 2:
		LED_Set(LIGHT8, yellowCamera.channel_A);
		currentCameraLight = 3;
		break;
	case 3:
		LED_Set(LIGHT10, blueCamera.channel_C);
		currentCameraLight = 4;
		break;
	case 4:
		LED_Set(LIGHT11, blueCamera.channel_B);
		currentCameraLight = 5;
	case 5:
		LED_Set(LIGHT12, blueCamera.channel_A);
		currentCameraLight = 6;
		break;
	case 6:
		LED_Set(LIGHT13, redCamera.channel_C);
		currentCameraLight = 7;
		break;
	case 7:
		LED_Set(LIGHT14, redCamera.channel_B);
		currentCameraLight = 8;
		break;
	case 8:
		LED_Set(LIGHT15, redCamera.channel_A);
		currentCameraLight = 0;
		break;
	}
}
