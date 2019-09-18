#include "KeyBoard.h"
#include "Smart-Garden.h"

#define LONG_PRESS_DELAY	2000
#define LONG_PRESS_ON    	true
#define LONG_PRESS_OFF     false

#define N_BUTTONS	           3

// uint8_t ButtonPress;
bool ActiveButton[N_BUTTONS];
bool LongPress[N_BUTTONS];
uint16_t StartMillis[N_BUTTONS];
// uint16_t ActualMillis[N_BUTTONS];

static uint8_t ButtonPressManagement(uint8_t Pin, uint8_t ButtonPressIndex, bool EnableLongPress)
{
	uint8_t ButtonPress = NO_PRESS;
	if(EnableLongPress)
	{
		if(digitalRead(Pin))
		{
			if(ButtonPress == NO_PRESS)
			{
				if(!ActiveButton[ButtonPressIndex])
				{
					ActiveButton[ButtonPressIndex] = true;
					StartMillis[ButtonPressIndex] = millis();
				}
				if(ActiveButton[ButtonPressIndex])
				{
					// ActualMillis[ButtonPressIndex] = millis();
					if(millis() - StartMillis[ButtonPressIndex] >= LONG_PRESS_DELAY)
					{
						LongPress[ButtonPressIndex] = true;
						ButtonPress = ButtonPressIndex + N_BUTTONS;
					}
				}
			}
		}
		else
		{
			if(!LongPress[ButtonPressIndex] && ActiveButton[ButtonPressIndex])
			{
				ButtonPress = ButtonPressIndex;
				ActiveButton[ButtonPressIndex] = false;
			}
			else
			{
				// ActualMillis[ButtonPressIndex] = 0;
				StartMillis[ButtonPressIndex] = 0;
				ActiveButton[ButtonPressIndex] = false;
				LongPress[ButtonPressIndex] = false;
			}
		}
	}
	else
	{
		if(digitalRead(Pin))
		{
			ButtonPress = ButtonPressIndex;
			if(ButtonPress != NO_PRESS)
				delay(100);
		}
	}
	
	return ButtonPress;
}

uint8_t CheckButtons()
{
	uint8_t ButtonPress = NO_PRESS;
	ButtonPress = ButtonPressManagement(UP_BUTTON, UP, LONG_PRESS_OFF);
	if(ButtonPress != NO_PRESS)
		return ButtonPress;
	ButtonPress = ButtonPressManagement(DOWN_BUTTON, DOWN, LONG_PRESS_OFF);
	if(ButtonPress != NO_PRESS)
		return ButtonPress;
	ButtonPress = ButtonPressManagement(OK_BUTTON, OK, LONG_PRESS_OFF);
	if(ButtonPress != NO_PRESS)
		return ButtonPress;
	ButtonPress = ButtonPressManagement(RESET_EXIT_BUTTON, RESET_EXIT, LONG_PRESS_OFF);
	if(ButtonPress != NO_PRESS)
		return ButtonPress;
	// if(digitalRead(UP_BUTTON))
		// ButtonPress = UP;
	// else if(digitalRead(DOWN_BUTTON))
		// ButtonPress = DOWN;
	// else if(digitalRead(OK_BUTTON))
		// ButtonPress = OK;
	// else if(digitalRead(RESET_EXIT_BUTTON))
		// ButtonPress = RESET_EXIT;
	// else 
		// ButtonPress = NO_PRESS;
	// if(ButtonPress != NO_PRESS)
		// delay(100);
	return ButtonPress;
}