cmake_minimum_required(VERSION 3.23)

# Set project name
project(AVR_PRJ_bare-metal)

# Spec comp AVR-GCC and set opt 
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_C_FLAGS "-mmcu=atmega328p -Os")

# Add sources 
set(SOURCES main.c foo.c bar.c)

# Gen exec file
add_executable(AVR_PRJ_bare-metal ${SOURCES})
