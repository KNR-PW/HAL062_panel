/**
 *************************************************************************************
 * @file           : joystick.c
 * @author         : K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          : Joystick module - implementing functionality and initialization
 *************************************************************************************
 */

//INCLUDATION:
#include "joystick.h"
#include "error_handlers/error_handlers.h"
#include "joystick_const.h"
//#include "timers/joystick_timer.h"
#include "ethernet/ethernet.h"
#include <stdbool.h>

//VARIABLES DEFINITIONS:
I2C_HandleTypeDef hi2c2;

bool receiveIsReady = false;
bool joyInitFinished = false;
static uint8_t receiveData[24];
struct Joystick motorJoy;
struct Joystick manipJoy;
struct Joystick gripperJoy;

static uint8_t currentReading = 0;
//static uint8_t msgData;
extern bool ethTxLineOpen;

//FUNCTIONS DEFINITIONS:
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

void Joystick_Write_Conditions(void) {
	uint8_t data[2];
	data[0] = CONFIG_DATA;
	data[1] = SETUP_DATA;
	uint16_t address = SLAVE_ADDRESS << 1;
	HAL_StatusTypeDef test = HAL_I2C_Master_Transmit_IT(&hi2c2, address, data, 2);
	HAL_Delay(500);
}

void Joystick_Read_Value_Start(void) {
	receiveIsReady = true;
}

void Jostick_Read_value_Done(void){
		receiveIsReady = false;
		uint16_t address = (SLAVE_ADDRESS << 1) | 0x01;
		HAL_I2C_Master_Receive_IT(&hi2c2, address, receiveData, 24);
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
	if (hi2c->Instance == I2C2) {
	joyInitFinished = true;
	Joystick_Read_Value_Start();
	}
}

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
		motorJoy.xPos = ((int16_t) motorJoy.midVal - (int16_t) motorJoy.xVal)
				/ 20;
		motorJoy.yPos = ((int16_t) motorJoy.midVal - (int16_t) motorJoy.yVal)
				/ 20;

		manipJoy.xPos = ((int16_t) manipJoy.midVal - (int16_t) manipJoy.xVal)
				/ 20;
		manipJoy.yPos = ((int16_t) manipJoy.midVal - (int16_t) manipJoy.yVal)
				/ 20;
		manipJoy.zPos = ((int16_t) manipJoy.midVal - (int16_t) manipJoy.zVal)
				/ 20;

		gripperJoy.yPos = ((int16_t) gripperJoy.midVal
				- (int16_t) gripperJoy.yVal) / 20;
		gripperJoy.xPos = ((int16_t) gripperJoy.midVal
				- (int16_t) gripperJoy.xVal) / 20;
		gripperJoy.zPos = ((int16_t) gripperJoy.midVal
				- (int16_t) gripperJoy.zVal) / 20;


		//sending joysticks' readings
		Joystick_Send_Readings();

		//enabling receiving again
		receiveIsReady = true;
	}

}

void Joystick_Send_Readings(void) {
	//sending info with speed
	uint8_t id[2] = { 0x50, 0x49 };

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

	// building frame
	if(currentReading == 0 && ethTxLineOpen){
	uint8_t msgData[16] = { (uint8_t)rightSpeed, (uint8_t)rightSpeed,
			(uint8_t) rightSpeed, (uint8_t)leftSpeed, (uint8_t)leftSpeed,
			(uint8_t)leftSpeed, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78,
			0x78 };
//	Eth_Send_Massage(id, msgData);
	currentReading = 1;
	}
	else if(currentReading == 1 && ethTxLineOpen){
	uint8_t msgData[16] = {(uint8_t) motorJoy.xPos,(uint8_t) motorJoy.yPos,(uint8_t) motorJoy.zPos,0x78,0x78,0x78,0x78,0x78,
			0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78};
	Eth_Send_Massage(id, msgData);
	currentReading = 2;
	}
	else if(currentReading == 2 && ethTxLineOpen){
		uint8_t msgData[16] = {0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,
				0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78};
//		Eth_Send_Massage(id, msgData);
		currentReading = 0;
	}

}



