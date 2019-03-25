#include "TaskDimmingIgro.h"
#include "Smart-Garden.h"
#include "IgroSensor.h"
#include <DHT.h>
#include "Time.h"

int16_t Dimming = 0;

#ifdef TASK_DIMMING_IGROSENSORPUMP

#define DHTTYPE DHT11 

DHT THSensor(DHT_PIN, DHTTYPE);
SENSOR_VAR SensorsValues;

static void ReadFromTHSensor()
{
	SensorsValues.Humidity = THSensor.readHumidity();
	SensorsValues.Temperature = THSensor.readTemperature();
}


static void GardenLight()
{
	if(!SystemFlag.BypassNormalDimming)
	{
		switch(SystemFlag.DayTime)
		{
			case IN_DAY:
				if(SystemFlag.RefreshDimming)
				{
					Dimming = 255;
					analogWrite(DIMMING_LED, Dimming);
					SystemFlag.RefreshDimming = false;
				}
				break;
			case IN_NIGHT:
				if(SystemFlag.RefreshDimming)
				{
					Dimming = 0;
					analogWrite(DIMMING_LED, Dimming);
					SystemFlag.RefreshDimming = false;
				}
				break;
			case TO_NIGHT:
				if(SecondCounter % SecondForDimming)
				{
					analogWrite(DIMMING_LED, Dimming--);
					if(Dimming < 0)
						Dimming = 0;
				}	
				break;
			case TO_DAY:
				if(SecondCounter % SecondForDimming)
				{
					analogWrite(DIMMING_LED, Dimming++);
					if(Dimming > 255)
						Dimming = 255;
				}
				break;
			default:
				break;			
		}
	}
	else
	{
		if(SystemFlag.RefreshDimming)
		{
			analogWrite(DIMMING_LED, Dimming);		
			SystemFlag.RefreshDimming = false;
		}
	}	
}

static void SensorsValutation()	
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
}


void TaskDimmingLed_Igro(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	THSensor.begin();
	for (;;)
	{
		GardenLight();
		SensorsValutation();
		OsDelay(TASK_DIMMING_IGROSENSORPUMP_DELAY);
	}
}
#endif