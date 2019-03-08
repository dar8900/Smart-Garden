#include "TaskSD.h"
#include "Smart-Garden.h"
#include "SDLog.h"

#ifdef TASK_SD
void TaskSD(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	SDBegin();	
	for(;;)
	{
		LogToSD();
		OsDelay(TASK_SD_DELAY);
	}
}

#endif
