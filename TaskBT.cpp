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

void TaskBT(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	uint16_t CheckBtActiveCnt = 0;
	String Command = "";
	uint8_t CmdRspIndex = 0;
	for(;;)
	{
		IsBTActive();
		if(SystemFlag.BTActive)
		{
			for(CmdRspIndex = 0; CmdRspIndex < MAX_CMD_RSP_VALUES; CmdRspIndex++)
			{
				Command = ReadCommand();
				if(Command == PossibleCommandsResponse[CmdRspIndex].Command)
				{
					switch(PossibleCommandsResponse[CmdRspIndex].CmdRspValue)
					{
						case BT_PUMP_ON:
							SystemFlag.BypassNormalLcd = true;
							SystemFlag.BypassIgrosensor = true;
							SystemFlag.ManualPumpState = PUMP_ON;
							break;
						case BT_PUMP_OFF:
							SystemFlag.BypassNormalLcd = true;
							SystemFlag.BypassIgrosensor = true;
							SystemFlag.ManualPumpState = PUMP_OFF;
							break;
						case BT_LED_ON:
							SystemFlag.BypassNormalLcd = true;
							SystemFlag.BypassNormalDimming = true;
							Dimming = 255;
							SystemFlag.RefreshDimming = true;
							break;
						case BT_LED_OFF:
							SystemFlag.BypassNormalLcd = true;
							SystemFlag.BypassNormalDimming = true;
							Dimming = 0;
							SystemFlag.RefreshDimming = true;
							break;
						case BT_SET_DAY:
							break;
						case BT_SET_NIGHT:
							break;
						case BT_SET_MIDHOUR:
							break;
						default:	
							break;
					}
					WriteResponse(PossibleCommandsResponse[CmdRspIndex].Response);
				}
			}
			
		}
		OsDelay(100);
	}
}

#endif