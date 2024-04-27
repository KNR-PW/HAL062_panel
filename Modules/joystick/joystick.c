/**
 *************************************************************************************
 * @file           joystick.c
 * @author         K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          Joystick module - implementing functionality and initialization
 *************************************************************************************
 */

/* Includes ------------------*/
#include "joystick.h"
#include "error_handlers/error_handlers.h"
#include "joystick_const.h"
#include "ethernet/ethernet.h"
#include "buttons/buttons.h"
#include "buttons/buttons_const.h"
#include <stdbool.h>


I2C_HandleTypeDef hi2c2; /*!< I2C handler (HAL library)*/

bool receiveIsReady = false; /*!< variable true if we can receive*/
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
static uint8_t maniOrMotor = 0;

static uint8_t buttonOpt = 0;

extern bool ethTxLineOpen; /*!< Flag to allow data transmition. @see ethernet.c file documentation. */

static uint8_t gripperMode; /*!< Additional data sent with gripper joystick to know gripper mode. \n
							If gripper should keep the level this data is '1', otherwise, if joystick steer
							gripper this data is '0' */

static volatile uint8_t coded_left_speed[2];
static volatile uint8_t coded_right_speed[2];

union F2I speedDoF1, speedDoF2, speedDoF3, speedDoF4, speedDoF5, speedDoF6;
struct manipData dataDoF1, dataDoF2, dataDoF3, dataDoF4, dataDoF5, dataDoF6;

extern int on, off;

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

	if(maniOrMotor % 2 == 0 && ethTxLineOpen){
		//differential speed calculating
		float V = (float)motorJoy.yPos/100.0; //max metr na sekunde
		float Omega = (float)motorJoy.xPos/100.0;  //max pół radiana na sekunde

		float coeff = 100.0/6.45;
		float rightSpeedFloat = coeff*(6.45*V + 2.97*Omega);
		float leftSpeedFloat = coeff*(6.45*V - 2.97*Omega);

		int rightSpeed = (int)rightSpeedFloat;
		int leftSpeed = (int)leftSpeedFloat;


		//checking if values are in range
		if (rightSpeed > 100)
			rightSpeed = 100;
		if (leftSpeed > 100)
			leftSpeed = 100;

		if (rightSpeed < -100)
			rightSpeed = -100;
		if (leftSpeed < -100)
			leftSpeed = -100;

		leftSpeed = (int8_t)leftSpeed;
		rightSpeed = (int8_t)rightSpeed;

		uint8_t msgID[2] = {0x31,0x34};
		uint8_t msgData[16];

		//coding right motors velocity acording to UART fame

		ETH_Code_UART(rightSpeed,coded_right_speed);
		msgData[0] = coded_right_speed[0];
		msgData[1] = coded_right_speed[1];

		//coding left motors velocity acordnig to UART frame

		ETH_Code_UART(leftSpeed,coded_left_speed);
		msgData[2] = coded_left_speed[0];
		msgData[3] = coded_left_speed[1];

		for(uint8_t i = 4; i<16; i++){
			msgData[i] = 'X';
		}
//		if(((rightSpeed > 50||rightSpeed < -50) && (leftSpeed > 50 || leftSpeed < -50))||rightSpeed==0||leftSpeed==0){
		Eth_Send_Massage(msgID, msgData);
//		}
		maniOrMotor = 1;
	}

	else if(currentReading == 0 && maniOrMotor % 2 == 1 && ethTxLineOpen){
		uint8_t codedData[2];
		uint8_t msgData[16];
		uint8_t msgID[2];

		speedDoF1.f = -1.0*(float)manipJoy.yPos/1.0;

		dataDoF1.byte0 = (speedDoF1.uint & 0xFF000000)>>24;
		ETH_Code_UART(dataDoF1.byte0, codedData);
		msgData[0] = codedData[0];
		msgData[1] = codedData[1];

		dataDoF1.byte1 = (speedDoF1.uint & 0x00FF0000)>>16;
		ETH_Code_UART(dataDoF1.byte1, codedData);
		msgData[2] = codedData[0];
		msgData[3] = codedData[1];

		dataDoF1.byte2 = (speedDoF1.uint & 0x0000FF00)>>8;
		ETH_Code_UART(dataDoF1.byte2, codedData);
		msgData[4] = codedData[0];
		msgData[5] = codedData[1];

		dataDoF1.byte3 = (speedDoF1.uint & 0x000000FF);
		ETH_Code_UART(dataDoF1.byte3, codedData);
		msgData[6] = codedData[0];
		msgData[7] = codedData[1];


		speedDoF2.f = -1.0*(float)manipJoy.xPos/1.0;

		dataDoF2.byte0 = (speedDoF2.uint & 0xFF000000)>>24;
		ETH_Code_UART(dataDoF2.byte0, codedData);
		msgData[8] = codedData[0];
		msgData[9] = codedData[1];

		dataDoF2.byte1 = (speedDoF2.uint & 0x00FF0000)>>16;
		ETH_Code_UART(dataDoF2.byte1, codedData);
		msgData[10] = codedData[0];
		msgData[11] = codedData[1];

		dataDoF2.byte2 = (speedDoF2.uint & 0x0000FF00)>>8;
		ETH_Code_UART(dataDoF2.byte2, codedData);
		msgData[12] = codedData[0];
		msgData[13] = codedData[1];

		dataDoF2.byte3 = (speedDoF2.uint & 0x000000FF);
		ETH_Code_UART(dataDoF2.byte3, codedData);
		msgData[14] = codedData[0];
		msgData[15] = codedData[1];

		ETH_Code_UART(129, msgID);

		Eth_Send_Massage(msgID, msgData);

		currentReading = 1;
		maniOrMotor = 0;
	}

	else if(currentReading == 1 && maniOrMotor % 2 == 1 && ethTxLineOpen){
		uint8_t codedData[2];
		uint8_t msgData[16];
		uint8_t msgID[2];

		speedDoF3.f = -1.0*(float)manipJoy.zPos/1.0;

		dataDoF3.byte0 = (speedDoF3.uint & 0xFF000000)>>24;
		ETH_Code_UART(dataDoF3.byte0, codedData);
		msgData[0] = codedData[0];
		msgData[1] = codedData[1];

		dataDoF3.byte1 = (speedDoF3.uint & 0x00FF0000)>>16;
		ETH_Code_UART(dataDoF3.byte1, codedData);
		msgData[2] = codedData[0];
		msgData[3] = codedData[1];

		dataDoF3.byte2 = (speedDoF3.uint & 0x0000FF00)>>8;
		ETH_Code_UART(dataDoF3.byte2, codedData);
		msgData[4] = codedData[0];
		msgData[5] = codedData[1];

		dataDoF3.byte3 = (speedDoF3.uint & 0x000000FF);
		ETH_Code_UART(dataDoF3.byte3, codedData);
		msgData[6] = codedData[0];
		msgData[7] = codedData[1];


		speedDoF4.f = (float)gripperJoy.xPos/1.0;

		dataDoF4.byte0 = (speedDoF4.uint & 0xFF000000)>>24;
		ETH_Code_UART(dataDoF4.byte0, codedData);
		msgData[8] = codedData[0];
		msgData[9] = codedData[1];

		dataDoF4.byte1 = (speedDoF4.uint & 0x00FF0000)>>16;
		ETH_Code_UART(dataDoF4.byte1, codedData);
		msgData[10] = codedData[0];
		msgData[11] = codedData[1];

		dataDoF4.byte2 = (speedDoF4.uint & 0x0000FF00)>>8;
		ETH_Code_UART(dataDoF4.byte2, codedData);
		msgData[12] = codedData[0];
		msgData[13] = codedData[1];

		dataDoF4.byte3 = (speedDoF4.uint & 0x000000FF);
		ETH_Code_UART(dataDoF4.byte3, codedData);
		msgData[14] = codedData[0];
		msgData[15] = codedData[1];

		ETH_Code_UART(130, msgID);

		Eth_Send_Massage(msgID, msgData);


		currentReading = 2;
		maniOrMotor = 0;
	}
	else if(currentReading == 2 && maniOrMotor % 2 == 1 && ethTxLineOpen){
		uint8_t codedData[2];
		uint8_t msgData[16];
		uint8_t msgID[2];

		speedDoF5.f = (float)gripperJoy.yPos/1.0;

		dataDoF5.byte0 = (speedDoF5.uint & 0xFF000000)>>24;
		ETH_Code_UART(dataDoF5.byte0, codedData);
		msgData[0] = codedData[0];
		msgData[1] = codedData[1];

		dataDoF5.byte1 = (speedDoF5.uint & 0x00FF0000)>>16;
		ETH_Code_UART(dataDoF5.byte1, codedData);
		msgData[2] = codedData[0];
		msgData[3] = codedData[1];

		dataDoF5.byte2 = (speedDoF5.uint & 0x0000FF00)>>8;
		ETH_Code_UART(dataDoF5.byte2, codedData);
		msgData[4] = codedData[0];
		msgData[5] = codedData[1];

		dataDoF5.byte3 = (speedDoF5.uint & 0x000000FF);
		ETH_Code_UART(dataDoF5.byte3, codedData);
		msgData[6] = codedData[0];
		msgData[7] = codedData[1];


		speedDoF6.f = -1.0*(float)gripperJoy.zPos/1.0;

		dataDoF6.byte0 = (speedDoF6.uint & 0xFF000000)>>24;
		ETH_Code_UART(dataDoF6.byte0, codedData);
		msgData[8] = codedData[0];
		msgData[9] = codedData[1];

		dataDoF6.byte1 = (speedDoF6.uint & 0x00FF0000)>>16;
		ETH_Code_UART(dataDoF6.byte1, codedData);
		msgData[10] = codedData[0];
		msgData[11] = codedData[1];

		dataDoF6.byte2 = (speedDoF6.uint & 0x0000FF00)>>8;
		ETH_Code_UART(dataDoF6.byte2, codedData);
		msgData[12] = codedData[0];
		msgData[13] = codedData[1];

		dataDoF6.byte3 = (speedDoF6.uint & 0x000000FF);
		ETH_Code_UART(dataDoF6.byte3, codedData);
		msgData[14] = codedData[0];
		msgData[15] = codedData[1];

		ETH_Code_UART(131, msgID);

		Eth_Send_Massage(msgID, msgData);

		currentReading = 3;
		maniOrMotor = 0;
	}
	else if(currentReading == 3 && maniOrMotor % 2 == 1 && ethTxLineOpen){
		if(buttonOpt % 4 == 0){
		GPIO_PinState manipState;
		manipState = HAL_GPIO_ReadPin(BI_BUTTON_GREEN_3_GPIO_Port, BI_BUTTON_GREEN_3_Pin);
		if(manipState == GPIO_PIN_RESET){
			uint8_t msgID[2];
			uint8_t msgData[16];
			uint8_t codedData[2];

			ETH_Code_UART(128, msgID);
			msgData[0] = 0x30; msgData[1] = 0x31;
			msgData[2] = 0x30; msgData[3] = 0x31;
			msgData[4] = 0x30; msgData[5] = 0x32;
			msgData[6] = 0x30; msgData[7] = 0x32;
			msgData[8] = 0x30; msgData[9] = 0x30;
			msgData[10] = 0x30; msgData[11] = 0x30;
			for(uint8_t i = 12; i<16;i++){
				msgData[i] = 'X';
			}
			Eth_Send_Massage(msgID, msgData);
		}
		else if (manipState == GPIO_PIN_SET) {
			uint8_t msgID[2];
			uint8_t msgData[16];
			uint8_t codedData[2];

			ETH_Code_UART(128, msgID);
			msgData[0] = 0x30; msgData[1] = 0x31;
			msgData[2] = 0x30; msgData[3] = 0x31;
			msgData[4] = 0x30; msgData[5] = 0x33;
			msgData[6] = 0x30; msgData[7] = 0x33;
			msgData[8] = 0x30; msgData[9] = 0x30;
			msgData[10] = 0x30; msgData[11] = 0x30;
			for(uint8_t i = 12; i<16;i++){
				msgData[i] = 'X';
			}
			Eth_Send_Massage(msgID, msgData);
		}
		buttonOpt=1;
		}
		else
		{
			GPIO_PinState gripperClose;
			gripperClose = HAL_GPIO_ReadPin(BI_BUTTON_GREEN_2_GPIO_Port, BI_BUTTON_GREEN_2_Pin);
			GPIO_PinState gripperOpen;
			gripperOpen = HAL_GPIO_ReadPin(BI_BUTTON_GREEN_1_GPIO_Port, BI_BUTTON_GREEN_1_Pin);
			if((gripperClose == GPIO_PIN_RESET && gripperOpen == GPIO_PIN_RESET)||(gripperClose == GPIO_PIN_SET && gripperOpen == GPIO_PIN_SET)){
				uint8_t msgID[2];
				uint8_t msgData[16];
				uint8_t codedData[2];

				ETH_Code_UART(157, msgID);
				msgData[0] = 0x30; msgData[1] = 0x30;
				for(uint8_t i = 2; i<16;i++){
					msgData[i] = 'X';
				}
				Eth_Send_Massage(msgID, msgData);
			}
			else if(gripperClose == GPIO_PIN_SET){
				uint8_t msgID[2];
				uint8_t msgData[16];
				uint8_t codedData[2];

				ETH_Code_UART(157, msgID);
				msgData[0] = 0x30; msgData[1] = 0x31;
				for(uint8_t i = 2; i<16;i++){
					msgData[i] = 'X';
				}
				Eth_Send_Massage(msgID, msgData);
			}
			else if(gripperOpen == GPIO_PIN_SET){

				uint8_t msgID[2];
				uint8_t msgData[16];
				uint8_t codedData[2];

				ETH_Code_UART(157, msgID);
				msgData[0] = 0x30; msgData[1] = 0x32;
				for(uint8_t i = 2; i<16;i++){
					msgData[i] = 'X';
				}
				Eth_Send_Massage(msgID, msgData);
			}


			buttonOpt++;
		}
		currentReading = 4;
		maniOrMotor = 0;
	}
	else if(currentReading == 4 && maniOrMotor % 2 == 1 && ethTxLineOpen){

		 if (on == 1){
			uint8_t msgData[16] = {0x30, 0x33, 0x33, 0x32, 0x30,0x30, 0x30, 0x30,0x33,
								0x32,0x78,0x78,0x78,0x78,0x78,0x78};
			uint8_t msgID[2] = {0x43, 0x32};
		    Eth_Send_Massage(msgID, msgData);
			on = 0;
		}
		else if(off == 1){
		    uint8_t msgData[16] = {0x30, 0x33, 0x30, 0x30, 0x30,0x30,0x30,0x30,0x30,
								0x30,0x78,0x78,0x78,0x78,0x78,0x78};
			uint8_t msgID[2] = {0x43, 0x32};
			Eth_Send_Massage(msgID, msgData);
			off = 0;
		}

		currentReading = 0;
		maniOrMotor = 0;
	}

}





