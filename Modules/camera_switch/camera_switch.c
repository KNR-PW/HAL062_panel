#include "camera_switch.h"
#include "buttons/buttons_const.h"
#include "error_handlers/error_handlers.h"
#include "ethernet/ethernet.h"
#include "LED_switch/LED_switch.h"
#include "LED_switch/LED_const.h"

struct cameraSwitch yellowCamera = {1,0,0,0,0};
struct cameraSwitch blueCamera = {2,0,0,0,0};
struct cameraSwitch redCamera = {3,0,0,0,0};

uint8_t cameraMsgID[2] = {4,1};
uint8_t cameraMsgData[16] = {1,0,0,0,1,0,0,0,1,0,0,0,'x','x','x','x'};

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

void Read_Camera_Switch_Value(void){

	yellowCamera.channel_A = HAL_GPIO_ReadPin(CAM_SWITCH_1_A_Pin, CAM_SWITCH_1_A_GPIO_Port);
	yellowCamera.channel_B = HAL_GPIO_ReadPin(CAM_SWITCH_1_B_Pin, CAM_SWITCH_1_B_GPIO_Port);
	yellowCamera.channel_C = HAL_GPIO_ReadPin(CAM_SWITCH_1_C_Pin, CAM_SWITCH_1_C_GPIO_Port);
	yellowCamera.channel_D = HAL_GPIO_ReadPin(CAM_SWITCH_1_D_Pin, CAM_SWITCH_1_D_GPIO_Port);

	blueCamera.channel_A = HAL_GPIO_ReadPin(CAM_SWITCH_2_A_Pin, CAM_SWITCH_2_A_GPIO_Port);
	blueCamera.channel_B = HAL_GPIO_ReadPin(CAM_SWITCH_2_B_Pin, CAM_SWITCH_2_B_GPIO_Port);
	blueCamera.channel_C = HAL_GPIO_ReadPin(CAM_SWITCH_2_C_Pin, CAM_SWITCH_2_C_GPIO_Port);
	blueCamera.channel_D = HAL_GPIO_ReadPin(CAM_SWITCH_2_D_Pin, CAM_SWITCH_2_D_GPIO_Port);

	redCamera.channel_A = HAL_GPIO_ReadPin(CAM_SWITCH_3_A_Pin, CAM_SWITCH_3_A_GPIO_Port);
	redCamera.channel_B = HAL_GPIO_ReadPin(CAM_SWITCH_3_B_Pin, CAM_SWITCH_3_B_GPIO_Port);
	redCamera.channel_C = HAL_GPIO_ReadPin(CAM_SWITCH_3_C_Pin, CAM_SWITCH_3_C_GPIO_Port);
	redCamera.channel_D = HAL_GPIO_ReadPin(CAM_SWITCH_3_D_Pin, CAM_SWITCH_3_D_GPIO_Port);

}

void Send_Cameras_State(void){

	cameraMsgData[0] = (uint8_t)yellowCamera.channel_A;
	cameraMsgData[1] = (uint8_t)yellowCamera.channel_B;
	cameraMsgData[2] = (uint8_t)yellowCamera.channel_C;
	cameraMsgData[3] = (uint8_t)yellowCamera.channel_D;
	cameraMsgData[4] = (uint8_t)blueCamera.channel_A;
	cameraMsgData[5] = (uint8_t)blueCamera.channel_B;
	cameraMsgData[6] = (uint8_t)blueCamera.channel_C;
	cameraMsgData[7] = (uint8_t)blueCamera.channel_D;
	cameraMsgData[8] = (uint8_t)redCamera.channel_A;
	cameraMsgData[9] = (uint8_t)redCamera.channel_B;
	cameraMsgData[10] = (uint8_t)redCamera.channel_C;
	cameraMsgData[11] = (uint8_t)redCamera.channel_D;
	cameraMsgData[12] = (uint8_t)'x';
	cameraMsgData[13] = (uint8_t)'x';
	cameraMsgData[14] = (uint8_t)'x';
	cameraMsgData[15] = (uint8_t)'x';

	Eth_Send_Massage(cameraMsgID, cameraMsgData);

}

void Set_Camera_LED(void){
	LED_Set(LIGHT7, yellowCamera.channel_B);
	LED_Set(LIGHT8, yellowCamera.channel_C);
	LED_Set(LIGHT9, yellowCamera.channel_D);

	LED_Set(LIGHT10, blueCamera.channel_B);
	LED_Set(LIGHT11, blueCamera.channel_C);
	LED_Set(LIGHT12, blueCamera.channel_D);

	LED_Set(LIGHT13, redCamera.channel_B);
	LED_Set(LIGHT14, redCamera.channel_C);
	LED_Set(LIGHT15, redCamera.channel_D);
}