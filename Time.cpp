#include <RTClib.h>
#include "Time.h"
#include "Smart-Garden.h"
#include "EepromAddr.h"



#define MINUTE_TO_LOG	  15

#ifndef FAST_TIME
#define DELAY_SECOND 	   1
#else
#define DELAY_FAST_SEC	 25	
#endif

static RTC_DS1307 rtc;

static uint8_t CalendarSecond;
static uint32_t CounterToLog;
static uint32_t TimeDateCounterForSave;
static uint32_t TakeTime;
static bool SecondTick;

uint32_t SecondCounter;
uint16_t SecondForDimming;
DAY_TIME_HOURS DayTimeHours;
CALENDAR_VAR TimeDate;
CALENDAR_VAR TimeInLive;
uint16_t LogToSDPeriod;
bool DoDimming = false;
uint8_t	HourLeft;

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


static void RefreshTimeInLive()
{
	TimeInLive.Second++;
	if(TimeInLive.Second == 60)
	{
		TimeInLive.Second = 0;
		TimeInLive.Minute++;
	}
	if(TimeInLive.Minute == 60)
	{
		TimeInLive.Minute = 0;
		TimeInLive.Hour++;		
	}
	if(TimeInLive.Hour == 24)
	{
		TimeInLive.Hour = 0;		
	}
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
	while (! rtc.begin()) 
	{
		delay(1);
	}
	if (! rtc.isrunning())
	{
		SetTimeDate(DFLT_HOUR, DFLT_MINUTE, DFLT_DAY, DFLT_MONTH, DFLT_YEAR);
	}
}

void TaskTime()
{
#ifndef FAST_TIME
	if(TakeSecondDelayTime(&TakeTime, DELAY_SECOND))
	{
		SecondCounter++;
		CounterToLog++;	
#ifdef TASK_ETH_SD
		if(SystemFlag.SDInitialize && LogToSDPeriod < LOG_PERIOD_SD)
			LogToSDPeriod++;
#endif
		CalendarSecond++;
		SecondTick = true;
	}
	
#else // #ifndef FAST_TIME

	if(TakeTime == 0)
		TakeTime = millis();
	if(millis() - TakeTime >= DELAY_FAST_SEC)
	{
		SecondCounter++;
		SecondTick = true;
	}
#endif	// #ifndef FAST_TIME
	if(SecondTick)
	{
		switch(SystemFlag.DayTime)
		{
			case IN_DAY:
				if(SecondCounter >= SECONDS_HOUR(DayTimeHours.DayHours))
				{
					SecondCounter = 0;
					FlagForSave.ClearSecondCounter = true;
					SystemFlag.DayTime = TO_NIGHT;
					HourLeft = DayTimeHours.TransitionHours;
				}
				break;
			case IN_NIGHT:
				if(SecondCounter >= SECONDS_HOUR(DayTimeHours.NightHours))
				{
					SecondCounter = 0;
					FlagForSave.ClearSecondCounter = true;
					SystemFlag.DayTime = TO_DAY;
					HourLeft = DayTimeHours.NightHours;
				}	
				break;
			case TO_NIGHT:
				if(SecondCounter >= SECONDS_HOUR(DayTimeHours.TransitionHours))
				{
					SecondCounter = 0;
					FlagForSave.ClearSecondCounter = true;
					SystemFlag.DayTime = IN_NIGHT;
					SystemFlag.RefreshDimming = true;
					HourLeft = DayTimeHours.TransitionHours;
				}	
				break;
			case TO_DAY:
				if(SecondCounter >= SECONDS_HOUR(DayTimeHours.TransitionHours))
				{
					SecondCounter = 0;
					FlagForSave.ClearSecondCounter = true;
					SystemFlag.DayTime = IN_DAY;
					SystemFlag.RefreshDimming = true;
					HourLeft = DayTimeHours.TransitionHours;
				}	
				break;
			default:
				break;		
		}
		
		if((SystemFlag.DayTime == TO_NIGHT || SystemFlag.DayTime == TO_DAY) && (SecondCounter % SecondForDimming == 0))
			DoDimming = true;
		if(SecondCounter == SECOND_IN_HOUR)
		{
			HourLeft--;
		}
		RefreshTimeInLive();
		SecondTick = false;
	}
	if(CounterToLog == LOG_PERIOD(MINUTE_TO_LOG))
	{
		CounterToLog = 0;
		LogDayTime();
		LogDimming();
		LogSecondCounter();
	}
	if(CalendarSecond == 5)
	{
		CalendarSecond = 0;
		RefreshCalendar(&TimeDate);	
	}

}

void SetTimeDate(uint8_t Hour, uint8_t Minute, uint8_t Day, uint8_t Month, uint16_t Year)
{
	rtc.adjust(DateTime(Year, Month, Day, Hour, Minute, 0)); // anno, mese, giorno, h ,m,s
}


bool TakeSecondDelayTime(uint32_t *Cnt, uint16_t DelaySec)
{
	uint32_t Delaymsec = DelaySec * 1000;
	if(*Cnt == 0)
		*Cnt = millis();
	if((millis() - *Cnt) >= Delaymsec)
	{
		*Cnt = 0;
		return true;
	}
	else
		return false;
}

