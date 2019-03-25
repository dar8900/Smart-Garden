#include "TaskLcd.h"
#include "Smart-Garden.h"
#include <LCDLib.h>
#include "Time.h"
#include "Keyboard.h"
#include "IgroSensor.h"
#include "TaskBT.h"


#define MAX_LCD_CHARS	20

#define REGULAR_SCREEN_REFRESH_DELAY 	40


#define ICONS_ROW				ONE
#define ETH_ICON_COL			 6
#define BT_ICON_COL				 8
#define WHICH_HOUR_ICON_COL		10
#define PUMP_ICON_COL			12
#define SD_LOG_ICON_COL			14

const char *TimePeriod[MAX_DAY_TIME] = 
{
	"Giorno",
	"Sera",
	"Notte",
	"Mattino",
};

const char *SetTimePeriod[3] = 
{
	"Luce",
	"Buio",
	"Mezzo",
};

const char *PumpState[MAX_PUMP_STATE] = 
{
	"Pompa spenta",
	"Pompa accesa",
};

const char *TimeDateStr[] = 
{
	"Ore",
	"Minuti",
	"Mese",
	"Giorno",
	"Anno",
};

#ifdef TASK_LCD

char Toprint[MAX_LCD_CHARS + 1];

static void WaitForOk(uint8_t Row)
{
	ButtonPress = NO_PRESS;
	while(ButtonPress != OK)
	{
		LCDPrintString(Row, CENTER_ALIGN, "Premere ok...");
		OsDelay(500);
	}
	ButtonPress = NO_PRESS;
}


void TaskLCD(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	bool RegularScreen = true, SetHour = false, SetPump = false, SetTime = false;
	bool SetHourDay = true, SetHourTransition = false, SetHourNight = false;
	bool ClearLCDBT = false;
	bool ToggleEthIcon = false;
	uint8_t ActualTime = 0;
	uint8_t Hours = 0, TotDayHours = 24;
	uint8_t RegularScreenCnt = 0;
	uint8_t TimeDateValue[5] = {0, 0, 0, 1, 19}; 
	uint8_t MaxTimeSetValue = 23, MinTimeSetValue = 0;
	uint8_t WichTimeStrValues = 0;
	uint16_t LastTaskWakeTime = xTaskGetTickCount();

	for (;;)
	{
		if(SystemFlag.Restart)
		{
			SystemFlag.Restart = false;
			RegularScreen = false;
			SetTime = true;
		}
		if(!SystemFlag.BypassNormalLcd)
		{
			if(!ClearLCDBT)
			{
				ClearLCD();
				ClearLCDBT = true;
			}
			if(RegularScreen)
			{
				// Disegna orario
				snprintf(Toprint, MAX_LCD_CHARS, "%02d:%02d", TimeDate.Hour, TimeDate.Minute);
				LCDPrintString(ONE, LEFT_ALIGN, Toprint);
				snprintf(Toprint, MAX_LCD_CHARS, "%02d/%02d/%02d", TimeDate.Hour, TimeDate.Minute, TimeDate.Year % 100);
				LCDPrintString(ONE, RIGHT_ALIGN, Toprint);
				if(RegularScreenCnt < REGULAR_SCREEN_REFRESH_DELAY)
				{
					switch(SystemFlag.DayTime)
					{
						case IN_DAY:
							LCDShowIcon(SUN_ICON, ICONS_ROW, WHICH_HOUR_ICON_COL);
							break;
						case IN_NIGHT:
							LCDShowIcon(MOON_ICON, ICONS_ROW, WHICH_HOUR_ICON_COL);
						default:
							break;
					}					
					LCDPrintString(THREE, LEFT_ALIGN, TimePeriod[SystemFlag.DayTime]);
					LCDPrintString(FOUR, LEFT_ALIGN, PumpState[SystemFlag.ManualPumpState]);
				}
				if(RegularScreenCnt >= REGULAR_SCREEN_REFRESH_DELAY)
				{
					if((SystemFlag.DayTime + 1) < MAX_DAY_TIME - 1)
						snprintf(Toprint, MAX_LCD_CHARS, "Ore per: %s", TimePeriod[SystemFlag.DayTime + 1]);
					else
						snprintf(Toprint, MAX_LCD_CHARS, "Ore per: %s", TimePeriod[IN_DAY]);
					LCDPrintString(THREE, CENTER_ALIGN, Toprint);

					switch(SystemFlag.DayTime)
					{
						case IN_DAY:
							snprintf(Toprint, MAX_LCD_CHARS, "%d%c", (SECONDS_MINUTE(DayTimeHours.DayHours) - MINUTE_SECOND(SecondCounter)), 'h');							
							break;
						case IN_NIGHT:
							snprintf(Toprint, MAX_LCD_CHARS, "%d%c", (SECONDS_MINUTE(DayTimeHours.NightHours) - MINUTE_SECOND(SecondCounter)), 'h');						
							break;
						case TO_DAY:
						case TO_NIGHT:
							snprintf(Toprint, MAX_LCD_CHARS, "%d%c", (SECONDS_MINUTE(DayTimeHours.TransitionHours) - MINUTE_SECOND(SecondCounter)), 'h');
							break;
						default:
							break;
					}
					LCDPrintString(FOUR, CENTER_ALIGN, Toprint);

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
				
				// Disegno icone di stato periferiche
				if(SystemFlag.EthCableConnected)
				{
					if(SystemFlag.EthClient)
					{
						if(ToggleEthIcon)
							LCDShowIcon(ETH_ICON, ICONS_ROW, ETH_ICON_COL);
						else
							ClearChar(ICONS_ROW, ETH_ICON_COL);
						ToggleEthIcon = !ToggleEthIcon;
					}
					else
						LCDShowIcon(ETH_ICON, ICONS_ROW, ETH_ICON_COL);
				}
				else
					ClearChar(ICONS_ROW, ETH_ICON_COL);
				if(SystemFlag.BTActive)
					LCDShowIcon(BT_ICON, ICONS_ROW, BT_ICON_COL);
				else
					ClearChar(ICONS_ROW, BT_ICON_COL);
				if(SystemFlag.SDLogging)
					LCDShowIcon(SD_ICON, ICONS_ROW, SD_LOG_ICON_COL);
				else
					ClearChar(ICONS_ROW, SD_LOG_ICON_COL);
			}
			else if(SetPump)
			{
				SystemFlag.BypassIgrosensor = true;
				LCDPrintString(TWO, LEFT_ALIGN, PumpState[SystemFlag.ManualPumpState]);
				if(SystemFlag.ManualPumpState == PUMP_ON)
					LCDShowIcon(PUMP_ICON, ICONS_ROW, PUMP_ICON_COL);
				else
					ClearChar(ICONS_ROW, PUMP_ICON_COL);
				switch(ButtonPress)
				{
					case UP:
						if(SystemFlag.ManualPumpState == PUMP_ON)
							SystemFlag.ManualPumpState = PUMP_OFF;
						else
							SystemFlag.ManualPumpState = PUMP_ON;
						ClearLCDLine(TWO);
						break;
					case DOWN:
						if(SystemFlag.ManualPumpState == PUMP_ON)
							SystemFlag.ManualPumpState = PUMP_OFF;
						else
							SystemFlag.ManualPumpState = PUMP_ON;
						ClearLCDLine(TWO);
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
				snprintf(Toprint, MAX_LCD_CHARS, "%s%d", "Ore di:", SetTimePeriod[ActualTime]);
				LCDPrintString(ONE, CENTER_ALIGN, Toprint);
				snprintf(Toprint, MAX_LCD_CHARS, "%d%c", Hours, 'h');
				LCDPrintString(TWO, CENTER_ALIGN, Toprint);
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
				LCDPrintValue(THREE, CENTER_ALIGN, TimeDateValue[WichTimeStrValues]);
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
								SetTimeDate(TimeDateValue[0], TimeDateValue[1], TimeDateValue[3], TimeDateValue[2], TimeDateValue[4] + 2000);
								SetTime = false;
								RegularScreen = true;
								ClearLCD();	
								LCDPrintString(ONE, CENTER_ALIGN, "Ora/data impostata:");
								snprintf(Toprint, MAX_LCD_CHARS, "%02d:%02d %02d/%02d/%04d", TimeDateValue[0], TimeDateValue[1], TimeDateValue[3], TimeDateValue[2], TimeDateValue[4] + 2000);
								LCDPrintString(TWO, CENTER_ALIGN, Toprint);
								WaitForOk(THREE);
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
		}
		else
		{
			if(ClearLCDBT)
			{
				ClearLCD();
				ClearLCDBT = false;
			}
			LCDShowIcon(BT_ICON, ICONS_ROW, BT_ICON_COL);
			if(SystemFlag.ManualPumpState == PUMP_ON)
				LCDShowIcon(PUMP_ICON, ICONS_ROW, PUMP_ICON_COL);
			else
				ClearChar(ICONS_ROW, PUMP_ICON_COL);
			if(Dimming == 255)
				LCDShowIcon(SUN_ICON, ICONS_ROW, WHICH_HOUR_ICON_COL);
			else
				ClearChar(ICONS_ROW, WHICH_HOUR_ICON_COL);
				
			LCDPrintString(TWO, CENTER_ALIGN, "Dispositivo BT");
			LCDPrintString(THREE, CENTER_ALIGN, "connesso");
		}
		OsDelayUntill(&LastTaskWakeTime, TASK_LCD_DELAY);
	}
}
#endif
