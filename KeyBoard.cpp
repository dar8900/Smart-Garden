#include "KeyBoard.h"
#include "Smart-Garden.h"

#define LONG_PRESS_DELAY	2000

uint8_t ButtonPress;
bool ActiveButton[3];
bool LongPress[3];
uint16_t ActualMillis[3], StartMillis[3];

static void ButtonPressManagement(uint8_t Pin, uint8_t ButtonPressIndex)
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
				ActualMillis[ButtonPressIndex] = millis();
				if(ActualMillis[ButtonPressIndex] - StartMillis[ButtonPressIndex] > LONG_PRESS_DELAY)
				{
					LongPress[ButtonPressIndex] = true;
					ButtonPress = ButtonPressIndex + 3;
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
			ActualMillis[ButtonPressIndex] = 0;
			StartMillis[ButtonPressIndex] = 0;
			ActiveButton[ButtonPressIndex] = false;
			LongPress[ButtonPressIndex] = false;
		}
	}
}
void CheckButtons()
{
	ButtonPressManagement(UP_BUTTON, UP);
	ButtonPressManagement(DOWN_BUTTON, DOWN);
	ButtonPressManagement(OK_BUTTON, OK);
	// if(digitalRead(UP_BUTTON))
		// ButtonPress = UP;
	// else if(digitalRead(DOWN_BUTTON))
		// ButtonPress = DOWN;
	// else if(digitalRead(OK_BUTTON))
		// ButtonPress = OK;
	// else
		// ButtonPress = NO_PRESS;
}