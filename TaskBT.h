#ifndef TASK_BT_H
#define TASK_BT_H

#include <Arduino.h>

typedef struct
{
	String CommandList;
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
	BT_SET_TIME,
	BT_SET_DATE,
	MAX_CMD_RSP_VALUES
}COMMAND_RESPONSE_VALUE;

typedef enum
{
	CARD_CONNECTION_STATE = 0,
	DEVICE_CONNECTION_STATE,
	CHECK_OR_SHOW_COMMAND_LIST_STATE,
	EXECUTE_COMMAND_STATE,
	
	
}BT_STATE;

extern const COMMAND_RESPONSE_S PossibleCommandsResponse[];
extern uint8_t BT_SM;

void TaskBT(void);
#endif