#include "TaskEth-SD.h"
#include "Smart-Garden.h"
#include "SDLog.h"
#include "EthernetConn.h"


#ifdef TASK_ETH_SD
void TaskEthSD(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	
	uint16_t LastTaskWakeTime = xTaskGetTickCount();
	
	EthInit();
	SDInit();	
	
	for(;;)
	{
		LogToSD();
		CheckCableConnection();
		EthServer();
		OsDelayUntill(&LastTaskWakeTime, TASK_ETH_SD_DELAY);
	}
}

#endif
