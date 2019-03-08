#include <SPI.h>
#include <SD.h>
#include "Smart-Garden.h"
#include "SDLog.h"
#include "IgroSensor.h"
#include "Time.h"

#define CS_PIN 	53
#define LOG_PERIOD_SD		60    // In secondi

#define LOG_FILE_NAME	"IgroValues "

File LogFile;

void SDBegin()
{
	// see if the card is present and can be initialized:
	if (!SD.begin(CS_PIN)) 
	{
		SystemFlag.SDInitialize = false;
	}
	else
		SystemFlag.SDInitialize = true;	
}

void LogToSD()
{
	String NameOfFile = LOG_FILE_NAME;
	if(SystemFlag.SDInitialize)
	{
		if(LogToSDPeriod == LOG_PERIOD_SD)
		{
			SystemFlag.SDLogging = true;
			LogToSDPeriod = 0;
			NameOfFile += String(TimeDate.Day) + "-" + String(TimeDate.Month) + "-" + String(TimeDate.Year);
			NameOfFile += ".CSV";
			LogFile = SD.open(NameOfFile, FILE_WRITE);
			// if the file is available, write to it:
			if (LogFile) 
			{
				LogFile.print("Igrometro;");
				LogFile.print(String(SensorsMeanResponse) + ";Ora;");
				LogFile.print(String(TimeDate.Hour) + ":" + String(TimeDate.Minute));
				LogFile.println();
				LogFile.close();
			}
			// if the file isn't open, pop up an error:
			else
			{
				Serial.println("TASK SD: file non aperto");
			}
			delay(500);
		}
		SystemFlag.SDLogging = false;
	}
}