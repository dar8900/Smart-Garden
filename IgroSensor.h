#ifndef IGRO_SENSOR_H
#define IGRO_SENSOR_H
#include <Arduino.h>

typedef enum
{
	SENSOR_1 = 0,
	SENSOR_2,
	MAX_SENSORS
}IGRO_SENSORS;

typedef enum
{
	PUMP_OFF = 0,
	PUMP_ON,
	MAX_PUMP_STATE
}PUMP_STATE;

void ReadIgroThr(void);
void SensorsResponse(void);
void HygroValutation(void);
void PumpAction(bool IsOn);

#endif