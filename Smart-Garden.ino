#include "Smart-Garden.h"
#include "Time.h"
#include "IgroSensor.h"
#include "TaskSelection.h"
#include "Keyboard.h"
#include "LCDLib.h"

#define REGULAR_SCREEN_REFRESH_DELAY 	40

GENERAL_FLAG SystemFlag;
FLAG_EEPROM FlagForSave;
int16_t Dimming = 0;

void TaskDimmingLed( void *pvParameters );
void TaskTime( void *pvParameters );
void TaskIgroSensorPump( void *pvParameters );
void TaskLCD(void *pvParameters);
void TaskKeyboard(void *pvParameters);
void TaskEeprom(void *pvParameters);

static void InitSystem()
{
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
	}
}

void setup() 
{
	Serial.begin(9600);
	pinMode(DIMMING_LED, OUTPUT);
	pinMode(PUMP, OUTPUT);
	pinMode(UP_BUTTON, INPUT);
	pinMode(DOWN_BUTTON, INPUT);
	pinMode(OK_BUTTON, INPUT);
	InitSystem();
	LCDInit();	
#ifdef TASK_DIMMING
	xTaskCreate(
	TaskDimmingLed
	,  (const portCHAR *) "DimmingLed"
	,  64  // Stack size
	,  NULL
	,  1  // Priority
	,  NULL );
#endif

#ifdef TASK_TIME	
	xTaskCreate(
	TaskTime
	,  (const portCHAR *) "Time"
	,  64  // Stack size
	,  NULL
	,  3  // Priority
	,  NULL );
#endif

#ifdef TASK_IGROSENSORPUMP	
		xTaskCreate(
	TaskIgroSensorPump
	,  (const portCHAR *) "IgroSensorPump"
	,  64  // Stack size
	,  NULL
	,  2  // Priority
	,  NULL );
#endif

#ifdef TASK_LCD	
		xTaskCreate(
	TaskLCD
	,  (const portCHAR *) "LCD"
	,  128 // Stack size
	,  NULL
	,  1  // Priority
	,  NULL );
#endif

#ifdef TASK_KEYBOARD	
		xTaskCreate(
	TaskKeyboard
	,  (const portCHAR *) "Keyboard"
	,  64  // Stack size
	,  NULL
	,  3  // Priority
	,  NULL );
#endif

#ifdef TASK_EEPROM	
		xTaskCreate(
	TaskEeprom
	,  (const portCHAR *) "Eeprom"
	,  64  // Stack size
	,  NULL
	,  0 // Priority
	,  NULL );
#endif
}

void loop() 
{

}

#ifdef TASK_DIMMING
void TaskDimmingLed(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	for (;;)
	{
		switch(SystemFlag.DayTime)
		{
			case IN_DAY:
				if(SystemFlag.RefreshDimming)
				{
					Dimming = 255;
					analogWrite(DIMMING_LED, Dimming);
					SystemFlag.RefreshDimming = false;
				}
				break;
			case IN_NIGHT:
				if(SystemFlag.RefreshDimming)
				{
					Dimming = 0;
					analogWrite(DIMMING_LED, Dimming);
					SystemFlag.RefreshDimming = false;
				}
				break;
			case TO_NIGHT:
				if(SecondCounter % SecondForDimming)
				{
					analogWrite(DIMMING_LED, Dimming--);
					if(Dimming < 0)
						Dimming = 0;
				}	
				break;
			case TO_DAY:
				if(SecondCounter % SecondForDimming)
				{
					analogWrite(DIMMING_LED, Dimming++);
					if(Dimming > 255)
						Dimming = 255;
				}
				break;
			default:
				break;			
		}
		OsDelay(500);
	}
}
#endif

#ifdef TASK_TIME
void TaskTime(void *pvParameters)  // This is a task.
{
	(void) pvParameters;

	for (;;)
	{
		CheckTime();
		OsDelay(100);
	}
}
#endif

#ifdef TASK_IGROSENSORPUMP
void TaskIgroSensorPump(void *pvParameters)  // This is a task.
{
	(void) pvParameters;

	for (;;)
	{
		if(!SystemFlag.BypassIgrosensor)
		{
			SensorsResponse();
			PumpAction(SystemFlag.TurnOnPumpAuto);
		}
		else
		{
			PumpAction(SystemFlag.ManualPumpState);
		}
		OsDelay(50);
	}
}
#endif

#ifdef TASK_LCD
void TaskLCD(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	char *TimePeriod[MAX_DAY_TIME] = 
	{
		"Giorno",
		"Sera",
		"Notte",
		"Mattino",
	};
	char *SetTimePeriod[3] = 
	{
		"Luce",
		"Buio",
		"Mezzo",
	};
	char *PumpState[MAX_PUMP_STATE] = 
	{
		"Pompa spenta",
		"Pompa accesa",
	};
	bool RegularScreen = true, SetHour = false, SetPump = false, SetTime = false;
	bool SetHourDay = true, SetHourTransition = false, SetHourNight = false;
	uint8_t ActualTime = 0;
	uint8_t Hours = 0, TotDayHours = 24;
	uint8_t RegularScreenCnt = 0;
	for (;;)
	{
		if(RegularScreen)
		{
			if(RegularScreenCnt < REGULAR_SCREEN_REFRESH_DELAY)
			{
				LCDPrintString(THREE, LEFT_ALIGN, String(TimePeriod[SystemFlag.DayTime]));
				LCDPrintString(FOUR, LEFT_ALIGN, String(PumpState[SystemFlag.ManualPumpState]));
			}
			if(RegularScreenCnt >= (REGULAR_SCREEN_REFRESH_DELAY + 10))
			{
				if((SystemFlag.DayTime + 1) < MAX_DAY_TIME - 1)
					LCDPrintString(THREE, CENTER_ALIGN, "Ore per:" + String(TimePeriod[SystemFlag.DayTime + 1]));
				else
					LCDPrintString(THREE, CENTER_ALIGN, "Ore per:" + String(TimePeriod[IN_DAY]));
				switch(SystemFlag.DayTime)
				{
					case IN_DAY:
						LCDPrintString(FOUR, CENTER_ALIGN, String(SECONDS_MINUTE(DayTimeHours.DayHours) - MINUTE_SECOND(SecondCounter)) + "h");
						break;
					case IN_NIGHT:
						LCDPrintString(FOUR, CENTER_ALIGN, String(SECONDS_MINUTE(DayTimeHours.NightHours) - MINUTE_SECOND(SecondCounter)) + "h");
						break;
					case TO_DAY:
					case TO_NIGHT:
						LCDPrintString(FOUR, CENTER_ALIGN, String(SECONDS_MINUTE(DayTimeHours.TransitionHours) - MINUTE_SECOND(SecondCounter)) + "h");
						break;
					default:
						break;
				}
			}
			switch(ButtonPress)
			{
				case UP:
					RegularScreen = false;
					SetPump = true;
					ClearLCD();
					break;
				case DOWN:
					RegularScreen = false;
					SetTime = true;
					ClearLCD();
					break;
				case OK:
					RegularScreen = false;
					SetHour = true;
					ClearLCD();
					break;
				default:
					break;
			}
			ButtonPress = NO_PRESS;
			RegularScreenCnt++;
			if(RegularScreenCnt == REGULAR_SCREEN_REFRESH_DELAY)
				ClearLCD();
			if(RegularScreenCnt == (REGULAR_SCREEN_REFRESH_DELAY + 10))
			{
				RegularScreenCnt = 0;
				ClearLCD();
			}
		}
		else if(SetPump)
		{
			SystemFlag.BypassIgrosensor = true;
			LCDPrintString(ONE, LEFT_ALIGN, String(PumpState[SystemFlag.ManualPumpState]));
			switch(ButtonPress)
			{
				case UP:
					if(SystemFlag.ManualPumpState == PUMP_ON)
						SystemFlag.ManualPumpState = PUMP_OFF;
					else
						SystemFlag.ManualPumpState = PUMP_ON;
					ClearLCDLine(ONE);
					break;
				case DOWN:
					if(SystemFlag.ManualPumpState == PUMP_ON)
						SystemFlag.ManualPumpState = PUMP_OFF;
					else
						SystemFlag.ManualPumpState = PUMP_ON;
					ClearLCDLine(ONE);
					break;
				case OK:
					SetPump = false;
					SystemFlag.BypassIgrosensor = false;
					RegularScreen = true;
					ClearLCD();
					break;
				default:
					break;
			}		
			ButtonPress = NO_PRESS;	
		}
		else if(SetHour)
		{
			LCDPrintString(ONE, LEFT_ALIGN, "Ore di:");
			LCDPrintString(ONE, RIGHT_ALIGN, SetTimePeriod[ActualTime]);
			LCDPrintString(TWO, CENTER_ALIGN, String(Hours) + "h");
			switch(ButtonPress)
			{
				case UP:
					if(Hours > 0)
						Hours--;
					else
						Hours = TotDayHours;
					ClearLCDLine(TWO);
					break;
				case DOWN:
					if(Hours < TotDayHours)
						Hours++;
					else
						Hours = 0;						
					ClearLCDLine(TWO);
					break;
				case OK:
					if(SetHourDay)
					{
						SetHourDay = false;
						SetHourNight = true;
						ActualTime++;
						DayTimeHours.DayHours = Hours;
						TotDayHours -= Hours;
						Hours = 0;
						ClearLCD();
					}
					else if(SetHourNight)
					{
						SetHourNight = false;
						SetHourTransition = true;	
						ActualTime++;
						DayTimeHours.NightHours = Hours;
						TotDayHours -= Hours;
						Hours = 0;
						ClearLCD();
					}
					else if(SetHourTransition)
					{
						SetHourTransition = false;
						SetHourDay = true;
						ActualTime = 0;
						DayTimeHours.TransitionHours = Hours;
						TotDayHours = 24;
						Hours = 0;
						SetHour = false;
						RegularScreen = true;
						FlagForSave.SaveHours = true;
						ClearLCD();
					}
					break;
				default:
					break;
			}
			ButtonPress = NO_PRESS;
		}
		else if(SetTime)
		{
						
		}
		OsDelay(250);
	}
}
#endif

#ifdef TASK_KEYBOARD
void TaskKeyboard(void *pvParameters)  // This is a task.
{
	(void) pvParameters;

	for (;;)
	{
		CheckButtons();
		OsDelay(25);
	}
}
#endif

#ifdef TASK_EEPROM
void TaskEeprom(void *pvParameters)  // This is a task.
{
	(void) pvParameters;

	for (;;)
	{
		if(FlagForSave.SaveDayTime)
		{
			FlagForSave.SaveDayTime = false;
			EEPROM.update(DAY_TIME_ADDR, SystemFlag.DayTime);
		}
		if(FlagForSave.SaveHours)
		{
			FlagForSave.SaveHours = false;
			EEPROM.update(DAY_HOUR_ADDR, DayTimeHours.DayHours);
			EEPROM.update(NIGHT_HOUR_ADDR, DayTimeHours.NightHours);
			EEPROM.update(TRANSITION_HOUR_ADDR, DayTimeHours.TransitionHours);
		}
		if(FlagForSave.SaveDimming)
		{
			FlagForSave.SaveDimming = false;
			EEPROM.update(DIMMING_ADDR ,(uint8_t)Dimming);
		}
		if(FlagForSave.SaveSecondCouter)
		{
			FlagForSave.SaveSecondCouter = false;
			EEPROM.put(SECOND_COUNTER_ADDR, SecondCounter);
		}
		if(FlagForSave.SaveCalendar)
		{
			FlagForSave.SaveCalendar = false;
			SaveTimeDate();
		}
		OsDelay(1000);
	}
}
#endif