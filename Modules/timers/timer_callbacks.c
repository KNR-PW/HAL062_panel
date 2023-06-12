#include <stdbool.h>
#include "joystick/joystick_const.h"
#include "joystick/joystick.h"
#include "camera_switch/camera_switch.h"

extern bool receiveIsReady;
extern uint8_t receiveData[8];
extern I2C_HandleTypeDef hi2c2;
extern struct cameraSwitch yellowCamera;
extern struct cameraSwitch redCamera;
extern struct cameraSwitch blueCamera;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM7) {
		if (receiveIsReady) {
			receiveIsReady = false;
			uint16_t address = (SLAVE_ADDRESS << 1) | 0x01;
			HAL_I2C_Master_Receive_IT(&hi2c2, address, receiveData, 8);

		}
	}
	if(htim->Instance == TIM6) {

		//camera switch handling
		Read_Camera_Switch_Value();
		Check_Camera_State(yellowCamera);
		Check_Camera_State(blueCamera);
		Check_Camera_State(redCamera);
		Set_Camera_LED();
		Send_Cameras_State();

	}
}
