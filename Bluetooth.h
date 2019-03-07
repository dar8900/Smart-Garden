#ifndef BLUETOOTH_H
#define BLUETOOTH_H
#include <Arduino.h>

#define ARDUINO_MEGA
#undef  ARDUINO_UNO

#ifdef ARDUINO_UNO
#include <SoftwareSerial.h>

#endif

void IsBTActive(void);

#endif