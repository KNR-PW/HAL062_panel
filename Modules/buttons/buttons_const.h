/**
 ******************************************************************************
 * @file           buttons_const.h
 * @author         K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          This a file containing macro definitions for GPIO inputs and outputs
 *
 ******************************************************************************
 * @details
 *                 This file consists of macro definitions for all modules using GPIO pin, which are
 *                 buttons, lights and camera switches, each specyfing the GPIO port and GPIO pin number
 *                 This is used for easier decoding, better readability of the code. 
 ******************************************************************************    
 *
 */

#ifndef BUTTONS_BUTTONS_CONST_H_
#define BUTTONS_BUTTONS_CONST_H_

/* Includes --------------------------------------------------------- */
#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_gpio.h>

#define MONO_BUTTON_JOY_RED_Pin GPIO_PIN_4 // IN9
#define MONO_BUTTON_JOY_RED_GPIO_Port GPIOD
#define MONO_BUTTON_JOY_RED_EXTI_IRQn EXTI4_IRQn
#define MONO_BUTTON_JOY_BLUE_Pin GPIO_PIN_5 // IN 10
#define MONO_BUTTON_JOY_BLUE_GPIO_Port GPIOD
#define MONO_BUTTON_JOY_BLUE_EXTI_IRQn EXTI9_5_IRQn
#define MONO_BUTTON_JOY_GREEN_Pin GPIO_PIN_6 // IN 11
#define MONO_BUTTON_JOY_GREEN_GPIO_Port GPIOD
#define MONO_BUTTON_JOY_GREEN_EXTI_IRQn EXTI9_5_IRQn
#define MONO_BUTTON_RED_1_Pin GPIO_PIN_7 // IN 12
#define MONO_BUTTON_RED_1_GPIO_Port GPIOD
#define MONO_BUTTON_RED_1_EXTI_IRQn EXTI9_5_IRQn
#define MONO_BUTTON_BLACK_1_Pin GPIO_PIN_9 // IN 13
#define MONO_BUTTON_BLACK_1_GPIO_Port GPIOG
#define MONO_BUTTON_BLACK_1_EXTI_IRQn EXTI9_5_IRQn
#define MONO_BUTTON_GREEN_1_Pin GPIO_PIN_10 // IN 14
#define MONO_BUTTON_GREEN_1_GPIO_Port GPIOG
#define MONO_BUTTON_GREEN_1_EXTI_IRQn EXTI15_10_IRQn
#define MONO_BUTTON_BLUE_1_Pin GPIO_PIN_11 // IN 15
#define MONO_BUTTON_BLUE_1_GPIO_Port GPIOG
#define MONO_BUTTON_BLUE_1_EXTI_IRQn EXTI15_10_IRQn
#define MONO_BUTTON_RED_2_Pin GPIO_PIN_12 // IN 16
#define MONO_BUTTON_RED_2_GPIO_Port GPIOG
#define MONO_BUTTON_RED_2_EXTI_IRQn EXTI15_10_IRQn
#define MONO_BUTTON_BLACK_2_Pin GPIO_PIN_13 // IN 17
#define MONO_BUTTON_BLACK_2_GPIO_Port GPIOG
#define MONO_BUTTON_BLACK_2_EXTI_IRQn EXTI15_10_IRQn
#define MONO_BUTTON_GREEN_2_Pin GPIO_PIN_14 // IN 18
#define MONO_BUTTON_GREEN_2_GPIO_Port GPIOG
#define MONO_BUTTON_GREEN_2_EXTI_IRQn EXTI15_10_IRQn
#define MONO_BUTTON_BLUE_2_Pin GPIO_PIN_15 // IN 19
#define MONO_BUTTON_BLUE_2_GPIO_Port GPIOG
#define MONO_BUTTON_BLUE_2_EXTI_IRQn EXTI15_10_IRQn

#define BI_BUTTON_RED_1_Pin GPIO_PIN_3 // IN 20
#define BI_BUTTON_RED_1_GPIO_Port GPIOB
#define BI_BUTTON_RED_2_Pin GPIO_PIN_4 // IN 21
#define BI_BUTTON_RED_2_GPIO_Port GPIOB
#define BI_BUTTON_BLUE_1_Pin GPIO_PIN_5 // IN 22
#define BI_BUTTON_BLUE_1_GPIO_Port GPIOB
#define BI_BUTTON_BLUE_2_Pin GPIO_PIN_6 // IN 23
#define BI_BUTTON_BLUE_2_GPIO_Port GPIOB
#define BI_BUTTON_BLUE_3_Pin GPIO_PIN_7 // IN 24
#define BI_BUTTON_BLUE_3_GPIO_Port GPIOB
#define BI_BUTTON_GREEN_1_Pin GPIO_PIN_0 // IN 25
#define BI_BUTTON_GREEN_1_GPIO_Port GPIOE
#define BI_BUTTON_GREEN_2_Pin GPIO_PIN_1 // IN 26
#define BI_BUTTON_GREEN_2_GPIO_Port GPIOE
#define BI_BUTTON_GREEN_3_Pin GPIO_PIN_2 // IN 27
#define BI_BUTTON_GREEN_3_GPIO_Port GPIOE

#define CAM_SWITCH_3_A_Pin GPIO_PIN_15
#define CAM_SWITCH_3_A_GPIO_Port GPIOB
#define CAM_SWITCH_3_B_Pin GPIO_PIN_14
#define CAM_SWITCH_3_B_GPIO_Port GPIOD
#define CAM_SWITCH_3_C_Pin GPIO_PIN_15
#define CAM_SWITCH_3_C_GPIO_Port GPIOD
#define CAM_SWITCH_3_D_Pin GPIO_PIN_2
#define CAM_SWITCH_3_D_GPIO_Port GPIOG
#define CAM_SWITCH_2_A_Pin GPIO_PIN_3
#define CAM_SWITCH_2_A_GPIO_Port GPIOG
#define CAM_SWITCH_2_B_Pin GPIO_PIN_4
#define CAM_SWITCH_2_B_GPIO_Port GPIOG
#define CAM_SWITCH_2_C_Pin GPIO_PIN_5
#define CAM_SWITCH_2_C_GPIO_Port GPIOG
#define CAM_SWITCH_2_D_Pin GPIO_PIN_6
#define CAM_SWITCH_2_D_GPIO_Port GPIOG
#define CAM_SWITCH_1_A_Pin GPIO_PIN_7
#define CAM_SWITCH_1_A_GPIO_Port GPIOG
#define CAM_SWITCH_1_B_Pin GPIO_PIN_8
#define CAM_SWITCH_1_B_GPIO_Port GPIOG
#define CAM_SWITCH_1_C_Pin GPIO_PIN_8
#define CAM_SWITCH_1_C_GPIO_Port GPIOC
#define CAM_SWITCH_1_D_Pin GPIO_PIN_9
#define CAM_SWITCH_1_D_GPIO_Port GPIOC

#define MOTOR_LAUNCHER GPIO_PIN_6 // IN41
#define MOTOR_LAUNCHER_GPIO_Port GPIOF
#define AUTONOMY_LAUNCHER GPIO_PIN_7 // IN42
#define AUTONOMY_LAUNCHER_GPIO_Port GPIOF
#define SWITCH_12V3 GPIO_PIN_8 // IN43
#define SWITCH_12V4 GPIO_PIN_9 // IN44
#define SWITCH_12V5 GPIO_PIN_10 // IN45
#define SWITCH_12V_3_5_GPIO_Port GPIOF


#endif //BUTTONS_BUTTONS_CONST_H_
