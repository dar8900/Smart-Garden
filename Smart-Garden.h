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

#define IS_ON 	true
#define IS_OFF	false

typedef struct 
{
	bool Restart;
	uint8_t DayTime;
	uint8_t LCD_SM;
	bool TurnOnPumpAuto;
	bool ManualPumpState;
	bool PumpState;
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
	BT_LED_ACTIVE   	=  2,
	DIMMING_RED_LED 	=  3,	
	DIMMING_BLUE_LED 	=  4,
	DIMMING_GREEN_LED 	=  5,
	UP_BUTTON   		= 22,
	DOWN_BUTTON 		= 23,
	OK_BUTTON   		= 24,
	DHT_PIN         	= 26,
	PUMP        		= 27,
	SD_CS				= 11,
	ETH_CS          	= 10,
	RED_LED         	= 28,
	YELLOW_LED      	= 29,
	GREEN_LED       	= 30,
	RESET_EXIT_BUTTON   = 31,
	RESET_PIN			= 32
}PINS;

typedef enum
{
	BT_ICON = 0,
	SD_ICON,
	ETH_ICON,
	SUN_ICON,
	MOON_ICON,
	MID_HOUR_ICON,
	PUMP_ICON,
	MAX_ICON
}ICONS;

typedef enum
{
	REGULAR_SCREEN_STATE = 0,
	SET_HOUR_STATE,
	SET_PUMP_STATE,
	SET_TIME_STATE,
	RESET_STATE,
	BT_LCD_STATE,
	MAX_STATE
	
}LCD_STATE;

extern GENERAL_FLAG SystemFlag;
extern int16_t Dimming;
extern FLAG_EEPROM FlagForSave;

#endif