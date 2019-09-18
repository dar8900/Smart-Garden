#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <Arduino.h>

typedef enum
{
	UP = 0,
	DOWN,
	OK,
	RESET_EXIT,
	LONG_UP,
	LONG_DOWN,
	LONG_OK,
	LONG_RESET_EXIT,
	NO_PRESS
}KEYBOARD_BUTTONS;

uint8_t CheckButtons(void);

#endif