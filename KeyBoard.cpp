#include "KeyBoard.h"
#include "Smart-Garden.h"

#define LONG_PRESS_DELAY	2000
#define LONG_PRESS_ON    	true
#define LONG_PRESS_OFF    	false

uint8_t ButtonPress;
bool ActiveButton[3];
bool LongPress[3];
uint16_t ActualMillis[3], StartMillis[3];

static void ButtonPressManagement(uint8_t Pin, uint8_t ButtonPressIndex, bool EnableLongPress)
{
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
	else
	{
		if(digitalRead(Pin))
		{
			if(ButtonPress == NO_PRESS)
				ButtonPress = ButtonPressIndex;
		}
	}
}
void CheckButtons()
{
	ButtonPressManagement(UP_BUTTON, UP, LONG_PRESS_OFF);
	ButtonPressManagement(DOWN_BUTTON, DOWN, LONG_PRESS_OFF);
	ButtonPressManagement(OK_BUTTON, OK, LONG_PRESS_ON);
}