#ifndef EEPROM_ADDR_H
#define EEPROM_ADDR_H

#define MAX_EEPROM_WRITES   100000

#define DAY_TIME_ADDR		    	0

#define DAY_HOUR_ADDR				1
#define TRANSITION_HOUR_ADDR		2
#define NIGHT_HOUR_ADDR				3

#define DIMMING_ADDR				4
#define SECOND_COUNTER_ADDR			5 // 4 byte

#define SECOND_DIMMING_ADDR			11 // 2	byte

#define CALENDAR_HOUR_ADDR          20
#define CALENDAR_MINUTE_ADDR		21
#define CALENDAR_DAY_ADDR           22
#define CALENDAR_MONTH_ADDR         23
#define CALENDAR_YEAR_ADDR          24 // 2 byte

#define FIRST_GO_ADDR			   100
#define NUMBER_OF_WRITES_ADDR	   200
                                   

typedef struct
{
	bool SaveDayTime;
	bool SaveHours;
	bool SaveDimming;
	bool SaveSecondCouter;
	bool SaveSecondDimming;
	bool ClearSecondCounter;
	// bool SaveCalendar;
}FLAG_EEPROM;

#endif