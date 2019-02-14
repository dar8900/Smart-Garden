#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <Arduino.h>

typedef enum
{
	UP = 0,
	DOWN,
	OK,
	NO_PRESS
}KEYBOARD_BUTTONS;

extern uint8_t ButtonPress;
void CheckButtons(void);

#endif