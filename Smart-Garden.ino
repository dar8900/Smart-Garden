#include <EEPROM.h>
#include "Smart-Garden.h"
#include "Time.h"
#include "Bluetooth.h"
#include <LCDLib.h>
#include "TaskLcd.h"
#include "Icons.cpp"
#include "TaskBT.h"
#include "TaskDimmingSensors.h"
#include "TaskEeprom.h"

#define TASK_BT

#undef CHECK_CYCLE_TIME

extern uint32_t NumberOfWrites;

GENERAL_FLAG SystemFlag;
FLAG_EEPROM FlagForSave;

#ifdef CHECK_CYCLE_TIME
static uint32_t CompleteCycleTime;
#endif

void TempHydroInit(void);

static void InitBluetoothCard()
{
	if(BTInit())
	{
		BT_SM = DEVICE_CONNECTION_STATE;
	}
}


static void InitSystem()
{
#ifdef DBG_SERIAL
	Serial.begin(9600);
#endif	

	Serial2.begin(9600);

	pinMode(DIMMING_LED, OUTPUT);
	pinMode(PUMP, OUTPUT);
	pinMode(UP_BUTTON, INPUT);
	pinMode(DOWN_BUTTON, INPUT);
	pinMode(OK_BUTTON, INPUT);
	pinMode(BT_LED_ACTIVE, INPUT);
	pinMode(RED_LED, OUTPUT),
	pinMode(YELLOW_LED, OUTPUT),
	pinMode(GREEN_LED, OUTPUT),
	
	
	SystemFlag.Restart = true;
	
	SystemFlag.DayTime = EEPROM.read(DAY_TIME_ADDR);
	
	// Se è il primo avvio
	if(SystemFlag.DayTime > MAX_DAY_TIME)
	{
		SystemFlag.DayTime = IN_DAY;
		Dimming = 255;
		SecondCounter = 0;
		LogDayTime();
		LogDimming();
		LogSecondCounter();
		DayTimeHours.DayHours = DAY_HOURS_DFL;
		DayTimeHours.NightHours = NIGHT_HOURS_DFL;
		DayTimeHours.TransitionHours = TRANSITION_HOURS_DFL;
		SecondForDimming = (SECONDS_MINUTE(TRANSITION_HOURS_DFL) / 255);
		FlagForSave.SaveHours = true;
		FlagForSave.SaveSecondDimming = true;
		SetTimeDate(DFLT_HOUR, DFLT_MINUTE, DFLT_DAY, DFLT_MONTH, DFLT_YEAR);
		EEPROM.write(FIRST_GO_ADDR, 1);
	}
	else
	{
		Dimming = EEPROM.read(DIMMING_ADDR);
		EEPROM.get(SECOND_COUNTER_ADDR, SecondCounter);
		DayTimeHours.DayHours = EEPROM.read(DAY_HOUR_ADDR);
		DayTimeHours.NightHours = EEPROM.read(NIGHT_HOUR_ADDR);
		DayTimeHours.TransitionHours = EEPROM.read(TRANSITION_HOUR_ADDR);
		EEPROM.get(SECOND_DIMMING_ADDR, SecondForDimming);
		EEPROM.update(FIRST_GO_ADDR, 0);
		if(EEPROM.read(FIRST_GO_ADDR) == 0)
			EEPROM.get(NUMBER_OF_WRITES_ADDR, NumberOfWrites);
		
		// FlagForSave.ClearSecondCounter = true;
		// LoadTimeDate(&TimeDate);
	}
	TempHydroInit();
	TaskLcdInitVar();
	InitBluetoothCard();
	SystemFlag.RefreshDimming = true;
}

static void CreateSystemIcons()
{
	LCDCreateIcon(BTIcon, BT_ICON);
	LCDCreateIcon(SDIcon, SD_ICON);
	LCDCreateIcon(SunIcon, SUN_ICON);
	LCDCreateIcon(MoonIcon, MOON_ICON);
	LCDCreateIcon(EthIcon, ETH_ICON);
	LCDCreateIcon(PumpIcon, PUMP_ICON);	
}



void setup() 
{
	RtcInit();
	InitSystem();
	LCDInit();
	CreateSystemIcons();
	LCDPrintString(TWO, CENTER_ALIGN, "Home Microtech");
	delay(1500);
	ClearLCD();
	RefreshCalendar(&TimeDate);
}

void loop() 

{
#ifdef CHECK_CYCLE_TIME	
	if(CompleteCycleTime == 0)
		CompleteCycleTime = millis();
#endif	
	// Refresh tempo
	CheckTime();
#ifdef TASK_BT	
	// Macchina a stati per controllo BT
	TaskBT();
#endif	
	// Gestisco il dimming e i sensori
	TaskDimmingLed_Igro();
	// Refresh macchina a stati per LCD
	TaskLCD();
	// Aggiorno la eeprom se sono stati alzati i flag
	TaskEeprom();
	
#ifdef CHECK_CYCLE_TIME	
	CompleteCycleTime = millis() - CompleteCycleTime;
	DBG("Tempo per ciclo completo:");
	DBG(CompleteCycleTime);
	CompleteCycleTime = 0;
	ClearLCD();
	WaitForOk(TWO);
#endif	
}

