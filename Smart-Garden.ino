#include "Smart-Garden.h"
#include "Time.h"
// #include "IgroSensor.h"
// #include "Keyboard.h"
#include "LCDLib.h"
// #include "SDLog.h"
#include "TaskCreate.h"


GENERAL_FLAG SystemFlag;
FLAG_EEPROM FlagForSave;



static void InitSystem()
{
	Serial.begin(9600);
	pinMode(DIMMING_LED, OUTPUT);
	pinMode(PUMP, OUTPUT);
	pinMode(UP_BUTTON, INPUT);
	pinMode(DOWN_BUTTON, INPUT);
	pinMode(OK_BUTTON, INPUT);
	pinMode(BT_LED_ACTIVE, INPUT);
	
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
		SetTimeDate(DFLT_HOUR, DFLT_MINUTE, DFLT_DAY, DFLT_MONTH, DFLT_YEAR, &TimeDate);
	}
	else
	{
		Dimming = EEPROM.read(DIMMING_ADDR);
		EEPROM.get(SECOND_COUNTER_ADDR, SecondCounter);
		DayTimeHours.DayHours = EEPROM.read(DAY_HOUR_ADDR);
		DayTimeHours.NightHours = EEPROM.read(NIGHT_HOUR_ADDR);
		DayTimeHours.TransitionHours = EEPROM.read(TRANSITION_HOUR_ADDR);
		LoadTimeDate(&TimeDate);
	}
}

void setup() 
{
	InitSystem();
	LCDInit();	
	OSInit();
}

void loop() 
{

}

