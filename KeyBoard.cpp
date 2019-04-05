#include "KeyBoard.h"
#include "Smart-Garden.h"

#define LONG_PRESS_DELAY	2000
#define LONG_PRESS_ON    	true
#define LONG_PRESS_OFF     false

#define N_BUTTONS	           3


uint8_t CheckButtons()
{
	uint8_t ButtonPress = NO_PRESS;
	if(digitalRead(UP_BUTTON))
		ButtonPress = UP;
	else if(digitalRead(DOWN_BUTTON))
		ButtonPress = DOWN;
	else if(digitalRead(OK_BUTTON))
		ButtonPress = OK;
	else 
		ButtonPress = NO_PRESS;
	if(ButtonPress != NO_PRESS)
		delay(100);
	return ButtonPress;
}