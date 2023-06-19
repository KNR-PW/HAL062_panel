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

void Error_Handler(enum errorCode code) {
	switch(code){
	case ReceivedFrameError:
		continue;
	case TransmittedFrameError:
		continue;
	case ConnectionLostError:
		continue;
	case CriticalSystemError:
		continue;
	default:
		continue;
	}

	while (1) {
	}
}

void Error_Handler(void){
	while(1){

	}
}
