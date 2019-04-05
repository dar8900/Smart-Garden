#include "TaskEeprom.h"
#include "Smart-Garden.h"
#include "Time.h"
#include "EepromAddr.h"
#include <EEPROM.h>


uint32_t NumberOfWrites;
static bool WriteInMemory = false;

void TaskEeprom(void)  
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
	if(FlagForSave.SaveSecondDimming)
	{
		FlagForSave.SaveSecondDimming = false;
		EEPROM.put(SECOND_DIMMING_ADDR, SecondForDimming);
		WriteInMemory = true;
		NumberOfWrites++;
	}
	if(FlagForSave.ClearSecondCounter)
	{
		FlagForSave.ClearSecondCounter = false;
		EEPROM.put(SECOND_COUNTER_ADDR, 0);
		WriteInMemory = true;
		NumberOfWrites++;		
	}
	if(WriteInMemory)
	{
		WriteInMemory = false;
		DBG("Task Eeprom-> numero scritture in memoria: ");
		DBG(NumberOfWrites);
		EEPROM.put(NUMBER_OF_WRITES_ADDR, NumberOfWrites);
	}
}
