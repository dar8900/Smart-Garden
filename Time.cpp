#include <RTClib.h>
#include "Time.h"
#include "Smart-Garden.h"

#define MINUTE_TO_LOG	15
#define DELAY_SECOND 	1000

uint32_t SecondCounter;
uint32_t CalendarSecond;
static uint32_t CounterToLog;
static uint32_t TimeDateCounterForSave;
static RTC_DS1307 rtc;
uint32_t StartTime;
uint16_t SecondForDimming = (SECONDS_MINUTE(TRANSITION_HOURS_DFL) / 255);
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



static void RefreshCalendar(CALENDAR_VAR *TimeToRefresh)
{
	
	TimeToRefresh->Hour =   rtc.now().hour();
	TimeToRefresh->Minute = rtc.now().minute();
	TimeToRefresh->Second = rtc.now().second();
	TimeToRefresh->Day = 	rtc.now().day();
	TimeToRefresh->Month = 	rtc.now().month();
	TimeToRefresh->Year =   rtc.now().year();
	
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
	if(StartTime == 0)
		StartTime = millis();
	if(millis() - StartTime >= DELAY_SECOND)
	{
		StartTime = 0;
		// CalendarSecond++;
		SecondCounter++;
		CounterToLog++;
		// TimeDateCounterForSave++;
#ifdef TASK_ETH_SD
		if(SystemFlag.SDInitialize && LogToSDPeriod < LOG_PERIOD_SD)
			LogToSDPeriod++;
#endif
		SecondTick = true;
	}
	if(SecondTick)
	{
		switch(SystemFlag.DayTime)
		{
			// Per debug ho messo i minuti invece delle ore
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
		RefreshCalendar(&TimeDate);
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

