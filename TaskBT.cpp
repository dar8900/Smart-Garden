#include "TaskBT.h"
#include "Smart-Garden.h"
#include "IgroSensor.h"
#include "Bluetooth.h"
#include "Time.h"

#ifdef TASK_BT

const COMMAND_RESPONSE_S PossibleCommandsResponse[MAX_CMD_RSP_VALUES]
{
	{"Accendi pompa"     ,	"Pompa accesa"			,   BT_PUMP_ON       },
	{"Spegni pompa"      ,  "Pompa spenta"          ,   BT_PUMP_OFF      },
	{"Accendi led"       ,  "Led accesi"            ,   BT_LED_ON        },
	{"Spegni led" 	  	 ,  "Led spenti"            ,   BT_LED_OFF       },
	{"Imposta ore giorno",  "Ore giorno impostate"  ,   BT_SET_DAY       },
	{"Imposta ore notte" ,  "Ore notte impostate"   ,   BT_SET_NIGHT     },
	{"Imposta ore mezzo" ,  "Ore mezzo impostate"   ,   BT_SET_MIDHOUR   },
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
	do
	{
		Hour = (uint8_t)ReadValue();
		OsDelay(50);
	}while(Hour == 25);
	return Hour; 
}



void TaskBT(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	uint16_t CheckBtActiveCnt = 0;
	bool CmdExecuted = false, CommandFound = false;
	String Command = "";
	uint8_t CmdRspIndex = 0;
	uint8_t HourToRead = 0;
	for(;;)
	{
		IsBTActive();
		if(SystemFlag.BTActive)
		{
			Command = ReadCommand();
			if(Command != "")
			{
				for(CmdRspIndex = 0; CmdRspIndex < MAX_CMD_RSP_VALUES; CmdRspIndex++)
				{
					if(Command == PossibleCommandsResponse[CmdRspIndex].Command)
					{
						CommandFound = true;
						switch(PossibleCommandsResponse[CmdRspIndex].CmdRspValue)
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
								break;
							case BT_SET_MIDHOUR:
								break;
							default:	
								break;
						}
						if(CmdExecuted)
						{
							WriteResponse(PossibleCommandsResponse[CmdRspIndex].Response);
							CmdExecuted = false;
						}
						break;
					}
				}
				if(CommandFound)
					CommandFound = false;
				else
					WriteResponse("Comando non trovato");
			}
			
		}
		else
		{
			SystemFlag.BypassNormalLcd = false;
			SystemFlag.BypassNormalDimming = false;		
			SystemFlag.BypassIgrosensorBT = false;			
		}
		OsDelay(200);
	}
}


#endif