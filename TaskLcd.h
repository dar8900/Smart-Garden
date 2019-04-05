#ifndef TASK_LCD_H
#define TASK_LCD_H

#include <stdint.h>

void TaskLCD(void);
void TaskLcdInitVar(void);
void WaitForOk(uint8_t Row);
#endif