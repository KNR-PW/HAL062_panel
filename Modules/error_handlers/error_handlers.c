/**
 ******************************************************************************
 * @file           : error_handlers.c
 * @author         : K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          : Error handlers - functionality
 ******************************************************************************
 */

#include <stm32h7xx_hal.h>
#include "error_handlers/error_handlers.h"
#include "LED_switch/LED_switch.h"

#define ERROR_CODE_ADDRESS ((uint32_t)0x0800F000)
#define FLASH_TYPEPROGRAM_WORD ((uint32_t)0x02U)  /*!< Program a word (32-bit) at a specified address*/

void Error_Handler(enum errorCode code) {
	/// TODO: FInish Error handler!
	switch(code){
	case ReceivedFrameError:
		HAL_FLASH_Unlock();
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, ERROR_CODE_ADDRESS, (uint32_t)code);
		HAL_FLASH_Lock();
		while(1){}
		break;
//		continue;
	case TransmittedFrameError:
//		continue;
		while(1){}
		break;
	case ConnectionLostError:
//		continue;
		while(1){}
		break;
	case CriticalSystemError:
//		continue;
		while(1){}
		break;
	default:
		while(1){}
	}
}
//
//	while (1) {
//	}
//}

//void Error_Handler(void){
//	while(1){
//
//	}
//}
