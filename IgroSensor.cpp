#include "Smart-Garden.h"
#include "IgroSensor.h"
#include "TaskDimmingSensors.h"

#define MAX_SAMPLE_HYGRO	25
#define MAX_SAMPLE_POT		10
#define HYGRO_DRY_LEVEL	  1017	

// #define PUMP_ACTIVATION_THR		1000 // DBG da valutare

extern uint8_t HygroState;

uint16_t SensorsValue[MAX_SENSORS][MAX_SAMPLE_HYGRO];
uint8_t Sample;
static bool StopReading;

void HygroValutation()
{
	int16_t Diff_1 = 0, Diff_2 = 0;
	Diff_1 = SensorsValues.HygroMeanResponse - SensorsValues.HygroPotThr;
	if(SensorsValues.HygroPotThr >= HYGRO_DRY_LEVEL)
		Diff_2 = HYGRO_DRY_LEVEL;
	else
		Diff_2 = HYGRO_DRY_LEVEL - ((HYGRO_DRY_LEVEL - SensorsValues.HygroPotThr)/2);
	if(Diff_1 <= 0)
	{
		HygroState = HYGRO_OK;
	}
	else if((Diff_1 > 0) && (SensorsValues.HygroMeanResponse <= Diff_2))
	{
		HygroState = HYGRO_MID;
	}
	else if((Diff_1 > 0) && (SensorsValues.HygroMeanResponse > Diff_2))
	{
		HygroState = HYGRO_KO;
	}
}

void ReadIgroThr()
{
	uint16_t PotReading[MAX_SAMPLE_POT] = {0}, PotMeanRead = 0;
	uint8_t Sample = 0;
	
	for(Sample = 0; Sample < MAX_SAMPLE_POT; Sample++)
	{
		PotReading[Sample] = analogRead(A2);
		PotMeanRead += PotReading[Sample];
	}
	PotMeanRead /= MAX_SAMPLE_POT;
	SensorsValues.HygroPotThr = PotMeanRead;
}


void SensorsResponse()
{
	uint16_t MeanSensors[MAX_SENSORS] = {0};
	uint8_t SampleIndex = 0, SensorIndex = 0;
	while(Sample < MAX_SAMPLE_HYGRO)
	{
		SensorsValue[SENSOR_1][Sample] = analogRead(A0);
		SensorsValue[SENSOR_2][Sample] = analogRead(A1);
		Sample++;
	}
	Sample = 0;
	SensorsValues.HygroMeanResponse = 0;
	for(SensorIndex = 0; SensorIndex < MAX_SENSORS; SensorIndex++)
	{
		for(SampleIndex = 0; SampleIndex < MAX_SAMPLE_HYGRO; SampleIndex++)
		{
			MeanSensors[SensorIndex] += SensorsValue[SensorIndex][SampleIndex];
		}
		MeanSensors[SensorIndex] /= MAX_SAMPLE_HYGRO;
		SensorsValues.HygroMeanResponse += MeanSensors[SensorIndex];
	}
	SensorsValues.HygroMeanResponse /= MAX_SENSORS;
}


void PumpAction(bool IsOn)
{
	if(IsOn)
	{
		digitalWrite(PUMP, HIGH);
		SystemFlag.PumpState = true;
	}
	else
	{
		digitalWrite(PUMP, LOW);
		SystemFlag.PumpState = false;
	}
}