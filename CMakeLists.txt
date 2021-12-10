cmake_minimum_required(VERSION 3.20)
project(HD44780_LCD C)

set(CMAKE_C_STANDARD 99)

include(cmake/CPM.cmake)

CPMAddPackage(
        NAME DWTDelay
        GITHUB_REPOSITORY ximtech/DWTDelay
        GIT_TAG origin/main)

set(HD44780_LCD_DIRECTORY
        ${DWT_DELAY_DIRECTORY}
        ${HD44780_LCD_SOURCE_DIR}
        ${HD44780_LCD_SOURCE_DIR}/include
        CACHE STRING "HD44780 LCD directories include to the main project" FORCE)

set(HD44780_LCD_SOURCES
        ${DWT_DELAY_SOURCES}
        ${HD44780_LCD_SOURCE_DIR}/${PROJECT_NAME}.c
        ${HD44780_LCD_SOURCE_DIR}/include/${PROJECT_NAME}.h
        CACHE STRING "HD44780 LCD source files include to the main project" FORCE)

set(SOURCE_FILES
        ${PROJECT_NAME}.c
        include/${PROJECT_NAME}.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

target_link_libraries(${PROJECT_NAME} DWT_Delay)
target_include_directories(${PROJECT_NAME} PUBLIC ${HD44780_LCD_DIRECTORY})