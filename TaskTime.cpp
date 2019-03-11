#include "TaskTime.h"
#include "Smart-Garden.h"
#include "Time.h"

#ifdef TASK_TIME
void TaskTime(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	uint16_t LastTaskWakeTime = xTaskGetTickCount();
	for (;;)
	{
		OsDelayUntill(&LastTaskWakeTime, TASK_TIME_DELAY);
		CheckTime();
	}
}
#endif
