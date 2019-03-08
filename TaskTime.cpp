#include "TaskTime.h"
#include "Smart-Garden.h"
#include "Time.h"

#ifdef TASK_TIME
void TaskTime(void *pvParameters)  // This is a task.
{
	(void) pvParameters;

	for (;;)
	{
		CheckTime();
		OsDelay(TASK_TIME_DELAY);
	}
}
#endif
