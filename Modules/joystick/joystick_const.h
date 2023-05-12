#ifndef JOYSTICK_JOYSTICK_CONST_H
#define JOYSTICK_JOYSTICK__CONST_H

#define SLAVE_ADDRESS 0x35
#define CONFIG_DATA 0x17
#define SETUP_DATA 0x82

struct Joystick{
	uint8_t number;
	uint16_t xVal;
	uint16_t yVal;
	uint16_t zVal;
	uint16_t midVal;
	int16_t xPos;
	int16_t yPos;
	int16_t zPos;
};

#endif // JOYSTICK_JOYSTICK_CONST_H