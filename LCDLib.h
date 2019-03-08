#ifndef LCDLIB_H
#define LCDLIB_H
#include <Arduino.h>
#include <stdint.h>

#undef LCD16
#define  LCD20

#define LCD_ADDR                0x27

#ifdef LCD16
#define LCD_COL 	              16
#define LCD_ROW                    2
#endif 

#ifdef LCD20
#define LCD_COL 	              20
#define LCD_ROW                    4
#endif

#define MAX_LCD_ROW    (LCD_ROW - 1)
#define MAX_LCD_COL    (LCD_COL - 1)

#define CENTER_ALIGN              25
#define RIGHT_ALIGN      MAX_LCD_COL
#define LEFT_ALIGN                 0
#define AFTER_ARROW_POS			   1

#ifdef LCD16
typedef enum
{
	ONE = 0,
	TWO
} LCD_ROWS;	
#endif

#ifdef LCD20
typedef enum
{
	ONE = 0,
	TWO,
	THREE,
	FOUR
} LCD_ROWS;	
#endif

typedef enum
{
	TO_LEFT = 0,
	TO_RIGHT	
}SCROLL_MOVEMENT;

void LCDInit(void);
void LCDMoveCursor(short row, short col);
void ClearLCD(void);
void BlinkDisplay(short NumTimes);
void LCDPrintString(short row, short col, String string);
void LCDPrintValue(short row, short col, short value);
void LCDPrintChar(short Row, short Col, String Character);
void ClearChar(short Row, short Col);
void ClearLCDLine(short row);
void LCDDisplayLight(bool IsOn);
void LCDBlink(bool IsBlinking);
bool LCDCreateIcon(short Icon[], short IconNum);
void LCDShowIcon(short IconNum, short Row, short Col);
void ScrollText(String Text, short Where, short DelayMs, short ScreenPos);
void LCDPrintMessage(String Message, short Row);
#endif