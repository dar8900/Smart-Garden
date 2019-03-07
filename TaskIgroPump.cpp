#include "TaskIgroPump.h"
#include "Smart-Garden.h"
#include "IgroSensor.h"

#ifdef TASK_IGROSENSORPUMP
void TaskIgroSensorPump(void *pvParameters)  // This is a task.
{
	(void) pvParameters;

	for (;;)
	{
		if(!SystemFlag.BypassIgrosensor)
		{
			SensorsResponse();
			PumpAction(SystemFlag.TurnOnPumpAuto);
		}
		else
		{
			PumpAction(SystemFlag.ManualPumpState);
		}
		OsDelay(50);
	}
}
#endif