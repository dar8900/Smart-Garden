#include "TaskEeprom.h"
#include "Smart-Garden.h"
#include "Time.h"

#ifdef TASK_EEPROM
void TaskEeprom(void *pvParameters)  // This is a task.
{
	(void) pvParameters;

	for (;;)
	{
		if(FlagForSave.SaveDayTime)
		{
			FlagForSave.SaveDayTime = false;
			EEPROM.update(DAY_TIME_ADDR, SystemFlag.DayTime);
		}
		if(FlagForSave.SaveHours)
		{
			FlagForSave.SaveHours = false;
			EEPROM.update(DAY_HOUR_ADDR, DayTimeHours.DayHours);
			EEPROM.update(NIGHT_HOUR_ADDR, DayTimeHours.NightHours);
			EEPROM.update(TRANSITION_HOUR_ADDR, DayTimeHours.TransitionHours);
		}
		if(FlagForSave.SaveDimming)
		{
			FlagForSave.SaveDimming = false;
			EEPROM.update(DIMMING_ADDR ,(uint8_t)Dimming);
		}
		if(FlagForSave.SaveSecondCouter)
		{
			FlagForSave.SaveSecondCouter = false;
			EEPROM.put(SECOND_COUNTER_ADDR, SecondCounter);
		}
		if(FlagForSave.SaveCalendar)
		{
			FlagForSave.SaveCalendar = false;
			SaveTimeDate();
		}
		OsDelay(TASK_EEPROM_DELAY);
	}
}
#endif