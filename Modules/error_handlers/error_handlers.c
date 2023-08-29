/**
 ******************************************************************************
 * @file           error_handlers.c
 * @author         K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          Error handlers - functionality
 ******************************************************************************
 */

#include <stm32h7xx_hal.h>
#include "error_handlers/error_handlers.h"
#include "LED_switch/LED_switch.h"

/**
 * @brief Function for handling specific errors, registered during system performance.
 * @details
 * 			After entering Error_Handler() function, the system shall register the type of
 * 			an error and afterwards enter infinite loop, which with an use of watchdog module
 * 			will cause the system to reboot.
 * @param code specifies type of an error, which causes proper error handling
 * @returns void
 */

//void Error_Handler(enum errorCode code) {
//	switch(code){
//	case ReceivedFrameError:
//		continue;
//	case TransmittedFrameError:
//		continue;
//	case ConnectionLostError:
//		continue;
//	case CriticalSystemError:
//		continue;
//	default:
//		continue;
//	}
//
//	while (1) {
//	}
//}

/**
 * @brief Function for handling general errors.
 * @details
 * 			After entering Error_Handler() function, the system shall enter infinite loop, 
 * 			which with an use of watchdog module will cause the system to reboot.
 * @param void
 * @returns void
 */
void Error_Handler(void){
	while(1){

	}
}
