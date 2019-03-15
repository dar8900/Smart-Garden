#include <SPI.h>
#include <SD.h>
#include <Ethernet.h>
#include "Smart-Garden.h"
#include "SDLog.h"
#include "IgroSensor.h"
#include "Time.h"


#define LOG_FILE_NAME	"IgroValues "


extern EthernetClient GeneralClient;

File LogFile;
File WebPage;

void SDInit()
{
	// see if the card is present and can be initialized:
	if (!SD.begin(SD_CS)) 
	{
		SystemFlag.SDInitialize = false;
		DBG("Task ETH_SD-> SD non inizializzata");
	}
	else
	{
		DBG("Task ETH_SD-> SD inizializzata");
		SystemFlag.SDInitialize = true;	
		if (!SD.exists("index.htm"))
			SystemFlag.SDWebPagePresent = false;
		else
			SystemFlag.SDWebPagePresent = true;
		delay(1000);
	}
}

void LogToSD()
{
	String NameOfFile = LOG_FILE_NAME;
	if(SystemFlag.SDInitialize)
	{
		if(LogToSDPeriod == LOG_PERIOD_SD)
		{
			DBG("Task ETH_SD-> logging su sd card");
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
				DBG("Task ETH_SD-> file non aperto");		
			}
			delay(500);
		}
		SystemFlag.SDLogging = false;
	}
}


void WriteWebPageToClient()
{
	WebPage = SD.open("index.htm");
	if(WebPage)
	{
		while(WebPage.available())
		{
			GeneralClient.write(WebPage.read());
		}
		WebPage.close();
	}
}


