#include "TaskKeyboardTime.h"
#include "Smart-Garden.h"
#include "Keyboard.h"
#include "Time.h"

#ifdef TASK_KEYBOARD_TIME_DELAY
void TaskKeyboardTime(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	uint16_t LastTaskWakeTime = xTaskGetTickCount();
	for (;;)
	{
		CheckButtons();
		CheckTime();
		OsDelayUntill(&LastTaskWakeTime, TASK_KEYBOARD_TIME_DELAY);
	}
}
#endif