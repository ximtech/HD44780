# HD44780

**STM32** LL(Low Layer) GPIO library for LCD. Works for both 16x2 and 20x4 etc.

### Features

- Small code footprint
- Supports progress bar
- Display printf function
- No HAL dependencies

### LCD 16x2

<img src="https://github.com/ximtech/HD44780/blob/main/example/LCD16x2.PNG" alt="image" width="300"/>

### LCD 20x4

<img src="https://github.com/ximtech/HD44780/blob/main/example/LCD20x4.PNG" alt="image" width="300"/>

### Add as CPM project dependency

How to add CPM to the project, check the [link](https://github.com/cpm-cmake/CPM.cmake)

```cmake
CPMAddPackage(
        NAME HD44780
        GITHUB_REPOSITORY ximtech/HD44780
        GIT_TAG origin/main)
```

### Project configuration

1. Start project with STM32CubeMX:
2. Set output pins
3. Select: Project Manager -> Advanced Settings -> GPIO -> LL
4. Generate Code
5. Add sources to project:

```cmake
include_directories(${includes}
        ${HD44780_LCD_DIRECTORY})   # source directories

file(GLOB_RECURSE SOURCES ${sources}
        ${HD44780_LCD_SOURCES})    # source files
```

3. Then Build -> Clean -> Rebuild Project

## Wiring

<img src="https://github.com/ximtech/HD44780/blob/main/example/wiring.PNG" alt="image" width="300"/>

## Usage

In `HD44780_LCD.h` default defines. Override them in `main.h` if needed

```c
#define HD44780_LCD_COL_COUNT 20
#define HD44780_LCD_ROW_COUNT 4

//#define HD44780_USE_8_BIT_MODE    // define LCD working mode, choose 4 or 8 bit mode, but not both
#define HD44780_USE_4_BIT_MODE

#define HD44780_USE_CUSTOM_PORT // override values in main.h declaring custom define ports 
#define LCD_PORT GPIOA  // Pin and port for 4 bit mode, for 8 bit mode see declarations in HD44780_LCD.h

#define LCD_RS LL_GPIO_PIN_9
#define LCD_EN LL_GPIO_PIN_10

#define LCD_D4 LL_GPIO_PIN_0
#define LCD_D5 LL_GPIO_PIN_1
#define LCD_D6 LL_GPIO_PIN_5
#define LCD_D7 LL_GPIO_PIN_6
```

- [Base example](https://github.com/ximtech/HD44780/blob/main/example/example_basics.c)
- [Progress bar](https://github.com/ximtech/HD44780/blob/main/example/example_progress_bar.c)