#ifndef SMART_GARDEN_H
#define SMART_GARDEN_H
#include <stdint.h>
#include <Arduino_FreeRTOS.h>
#include <EEPROM.h>
#include "EepromAddr.h"

#define OsDelay(ms)  vTaskDelay( ms / portTICK_PERIOD_MS ); // wait for one second

typedef struct 
{
	uint8_t DayTime;
	bool TurnOnPumpAuto;
	uint8_t ManualPumpState;
	bool RefreshDimming;
	bool SettingHours;
	bool BypassIgrosensor;
}GENERAL_FLAG;

typedef enum
{
	DIMMING_LED = 3,
	PUMP        = 5,	
	UP_BUTTON   = 6,
	DOWN_BUTTON = 7,
	OK_BUTTON   = 8
}PINS;

extern GENERAL_FLAG SystemFlag;
extern int16_t Dimming;
extern FLAG_EEPROM FlagForSave;

#endif