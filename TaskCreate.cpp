#include "TaskCreate.h"
#include "Smart-Garden.h"
#include "TaskLcd.h"
#include "TaskKeyboardTime.h"
#include "TaskEeprom.h"
#include "TaskEth-SD.h"
// #include "TaskIgroPump.h"
// #include "TaskTime.h"
#include "TaskDimmingIgro.h"
#include "TaskBT.h"
// #include "TaskEth.h"

void OSInit()
{

#ifdef TASK_DIMMING_IGROSENSORPUMP
	xTaskCreate(
	TaskDimmingLed_Igro
	,  (const portCHAR *) "DimmingLedIgro"
	,  TASK_DIMMING_IGROSENSORPUMP_STACK  // Stack size
	,  NULL
	,  TASK_DIMMING_IGROSENSORPUMP_PRIORITY  // Priority
	,  NULL );
#endif

#ifdef TASK_LCD	
	xTaskCreate(
	TaskLCD
	,  (const portCHAR *) "LCD"
	,  TASK_LCD_STACK // Stack size
	,  NULL
	,  TASK_LCD_PRIORITY  // Priority
	,  NULL );
#endif

#ifdef TASK_KEYBOARD_TIME_DELAY	
	xTaskCreate(
	TaskKeyboardTime
	,  (const portCHAR *) "KeyboardTime"
	,  TASK_KEYBOARD_TIME_STACK  // Stack size
	,  NULL
	,  TASK_KEYBOARD_TIME_PRIORITY  // Priority
	,  NULL );
#endif

#ifdef TASK_EEPROM	
	xTaskCreate(
	TaskEeprom
	,  (const portCHAR *) "Eeprom"
	,  TASK_EEPROM_STACK  // Stack size
	,  NULL
	,  TASK_EEPROM_PRIORITY // Priority
	,  NULL );
#endif

#ifdef TASK_ETH_SD	
	xTaskCreate(
	TaskEthSD
	,  (const portCHAR *) "SD"
	,  TASK_ETH_SD_STACK  // Stack size
	,  NULL
	,  TASK_ETH_SD_PRIORITY // Priority
	,  NULL );
#endif

#ifdef TASK_BT	
	xTaskCreate(
	TaskBT
	,  (const portCHAR *) "BT"
	,  TASK_BT_STACK  // Stack size
	,  NULL
	,  TASK_BT_PRIORITY // Priority
	,  NULL );
#endif



}