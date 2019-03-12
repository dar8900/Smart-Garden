#ifndef ETHERNET_CONN_H
#define ETHERNET_CONN_H
#include <Arduino.h>

void EthInit(void);
void CheckCableConnection(void);
void EthServer(void);
#endif