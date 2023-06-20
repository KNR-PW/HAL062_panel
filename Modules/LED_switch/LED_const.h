/**
 ******************************************************************************************
 * @file           LED_const.h
 * @author         K. Czechowicz, A. Rybojad, S. Kołodziejczyk
 * @brief          File containing macro definitions for all GPIO outputs at GPIO expander
 * @details
 * 				This file contains macro definitions for all GPIO outputs at GPIO
 * 				expander module. The light is defined according to the description
 * 				on PCB board (LIGHT1, LIGHT2, etc.) and is associated with slave address
 * 				of an proper GPIO pin.
 * 				There are two devices, which have addresses 0x40 and 0x42. The devices have
 * 				ports A and B, which respectively have addresses 0x12 and 0x13. Finally,
 * 				pins 0-7 at each port at each device have addresses from range 0x01 to 0x80
 * 				The construction of the macro is following:
 * 				@verbatim
 * 				device_address	 port     pin_number
 * 				[	8 bits  ] [ 8 bits ] [ 8 bits ]
 * 				@endverbatim
 * 
 * @see			MCP23017 GPIO expander datasheet
 *******************************************************************************************
 */

#ifndef LED_CONST_LED_CONST_H_
#define LED_CONST_LED_CONST_H_


//dev 0x40 , port A, pins 0-7
#define LIGHT1 	0x401201
#define LIGHT2 	0x401202
#define LIGHT3 	0x401204
#define LIGHT4 	0x401208
#define LIGHT5 	0x401210
#define LIGHT6 	0x401220
#define LIGHT7 	0x401240
#define LIGHT8 	0x401280


//dev 0x40 , port B, pins 0-7
#define LIGHT9 	0x401301
#define LIGHT10 0x401302
#define LIGHT11 0x401304
#define LIGHT12 0x401308
#define LIGHT13 0x401310
#define LIGHT14 0x401320
#define LIGHT15 0x401340
#define LIGHT16 0x401380


//dev 0x42 , port A, pins 0-7
#define LIGHT17 0x421201
#define LIGHT18 0x421202
#define LIGHT19 0x421204
#define LIGHT20 0x421208
#define LIGHT21 0x421210
#define LIGHT22 0x421220
#define LIGHT23 0x421240
#define LIGHT24 0x421280


//dev 0x42 , port B, pins 0-7
#define LIGHT25 0x401301
#define LIGHT26 0x401302
#define LIGHT27 0x401304
#define LIGHT28 0x401308
#define LIGHT29 0x401310
#define LIGHT30 0x401320
#define LIGHT31 0x401340
#define LIGHT32 0x401380

// defining devices and ports
#define DEV_1 	0x40
#define DEV_2	0x42
#define PORT_A  0x12
#define PORT_B	0x13


/**
 * @brief This structure is used to remember the state of LED light
 * 		  after any action of even system restart. It saves the state of the whole port
 * 		  which is then logically added to the given state of a given LED light in LED_Set()
 * 		  function.
 * 
 * @see LED_Set() documentation
*/
struct currentLEDstate{
	uint8_t dev1portA;
	uint8_t dev1portB;
	uint8_t dev2portA;
	uint8_t dev2portB;
};

#endif //   LED_CONST_LED_CONST_H_
