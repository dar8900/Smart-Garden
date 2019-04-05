#include <RTClib.h>
#include "Time.h"
#include "Smart-Garden.h"
#include "EepromAddr.h"

#undef FAST_TIME

#define MINUTE_TO_LOG	15
#ifndef FAST_TIME
#define DELAY_SECOND 	1000
#else
#define DELAY_SECOND 	  50	
#endif

uint32_t SecondCounter;
uint32_t CalendarSecond;
static uint32_t CounterToLog;
static uint32_t TimeDateCounterForSave;
static RTC_DS1307 rtc;
static uint32_t TakeTime;
uint16_t SecondForDimming;
DAY_TIME_HOURS DayTimeHours;
CALENDAR_VAR TimeDate;
uint16_t LogToSDPeriod;
static bool SecondTick;

const uint8_t DayForMonth[12]
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



void RefreshCalendar(CALENDAR_VAR *TimeToRefresh)
{
	
	TimeToRefresh->Hour   = rtc.now().hour();
	TimeToRefresh->Minute = rtc.now().minute();
	TimeToRefresh->Second = rtc.now().second();
	TimeToRefresh->Day    = rtc.now().day();
	TimeToRefresh->Month  = rtc.now().month();
	TimeToRefresh->Year   = rtc.now().year();
	
}


void SecondToCalendar(CALENDAR_VAR *ToCalendar, uint32_t Second)
{
	ToCalendar->Second = TIMESTAMP_TO_SEC(Second);	
	ToCalendar->Minute = TIMESTAMP_TO_MIN(Second);  
	ToCalendar->Hour   = TIMESTAMP_TO_HOUR(Second); 
	ToCalendar->Day    = TIMESTAMP_TO_DAY(Second);  
	ToCalendar->Month  = TIMESTAMP_TO_MONTH(Second);
	ToCalendar->Year   = TIMESTAMP_TO_YEAR(Second); 
}

void RtcInit()
{
	if (! rtc.begin()) 
	{
		
	}
	if (! rtc.isrunning())
	{
		SetTimeDate(DFLT_HOUR, DFLT_MINUTE, DFLT_DAY, DFLT_MONTH, DFLT_YEAR);
	}
}

void CheckTime()
{
	if(TakeTime == 0)
		TakeTime = millis();
	if(millis() - TakeTime >= DELAY_SECOND)
	{
		TakeTime = 0;
		SecondCounter++;
#ifndef FAST_TIME
		CounterToLog++;
		
#ifdef TASK_ETH_SD
		if(SystemFlag.SDInitialize && LogToSDPeriod < LOG_PERIOD_SD)
			LogToSDPeriod++;
#endif

#endif
		SecondTick = true;
	}
	if(SecondTick)
	{
		switch(SystemFlag.DayTime)
		{
			// Per debug ho messo i minuti invece delle ore
			case IN_DAY:
				if(SecondCounter >= SECONDS_HOUR(DayTimeHours.DayHours))
				{
					SecondCounter = 0;
					FlagForSave.ClearSecondCounter = true;
					SystemFlag.DayTime = TO_NIGHT;
				}
				break;
			case IN_NIGHT:
				if(SecondCounter >= SECONDS_HOUR(DayTimeHours.NightHours))
				{
					SecondCounter = 0;
					FlagForSave.ClearSecondCounter = true;
					SystemFlag.DayTime = TO_DAY;
				}	
				break;
			case TO_NIGHT:
				if(SecondCounter >= SECONDS_HOUR(DayTimeHours.TransitionHours))
				{
					SecondCounter = 0;
					FlagForSave.ClearSecondCounter = true;
					SystemFlag.DayTime = IN_NIGHT;
					SystemFlag.RefreshDimming = true;
				}	
				break;
			case TO_DAY:
				if(SecondCounter >= SECONDS_HOUR(DayTimeHours.TransitionHours))
				{
					SecondCounter = 0;
					FlagForSave.ClearSecondCounter = true;
					SystemFlag.DayTime = IN_DAY;
					SystemFlag.RefreshDimming = true;
				}	
				break;
			default:
				break;		
		}
		
#ifndef FAST_TIME
		RefreshCalendar(&TimeDate);
#endif
		SecondTick = false;
	}
	if(CounterToLog == LOG_PERIOD(MINUTE_TO_LOG))
	{
		CounterToLog = 0;
		LogDayTime();
		LogDimming();
		LogSecondCounter();
	}

}

void SetTimeDate(uint8_t Hour, uint8_t Minute, uint8_t Day, uint8_t Month, uint16_t Year)
{
	rtc.adjust(DateTime(Year, Month, Day, Hour, Minute, 0)); // anno, mese, giorno, h ,m,s
}

