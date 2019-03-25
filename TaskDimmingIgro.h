#ifndef TASK_DIMMING_IGRO_H
#define TASK_DIMMING_IGRO_H
#include <Arduino.h>

typedef struct
{
	float Temperature;
	float Humidity;
	uint16_t HygroMeanResponse;
}SENSOR_VAR;

extern SENSOR_VAR SensorsValues;


void TaskDimmingLed_Igro( void *pvParameters );
#endif