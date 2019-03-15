#include "TaskIgroPump.h"
#include "Smart-Garden.h"
#include "IgroSensor.h"
#include <DHT.h>

#define DHTTYPE DHT11 


#ifdef TASK_IGROSENSORPUMP

DHT THSensor(DHT_PIN, DHTTYPE);
SENSOR_VAR SensorsValues;

static void ReadFromTHSensor()
{
	SensorsValues.Humidity = THSensor.readHumidity();
	SensorsValues.Temperature = THSensor.readTemperature();
}

void TaskIgroSensorPump(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	
	THSensor.begin();
	
	for (;;)
	{
		if(!SystemFlag.BypassIgrosensor && !SystemFlag.BypassIgrosensorBT)
		{
			SensorsResponse();
			PumpAction(SystemFlag.TurnOnPumpAuto);
		}
		else
		{
			PumpAction(SystemFlag.ManualPumpState);
		}
		ReadFromTHSensor();
		OsDelay(TASK_IGROSENSORPUMP_DELAY);
	}
}
#endif