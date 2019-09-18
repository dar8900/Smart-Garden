#include "Bluetooth.h"
#include "Smart-Garden.h"

#ifdef ARDUINO_UNO
SoftwareSerial BT(3,2); // pin 3 RX, pin 2 TX
#define Serial2		BT
#endif



#ifdef CONNECTION_METHOD_3
volatile uint32_t InterruptCounter = 1;
uint32_t OldInterruptCounter;
#endif

/*
Lista dei comandi HC-06:
AT              Se la comunicazione funziona il modulo risponde OK
AT+VERSION      Restituisce la versione del firmware
AT+BAUDx        Imposta il Baudrate, al posto di x mettere 1 per 1200 bps, 2=2400, 3=4800, 4=9600, 5=19200, 
				6=38400, 7=57600, 8=115200, 9=230400, A=460800, B=921600, C=1382400
AT+NAMEstring   Al posto di "string" mettere il nome che vuoi dare al modulo (massimo 20 caratteri)
AT+PINxxxx      Imposta il pincode del modulo bluetooth (es.1234)
*/


uint16_t StartCntToActive;
bool BTActive = false;

// bool BTConnected;

bool BTInit()
{
	DBG("TASK BT-> init");
	String ModuleName = "AT+NAMESG-001";
	Serial2.write(ModuleName.c_str());
	delay(1500);
	ModuleName = "";
	while(Serial2.available())
	{  
		ModuleName += char(Serial2.read());
	}
	if(ModuleName != "")
	{
		DBG("Task BT-> nome modulo:");
		DBG(ModuleName);
		return true;
	}
	else
	{
		DBG("Task BT-> scheda BT non presente");
		return false;
	}
	// Serial2.write("AT+PIN2806");
	// delay(1500);	
}

#ifndef CONNECTION_METHOD_3
bool IsDeviceBTConnected()
{
	bool Connected = false;
	
#ifdef CONNECTION_METHOD_1
	uint16_t CntLive = 3000;
	while(!Connected)
	{
		if(StartCntToActive == 0 && digitalRead(BT_LED_ACTIVE))
			StartCntToActive = millis();		

		if(StartCntToActive > 0)
		{
			if(digitalRead(BT_LED_ACTIVE))
				BTActive = true;
			else
			{
				StartCntToActive = 0;
				BTActive = false;
			}
			if((millis() - StartCntToActive) >= 2000 && BTActive && digitalRead(BT_LED_ACTIVE))
			{
				StartCntToActive = 0;
				Connected = true;
			}
		}
		CntLive--;
		if(CntLive == 0 && !Connected)
		{
			break;
		}
		delay(1);
	}
#endif

#ifdef CONNECTION_METHOD_2
	uint16_t CntLive = 5000;
	String Hallo = "";
	WriteResponse("Connettersi a SG-001? Digitare 's'");
	while(!Connected)
	{
		Hallo = ReadString();
		if(Hallo == "s")
		{
			WriteResponse("");
			Connected = true;
		}
		CntLive--;
		if(CntLive == 0 && !Connected)
		{
			break;
		}
		delay(1);
	}
#endif
	return Connected;
}

#else // ifndef CONNECTION_METHOD_3

void IsDeviceBTConnected()
{
	InterruptCounter++;
	if(InterruptCounter >= 4294967290)
	{
		InterruptCounter = 1;
		OldInterruptCounter = 0;
	}
}

#endif // ifndef CONNECTION_METHOD_3



void WriteResponse(String Response)
{
	Serial2.println(Response);	
}

String ReadString()
{
	String StringReaded = "";
	while(Serial2.available())
	{
		StringReaded += char(Serial2.read());
	}
	return StringReaded;
}

uint16_t ReadValue()
{
	String ReadedValue = "";

	while(Serial2.available())
	{
		ReadedValue += char(Serial2.read());
	}
	if(ReadedValue != "")
		return ReadedValue.toInt();
	else 
		return INVALID_BT_HOUR;
}
