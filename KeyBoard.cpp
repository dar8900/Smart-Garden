#include "KeyBoard.h"
#include "Smart-Garden.h"

uint8_t ButtonPress;

void CheckButtons()
{
	if(digitalRead(UP_BUTTON))
		ButtonPress = UP;
	else if(digitalRead(DOWN_BUTTON))
		ButtonPress = DOWN;
	else if(digitalRead(OK_BUTTON))
		ButtonPress = OK;
	else
		ButtonPress = NO_PRESS;
}