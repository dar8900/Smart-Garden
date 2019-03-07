#ifndef TASK_BT_H
#define TASK_BT_H

#include <Arduino.h>

typedef struct
{
	String Command;
	String Response;
	uint8_t CmdRspValue;
}COMMAND_RESPONSE_S;


typedef enum
{
	BT_PUMP_ON      = 0,
	BT_PUMP_OFF,
	BT_LED_ON,
	BT_LED_OFF,
	BT_SET_DAY,
	BT_SET_NIGHT,
	BT_SET_MIDHOUR,
	MAX_CMD_RSP_VALUES
}COMMAND_RESPONSE_VALUE;

extern const COMMAND_RESPONSE_S PossibleCommandsResponse[];

void TaskBT(void *pvParameters);
#endif