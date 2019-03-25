#include "Smart-Garden.h"
#include "Time.h"
#include <LCDLib.h>
#include "TaskCreate.h"
#include "Icons.cpp"



GENERAL_FLAG SystemFlag;
FLAG_EEPROM FlagForSave;


static void InitSystem()
{
#ifdef DBG_SERIAL
	Serial.begin(9600);
#endif	
	pinMode(DIMMING_LED, OUTPUT);
	pinMode(PUMP, OUTPUT);
	pinMode(UP_BUTTON, INPUT);
	pinMode(DOWN_BUTTON, INPUT);
	pinMode(OK_BUTTON, INPUT);
	pinMode(BT_LED_ACTIVE, INPUT);

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
		FlagForSave.SaveHours = true;
		SetTimeDate(DFLT_HOUR, DFLT_MINUTE, DFLT_DAY, DFLT_MONTH, DFLT_YEAR);
		EEPROM.write(FIRST_GO_ADDR, 1);
		DBG("Init System-> Primo avvio");
	}
	else
	{
		Dimming = EEPROM.read(DIMMING_ADDR);
		EEPROM.get(SECOND_COUNTER_ADDR, SecondCounter);
		DayTimeHours.DayHours = EEPROM.read(DAY_HOUR_ADDR);
		DayTimeHours.NightHours = EEPROM.read(NIGHT_HOUR_ADDR);
		DayTimeHours.TransitionHours = EEPROM.read(TRANSITION_HOUR_ADDR);
		EEPROM.update(FIRST_GO_ADDR, 0);
		// LoadTimeDate(&TimeDate);
	}
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
	OSInit();
}

void loop() 
{

}

