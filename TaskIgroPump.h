#ifndef TASK_IGRO_PUMP_H
#define TASK_IGRO_PUMP_H
#include <Arduino.h>

typedef struct
{
	float Temperature;
	float Humidity;
	uint16_t HygroMeanResponse;
}SENSOR_VAR;

extern SENSOR_VAR SensorsValues;

void TaskIgroSensorPump( void *pvParameters );
#endif