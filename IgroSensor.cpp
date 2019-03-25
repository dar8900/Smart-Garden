#include "Smart-Garden.h"
#include "IgroSensor.h"
#include "TaskDimmingIgro.h"

#define MAX_SAMPLE	50

#define PUMP_ACTIVATION_THR		512

uint16_t SensorsValue[MAX_SENSORS][MAX_SAMPLE];
uint8_t Sample;
static bool StopReading;


void SensorsResponse()
{
	uint16_t MeanSensors[MAX_SENSORS] = {0};
	uint8_t SampleIndex = 0, SensorIndex = 0;
	if(!StopReading)
	{
		SensorsValue[SENSOR_1][Sample] = analogRead(A0);
		SensorsValue[SENSOR_2][Sample] = analogRead(A1);
		Sample++;
		if(Sample == MAX_SAMPLE)
			StopReading = true;
	}
	if(StopReading)
	{
		StopReading = false;
		Sample = 0;
		for(SensorIndex = 0; SensorIndex < MAX_SENSORS; SensorIndex++)
		{
			for(SampleIndex = 0; SampleIndex < MAX_SAMPLE; SampleIndex++)
			{
				MeanSensors[SensorIndex] += SensorsValue[SensorIndex][SampleIndex];
			}
			MeanSensors[SensorIndex] /= MAX_SAMPLE;
			SensorsValues.HygroMeanResponse += MeanSensors[SensorIndex];
		}
		SensorsValues.HygroMeanResponse /= MAX_SENSORS;
		DBG("Task Igro-> risposta sensori: " + String(SensorsValues.HygroMeanResponse));
		if(SensorsValues.HygroMeanResponse >= PUMP_ACTIVATION_THR)
			SystemFlag.TurnOnPumpAuto = true;
		else 
			SystemFlag.TurnOnPumpAuto = false;
	}
}

void PumpAction(bool IsOn)
{
	if(IsOn)
	{
		digitalWrite(PUMP, HIGH);
		SystemFlag.ManualPumpState = PUMP_ON;
	}
	else
	{
		digitalWrite(PUMP, LOW);
		SystemFlag.ManualPumpState = PUMP_OFF;
	}
}