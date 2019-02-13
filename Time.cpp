#include "Time.h"
#include "Smart-Garden.h"

#define MINUTE_TO_LOG	10

typedef enum
{
	DAY_HOURS = 12,
	NIGHT_HOURS = 8,
	TRANSITION_HOURS = 2
}HOURS;

uint32_t SecondCounter;
static uint32_t LogSecondCounter;
uint32_t ActualMillis, PreviousMillis, Delay = 1000;
uint16_t SecondForDimming = (SECONDS_HOUR(TRANSITION_HOURS) / 255);

void LogFlag()
{
	EEPROM.update(IN_DAY_ADDR,   SystemFlag.InDay);
	EEPROM.update(IN_NIGHT_ADDR, SystemFlag.InNight);
	EEPROM.update(TO_DAY_ADDR,   SystemFlag.ToDay);
	EEPROM.update(TO_NIGHT_ADDR, SystemFlag.ToNight);
}


void CheckTime()
{
	ActualMillis = millis();
	if(ActualMillis - PreviousMillis >= Delay)
	{
		PreviousMillis = ActualMillis;
		SecondCounter++;
		LogSecondCounter++;
	}
	if(SystemFlag.InDay)
	{
		if(SecondCounter == SECONDS_HOUR(DAY_HOURS))
		{
			SecondCounter = 0;
			SystemFlag.InDay = false;
			SystemFlag.ToNight = true;
		}
	}
	if(SystemFlag.ToNight)
	{
		if(SecondCounter == SECONDS_HOUR(TRANSITION_HOURS))
		{
			SecondCounter = 0;
			SystemFlag.ToNight = false;
			SystemFlag.InNight = true;
			SystemFlag.RefreshDimming = true;
		}		
	}
	if(SystemFlag.InNight)
	{
		if(SecondCounter == SECONDS_HOUR(NIGHT_HOURS))
		{
			SecondCounter = 0;
			SystemFlag.InNight = false;
			SystemFlag.ToDay = true;
		}		
	}
	if(SystemFlag.ToDay)
	{
		if(SecondCounter == SECONDS_HOUR(TRANSITION_HOURS))
		{
			SecondCounter = 0;
			SystemFlag.ToDay = false;
			SystemFlag.InDay = true;
			SystemFlag.RefreshDimming = true;
		}		
	}	
	if(LogSecondCounter == LOG_PERIOD(MINUTE_TO_LOG))
	{
		LogSecondCounter = 0;
		LogFlag();
	}
}