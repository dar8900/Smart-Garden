#include "TaskDimmingIgro.h"
#include "Smart-Garden.h"
#include "IgroSensor.h"
#include <DHT.h>
#include "Time.h"

int16_t Dimming = 0;



#define USING_IGRO	
#define USING_DHT11

#define DHTTYPE DHT11 

DHT THSensor(DHT_PIN, DHTTYPE);
SENSOR_VAR SensorsValues;

void TempHydroInit()
{
#ifdef USING_DHT11
	THSensor.begin();
	delay(500);
#endif
}


static void ReadFromTHSensor()
{
	SensorsValues.Humidity = THSensor.readHumidity();
	SensorsValues.Temperature = THSensor.readTemperature();
	// DBG(SensorsValues.Humidity);
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
				if(SecondCounter % SecondForDimming == 0)
				{
					analogWrite(DIMMING_LED, Dimming--);
					if(Dimming < 0)
						Dimming = 0;
				}	
				break;
			case TO_DAY:
				if(SecondCounter % SecondForDimming == 0)
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
#ifdef USING_IGRO
	if(!SystemFlag.BypassIgrosensor && !SystemFlag.BypassIgrosensorBT)
	{
		SensorsResponse();
		PumpAction(SystemFlag.TurnOnPumpAuto);
	}
	else
	{
		PumpAction(SystemFlag.ManualPumpState);
	}
#endif
#ifdef USING_DHT11
	ReadFromTHSensor();
#endif
}


void TaskDimmingLed_Igro()  
{
	GardenLight();
	SensorsValutation();
}
