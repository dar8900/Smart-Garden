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
	,  64  // Stack size
	,  NULL
	,  1  // Priority
	,  NULL );
#endif

#ifdef TASK_TIME	
	xTaskCreate(
	TaskTime
	,  (const portCHAR *) "Time"
	,  64  // Stack size
	,  NULL
	,  3  // Priority
	,  NULL );
#endif

#ifdef TASK_IGROSENSORPUMP	
	xTaskCreate(
	TaskIgroSensorPump
	,  (const portCHAR *) "IgroSensorPump"
	,  64  // Stack size
	,  NULL
	,  2  // Priority
	,  NULL );
#endif

#ifdef TASK_LCD	
	xTaskCreate(
	TaskLCD
	,  (const portCHAR *) "LCD"
	,  128 // Stack size
	,  NULL
	,  1  // Priority
	,  NULL );
#endif

#ifdef TASK_KEYBOARD	
	xTaskCreate(
	TaskKeyboard
	,  (const portCHAR *) "Keyboard"
	,  64  // Stack size
	,  NULL
	,  3  // Priority
	,  NULL );
#endif

#ifdef TASK_EEPROM	
	xTaskCreate(
	TaskEeprom
	,  (const portCHAR *) "Eeprom"
	,  64  // Stack size
	,  NULL
	,  0 // Priority
	,  NULL );
#endif

#ifdef TASK_SD	
	xTaskCreate(
	TaskSD
	,  (const portCHAR *) "SD"
	,  64  // Stack size
	,  NULL
	,  2 // Priority
	,  NULL );
#endif

#ifdef TASK_BT	
	xTaskCreate(
	TaskBT
	,  (const portCHAR *) "BT"
	,  64  // Stack size
	,  NULL
	,  3 // Priority
	,  NULL );
#endif
}