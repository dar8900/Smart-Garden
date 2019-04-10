#ifndef TASK_DIMMING_IGRO_H
#define TASK_DIMMING_IGRO_H
#include <Arduino.h>

typedef struct
{
	float Temperature;
	float Humidity;
	uint16_t HygroPotThr;
	uint16_t HygroMeanResponse;
}SENSOR_VAR;

typedef enum
{
	HYGRO_OK = 0,
	HYGRO_MID,
	HYGRO_KO,
	MAX_HYGRO_STATE
}IGRO_LED_STATE;


extern SENSOR_VAR SensorsValues;


void TaskDimmingLed_Igro( void );
#endif