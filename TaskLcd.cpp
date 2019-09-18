#include "TaskLcd.h"
#include "Smart-Garden.h"
#include <LCDLib.h>
#include "Time.h"
#include "Keyboard.h"
#include "IgroSensor.h"
#include "TaskBT.h"
#include "TaskDimmingSensors.h"
#include <EEPROM.h>
#include "EepromAddr.h"


#define MAX_LCD_CHARS	20

#define REGULAR_SCREEN_REFRESH_DELAY 	40


#define ICONS_ROW				ONE
#define ETH_ICON_COL			 6
#define BT_ICON_COL				 7
#define WHICH_HOUR_ICON_COL		 8 
#define PUMP_ICON_COL			 9
#define SD_LOG_ICON_COL			10

enum
{
	YES = 0,
	NO
};


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

const char *YesNo[] = 
{
	"Si",
	"No",
};

typedef enum
{
	HOUR = 0,
	MINUTE,
	MONTH,
	DAY,
	YEAR,
	MAX_SET_TIME_POS
}SET_TIME_POSITION;

extern uint8_t	HourLeft;

static bool SetHourDay = true, SetHourTransition = false, SetHourNight = false;
static bool ClearLCDBT = false;
static bool ToggleEthIcon = false;
static bool ToggleInfo = false, ToggleTempTDV = false;
static bool FirstEntry = true;
static uint8_t ActualTime = 0;
static uint8_t DayHoursSet = 0, TotDayHours = HOUR_IN_DAY;
static uint8_t OldDayTime = 0, OldPumpState = 0;
static uint8_t YesNoStr;
static uint8_t ButtonPress = NO_PRESS;
static uint8_t TimeDateValue[MAX_SET_TIME_POS] = {DFLT_HOUR, DFLT_MINUTE, DFLT_MONTH + 1, DFLT_DAY, DFLT_YEAR - 2000}; 
static uint8_t MaxTimeSetValue = 23, MinTimeSetValue = 0;
static uint8_t WichTimeStrValues = HOUR;
static uint32_t RegularScreenCnt = 0, SwitchToTempCnt = 0;
static uint32_t HourCalculated = 0, MinuteCalculated = 0, SecondCalculated = 0;
static CALENDAR_VAR SecondCalendarLcd;

static char Toprint[MAX_LCD_CHARS + 1];
static String ToPrintStr = "";

void WaitForOk(uint8_t Row)
{
	ButtonPress = NO_PRESS;
	while(ButtonPress != OK)
	{
		ButtonPress = CheckButtons();
		LCDPrintString(Row, CENTER_ALIGN, "Premere ok...");
	}
	ButtonPress = NO_PRESS;
}

void TaskLcdInitVar()
{
	DayHoursSet = DayTimeHours.DayHours;
}

void TaskLCD()  
{
	if(FirstEntry)
	{
		FirstEntry = false;
		ClearLCD();
	}
	switch(SystemFlag.LCD_SM)
	{
		case REGULAR_SCREEN_STATE:
			// Disegna orario
			snprintf(Toprint, MAX_LCD_CHARS, "%02d:%02d", TimeDate.Hour, TimeDate.Minute);
			LCDPrintString(ONE, LEFT_ALIGN, Toprint);
			snprintf(Toprint, MAX_LCD_CHARS, "%02d/%02d/%02d", TimeDate.Day, TimeDate.Month, TimeDate.Year % 100);
			LCDPrintString(ONE, RIGHT_ALIGN, Toprint);
			if(!ToggleInfo)
			{
				switch(SystemFlag.DayTime)
				{
					case IN_DAY:
						LCDShowIcon(SUN_ICON, ICONS_ROW, WHICH_HOUR_ICON_COL);
						break;
					case IN_NIGHT:
						LCDShowIcon(MOON_ICON, ICONS_ROW, WHICH_HOUR_ICON_COL);
						break;
					case TO_DAY:
					case TO_NIGHT:
						LCDShowIcon(MID_HOUR_ICON, ICONS_ROW, WHICH_HOUR_ICON_COL);
						break;
					default:
						break;
				}					
				if(SystemFlag.DayTime != OldDayTime)
				{
					ClearLCDLine(THREE);
					OldDayTime = SystemFlag.DayTime;
				}
				if(SystemFlag.PumpState != OldPumpState)
				{
					ClearLCDLine(FOUR);
					OldPumpState = SystemFlag.PumpState;
				}
				if(!ToggleTempTDV)
				{
					snprintf(Toprint, MAX_LCD_CHARS, "T.T.: %02d:%02d:%02d", TimeInLive.Hour, TimeInLive.Minute, TimeInLive.Second);
					LCDPrintString(TWO, CENTER_ALIGN, Toprint);		
				}
				else
				{
					ToPrintStr = "T: " + String(SensorsValues.Temperature) + "C";
					LCDPrintString(TWO, LEFT_ALIGN, ToPrintStr.c_str());
					ToPrintStr = "U: " + String(SensorsValues.Humidity) + "%";
					LCDPrintString(TWO, RIGHT_ALIGN, ToPrintStr.c_str());
				}
				LCDPrintString(THREE, CENTER_ALIGN, TimePeriod[SystemFlag.DayTime]);
				LCDPrintString(FOUR, CENTER_ALIGN, PumpState[SystemFlag.ManualPumpState]);
			}
			else
			{
				if(OldDayTime != SystemFlag.DayTime)
				{
					OldDayTime = SystemFlag.DayTime;
					ClearLCDLine(THREE);
				}
				if((SystemFlag.DayTime + 1) <= MAX_DAY_TIME - 1)
					snprintf(Toprint, MAX_LCD_CHARS, "Ore per: %s", TimePeriod[SystemFlag.DayTime + 1]);
				else
					snprintf(Toprint, MAX_LCD_CHARS, "Ore per: %s", TimePeriod[IN_DAY]);
				LCDPrintString(THREE, CENTER_ALIGN, Toprint);
				snprintf(Toprint, MAX_LCD_CHARS, "%02dh", HourLeft);
				LCDPrintString(FOUR, CENTER_ALIGN, Toprint);
			}
								
			/* DISEGNO ICONE DI STATO PERIFERICHE */
			// Icona Eth
			if(SystemFlag.EthCableConnected)
			{
				if(SystemFlag.EthClient)
				{
					LCDShowIcon(ETH_ICON, ICONS_ROW, ETH_ICON_COL);
				}
				else
					LCDShowIcon(ETH_ICON, ICONS_ROW, ETH_ICON_COL);
			}
			else
				ClearChar(ICONS_ROW, ETH_ICON_COL);
			// Icona BT
			if(SystemFlag.BTActive)
				LCDShowIcon(BT_ICON, ICONS_ROW, BT_ICON_COL);
			else
				ClearChar(ICONS_ROW, BT_ICON_COL);
			// Icona SD
			if(SystemFlag.SDLogging)
				LCDShowIcon(SD_ICON, ICONS_ROW, SD_LOG_ICON_COL);
			else
				ClearChar(ICONS_ROW, SD_LOG_ICON_COL);
			// Icona pompa
			if(SystemFlag.PumpState == IS_ON)
				LCDShowIcon(PUMP_ICON, ICONS_ROW, PUMP_ICON_COL);
			else
				ClearChar(ICONS_ROW, PUMP_ICON_COL);
			
			ButtonPress = CheckButtons();
			switch(ButtonPress)
			{
				case UP:
					SystemFlag.LCD_SM = SET_PUMP_STATE;
					break;
				case DOWN:
					SystemFlag.LCD_SM = SET_TIME_STATE;
					break;
				case OK:
					SystemFlag.LCD_SM = SET_HOUR_STATE;			
					break;
				case RESET_EXIT:
					SystemFlag.LCD_SM = RESET_STATE;	
					ClearLCD();
					break;
				default:
					break;
			}
			if(ButtonPress != NO_PRESS)
				ClearLCD();
			if(TakeSecondDelayTime(&RegularScreenCnt, 15))
			{
				RegularScreenCnt = 0;
				ToggleInfo = !ToggleInfo;
				ClearLCD();				
			}
			if(TakeSecondDelayTime(&SwitchToTempCnt, 5))
			{
				SwitchToTempCnt = 0;
				ToggleTempTDV = !ToggleTempTDV;
				ClearLCDLine(TWO);				
			}

			// Questo flag viene sempre refreshato per gestire la connessione asincrona BT
			ClearLCDBT = true;
			break;
		case SET_HOUR_STATE:
			snprintf(Toprint, MAX_LCD_CHARS, "%s%s", "Ore di:", SetTimePeriod[ActualTime]);
			LCDPrintString(ONE, CENTER_ALIGN, Toprint);
			snprintf(Toprint, MAX_LCD_CHARS, "%d%c", DayHoursSet, 'h');
			LCDPrintString(TWO, CENTER_ALIGN, Toprint);
			ButtonPress = CheckButtons();
			switch(ButtonPress)
			{
				case UP:
					if(DayHoursSet > 0)
						DayHoursSet--;
					else
						DayHoursSet = TotDayHours;
					ClearLCDLine(TWO);
					break;
				case DOWN:
					if(DayHoursSet < TotDayHours)
						DayHoursSet++;
					else
						DayHoursSet = 0;						
					ClearLCDLine(TWO);
					break;
				case OK:
					if(SetHourDay)
					{
						SetHourDay = false;
						SetHourNight = true;
						ActualTime++;
						DayTimeHours.DayHours = DayHoursSet;
						TotDayHours -= DayHoursSet;
						DayHoursSet = DayTimeHours.NightHours;
						ClearLCD();
					}
					else if(SetHourNight)
					{
						SetHourNight = false;
						SetHourTransition = true;	
						ActualTime++;
						DayTimeHours.NightHours = DayHoursSet;
						TotDayHours -= DayHoursSet;
						DayHoursSet = DayTimeHours.TransitionHours;
						ClearLCD();
					}
					else if(SetHourTransition)
					{
						SetHourTransition = false;
						SetHourDay = true;
						ActualTime = 0;
						DayTimeHours.TransitionHours = DayHoursSet;
						TotDayHours = HOUR_IN_DAY;
						DayHoursSet = DayTimeHours.DayHours;
						SecondForDimming = (SECONDS_HOUR(DayTimeHours.TransitionHours) / 255);
						SystemFlag.LCD_SM = REGULAR_SCREEN_STATE;
						FlagForSave.SaveHours = true;
						FlagForSave.SaveSecondDimming = true;
						switch(SystemFlag.DayTime)
						{
							case IN_DAY:
								HourLeft = DayTimeHours.DayHours;
								break;
							case TO_NIGHT:
							case TO_DAY:
								HourLeft = DayTimeHours.TransitionHours;
								break;
							case IN_NIGHT:
								HourLeft = DayTimeHours.NightHours;
								break;
							default:
								break;
						}
						ClearLCD();
					}
					break;
				case RESET_EXIT:			
					break;
				default:
					break;
			}
			break;
		case SET_PUMP_STATE:
			SystemFlag.BypassIgrosensor = true;
			// Disegna orario
			snprintf(Toprint, MAX_LCD_CHARS, "%02d:%02d", TimeDate.Hour, TimeDate.Minute);
			LCDPrintString(ONE, LEFT_ALIGN, Toprint);
			snprintf(Toprint, MAX_LCD_CHARS, "%02d/%02d/%02d", TimeDate.Day, TimeDate.Month, TimeDate.Year % 100);
			LCDPrintString(ONE, RIGHT_ALIGN, Toprint);	
			// Scrive stato pompa
			LCDPrintString(TWO, CENTER_ALIGN, PumpState[SystemFlag.ManualPumpState]);
			if(SystemFlag.ManualPumpState == IS_ON)
				LCDShowIcon(PUMP_ICON, ICONS_ROW, PUMP_ICON_COL);
			else
				ClearChar(ICONS_ROW, PUMP_ICON_COL);
			ButtonPress = CheckButtons();
			switch(ButtonPress)
			{
				case UP:
					if(SystemFlag.ManualPumpState == IS_ON)
						SystemFlag.ManualPumpState = IS_OFF;
					else
						SystemFlag.ManualPumpState = IS_ON;
					ClearLCDLine(TWO);
					break;
				case DOWN:
					if(SystemFlag.ManualPumpState == IS_ON)
						SystemFlag.ManualPumpState = IS_OFF;
					else
						SystemFlag.ManualPumpState = IS_ON;
					ClearLCDLine(TWO);
					break;
				case OK:
					SystemFlag.LCD_SM = REGULAR_SCREEN_STATE;
					SystemFlag.BypassIgrosensor = false;
					ClearLCD();
					break;
				case RESET_EXIT:			
					break;
				default:
					break;
			}		
			break;
		case SET_TIME_STATE:
			LCDPrintString(ONE, CENTER_ALIGN, "Impostare:");
			LCDPrintString(TWO, CENTER_ALIGN, TimeDateStr[WichTimeStrValues]);
			LCDPrintValue(THREE, CENTER_ALIGN, TimeDateValue[WichTimeStrValues]);
			ButtonPress = CheckButtons();
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
							WichTimeStrValues = MINUTE;
							ClearLCDLine(TWO);
							ClearLCDLine(THREE);
							break;
						case 1: // Setto i max/min per i mesi
							MaxTimeSetValue = 12;
							MinTimeSetValue = 1;
							WichTimeStrValues = MONTH;
							ClearLCDLine(TWO);
							ClearLCDLine(THREE);
							break;
						case 2: // Setto i max/min per i giorni
							MaxTimeSetValue = DayForMonth[TimeDateValue[2] - 1];
							MinTimeSetValue = 1;
							WichTimeStrValues = DAY;
							ClearLCDLine(TWO);
							ClearLCDLine(THREE);
							break;
						case 3: // Setto i max/min per gli anni
							MaxTimeSetValue = 99;
							MinTimeSetValue = 19;
							WichTimeStrValues = YEAR;
							ClearLCDLine(TWO);
							ClearLCDLine(THREE);
							break;
						case 4: // Resetto i max/min per le ore
							MaxTimeSetValue = 23;
							MinTimeSetValue = 0;
							WichTimeStrValues = HOUR;
							SetTimeDate(TimeDateValue[HOUR], TimeDateValue[MINUTE], TimeDateValue[DAY], TimeDateValue[MONTH], TimeDateValue[YEAR] + 2000);
							SystemFlag.LCD_SM = REGULAR_SCREEN_STATE;
							ClearLCD();	
							LCDPrintString(ONE, CENTER_ALIGN, "Ora/data impostata:");
							snprintf(Toprint, MAX_LCD_CHARS, "%02d:%02d %02d/%02d/%04d", TimeDateValue[HOUR], TimeDateValue[MINUTE], TimeDateValue[DAY], TimeDateValue[MONTH], TimeDateValue[YEAR] + 2000);
							LCDPrintString(TWO, CENTER_ALIGN, Toprint);
							WaitForOk(THREE);
							ClearLCD();
							break;
						default:
							break;						
					}
					break;
				case RESET_EXIT:			
					break;
				default:
					break;
			}	
			break;
		case RESET_STATE:
			LCDPrintString(ONE, CENTER_ALIGN, "Impostare valori");
			LCDPrintString(TWO, CENTER_ALIGN, "di default?");
			LCDPrintString(THREE, CENTER_ALIGN, YesNo[YesNoStr]);
			ButtonPress = CheckButtons();
			switch(ButtonPress)
			{
				case UP:
					if(YesNoStr == YES)
						YesNoStr = NO;
					else
						YesNoStr = YES;
					break;
				case DOWN:
					if(YesNoStr == NO)
						YesNoStr = YES;
					else
						YesNoStr = NO;
					break;
				case OK:	
					if(YesNoStr == YES)
					{
						EEPROM.write(DAY_TIME_ADDR, 255);
						ClearLCD();
						LCDPrintString(TWO, CENTER_ALIGN, "Reset in corso...");
						delay(2000);
						digitalWrite(RESET_PIN, LOW);
					}
					else
					{
						ClearLCD();
						SystemFlag.LCD_SM = REGULAR_SCREEN_STATE;
					}
					break;
				case RESET_EXIT:			
					break;
				default:
					break;
			}			
			break;
		case BT_LCD_STATE:
			if(ClearLCDBT)
			{
				ClearLCD();
				ClearLCDBT = false;
			}
			// Disegna orario
			snprintf(Toprint, MAX_LCD_CHARS, "%02d:%02d", TimeDate.Hour, TimeDate.Minute);
			LCDPrintString(ONE, LEFT_ALIGN, Toprint);
			snprintf(Toprint, MAX_LCD_CHARS, "%02d/%02d/%02d", TimeDate.Day, TimeDate.Month, TimeDate.Year % 100);
			LCDPrintString(ONE, RIGHT_ALIGN, Toprint);
			LCDShowIcon(BT_ICON, ICONS_ROW, BT_ICON_COL);
			if(SystemFlag.PumpState == IS_ON)
				LCDShowIcon(PUMP_ICON, ICONS_ROW, PUMP_ICON_COL);
			else
				ClearChar(ICONS_ROW, PUMP_ICON_COL);
			if(Dimming == 255)
				LCDShowIcon(SUN_ICON, ICONS_ROW, WHICH_HOUR_ICON_COL);
			else if(Dimming == 0)
				LCDShowIcon(MOON_ICON, ICONS_ROW, WHICH_HOUR_ICON_COL);
			else
				LCDShowIcon(MID_HOUR_ICON, ICONS_ROW, WHICH_HOUR_ICON_COL);
			LCDPrintString(TWO, CENTER_ALIGN, "Dispositivo BT");
			LCDPrintString(THREE, CENTER_ALIGN, "connesso");							
			break;
		default:
			break;
	}
}

