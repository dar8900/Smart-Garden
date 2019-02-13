#ifndef TIME_H
#define TIME_H
#include <Arduino.h>

#define SECOND_IN_MINUTE 			   60
#define SECOND_IN_24H				86400
#define SECONDS_HOUR(Hour)  		((SECOND_IN_24H * Hour)/24)

#define LOG_PERIOD(Minute)  	    (Minute * SECOND_IN_MINUTE)

extern uint32_t SecondCounter;
extern uint16_t SecondForDimming;

void CheckTime(void);
void LogFlag(void);
#endif