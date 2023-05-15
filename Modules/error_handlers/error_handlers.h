/**
 ******************************************************************************
 * @file           : error_handlers.h
 * @author         : K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          : Error handlers - headers file
 ******************************************************************************
 */

#ifndef ERROR_HANDLERS_ERROR_HANDLERS_H_
#define ERROR_HANDLERS_ERROR_HANDLERS_H_

enum errorCode{
	ReceivedFrameError = 0,
	TransmittedFrameError = 1,
	ConnectionLostError = 2,
	CriticalSystemError = 3
};

void Error_Handler(enum errorCode code);

#endif /* ERROR_HANDLERS_ERROR_HANDLERS_H_ */
