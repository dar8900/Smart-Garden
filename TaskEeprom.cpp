#include "TaskEeprom.h"
#include "Smart-Garden.h"
#include "Time.h"

#ifdef TASK_EEPROM

uint32_t NumberOfWrites;

void TaskEeprom(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	
	bool WriteInMemory = false; 
	if(EEPROM.read(FIRST_GO_ADDR) == 0)
		EEPROM.get(NUMBER_OF_WRITES_ADDR, NumberOfWrites);
	
	for (;;)
	{
		if(FlagForSave.SaveDayTime)
		{
			FlagForSave.SaveDayTime = false;
			EEPROM.update(DAY_TIME_ADDR, SystemFlag.DayTime);
			WriteInMemory = true;
			NumberOfWrites++;
		}
		if(FlagForSave.SaveHours)
		{
			FlagForSave.SaveHours = false;
			EEPROM.update(DAY_HOUR_ADDR, DayTimeHours.DayHours);
			EEPROM.update(NIGHT_HOUR_ADDR, DayTimeHours.NightHours);
			EEPROM.update(TRANSITION_HOUR_ADDR, DayTimeHours.TransitionHours);
			WriteInMemory = true;
			NumberOfWrites++;
		}
		if(FlagForSave.SaveDimming)
		{
			FlagForSave.SaveDimming = false;
			EEPROM.update(DIMMING_ADDR ,(uint8_t)Dimming);
			WriteInMemory = true;
			NumberOfWrites++;
		}
		if(FlagForSave.SaveSecondCouter)
		{
			FlagForSave.SaveSecondCouter = false;
			EEPROM.put(SECOND_COUNTER_ADDR, SecondCounter);
			WriteInMemory = true;
			NumberOfWrites++;
		}
		// if(FlagForSave.SaveCalendar)
		// {
			// FlagForSave.SaveCalendar = false;
			// SaveTimeDate();
		// }
		if(WriteInMemory)
		{
			WriteInMemory = false;
			DBG("Task Eeprom-> numero scritture in memoria: " + String(NumberOfWrites));
			EEPROM.put(NUMBER_OF_WRITES_ADDR, NumberOfWrites);
		}
		OsDelay(TASK_EEPROM_DELAY);
	}
}
#endif