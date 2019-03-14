#include "TaskDimming.h"
#include "Smart-Garden.h"
#include "Time.h"

int16_t Dimming = 0;

#ifdef TASK_DIMMING

static void GardenLight()
{
	if(!SystemFlag.BypassNormalDimming)
	{
		switch(SystemFlag.DayTime)
		{
			case IN_DAY:
				if(SystemFlag.RefreshDimming)
				{
					Dimming = 255;
					analogWrite(DIMMING_LED, Dimming);
					SystemFlag.RefreshDimming = false;
				}
				break;
			case IN_NIGHT:
				if(SystemFlag.RefreshDimming)
				{
					Dimming = 0;
					analogWrite(DIMMING_LED, Dimming);
					SystemFlag.RefreshDimming = false;
				}
				break;
			case TO_NIGHT:
				if(SecondCounter % SecondForDimming)
				{
					analogWrite(DIMMING_LED, Dimming--);
					if(Dimming < 0)
						Dimming = 0;
				}	
				break;
			case TO_DAY:
				if(SecondCounter % SecondForDimming)
				{
					analogWrite(DIMMING_LED, Dimming++);
					if(Dimming > 255)
						Dimming = 255;
				}
				break;
			default:
				break;			
		}
	}
	else
	{
		if(SystemFlag.RefreshDimming)
		{
			analogWrite(DIMMING_LED, Dimming);		
			SystemFlag.RefreshDimming = false;
		}
	}	
}





void TaskDimmingLed(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	for (;;)
	{
		GardenLight();
		OsDelay(TASK_DIMMING_DELAY);
	}
}
#endif