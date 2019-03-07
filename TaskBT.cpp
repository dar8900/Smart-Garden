#include "TaskBT.h"
#include "Smart-Garden.h"
#include "Bluetooth.h"
#include "Time.h"

#ifdef TASK_BT
void TaskBT(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	uint16_t CheckBtActiveCnt = 0;

	for(;;)
	{
		if(CheckBtActiveCnt == 15)
		{
			IsBTActive();
			CheckBtActiveCnt = 0;
		}
		CheckBtActiveCnt++;
		OsDelay(100);
	}
}

#endif