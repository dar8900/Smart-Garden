#ifndef BLUETOOTH_H
#define BLUETOOTH_H
#include <Arduino.h>

#define ARDUINO_MEGA
#undef  ARDUINO_UNO

#ifdef ARDUINO_UNO
#include <SoftwareSerial.h>

#endif

bool BTInit(void);
void IsBTConnected(void);
void WriteResponse(String Response);
String ReadString(void);
uint16_t ReadValue(void);

#endif