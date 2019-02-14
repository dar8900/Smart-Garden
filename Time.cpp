#include "Time.h"
#include "Smart-Garden.h"

#define MINUTE_TO_LOG	10



uint32_t SecondCounter;
static uint32_t CounterToLog;
uint32_t ActualMillis, PreviousMillis, Delay = 1000;
uint16_t SecondForDimming = (SECONDS_MINUTE(TRANSITION_HOURS) / 255);
DAY_TIME_HOURS DayTimeHours;

void LogDayTime()
{
	EEPROM.update(DAY_TIME_ADDR, SystemFlag.DayTime);
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
	switch(SystemFlag.DayTime)
	{
		case IN_DAY:
			if(SecondCounter >= SECONDS_MINUTE(DayTimeHours.DayHours))
			{
				SecondCounter = 0;
				SystemFlag.DayTime = TO_NIGHT;
			}
			break;
		case IN_NIGHT:
			if(SecondCounter >= SECONDS_MINUTE(DayTimeHours.NightHours))
			{
				SecondCounter = 0;
				SystemFlag.DayTime = TO_DAY;
			}	
			break;
		case TO_NIGHT:
			if(SecondCounter >= SECONDS_MINUTE(DayTimeHours.TransitionHours))
			{
				SecondCounter = 0;
				SystemFlag.DayTime = IN_NIGHT;
				SystemFlag.RefreshDimming = true;
			}	
			break;
		case TO_DAY:
			if(SecondCounter >= SECONDS_MINUTE(DayTimeHours.TransitionHours))
			{
				SecondCounter = 0;
				SystemFlag.DayTime = IN_DAY;
				SystemFlag.RefreshDimming = true;
			}	
			break;
		default:
			break;		
	}
	if(CounterToLog == LOG_PERIOD(MINUTE_TO_LOG))
	{
		CounterToLog = 0;
		LogDayTime();
		LogDimming();
		LogSecondCounter();
	}
}