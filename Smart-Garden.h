#ifndef SMART_GARDEN_H
#define SMART_GARDEN_H
#include <stdint.h>
#include <Arduino_FreeRTOS.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>
#include "EepromAddr.h"
#include "TaskSelection.h"

#define OsDelay(ms)  		vTaskDelay( ms / portTICK_PERIOD_MS ); // wait for ms second
#define OsDelayUntill(LastWakeTime, ms)   vTaskDelayUntil(LastWakeTime ,(ms / portTICK_PERIOD_MS) ); // wait for ms second

typedef struct 
{
	uint8_t DayTime;
	uint8_t ManualPumpState;
	bool TurnOnPumpAuto;
	bool RefreshDimming;
	bool SettingHours;
	bool BypassIgrosensor;
#ifdef TASK_SD
	bool SDInitialize;
	bool SDLogging;
#endif
	bool BTActive;
	bool BypassNormalLcd;
	bool BypassNormalDimming;
	bool BypassIgrosensorBT;
#ifdef TASK_ETH
	bool SwitchToSD;
#endif
}GENERAL_FLAG;

typedef enum
{
	DIMMING_LED 	= 3,
	PUMP        	= 5,	
	UP_BUTTON   	= 6,
	DOWN_BUTTON 	= 7,
	OK_BUTTON   	= 8,
	BT_LED_ACTIVE   = 10,
	ETH_CS          = 49
}PINS;

typedef enum
{
	BT_ICON = 0,
	SD_ICON,
	SUN_ICON,
	MOON_ICON,
	MIDHOURS_ICON,
	PUMP_ICON,
	MAX_ICON
}ICONS;

extern GENERAL_FLAG SystemFlag;
extern int16_t Dimming;
extern FLAG_EEPROM FlagForSave;

#endif