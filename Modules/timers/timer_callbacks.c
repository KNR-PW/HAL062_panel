#include <stdbool.h>
#include "joystick/joystick_const.h"
#include "camera_switch/camera_switch.h"
#include "joystick/joystick.h"
#include "buttons/buttons.h"
#include "LED_switch/LED_switch.h"
#include "ethernet/ethernet.h"
#include "buttons/buttons_const.h"

extern bool receiveIsReady;
//uint8_t receiveData[24];
//extern I2C_HandleTypeDef hi2c2;
extern struct cameraSwitch yellowCamera;
extern struct cameraSwitch redCamera;
extern struct cameraSwitch blueCamera;

extern volatile uint8_t boudryButtonStates[3];

uint8_t currentButtonsRead = 0;
static uint8_t id[2] = {'3','1'};
static uint8_t data[16];


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

		ADC_Try_Read();

//		Set_LED_For_Manip_Bounds(boudryButtonStates);


		//camera switch handling
//		Check_Camera_State(yellowCamera);
//		Check_Camera_State(blueCamera);
//		Check_Camera_State(redCamera);


	}
}
