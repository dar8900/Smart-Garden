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
		OsDelay(100);
	}
}
#endif
