/**
 *************************************************************************************
 * @file           : joystick.c
 * @author         : K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          : Joystick module - implementing functionality and initialization
 *************************************************************************************
 */

/* Includes ------------------*/
#include "joystick.h"
#include "error_handlers/error_handlers.h"
#include "joystick_const.h"
#include "ethernet/ethernet.h"
#include <stdbool.h>
#include "buttons/buttons.h"
#include "buttons/buttons_const.h"


I2C_HandleTypeDef hi2c2; /*!< I2C handler (HAL library)*/

bool receiveIsReady = false; /*!< varaible true if we can receive*/
bool joyInitFinished = false; /*!< variable true after external ADC converter configuration */

static uint8_t receiveData[24]; /*!< buffer for external adc converter readings
								@see MAX11616EEE+ ADC converter documentation */

struct Joystick motorJoy; /*!< structur representing joystick for motors*/
struct Joystick manipJoy; /*!< structur representing joystick for manipulator*/
struct Joystick gripperJoy; /*!< structur representing joystick for gripper*/

extern double val; /*!< raw value devider to shorten range of sent joystick value 
					@see buttons.c file documentation.*/

static uint8_t currentReading = 0; /*!< Variable using to know which joystick is currently read. \n
										It is important to form correct UART frame */

extern bool ethTxLineOpen; /*!< Flag to allow data transmition. @see ethernet.c file documentation. */

static uint8_t gripperMode; /*!< Additional data sent with gripper joystick to know gripper mode. \n
							If gripper should keep the level this data is '1', otherwise, if joystick steer
							gripper this data is '0' */

/**
 * @brief Function for initializing I2C in interruption mode.
 */
void Joystick_I2C_Init(void) {

	__HAL_RCC_GPIOB_CLK_ENABLE();

	hi2c2.Instance = I2C2;
	hi2c2.Init.Timing = 0x10707DBC;
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c2) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief Function to send configuration to external ADC converter
 * @details
 * To maintain proper functionality external ADC converter should be configure after every power reset.\n
 * This function has to be called in order to software begin reading converter values. After configuration,
 * transmition callback will set receiveIsReade flag.
 * @see MAX11616EEE+ ADC converter documentation.
 */
void Joystick_Write_Conditions(void) {
	uint8_t configData[2];
	configData[0] = CONFIG_DATA;
	configData[1] = SETUP_DATA;
	uint16_t address = SLAVE_ADDRESS << 1;
	HAL_I2C_Master_Transmit_IT(&hi2c2, address, configData, 2);
	HAL_Delay(500);
}

/**
 * @brief Function that sets flag, whitch allows to reading value from converter.\n
 * It is important not to call this function before MAX11616EEE+ converter configuration.
 */
void Joystick_Read_Value_Start(void) {
	receiveIsReady = true;
}

/**
 * @brief Function to send request by I2C to ADC converter in order to receive readings.
 * @see MAX11616EEE+ ADC converter documentation.
 */
void Jostick_Read_Value(void){
		receiveIsReady = false;
		uint16_t address = (SLAVE_ADDRESS << 1) | 0x01;
		HAL_I2C_Master_Receive_IT(&hi2c2, address, receiveData, 24);
}

/**
 * @brief This transmition callback is evoked after transmitiion.
 * (First after configuration, then after every request to ADC converter)
 */
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
	if (hi2c->Instance == I2C2) {
	joyInitFinished = true;
	Joystick_Read_Value_Start();
	}
}

/**
 * @brief This reception callback is evoked after receiving readings from ADC converter
 * in order to handle them.
 * @details
 * After readings from joystick reception this function will set correct attributes in strucutres that
 * represents joysticks. After that it calls Joystick_Send_Readings() function to send readings to rover
 * via Ethernet.
 */
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	if (hi2c->Instance == I2C2) {

		//reading value from first joystick
		motorJoy.xVal = (((uint16_t) (receiveData[0] & 0x0F)) << 8)
				| (uint16_t) receiveData[1];
		motorJoy.yVal = (((uint16_t) (receiveData[2] & 0x0F)) << 8)
				| (uint16_t) receiveData[3];
		motorJoy.zVal = (((uint16_t) (receiveData[4] & 0x0F)) << 8)
				| (uint16_t) receiveData[5];
		motorJoy.midVal = (((uint16_t) (receiveData[6] & 0x0F)) << 8)
				| (uint16_t) receiveData[7];

		//reading value from second joystick
		manipJoy.xVal = (((uint16_t) (receiveData[8] & 0x0F)) << 8)
				| (uint16_t) receiveData[9];
		manipJoy.yVal = (((uint16_t) (receiveData[10] & 0x0F)) << 8)
				| (uint16_t) receiveData[11];
		manipJoy.zVal = (((uint16_t) (receiveData[12] & 0x0F)) << 8)
				| (uint16_t) receiveData[13];
		manipJoy.midVal = (((uint16_t) (receiveData[14] & 0x0F)) << 8)
				| (uint16_t) receiveData[15];

		//reading value from third joystick
		gripperJoy.xVal = (((uint16_t) (receiveData[16] & 0x0F)) << 8)
				| (uint16_t) receiveData[17];
		gripperJoy.yVal = (((uint16_t) (receiveData[18] & 0x0F)) << 8)
				| (uint16_t) receiveData[19];
		gripperJoy.zVal = (((uint16_t) (receiveData[20] & 0x0F)) << 8)
				| (uint16_t) receiveData[21];
		gripperJoy.midVal = (((uint16_t) (receiveData[22] & 0x0F)) << 8)
				| (uint16_t) receiveData[23];

		//interpreting readings and scaling values down

		motorJoy.xPos = (int16_t)((int16_t) motorJoy.midVal - (int16_t) motorJoy.xVal)
				* val;
		motorJoy.yPos = (int16_t)((int16_t) motorJoy.midVal - (int16_t) motorJoy.yVal)
				* val;

		manipJoy.xPos = (int16_t)(((int16_t) manipJoy.midVal - (int16_t)manipJoy.xVal)
				* val);
		manipJoy.yPos = (int16_t)(((int16_t) manipJoy.midVal - (int16_t)manipJoy.yVal)
						* val);
		manipJoy.zPos = (int16_t)(((int16_t) manipJoy.midVal - (int16_t)manipJoy.zVal)
						* val);

		gripperJoy.yPos = (int16_t)( (int16_t) gripperJoy.midVal
				- (int16_t)(int16_t) gripperJoy.yVal) * val;
		gripperJoy.xPos = ( (int16_t)gripperJoy.midVal
				-  (int16_t)(int16_t)gripperJoy.xVal) * val;
		gripperJoy.zPos = ( (int16_t)gripperJoy.midVal
				-  (int16_t)gripperJoy.zVal) * val;


		//sending joysticks' readings
		Joystick_Send_Readings();

		//enabling receiving again
		receiveIsReady = true;
	}

}

/**
 * @brief This function send joystick values via ethernet in proper UART frame
 * @details It is important to form correct data frame. In order to do that this function
 * (depending of which joystick we currently want to send) prepare data in correct form. \n
 * There are IDs that represents joystick: \n
 * - 20 - ID of motors joystick value message \n
 * - 21 - ID of manipulator joystick value message \n
 * - 22 - ID of gripper joystick value message \n
 */
void Joystick_Send_Readings(void) {

	if(currentReading == 0 && ethTxLineOpen){
		//differential speed calculating
		int8_t rightSpeed = motorJoy.xPos + motorJoy.yPos;
		int8_t leftSpeed = motorJoy.xPos - motorJoy.yPos;

		//checking if values are in range
		if (rightSpeed > 100)
			rightSpeed = 100;
		if (leftSpeed > 100)
			leftSpeed = 100;

		if (rightSpeed < -100)
			rightSpeed = -100;
		if (leftSpeed < -100)
			leftSpeed = -100;

		uint8_t msgID[2] = {'2', '0'};
		uint8_t msgData[16] = { (uint8_t)rightSpeed, (uint8_t)rightSpeed,(uint8_t) rightSpeed,
								(uint8_t)leftSpeed, (uint8_t)leftSpeed, (uint8_t)leftSpeed,
								0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78 };
		Eth_Send_Massage(msgID, msgData);
		currentReading = 1;
	}

	else if(currentReading == 1 && ethTxLineOpen){
		uint8_t msgData[16] = {(uint8_t) manipJoy.xPos,(uint8_t) manipJoy.yPos,(uint8_t) manipJoy.zPos,
								0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78};
		uint8_t msgID[2] = {'2', '1'};
		Eth_Send_Massage(msgID, msgData);
		currentReading = 2;
	}

	else if(currentReading == 2 && ethTxLineOpen){
		// reading if gripper should keep level (button is pressed)
		uint8_t state_fb= HAL_GPIO_ReadPin(BI_BUTTON_RED_2_GPIO_Port, BI_BUTTON_RED_2_Pin);

		// creating correct data
		if(state_fb == GPIO_PIN_SET){
		gripperMode = '1';
		}
		else{
		gripperMode = '0';
		}

		uint8_t msgData[16] = {(uint8_t)gripperJoy.xPos,(uint8_t)gripperJoy.yPos,(uint8_t)gripperJoy.zPos,gripperMode,
		0x78,0x78,0x78,0x78,
				0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78};
		uint8_t msgID[2] = {'2', '2'};
		Eth_Send_Massage(msgID, msgData);
		currentReading = 0;
	}

}



