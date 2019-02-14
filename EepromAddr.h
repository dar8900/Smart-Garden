#ifndef EEPROM_ADDR_H
#define EEPROM_ADDR_H

#define MAX_EEPROM_WRITES   100000

#define DAY_TIME_ADDR		    	0

#define DAY_HOUR_ADDR				1
#define TRANSITION_HOUR_ADDR		2
#define NIGHT_HOUR_ADDR				3

#define DIMMING_ADDR				4
#define SECOND_COUNTER_ADDR			5 // 4byte

typedef struct
{
	bool SaveDayTime;
	bool SaveHours;
	bool SaveDimming;
	bool SaveSecondCouter;
}FLAG_EEPROM;

#endif