#ifndef BLUETOOTH_H
#define BLUETOOTH_H
#include <Arduino.h>

#define ARDUINO_MEGA
#undef  ARDUINO_UNO

#ifdef ARDUINO_UNO
#include <SoftwareSerial.h>

#endif

void BTInit(String ModuleName);
void IsBTActive(void);
void WriteResponse(String Response);
String ReadCommand(void);
uint16_t ReadValue(void);

#endif