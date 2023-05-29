cmake_minimum_required(VERSION 3.23)

# Set project name
project(AVR_PRJ_bare-metal  C CXX)

# Setează setările pentru compilatorul C/C++
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


set(BUILD_DIR ${CMAKE_BINARY_DIR}/build)

file(MAKE_DIRECTORY ${BUILD_DIR})

# Add sources 
add_executable(${PROJECT_NAME}
     ${CMAKE_CURRENT_SOURCE_DIR}/main.c
     ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/uart.c
     ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/uart.h
    # Adaugă toate celelalte fișiere sursă aici
)

# Spec comp AVR-GCC and set opt 
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_C_FLAGS "-mmcu=atmega328p -Os")

# Setează opțiunile de compilare pentru Arduino Nano
target_compile_options(${PROJECT_NAME} PRIVATE
    -mmcu=atmega328p
    -DF_CPU=16000000L
    # Alte opțiuni de compilare specifice
)


# Setează output dir 
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR}
)

# Generate Makefile
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Generate .hex file
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND avr-objcopy -O ihex
    ${BUILD_DIR}/${PROJECT_NAME} ${BUILD_DIR}/${PROJECT_NAME}.hex
    COMMENT "Generating .hex file"
)
