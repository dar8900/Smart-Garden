#include "Smart-Garden.h"
#include "Time.h"
#include "IgroSensor.h"

GENERAL_FLAG SystemFlag;
int16_t Dimming = 0;

void TaskDimmingLed( void *pvParameters );
void TaskTime( void *pvParameters );
void TaskIgroSensorPump( void *pvParameters );

static void InitSystem()
{

	if(EEPROM.read(IN_DAY_ADDR) > 1)
	{
		SystemFlag.InDay = true;
		SystemFlag.InNight = false;
		SystemFlag.ToDay = false;
		SystemFlag.ToNight = false;
		Dimming = 255;
		SecondCounter = SECONDS_HOUR(DAY_HOURS);
		LogFlag();
		LogDimming();
		LogSecondCounter();
	}
	else
	{
		SystemFlag.InDay = EEPROM.read(IN_DAY_ADDR);
		SystemFlag.InNight = EEPROM.read(IN_NIGHT_ADDR);
		SystemFlag.ToDay = EEPROM.read(TO_DAY_ADDR);
		SystemFlag.ToNight = EEPROM.read(TO_NIGHT_ADDR);
		Dimming = EEPROM.read(DIMMING_ADDR);
		EEPROM.get(SECOND_COUNTER_ADDR, SecondCounter);
	}
}

void setup() 
{
	Serial.begin(9600);
	pinMode(DIMMING_LED, OUTPUT);
	pinMode(PUMP, OUTPUT);
	InitSystem();
	
	xTaskCreate(
	TaskDimmingLed
	,  (const portCHAR *) "DimmingLed"
	,  64  // Stack size
	,  NULL
	,  1  // Priority
	,  NULL );
	
	xTaskCreate(
	TaskTime
	,  (const portCHAR *) "Time"
	,  64  // Stack size
	,  NULL
	,  3  // Priority
	,  NULL );
	
		xTaskCreate(
	TaskIgroSensorPump
	,  (const portCHAR *) "IgroSensorPump"
	,  64  // Stack size
	,  NULL
	,  2  // Priority
	,  NULL );

}

void loop() 
{

}


void TaskDimmingLed(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	for (;;)
	{
		if(SystemFlag.InDay)
		{
			if(SystemFlag.RefreshDimming)
			{
				Dimming = 255;
				analogWrite(DIMMING_LED, Dimming);
				SystemFlag.RefreshDimming = false;
			}
		}
		if(SystemFlag.InNight)
		{
			if(SystemFlag.RefreshDimming)
			{
				Dimming = 0;
				analogWrite(DIMMING_LED, Dimming);
				SystemFlag.RefreshDimming = false;
			}
		}
		if(SystemFlag.ToDay)
		{
			if(SecondCounter % SecondForDimming)
			{
				analogWrite(DIMMING_LED, Dimming++);
				if(Dimming > 255)
					Dimming = 255;
			}
		}
		if(SystemFlag.ToNight)
		{
			if(SecondCounter % SecondForDimming)
			{
				analogWrite(DIMMING_LED, Dimming--);
				if(Dimming < 0)
					Dimming = 0;
			}			
		}
		OsDelay(500);
	}
}

void TaskTime(void *pvParameters)  // This is a task.
{
	(void) pvParameters;

	for (;;)
	{
		CheckTime();
		OsDelay(10);
	}
}

void TaskIgroSensorPump(void *pvParameters)  // This is a task.
{
	(void) pvParameters;

	for (;;)
	{
		SensorsResponse();
		if(SystemFlag.TurnOnPump)
			digitalWrite(PUMP, HIGH);
		else
			digitalWrite(PUMP, LOW);
			
		OsDelay(50);
	}
}