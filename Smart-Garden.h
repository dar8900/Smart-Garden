#ifndef SMART_GARDEN_H
#define SMART_GARDEN_H
#include <stdint.h>
#include <Arduino_FreeRTOS.h>
#include <EEPROM.h>
#include "EepromAddr.h"

#define OsDelay(ms)  vTaskDelay( ms / portTICK_PERIOD_MS ); // wait for one second

typedef struct 
{
	bool InDay;
	bool ToNight;
	bool ToDay;
	bool InNight;
	bool TurnOnPump;
	bool RefreshDimming;
}GENERAL_FLAG;

typedef enum
{
	DIMMING_LED = 3,
	PUMP = 5	
}PINS;

extern GENERAL_FLAG SystemFlag;


#endif