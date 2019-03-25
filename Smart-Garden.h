#ifndef SMART_GARDEN_H
#define SMART_GARDEN_H
#include <stdint.h>
#include <Arduino_FreeRTOS.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>
#include "EepromAddr.h"
#include "TaskSelection.h"

#define OsDelay(ms)  		              vTaskDelay( ms / portTICK_PERIOD_MS ); // wait for ms second
#define OsDelayUntill(LastWakeTime, ms)   vTaskDelayUntil(LastWakeTime ,(ms / portTICK_PERIOD_MS) ); // wait for ms second

#define DBG_SERIAL	  // Opzione per attivazione debug su seriale

#ifdef DBG_SERIAL
#define DBG(Str)	Serial.println(Str)
#else
#define DBG(Str)
#endif	

typedef struct 
{
	bool Restart;
	uint8_t DayTime;
	uint8_t ManualPumpState;
	bool TurnOnPumpAuto;
	bool RefreshDimming;
	bool SettingHours;
	bool BypassIgrosensor;
	bool SDInitialize;
	bool SDLogging;
	bool SDWebPagePresent;
	bool EthCableConnected;
	bool EthClient;
	bool BTActive;
	bool BypassNormalLcd;
	bool BypassNormalDimming;
	bool BypassIgrosensorBT;
// #ifdef TASK_ETH
	// bool CableConnected;
	// bool EthClient;
// #endif
}GENERAL_FLAG;

typedef enum
{
	DIMMING_LED 	= 3,	
	UP_BUTTON   	= 22,
	DOWN_BUTTON 	= 23,
	OK_BUTTON   	= 24,
	BT_LED_ACTIVE   = 25,
	DHT_PIN         = 26,
	PUMP        	= 27,
	SD_CS			= 11,
	ETH_CS          = 10
}PINS;

typedef enum
{
	BT_ICON = 0,
	SD_ICON,
	ETH_ICON,
	SUN_ICON,
	MOON_ICON,
	PUMP_ICON,
	MAX_ICON
}ICONS;

extern GENERAL_FLAG SystemFlag;
extern int16_t Dimming;
extern FLAG_EEPROM FlagForSave;

#endif