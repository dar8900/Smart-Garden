#include "TaskDimmingSensors.h"
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
uint8_t HygroState = HYGRO_KO;

static bool DimmingDone = false;

void TempHydroInit()
{
	THSensor.begin();
	delay(500);
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
				if(DoDimming)
				{
					DBG("Task Dimming-> Dimming");
					DBG(Dimming);
					analogWrite(DIMMING_LED, Dimming--);
					if(Dimming < 0)
						Dimming = 0;
					DoDimming = false;
				}	

				break;
			case TO_DAY:
				if(DoDimming)
				{
					analogWrite(DIMMING_LED, Dimming++);
					if(Dimming > 255)
						Dimming = 255;
					DimmingDone = true;
				}
				DoDimming = true;
				
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
		ReadIgroThr();
		SensorsResponse();
		HygroValutation();
		if(HygroState == HYGRO_KO)
			SystemFlag.TurnOnPumpAuto = true; 
		else 
			SystemFlag.TurnOnPumpAuto = false;
	}
	ReadFromTHSensor();
}

static void IgroLed()
{

	switch(HygroState)
	{
		case HYGRO_OK:
			digitalWrite(RED_LED, LOW);
			digitalWrite(YELLOW_LED, LOW);
			digitalWrite(GREEN_LED, HIGH);
			break;
		case HYGRO_MID:
			digitalWrite(RED_LED, LOW);
			digitalWrite(YELLOW_LED, HIGH);
			digitalWrite(GREEN_LED, LOW);
			break;
		case HYGRO_KO:
			digitalWrite(RED_LED, HIGH);
			digitalWrite(YELLOW_LED, LOW);
			digitalWrite(GREEN_LED, LOW);
			break;
		default:
			break;
	}
}

void TaskDimmingLed_Igro()  
{
	GardenLight();
	SensorsValutation();
	IgroLed();
	if(!SystemFlag.BypassIgrosensor && !SystemFlag.BypassIgrosensorBT)
		PumpAction(SystemFlag.TurnOnPumpAuto);
	else
		PumpAction(SystemFlag.ManualPumpState);
}
