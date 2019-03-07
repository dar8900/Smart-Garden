#include "TaskLcd.h"
#include "Smart-Garden.h"
#include "LCDLib.h"
#include "Time.h"
#include "Keyboard.h"
#include "IgroSensor.h"
#include "TaskBT.h"

#define REGULAR_SCREEN_REFRESH_DELAY 	40

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
	char *TimeDateStr[] = 
	{
		"Ore",
		"Minuti",
		"Mese",
		"Giorno",
		"Anno",
	};
	bool RegularScreen = true, SetHour = false, SetPump = false, SetTime = false;
	bool SetHourDay = true, SetHourTransition = false, SetHourNight = false;
	uint8_t ActualTime = 0;
	uint8_t Hours = 0, TotDayHours = 24;
	uint8_t RegularScreenCnt = 0;
	uint8_t TimeDateValue[5] = {0, 0, 0, 1, 19}; 
	uint8_t MaxTimeSetValue = 23, MinTimeSetValue = 0;
	uint8_t WichTimeStrValues = 0;
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
			LCDPrintString(ONE, CENTER_ALIGN, "Imposta");
			LCDPrintString(TWO, CENTER_ALIGN, TimeDateStr[WichTimeStrValues]);
			LCDPrintString(THREE, CENTER_ALIGN, String(TimeDateValue[WichTimeStrValues]));
			switch(ButtonPress)
			{
				case UP:
					if(TimeDateValue[WichTimeStrValues] > MinTimeSetValue)
						TimeDateValue[WichTimeStrValues]--;
					else
						TimeDateValue[WichTimeStrValues] = MaxTimeSetValue;
					ClearLCDLine(THREE);
					break;
				case DOWN:
					if(TimeDateValue[WichTimeStrValues] < MaxTimeSetValue)
						TimeDateValue[WichTimeStrValues]++;
					else
						TimeDateValue[WichTimeStrValues] = MinTimeSetValue;	
					ClearLCDLine(THREE);
					break;
				case OK:
					switch(WichTimeStrValues)
					{
						case 0: // Setto i max/min per i minuti
							MaxTimeSetValue = 59;
							MinTimeSetValue = 0;
							WichTimeStrValues++;
							ClearLCDLine(TWO);
							ClearLCDLine(THREE);
							break;
						case 1: // Setto i max/min per i mesi
							MaxTimeSetValue = 12;
							MinTimeSetValue = 1;
							WichTimeStrValues++;
							ClearLCDLine(TWO);
							ClearLCDLine(THREE);
							break;
						case 2: // Setto i max/min per i giorni
							MaxTimeSetValue = DayForMonth[TimeDateValue[2] - 1];
							MinTimeSetValue = 1;
							WichTimeStrValues++;
							ClearLCDLine(TWO);
							ClearLCDLine(THREE);
							break;
						case 3: // Setto i max/min per gli anni
							MaxTimeSetValue = 99;
							MinTimeSetValue = 19;
							WichTimeStrValues++;
							ClearLCDLine(TWO);
							ClearLCDLine(THREE);
							break;
						case 4: // Resetto i max/min per le ore
							MaxTimeSetValue = 23;
							MinTimeSetValue = 0;
							WichTimeStrValues = 0;
							SetTimeDate(TimeDateValue[0], TimeDateValue[1], TimeDateValue[3], TimeDateValue[2] - 1, TimeDateValue[4] + 2000, &TimeDate);
							SetTime = false;
							RegularScreen = true;
							ClearLCD();							
							break;
						default:
							break;						
					}
					break;
				default:
					break;
			}		
			ButtonPress = NO_PRESS;							
		}
		OsDelay(250);
	}
}
#endif
