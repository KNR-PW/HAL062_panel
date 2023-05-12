
//dev 0x40 , port A, pins 0-7
#define LIGHT1 0x401201
#define LIGHT2 0x401202
#define LIGHT3 0x401204
#define LIGHT4 0x401208
#define LIGHT5 0x401210
#define LIGHT6 0x401220
#define LIGHT7 0x401240
#define LIGHT8 0x401280


//dev 0x40 , port B, pins 0-7
#define LIGHT9 0x401301
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

struct currentLEDstate{
	uint8_t dev40portA;
	uint8_t dev40portB;
	uint8_t dev42portA;
	uint8_t dev42portB;
};
