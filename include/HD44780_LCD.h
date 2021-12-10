#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#include "main.h"
#include "DWT_Delay.h"

/*
* Library HD44780_LCD usage:
* In HD44780_LCD.h
* 1. Set display type(16x2, 20x4), by default 16x2
* 2. Uncomment LCD working mode(USE_8_BIT_MODE or USE_4_BIT_MODE, but not both). By default configured for 4 bit mode.
* 3. Set preferred ports(or use defaults) in LCD mode block
* 4. In your main class:
	initLCD(); // by default 2 lines is initialized, cursor is not showing and writing from left to right
	then you start use LCD
*/

//#define USE_8_BIT_MODE		// define LCD working mode
#define USE_4_BIT_MODE

#define LCD_COL_COUNT 16
#define LCD_ROW_COUNT 2

#if !defined(USE_8_BIT_MODE) && !defined(USE_4_BIT_MODE)
#warning "LCD mode is not defined. By default 4 bit mode will be used"
#define USE_4_BIT_MODE
#endif

#if defined(USE_8_BIT_MODE) && defined(USE_4_BIT_MODE)
#error "LCD mode settings is incorrect. Should be set only four(USE_4_BIT_MODE) or eight(USE_8_BIT_MODE) bit mode only"
#endif

/*
 * HD44780 wiring at 8 bit mode
LCD   STM32Fxxx         DESCRIPTION
GND   GND               Ground
VCC   +5V               Power supply for LCD
V0    Potentiometer	    Contrast voltage. Connect to potentiometer
RS    PC0               Register select
RW    GND               Read/write, write only in current implementation
E     PC1               Enable pin
D0    PB0               Data 0
D1    PB1               Data 1
D2    PB2               Data 2
D3    PB3               Data 3
D4    PB4               Data 4
D5    PB5               Data 5
D6    PB6               Data 6
D7    PB7               Data 7
A     +3V3              Back light positive power
K     GND               Ground for back light
*/
#if defined(USE_8_BIT_MODE)			// if not redefined, then default ports will be used for 8 bit mode
#define LCD_DATA_PORT GPIOB
#define LCD_COMMAND_PORT GPIOC

#define LCD_RS LL_GPIO_PIN_0
#define LCD_EN LL_GPIO_PIN_1

#define LCD_D0 LL_GPIO_PIN_0
#define LCD_D1 LL_GPIO_PIN_1
#define LCD_D2 LL_GPIO_PIN_2
#define LCD_D3 LL_GPIO_PIN_3
#define LCD_D4 LL_GPIO_PIN_4
#define LCD_D5 LL_GPIO_PIN_5
#define LCD_D6 LL_GPIO_PIN_6
#define LCD_D7 LL_GPIO_PIN_7
#endif	//USE_8_BIT_MODE

/*
 * HD44780 wiring at 4 bit mode
LCD   STM32Fxxx         DESCRIPTION
GND   GND               Ground
VCC   +5V               Power supply for LCD
V0    Potentiometer	    Contrast voltage. Connect to potentiometer
RS    PB4               Register select
RW    GND               Read/write, write only in current implementation
E     PB5               Enable pin
D0    -                 Data 0 - doesn't care
D1    -                 Data 1 - doesn't care
D2    -                 Data 2 - doesn't care
D3    -                 Data 3 - doesn't care
D4    PB6               Data 4
D5    PB7               Data 5
D6    PB8               Data 6
D7    PB9               Data 7
A     +3V3              Back light positive power
K     GND               Ground for back light
*/
#if defined(USE_4_BIT_MODE)   // if not redefined, then default ports will be used for 4 bit mode
#define LCD_PORT GPIOA

#define LCD_RS LL_GPIO_PIN_9
#define LCD_EN LL_GPIO_PIN_10

#define LCD_D4 LL_GPIO_PIN_0
#define LCD_D5 LL_GPIO_PIN_1
#define LCD_D6 LL_GPIO_PIN_5
#define LCD_D7 LL_GPIO_PIN_6
#endif    //USE_4_BIT_MODE

// Basic commands
#define LCD_CLEAR_DISPLAY 0x01            // Clear the display screen. Execution Time: 1.64ms
#define LCD_SHIFT_CURSOR_RIGHT 0x06        // Shift the cursor right (e.g. data gets written in an incrementing order, left to right)

#define LCD_DISPLAY_ON_CURSOR_OFF 0x0C
#define LCD_DISPLAY_ON_CURSOR_BLINKING 0x0E

#define LCD_MOVE_CURSOR_AT_FIRST_LINE_BEGINNING 0x80    // Force the cursor to the beginning of the 1st line
#define LCD_MOVE_CURSOR_AT_SECOND_LINE_BEGINNING 0xC0    // Force the cursor to the beginning of the 2nd line

#define LCD_SHIFT_POSITION_LEFT 0x10                    // Shift cursor position to the left
#define LCD_SHIFT_POSITION_RIGHT 0x14                    // Shift cursor position to the right
#define LCD_SHIFT_DISPLAY_LEFT 0x18                        // Shift entire display to the left
#define LCD_SHIFT_DISPLAY_RIGHT 0x1C                    // Shift entire display to the right

#define LCD_TWO_LINES_5X8_MATRIX_8BIT_MODE 0x38
#define LCD_TWO_LINES_5X8_MATRIX_4BIT_MODE 0x28
#define LCD_ONE_LINE_8BIT_MODE 0x30
#define LCD_ONE_LINE_4BIT_MODE 0x20

// commands
#define LCD_CLEARDISPLAY   0x01
#define LCD_RETURNHOME     0x02
#define LCD_ENTRYMODESET   0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT    0x10
#define LCD_FUNCTIONSET    0x20
#define LCD_SETCGRAMADDR   0x40
#define LCD_SETDDRAMADDR   0x80

#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON  0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON   0x02
#define LCD_CURSOROFF  0x00
#define LCD_BLINKON    0x01
#define LCD_BLINKOFF   0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE  0x00
#define LCD_MOVERIGHT   0x04
#define LCD_MOVELEFT    0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE    0x08
#define LCD_1LINE    0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS  0x00


void initLCD();
void commandLCD(uint8_t command);

void clearLCD();

void turnOnLCD();
void turnOffLCD();

void returnHomeLCD();
void moveCursorAtSecondLineBeginningLCD();

void enableBlinkingCursorLCD();
void disableBlinkingCursorLCD();
void enableCursorLCD();
void disableCursorLCD();
void moveCursorLeftLCD();
void moveCursorRightLCD();
void setCursorIncrementFromLeftToRightLCD();
void setCursorIncrementFromRightToLeftLCD();

void moveDisplayLeftLCD();
void moveDisplayRightLCD();

void enableAutoScrollLCD();
void disableAutoScrollLCD();

void printLCD(char *string);
void printfLCD(char *format, ...);
void cleanPrintLCD(const char *string);
void cleanPrintfLCD(char *format, ...);
void printLCDChar(unsigned char charData);

void goToXYLCD(uint8_t row, uint8_t pos);
void printAtPositionLCD(uint8_t row, uint8_t pos, char *str);
void printfAtPositionLCD(uint8_t row, uint8_t pos, char *format, ...);
void cleanPrintAtPositionLCD(uint8_t row, uint8_t col, const char *string);
void cleanPrintfAtPositionLCD(uint8_t row, uint8_t col, char *format, ...);

void createCustomCharLCD(uint8_t location, const uint8_t *charmap);
void printCustomCharLCD(uint8_t location);

void initProgressBar(uint8_t len, uint8_t row, uint8_t col);
void incrementProgressBar();
void decrementProgressBar();