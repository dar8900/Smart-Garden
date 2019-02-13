#ifndef IGRO_SENSOR_H
#define IGRO_SENSOR_H
#include <Arduino.h>

typedef enum
{
	SENSOR_1 = 0,
	SENSOR_2,
	MAX_SENSORS
}IGRO_SENSORS;

void SensorsResponse(void);

#endif