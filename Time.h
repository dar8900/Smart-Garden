#ifndef TIME_H
#define TIME_H
#include <Arduino.h>

#define SECOND_IN_MINUTE 			   60
#define SECOND_IN_HOUR				 3600
#define SECOND_IN_24H				86400


#define SECONDS_HOUR(Hour)  		((SECOND_IN_24H * Hour)/24)
#define SECONDS_MINUTE(Minute)      (SECOND_IN_MINUTE * Minute)

#define HOUR_SECONDS(Second)  		(Second / SECOND_IN_HOUR)
#define MINUTE_SECOND(Second)       (Second / SECOND_IN_MINUTE)
 
#define LOG_PERIOD(Minute)  	    (Minute * SECOND_IN_MINUTE)

typedef struct
{
	uint8_t DayHours;
	uint8_t NightHours;
	uint8_t TransitionHours;
}DAY_TIME_HOURS;

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

void CheckTime(void);
void LogDayTime(void);
void LogDimming(void);
void LogSecondCounter(void);
#endif