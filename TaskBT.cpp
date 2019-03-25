#include "TaskBT.h"
#include "Smart-Garden.h"
#include "IgroSensor.h"
#include "Bluetooth.h"
#include "Time.h"

#ifdef TASK_BT

const COMMAND_RESPONSE_S PossibleCommandsResponse[MAX_CMD_RSP_VALUES]
{
	{"Pompa accesa: p on"         ,	 "Pompa accesa"			 ,   BT_PUMP_ON       },
	{"Pompa spenta: p off"        ,  "Pompa spenta"          ,   BT_PUMP_OFF      },
	{"Led accesi: led on"         ,  "Led accesi"            ,   BT_LED_ON        },
	{"Led spenti: led off" 	  	  ,  "Led spenti"            ,   BT_LED_OFF       },
	{"Imposta ore giorno: dayH"   ,  "Ore giorno impostate"  ,   BT_SET_DAY       },
	{"Imposta ore notte: nightH"  ,  "Ore notte impostate"   ,   BT_SET_NIGHT     },
	{"Imposta ore di mezzo: midH" ,  "Ore mezzo impostate"   ,   BT_SET_MIDHOUR   },
};

static String ReadCommand()
{
	String ReadedString = "";
	ReadedString = ReadString();
	return ReadedString;	
}


static uint8_t ReadHour()
{
	uint8_t Hour = 0;
	WriteResponse("");
	do
	{
		Hour = (uint8_t)ReadValue();
		DBG(Hour);
		OsDelay(100);
	}while(Hour == 25);
	DBG(Hour);
	return Hour; 
}

static void CheckBTConn()
{
	if(!SystemFlag.BTActive)
		DBG("TaskBT-> Controllo connessione");
	while(!SystemFlag.BTActive)
	{
		IsBTConnected();
		OsDelay(10);
	}	
}

static bool ExecuteCommand(uint8_t CmdValue)
{
	bool CmdExecuted = false;
	uint8_t HourToRead = 0;
	DBG("Task BT-> comando da eseguire" + String(CmdValue));
	switch(CmdValue)
	{
		case BT_PUMP_ON:
			SystemFlag.BypassNormalLcd = true;
			SystemFlag.BypassIgrosensorBT = true;
			SystemFlag.ManualPumpState = PUMP_ON;
			CmdExecuted = true;
			break;
		case BT_PUMP_OFF:
			SystemFlag.BypassNormalLcd = true;
			SystemFlag.BypassIgrosensorBT = true;
			SystemFlag.ManualPumpState = PUMP_OFF;
			CmdExecuted = true;
			break;
		case BT_LED_ON:
			SystemFlag.BypassNormalLcd = true;
			SystemFlag.BypassNormalDimming = true;
			Dimming = 255;
			SystemFlag.RefreshDimming = true;
			CmdExecuted = true;
			break;
		case BT_LED_OFF:
			SystemFlag.BypassNormalLcd = true;
			SystemFlag.BypassNormalDimming = true;
			Dimming = 0;
			SystemFlag.RefreshDimming = true;
			CmdExecuted = true;
			break;
		case BT_SET_DAY:
			HourToRead = ReadHour();
			if(HourToRead < 24)
			{
				DayTimeHours.DayHours = HourToRead;
				CmdExecuted = true;
			}
			else
			{
				CmdExecuted = false;
				WriteResponse("Comando non eseguito");
				WriteResponse("Ora inserita incorretta");
			}
			break;
		case BT_SET_NIGHT:
			HourToRead = ReadHour();
			if(HourToRead < 24)
			{
				DayTimeHours.NightHours = HourToRead;
				CmdExecuted = true;
			}
			else
			{
				CmdExecuted = false;
				WriteResponse("Comando non eseguito");
				WriteResponse("Ora inserita incorretta");
			}
			break;
		case BT_SET_MIDHOUR:
			HourToRead = ReadHour();
			if(HourToRead < 24)
			{
				DayTimeHours.TransitionHours = HourToRead;
				CmdExecuted = true;
			}
			else
			{
				CmdExecuted = false;
				WriteResponse("Comando non eseguito");
				WriteResponse("Ora inserita incorretta");
			}
			break;
		default:	
			break;
	}	
	return CmdExecuted;
} 


void TaskBT(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	uint16_t CheckBtActiveCnt = 0;
	bool BTPresent = false, CmdExecuted = false, CommandFound = false;
	String Command = "";
	uint8_t CmdRspIndex = 0;
	uint16_t ShowCmdListCnt = (30000/TASK_BT_DELAY);
	uint16_t LastTaskWakeTime = xTaskGetTickCount();
	bool First = true;
	BTPresent = BTInit();
	for(;;)
	{
		if(BTPresent)
		{
			CheckBTConn();
			if(SystemFlag.BTActive)
			{
				if(First)
				{
					WriteResponse("Smart Garden 001 connessa");
					First = false;
				}
				if(SystemFlag.BTActive)
				{
					DBG("Task BT-> dispositivo connesso");
					if(ShowCmdListCnt == (30000/TASK_BT_DELAY))
					{
						ShowCmdListCnt = 0;
						WriteResponse("Lista comandi:");
						for(CmdRspIndex = 0; CmdRspIndex < MAX_CMD_RSP_VALUES; CmdRspIndex++)
						{
							WriteResponse(PossibleCommandsResponse[CmdRspIndex].Command);
							OsDelay(50);
						}
						SystemFlag.BTActive = false;
						CheckBTConn();
					}
					else
					{
						Command = ReadCommand();
						if(Command != "")
						{
							DBG("Task BT-> comando letto:" + Command);
							for(CmdRspIndex = 0; CmdRspIndex < MAX_CMD_RSP_VALUES; CmdRspIndex++)
							{
								if(Command == PossibleCommandsResponse[CmdRspIndex].Command)
								{
									CommandFound = true;
									CmdExecuted = ExecuteCommand(PossibleCommandsResponse[CmdRspIndex].CmdRspValue);
									if(CmdExecuted)
									{
										WriteResponse("");
										WriteResponse(PossibleCommandsResponse[CmdRspIndex].Response);
										CmdExecuted = false;
									}
									break;
								}
							}
							if(CommandFound)
								CommandFound = false;
							else
							{
								WriteResponse("");
								WriteResponse(Command);
								WriteResponse("Comando non riconosciuto");
							}
						}
					}
					ShowCmdListCnt++;
				}
				else
				{
					SystemFlag.BypassNormalLcd = false;
					SystemFlag.BypassNormalDimming = false;		
					SystemFlag.BypassIgrosensorBT = false;			
				}
				OsDelayUntill(&LastTaskWakeTime, TASK_BT_DELAY);
			}
			else
			{
				BTPresent = BTInit();
				First = true;
				OsDelay(1000);
			}
		}
	}
}


#endif