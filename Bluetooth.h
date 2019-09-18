#ifndef BLUETOOTH_H
#define BLUETOOTH_H
#include <Arduino.h>

#define ARDUINO_MEGA
#undef  ARDUINO_UNO

#ifdef ARDUINO_UNO
#include <SoftwareSerial.h>

#endif

#undef CONNECTION_METHOD_1

#undef CONNECTION_METHOD_2

#define CONNECTION_METHOD_3

#define INVALID_BT_HOUR	200

bool BTInit(void);

#ifndef CONNECTION_METHOD_3
bool IsDeviceBTConnected(void);
#else
void IsDeviceBTConnected(void);
#endif

void WriteResponse(String Response);
String ReadString(void);
uint16_t ReadValue(void);

#endif