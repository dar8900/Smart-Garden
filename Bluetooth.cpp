#include "Bluetooth.h"
#include "Smart-Garden.h"

#ifdef ARDUINO_UNO
SoftwareSerial BT(3,2); // pin 3 RX, pin 2 TX
#define Serial2		BT
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

void BTInit(String ModuleName)
{
	DBG("TASK BT: init");

	String ModuleNameTot = "AT+NAME" + ModuleName;
	Serial2.begin(9600);	
	Serial2.write(ModuleNameTot.c_str());
	delay(1500);
	if (Serial2.available())
	{  
		Serial.write(Serial2.read());
	}
	// Serial2.write("AT+PIN2806");
	// delay(1500);	
}

void IsBTActive()
{
	if(StartCntToActive == 0 && digitalRead(BT_LED_ACTIVE))
		StartCntToActive = millis();		
	else
	{
		StartCntToActive = 0;
		SystemFlag.BTActive = false;
	}
	if(StartCntToActive > 0)
	{
		if(digitalRead(BT_LED_ACTIVE))
			BTActive = true;
		else
			BTActive = false;
		if(millis() - StartCntToActive >= 2000 && BTActive)
		{
			StartCntToActive = 0;
			SystemFlag.BTActive = true;
		}
	}
}


void WriteResponse(String Response)
{
	Serial2.println(Response);	
}

String ReadString()
{
	String ReadedCommand = "";
	while(Serial2.available())
	{
		ReadedCommand += Serial2.readString();
	}
	return ReadedCommand;
}

uint16_t ReadValue()
{
	uint16_t ReadedValue = 25;
	while(Serial2.available())
	{
		ReadedValue = (uint16_t)Serial2.read();
	}
	return ReadedValue;	
}
