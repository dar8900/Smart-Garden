#ifndef SMART_GARDEN_H
#define SMART_GARDEN_H
#include <stdint.h>

#include "EepromAddr.h"
#define DBG_SERIAL	  // Opzione per attivazione debug su seriale

#ifdef DBG_SERIAL
#define DBG(Str)	  Serial.println(Str)
#define DBG_LINE(Str) Serial.print(Str)
#else
#define DBG(Str)
#define DBG_LINE(Str) 
#endif	

typedef struct 
{
	bool Restart;
	uint8_t DayTime;
	uint8_t ManualPumpState;
	uint8_t LCD_SM;
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
	// bool BypassNormalLcd;
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
	PUMP        	= 37,
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

typedef enum
{
	REGULAR_SCREEN_STATE = 0,
	SET_HOUR_STATE,
	SET_PUMP_STATE,
	SET_TIME_STATE,
	BT_LCD_STATE,
	MAX_STATE
	
}LCD_STATE;

extern GENERAL_FLAG SystemFlag;
extern int16_t Dimming;
extern FLAG_EEPROM FlagForSave;

#endif