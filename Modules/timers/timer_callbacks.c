#include <stdbool.h>
#include "joystick/joystick_const.h"
#include "camera_switch/camera_switch.h"
#include "joystick/joystick.h"
#include "buttons/buttons.h"

extern bool receiveIsReady;
//uint8_t receiveData[24];
//extern I2C_HandleTypeDef hi2c2;
extern struct cameraSwitch yellowCamera;
extern struct cameraSwitch redCamera;
extern struct cameraSwitch blueCamera;

uint8_t currentButtonsRead = 0;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM7) {
		if (receiveIsReady) {
		Jostick_Read_value_Done();
		}
	}
	if(htim->Instance == TIM6) {
		Set_LED_For_Bistable();
		Read_Camera_Switch_Value();
		Send_Cameras_State();

		//camera switch handling
//		Check_Camera_State(yellowCamera);
//		Check_Camera_State(blueCamera);
//		Check_Camera_State(redCamera);


	}
}
