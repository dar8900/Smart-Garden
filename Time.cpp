#include "Time.h"
#include "Smart-Garden.h"

#define MINUTE_TO_LOG	10
#define DELAY_SECOND 	1000

uint32_t SecondCounter;
uint32_t CalendarSecond;
static uint32_t CounterToLog;
static uint32_t TimeDateCounterForSave;
uint32_t StartTime;
uint16_t SecondForDimming = (SECONDS_MINUTE(TRANSITION_HOURS_DFL) / 255);
DAY_TIME_HOURS DayTimeHours;
CALENDAR_VAR TimeDate;
uint16_t LogToSDPeriod;

uint8_t DayForMonth[12]
{
	31,
	28,
	31,
	30,
	31,
	30,
	31,
	31,
	30,
	31,
	30,
	31,	
};


void LogDayTime()
{
	FlagForSave.SaveDayTime = true;
}

void LogDimming()
{
	FlagForSave.SaveDimming = true;
}

void LogSecondCounter()
{
	FlagForSave.SaveSecondCouter = true;
}


static void RefreshCalendar(CALENDAR_VAR *TimeToRefresh)
{
	if(CalendarSecond == 60)
	{
		CalendarSecond = 0;
		TimeToRefresh->Minute++;
	}
	if(TimeToRefresh->Minute == 60)
	{
		TimeToRefresh->Hour++;
		TimeToRefresh->Minute = 0;
	}
	if(TimeToRefresh->Hour == 24)
	{
		TimeToRefresh->Day++;
		TimeToRefresh->Hour = 0;
	}
	if(TimeToRefresh->Day == (DayForMonth[TimeToRefresh->Month] + 1))
	{
		TimeToRefresh->Month++;
		TimeToRefresh->Day = 0;
	}
	if(TimeToRefresh->Month == 12)
	{
		TimeToRefresh->Year++;
		TimeToRefresh->Month = 0;
	}		
}


void CheckTime()
{
	if(StartTime == 0)
		StartTime = millis();
	if(millis() - StartTime >= DELAY_SECOND)
	{
		StartTime = 0;
		CalendarSecond++;
		SecondCounter++;
		CounterToLog++;
		TimeDateCounterForSave++;
#ifdef TASK_SD
		if(SystemFlag.SDInitialize && LogToSDPeriod < LOG_PERIOD_SD)
			LogToSDPeriod++;
#endif
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
	RefreshCalendar(&TimeDate);
	if(TimeDateCounterForSave == LOG_PERIOD(30))
	{
		FlagForSave.SaveCalendar = true;
		TimeDateCounterForSave = 0;
	}
}

void SetTimeDate(uint8_t Hour, uint8_t Minute, uint8_t Day, uint8_t Month, uint16_t Year, CALENDAR_VAR *TimeDateToSet)
{
	CalendarSecond = 0;
	TimeDateToSet->Hour   = Hour;
	TimeDateToSet->Minute = Minute;
	TimeDateToSet->Day    = Day;
	TimeDateToSet->Month  = Month;
	TimeDateToSet->Year   = Year;
	FlagForSave.SaveCalendar = true;
}

void SaveTimeDate()
{
	EEPROM.update(CALENDAR_HOUR_ADDR, TimeDate.Hour);
	EEPROM.update(CALENDAR_MINUTE_ADDR, TimeDate.Minute);
	EEPROM.update(CALENDAR_DAY_ADDR, TimeDate.Day);
	EEPROM.update(CALENDAR_MONTH_ADDR, TimeDate.Month);
	EEPROM.put(CALENDAR_YEAR_ADDR, TimeDate.Year);
}

void LoadTimeDate(CALENDAR_VAR *TimeDateToLoad)
{
	CalendarSecond = 0;
	TimeDateToLoad->Hour   = EEPROM.read(CALENDAR_HOUR_ADDR);
	TimeDateToLoad->Minute = EEPROM.read(CALENDAR_MINUTE_ADDR);
	TimeDateToLoad->Day    = EEPROM.read(CALENDAR_DAY_ADDR);
	TimeDateToLoad->Month  = EEPROM.read(CALENDAR_MONTH_ADDR);
	EEPROM.get(CALENDAR_YEAR_ADDR, TimeDateToLoad->Year);
}
