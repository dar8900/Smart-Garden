#include "TaskCreate.h"
#include "Smart-Garden.h"
#include "TaskLcd.h"
#include "TaskKeyboard.h"
#include "TaskEeprom.h"
#include "TaskSD.h"
#include "TaskIgroPump.h"
#include "TaskTime.h"
#include "TaskDimming.h"
#include "TaskBT.h"


void OSInit()
{

#ifdef TASK_DIMMING
	xTaskCreate(
	TaskDimmingLed
	,  (const portCHAR *) "DimmingLed"
	,  TASK_DIMMING_HEAP  // Stack size
	,  NULL
	,  TASK_DIMMING_PRIORITY  // Priority
	,  NULL );
#endif

#ifdef TASK_TIME	
	xTaskCreate(
	TaskTime
	,  (const portCHAR *) "Time"
	,  TASK_TIME_HEAP  // Stack size
	,  NULL
	,  TASK_TIME_PRIORITY  // Priority
	,  NULL );
#endif

#ifdef TASK_IGROSENSORPUMP	
	xTaskCreate(
	TaskIgroSensorPump
	,  (const portCHAR *) "IgroSensorPump"
	,  TASK_IGROSENSORPUMP_HEAP  // Stack size
	,  NULL
	,  TASK_IGROSENSORPUMP_PRIORITY  // Priority
	,  NULL );
#endif

#ifdef TASK_LCD	
	xTaskCreate(
	TaskLCD
	,  (const portCHAR *) "LCD"
	,  TASK_LCD_HEAP // Stack size
	,  NULL
	,  TASK_LCD_PRIORITY  // Priority
	,  NULL );
#endif

#ifdef TASK_KEYBOARD	
	xTaskCreate(
	TaskKeyboard
	,  (const portCHAR *) "Keyboard"
	,  TASK_KEYBOARD_HEAP  // Stack size
	,  NULL
	,  TASK_KEYBOARD_PRIORITY  // Priority
	,  NULL );
#endif

#ifdef TASK_EEPROM	
	xTaskCreate(
	TaskEeprom
	,  (const portCHAR *) "Eeprom"
	,  TASK_EEPROM_HEAP  // Stack size
	,  NULL
	,  TASK_EEPROM_PRIORITY // Priority
	,  NULL );
#endif

#ifdef TASK_SD	
	xTaskCreate(
	TaskSD
	,  (const portCHAR *) "SD"
	,  TASK_SD_HEAP  // Stack size
	,  NULL
	,  TASK_SD_PRIORITY // Priority
	,  NULL );
#endif

#ifdef TASK_BT	
	xTaskCreate(
	TaskBT
	,  (const portCHAR *) "BT"
	,  TASK_BT_HEAP  // Stack size
	,  NULL
	,  TASK_BT_PRIORITY // Priority
	,  NULL );
#endif
}