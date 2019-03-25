#include <SPI.h>
#include <SD.h>
#include <Ethernet.h>
#include "Smart-Garden.h"
#include "SDLog.h"

#define UINT16_T_MAX	65534

#define LOG_FILE_NAME	"IgroValues.txt"


extern EthernetClient GeneralClient;

File LogFile;
File WebPage;
uint16_t LogCnt;

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
		LogCnt++;
		if(LogCnt > UINT16_T_MAX)
			LogCnt = 1;
		DBG("Task ETH_SD-> logging su sd card");
		SystemFlag.SDLogging = true;
		LogFile = SD.open(NameOfFile, FILE_WRITE);
		// if the file is available, write to it:
		if (LogFile) 
		{
			LogFile.print("lOG NUMERO ");
			LogFile.print(String(LogCnt));
			LogFile.println();
			LogFile.close();
		}
		// if the file isn't open, pop up an error:
		else
		{
			DBG("Task ETH_SD-> file non aperto");		
		}
		delay(500);
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


