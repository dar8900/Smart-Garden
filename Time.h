#ifndef TIME_H
#define TIME_H
#include <Arduino.h>

#define TIMESTAMP_TO_SEC(TimeStamp)		((TimeStamp) % 60)
#define TIMESTAMP_TO_MIN(TimeStamp)     (((TimeStamp) / 60) % 60)
#define TIMESTAMP_TO_HOUR(TimeStamp)    (((TimeStamp) / 3600) % 24)
#define TIMESTAMP_TO_DAY(TimeStamp)     (((TimeStamp) / 86400) % 31)
#define TIMESTAMP_TO_MONTH(TimeStamp)   (((TimeStamp) / 2678400) % 12)
#define TIMESTAMP_TO_YEAR(TimeStamp)    ((TimeStamp) / 32140800)



#define SEC_TO_MILLIS(s)	   (s * 1000)
#define MINUTE_TO_MILLIS(m)    ((m * 60) * 1000)

#define HOUR_IN_DAY					   24	

#define SECOND_IN_MINUTE 			   60
#define SECOND_IN_HOUR				 3600
#define SECOND_IN_24H				86400


#define SECONDS_HOUR(Hour)  		(Hour * SECOND_IN_HOUR)//((SECOND_IN_24H * Hour)/24)
#define SECONDS_MINUTE(Minute)      (SECOND_IN_MINUTE * Minute)

#define HOUR_SECONDS(Second)  		(Second / SECOND_IN_HOUR)
#define MINUTE_SECOND(Second)       (Second / SECOND_IN_MINUTE)
 
#define LOG_PERIOD(Minute)  	    (Minute * SECOND_IN_MINUTE)


#define DFLT_HOUR					9
#define DFLT_MINUTE					0
#define DFLT_DAY				   28
#define DFLT_MONTH					5  // uno in meno per il vettore dei giorni
#define DFLT_YEAR				 2019

#define LOG_PERIOD_SD		        SECONDS_MINUTE(5)    // In secondi

typedef struct
{
	uint8_t DayHours;
	uint8_t NightHours;
	uint8_t TransitionHours;
}DAY_TIME_HOURS;

typedef struct
{
	uint16_t Hour;
	uint16_t Minute;
	uint16_t Second;
	uint16_t Day;
	uint16_t Month;
	uint16_t Year;
}CALENDAR_VAR;

typedef enum
{
	IN_DAY = 0,
	TO_NIGHT,
	IN_NIGHT,
	TO_DAY,
	MAX_DAY_TIME
}DAY_TIME_VALUES;

typedef enum
{
	DAY_HOURS_DFL = 12,
	NIGHT_HOURS_DFL = 8,
	TRANSITION_HOURS_DFL = 2
}HOURS;

extern uint32_t SecondCounter;
extern uint16_t SecondForDimming;
extern DAY_TIME_HOURS DayTimeHours;
extern CALENDAR_VAR TimeDate;
extern const uint8_t DayForMonth[];
extern uint16_t LogToSDPeriod;
extern bool SecondTick;
extern bool DoDimming;

void RtcInit(void);
void RefreshCalendar(CALENDAR_VAR *TimeToRefresh);
void CheckTime(void);
void LogDayTime(void);
void LogDimming(void);
void LogSecondCounter(void);
void SaveTimeDate(void);
void SetTimeDate(uint8_t Hour, uint8_t Minute, uint8_t Day, uint8_t Month, uint16_t Year);
void SecondToCalendar(CALENDAR_VAR *ToCalendar, uint32_t Second);
void LoadTimeDate(CALENDAR_VAR *TimeDateToLoad);
#endif