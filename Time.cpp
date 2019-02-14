#include "Time.h"
#include "Smart-Garden.h"

#define MINUTE_TO_LOG	10



uint32_t SecondCounter;
static uint32_t CounterToLog;
uint32_t ActualMillis, PreviousMillis, Delay = 1000;
uint16_t SecondForDimming = (SECONDS_MINUTE(TRANSITION_HOURS) / 255);

void LogFlag()
{
	EEPROM.update(IN_DAY_ADDR,   (uint8_t)SystemFlag.InDay);
	EEPROM.update(IN_NIGHT_ADDR, (uint8_t)SystemFlag.InNight);
	EEPROM.update(TO_DAY_ADDR,   (uint8_t)SystemFlag.ToDay);
	EEPROM.update(TO_NIGHT_ADDR, (uint8_t)SystemFlag.ToNight);
}

void LogDimming()
{
	EEPROM.update(DIMMING_ADDR ,(uint8_t)Dimming);
}

void LogSecondCounter()
{
	EEPROM.put(SECOND_COUNTER_ADDR, SecondCounter);
}

void CheckTime()
{
	ActualMillis = millis();
	if(ActualMillis - PreviousMillis >= Delay)
	{
		PreviousMillis = ActualMillis;
		SecondCounter++;
		CounterToLog++;
	}
	if(SystemFlag.InDay)
	{
		if(SecondCounter >= SECONDS_MINUTE(DAY_HOURS))
		{
			SecondCounter = 0;
			SystemFlag.InDay = false;
			SystemFlag.ToNight = true;
		}
	}
	if(SystemFlag.ToNight)
	{
		if(SecondCounter >= SECONDS_MINUTE(TRANSITION_HOURS))
		{
			SecondCounter = 0;
			SystemFlag.ToNight = false;
			SystemFlag.InNight = true;
			SystemFlag.RefreshDimming = true;
		}		
	}
	if(SystemFlag.InNight)
	{
		if(SecondCounter >= SECONDS_MINUTE(NIGHT_HOURS))
		{
			SecondCounter = 0;
			SystemFlag.InNight = false;
			SystemFlag.ToDay = true;
		}		
	}
	if(SystemFlag.ToDay)
	{
		if(SecondCounter >= SECONDS_MINUTE(TRANSITION_HOURS))
		{
			SecondCounter = 0;
			SystemFlag.ToDay = false;
			SystemFlag.InDay = true;
			SystemFlag.RefreshDimming = true;
		}		
	}	
	if(CounterToLog == LOG_PERIOD(MINUTE_TO_LOG))
	{
		CounterToLog = 0;
		LogFlag();
		LogDimming();
		LogSecondCounter();
	}
}