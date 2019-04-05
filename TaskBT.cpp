#include "TaskBT.h"
#include "Smart-Garden.h"
#include "IgroSensor.h"
#include "Bluetooth.h"
#include "Time.h"


#define BT_DEVICE_CONN_DELAY	10000
#define TIMEOUT_USER_LIVE		 5000

#define SEND_NL	WriteResponse("");

static uint16_t CheckBtActiveCnt = 0;
static bool BTPresent = false, UserNotPresentButConnected = false;
static String Command = "";
static uint8_t CmdRspIndex = 0;
static bool First = true;
static uint32_t CommandListTime;

uint8_t BT_SM;

const COMMAND_RESPONSE_S PossibleCommandsResponse[MAX_CMD_RSP_VALUES]
{
	{"Pompa accesa: p on"         , "p on"    ,  "Pompa accesa"			 ,   BT_PUMP_ON       },
	{"Pompa spenta: p off"        , "p off"   ,  "Pompa spenta"          ,   BT_PUMP_OFF      },
	{"Led accesi: led on"         , "led on"  ,  "Led accesi"            ,   BT_LED_ON        },
	{"Led spenti: led off" 	  	  , "led off" ,  "Led spenti"            ,   BT_LED_OFF       },
	{"Imposta ore giorno: dayH"   , "dayH"    ,  "Ore giorno impostate"  ,   BT_SET_DAY       },
	{"Imposta ore notte: nightH"  , "nightH"  ,  "Ore notte impostate"   ,   BT_SET_NIGHT     },
	{"Imposta ore di mezzo: midH" , "midH"    ,  "Ore mezzo impostate"   ,   BT_SET_MIDHOUR   },
	{"Imposta orario: settime"    , "settime" ,  "Ora impostata"         ,   BT_SET_TIME      },
	{"Imposta data: setdate"      , "setdate" ,  "Data impostata"        ,   BT_SET_DATE      },
};

static String ReadCommand()
{
	return ReadString();
}


static uint8_t ReadHour()
{
	uint8_t Hour = 0;
	WriteResponse("");
	do
	{
		Hour = (uint8_t)ReadValue();
		delay(50);
	}while(Hour == INVALID_BT_HOUR);
	return Hour; 
}

static uint16_t ReadTimeDate()
{
	uint16_t HourDate = 0;
	WriteResponse("");
	do
	{
		HourDate = ReadValue();
		delay(50);
	}while(HourDate == INVALID_BT_HOUR);
	return HourDate; 
}

// static bool CheckBTConn()
// {
	// uint8_t ReTryCnt = 52;
	// bool Connected = false;
	// while(!SystemFlag.BTActive)
	// {
		// IsBTConnected();
		// ReTryCnt--;
		// if(ReTryCnt == 0)
			// break;
		// delay(20);
	// }	
	// if(SystemFlag.BTActive)
		// Connected = true;
	// return Connected;
// }

static void WriteCommandList()
{
	WriteResponse("Lista comandi:");
	for(CmdRspIndex = 0; CmdRspIndex < MAX_CMD_RSP_VALUES; CmdRspIndex++)
	{
		WriteResponse(PossibleCommandsResponse[CmdRspIndex].Command);
		delay(50);
	}
}

static bool IsUserLive()
{
	uint16_t Cnt = 0;
	Command = "";
	WriteResponse("");
	WriteResponse("Ancora connesso?");
	while(Command == "")
	{
		Command == ReadCommand();
		if(Command == "s")
		{
			Command = "";
			return true;
		}
		Cnt++;
		if(Cnt == TIMEOUT_USER_LIVE)
			return false;
		delay(1);
	}
}

static bool ExecuteCommand(uint8_t CmdValue)
{
	bool CmdExecuted = false;
	uint8_t HourToRead = 0;
	uint16_t HourDateTime[3] = {0};
	DBG("Task BT-> comando da eseguire");
	DBG(CmdValue);
	switch(CmdValue)
	{
		case BT_PUMP_ON:
			SystemFlag.ManualPumpState = PUMP_ON;
			CmdExecuted = true;
			break;
		case BT_PUMP_OFF:
			SystemFlag.ManualPumpState = PUMP_OFF;
			CmdExecuted = true;
			break;
		case BT_LED_ON:
			Dimming = 255;
			SystemFlag.RefreshDimming = true;
			CmdExecuted = true;
			break;
		case BT_LED_OFF:
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
				WriteResponse("");
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
				WriteResponse("");
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
				WriteResponse("");
				WriteResponse("Comando non eseguito");
				WriteResponse("Ora inserita incorretta");
			}
			break;
		case BT_SET_TIME:
			WriteResponse("Imposta l'ora");
			WriteResponse("");
			HourDateTime[0] = ReadTimeDate();
			if(HourDateTime[0] > 23)
			{
				CmdExecuted = false;
				break;
			}
			WriteResponse("");
			WriteResponse("Imposta i minuti");
			WriteResponse("");
			HourDateTime[1] = ReadTimeDate();
			if(HourDateTime[1] > 59)
			{
				CmdExecuted = false;
				break;
			}
			SetTimeDate((uint8_t)HourDateTime[0], (uint8_t)HourDateTime[1], TimeDate.Day, TimeDate.Month, TimeDate.Year);
			CmdExecuted = true;
			break;
		case BT_SET_DATE:
			WriteResponse("Imposta il mese");
			WriteResponse("");
			HourDateTime[0] = ReadTimeDate();
			if(HourDateTime[0] > 12 || HourDateTime[0] < 1)
			{
				CmdExecuted = false;
				break;
			}
			WriteResponse("");
			WriteResponse("Imposta il giorno");
			WriteResponse("");
			HourDateTime[1] = ReadTimeDate();
			if(HourDateTime[1] > DayForMonth[HourDateTime[0] - 1] || HourDateTime[1] < 1)
			{
				CmdExecuted = false;
				break;
			}
			WriteResponse("");
			WriteResponse("Imposta l'anno");
			WriteResponse("");
			HourDateTime[2] = ReadTimeDate();
			if(HourDateTime[2] > 2099 || HourDateTime[2] < 2019)
			{
				CmdExecuted = false;
				break;
			}
			SetTimeDate(TimeDate.Hour, TimeDate.Minute, (uint8_t)HourDateTime[1], (uint8_t)HourDateTime[0], HourDateTime[2]);
			CmdExecuted = true;
			break;
			break;
		default:	
			break;
	}	
	return CmdExecuted;
} 

static bool CheckAndExecuteCommand()
{
	bool CommandFound = false, CmdExecuted = false;
	DBG("Task BT-> comando letto:");
	DBG(Command);
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
			}
			break;
		}
	}
	if(!CommandFound)
	{
		WriteResponse("");
		WriteResponse(Command);
		WriteResponse("");
		WriteResponse("Comando non riconosciuto");
	}
	if(CommandFound && CmdExecuted)
		return true;
	else
		return false;
}

void TaskBT()  
{
	switch(BT_SM)
	{
		case CARD_CONNECTION_STATE:
			if(BTInit())
			{
				BT_SM = DEVICE_CONNECTION_STATE;
			}
			else
			{
				BT_SM = CARD_CONNECTION_STATE;
				First = true;
			}
			break;
		case DEVICE_CONNECTION_STATE:
			if(IsDeviceBTConnected())
			{
				BT_SM = CHECK_OR_SHOW_COMMAND_LIST_STATE;
				if(!UserNotPresentButConnected)
					SystemFlag.LCD_SM = BT_LCD_STATE;
				else
				{
					UserNotPresentButConnected = false;
					SystemFlag.LCD_SM = REGULAR_SCREEN_STATE;
				}
				SystemFlag.BypassIgrosensorBT = true;
				SystemFlag.BypassNormalDimming = true;
				SystemFlag.BTActive = true;
			}
			else
			{
				BT_SM = DEVICE_CONNECTION_STATE;
				SystemFlag.LCD_SM = REGULAR_SCREEN_STATE;
				SystemFlag.BypassNormalDimming = false;		
				SystemFlag.BypassIgrosensorBT = false;	
				First = true;
			}
			break;
		case CHECK_OR_SHOW_COMMAND_LIST_STATE:
			if(First)
			{
				WriteResponse("Smart Garden 001 connessa");
				DBG("Task BT-> dispositivo connesso");
				First = false;
				WriteCommandList();
			}
			if(CommandListTime == 0)
				CommandListTime = millis();
			if(millis() - CommandListTime >= BT_DEVICE_CONN_DELAY)
			{
				CommandListTime = 0;
				// WriteCommandList();
				if(!IsUserLive())
				{
					First = true;
					SystemFlag.BTActive = false;
					UserNotPresentButConnected = true;
					BT_SM = DEVICE_CONNECTION_STATE;
				}
				else
				{
					UserNotPresentButConnected = false;
					BT_SM = CHECK_OR_SHOW_COMMAND_LIST_STATE;
				}
				break;
			}
			else 
			{
				Command = ReadCommand();
				if(Command != "")
					BT_SM = EXECUTE_COMMAND_STATE;
				else 
					break;
			}
		case EXECUTE_COMMAND_STATE:
			CheckAndExecuteCommand();
			Command = "";
			BT_SM = CHECK_OR_SHOW_COMMAND_LIST_STATE;
			break;
		default:
			break;		
	}
}
