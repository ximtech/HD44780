#include "HD44780_LCD.h"

static uint32_t dataPins[8];
static uint8_t displayControlParameters;
static char formatBuffer[HD44780_LCD_COL_COUNT + 1];

static const uint8_t loadBarStartElement[8] = {0b10000, 0b11000, 0b11100, 0b11110, 0b11110, 0b11100, 0b11000, 0b10000};
static const uint8_t loadBarProgressElement[8] = {0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111};
static const uint8_t loadBarEndElement[8] = {0b00001, 0b00011, 0b00111, 0b01111, 0b01111, 0b00111, 0b00011, 0b00001};

uint8_t barRowLocation = 0;
uint8_t barColLocation = 0;
uint8_t barTotalLength = 0;
uint8_t barProgress = 0;

#ifdef HD44780_USE_8_BIT_MODE

void initLCDinEightBitMode() {
    dwtDelayInit();
    delay_ms(20);        // LCD Power ON delay always >15ms

    dataPins[0] = LCD_D0;
    dataPins[1] = LCD_D1;
    dataPins[2] = LCD_D2;
    dataPins[3] = LCD_D3;
    dataPins[4] = LCD_D4;
    dataPins[5] = LCD_D5;
    dataPins[6] = LCD_D6;
    dataPins[7] = LCD_D7;

    commandLCD(LCD_TWO_LINES_5X8_MATRIX_8BIT_MODE);
}

void pulseEnable() {
    LL_GPIO_SetOutputPin(LCD_COMMAND_PORT, LCD_EN);
    delay_us(1);
    LL_GPIO_ResetOutputPin(LCD_COMMAND_PORT, LCD_EN);
    delay_ms(2);
}

void writeToPort(uint8_t port, uint8_t value) {
    if (value) {
        LL_GPIO_SetOutputPin(LCD_DATA_PORT, port);
    } else {
        LL_GPIO_ResetOutputPin(LCD_DATA_PORT, port);
    }
}

void writeEightBits(uint8_t value) {
    for (uint8_t i = 0; i < 8; i++) {
        writeToPort(dataPins[i], (value >> i) & 0x01);
    }
}

void commandLCD(uint8_t command) {
    writeEightBits(command);
    LL_GPIO_ResetOutputPin(LCD_COMMAND_PORT, LCD_RS);   // RS=0 command reg.
    pulseEnable();
}

void printLCDChar(unsigned char charData) {
    writeEightBits(charData);
    LL_GPIO_SetOutputPin(LCD_COMMAND_PORT, LCD_RS);     // RS=1 data reg.
    pulseEnable();
}
#endif    //USE_8_BIT_MODE

//-----------------------------------------------------------------------------

#ifdef HD44780_USE_4_BIT_MODE

void initLCDinFourBitMode() {
    dwtDelayInit();
    delay_ms(20);        // LCD Power ON delay always >15ms

    dataPins[0] = LCD_D4;
    dataPins[1] = LCD_D5;
    dataPins[2] = LCD_D6;
    dataPins[3] = LCD_D7;

    commandLCD(0x33);
    commandLCD(0x32);    // Send for 4 bit initialization of LCD
    commandLCD(LCD_TWO_LINES_5X8_MATRIX_4BIT_MODE);
}

void writeToPort(uint8_t port, uint8_t value) {
    if (value) {
        LL_GPIO_SetOutputPin(LCD_PORT, port);
    } else {
        LL_GPIO_ResetOutputPin(LCD_PORT, port);
    }
}

void writeFourBits(uint8_t nibble) {
    for (uint8_t i = 0; i < 4; i++) {
        writeToPort(dataPins[i], (nibble >> i) & 0x01);
    }
}

void pulseEnableUpper() {
    LL_GPIO_SetOutputPin(LCD_PORT, LCD_EN); // Enable pulse
    delay_ms(1);
    LL_GPIO_ResetOutputPin(LCD_PORT, LCD_EN);
    delay_us(200);
}

void pulseEnableLower() {
    LL_GPIO_SetOutputPin(LCD_PORT, LCD_EN); // Enable pulse
    delay_us(1);
    LL_GPIO_ResetOutputPin(LCD_PORT, LCD_EN);
    delay_ms(2);
}

void commandLCD(uint8_t command) {
    writeFourBits(command >> 4);    // sending upper nibble
    LL_GPIO_ResetOutputPin(LCD_PORT, LCD_RS);    // RS=0, command reg.
    pulseEnableUpper();

    writeFourBits(command);    // sending lower nibble
    pulseEnableLower();
}

void printLCDChar(unsigned char charData) {
    writeFourBits(charData >> 4);    // sending upper nibble

    LL_GPIO_SetOutputPin(LCD_PORT, LCD_RS);    // RS=1, data reg.
    pulseEnableUpper();

    writeFourBits(charData);
    pulseEnableLower();
}
#endif    //USE_4_BIT_MODE


void initLCD() {
#ifdef HD44780_USE_8_BIT_MODE            // Configure lcd for 8 bit mode
    initLCDinEightBitMode();
#elif defined(HD44780_USE_4_BIT_MODE)    // Configure lcd for 4 bit mode
    initLCDinFourBitMode();
#endif

    commandLCD(LCD_DISPLAY_ON_CURSOR_OFF);
    commandLCD(LCD_SHIFT_CURSOR_RIGHT);        // Auto Increment cursor from left to right
    clearLCD();
}

void clearLCD() {
    commandLCD(LCD_CLEAR_DISPLAY);
    delay_ms(2);        // Clear display command delay > 1.63 ms
    returnHomeLCD();
}

void turnOnLCD() {
    displayControlParameters |= LCD_DISPLAYON;
    commandLCD(LCD_DISPLAYCONTROL | displayControlParameters);
}

void turnOffLCD() {
    displayControlParameters &= ~LCD_DISPLAYON;
    commandLCD(LCD_DISPLAYCONTROL | displayControlParameters);
}

void returnHomeLCD() {
    commandLCD(LCD_MOVE_CURSOR_AT_FIRST_LINE_BEGINNING);
}

void enableBlinkingCursorLCD() {
    displayControlParameters |= LCD_DISPLAY_ON_CURSOR_BLINKING;
    commandLCD(LCD_DISPLAYCONTROL | displayControlParameters);
}

void disableBlinkingCursorLCD() {
    commandLCD(LCD_DISPLAY_ON_CURSOR_OFF);
}

void enableCursorLCD() {
    displayControlParameters |= LCD_CURSORON;
    commandLCD(LCD_DISPLAYCONTROL | displayControlParameters);
}

void disableCursorLCD() {
    displayControlParameters &= ~LCD_CURSORON;
    commandLCD(LCD_DISPLAYCONTROL | displayControlParameters);
}

void moveCursorLeftLCD() {
    commandLCD(LCD_SHIFT_POSITION_LEFT);
}

void moveCursorRightLCD() {
    commandLCD(LCD_SHIFT_POSITION_LEFT);
}

void moveCursorAtSecondLineBeginningLCD() {
    commandLCD(LCD_MOVE_CURSOR_AT_SECOND_LINE_BEGINNING);
}

void moveDisplayLeftLCD() {
    commandLCD(LCD_SHIFT_DISPLAY_LEFT);
}

void moveDisplayRightLCD() {
    commandLCD(LCD_SHIFT_DISPLAY_RIGHT);
}

void setCursorIncrementFromLeftToRightLCD() {
    displayControlParameters |= LCD_ENTRYLEFT;
    commandLCD(LCD_ENTRYMODESET | displayControlParameters);
}

void setCursorIncrementFromRightToLeftLCD() {
    displayControlParameters &= ~LCD_ENTRYLEFT;
    commandLCD(LCD_ENTRYMODESET | displayControlParameters);
}

void enableAutoScrollLCD() {
    displayControlParameters |= LCD_ENTRYSHIFTINCREMENT;
    commandLCD(LCD_ENTRYMODESET | displayControlParameters);
}

void disableAutoScrollLCD() {
    displayControlParameters &= ~LCD_ENTRYSHIFTINCREMENT;
    commandLCD(LCD_ENTRYMODESET | displayControlParameters);
}

void goToXYLCD(uint8_t row, uint8_t pos) {
    static const uint8_t rowOffsets[] = {0x00, 0x40, 0x14, 0x54};

    if (row < HD44780_LCD_ROW_COUNT && pos < HD44780_LCD_COL_COUNT) {
        commandLCD(LCD_SETDDRAMADDR | (pos + rowOffsets[row]));
    }
}

void printLCD(char *string) {
    for (uint8_t i = 0; string[i] != 0; i++) {
        printLCDChar(string[i]);
    }
}

void printAtPositionLCD(uint8_t row, uint8_t pos, char *str) {            // Send string to LCD with xy position
    goToXYLCD(row, pos);
    printLCD(str);
}

void printfAtPositionLCD(uint8_t row, uint8_t pos, char *format, ...) {
    va_list args;

    va_start(args, format);
    vsnprintf(formatBuffer, HD44780_LCD_COL_COUNT + 1, format, args);
    va_end(args);

    goToXYLCD(row, pos);
    printLCD(formatBuffer);
}

void cleanPrintAtPositionLCD(uint8_t row, uint8_t col, const char *string) {
    char buffer[HD44780_LCD_COL_COUNT] = {[0 ... HD44780_LCD_COL_COUNT - 1] = ' '};
    buffer[HD44780_LCD_COL_COUNT - 1] = '\0';
    for (uint8_t i = col, j = 0; i < HD44780_LCD_COL_COUNT && string[j] != '\0'; i++, j++) {
        buffer[i] = string[j];
    }
    goToXYLCD(row, 0);
    printLCD(buffer);
}

void cleanPrintfAtPositionLCD(uint8_t row, uint8_t col, char *format, ...) {
    va_list args;

    va_start(args, format);
    vsnprintf(formatBuffer, HD44780_LCD_COL_COUNT + 1, format, args);
    va_end(args);

    cleanPrintAtPositionLCD(row, col, formatBuffer);
}

void createCustomCharLCD(uint8_t location, const uint8_t *charmap) {    // location from 0 to 7
    if (location < 8) {
        commandLCD(LCD_SETCGRAMADDR + (location * 8));// Command 0x40 and onwards forces the device to point CGRAM address
        for (uint8_t i = 0; i < 8; i++) {
            printLCDChar(charmap[i]);
        }
        returnHomeLCD();
    }
}

void printCustomCharLCD(uint8_t location) {    // location from 0 to 7
    if (location < 8) {
        printLCDChar(location);
    }
}

void printfLCD(char *format, ...) {
    va_list args;

    va_start(args, format);
    vsnprintf(formatBuffer, HD44780_LCD_COL_COUNT + 1, format, args);
    va_end(args);

    printLCD(formatBuffer);
}

void cleanPrintLCD(const char *string) {
    char buffer[HD44780_LCD_COL_COUNT] = {[0 ... HD44780_LCD_COL_COUNT - 1] = ' '};
    buffer[HD44780_LCD_COL_COUNT - 1] = '\0';
    for (uint8_t i = 0; i < HD44780_LCD_COL_COUNT && string[i] != '\0'; i++) {
        buffer[i] = string[i];
    }
    printLCD(buffer);
}

void cleanPrintfLCD(char *format, ...) {
    va_list args;

    va_start(args, format);
    vsnprintf(formatBuffer, HD44780_LCD_COL_COUNT + 1, format, args);
    va_end(args);

    cleanPrintLCD(formatBuffer);
}

void initProgressBar(uint8_t len, uint8_t row, uint8_t col) {
    bool isBarLengthValid = (len <= HD44780_LCD_COL_COUNT - 1) && ((len - 2) >= 0);
    bool isRowValid = row < HD44780_LCD_ROW_COUNT;

    if (isBarLengthValid && isRowValid) {
        createCustomCharLCD(0, loadBarStartElement);
        createCustomCharLCD(1, loadBarProgressElement);
        createCustomCharLCD(2, loadBarEndElement);

        barRowLocation = row;
        barColLocation = col + 1;
        barTotalLength = len - 1;

        goToXYLCD(row, col);
        printCustomCharLCD(0);

        goToXYLCD(row, (col + len));
        printCustomCharLCD(2);
    }
}

void incrementProgressBar() {
    if (barTotalLength > 0 && barProgress < barTotalLength) {
        goToXYLCD(barRowLocation, barColLocation++);
        printCustomCharLCD(1);
        barProgress++;
    }
}

void decrementProgressBar() {
    if (barTotalLength > 0 && barProgress > 0) {
        goToXYLCD(barRowLocation, barProgress--);
        printLCD(" ");
        barColLocation--;
    }
}
