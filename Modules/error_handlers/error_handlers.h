/**
 ******************************************************************************
 * @file           error_handlers.h
 * @author         K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          Error handlers - headers file
 ******************************************************************************
 */

#ifndef ERROR_HANDLERS_ERROR_HANDLERS_H_
#define ERROR_HANDLERS_ERROR_HANDLERS_H_

enum errorCode{
	ReceivedFrameError = 0,	/*!< Raises error of unproperly constructed frame*/
	TransmittedFrameError = 1, /*!< Raises error of unproperly construced frame to send*/
	ConnectionLostError = 2, /*!< Raises error of lost connection*/
	CriticalSystemError = 3 /*!< Raises error of any other system malfunction*/
};

void Error_Handler(void);

#endif /* ERROR_HANDLERS_ERROR_HANDLERS_H_ */
